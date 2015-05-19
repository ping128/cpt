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
    vector<vector<T> > mat;
    int N, M;
    
    Matrix (int n, int m) {
        N = n;
        M = m;
        mat = vector<vector<T> >(N);
        for (int i = 0; i < N; i++)
            mat[i] = vector<T> (M);
    }

    Matrix mult(const Matrix &a) {
        assert(M == a.N);
        Matrix ret = Matrix(N, a.M);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < a.M; j++) {
                for (int k = 0; k < M; k++) {
                    ret.mat[i][j] += mat[i][k] * a.mat[k][j];
                }
            }
        }
        return ret;
    }

    Matrix power(long long ex) {
        assert(N == M);
        Matrix ret = identity(N), b = *this;
        while (ex) { if (ex & 1) ret = ret.mult(b); b = b.mult(b); ex >>= 1; }
        return ret;
    }

    Matrix identity(int n) {
        Matrix ret = Matrix(n, n);
        for (int i = 0; i < n; i++) ret.mat[i][i] = 1;
        return ret;
    }
    
    void print() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }
};
    

int main() {
    
    Matrix<int> start = Matrix<int>(2, 2);
    start.mat[0][1] = start.mat[1][0] = start.mat[1][1] = 1;

    start.print();
    Matrix<int> now = start;
    for (int i = 1; i < 10; i++) {
        now.print();
        now = now.mult(start);
    }

    for (int i = 1; i < 10; i++) {
        start.power(i).print();
    }
        
    
    return 0;
}
