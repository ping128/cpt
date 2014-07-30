///////////////////////////////////////////////////////////////////////////////
//  Shortest Path
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <queue>
#include <vector>
#include <cassert>

using namespace std;

template<class Cost_t, Cost_t INF>
class ShortestPath {
public:
    typedef pair<Cost_t, int> PCI; // (cost, vertex)
    vector<vector<PCI> > adj;
    vector<Cost_t> min_cost;
    int N;
    // Vertices are 1-based indexed.
    ShortestPath(int n) : N(n) { adj.resize(N + 5); min_cost.resize(N + 5); }
    void add_edge(int u, int v, Cost_t c) { adj[u].push_back(PCI(c, v)); }
    Cost_t dijkstra(int from, int to) { dijkstra_algorithm(from, to); return min_cost[to]; }
    vector<Cost_t> dijkstra(int from) { dijkstra_algorithm(from, -1); return min_cost; }
private:
    void dijkstra_algorithm(int from, int to){
        int u, v;
        Cost_t c;
        for(int i = 1; i <= N; i++ ) { min_cost[i] = INF; }
        min_cost[from] = 0;
        priority_queue<PCI, vector<PCI>, greater<PCI> > Q;
        vector<bool> seen (N + 5);
        Q.push(PCI(min_cost[from], from));
        while (!Q.empty()) {
            u = Q.top().second;
            Q.pop();
            if (u == to) break;
            if (seen[u]) continue;
            seen[u] = true;
            int sz = adj[u].size();
            for (int i = 0; i < sz; i++) {
                v = adj[u][i].second;
                c = adj[u][i].first;
                if (!seen[v] && min_cost[v] > min_cost[u] + c) {
                    min_cost[v] = min_cost[u] + c;
                    Q.push(PCI(min_cost[v], v));
                }
            }
        }
    }
};

#define INF (1000000000)

int main() {
    ShortestPath<int, INF> solver(5);
    solver.add_edge(1, 2, 3);
    solver.add_edge(2, 1, 3);
    solver.add_edge(1, 3, 1);
    solver.add_edge(3, 1, 1);
    solver.add_edge(3, 2, 1);
    solver.add_edge(2, 3, 1);
    solver.add_edge(4, 2, 2);
    solver.add_edge(2, 4, 2);
    assert(solver.dijkstra(1, 4) == 4);
    assert(solver.dijkstra(1, 5) == INF);
    solver.add_edge(4, 5, 1);
    solver.add_edge(5, 4, 1);
    assert(solver.dijkstra(1, 5) == 5);
    solver.add_edge(1, 5, 2);
    solver.add_edge(5, 1, 2);
    assert(solver.dijkstra(1, 5) == 2);
    vector<int> dist = solver.dijkstra(5);
    assert(dist[1] == 2);
    assert(dist[2] == 3);
    assert(dist[3] == 3);
    assert(dist[4] == 1);
    assert(dist[5] == 0);
    return 0;
}
