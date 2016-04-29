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

typedef complex<double> cpx;
class FFT {
public:
    static int rev(int n, int x) {
        int r = 0;
        for (int i = 0; i < n; i++)
            if (x & (1 << i))
                r |= (1 << (n - i - 1));
        return r;
    }
    // Size of a must be a power of 2
    static vector<cpx> fft(const vector<cpx> &f, bool inv) {
        const double TWO_PI = 2 * acos(-1);
        int n = SZ(f);
        assert(__builtin_popcount(n) == 1);
        vector<cpx> v(n);
        int r = __builtin_ctz(n);
        REP(i, n) v[rev(r, i)] = f[i];
        REP(s, r) {
            int m = 1 << (s + 1);
            double a = (inv ? 1 : -1) * TWO_PI / m;
            cpx b(cos(a), sin(a));
            for (int k = 0; k < n; k += m) {
                cpx w(1, 0);
                int h = m >> 1;
                for (int j = 0; j < h; j++) {
                    cpx t = w * v[k + j + h];
                    cpx u = v[k + j];
                    v[k + j] = u + t;
                    v[k + j + h] = u - t;
                    w *= b;
                }
            }
        }
        if (inv) for (int i = 0; i < n; i++) v[i] /= n;
        return v;
    }
    static vector<cpx> multiply(vector<cpx> &a, vector<cpx> &b) {
        int n = SZ(a);
        vector<cpx> A = fft(a, false);
        vector<cpx> B = fft(b, false);
        REP(i, n) A[i] *= B[i];
        return fft(A, true);
    }
};

int main() {
    vector<cpx> a = {1, 2, 3, 4};
    vector<cpx> b = {5, 6, 7, 8};
    vector<cpx> c = FFT::multiply(a, b);
    for (auto x : c) cout << x << " ";
    cout << endl;
    return 0;
}
