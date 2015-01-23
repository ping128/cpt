///////////////////////////////////////////////////////////////////////
//  Lowest Common Ancestor (LCA)
//  O(lgn) per query
//  Link: http://www.spoj.com/status/LCA/
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class LCA {
public:
    typedef vector<int> VI;
    typedef vector<VI> VVI;
    VI depths;
    VVI parents;
    int N, root, max_lvl;
    // Vertices are 0-based indexed.
    LCA (int root_, VVI graph) {
        root = root_; N = graph.size(); depths.resize(N);
        max_lvl = 1; while ((1<<max_lvl) < N) max_lvl++;
        parents.resize(max_lvl, VI(N));
        build_lca(root, -1, graph, 0);
        for (int i = 1; i < max_lvl; i++)
            for (int j = 0; j < N; j++)
                if (parents[i - 1][j] != -1)
                    parents[i][j] = parents[i - 1][parents[i - 1][j]];
    }
    int lca(int u, int v) {
        if (depths[u] < depths[v]) swap(u, v);
        int diff_dep = depths[u] - depths[v];
        for (int i = 0; i < max_lvl; i++)
            if ((1<<i) & diff_dep)
                u = parents[i][u];
        if (u == v) return u;
        for (int i = max_lvl - 1; i >= 0; i--) {
            if (parents[i][u] != parents[i][v]) {
                u = parents[i][u];
                v = parents[i][v];
            }
        }
        return parents[0][u];
    }
    int get_depth(int u) { return depths[u]; }
private:
    void build_lca(int at, int par, VVI &adj, int dep) {
        int sz = adj[at].size();
        depths[at] = dep;
        parents[0][at] = par;
        for (int i = 1; i < max_lvl; i++) parents[i][at] = -1;
        for (int i = 0; i < sz; i++) {
            if (adj[at][i] != par) build_lca(adj[at][i], at, adj, dep + 1);
        }
    }
};

typedef vector<int> VI;
typedef vector<VI> VVI;
#define MAXN 1005

int main() {
    int test;
    scanf("%d", &test);
    for (int tt = 0; tt < test; tt++) {
        printf("Case %d:\n", tt + 1);
        int N;
        scanf("%d", &N);
        VI parents (N, -1);
        VVI graph (N);
        for (int i = 0; i < N; i++) {
            int sz;
            scanf("%d", &sz);
            int v;
            for (int j = 0; j < sz; j++) {
                scanf("%d", &v); v--;
                parents[v] = i;
                graph[i].push_back(v);
                graph[v].push_back(i);
            }
        }
        int root = 0;
        while (parents[root] != -1) root = parents[root];
        LCA solver(root, graph);
        int Q;
        scanf("%d", &Q);
        int u, v;
        for (int i = 0; i < Q; i++) {
            scanf("%d %d", &u, &v); u--, v--;
            printf("%d\n", solver.lca(u, v) + 1);
        }
    }
    return 0;
}
