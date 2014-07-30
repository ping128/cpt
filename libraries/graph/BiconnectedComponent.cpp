///////////////////////////////////////////////////////////////////////////////
//  Biconnected Component, Articulation points, Bridges
//  Link: http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=3785
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <cstdio>
#include <iostream>

using namespace std;

class BiconnectedComponent {
public:
    int N, M;
    vector<vector<int> > adj;
    vector<int> bicomponents; // biconnected component
    vector<int> bridges; // component whose all pairs are connected by exactly one simple path
    vector<int> dfs_numbers;
    vector<int> low_numbers;
    vector<bool> is_articulation_point;
    int dfs_number;
    
    // Vertices are 1-based
    BiconnectedComponent(int n) : N(n) {
        M = 0; adj.resize(N + 1); bicomponents.resize(N + 1); bridges.resize(N + 1);
        dfs_numbers.resize(N + 1); low_numbers.resize(N + 1);
        is_articulation_point.resize(N + 1); dfs_number = 0;
    }

    int find_bicomponent(int x) { return find_component(bicomponents, x); }
    int find_bridge(int x) { return find_component(bridges, x); }
    void add_edge(int u, int v) { adj[u].push_back(v); }
    void cal_components() {
        for (int i = 1; i <= N; i++) { bicomponents[i] = i; bridges[i] = i; }
        for (int i = 1; i <= N; i++) {
            if (dfs_numbers[i] == 0) cal_components(i, -1);
        }
    }
    bool is_articulation(int x) { return is_articulation_point[x]; }

private:
    int find_component(vector<int> &com, int x) {
        if (com[x] == x) return x;
        return com[x] = find_component(com, com[x]);
    }

    void union_set(vector<int> &com, int x, int y) {
        int a = find_component(com, x);
        int b = find_component(com, y);
        com[a] = b;
    }

    void cal_components(int at, int parent) {
        dfs_numbers[at] = low_numbers[at] = ++dfs_number;
        int sz = adj[at].size();
        for (int i = 0; i < sz; i++) {
            int v = adj[at][i];
            if (!dfs_numbers[v]) {
                cal_components(v, at);
                low_numbers[at] = min(low_numbers[at], low_numbers[v]);
                if (low_numbers[v] > dfs_numbers[at]) {
                    // at is an articulation point separating v
                    if (at > 1) is_articulation_point[at] = true;
                    union_set(bridges, v, at);
                } else {
                    union_set(bicomponents, v, at);
                }
            } else if (v != parent) {
                low_numbers[at] = min(low_numbers[at], dfs_numbers[v]);
            }
        }
    }
};

int main() {
    while (1) {
        int N, M, Q;
        scanf("%d %d %d", &N, &M, &Q);
        if (N + M + Q == 0) break;
        int u, v;
        BiconnectedComponent solver(N);
        for (int i = 0; i < M; i++) {
            scanf("%d %d", &u, &v);
            solver.add_edge(u, v);
            solver.add_edge(v, u);
        }
        
        solver.cal_components();
        for (int q = 0; q < Q; q++) {
            int start, finish;
            scanf("%d %d", &start, &finish);
            int a = solver.find_bridge(start);
            int b = solver.find_bridge(finish);
            if (a == b) printf("Y\n");
            else printf("N\n");
        }
        printf("-\n");
    }
    return 0;
}
