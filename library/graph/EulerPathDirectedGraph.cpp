///////////////////////////////////////////////////////////////////////////////
//  Euler Path (directed graph)
//
///////////////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

using namespace std;

#define SZ(x) ((int)((x).size()))
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

class EulerPathDirectedGraph {
public:
    int N; // zero-based indexed.
    vector<vector<int>> adj;
    EulerPathDirectedGraph (int n) : N(n) { adj.resize(n); }
    void add_edge(int u, int v) { adj[u].push_back(v); }
    // Return an empty vector if no euler path exists.
    vector<int> get_euler_path() {
        vector<int> outdeg(N), indeg(N);
        REP(u, N) for (int v : adj[u]) { outdeg[u]++, indeg[v]++; }
        int num_plus = 0, num_minus = 0;
        int start = -1;
        bool good = true;
        REP(i, N) {
            if (abs(indeg[i] - outdeg[i]) > 1) {
                good = false;
            } else if (abs(indeg[i] - outdeg[i]) == 1) {
                if (outdeg[i] > indeg[i]) start = i, num_plus++;
                else num_minus++;
            }
        }
        if (start == -1) REP(i, N) if (outdeg[i]) { start = i; break; } // also have an euler circuit
        if (good && ((num_plus == 1 && num_minus == 1) || (num_plus == 0 && num_minus == 0))) {
            vector<int> cur_i(N), ans;
            stack<int> st;
            st.push(start);
            vector<bool> visited(N);
            while (!st.empty()) {
                int u = st.top();
                visited[u] = true;
                if (cur_i[u] == SZ(adj[u])) {
                    ans.push_back(u);
                    st.pop();
                } else {
                    int v = adj[u][cur_i[u]];
                    st.push(v);
                    cur_i[u]++;
                }
            }
            reverse(ans.begin(), ans.end());
            REP(i, N) { if (!visited[i]) { return vector<int>(); } }
            return ans;
        } else {
            return vector<int>();
        }
    }
};

int main() {
    EulerPathDirectedGraph solver(4);
    solver.add_edge(0, 1);
    solver.add_edge(1, 2);
    solver.add_edge(2, 1);
    solver.add_edge(1, 0);
    solver.add_edge(0, 2);
    solver.add_edge(2, 3);
    vector<int> ans = solver.get_euler_path();
    for (int u : ans) {
        cout << u << endl;
    }
    return 0;
}
