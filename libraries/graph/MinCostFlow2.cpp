//////////////////////////////////////////////////////////////////////////////////////
//  Minimum Cost Flow Algorithm (with required flow)
//////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define inf_cost (1000000000000.0)
template<class Flow_t, class Cost_t>
class MinCostFlow {
public:
    /* Min Cost Flow with a required flow (can't be changed to normal by
     * a little modification
     * using SPFA (Shortest Path Faster Algor) in case the cost is negative
     * so if the cost is always positive, it can be replaced by Dijkstra
     */
    struct Edge {
        int v, rev;
        Flow_t cap;
        Cost_t cost;
    };

    vector<bool> visited;
    vector<vector<Edge> > edges;
    vector<pair<int, int> > prev; // for tracing back the path getting from the shortest path, prev[v] : (u, index of u in edges[v])
    vector<Cost_t> dist;
    int N;
    // Vertices are 0-based indexed.    
    MinCostFlow(int n) {
        N = n; visited.resize(N);
        edges.resize(N); prev.resize(N);
        dist.resize(N);
    }

    void add_edge(int u, int v, Flow_t capacity, Cost_t cost){
        Edge e1 = {v, edges[v].size(), capacity, cost};
        Edge e2 = {u, edges[u].size(), 0, -cost};
        edges[u].push_back(e1);
        edges[v].push_back(e2);
    }

    // return the minimum cost for getting that required_flow
    // return inf_cost if maxflow < required_flow
    Cost_t get_mincost(int s, int t, Flow_t required_flow){
        Cost_t totalCost = 0;
        while(required_flow > 0){
            Flow_t flow = required_flow;
            if(!spfa(s, t)){ // can't reach
                break;
            }

            for(int i = t; i != s; i = prev[i].first)
                flow = min(flow, edges[prev[i].first][prev[i].second].cap);

            if(flow == 0) break;
            required_flow -= flow;
            for(int i = t; i != s; i = prev[i].first){
                edges[prev[i].first][prev[i].second].cap -= flow;
                edges[i][edges[prev[i].first][prev[i].second].rev].cap += flow;
            }
            totalCost += flow * dist[t];
        }
        return required_flow == 0 ? totalCost : inf_cost;
    }

private:
    bool spfa(int s, int t){
        for(int i = 0; i < N; i++ ) dist[i] = inf_cost;
        dist[s] = 0;
        queue<int> Q;
        Q.push(s);
        visited[s] = true;
        while(!Q.empty()){
            int u = Q.front();
            Q.pop();
            visited[u] = 0;
            for(int i = 0; i < edges[u].size(); i++ ){
                int v = edges[u][i].v;
                if(edges[u][i].cap > 0 && dist[v] > dist[u] + edges[u][i].cost){
                    dist[v] = dist[u] + edges[u][i].cost;
                    prev[v] = pair<int, int>(u, i);
                    if(!visited[v]){
                        visited[v] = true;
                        Q.push(v);
                    }
                }
            }
        }
        return dist[t] != inf_cost;
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
        MinCostFlow<int, long long> solver(N);
        for(int i = 0; i < N; i++ )
            for(int j = 0; j < N; j++ ){
                if(caps[i][j] == 0) continue;
                solver.add_edge(i, j, caps[i][j], 0);
                solver.add_edge(i, j, INF, 1);
            }

        long long cost = solver.get_mincost(0, N - 1, mid);
        if(cost <= K) left = mid + 1;
        else right = mid - 1;
    }

    cout << right << endl;
    return 0;
}

