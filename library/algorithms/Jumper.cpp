////////////////////////////////////////////////////////////
//  Jumper
//  Computes the jumper table
//  Link: http://codeforces.com/contest/500/problem/E
//        http://codeforces.com/contest/500/submission/9440685
////////////////////////////////////////////////////////////
#include <bits/stdc++.h>

using namespace std;

#define x first
#define y second
#define INF (0x3f3f3f3f)

#define SZ(x) ((int)((x).size()))
#define PB(x) push_back(x)
#define MEMSET(x,v) memset(x,v,sizeof(x))
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

typedef long long LL;
typedef pair<int, int> PII; typedef pair<PII, int> PII2;

class Jumper {
public:
    int N, H;
    vector<vector<int> > P;
    Jumper (int n, int *parent) { // zero-based index
        N = n;
        H = log2(N) + 2;
        P.resize(N, vector<int>(H));
        REP(i, N) REP(j, H) P[i][j] = -1;
        REP(i, N) {
            assert(parent[i] < N);
            P[i][0] = parent[i];
        }
        for (int h = 1; h < H; h++)
            REP(i, N)
                if (P[i][h - 1] != -1) {
                    P[i][h] = P[P[i][h - 1]][h - 1];
                    // Note for a value updating
                    // V[i][h] = V[i][h - 1] + V[P[i][h - 1]][h - 1];
                }
    }
};
