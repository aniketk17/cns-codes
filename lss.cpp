//B2
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stack>
using namespace std;

const int INF = numeric_limits<int>::max();


void dijkstra(int src, int n, vector<vector<pair<int, int>>>& adj, vector<int>& dist, vector<int>& prev) {
    dist.assign(n, INF);
    prev.assign(n, -1);
    dist[src] = 0;

    
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue; 

        for (auto& edge : adj[u]) {
            int v = edge.first;   
            int weight = edge.second;  

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;  
                pq.push({dist[v], v});
            }
        }
    }
}


void printPath(int src, int dest, vector<int>& prev) {
    if (prev[dest] == -1) {
        cout << "No path";
        return;
    }
    stack<int> path;
    for (int at = dest; at != -1; at = prev[at]) {
        path.push(at);
    }

    cout << path.top();
    path.pop();
    while (!path.empty()) {
        cout <<  "->" << path.top() ;
        path.pop();
    }
}

int main() {
    int n, e;

    
    cout << "Enter the number of nodes: ";
    cin >> n;
    cout << "Enter the number of edges: ";
    cin >> e;

    
    vector<vector<pair<int, int>>> adj(n);

    
    cout << "Enter the edges (format: src, dest, weight):" << endl;
    for (int i = 0; i < e; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    
    for (int src = 0; src < n; ++src) {
        vector<int> dist(n);  
        vector<int> prev(n);  

        dijkstra(src, n, adj, dist, prev);

        
        cout << "\nShortest paths from node " << src << ":\n";
        for (int dest = 0; dest < n; ++dest) {
            if (src != dest) {
                cout << "Path to " << dest << ": ";
                if (dist[dest] == INF) {
                    cout << "No path exist!!!" << endl;
                } else {
                    printPath(src, dest, prev);
                    cout << " [Cost = " << dist[dest] << "]" << endl;
                }
            }
        }
    }

    return 0;
}
