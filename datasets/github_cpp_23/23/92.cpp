#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include <climits>
#include <queue>

using namespace std;

const int MAXN    = 100;
const int MAXFLOW = INT_MAX;

bool BFS(int G[MAXN][MAXN], int s, int t, int Predecessor[MAXN])
{
   
   bool visited[MAXN];
   memset(visited, 0, sizeof(visited));
 
   
   
   queue <int> q;
   q.push(s);
   visited[s] = true;
   Predecessor[s] = -1;
 
   
   while (!q.empty())
   {
      int u = q.front();
      q.pop();
 
      for (int v=0; v<MAXN; v++)
      {
         if (visited[v]==false && G[u][v] > 0)
         {
            q.push(v);
            Predecessor[v] = u;
            visited[v] = true;
         }
      }
   }
 
   
   
   return (visited[t] == true);
}

int FordFulkerson(int G[MAXN][MAXN], int s, int t)
{
   int GRes[MAXN][MAXN];
   
   copy_n((int *)G, MAXN * MAXN, (int *)GRes);
   
   int Predecessor[MAXN];
   int Maxflow = 0;
   while(BFS(GRes, s, t, Predecessor))
   {
      
      int Bottleneck = MAXFLOW;
      
      for(int v = t, u = Predecessor[t]; v != s; v = u, u = Predecessor[u])
         Bottleneck = min(Bottleneck, GRes[u][v]);
      for(int v = t, u = Predecessor[t]; v != s; v = u, u = Predecessor[u])
      {
         GRes[u][v] -= Bottleneck;
         
         GRes[v][u] += Bottleneck;
      }
      Maxflow += Bottleneck;
   }
   return Maxflow;
}

int main(void)
{
   return 0;
}
