/////////////////////////////////////////////////////
//  MaximumFlow by FordFulkerson (for sparse graph)
/////////////////////////////////////////////////////

#include <vector>
#include <cassert>

using namespace std;

const int inf_flow = 2000000000;
class FordFulkerson {
public:
    // runtime: O(E F), E = #edges, F = max flow values
    typedef struct edge {
        int v;
        int capacity; 
        int flow; // current amount allowed to flow
        int rev_index; // store its reversed edge's index
        edge(int v_, int c_, int f_, int rev) : v(v_), capacity(c_), flow(f_), rev_index(rev) {}
    } Edge;
    int N; // node from 0 to N - 1
    vector<vector<Edge> > maxFlowEdge;
    int mark;   // global variable for checking if a node is already visited
    vector<int> seen;  // status of each node
    FordFulkerson(int numNode){
        N = numNode;
        maxFlowEdge = vector<vector<Edge> >(numNode);
        seen = vector<int>(numNode);
        mark = 0;
    }

    void add_edge(int from, int to, int cap){
        maxFlowEdge[from].push_back(Edge(to, cap, cap, maxFlowEdge[to].size()));
        maxFlowEdge[to].push_back(Edge(from, 0, 0, maxFlowEdge[from].size() - 1));
    }
 
    void reset_flow(){
        for(int i = 0; i < N; i++ ){
            seen[i] = false;
            int sz = maxFlowEdge[i].size();
            for(int j = 0; j < sz; j++ ){
                maxFlowEdge[i][j].flow = maxFlowEdge[i][j].capacity;
            }
        }
    }
    
    long long get_maxflow(int S, int T){
        long long res = 0;
        while(1){
            mark++;
            int flow = find_augmenting_path(S, T, inf_flow);
            if (flow == 0) break;
            else res += flow;
        }
        return res;
    }
private:
    int find_augmenting_path(int at, int sink, int val){
        int sol = 0;
        seen[at] = mark;
        if (at == sink) return val;
        int sz = maxFlowEdge[at].size();
        for (int i = 0; i < sz; i++ ) {
            int v = maxFlowEdge[at][i].v;
            int f = maxFlowEdge[at][i].flow;
            if (seen[v] != mark && f > 0){
                sol = find_augmenting_path(v, sink, min(f, val));
                if (sol) {
                    maxFlowEdge[at][i].flow -= sol;
                    int rev = maxFlowEdge[at][i].rev_index;
                    maxFlowEdge[v][rev].flow += sol;
                    return sol;
                }
            }
        }
        return 0;
    }
};

int main() {
    FordFulkerson mf(3);
    mf.add_edge(0, 1, 2);
    mf.add_edge(1, 2, 2);
    assert(mf.get_maxflow(0, 2) == 2);

    mf = FordFulkerson(4);
    mf.add_edge(0, 1, 2);
    mf.add_edge(0, 2, 2);
    mf.add_edge(1, 3, 1);
    mf.add_edge(1, 2, 1);
    mf.add_edge(2, 3, 2);
    assert(mf.get_maxflow(0, 3) == 3);
    return 0;
}
