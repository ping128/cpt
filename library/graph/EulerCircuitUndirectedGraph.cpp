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
///////////////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;
#define SZ(x) ((int)(x).size())
#define REP(i,n) for(int i=0;(i)<(n);++(i))

class EulerCircuitUndirectedGraph {
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
    EulerCircuitUndirectedGraph() { N = 0; }
    EulerCircuitUndirectedGraph(int n) : N(n) { deg = VI(N); }
    // Add an edge between u and v
    void add_edge(int u, int v) { edges.push_back(Edge(u, v)), deg[u]++, deg[v]++;}
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
    int N = 3;
    {
        EulerCircuitUndirectedGraph euler(N);
        euler.add_edge(0, 1);
        euler.add_edge(1, 2);
        euler.add_edge(2, 0);
        assert(SZ(euler.get_euler_circuit(0)) == 4);
    }
    EulerCircuitUndirectedGraph euler(N);
    euler.add_edge(0, 1);
    euler.add_edge(1, 2);

    int start = -1;
    int end = -1;
    REP(i, N) {
        if (euler.deg[i] % 2 == 1) {
            if (start == -1) {
                start = i;
            } else if (end == -1) {
                end = i;
            } else {
                cout << -1 << endl;
                return 0;
            }
        }
    }
    vector<int> circuit;
    if (start != -1) {
        // Case 1: euler path
        euler.add_edge(start, end);
        circuit = euler.get_euler_circuit(start);
    } else {
        // Case 2: euler circuit
        circuit = euler.get_euler_circuit(0);
    }
    if (start != -1) {
        // Print euler path
        circuit.pop_back();
        REP(i, SZ(circuit)) {
            int a = circuit[i];
            int b = circuit[(i + 1) % SZ(circuit)];
            if ((a == start && b == end) || (a == end && b == start)) {
                REP(j, SZ(circuit)) {
                    cout << circuit[(i + j + 1) % SZ(circuit)] << " ";
                }
                break;
            }
        }
    } else {
        REP(i, SZ(circuit)) {
            cout << circuit[i] << " ";
        }
    }
    cout << endl;
    return 0;
}
