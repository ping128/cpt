///////////////////////////////////////////////////////////////////////////////
//  Strongly Connected Component
///////////////////////////////////////////////////////////////////////////////


#include <algorithm>
#include <cstring>

using namespace std;

class StronglyConnectedComponent {
    typedef vector<int> VI; typedef vector<VI> VVI;
public:
    struct edge{
        int e, nxt;
        edge() {}
        edge(int _e, int _nxt) : e(_e), nxt(_nxt) {}
    };
    int V, E;
    vector<edge> e, er;
    vector<int> sp, spr, group_num, stk;
    int group_cnt; vector<set<int>> graph; // reduced graph
    vector<bool> visited;
    // Vertices are 1-based indexed.
    StronglyConnectedComponent() {}
    StronglyConnectedComponent(int _V) {
        V = _V; E = 0; group_cnt = 0; group_num = vector<int>(V + 5);
        sp = vector<int>(V + 5); spr = vector<int>(V + 5); stk = vector<int>(V + 5);
        visited = vector<bool>(V + 5); e.push_back(edge()); er.push_back(edge());
    }
    void fill_forward(int x){
        visited[x] = true;
        for(int i = sp[x]; i; i = e[i].nxt) if (!visited[e[i].e]) fill_forward(e[i].e);
        stk[++stk[0]] = x;
    }
    void fill_backward(int x){
        visited[x] = false;
        group_num[x] = group_cnt;
        for(int i = spr[x]; i; i = er[i].nxt) if (visited[er[i].e]) fill_backward(er[i].e);
    }
    void add_edge(int v1, int v2){ // Add edge v1->v2
        E++;
        e.push_back(edge(v2, sp[v1])); sp[v1] = E;
        er.push_back(edge(v1, spr[v2])); spr[v2] = E;
    }
    void cal_scc() {
        stk[0] = 0;
        fill(visited.begin(), visited.end(), false);
        for(int i = 1; i <= V; i++) if (!visited[i]) fill_forward(i);
        group_cnt = 0;
        for(int i = stk[0]; i >= 1; i--) if (visited[stk[i]]) { group_cnt++; fill_backward(stk[i]); }
    }
    void cal_reduced_graph() { // need to call cal_scc() before
        graph = vector<set<int>>(group_cnt + 1);
        for (int i = 1; i <= V; i++) { int u = group_num[i];
            for (int j = sp[i]; j; j = e[j].nxt) {
                int v = group_num[e[j].e];
                // If we don't need to remove duplicates, use Vector instead of Set.
                if (u != v) graph[u].insert(v); 
            }
        }
    }
};

///////////
//  2-SAT
///////////

/*
0. V = 2 * N (N = the number of variables)
1. Add edges:(a || b) -> edge(~a, b), edge(~b, a)
2. SCC()
3. it's possible if a and ~a are in the difference components (different group_num[])
4. Ans: if group_num[a] > group_num[~a], assign a to True
*/
