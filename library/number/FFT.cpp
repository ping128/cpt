///////////////////////////////////////////////////////////////////////////////
//  Fast Fourier Transform
//
//  fft  X_k =         \sum_{j=0}^{N - 1} x_k e^{-i * 2pi * j * k / N}
//  ifft x_k = (1/N) * \sum_{j=0}^{N - 1} x_k e^{ i * 2pi * j * k / N}
//
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

class Complex {
public:
    double real, img;
    Complex() : real(0), img(0) {}
    Complex(double r) : real(r) {}
    Complex(double r, double i) : real(r), img(i) {}
    double square() const { return real * real + img * img; }
    Complex bar() const { return Complex(real, -img); }
    Complex operator + (Complex o) const { return Complex(real + o.real, img + o.img); }
    Complex operator * (Complex o) const { return Complex(real * o.real - img * o.img, real * o.img + img * o.real); }
    Complex operator / (Complex o) const {
        Complex res = *this * o.bar();
        return Complex(res.real / o.square(), res.img / o.square());
    }
    Complex exp(double theta) const { // e^i(theta)
        return Complex(cos(theta), sin(theta));
    }
};

class FastFourierTransform {
public:
    FastFourierTransform() {}
    vector<Complex> FFT(vector<Complex> &a) { return FFT(a, false); }
    vector<Complex> iFFT(vector<Complex> &a) { return FFT(a, true); }
    vector<Complex> multiply(vector<Complex> &x, vector<Complex> &y) {
        vector<Complex> X = FFT(x);
        vector<Complex> Y = FFT(y);
        vector<Complex> Z;
        REP(i, SZ(x)) Z.PB(X[i] * Y[i]);
        return iFFT(Z);
    }
private:
    // Size of a must be a power of 2
    vector<Complex> FFT(vector<Complex> &a, bool inverse) {
        int N = SZ(a);
        assert(__builtin_popcount(N) == 1);
        Complex *in = new Complex[N];
        Complex *out = new Complex[N];
        REP(i, N) in[i] = a[i];
        FFT(in, out, N, inverse ? 1 : -1);
        vector<Complex> res(out, out + N);
        if (inverse) { REP(i, N) res[i] = res[i] / N; }
        delete in; delete out;
        return res;
    }
    const double TWO_PI = 2 * acos(-1);
    void FFT(Complex *in, Complex *out, int N, int dir, int step = 1){
        if (N < 1) return;
        if (N == 1) {
            out[0] = in[0];
            return;
        }
        FFT(in, out, N / 2, dir, step * 2);
        FFT(in + step, out + N / 2, N / 2, dir, step * 2);
        for (int i = 0; i < N / 2; i++) {
            Complex even = out[i];
            Complex odd = out[i + N / 2];
            Complex temp;
            out[i] = even + temp.exp(dir * TWO_PI * i / N) * odd;
            out[i + N / 2] = even + temp.exp(dir * TWO_PI * (i + N / 2) / N) * odd;
        }
    }
};

int main() {
    const double TWO_PI = 2 * acos(-1);
    int N = 8;
    Complex a[N] = {0, 1, Complex(1,3), Complex(0,5), 1, 0, 2, 0};
    Complex b[N] = {1, Complex(0,-2), Complex(0,1), 3, -1, -3, 1, -2};
    vector<Complex> aa(a, a + N);
    vector<Complex> bb(b, b + N);
    FastFourierTransform fft;
    vector<Complex> fft_a = fft.FFT(aa);

    cout << "FFT nlgn :  ";
    REP(i, N) printf("%.5lf %.5lf ", fft_a[i].real, fft_a[i].img);
    cout << endl;

    cout << "FFT n^2  :  ";
    REP(i, N) {
        Complex x;
        Complex temp;
        REP(j, N) {
            x = x + a[j] * temp.exp(-j * i * TWO_PI / N);
        }
        printf("%.5lf %.5lf ", x.real, x.img);
    }
    cout << endl;

    vector<Complex> AB = fft.multiply(aa, bb);
    cout << "A*B (nlgn): ";
    REP(i, N) printf("%.5lf %.5lf ", AB[i].real, AB[i].img);
    cout << endl;

    cout << "A*B (n^2) : ";
    REP(i, N) {
        Complex x;
        REP(j, N) {
            x = x + a[j] * b[(N + i - j) % N];
        }
        printf("%.5lf %.5lf ", x.real, x.img);
    }
    cout << endl;

    return 0;
}
