



#include <GraphUtils/TopologicalSort.hpp>

TopologicalSort::TopologicalSort() : CriticalPath(){

}

vector<Edge> TopologicalSort::getCriticalPath(map<int, vector<Node> >& graph, map<int, Node>& vertexList,
                                              vector<int> &distances) {

    stack<int> sort = topologicalSort(graph, distances);
    vector<int> path = updateDistancesFromTopOrder(graph, sort, distances, vertexList);

    vector<Edge> criticalPath;
    for (int job = 0; job < ProblemInstance::getNumJobs(); job++) {

        int i = (job + 1) * vertexPerJob;
        int tamanho = 0;

        while (i != 0 && tamanho < nVertex - 1) {
            criticalPath.emplace_back(make_pair(vertexList[path[i]], vertexList[i]));
            i = path[i];
            tamanho++;
        }
    }

    return criticalPath;
}

stack<int> TopologicalSort::topologicalSort(map<int, vector<Node> >& graph,
                                             vector<int> &distances){

    stack<int> stack;
    vector<bool> visited(nVertex, false);

    
    
    for (int i = 0; i < nVertex; i++)
        if (visited[i] == false)
            topologicalSortUtil(i, visited, stack, graph);

    return (stack);
}

void TopologicalSort::topologicalSortUtil(int v, vector<bool>& visited,
                                          stack<int>& stack, map<int, vector<Node> >& graph){
    
    visited[v] = true;

    
    for (Node i: graph[v])
        if (!visited[i.index])
            topologicalSortUtil(i.index, visited, stack, graph);

    
    stack.push(v);
}

vector<int> TopologicalSort::updateDistancesFromTopOrder(map<int, vector<Node> >& graph,
                                                  std::stack<int>& order,
                                                  vector<int>& distances,
                                                  map<int, Node>& vertexList){

    distances.clear();
    distances.resize(nVertex, -INF);
    distances[0] = 0;

    vector<int> path(nVertex);

    
    while (!order.empty()){
        
        int u = order.top();
        order.pop();

        
        if (distances[u] != -INF){
            for (Node i: graph[u]){
                if (distances[i.index] < distances[u] + vertexList[u].weight){
                    distances[i.index] = distances[u] + vertexList[u].weight;
                    path[i.index] = u;
                }
            }
        }
    }

    return path;
}