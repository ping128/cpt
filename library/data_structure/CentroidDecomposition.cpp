/////////////////////////////////////////////////////////
//  Centroid Decomposition
//  Complexity: N log N
//  Link: http://codeforces.com/contest/321/problem/C
/////////////////////////////////////////////////////////

#include <bits/stdc++.h>

using namespace std;

#define SZ(x) ((int)((x).size()))
#define PB(x) push_back(x)
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

class CentroidDecomposition {
public:
    vector<bool> removed;
    int N;
    int cd_root;
    vector<int> cd_depths, cd_parents, tree_size;
    vector<vector<int>> adj;
    // Vertices are 0-based indexed.
    CentroidDecomposition (int n) : N(n) {
        adj.resize(N); cd_depths.resize(N); removed.resize(N);
        tree_size.resize(N); cd_parents.resize(N);
    }
    void add_edge(int u, int v) {
        adj[u].PB(v); adj[v].PB(u);
    }
    void decompose() {
        cd_root = decompose(0, 0);
        cd_parents[cd_root] = -1;
    }
private:
    // Returns the centroid of the current subtree
    int decompose(int at, int dep) {
        int center = find_center(at);
        cd_depths[center] = dep;
        removed[center] = true;
        for (int v : adj[center]) {
            if (!removed[v]) {
                int subroot = decompose(v, dep + 1);
                cd_parents[subroot] = center;
            }
        }
        return center;
    }
    // Center node is the node whose all subtrees' size is not greater than half of the whole tree
    int find_center(int at) {
        cal_size(at, -1);
        int limit = tree_size[at] / 2, par = -1;
        bool found = false;
        while (!found) {
            found = true;
            for (int v : adj[at]) {
                if (!removed[v] && v != par && tree_size[v] > limit) {
                    found = false;
                    par = at, at = v;
                    break;
                }
            }
        }
        return at;
    }
    void cal_size(int at, int par) {
        tree_size[at] = 1;
        for (int v : adj[at]) {
            if (v != par && !removed[v]) {
                cal_size(v, at);
                tree_size[at] += tree_size[v];
            }
        }
    }
};

int main(){
    int N;
    int u, v;
    scanf("%d", &N);
    CentroidDecomposition cd(N);
    for(int i = 0; i < N - 1; i++ ){
        scanf("%d %d", &u, &v); u--, v--;
        cd.add_edge(u, v);
    }
    cd.decompose();
    for (int i = 0; i < N; i++) {
        printf("%c ", cd.cd_depths[i] + 'A');
    }
    return 0;
}
