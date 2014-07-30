///////////////////////////////////////////////////////////////////////////////
//  Global Minimum Cut
///////////////////////////////////////////////////////////////////////////////

// Adjacency matrix implementation of Stoer-Wagner min cut algorithm.
//
// Running time:
//     O(|V|^3)
//
// INPUT: 
//     - graph - adjacency matrix
//
// OUTPUT:
//     - (min cut value, nodes in half of min cut)

#include <cmath>
#include <vector>
#include <iostream>
#include <cassert>

using namespace std;

template<class Cost_t, Cost_t INF>

class GlobalMinCut {
public:
    typedef vector<Cost_t> VI;
    typedef vector<VI> VVI;
    pair<Cost_t, VI> GetMinCut(VVI &weights) {
        int N = weights.size();
        VI used(N), cut, best_cut;
        int best_weight = -1;
        for (int phase = N-1; phase >= 0; phase--) {
            VI w = weights[0];
            VI added = used;
            int prev, last = 0;
            for (int i = 0; i < phase; i++) {
                prev = last;
                last = -1;
                for (int j = 1; j < N; j++)
                    if (!added[j] && (last == -1 || w[j] > w[last])) last = j;
                if (i == phase-1) {
                    for (int j = 0; j < N; j++) weights[prev][j] += weights[last][j];
                    for (int j = 0; j < N; j++) weights[j][prev] = weights[prev][j];
                    used[last] = true;
                    cut.push_back(last);
                    if (best_weight == -1 || w[last] < best_weight) {
                        best_cut = cut;
                        best_weight = w[last];
                    }
                } else {
                    for (int j = 0; j < N; j++)
                        w[j] += weights[last][j];
                    added[last] = true;
                }
            }
        }
        return make_pair(best_weight, best_cut);
    }
};

const int INF = 1000000000;
typedef vector<int> VI;
typedef vector<VI> VVI;

int main() {
    int N = 4;
    VVI graph (N);
    for (int i = 0; i < N; i++) graph[i] = VI(N);
    int in[4][4] = {{0, 4, 10, 0},
                    {0, 0, 1, 10},
                    {2, 0, 0, 2},
                    {0, 0, 0, 0}};
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            graph[i][j] = in[i][j];
    GlobalMinCut<int, INF> solver;
    pair<int, VI> ret = solver.GetMinCut(graph);
    assert(ret.first == 1);
    cout << ret.first << endl;
    for (int i = 0; i < (int)ret.second.size(); i++) cout << ret.second[i] << endl;
    return 0;
}
    

