///////////////////////////////////////////////////////////////////////////////
//  Euler Circuit (undirected graph)
//  Considering the number of odd-degree nodes
//     if 0, there is an euler circuit
//        2, there is an euler path
//     otherwise, neither of them exists
//  To find an euler path
//     - Add one edge between the two odd-degree nodes,
//     - Find an euler circuit and then remove that edge later
//
//  Implementation of Euler Circuit
//    procedure FindEuler (u)
//      Iterate over all edges (u, v):
//        remove edge(u, v) from the graph
//        FindEuler(v)
//      add vertex u in the result
//  Link: http://www.spoj.com/KSTN/problems/EULER/
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <algorithm>
#include <cassert>
#include <stack>
#include <iostream>

using namespace std;

class EulerCircuit {
public:
    typedef vector<int> VI;
    typedef pair<int, int> PII; // (v, id)
    typedef vector<vector<PII> > VVP;
    typedef struct edge_st {
        int u, v;
        edge_st () {}
        edge_st(int u_, int v_) : u(u_), v(v_) {}
    } Edge;
    int N;
    vector<Edge> edges;
    VI deg; // need to run get_euler_circuit first
    // Vertices are 0-based indexed.
    EulerCircuit() { N = 0; }
    EulerCircuit(int n) : N(n) {}
    // Add an edge between u and v
    void add_edge(int u, int v) { edges.push_back(Edge(u, v)); }
    // Return an empty list if no an euler circuit
    VI get_euler_circuit(int start) {
        if (N < 1) return VI();
        int M = edges.size(); VVP adj (N); VI empty_ret, ret;
        if (M == 0) {
            if (N > 1) { return empty_ret; } else { ret.push_back(0); return ret; }
        }
        vector<bool> removed (M);
        for (int i = 0; i < M; i++) {
            adj[edges[i].u].push_back(PII(edges[i].v, i));
            adj[edges[i].v].push_back(PII(edges[i].u, i));
        }
        deg = VI (N);
        for (int i = 0; i < N; i++) { int sz = adj[i].size();
            for (int j = 0; j < sz; j++) {
                deg[adj[i][j].first]++;
            }
        }
        for (int i = 0; i < N; i++) { if (deg[i] % 2) return empty_ret; }
        stack<int> st; VI cur_pt(N);
        st.push(start);
        while (!st.empty()) {
            int u = st.top(), v = -1, id = 0;
            int sz = adj[u].size();
            while (cur_pt[u] < sz && v == -1) {
                if (!removed[adj[u][cur_pt[u]].second]) {
                    v = adj[u][cur_pt[u]].first;
                    id = adj[u][cur_pt[u]].second;
                }
                cur_pt[u]++;
            }

            if (v == -1) { ret.push_back(u); st.pop(); }
            else { removed[id] = true; st.push(v); }
        }
        for (int i = 0; i < M; i++) { if (!removed[i]) return empty_ret; }
        return ret;
    }
};

int main() {
    int N, M;
    int u, v;
    scanf("%d %d", &N, &M);
    EulerCircuit solver(N);
    for (int i = 0; i < M; i++) {
        scanf("%d %d", &u, &v);
        u--, v--;
        solver.add_edge(u, v);
    }
    vector<int> res = solver.get_euler_circuit(0);
    int sz = res.size();

    if (sz) {
        // Print an euler curcuit
        printf("1");
        for (int i = 1; i < sz; i++) {
            printf(" %d", res[i] + 1);
        }
        printf("\n");
    } else {
        bool bad = false;
        u = -1, v = -1;
        for (int i = 0; i < N; i++) {
            if (solver.deg[i] % 2) {
                if (u == -1) u = i;
                else if (v == -1) v = i;
                else bad = true; // No euler path as well.
            }
        }

        if (bad) {
            // Neither euler path nor euler curcuit exists.
        } else {
            // Print euler path.
            solver.add_edge(u, v);
            res = solver.get_euler_circuit(0);
            sz = res.size();
            sz--;
            for (int i = 0; i < sz; i++) {
                int a = res[i];
                int b = res[(i + 1) % sz];
                if ((a == u && b == v) || (a == v && b == u)) {
                    printf("%d", b + 1);
                    for (int j = 0; j < sz - 1; j++) {
                        printf(" %d", res[(i + j + 2) % sz] + 1);
                    }
                    printf("\n");
                    break;
                }
            }
        }
    }
    return 0;
}

/* tests
int main() {
    EulerCircuit solver;
    assert(solver.get_euler_circuit(0).size() == 0);
    solver = EulerCircuit(2);
    solver.add_edge(0, 1);
    assert(solver.get_euler_circuit(0).size() == 0);
    solver = EulerCircuit(1);
    assert(solver.get_euler_circuit(0).size() == 1);
    solver.add_edge(0, 0);
    solver.add_edge(0, 0);
    vector<int> res = solver.get_euler_circuit(0);
    assert(res.size() > 0);
    for (int i = 0; i < (int)res.size(); i++) printf("%d ", res[i]); printf("\n");
    solver = EulerCircuit(6);
    solver.add_edge(0, 1);
    solver.add_edge(0, 2);
    solver.add_edge(2, 1);
    solver.add_edge(0, 3);
    solver.add_edge(3, 4);
    solver.add_edge(4, 5);
    solver.add_edge(5, 3);
    assert(solver.get_euler_circuit(0).size() == 0);
    solver.add_edge(0, 3);
    res = solver.get_euler_circuit(0);
    assert(res.size() > 0);
    for (int i = 0; i < (int)res.size(); i++) printf("%d ", res[i]); printf("\n");
    res = solver.get_euler_circuit(1);
    assert(res.size() > 0);
    for (int i = 0; i < (int)res.size(); i++) printf("%d ", res[i]); printf("\n");
    return 0;
}
*/
