///////////////////////////////////////////////////////////////////////////////
//  Matrix
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

template<class T>
class Matrix {
public:
    vector<vector<T>> a;
    int N, M;
    Matrix() {}
    Matrix (int n, int m) {
        N = n;
        M = m;
        a = vector<vector<T>>(N);
        for (int i = 0; i < N; i++)
            a[i] = vector<T>(M);
    }
    Matrix mult(const Matrix &o) {
        assert(M == o.N);
        Matrix ret = Matrix(N, o.M);
        REP(i, N) REP(j, o.M) REP(k, M)
            ret[i][j] += a[i][k] * o.a[k][j];
        return ret;
    }
    Matrix power(long long ex) {
        assert(N == M);
        Matrix ret = identity(), b = *this;
        while (ex) { if (ex & 1) ret = ret.mult(b); b = b.mult(b); ex >>= 1; }
        return ret;
    }
    Matrix identity() {
        assert(N == M);
        Matrix ret = Matrix(N, N);
        REP(i, N) ret[i][i] = 1;
        return ret;
    }
    vector<T> &operator[](int ind) {
        assert(ind >= 0 && ind < N);
        return a[ind];
    }
    inline friend ostream& operator << (ostream &out, Matrix m) {
        REP(i, m.N) { out << "|"; REP(j, m.M) out << " " << m.a[i][j]; out << " |" << endl; }
        return out;
    }
};

int main() {
    Matrix<int> start = Matrix<int>(2, 2);
    start[0][1] = start[1][0] = start[1][1] = 1;
    cout << start << endl;
    Matrix<int> now = start;
    for (int i = 1; i < 10; i++) {
        cout << now << endl;
        now = now.mult(start);
    }
    for (int i = 1; i < 10; i++) {
        cout << start.power(i) << endl;
    }
    return 0;
}
