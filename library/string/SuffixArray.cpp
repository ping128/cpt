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
    int n;
    vector<int> sa, ra, lcp;
    SuffixArray(const string& S) : n(S.size()) {
        vector<int> v; for (int i = 0; i < n; i++) v.push_back(S[i]); init(v);
    }
    SuffixArray(const vector<int> &VV) : n(VV.size()) { vector<int> v(VV); init(v); }
private:
    void init(vector<int>& v) {
        vector<int> ov(v), c(n);
        compress(v, c); compute_sa(v, c);
        ra.resize(n + 1); for(int i = 0; i <= n; i++) ra[sa[i]] = i;
        compute_lcp(ov);
    }
    void compress(vector<int>& v, vector<int>& c) {
        copy(v.begin(), v.end(), c.begin());
        sort(c.begin(), c.end());
        vector<int>::iterator cend = unique(c.begin(), c.end());
        for (int i = 0; i < n; i++) v[i] = lower_bound(c.begin(), cend, v[i]) - c.begin() + 1;
        v.push_back(0); c.push_back(0);
    }
    void compute_sa(vector<int>& v, vector<int>& c) {
        vector<int> t(n + 1);
        for (int i = 0; i <= n; i++) sa.push_back(i);
        for (int ski = 0; v[sa[n]] < n; ski = ski ? ski << 1 : 1) {
            fill(c.begin(), c.end(), 0);
            for (int i = 0; i < ski; i++) t[i] = n - i;
            for (int i = 0, p = ski; i <= n; i++) if(sa[i] >= ski) t[p++] = sa[i] - ski;
            for (int i = 0; i <= n; i++) c[v[i]]++;
            for (int i = 1; i <= n; i++) c[i] += c[i - 1];
            for (int i = n; i >= 0; i--) sa[--c[v[t[i]]]] = t[i];
            t[sa[0]] = 0;
            for (int j = 1; j <= n; j++) {
                int a = sa[j];
                int b = sa[j - 1];
                t[a] = t[b] + (a + ski >= n || b + ski >= n ||
                               v[a] != v[b] || v[a + ski] != v[b + ski]);
            }
            v.swap(t);
        }
    }
    void compute_lcp(const vector<int>& ov) {
        lcp = vector<int>(n);
        int len = 0;
        for (int i = 0; i < n; i++, len = max(0, len - 1)) {
            int si = ra[i], j = sa[si - 1];
            for (; i + len < n && j + len < n && ov[i + len] == ov[j + len]; len++);
            lcp[si - 1] = len;
        }
    }
};

int main() {
    SuffixArray sa("banana");
    for (int i = 0; i <= sa.n; i++) {
        printf("%d ", sa.sa[i]);
    }
    printf("\n");
    return 0;
}
