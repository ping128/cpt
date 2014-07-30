///////////////////////////////////////////////////////////////////////////////////////////
//  Maximum Bipartite Matching on undirected graph
///////////////////////////////////////////////////////////////////////////////////////////

#include <vector>
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
    // Vertices are 0-based indexed.
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

int main() {
    MaximumBipartiteMatching solver(7);
    solver.add_edge(0, 3);
    solver.add_edge(3, 0);
    solver.add_edge(1, 4);
    solver.add_edge(4, 1);
    solver.add_edge(2, 4);
    solver.add_edge(4, 2);
    assert(solver.maximum_matching() == 2);
    solver.add_edge(2, 5);
    solver.add_edge(5, 2);
    assert(solver.maximum_matching() == 3);
    return 0;
}
        
    

