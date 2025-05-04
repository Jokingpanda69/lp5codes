#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// Parallel BFS using OpenMP
void parallelBFS(const vector<vector<int>> &graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "\nBFS Traversal:\n";

    while (!q.empty()) {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            int curr;

            #pragma omp critical
            {
                if (!q.empty()) {
                    curr = q.front();
                    q.pop();
                    cout << "Visited (BFS): " << curr << endl;
                }
            }

            for (int neighbor : graph[curr]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

// Parallel DFS utility using OpenMP
void parallelDFSUtil(const vector<vector<int>> &graph, int node, vector<bool> &visited) {
    #pragma omp critical
    {
        if (visited[node]) return;
        visited[node] = true;
        cout << "Visited (DFS): " << node << endl;
    }

    int n = graph[node].size();

    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        int neighbor = graph[node][i];

        // Avoid multiple threads exploring same node
        #pragma omp critical
        {
            if (!visited[neighbor]) {
                // Let each thread explore its own DFS subtree
                #pragma omp task
                parallelDFSUtil(graph, neighbor, visited);
            }
        }
    }
}

void parallelDFS(const vector<vector<int>> &graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);

    cout << "\nDFS Traversal:\n";

    #pragma omp parallel
    {
        #pragma omp single
        {
            parallelDFSUtil(graph, start, visited);
        }
    }
}

int main() {
    int nodes, edges;
    cout << "Enter number of nodes and edges: ";
    cin >> nodes >> edges;

    vector<vector<int>> graph(nodes);
    cout << "Enter " << edges << " edges (u v):" << endl;
    for (int i = 0; i < edges; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // Undirected graph
    }

    int start;
    cout << "Enter start node: ";
    cin >> start;

    parallelBFS(graph, start);
    parallelDFS(graph, start);

    return 0;
}
