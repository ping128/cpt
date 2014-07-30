///////////////////////////////////////////
//  Minimum Cost Flow Algorithm (example)
///////////////////////////////////////////

// Implementation of min cost max flow algorithm using adjacency
// matrix (Edmonds and Karp 1972).  This implementation keeps track of
// forward and reverse edges separately (so you can set cap[i][j] !=
// cap[j][i]).  For a regular max flow, set all edge costs to 0.
//
// Running time, O(|V|^2) cost per augmentation
//     max flow:           O(|V|^3) augmentations
//     min cost max flow:  O(|V|^4 * MAX_EDGE_COST) augmentations
//     
// INPUT: 
//     - graph, constructed using AddEdge()
//     - source
//     - sink
//
// OUTPUT:
//     - (maximum flow value, minimum cost value)
//     - To obtain the actual flow, look at positive values only.

#include <cmath>
#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

#define inf_cost (1000000000)
template<class L>
class MinCostFlow {
public:
    typedef vector <int> VI;
    typedef vector <VI> VVI;
    typedef vector <L> VL;
    typedef vector <VL> VVL;
    typedef pair <int, int> PII;
    typedef vector <PII> VPII;
    int N;
    VVL cap, flow, cost;
    VI found;
    VL dist, pi, width;
    VPII dad;
    // Vertices are 0-based indexed.
    MinCostFlow(int n):
        N(n), cap(n, VL(n)), flow(n, VL(n)), cost(N, VL(n)),
        found(n), dist(n), pi(n), width(n), dad(n) {}
	
    void add_edge(int from, int to, L cap_, L cost_){
        this->cap[from][to] = cap_;
        this->cost[from][to] = cost_;
    }
    
    // Return total max flow, total min cost
    // TOTAL COST = sum of cost per each unit of flow
    pair<L, L> get_maxflow(int s, int t){
        L totflow = 0, totcost = 0;
        while (L amt = dijkstra(s, t)) {
            totflow += amt;
            for (int x = t; x != s; x = dad[x].first) {
                if (dad[x].second == 1) {
                    flow[dad[x].first][x] += amt;
                    totcost += amt * cost[dad[x].first][x];
                } else {
                    flow[x][dad[x].first] -= amt;
                    totcost -= amt * cost[x][dad[x].first];
                }
            }
        }
        return make_pair(totflow, totcost);
    }
private:	
    void relax(int s, int k, L cap_, L cost_, int dir){
        L val = dist[s] + pi[s] - pi[k] + cost_;
        if (cap_ && val < dist[k]){
            dist[k] = val;
            dad[k] = make_pair(s, dir);
            width[k] = min(cap_, width[s]);
        }
    }
	
    L dijkstra(int s, int t){
        fill(found.begin(), found.end(), false);
        fill(dist.begin(), dist.end(), inf_cost);
        fill(width.begin(), width.end(), 0);
        dist[s] = 0;
        width[s] = inf_cost;
        while (s != -1){
            int best = -1;
            found[s] = true;
            for (int k = 0; k < N; k++){
                if (found[k])
                    continue;
                relax(s, k, cap[s][k] - flow[s][k], cost[s][k], 1);
                relax(s, k, flow[k][s], -cost[k][s], -1);
                if (best == -1 || dist[k] < dist[best])
                    best = k;
            }
            s = best;
        }
        for (int k = 0; k < N; k++)
            pi[k] = min(pi[k] + dist[k], inf_cost);
        return width[t];
    }
};

int main() {
    MinCostFlow<int> mcf(3);
    mcf.add_edge(0, 1, 2, 2);
    mcf.add_edge(1, 2, 2, 5);
    pair<int, int> res = mcf.get_maxflow(0, 2);
    assert(res == make_pair(2, 14));

    mcf = MinCostFlow<int>(4);
    mcf.add_edge(0, 1, 2, 2);
    mcf.add_edge(0, 2, 2, 10);
    mcf.add_edge(1, 3, 1, 2);
    mcf.add_edge(1, 2, 1, 2);
    mcf.add_edge(2, 3, 2, 2);
    res = mcf.get_maxflow(0, 3);
    assert(res == make_pair(3, 22));
    return 0;
}
