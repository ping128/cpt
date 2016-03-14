/////////////////////////////////////////////////////////////////////////////////////////////
//  Suffix Array
//
//  For example, s = "banana"
//                         0  1  2  3  4  5
//  0 a                    b  a  n  a  n  a
//  1 ana         pos[] = [3, 2, 5, 1, 4, 0] (Position of each suffix)
//  2 anana   ->   sa[] = [5, 3, 1, 0, 4, 2] (Suffix array)
//  3 banana      LCP[] = [1, 3, 0, 0, 2, 0] (Longest Common Prefix array
//  4 na                                      LCP between pairs of consecutive suffixes)
//  5 nana
/////////////////////////////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

using namespace std;

#define SZ(x) ((int)(x).size())
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

namespace SuffixArray
{
    const int MAXN = 1 << 21;
    const char * S;
    int N, gap;
    int sa[MAXN], pos[MAXN], tmp[MAXN], lcp[MAXN];
    bool sufCmp(int i, int j) {
        if (pos[i] != pos[j])
            return pos[i] < pos[j];
        i += gap;
        j += gap;
        return (i < N && j < N) ? pos[i] < pos[j] : i > j;
    }
    void buildSA(){
        N = strlen(S);
        REP(i, N) sa[i] = i, pos[i] = S[i];
        for (gap = 1;; gap *= 2) {
            sort(sa, sa + N, sufCmp);
            REP(i, N - 1) tmp[i + 1] = tmp[i] + sufCmp(sa[i], sa[i + 1]);
            REP(i, N) pos[sa[i]] = tmp[i];
            if (tmp[N - 1] == N - 1) break;
        }
    }
    void buildLCP() {
        for (int i = 0, k = 0; i < N; ++i) if (pos[i] != N - 1) {
            for (int j = sa[pos[i] + 1]; S[i + k] == S[j + k];)
            ++k;
            lcp[pos[i]] = k;
            if (k) --k;
        }
    }
}

int main() {
    string s = "banana";
    SuffixArray::S = s.c_str();
    SuffixArray::buildSA();
    SuffixArray::buildLCP();
    int N = SZ(s);
    REP(i, N) cout << SuffixArray::sa[i] << " ";
    cout << endl;
    REP(i, N) cout << SuffixArray::lcp[i] << " ";
    cout << endl;
    REP(i, N) cout << SuffixArray::pos[i] << " ";
    cout << endl;

    return 0;
}


