/////////////////////////////////////////////////////////
//  Centroid Decomposition
//  Link: http://codeforces.com/contest/321/problem/C
/////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


class CentroidDecomposition {
    typedef vector<int> VI;
    typedef vector<VI> VVI;
private:
    vector<bool> removed;
    VI tree_size;
public:
    int N;
    int cd_root;
    VI cd_depths, cd_parents;
    // Vertices are 0-based indexed.
    CentroidDecomposition (VVI &graph) {
        N = graph.size(); cd_depths.resize(N); removed.resize(N);
        tree_size.resize(N); cd_parents.resize(N);
        cd_root = decompose(0, 0, graph); cd_parents[cd_root] = -1;
    }
private:
    // Returns the centroid of the current subtree
    int decompose(int at, int dep, VVI &adj) {
        int center = find_center(at, adj);
        cd_depths[center] = dep; removed[center] = true;
        int sz = adj[center].size();
        for (int i = 0; i < sz; i++) {
            int v = adj[center][i];
            if (!removed[v]) {
                int subroot = decompose(v, dep + 1, adj);
                cd_parents[subroot] = center;
            }
        }
        return center;
    }

    // Center node is the node whose all subtrees' size is not greater than half of the whole tree
    int find_center(int at, VVI &adj) {
        int par = -1; cal_size(at, -1, adj);
        int limit = tree_size[at] / 2;
        while (1) {
            bool found = true; int sz = adj[at].size();
            for (int i = 0; i < sz; i++) {
                int v = adj[at][i];
                if (!removed[v] && v != par && tree_size[v] > limit) {
                    found = false; par = at; at = v; break;
                }
            }
            if (found) return at;
        }
    }

    void cal_size(int at, int par, VVI &adj) {
        tree_size[at] = 1; int sz = adj[at].size();
        for (int i = 0; i < sz; i++) {
            int v = adj[at][i];
            if (!removed[v] && par != v) {
                cal_size(v, at, adj); tree_size[at] += tree_size[v];
            }
        }
    }
};

typedef vector<int> VI;
typedef vector<VI> VVI;

int main(){
    int N;
    VVI adj;
    int u, v;
    scanf("%d", &N);
    adj.resize(N);
    for(int i = 0; i < N - 1; i++ ){
        scanf("%d %d", &u, &v); u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    CentroidDecomposition cd(adj);
    for (int i = 0; i < N; i++) {
        printf("%c ", cd.cd_depths[i] + 'A');
    }
    return 0;
}
