// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.IO;
using System.Net.Security;
using System.Net.Test.Common;
using System.Security.Authentication.ExtendedProtection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using Xunit;
using Xunit.Abstractions;
using System.Net.Sockets;
using System.Security.Cryptography.X509Certificates;

namespace System.Net.Http.Functional.Tests
{
    // Note:  Disposing the HttpClient object automatically disposes the handler within. So, it is not necessary
    // to separately Dispose (or have a 'using' statement) for the handler.
    public class HttpClientHandlerTest
    {
        readonly ITestOutputHelper _output;
        private const string ExpectedContent = "Test contest";
        private const string Username = "testuser";
        private const string Password = "password";

        private readonly NetworkCredential _credential = new NetworkCredential(Username, Password);

        public readonly static object[][] EchoServers = HttpTestServers.EchoServers;
        public readonly static object[][] VerifyUploadServers = HttpTestServers.VerifyUploadServers;
        public readonly static object[][] CompressedServers = HttpTestServers.CompressedServers;
        public readonly static object[][] HeaderValueAndUris = {
            new object[] { "X-CustomHeader", "x-value", HttpTestServers.RemoteEchoServer },
            new object[] { "X-Cust-Header-NoValue", "" , HttpTestServers.RemoteEchoServer },
            new object[] { "X-CustomHeader", "x-value", HttpTestServers.RedirectUriForDestinationUri(
                secure:false,
                statusCode:302,
                destinationUri:HttpTestServers.RemoteEchoServer,
                hops:1) },
            new object[] { "X-Cust-Header-NoValue", "" , HttpTestServers.RedirectUriForDestinationUri(
                secure:false,
                statusCode:302,
                destinationUri:HttpTestServers.RemoteEchoServer,
                hops:1) },
        };
        public readonly static object[][] Http2Servers = HttpTestServers.Http2Servers;

        public readonly static object[][] RedirectStatusCodes = {
            new object[] { 300 },
            new object[] { 301 },
            new object[] { 302 },
            new object[] { 303 },
            new object[] { 307 }
        };

        // Standard HTTP methods defined in RFC7231: http://tools.ietf.org/html/rfc7231#section-4.3
        //     "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE"
        public readonly static IEnumerable<object[]> HttpMethods =
            GetMethods("GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE", "CUSTOM1");
        public readonly static IEnumerable<object[]> HttpMethodsThatAllowContent =
            GetMethods("GET", "POST", "PUT", "DELETE", "CUSTOM1");

        private static IEnumerable<object[]> GetMethods(params string[] methods)
        {
            foreach (string method in methods)
            {
                foreach (bool secure in new[] { true, false })
                {
                    yield return new object[] { method, secure };
                }
            }
        }

        public HttpClientHandlerTest(ITestOutputHelper output)
        {
            _output = output;
        }

        [Fact]
        public void Ctor_ExpectedDefaultPropertyValues()
        {
            using (var handler = new HttpClientHandler())
            {
                // Same as .NET Framework (Desktop).
                Assert.True(handler.AllowAutoRedirect);
                Assert.Equal(ClientCertificateOption.Manual, handler.ClientCertificateOptions);
                CookieContainer cookies = handler.CookieContainer;
                Assert.NotNull(cookies);
                Assert.Equal(0, cookies.Count);
                Assert.Null(handler.Credentials);
                Assert.Equal(50, handler.MaxAutomaticRedirections);
                Assert.False(handler.PreAuthenticate);
                Assert.Equal(null, handler.Proxy);
                Assert.True(handler.SupportsAutomaticDecompression);
                Assert.True(handler.SupportsProxy);
                Assert.True(handler.SupportsRedirectConfiguration);
                Assert.True(handler.UseCookies);
                Assert.False(handler.UseDefaultCredentials);
                Assert.True(handler.UseProxy);
                
                // Changes from .NET Framework (Desktop).
                Assert.Equal(DecompressionMethods.GZip | DecompressionMethods.Deflate, handler.AutomaticDecompression);
                Assert.Equal(0, handler.MaxRequestContentBufferSize);
                Assert.NotNull(handler.Properties);
            }
        }

        [Fact]
        public void MaxRequestContentBufferSize_Get_ReturnsZero()
        {
            using (var handler = new HttpClientHandler())
            {
                Assert.Equal(0, handler.MaxRequestContentBufferSize);
            }
        }

        [Fact]
        public void MaxRequestContentBufferSize_Set_ThrowsPlatformNotSupportedException()
        {
            using (var handler = new HttpClientHandler())
            {
                Assert.Throws<PlatformNotSupportedException>(() => handler.MaxRequestContentBufferSize = 1024);
            }
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public async Task UseDefaultCredentials_SetToFalseAndServerNeedsAuth_StatusCodeUnauthorized(bool useProxy)
        {
            var handler = new HttpClientHandler();
            handler.UseProxy = useProxy;
            handler.UseDefaultCredentials = false;
            using (var client = new HttpClient(handler))
            {
                Uri uri = HttpTestServers.NegotiateAuthUriForDefaultCreds(secure:false);
                _output.WriteLine("Uri: {0}", uri);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.Unauthorized, response.StatusCode);
                }
            }
        }

        [Fact]
        public void Properties_Get_CountIsZero()
        {
            var handler = new HttpClientHandler();
            IDictionary<String, object> dict = handler.Properties;
            Assert.Same(dict, handler.Properties);
            Assert.Equal(0, dict.Count);
        }

        [Fact]
        public void Properties_AddItemToDictionary_ItemPresent()
        {
            var handler = new HttpClientHandler();
            IDictionary<String, object> dict = handler.Properties;

            var item = new Object();
            dict.Add("item", item);

            object value;
            Assert.True(dict.TryGetValue("item", out value));
            Assert.Equal(item, value);
        }

        [Theory, MemberData(nameof(EchoServers))]
        public async Task SendAsync_SimpleGet_Success(Uri remoteServer)
        {
            using (var client = new HttpClient())
            {
                using (HttpResponseMessage response = await client.GetAsync(remoteServer))
                {
                    string responseContent = await response.Content.ReadAsStringAsync();
                    _output.WriteLine(responseContent);                    
                    TestHelper.VerifyResponseBody(
                        responseContent,
                        response.Content.Headers.ContentMD5,
                        false,
                        null);                    
                }
            }
        }

        [Theory]
        [MemberData(nameof(GetAsync_IPBasedUri_Success_MemberData))]
        public async Task GetAsync_IPBasedUri_Success(IPAddress address)
        {
            using (var client = new HttpClient())
            {
                var options = new LoopbackServer.Options { Address = address };
                await LoopbackServer.CreateServerAsync(async (server, url) =>
                {
                    await TestHelper.WhenAllCompletedOrAnyFailed(
                        LoopbackServer.ReadRequestAndSendResponseAsync(server, options: options),
                        client.GetAsync(url));
                }, options);
            }
        }

        public static IEnumerable<object[]> GetAsync_IPBasedUri_Success_MemberData()
        {
            foreach (var addr in new[] { IPAddress.Loopback, IPAddress.IPv6Loopback, LoopbackServer.GetIPv6LinkLocalAddress() })
            {
                if (addr != null)
                {
                    yield return new object[] { addr };
                }
            }
        }

        [Fact]
        public async Task SendAsync_MultipleRequestsReusingSameClient_Success()
        {
            using (var client = new HttpClient())
            {
                HttpResponseMessage response;
                for (int i = 0; i < 3; i++)
                {
                    response = await client.GetAsync(HttpTestServers.RemoteEchoServer);
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    response.Dispose();
                }
            }
        }
        
        [Fact]
        public async Task GetAsync_ResponseContentAfterClientAndHandlerDispose_Success()
        {
            HttpResponseMessage response = null;
            using (var handler = new HttpClientHandler())
            using (var client = new HttpClient(handler))
            {
                response = await client.GetAsync(HttpTestServers.SecureRemoteEchoServer);
            }
            Assert.NotNull(response);
            string responseContent = await response.Content.ReadAsStringAsync();
            _output.WriteLine(responseContent);                    
            TestHelper.VerifyResponseBody(
                responseContent,
                response.Content.Headers.ContentMD5,
                false,
                null);                    
        }

        [Fact]
        public async Task SendAsync_Cancel_CancellationTokenPropagates()
        {
            var cts = new CancellationTokenSource();
            cts.Cancel();
            using (var client = new HttpClient())
            {
                var request = new HttpRequestMessage(HttpMethod.Post, HttpTestServers.RemoteEchoServer);
                TaskCanceledException ex = await Assert.ThrowsAsync<TaskCanceledException>(() =>
                    client.SendAsync(request, cts.Token));
                Assert.True(cts.Token.IsCancellationRequested, "cts token IsCancellationRequested");
                Assert.True(ex.CancellationToken.IsCancellationRequested, "exception token IsCancellationRequested");
            }
        }

        [Theory, MemberData(nameof(CompressedServers))]
        public async Task GetAsync_DefaultAutomaticDecompression_ContentDecompressed(Uri server)
        {
            using (var client = new HttpClient())
            {
                using (HttpResponseMessage response = await client.GetAsync(server))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    string responseContent = await response.Content.ReadAsStringAsync();
                    _output.WriteLine(responseContent);
                    TestHelper.VerifyResponseBody(
                        responseContent,
                        response.Content.Headers.ContentMD5,
                        false,
                        null);
                }
            }
        }

        [Theory, MemberData(nameof(CompressedServers))]
        public async Task GetAsync_DefaultAutomaticDecompression_HeadersRemoved(Uri server)
        {
            using (var client = new HttpClient())
            using (HttpResponseMessage response = await client.GetAsync(server, HttpCompletionOption.ResponseHeadersRead))
            {
                Assert.Equal(HttpStatusCode.OK, response.StatusCode);

                Assert.False(response.Content.Headers.Contains("Content-Encoding"), "Content-Encoding unexpectedly found");
                Assert.False(response.Content.Headers.Contains("Content-Length"), "Content-Length unexpectedly found");
            }
        }

        [Fact]
        public async Task GetAsync_ServerNeedsBasicAuthAndSetDefaultCredentials_StatusCodeUnauthorized()
        {
            var handler = new HttpClientHandler();
            handler.Credentials = CredentialCache.DefaultCredentials;
            using (var client = new HttpClient(handler))
            {
                Uri uri = HttpTestServers.BasicAuthUriForCreds(secure:false, userName:Username, password:Password);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.Unauthorized, response.StatusCode);
                }
            }
        }

        [OuterLoop] // TODO: Issue #11345
        [Fact]
        public async Task GetAsync_ServerNeedsAuthAndSetCredential_StatusCodeOK()
        {
            var handler = new HttpClientHandler();
            handler.Credentials = _credential;
            using (var client = new HttpClient(handler))
            {
                Uri uri = HttpTestServers.BasicAuthUriForCreds(secure:false, userName:Username, password:Password);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                }
            }
        }

        [Fact]
        public async Task GetAsync_ServerNeedsAuthAndNoCredential_StatusCodeUnauthorized()
        {
            using (var client = new HttpClient())
            {
                Uri uri = HttpTestServers.BasicAuthUriForCreds(secure:false, userName:Username, password:Password);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.Unauthorized, response.StatusCode);
                }
            }
        }

        [Theory]
        [InlineData("WWW-Authenticate: CustomAuth\r\n")]
        [InlineData("")] // RFC7235 requires servers to send this header with 401 but some servers don't.
        public async Task GetAsync_ServerNeedsNonStandardAuthAndSetCredential_StatusCodeUnauthorized(string authHeaders)
        {
            string responseHeaders =
                $"HTTP/1.1 401 Unauthorized\r\nDate: {DateTimeOffset.UtcNow:R}\r\n{authHeaders}Content-Length: 0\r\n\r\n";
            _output.WriteLine(responseHeaders);
            await LoopbackServer.CreateServerAsync(async (server, url) =>
            {
                var handler = new HttpClientHandler();
                handler.Credentials = new NetworkCredential("unused", "unused");
                using (var client = new HttpClient(handler))
                {
                    Task<HttpResponseMessage> getResponse = client.GetAsync(url);

                    await LoopbackServer.ReadRequestAndSendResponseAsync(server, responseHeaders);

                    using (HttpResponseMessage response = await getResponse)
                    {
                        Assert.Equal(HttpStatusCode.Unauthorized, response.StatusCode);
                    }
                }
            });
        }

        [Theory, MemberData(nameof(RedirectStatusCodes))]
        public async Task GetAsync_AllowAutoRedirectFalse_RedirectFromHttpToHttp_StatusCodeRedirect(int statusCode)
        {
            var handler = new HttpClientHandler();
            handler.AllowAutoRedirect = false;
            using (var client = new HttpClient(handler))
            {
                Uri uri = HttpTestServers.RedirectUriForDestinationUri(
                    secure:false,
                    statusCode:statusCode,
                    destinationUri:HttpTestServers.RemoteEchoServer,
                    hops:1);
                _output.WriteLine("Uri: {0}", uri);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(statusCode, (int)response.StatusCode);
                    Assert.Equal(uri, response.RequestMessage.RequestUri);
                }
            }
        }

        [Theory, MemberData(nameof(RedirectStatusCodes))]
        public async Task GetAsync_AllowAutoRedirectTrue_RedirectFromHttpToHttp_StatusCodeOK(int statusCode)
        {
            var handler = new HttpClientHandler();
            handler.AllowAutoRedirect = true;
            using (var client = new HttpClient(handler))
            {
                Uri uri = HttpTestServers.RedirectUriForDestinationUri(
                    secure:false,
                    statusCode:statusCode,
                    destinationUri:HttpTestServers.RemoteEchoServer,
                    hops:1);
                _output.WriteLine("Uri: {0}", uri);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    Assert.Equal(HttpTestServers.RemoteEchoServer, response.RequestMessage.RequestUri);
                }
            }
        }

        [Fact]
        public async Task GetAsync_AllowAutoRedirectTrue_RedirectFromHttpToHttps_StatusCodeOK()
        {
            var handler = new HttpClientHandler();
            handler.AllowAutoRedirect = true;
            using (var client = new HttpClient(handler))
            {
                Uri uri = HttpTestServers.RedirectUriForDestinationUri(
                    secure:false,
                    statusCode:302,
                    destinationUri:HttpTestServers.SecureRemoteEchoServer,
                    hops:1);
                _output.WriteLine("Uri: {0}", uri);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    Assert.Equal(HttpTestServers.SecureRemoteEchoServer, response.RequestMessage.RequestUri);
                }
            }
        }

        [Fact]
        public async Task GetAsync_AllowAutoRedirectTrue_RedirectFromHttpsToHttp_StatusCodeRedirect()
        {
            var handler = new HttpClientHandler();
            handler.AllowAutoRedirect = true;
            using (var client = new HttpClient(handler))
            {
                Uri uri = HttpTestServers.RedirectUriForDestinationUri(
                    secure:true,
                    statusCode:302,
                    destinationUri:HttpTestServers.RemoteEchoServer,
                    hops:1);
                _output.WriteLine("Uri: {0}", uri);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.Redirect, response.StatusCode);
                    Assert.Equal(uri, response.RequestMessage.RequestUri);
                }
            }
        }

        [Fact]
        public async Task GetAsync_AllowAutoRedirectTrue_RedirectToUriWithParams_RequestMsgUriSet()
        {
            var handler = new HttpClientHandler();
            handler.AllowAutoRedirect = true;
            Uri targetUri = HttpTestServers.BasicAuthUriForCreds(secure:false, userName:Username, password:Password);
            using (var client = new HttpClient(handler))
            {
                Uri uri = HttpTestServers.RedirectUriForDestinationUri(
                    secure:false,
                    statusCode:302,
                    destinationUri:targetUri,
                    hops:1);
                _output.WriteLine("Uri: {0}", uri);
                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.Unauthorized, response.StatusCode);
                    Assert.Equal(targetUri, response.RequestMessage.RequestUri);
                }
            }
        }

        [ActiveIssue(8945, PlatformID.Windows)]
        [Theory]
        [InlineData(3, 2)]
        [InlineData(3, 3)]
        [InlineData(3, 4)]
        public async Task GetAsync_MaxAutomaticRedirectionsNServerHops_ThrowsIfTooMany(int maxHops, int hops)
        {
            using (var client = new HttpClient(new HttpClientHandler() { MaxAutomaticRedirections = maxHops }))
            {
                Task<HttpResponseMessage> t = client.GetAsync(HttpTestServers.RedirectUriForDestinationUri(
                    secure: false,
                    statusCode: 302,
                    destinationUri: HttpTestServers.RemoteEchoServer,
                    hops: hops));

                if (hops <= maxHops)
                {
                    using (HttpResponseMessage response = await t)
                    {
                        Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                        Assert.Equal(HttpTestServers.RemoteEchoServer, response.RequestMessage.RequestUri);
                    }
                }
                else
                {
                    await Assert.ThrowsAsync<HttpRequestException>(() => t);
                }
            }
        }

        [Fact]
        public async Task GetAsync_AllowAutoRedirectTrue_RedirectWithRelativeLocation()
        {
            using (var client = new HttpClient(new HttpClientHandler() { AllowAutoRedirect = true }))
            {
                Uri uri = HttpTestServers.RedirectUriForDestinationUri(
                    secure: false,
                    statusCode: 302,
                    destinationUri: HttpTestServers.RemoteEchoServer,
                    hops: 1,
                    relative: true);
                _output.WriteLine("Uri: {0}", uri);

                using (HttpResponseMessage response = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    Assert.Equal(HttpTestServers.RemoteEchoServer, response.RequestMessage.RequestUri);
                }
            }
        }

        [Theory]
        [InlineData(200)]
        [InlineData(201)]
        [InlineData(400)]
        public async Task GetAsync_AllowAutoRedirectTrue_NonRedirectStatusCode_LocationHeader_NoRedirect(int statusCode)
        {
            using (var handler = new HttpClientHandler() { AllowAutoRedirect = true })
            using (var client = new HttpClient(handler))
            {
                await LoopbackServer.CreateServerAsync(async (origServer, origUrl) =>
                {
                    await LoopbackServer.CreateServerAsync(async (redirectServer, redirectUrl) =>
                    {
                        Task<HttpResponseMessage> getResponse = client.GetAsync(origUrl);

                        Task redirectTask = LoopbackServer.ReadRequestAndSendResponseAsync(redirectServer);

                        await LoopbackServer.ReadRequestAndSendResponseAsync(origServer,
                                $"HTTP/1.1 {statusCode} OK\r\n" +
                                $"Date: {DateTimeOffset.UtcNow:R}\r\n" +
                                $"Location: {redirectUrl}\r\n" +
                                "\r\n");

                        using (HttpResponseMessage response = await getResponse)
                        {
                            Assert.Equal(statusCode, (int)response.StatusCode);
                            Assert.Equal(origUrl, response.RequestMessage.RequestUri);
                            Assert.False(redirectTask.IsCompleted, "Should not have redirected to Location");
                        }
                    });
                });
            }
        }

        [Theory]
        [PlatformSpecific(PlatformID.AnyUnix)]
        [InlineData("#origFragment", "", "#origFragment", false)]
        [InlineData("#origFragment", "", "#origFragment", true)]
        [InlineData("", "#redirFragment", "#redirFragment", false)]
        [InlineData("", "#redirFragment", "#redirFragment", true)]
        [InlineData("#origFragment", "#redirFragment", "#redirFragment", false)]
        [InlineData("#origFragment", "#redirFragment", "#redirFragment", true)]
        public async Task GetAsync_AllowAutoRedirectTrue_RetainsOriginalFragmentIfAppropriate(
            string origFragment, string redirFragment, string expectedFragment, bool useRelativeRedirect)
        {
            using (var handler = new HttpClientHandler() { AllowAutoRedirect = true })
            using (var client = new HttpClient(handler))
            {
                await LoopbackServer.CreateServerAsync(async (origServer, origUrl) =>
                {
                    origUrl = new Uri(origUrl.ToString() + origFragment);
                    Uri redirectUrl = useRelativeRedirect ?
                        new Uri(origUrl.PathAndQuery + redirFragment, UriKind.Relative) :
                        new Uri(origUrl.ToString() + redirFragment);
                    Uri expectedUrl = new Uri(origUrl.ToString() + expectedFragment);

                    Task<HttpResponseMessage> getResponse = client.GetAsync(origUrl);
                    Task firstRequest = LoopbackServer.ReadRequestAndSendResponseAsync(origServer,
                            $"HTTP/1.1 302 OK\r\n" +
                            $"Date: {DateTimeOffset.UtcNow:R}\r\n" +
                            $"Location: {redirectUrl}\r\n" +
                            "\r\n");
                    Assert.Equal(firstRequest, await Task.WhenAny(firstRequest, getResponse));

                    Task secondRequest = LoopbackServer.ReadRequestAndSendResponseAsync(origServer,
                            $"HTTP/1.1 200 OK\r\n" +
                            $"Date: {DateTimeOffset.UtcNow:R}\r\n" +
                            "\r\n");
                    await TestHelper.WhenAllCompletedOrAnyFailed(secondRequest, getResponse);

                    using (HttpResponseMessage response = await getResponse)
                    {
                        Assert.Equal(200, (int)response.StatusCode);
                        Assert.Equal(expectedUrl, response.RequestMessage.RequestUri);
                    }
                });
            }
        }

        [Fact]
        public async Task GetAsync_CredentialIsNetworkCredentialUriRedirect_StatusCodeUnauthorized()
        {
            var handler = new HttpClientHandler();
            handler.Credentials = _credential;
            using (var client = new HttpClient(handler))
            {
                Uri redirectUri = HttpTestServers.RedirectUriForCreds(
                    secure:false,
                    statusCode:302,
                    userName:Username,
                    password:Password);
                using (HttpResponseMessage unAuthResponse = await client.GetAsync(redirectUri))
                {
                    Assert.Equal(HttpStatusCode.Unauthorized, unAuthResponse.StatusCode);
                }
            }
       }

        [Theory, MemberData(nameof(RedirectStatusCodes))]
        public async Task GetAsync_CredentialIsCredentialCacheUriRedirect_StatusCodeOK(int statusCode)
        {
            Uri uri = HttpTestServers.BasicAuthUriForCreds(secure:false, userName:Username, password:Password);
            Uri redirectUri = HttpTestServers.RedirectUriForCreds(
                secure:false,
                statusCode:statusCode,
                userName:Username,
                password:Password);
            _output.WriteLine(uri.AbsoluteUri);
            _output.WriteLine(redirectUri.AbsoluteUri);
            var credentialCache = new CredentialCache();
            credentialCache.Add(uri, "Basic", _credential);

            var handler = new HttpClientHandler();
            handler.Credentials = credentialCache;
            using (var client = new HttpClient(handler))
            {
                using (HttpResponseMessage response = await client.GetAsync(redirectUri))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    Assert.Equal(uri, response.RequestMessage.RequestUri);
                }
            }
        }

        [Fact]
        public async Task GetAsync_DefaultCoookieContainer_NoCookieSent()
        {
            using (HttpClient client = new HttpClient())
            {
                using (HttpResponseMessage httpResponse = await client.GetAsync(HttpTestServers.RemoteEchoServer))
                {
                    string responseText = await httpResponse.Content.ReadAsStringAsync();
                    _output.WriteLine(responseText);
                    Assert.False(TestHelper.JsonMessageContainsKey(responseText, "Cookie"));
                }
            }
        }

        [Theory]
        [InlineData("cookieName1", "cookieValue1")]
        public async Task GetAsync_SetCookieContainer_CookieSent(string cookieName, string cookieValue)
        {
            var handler = new HttpClientHandler();
            var cookieContainer = new CookieContainer();
            cookieContainer.Add(HttpTestServers.RemoteEchoServer, new Cookie(cookieName, cookieValue));
            handler.CookieContainer = cookieContainer;
            using (var client = new HttpClient(handler))
            {
                using (HttpResponseMessage httpResponse = await client.GetAsync(HttpTestServers.RemoteEchoServer))
                {
                    Assert.Equal(HttpStatusCode.OK, httpResponse.StatusCode);
                    string responseText = await httpResponse.Content.ReadAsStringAsync();
                    _output.WriteLine(responseText);
                    Assert.True(TestHelper.JsonMessageContainsKeyValue(responseText, cookieName, cookieValue));
                }
            }
        }

        [Theory]
        [InlineData("cookieName1", "cookieValue1")]
        public async Task GetAsync_RedirectResponseHasCookie_CookieSentToFinalUri(string cookieName, string cookieValue)
        {
            Uri uri = HttpTestServers.RedirectUriForDestinationUri(
                secure:false,
                statusCode:302,
                destinationUri:HttpTestServers.RemoteEchoServer,
                hops:1);
            using (HttpClient client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add(
                    "X-SetCookie",
                    string.Format("{0}={1};Path=/", cookieName, cookieValue));
                using (HttpResponseMessage httpResponse = await client.GetAsync(uri))
                {
                    string responseText = await httpResponse.Content.ReadAsStringAsync();
                    _output.WriteLine(responseText);
                    Assert.True(TestHelper.JsonMessageContainsKeyValue(responseText, cookieName, cookieValue));
                }
            }            
        }

        [Theory, MemberData(nameof(HeaderValueAndUris))]
        public async Task GetAsync_RequestHeadersAddCustomHeaders_HeaderAndValueSent(string name, string value, Uri uri)
        {
            using (var client = new HttpClient())
            {
                client.DefaultRequestHeaders.Add(name, value);
                using (HttpResponseMessage httpResponse = await client.GetAsync(uri))
                {
                    Assert.Equal(HttpStatusCode.OK, httpResponse.StatusCode);
                    string responseText = await httpResponse.Content.ReadAsStringAsync();
                    Assert.True(TestHelper.JsonMessageContainsKeyValue(responseText, name, value));
                }
            }
        }

        private static KeyValuePair<string, string> GenerateCookie(string name, char repeat, int overallHeaderValueLength)
        {
            string emptyHeaderValue = $"{name}=; Path=/";

            Debug.Assert(overallHeaderValueLength > emptyHeaderValue.Length);

            int valueCount = overallHeaderValueLength - emptyHeaderValue.Length;
            string value = new string(repeat, valueCount);

            return new KeyValuePair<string, string>(name, value);
        }

        public static object[][] CookieNameValues =
        {
            // WinHttpHandler calls WinHttpQueryHeaders to iterate through multiple Set-Cookie header values,
            // using an initial buffer size of 128 chars. If the buffer is not large enough, WinHttpQueryHeaders
            // returns an insufficient buffer error, allowing WinHttpHandler to try again with a larger buffer.
            // Sometimes when WinHttpQueryHeaders fails due to insufficient buffer, it still advances the
            // iteration index, which would cause header values to be missed if not handled correctly.
            //
            // In particular, WinHttpQueryHeader behaves as follows for the following header value lengths:
            //  * 0-127 chars: succeeds, index advances from 0 to 1.
            //  * 128-255 chars: fails due to insufficient buffer, index advances from 0 to 1.
            //  * 256+ chars: fails due to insufficient buffer, index stays at 0.
            //
            // The below overall header value lengths were chosen to exercise reading header values at these
            // edges, to ensure WinHttpHandler does not miss multiple Set-Cookie headers.

            new object[] { GenerateCookie(name: "foo", repeat: 'a', overallHeaderValueLength: 126) },
            new object[] { GenerateCookie(name: "foo", repeat: 'a', overallHeaderValueLength: 127) },
            new object[] { GenerateCookie(name: "foo", repeat: 'a', overallHeaderValueLength: 128) },
            new object[] { GenerateCookie(name: "foo", repeat: 'a', overallHeaderValueLength: 129) },

            new object[] { GenerateCookie(name: "foo", repeat: 'a', overallHeaderValueLength: 254) },
            new object[] { GenerateCookie(name: "foo", repeat: 'a', overallHeaderValueLength: 255) },
            new object[] { GenerateCookie(name: "foo", repeat: 'a', overallHeaderValueLength: 256) },
            new object[] { GenerateCookie(name: "foo", repeat: 'a', overallHeaderValueLength: 257) },

            new object[]
            {
                new KeyValuePair<string, string>(
                    ".AspNetCore.Antiforgery.Xam7_OeLcN4",
                    "CfDJ8NGNxAt7CbdClq3UJ8_6w_4661wRQZT1aDtUOIUKshbcV4P0NdS8klCL5qGSN-PNBBV7w23G6MYpQ81t0PMmzIN4O04fqhZ0u1YPv66mixtkX3iTi291DgwT3o5kozfQhe08-RAExEmXpoCbueP_QYM")
            }
        };

        [Theory]
        [MemberData(nameof(CookieNameValues))]
        public async Task GetAsync_ResponseWithSetCookieHeaders_AllCookiesRead(KeyValuePair<string, string> cookie1)
        {
            var cookie2 = new KeyValuePair<string, string>(".AspNetCore.Session", "RAExEmXpoCbueP_QYM");
            var cookie3 = new KeyValuePair<string, string>("name", "value");

            await LoopbackServer.CreateServerAsync(async (server, url) =>
            {
                using (var handler = new HttpClientHandler())
                using (var client = new HttpClient(handler))
                {
                    Task<HttpResponseMessage> getResponse = client.GetAsync(url);

                    await LoopbackServer.ReadRequestAndSendResponseAsync(server,
                            $"HTTP/1.1 200 OK\r\n" +
                            $"Date: {DateTimeOffset.UtcNow:R}\r\n" +
                            $"Set-Cookie: {cookie1.Key}={cookie1.Value}; Path=/\r\n" +
                            $"Set-Cookie: {cookie2.Key}={cookie2.Value}; Path=/\r\n" +
                            $"Set-Cookie: {cookie3.Key}={cookie3.Value}; Path=/\r\n" +
                            "\r\n");

                    using (HttpResponseMessage response = await getResponse)
                    {
                        Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                        CookieCollection cookies = handler.CookieContainer.GetCookies(url);

                        Assert.Equal(3, cookies.Count);
                        Assert.Equal(cookie1.Value, cookies[cookie1.Key].Value);
                        Assert.Equal(cookie2.Value, cookies[cookie2.Key].Value);
                        Assert.Equal(cookie3.Value, cookies[cookie3.Key].Value);
                    }
                }
            });
        }

        [Fact]
        public async Task GetAsync_ResponseHeadersRead_ReadFromEachIterativelyDoesntDeadlock()
        {
            using (var client = new HttpClient())
            {
                const int NumGets = 5;
                Task<HttpResponseMessage>[] responseTasks = (from _ in Enumerable.Range(0, NumGets)
                                                             select client.GetAsync(HttpTestServers.RemoteEchoServer, HttpCompletionOption.ResponseHeadersRead)).ToArray();
                for (int i = responseTasks.Length - 1; i >= 0; i--) // read backwards to increase likelihood that we wait on a different task than has data available
                {
                    using (HttpResponseMessage response = await responseTasks[i])
                    {
                        string responseContent = await response.Content.ReadAsStringAsync();
                        _output.WriteLine(responseContent);                    
                        TestHelper.VerifyResponseBody(
                            responseContent,
                            response.Content.Headers.ContentMD5,
                            false,
                            null);                    
                   }
                }
            }
        }

        [Fact]
        public async Task SendAsync_HttpRequestMsgResponseHeadersRead_StatusCodeOK()
        {
            HttpRequestMessage request = new HttpRequestMessage(HttpMethod.Get, HttpTestServers.SecureRemoteEchoServer);
            using (var client = new HttpClient())
            {
                using (HttpResponseMessage response = await client.SendAsync(request, HttpCompletionOption.ResponseHeadersRead))
                {
                    string responseContent = await response.Content.ReadAsStringAsync();
                    _output.WriteLine(responseContent);                    
                    TestHelper.VerifyResponseBody(
                        responseContent,
                        response.Content.Headers.ContentMD5,
                        false,
                        null);                    
                }
            }
        }

        [Fact]
        public async Task SendAsync_ReadFromSlowStreamingServer_PartialDataReturned()
        {
            await LoopbackServer.CreateServerAsync(async (server, url) =>
            {
                using (var client = new HttpClient())
                {
                    Task<HttpResponseMessage> getResponse = client.GetAsync(url, HttpCompletionOption.ResponseHeadersRead);

                    await LoopbackServer.AcceptSocketAsync(server, async (s, stream, reader, writer) =>
                    {
                        while (!string.IsNullOrEmpty(reader.ReadLine())) ;
                        await writer.WriteAsync(
                            "HTTP/1.1 200 OK\r\n" +
                            $"Date: {DateTimeOffset.UtcNow:R}\r\n" +
                            "Content-Length: 16000\r\n" +
                            "\r\n" +
                            "less than 16000 bytes");

                        using (HttpResponseMessage response = await getResponse)
                        {
                            var buffer = new byte[8000];
                            using (Stream clientStream = await response.Content.ReadAsStreamAsync())
                            {
                                int bytesRead = await clientStream.ReadAsync(buffer, 0, buffer.Length);
                                _output.WriteLine($"Bytes read from stream: {bytesRead}");
                                Assert.True(bytesRead < buffer.Length, "bytesRead should be less than buffer.Length");
                            }
                        }
                    });
                }
            });
        }

        [Fact]
        public async Task Dispose_DisposingHandlerCancelsActiveOperationsWithoutResponses()
        {
            await LoopbackServer.CreateServerAsync(async (socket1, url1) =>
            {
                await LoopbackServer.CreateServerAsync(async (socket2, url2) =>
                {
                    await LoopbackServer.CreateServerAsync(async (socket3, url3) =>
                    {
                        var unblockServers = new TaskCompletionSource<bool>(TaskContinuationOptions.RunContinuationsAsynchronously);

                        // First server connects but doesn't send any response yet
                        Task server1 = LoopbackServer.AcceptSocketAsync(socket1, async (s, stream, reader, writer) =>
                        {
                            await unblockServers.Task;
                        });

                        // Second server connects and sends some but not all headers
                        Task server2 = LoopbackServer.AcceptSocketAsync(socket2, async (s, stream, reader, writer) =>
                        {
                            while (!string.IsNullOrEmpty(await reader.ReadLineAsync())) ;
                            await writer.WriteAsync($"HTTP/1.1 200 OK\r\n");
                            await unblockServers.Task;
                        });

                        // Third server connects and sends all headers and some but not all of the body
                        Task server3 = LoopbackServer.AcceptSocketAsync(socket3, async (s, stream, reader, writer) =>
                        {
                            while (!string.IsNullOrEmpty(await reader.ReadLineAsync())) ;
                            await writer.WriteAsync($"HTTP/1.1 200 OK\r\nDate: {DateTimeOffset.UtcNow:R}\r\nContent-Length: 20\r\n\r\n");
                            await writer.WriteAsync("1234567890");
                            await unblockServers.Task;
                            await writer.WriteAsync("1234567890");
                            s.Shutdown(SocketShutdown.Send);
                        });

                        // Make three requests
                        Task<HttpResponseMessage> get1, get2;
                        HttpResponseMessage response3;
                        using (var client = new HttpClient())
                        {
                            get1 = client.GetAsync(url1, HttpCompletionOption.ResponseHeadersRead);
                            get2 = client.GetAsync(url2, HttpCompletionOption.ResponseHeadersRead);
                            response3 = await client.GetAsync(url3, HttpCompletionOption.ResponseHeadersRead);
                        }

                        // Requests 1 and 2 should be canceled as we haven't finished receiving their headers
                        await Assert.ThrowsAnyAsync<OperationCanceledException>(() => get1);
                        await Assert.ThrowsAnyAsync<OperationCanceledException>(() => get2);

                        // Request 3 should still be active, and we should be able to receive all of the data.
                        unblockServers.SetResult(true);
                        using (response3)
                        {
                            Assert.Equal("12345678901234567890", await response3.Content.ReadAsStringAsync());
                        }
                    });
                });
            });
        }

        #region Post Methods Tests

        [Theory, MemberData(nameof(VerifyUploadServers))]
        public async Task PostAsync_CallMethodTwice_StringContent(Uri remoteServer)
        {
            using (var client = new HttpClient())
            {
                string data = "Test String";
                var content = new StringContent(data, Encoding.UTF8);
                content.Headers.ContentMD5 = TestHelper.ComputeMD5Hash(data);
                HttpResponseMessage response;
                using (response = await client.PostAsync(remoteServer, content))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                }

                // Repeat call.
                content = new StringContent(data, Encoding.UTF8);
                content.Headers.ContentMD5 = TestHelper.ComputeMD5Hash(data);
                using (response = await client.PostAsync(remoteServer, content))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                }
            }
        }

        [Theory, MemberData(nameof(VerifyUploadServers))]
        public async Task PostAsync_CallMethod_UnicodeStringContent(Uri remoteServer)
        {
            using (var client = new HttpClient())
            {
                string data = "\ub4f1\uffc7\u4e82\u67ab4\uc6d4\ud1a0\uc694\uc77c\uffda3\u3155\uc218\uffdb";
                var content = new StringContent(data, Encoding.UTF8);
                content.Headers.ContentMD5 = TestHelper.ComputeMD5Hash(data);
                
                using (HttpResponseMessage response = await client.PostAsync(remoteServer, content))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                }
            }
        }

        [Theory, MemberData(nameof(VerifyUploadServersStreamsAndExpectedData))]
        public async Task PostAsync_CallMethod_StreamContent(Uri remoteServer, HttpContent content, byte[] expectedData)
        {
            using (var client = new HttpClient())
            {
                content.Headers.ContentMD5 = TestHelper.ComputeMD5Hash(expectedData);

                using (HttpResponseMessage response = await client.PostAsync(remoteServer, content))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                }
            }
        }

        private sealed class StreamContentWithSyncAsyncCopy : StreamContent
        {
            private readonly Stream _stream;
            private readonly bool _syncCopy;

            public StreamContentWithSyncAsyncCopy(Stream stream, bool syncCopy) : base(stream)
            {
                _stream = stream;
                _syncCopy = syncCopy;
            }

            protected override Task SerializeToStreamAsync(Stream stream, TransportContext context)
            {
                if (_syncCopy)
                {
                    try
                    {
                        _stream.CopyTo(stream, 128); // arbitrary size likely to require multiple read/writes
                        return Task.CompletedTask;
                    }
                    catch (Exception exc)
                    {
                        return Task.FromException(exc);
                    }
                }

                return base.SerializeToStreamAsync(stream, context);
            }
        }

        public static IEnumerable<object[]> VerifyUploadServersStreamsAndExpectedData
        {
            get
            {
                foreach (object[] serverArr in VerifyUploadServers) // target server
                foreach (bool syncCopy in new[] { true, false }) // force the content copy to happen via Read/Write or ReadAsync/WriteAsync
                {
                    Uri server = (Uri)serverArr[0];

                    byte[] data = new byte[1234];
                    new Random(42).NextBytes(data);

                    // A MemoryStream
                    {
                        var memStream = new MemoryStream(data, writable: false);
                        yield return new object[] { server, new StreamContentWithSyncAsyncCopy(memStream, syncCopy: syncCopy), data };
                    }

                    // A stream that provides the data synchronously and has a known length
                    {
                        var wrappedMemStream = new MemoryStream(data, writable: false);
                        var syncKnownLengthStream = new DelegateStream(
                            canReadFunc: () => wrappedMemStream.CanRead,
                            canSeekFunc: () => wrappedMemStream.CanSeek,
                            lengthFunc: () => wrappedMemStream.Length,
                            positionGetFunc: () => wrappedMemStream.Position,
                            readAsyncFunc: (buffer, offset, count, token) => wrappedMemStream.ReadAsync(buffer, offset, count, token));
                        yield return new object[] { server, new StreamContentWithSyncAsyncCopy(syncKnownLengthStream, syncCopy: syncCopy), data };
                    }

                    // A stream that provides the data synchronously and has an unknown length
                    {
                        int syncUnknownLengthStreamOffset = 0;
                        var syncUnknownLengthStream = new DelegateStream(
                            canReadFunc: () => true,
                            canSeekFunc: () => false,
                            readAsyncFunc: (buffer, offset, count, token) =>
                            {
                                int bytesRemaining = data.Length - syncUnknownLengthStreamOffset;
                                int bytesToCopy = Math.Min(bytesRemaining, count);
                                Array.Copy(data, syncUnknownLengthStreamOffset, buffer, offset, bytesToCopy);
                                syncUnknownLengthStreamOffset += bytesToCopy;
                                return Task.FromResult(bytesToCopy);
                            });
                        yield return new object[] { server, new StreamContentWithSyncAsyncCopy(syncUnknownLengthStream, syncCopy: syncCopy), data };
                    }

                    // A stream that provides the data asynchronously
                    {
                        int asyncStreamOffset = 0, maxDataPerRead = 100;
                        var asyncStream = new DelegateStream(
                            canReadFunc: () => true,
                            canSeekFunc: () => false,
                            readAsyncFunc: async (buffer, offset, count, token) =>
                            {
                                await Task.Delay(1).ConfigureAwait(false);
                                int bytesRemaining = data.Length - asyncStreamOffset;
                                int bytesToCopy = Math.Min(bytesRemaining, Math.Min(maxDataPerRead, count));
                                Array.Copy(data, asyncStreamOffset, buffer, offset, bytesToCopy);
                                asyncStreamOffset += bytesToCopy;
                                return bytesToCopy;
                            });
                        yield return new object[] { server, new StreamContentWithSyncAsyncCopy(asyncStream, syncCopy: syncCopy), data };
                    }
                }
            }
        }

        [Theory, MemberData(nameof(EchoServers))]
        public async Task PostAsync_CallMethod_NullContent(Uri remoteServer)
        {
            using (var client = new HttpClient())
            {
                using (HttpResponseMessage response = await client.PostAsync(remoteServer, null))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    
                    string responseContent = await response.Content.ReadAsStringAsync();
                    _output.WriteLine(responseContent);                    
                    TestHelper.VerifyResponseBody(
                        responseContent,
                        response.Content.Headers.ContentMD5,
                        false,
                        string.Empty);                    
                }
            }
        }

        [Theory, MemberData(nameof(EchoServers))]
        public async Task PostAsync_CallMethod_EmptyContent(Uri remoteServer)
        {
            using (var client = new HttpClient())
            {
                var content = new StringContent(string.Empty);
                using (HttpResponseMessage response = await client.PostAsync(remoteServer, content))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    
                    string responseContent = await response.Content.ReadAsStringAsync();
                    _output.WriteLine(responseContent);                    
                    TestHelper.VerifyResponseBody(
                        responseContent,
                        response.Content.Headers.ContentMD5,
                        false,
                        string.Empty);                  
                }
            }
        }

        [Theory]
        [InlineData(false)]
        [InlineData(true)]
        public async Task PostAsync_Redirect_ResultingGetFormattedCorrectly(bool secure)
        {
            const string ContentString = "This is the content string.";
            var content = new StringContent(ContentString);
            Uri redirectUri = HttpTestServers.RedirectUriForDestinationUri(
                secure,
                302,
                secure ? HttpTestServers.SecureRemoteEchoServer : HttpTestServers.RemoteEchoServer,
                1);

            using (var client = new HttpClient())
            using (HttpResponseMessage response = await client.PostAsync(redirectUri, content))
            {
                Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                string responseContent = await response.Content.ReadAsStringAsync();
                Assert.DoesNotContain(ContentString, responseContent);
                Assert.DoesNotContain("Content-Length", responseContent);
            }
        }

        [Theory]
        [InlineData(HttpStatusCode.MethodNotAllowed, "Custom description")]
        [InlineData(HttpStatusCode.MethodNotAllowed, "")]
        public async Task GetAsync_CallMethod_ExpectedStatusLine(HttpStatusCode statusCode, string reasonPhrase)
        {
            using (var client = new HttpClient())
            {
                using (HttpResponseMessage response = await client.GetAsync(HttpTestServers.StatusCodeUri(
                    false,
                    (int)statusCode,
                    reasonPhrase)))
                {
                    Assert.Equal(statusCode, response.StatusCode);
                    Assert.Equal(reasonPhrase, response.ReasonPhrase);
                }
            }
        }

        [PlatformSpecific(PlatformID.Windows)] // CopyToAsync(Stream, TransportContext) isn't used on unix
        [Fact]
        public async Task PostAsync_Post_ChannelBindingHasExpectedValue()
        {
            using (var client = new HttpClient())
            {
                string expectedContent = "Test contest";
                var content = new ChannelBindingAwareContent(expectedContent);
                using (HttpResponseMessage response = await client.PostAsync(HttpTestServers.SecureRemoteEchoServer, content))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);

                    ChannelBinding channelBinding = content.ChannelBinding;
                    Assert.NotNull(channelBinding);
                    _output.WriteLine("Channel Binding: {0}", channelBinding);
                }
            }
        }

        #endregion

        #region Various HTTP Method Tests

        [Theory, MemberData(nameof(HttpMethods))]
        public async Task SendAsync_SendRequestUsingMethodToEchoServerWithNoContent_MethodCorrectlySent(
            string method,
            bool secureServer)
        {
            using (var client = new HttpClient())
            {
                var request = new HttpRequestMessage(
                    new HttpMethod(method), 
                    secureServer ? HttpTestServers.SecureRemoteEchoServer : HttpTestServers.RemoteEchoServer);
                using (HttpResponseMessage response = await client.SendAsync(request))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    TestHelper.VerifyRequestMethod(response, method);
                }
            }        
        }

        [Theory, MemberData(nameof(HttpMethodsThatAllowContent))]
        public async Task SendAsync_SendRequestUsingMethodToEchoServerWithContent_Success(
            string method,
            bool secureServer)
        {
            using (var client = new HttpClient())
            {
                var request = new HttpRequestMessage(
                    new HttpMethod(method), 
                    secureServer ? HttpTestServers.SecureRemoteEchoServer : HttpTestServers.RemoteEchoServer);
                request.Content = new StringContent(ExpectedContent);
                using (HttpResponseMessage response = await client.SendAsync(request))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    TestHelper.VerifyRequestMethod(response, method);
                    string responseContent = await response.Content.ReadAsStringAsync();
                    _output.WriteLine(responseContent);

                    Assert.Contains($"\"Content-Length\": \"{request.Content.Headers.ContentLength.Value}\"", responseContent);
                    TestHelper.VerifyResponseBody(
                        responseContent,
                        response.Content.Headers.ContentMD5,
                        false,
                        ExpectedContent);                    
                }
            }        
        }
        #endregion

        #region Version tests
        [Fact]
        public async Task SendAsync_RequestVersion10_ServerReceivesVersion10Request()
        {
            Version receivedRequestVersion = await SendRequestAndGetRequestVersionAsync(new Version(1, 0));
            Assert.Equal(new Version(1, 0), receivedRequestVersion);
        }

        [Fact]
        public async Task SendAsync_RequestVersion11_ServerReceivesVersion11Request()
        {
            Version receivedRequestVersion = await SendRequestAndGetRequestVersionAsync(new Version(1, 1));
            Assert.Equal(new Version(1, 1), receivedRequestVersion);
        }

        [Fact]
        public async Task SendAsync_RequestVersionNotSpecified_ServerReceivesVersion11Request()
        {
            // The default value for HttpRequestMessage.Version is Version(1,1).
            // So, we need to set something different (0,0), to test the "unknown" version.
            Version receivedRequestVersion = await SendRequestAndGetRequestVersionAsync(new Version(0,0));
            Assert.Equal(new Version(1, 1), receivedRequestVersion);
        }

        [Theory, MemberData(nameof(Http2Servers))]
        public async Task SendAsync_RequestVersion20_ResponseVersion20IfHttp2Supported(Uri server)
        {
            // We don't currently have a good way to test whether HTTP/2 is supported without
            // using the same mechanism we're trying to test, so for now we allow both 2.0 and 1.1 responses.
            var request = new HttpRequestMessage(HttpMethod.Get, server);
            request.Version = new Version(2, 0);

            using (var handler = new HttpClientHandler())
            using (var client = new HttpClient(handler, false))
            {
                // It is generally expected that the test hosts will be trusted, so we don't register a validation
                // callback in the usual case.
                // 
                // However, on our Debian 8 test machines, a combination of a server side TLS chain,
                // the client chain processor, and the distribution's CA bundle results in an incomplete/untrusted
                // certificate chain. See https://github.com/dotnet/corefx/issues/9244 for more details.
                if (PlatformDetection.IsDebian8)
                {
                    // Func<HttpRequestMessage, X509Certificate2, X509Chain, SslPolicyErrors, bool>
                    handler.ServerCertificateCustomValidationCallback = (msg, cert, chain, errors) =>
                    {
                        Assert.InRange(chain.ChainStatus.Length, 0, 1);

                        if (chain.ChainStatus.Length > 0)
                        {
                            Assert.Equal(X509ChainStatusFlags.PartialChain, chain.ChainStatus[0].Status);
                        }

                        return true;
                    };
                }

                using (HttpResponseMessage response = await client.SendAsync(request))
                {
                    Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    Assert.True(
                        response.Version == new Version(2, 0) ||
                        response.Version == new Version(1, 1),
                        "Response version " + response.Version);
                }
            }
        }

        private async Task<Version> SendRequestAndGetRequestVersionAsync(Version requestVersion)
        {
            Version receivedRequestVersion = null;

            await LoopbackServer.CreateServerAsync(async (server, url) =>
            {
                var request = new HttpRequestMessage(HttpMethod.Get, url);
                request.Version = requestVersion;

                using (var client = new HttpClient())
                {
                    Task<HttpResponseMessage> getResponse = client.SendAsync(request);

                    await LoopbackServer.AcceptSocketAsync(server, async (s, stream, reader, writer) =>
                    {
                        string statusLine = reader.ReadLine();
                        while (!string.IsNullOrEmpty(reader.ReadLine())) ;

                        if (statusLine.Contains("/1.0"))
                        {
                            receivedRequestVersion = new Version(1, 0);
                        }
                        else if (statusLine.Contains("/1.1"))
                        {
                            receivedRequestVersion = new Version(1, 1);
                        }
                        else
                        {
                            Assert.True(false, "Invalid HTTP request version");
                        }

                        await writer.WriteAsync(
                            $"HTTP/1.1 200 OK\r\n" +
                            $"Date: {DateTimeOffset.UtcNow:R}\r\n" +
                            "Content-Length: 0\r\n" +
                            "\r\n");
                        s.Shutdown(SocketShutdown.Send);
                    });

                    using (HttpResponseMessage response = await getResponse)
                    {
                        Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                    }
                }
            });
            
            return receivedRequestVersion;
        }
        #endregion

        #region SSL Version tests
        [Theory]
        [InlineData("SSLv2", HttpTestServers.SSLv2RemoteServer)]
        [InlineData("SSLv3", HttpTestServers.SSLv3RemoteServer)]
        public async Task GetAsync_UnsupportedSSLVersion_Throws(string name, string url)
        {
            using (HttpClient client = new HttpClient())
            {
                await Assert.ThrowsAsync<HttpRequestException>(() => client.GetAsync(url));
            }
        }

        [Theory]
        [InlineData("TLSv1.0", HttpTestServers.TLSv10RemoteServer)]
        [InlineData("TLSv1.1", HttpTestServers.TLSv11RemoteServer)]
        [InlineData("TLSv1.2", HttpTestServers.TLSv12RemoteServer)]
        public async Task GetAsync_SupportedSSLVersion_Succeeds(string name, string url)
        {
            using (HttpClient client = new HttpClient())
            using (await client.GetAsync(url))
            {
            }
        }
        #endregion

        #region Proxy tests
        [Theory]
        [MemberData(nameof(CredentialsForProxy))]
        public void Proxy_BypassFalse_GetRequestGoesThroughCustomProxy(ICredentials creds, bool wrapCredsInCache)
        {
            int port;
            Task<LoopbackGetRequestHttpProxy.ProxyResult> proxyTask = LoopbackGetRequestHttpProxy.StartAsync(
                out port,
                requireAuth: creds != null && creds != CredentialCache.DefaultCredentials,
                expectCreds: true);
            Uri proxyUrl = new Uri($"http://localhost:{port}");

            const string BasicAuth = "Basic";
            if (wrapCredsInCache)
            {
                Assert.IsAssignableFrom<NetworkCredential>(creds);
                var cache = new CredentialCache();
                cache.Add(proxyUrl, BasicAuth, (NetworkCredential)creds);
                creds = cache;
            }

            using (var handler = new HttpClientHandler() { Proxy = new UseSpecifiedUriWebProxy(proxyUrl, creds) })
            using (var client = new HttpClient(handler))
            {
                Task<HttpResponseMessage> responseTask = client.GetAsync(HttpTestServers.RemoteEchoServer);
                Task<string> responseStringTask = responseTask.ContinueWith(t => t.Result.Content.ReadAsStringAsync(), TaskScheduler.Default).Unwrap();
                Task.WaitAll(proxyTask, responseTask, responseStringTask);

                TestHelper.VerifyResponseBody(responseStringTask.Result, responseTask.Result.Content.Headers.ContentMD5, false, null);
                Assert.Equal(Encoding.ASCII.GetString(proxyTask.Result.ResponseContent), responseStringTask.Result);

                NetworkCredential nc = creds?.GetCredential(proxyUrl, BasicAuth);
                string expectedAuth =
                    nc == null || nc == CredentialCache.DefaultCredentials ? null :
                    string.IsNullOrEmpty(nc.Domain) ? $"{nc.UserName}:{nc.Password}" :
                    $"{nc.Domain}\\{nc.UserName}:{nc.Password}";
                Assert.Equal(expectedAuth, proxyTask.Result.AuthenticationHeaderValue);
            }
        }

        [Theory]
        [MemberData(nameof(BypassedProxies))]
        public async Task Proxy_BypassTrue_GetRequestDoesntGoesThroughCustomProxy(IWebProxy proxy)
        {
            using (var client = new HttpClient(new HttpClientHandler() { Proxy = proxy }))
            using (HttpResponseMessage response = await client.GetAsync(HttpTestServers.RemoteEchoServer))
            {
                TestHelper.VerifyResponseBody(
                    await response.Content.ReadAsStringAsync(),
                    response.Content.Headers.ContentMD5,
                    false,
                    null);
            }
        }

        [Fact]
        public void Proxy_HaveNoCredsAndUseAuthenticatedCustomProxy_ProxyAuthenticationRequiredStatusCode()
        {
            int port;
            Task<LoopbackGetRequestHttpProxy.ProxyResult> proxyTask = LoopbackGetRequestHttpProxy.StartAsync(
                out port,
                requireAuth: true,
                expectCreds: false);
            Uri proxyUrl = new Uri($"http://localhost:{port}");

            using (var handler = new HttpClientHandler() { Proxy = new UseSpecifiedUriWebProxy(proxyUrl, null) })
            using (var client = new HttpClient(handler))
            {
                Task<HttpResponseMessage> responseTask = client.GetAsync(HttpTestServers.RemoteEchoServer);
                Task.WaitAll(proxyTask, responseTask);

                Assert.Equal(HttpStatusCode.ProxyAuthenticationRequired, responseTask.Result.StatusCode);
            }
        }        

        private static IEnumerable<object[]> BypassedProxies()
        {
            yield return new object[] { null };
            yield return new object[] { new PlatformNotSupportedWebProxy() };
            yield return new object[] { new UseSpecifiedUriWebProxy(new Uri($"http://{Guid.NewGuid().ToString().Substring(0, 15)}:12345"), bypass: true) };
        }

        private static IEnumerable<object[]> CredentialsForProxy()
        {
            yield return new object[] { null, false };
            foreach (bool wrapCredsInCache in new[] { true, false })
            {
                yield return new object[] { CredentialCache.DefaultCredentials, wrapCredsInCache };
                yield return new object[] { new NetworkCredential("user:name", "password"), wrapCredsInCache };
                yield return new object[] { new NetworkCredential("username", "password", "dom:\\ain"), wrapCredsInCache };
            }
        }
        #endregion

        #region Uri wire transmission encoding tests
        [Fact]
        public async Task SendRequest_UriPathHasReservedChars_ServerReceivedExpectedPath()
        {
            await LoopbackServer.CreateServerAsync(async (server, rootUrl) =>
            {
                var uri = new Uri($"http://{rootUrl.Host}:{rootUrl.Port}/test[]");
                _output.WriteLine(uri.AbsoluteUri.ToString());
                var request = new HttpRequestMessage(HttpMethod.Get, uri);
                string statusLine = string.Empty;

                using (var client = new HttpClient())
                {
                    Task<HttpResponseMessage> getResponse = client.SendAsync(request);

                    await LoopbackServer.AcceptSocketAsync(server, async (s, stream, reader, writer) =>
                    {
                        statusLine = reader.ReadLine();
                        _output.WriteLine(statusLine);
                        while (!string.IsNullOrEmpty(reader.ReadLine())) ;

                        await writer.WriteAsync(
                            $"HTTP/1.1 200 OK\r\n" +
                            $"Date: {DateTimeOffset.UtcNow:R}\r\n" +
                            "Content-Length: 0\r\n" +
                            "\r\n");
                        s.Shutdown(SocketShutdown.Send);
                    });

                    using (HttpResponseMessage response = await getResponse)
                    {
                        Assert.Equal(HttpStatusCode.OK, response.StatusCode);
                        Assert.True(statusLine.Contains(uri.PathAndQuery), $"statusLine should contain {uri.PathAndQuery}");
                    }
                }
            });
        }
        #endregion
    }
}
