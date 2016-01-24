////////////////////////////////////////////////////////////
//  Hungarian Algorithm
//  Adopted from http://e-maxx.ru/algo/assignment_hungary
////////////////////////////////////////////////////////////
#include <bits/stdc++.h>

using namespace std;

#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

template<class T>
class Hungarian {
public:
    T min_cost;
    vector<int> assignments;
    Hungarian() {}
    Hungarian(vector<vector<T>> cost, T inf = 1e9) { // zero-based index
        int n = cost.size();
        int m = cost[0].size();
        assignments.resize(n);
        vector<T> u(n+1), v(m+1), p(m+1), way(m+1);
        for (int i = 1; i <= n; i++) {
            p[0] = i;
            int j0 = 0;
            vector<T> minv (m+1, inf);
            vector<bool> used (m+1, false);
            do {
                used[j0] = true;
                int i0 = p[j0], j1 = 0;
                T delta = inf;
                for (int j = 1; j <= m; j++) {
                    if (!used[j]) {
                        int cur = cost[i0 - 1][j - 1] - u[i0] - v[j];
                        if (cur < minv[j])
                            minv[j] = cur, way[j] = j0;
                        if (minv[j] < delta)
                            delta = minv[j], j1 = j;
                    }
                }
                for (int j = 0; j <= m; j++) {
                    if (used[j])
                        u[p[j]] += delta, v[j] -= delta;
                    else
                        minv[j] -= delta;
                }
                j0 = j1;
            } while (p[j0] != 0);
            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }
        min_cost = -v[0];
        for (int j = 1; j <= m; j++) {
            if (p[j]) assignments[p[j] - 1] = j - 1;
        }
    }
};

int a[3][3] = {{10, 2, 3}, {5, 4, 3}, {5, 0, 2}};
int main() {
    vector<vector<long long>> costs;
    REP(i, 3) costs.push_back(vector<long long>(a[i], a[i] + 3));
    Hungarian<long long> solver(costs);
    cout << "Min cost: " << solver.min_cost << endl;
    REP(i, 3) cout << solver.assignments[i] << " ";
    cout << endl;
    return 0;
}
