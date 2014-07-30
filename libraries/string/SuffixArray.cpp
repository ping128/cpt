/////////////////////////////////////////////////////////////////////////////////////////////
//  Suffix Array
//
//  For example, s = "banana"
//  0 $(end of string)     0  1  2  3  4  5  6
//  1 a                    b  a  n  a  n  a  $
//  2 ana          RA[] = [4, 3, 6, 2, 5, 1, 0] (Rank array)
//  3 anana   ->   SA[] = [6, 5, 3, 1, 0, 4, 2] (Suffix array)
//  4 banana      LCP[] = [0, 1, 3, 0, 0, 2]    (Longest Common Prefix array
//  5 na                                         LCP between pairs of consecutive suffixes)
//  6 nana
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>

#include <cassert>

using namespace std;

#define x first
#define y second
#define SZ(x) ((int)((x).size()))

typedef long long LL;
typedef pair<int, int> PII; typedef pair<PII, int> PII2;
typedef vector<int> VI; typedef vector<VI> VVI;

class SuffixArray {
public:
    int N;
    VI SA, RA, LCP;
    SuffixArray(const char* S) : N(strlen(S)) {
        VI V; for (int i = 0; i < N; i++) V.push_back(S[i]); init(V);
    }
    SuffixArray(const VI &VV) : N(VV.size()) { VI V(VV); init(V); }
private:
    void init(VI& V) {
        VI OV(V), C(N);
        compress(V, C); compute_sa(V, C);
        RA.resize(N + 1); for(int i = 0; i <= N; i++) RA[SA[i]] = i;
        compute_lcp(OV);
    }
    void compress(VI& V, VI& C) {
        copy(V.begin(), V.end(), C.begin());
        sort(C.begin(), C.end());
        VI::iterator cend = unique(C.begin(), C.end());
        for (int i = 0; i < N; i++) V[i] = lower_bound(C.begin(), cend, V[i]) - C.begin() + 1;
        V.push_back(0); C.push_back(0);
    }
    void compute_sa(VI& V, VI& C) {
        VI T(N + 1);
        for (int i = 0; i <= N; i++) SA.push_back(i);
        for (int ski = 0; V[SA[N]] < N; ski = ski ? ski << 1 : 1) {
            fill(C.begin(), C.end(), 0);
            for (int i = 0; i < ski; i++) T[i] = N - i;
            for (int i = 0, p = ski; i <= N; i++) if(SA[i] >= ski) T[p++] = SA[i] - ski;
            for (int i = 0; i <= N; i++) C[V[i]]++;
            for (int i = 1; i <= N; i++) C[i] += C[i - 1];
            for (int i = N; i >= 0; i--) SA[--C[V[T[i]]]] = T[i];
            T[SA[0]] = 0;
            for (int j = 1; j <= N; j++) {
                int a = SA[j];
                int b = SA[j - 1];
                T[a] = T[b] + (a + ski >= N || b + ski >= N ||
                               V[a] != V[b] || V[a + ski] != V[b + ski]);
            }
            V.swap(T);
        }
    }
    void compute_lcp(const VI& OV) {
        LCP = VI(N);
        int len = 0;
        for (int i = 0; i < N; i++, len = max(0, len - 1)) {
            int si = RA[i], j = SA[si - 1];
            for (; i + len < N && j + len < N && OV[i + len] == OV[j + len]; len++);
            LCP[si - 1] = len;
        }
    }
};

int main() {
    string s = "banana";
    SuffixArray sa(s.c_str());
    for (int i = 0; i <= sa.N; i++) {
        printf("%d ", sa.SA[i]);
    }
    printf("\n");
    return 0;
}
