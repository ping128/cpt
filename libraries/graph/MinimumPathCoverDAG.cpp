///////////////////////////////////////////////////////////////////////////////////////////
//  Minimum Path Cover on DAG (the minimum number of paths we need so that all nodes are
//  part of exactly one path)
//
//  Notice: From the DAG with n points, we make a bipartite graph with n points on each side.
//  For each edge (u, v) in the DAG, we draw an edge between u on the left side and v on
//  the right side in the bipartite graph.
//  Then, the number of minimum path cover is just n - #max_matchings
///////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

class MaximumBipartiteMatching {
public:
    int N;
    vector<vector<int> > adj;
    vector<int> pairs; // stores the previous node on the path
    int mark;
    vector<int> seen;
    MaximumBipartiteMatching() { N = 0; }
    MaximumBipartiteMatching(int n) : N(n) { adj.resize(N); pairs.resize(N); seen.resize(N); mark = 0; }
    void add_edge(int u, int v) { adj[u].push_back(v); }
    // Requires the graph to be bipartite indirected.
    int maximum_matching() {
        assert(N > 0);
        int matching = 0;
        fill(pairs.begin(), pairs.end(), -1);
        for (int i = 0; i < N; i++) { mark++; matching += dfs(i); }
        return matching / 2; // return the number of pairs
    }
private:
    bool dfs(int at){
        if (seen[at] == mark) return false;
        seen[at] = mark;
        int ss = adj[at].size();
        for(int i = 0; i < ss; i++ ){
            int v = adj[at][i];
            if(pairs[v] == -1 || dfs(pairs[v])){
                pairs[v] = at;
                return true;
            }
        }
        return false;
    }
};

class MinimumPathCoverDAG {
public:
    int N;
    vector<vector<int> > adj;
    MaximumBipartiteMatching solver;
    // Vertices are 0-based indexed.
    MinimumPathCoverDAG(int n) : N(n) { adj.resize(N); }
    void add_edge(int u, int v) { adj[u].push_back(v); }
    int minimum_path_cover() {
        solver = MaximumBipartiteMatching(2 * N);
        for (int i = 0; i < N; i++) {
            int sz = adj[i].size();
            for (int j = 0; j < sz; j++) {
                solver.add_edge(i, adj[i][j] + N);
                solver.add_edge(adj[i][j] + N, i);
            }
        }
        return N - solver.maximum_matching();
    }
    void print_path() {
        vector<bool> seen (N);
        for (int i = 0; i < N; i++) {
            if (!seen[i]) { seen[i] = true; int now = i; bool found = true;
                printf("%d", i);
                while (found) { found = false;
                    for (int j = N; j < 2 * N; j++) {
                        if (solver.pairs[j] == now) {
                            now = j - N; seen[now] = true; found = true; break;
                        }
                    }
                    if (found) printf(" -> %d", now);
                }
                printf("\n");
            }
        }
    }
};

int main() {
    MinimumPathCoverDAG solver(7);
    solver.add_edge(0, 1);
    solver.add_edge(3, 1);
    solver.add_edge(1, 6);
    solver.add_edge(4, 6);
    solver.add_edge(6, 2);
    solver.add_edge(6, 5);
    assert(solver.minimum_path_cover() == 4);
    solver.print_path();
    solver.add_edge(3, 2);
    assert(solver.minimum_path_cover() == 3);
    printf("---------\n");
    solver.print_path();
    return 0;
}
