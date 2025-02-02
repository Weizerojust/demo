// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System.Collections.Immutable;
using System.Diagnostics;

namespace System.Reflection.Metadata.Ecma335
{
    internal struct MethodBodyEncoder
    {
        public BlobBuilder Builder { get; }

        private readonly ushort _maxStack;
        private readonly int _exceptionRegionCount;
        private readonly StandaloneSignatureHandle _localVariablesSignature;
        private readonly byte _attributes;

        internal MethodBodyEncoder(
            BlobBuilder builder,
            ushort maxStack,
            int exceptionRegionCount,
            StandaloneSignatureHandle localVariablesSignature,
            MethodBodyAttributes attributes)
        {
            Builder = builder;
            _maxStack = maxStack;
            _localVariablesSignature = localVariablesSignature;
            _attributes = (byte)attributes;
            _exceptionRegionCount = exceptionRegionCount;
        }

        internal bool IsTiny(int codeSize)
        {
            return codeSize < 64 && _maxStack <= 8 && _localVariablesSignature.IsNil && _exceptionRegionCount == 0;
        }

        private int WriteHeader(int codeSize)
        {
            Blob blob;
            return WriteHeader(codeSize, false, out blob);
        }

        private int WriteHeader(int codeSize, bool codeSizeFixup, out Blob codeSizeBlob)
        {
            const int TinyFormat = 2;
            const int FatFormat = 3;
            const int MoreSections = 8;
            const byte InitLocals = 0x10;

            int offset;

            if (IsTiny(codeSize))
            {
                offset = Builder.Count;
                Builder.WriteByte((byte)((codeSize << 2) | TinyFormat));

                Debug.Assert(!codeSizeFixup);
                codeSizeBlob = default(Blob);
            }
            else
            {
                Builder.Align(4);

                offset = Builder.Count;

                ushort flags = (3 << 12) | FatFormat;
                if (_exceptionRegionCount > 0)
                {
                    flags |= MoreSections;
                }

                if ((_attributes & (int)MethodBodyAttributes.InitLocals) != 0)
                {
                    flags |= InitLocals;
                }

                Builder.WriteUInt16((ushort)(_attributes | flags));
                Builder.WriteUInt16(_maxStack);
                if (codeSizeFixup)
                {
                    codeSizeBlob = Builder.ReserveBytes(sizeof(int));
                }
                else
                {
                    codeSizeBlob = default(Blob);
                    Builder.WriteInt32(codeSize);
                }

                Builder.WriteInt32(_localVariablesSignature.IsNil ? 0 : MetadataTokens.GetToken(_localVariablesSignature));
            }

            return offset;
        }

        private ExceptionRegionEncoder CreateExceptionEncoder()
        {
            return new ExceptionRegionEncoder(
                Builder, 
                _exceptionRegionCount,
                hasLargeRegions: (_attributes & (int)MethodBodyAttributes.LargeExceptionRegions) != 0);
        }

        public ExceptionRegionEncoder WriteInstructions(ImmutableArray<byte> instructions, out int bodyOffset)
        {
            bodyOffset = WriteHeader(instructions.Length);
            Builder.WriteBytes(instructions);
            return CreateExceptionEncoder();
        }

        public ExceptionRegionEncoder WriteInstructions(ImmutableArray<byte> instructions, out int bodyOffset, out Blob instructionBlob)
        {
            bodyOffset = WriteHeader(instructions.Length);
            instructionBlob = Builder.ReserveBytes(instructions.Length);
            new BlobWriter(instructionBlob).WriteBytes(instructions);
            return CreateExceptionEncoder();
        }

        public ExceptionRegionEncoder WriteInstructions(BlobBuilder codeBuilder, out int bodyOffset)
        {
            bodyOffset = WriteHeader(codeBuilder.Count);
            codeBuilder.WriteContentTo(Builder);
            return CreateExceptionEncoder();
        }

        public ExceptionRegionEncoder WriteInstructions(BlobBuilder codeBuilder, BranchBuilder branchBuilder, out int bodyOffset)
        {
            if (branchBuilder == null || branchBuilder.BranchCount == 0)
            {
                return WriteInstructions(codeBuilder, out bodyOffset);
            }
            
            // When emitting branches we emitted short branches.
            int initialCodeSize = codeBuilder.Count;
            Blob codeSizeFixup;
            if (IsTiny(initialCodeSize))
            {
                // If the method is tiny so far then all branches have to be short 
                // (the max distance between any label and a branch instruction is < 64).
                bodyOffset = WriteHeader(initialCodeSize);
                codeSizeFixup = default(Blob);
            }
            else
            {
                // Otherwise, it's fat format and we can fixup the size later on:
                bodyOffset = WriteHeader(initialCodeSize, true, out codeSizeFixup);
            }

            int codeStartOffset = Builder.Count;
            branchBuilder.FixupBranches(codeBuilder, Builder);
            if (!codeSizeFixup.IsDefault)
            {
                new BlobWriter(codeSizeFixup).WriteInt32(Builder.Count - codeStartOffset);
            }
            else
            {
                Debug.Assert(initialCodeSize == Builder.Count - codeStartOffset);
            }

            return CreateExceptionEncoder();
        }
    }
}
