
////////////////////////
//  Dinic algorithm
////////////////////////

// Adjacency list implementation of Dinic's blocking flow algorithm.
// This is very fast in practice, and only loses to push-relabel flow.
//
// Running time:
//     O(|V|^2 |E|)
//
// INPUT: 
//     - graph, constructed using AddEdge()
//     - source
//     - sink
//
// OUTPUT:
//     - maximum flow value
//     - To obtain the actual flow values, look at all edges with
//       capacity > 0 (zero capacity edges are residual edges).

#include <cmath>
#include <vector>
#include <iostream>
#include <queue>
#include <cassert>

using namespace std;

const int inf_flow = 2000000000;
class Dinic {
public:
    struct Edge {
        int from, to, cap, flow, index;
        Edge(int from_, int to_, int cap_, int flow_, int index_) :
            from(from_), to(to_), cap(cap_), flow(flow_), index(index_) {}
    };

    int N;
    vector<vector<Edge> > G;
    vector<Edge *> dad;
    vector<int> Q;
  
    Dinic(int n) : N(n), G(n), dad(n), Q(n) {}
  
    void add_edge(int from, int to, int cap) {
        G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
        if (from == to) G[from].back().index++;
        G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
    }

    void reset_flow() {
        for (int i = 0; i < N; i++) {
            int sz = G[i].size();
            for (int j = 0; j < sz; j++) {
                G[i][j].flow = 0;
            }
        }
    }
    long long get_maxflow(int s, int t) {
        long long totflow = 0;
        while (long long flow = blocking_flow(s, t))
            totflow += flow;
        return totflow;
    }
private:
    long long blocking_flow(int s, int t) {
        fill(dad.begin(), dad.end(), (Edge *) NULL);
        dad[s] = &G[0][0] - 1;
    
        int head = 0, tail = 0;
        Q[tail++] = s;
        while (head < tail) {
            int x = Q[head++];
            int sz = G[x].size();
            for (int i = 0; i < sz; i++) {
                Edge &e = G[x][i];
                if (!dad[e.to] && e.cap - e.flow > 0) {
                    dad[e.to] = &G[x][i];
                    Q[tail++] = e.to;
                }
            }
        }
        if (!dad[t]) return 0;

        long long totflow = 0;
        int sz = G[t].size();
        for (int i = 0; i < sz; i++) {
            Edge *start = &G[G[t][i].to][G[t][i].index];
            int amt = inf_flow;
            for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
                if (!e) { amt = 0; break; }
                amt = min(amt, e->cap - e->flow);
            }
            if (amt == 0) continue;
            for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
                e->flow += amt;
                G[e->to][e->index].flow -= amt;
            }
            totflow += amt;
        }
        return totflow;
    }
};

int main() {
    Dinic mf(3);
    mf.add_edge(0, 1, 2);
    mf.add_edge(1, 2, 2);
    assert(mf.get_maxflow(0, 2) == 2);

    mf = Dinic(4);
    mf.add_edge(0, 1, 2);
    mf.add_edge(0, 2, 2);
    mf.add_edge(1, 3, 1);
    mf.add_edge(1, 2, 1);
    mf.add_edge(2, 3, 2);
    assert(mf.get_maxflow(0, 3) == 3);
    return 0;
}
