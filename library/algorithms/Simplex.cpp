////////////////////////////////////////////////////////////
//  Simplx Algorithm
//  Two-phase simplex algorithm for solving linear programs of the form
//
//     maximize     c^T x
//     subject to   Ax <= b
//                  x >= 0
//
//  INPUT: A -- an m x n matrix
//         b -- an m-dimensional vector
//         c -- an n-dimensional vector
//         x -- a vector where the optimal solution will be stored
//  OUTPUT: value of the optimal solution (infinity if unbounded
//          above, nan if infeasible)
//  Link: https://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=1439
////////////////////////////////////////////////////////////
#include <bits/stdc++.h>

using namespace std;

#define PB(x) push_back(x)
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

typedef long double DOUBLE;
typedef vector<DOUBLE> VD;
typedef vector<VD> VVD;
typedef vector<int> VI;
const DOUBLE EPS = 1e-9;
class LPSolver {
public:
    int m, n;
    VI N, B;
    VVD D;
    LPSolver(const VVD &A, const VD &b, const VD &c) :
        m(b.size()), n(c.size()), N(n + 1), B(m), D(m + 2, VD(n + 2)) {
        REP(i, m) REP(j, n) D[i][j] = A[i][j];
        REP(i, m) { B[i] = n + i; D[i][n] = -1; D[i][n + 1] = b[i]; }
        REP(j, n) { N[j] = j; D[m][j] = -c[j]; }
        N[n] = -1; D[m + 1][n] = 1;
    }
    void Pivot(int r, int s) {
        REP(i, m + 2) if (i != r)
            REP(j, n + 2) if (j != s)
                D[i][j] -= D[r][j] * D[i][s] / D[r][s];
        REP(j, n + 2) if (j != s) D[r][j] /= D[r][s];
        REP(i, m + 2) if (i != r) D[i][s] /= -D[r][s];
        D[r][s] = 1.0 / D[r][s];
        swap(B[r], N[s]);
    }
    bool Simplex(int phase) {
        int x = phase == 1 ? m + 1 : m;
        while (true) {
            int s = -1;
            REP(j, n + 1) {
                if (phase == 2 && N[j] == -1) continue;
                if (s == -1 || D[x][j] < D[x][s] || (D[x][j] == D[x][s] && N[j] < N[s])) s = j;
            }
            if (D[x][s] > -EPS) return true;
            int r = -1;
            REP(i, m) {
                if (D[i][s] < EPS) continue;
                if (r == -1 || D[i][n + 1] / D[i][s] < D[r][n + 1] / D[r][s] ||
                    ((D[i][n + 1] / D[i][s]) == (D[r][n + 1] / D[r][s]) && B[i] < B[r])) r = i;
            }
            if (r == -1) return false;
            Pivot(r, s);
        }
    }
    DOUBLE Solve(VD &x) {
        int r = 0;
        for (int i = 1; i < m; i++) if (D[i][n + 1] < D[r][n + 1]) r = i;
        if (D[r][n + 1] < -EPS) {
            Pivot(r, n);
            if (!Simplex(1) || D[m + 1][n + 1] < -EPS) return -numeric_limits<DOUBLE>::infinity();
            for (int i = 0; i < m; i++) if (B[i] == -1) {
                int s = -1;
                for (int j = 0; j <= n; j++)
                    if (s == -1 || D[i][j] < D[i][s] || (D[i][j] == D[i][s] && N[j] < N[s])) s = j;
                Pivot(i, s);
            }
        }
        if (!Simplex(2)) return numeric_limits<DOUBLE>::infinity();
        x = VD(n);
        for (int i = 0; i < m; i++) if (B[i] < n) x[B[i]] = D[i][n + 1];
        return D[m][n + 1];
    }
};

int main() {
    int N, M;
    while (cin >> N >> M) {
        VD c;
        DOUBLE x;
        VVD A = VVD(M);
        VD b;
        REP(i, N) {
            cin >> x;
            c.PB(x);
        }
        REP(i, M) {
            REP(j, N) {
                cin >> x;
                A[i].PB(x);
            }
            cin >> x;
            b.PB(x);
        }
        LPSolver solver(A, b, c);
        VD xx;
        DOUBLE value = solver.Solve(xx);
        cout << "Nasa can spend " << round(ceil(value * M)) << " taka." << endl;
    }
    return 0;
}
