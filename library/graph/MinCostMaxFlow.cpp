////////////////////////////////////////////////////////////////////
//  Minimum Cost Maximum Flow Algorithm
//  Link: http://codeforces.com/contest/362/problem/E
////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

using namespace std;

#define inf_cost (1000000000)
template<class Flow_t, class Cost_t>
class MinCostMaxFlow {
    typedef pair<Cost_t, int> PCI;
public:
    struct Edge {
        int to, rev;
        Flow_t capacity;
        Cost_t cost;
    };
    vector<vector<Edge> > edges;
    MinCostMaxFlow (int n) { // Vertices are 0-based indexed.
        edges.assign(n, vector<Edge>());
    }
    void add_edge (int from, int to, Flow_t cap, Cost_t cost) {
        Edge f; f.to = to, f.rev = (int)edges[to].size(), f.capacity = cap, f.cost = cost;
        Edge b; b.to = from, b.rev = (int)edges[from].size(), b.capacity = 0, b.cost = -cost;
        edges[from].push_back(f), edges[to].push_back(b);
    }
    // Using SPFA (Shortest Path Faster Algor) for negative cost
    // Using Dijkstra for non-negative cost
    // f is the require amount of flow
    pair<Flow_t, Cost_t> get_min_cost_max_flow (int s, int t, Flow_t f = inf_cost, bool useSPFA = false) {
        int n = edges.size();
        vector<Cost_t> dist(n); vector<int> prev(n), prev_edge(n);
        pair<Flow_t, Cost_t> res = make_pair(0, 0);
        vector<Cost_t> potential(n);
        while (f > 0) {
            fill(dist.begin(), dist.end(), inf_cost);
            if (useSPFA || res.first == 0) {
                queue<int> Q;
                Q.push(s); dist[s] = 0; vector<bool> inqueue(n);
                while (!Q.empty()) {
                    int u = Q.front(); Q.pop();
                    inqueue[u] = false;
                    for (int i = 0; i < (int)edges[u].size(); i++) {
                        int v = edges[u][i].to;
                        if (edges[u][i].capacity > 0 && dist[u] + edges[u][i].cost < dist[v]) {
                            if (!inqueue[v]) {
                                inqueue[v] = true;
                                Q.push(v);
                            }
                            dist[v] = dist[u] + edges[u][i].cost;
                            prev[v] = u; prev_edge[v] = i;
                        }
                    }
                            
                }
            } else {
                priority_queue<PCI, vector<PCI>, greater<PCI> > Q;
                Q.push(PCI(0, s)); dist[s] = 0; vector<bool> visited(n);
                while (!Q.empty()) {
                    int u = Q.top().second; Q.pop();
                    if (visited[u]) continue;
                    visited[u] = true;
                    for (int i = 0; i < (int)edges[u].size(); i++) {
                        int v = edges[u][i].to;
                        if (edges[u][i].capacity > 0) {
                            Cost_t d = dist[u] + edges[u][i].cost + potential[u] - potential[v];
                            if (dist[v] > d) {
                                dist[v] = d;
                                prev[v] = u; prev_edge[v] = i;
                                Q.push(PCI(d, v));
                            }
                        }
                    }
                }
            }
            if (dist[t] == inf_cost) break;
            if (!useSPFA) for (int i = 0; i < n; i++) potential[i] += dist[i];
            Flow_t min_f = f; Cost_t c = 0;
            for (int v = t; v != s; v = prev[v]) {
                int u = prev[v], eid = prev_edge[v];
                min_f = min(min_f, edges[u][eid].capacity);
                c += edges[u][eid].cost;
            }
            f -= min_f; res.first += min_f; res.second += min_f * c;
            for (int v = t; v != s; v = prev[v]) {
                int u = prev[v], eid = prev_edge[v];
                edges[u][eid].capacity -= min_f;
                edges[edges[u][eid].to][edges[u][eid].rev].capacity += min_f;
            }
        }
        return res;
    }
};

#define INF (1000000000)
#define MAXN 55
int caps[MAXN][MAXN];

int main(){
    int N, K;
    scanf("%d %d", &N, &K);

    for(int i = 0; i < N; i++ )
        for(int j = 0; j < N; j++ )
            scanf("%d", &caps[i][j]);

    int left = 0, right = 1000000000;
    while(left <= right){
        int mid = (left + right) / 2;
        MinCostMaxFlow<int, long long> mcf(N);
        for(int i = 0; i < N; i++ )
            for(int j = 0; j < N; j++ ){
                if(caps[i][j] == 0) continue;
                mcf.add_edge(i, j, caps[i][j], 0);
                mcf.add_edge(i, j, INF, 1);
            }

        pair<int, long long> p = mcf.get_min_cost_max_flow(0, N - 1, mid);
        if (p.first != mid || p.second > K) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    cout << right << endl;
    return 0;
}
