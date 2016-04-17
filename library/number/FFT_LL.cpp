///////////////////////////////////////////////////////////////////////////////
//  Fast Fourier Transform
//
//  fft  X_k =         \sum_{j=0}^{N - 1} x_k e^{-i * 2pi * j * k / N}
//  ifft x_k = (1/N) * \sum_{j=0}^{N - 1} x_k e^{ i * 2pi * j * k / N}
//  ex:
//  [1, 2, 3, 4]
//  [5, 6, 7, 8]
//  66: 5 * 1 + 6 * 4 + 7 * 3 + 8 * 2
//  68: 5 * 2 + 6 * 1 + 7 * 4 + 8 * 3
//  66: 5 * 3 + 6 * 2 + 7 * 1 + 8 * 4
//  60: 5 * 4 + 6 * 3 + 7 * 2 + 8 * 1
//  Usage
//  Given x[0...N-1], y[0...N-1]
//  The convolution z[n] = sum of x[k]y[n-k] (k = 0,...,N-1)
//  The index is cyclic: x[-1]=x[N-1], etc.
//  To find z[] in O(NlgN)
//     1. Compute X = fft(x), Y = fft(y)
//     2. fft(z) = Z = X * Y (element-wise multiplication)
//     3. z = ifft(Z)
///////////////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

using namespace std;

#define SZ(x) ((int)(x).size())
#define PB(x) push_back(x)
#define MEMSET(x,v) memset(x,v,sizeof(x))
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

typedef long long LL;

class FFT {
public:
    const LL mod = 5 * (1 << 25) + 1;
    const LL root = 243;
    const LL root_1 = 114609789;
    const LL root_pw = 1<<25;
    LL gcd(LL a, LL b, LL &s, LL &t) { // a * s + b * t = g
        if (b==0) {
            t = 0; s = (a < 0) ? -1 : 1; return (a < 0) ? -a : a;
        } else {
            LL g = gcd(b, a%b, t, s);  t -= a/b*s;  return g;
        }
    }
    LL inverse(LL n, LL mod) {
        LL s, t;
        gcd(n, mod, s, t);
        return (s > 0) ? s : s + mod;
    }
    // Length of a is a power of 2
    void fft (vector<LL> & a, bool invert) {
        assert(__builtin_popcount(SZ(a)) == 1);
        int n = (int) a.size();
        for (int i = 1, j = 0; i < n; ++i) {
            int bit = n >> 1;
            for (; j >= bit; bit >>= 1)
                j -= bit;
            j += bit;
            if (i < j) swap (a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            LL wlen = invert ? root_1 : root;
            for (LL i = len; i < root_pw; i <<= 1)
                wlen = LL (wlen * 1LL * wlen % mod);
            for (int i = 0; i < n; i += len) {
                LL w = 1;
                for (int j = 0; j < len / 2; ++j) {
                    LL u = a[i + j],  v = LL (a[i + j + len/2] * 1LL * w % mod);
                    a[i + j] = u + v < mod ? u + v : u + v - mod;
                    a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + mod;
                    w = LL (w * 1LL * wlen % mod);
                }
            }
        }
        if (invert) {
            LL nrev = inverse(n, mod);
            for (int i = 0; i < n; ++i)
                a[i] = LL (a[i] * 1LL * nrev % mod);
        }
    }
    vector<LL> multiply(vector<LL> &a, vector<LL> &b) {
        int n = SZ(a);
        vector<LL> res(n);
        fft(a, false);
        fft(b, false);
        REP(i, n) res[i] = a[i] * b[i] % mod;
        fft(res, true);
        return res;
    }
};

int main() {
    vector<LL> a = {1, 2, 3, 4};
    vector<LL> b = {5, 6, 7, 8};
    FFT solver;
    vector<LL> c = solver.multiply(a, b);
    for (auto x : c) cout << x << " ";
    cout << endl;
    return 0;
}
