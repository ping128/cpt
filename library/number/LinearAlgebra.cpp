///////////////////////////////////////////////////////////////////////////////
//  Linear Algebra/ Solving a system of linear equation
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<int> VI;
typedef pair<int,int> PII;
typedef long long LL;

// Compute n choose m
LL binomial(int n, int m) {
    if (n < m || n < 0 || m < 0) return 0;
    LL ans = 1, ans2 = 1;
    for (int i = 0; i < m; i++) {
        ans *= n - i;
        ans2 *= i + 1;
    }
    return ans / ans2;
}

// compute catalan number (n >= 0)
// 1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786, 208012, 742900, 2674440, 9694845, ...
//
// Ex. Number of ways to insert n pairs of parentheses in a word of n+1 letters.
//     E.g., for n=3 there are 5 ways: ((ab)(cd)), (((ab)c)d), ((a(bc))d), (a((bc)d)), (a(b(cd))).
LL catalan_number(int n) {
    return binomial(n * 2, n) / (n + 1);
}

// return a % b (positive value)
template<typename T>
T mod(T a, T b) {
    return ((a%b)+b)%b;
}

// computes gcd(a,b)
template<typename T>
T gcd(T a, T b) {
    T tmp;
    while (b) {
        a%=b; tmp=a; a=b; b=tmp;
    }
    return a;
}

// computes lcm(a,b)
template<typename T>
T lcm(T a, T b) {
    return a / gcd(a,b) * b;
}

// returns d = gcd(a,b); finds x,y such that d = ax + by
template<typename T>
T extended_euclid(T a, T b, T &x, T &y) {  
    T xx = y = 0;
    T yy = x = 1;
    while (b) {
        T q = a / b;
        T t = b; b = a % b; a = t;
        t = xx; xx = x - q * xx; x = t;
        t = yy; yy = y - q * yy; y = t;
    }
    return a;
}

// computes b such that ab = 1 (mod n), returns -1 on failure
template<typename T>
T mod_inverse(T a, T n) {
    T x, y;
    T d = extended_euclid(a, n, x, y);
    if (d > 1) return -1;
    return mod(x,n);
}

// finds all solutions to ax = b (mod n)
template<typename T>
vector<T> modular_linear_equation_solver(T a, T b, T n) {
    T x, y;
    vector<T> solutions;
    T d = extended_euclid(a, n, x, y);
    if (!(b%d)) {
        x = mod (x*(b/d), n);
        for (T i = 0; i < d; i++)
            solutions.push_back(mod(x + i*(n/d), n));
    }
    return solutions;
}


//  return the smallest value of x s.t x >= lowerBound 
//  and ax = c (mod b) 
LL get_greater(LL c, LL a, LL b, LL lowerBound){
    LL x, y;
    LL A = a, B = b, C = c;
    LL g = extended_euclid(A, B, x, y);
    x = (x + B) % B;
    LL key = (-C) * x / g;
    LL k = (lowerBound - key) / B;
    LL ret = key + B * k;
    while(ret < lowerBound){
        if(B > 0) ret += B; 
        else ret -= B;
    }
    return ret;
}

//  return the largest value of x s.t x <= upperBound
//	and ax = c (mod b)
LL get_less(LL c, LL a, LL b, LL upperBound){
    LL x, y;
    LL A = a, B = b, C = c;
    LL g = extended_euclid(A, B, x, y);
    x = (x + B) % B;
    LL key = (-C) * x / g;
    LL k = (upperBound - key) / B;
    LL ret = key + B * k;
    // for edge case
    if(B > 0) ret += 2 * B;
    else ret -= 2 * B;
    while(ret > upperBound){
        if(B > 0) ret -= B; 
        else ret += B;
    }
    return ret;
}

// Chinese remainder theorem (special case): find z such that
// z % x = a, z % y = b.  Here, z is unique modulo M = lcm(x,y).
// Return (z,M).  On failure, M = -1.
template<typename T>
pair<T, T> chinese_remainder_theorem(T x, T a, T y, T b) {
    T s, t;
    T d = extended_euclid(x, y, s, t);
    if (a%d != b%d) return make_pair(0, -1);
    return make_pair(mod(s*b*x+t*a*y,x*y)/d, x*y/d);
}

// Chinese remainder theorem: find z such that
// z % x[i] = a[i] for all i.  Note that the solution is
// unique modulo M = lcm_i (x[i]).  Return (z,M).  On 
// failure, M = -1.  Note that we do not require the a[i]'s
// to be relatively prime.
template<typename T>
pair<T, T> chinese_remainder_theorem(const vector<T> &x, const vector<T> &a) {
    PII ret = make_pair(a[0], x[0]);
    for (T i = 1; i < x.size(); i++) {
        ret = chinese_remainder_theorem(ret.second, ret.first, x[i], a[i]);
        if (ret.second == -1) break;
    }
    return ret;
}

// computes x and y such that ax + by = c; on failure, x = y =-1
template<typename T>
void linear_diophantine(T a, T b, T c, T &x, T &y) {
    T d = gcd(a, b);
    if (c % d) {
        x = y = -1;
    } else {
        x = c / d * mod_inverse(a / d, b / d);
        y = (c - a * x) / b;
    }
}

///////////////////////////
//  GaussJordan
///////////////////////////

// Gauss-Jordan elimination with full pivoting.
//
// Uses:
//   (1) solving systems of linear equations (AX=B)
//   (2) inverting matrices (AX=I)
//   (3) computing determinants of square matrices
//
// Running time: O(n^3)
//
// INPUT:    a[][] = an nxn matrix
//           b[][] = an nxm matrix
//
// OUTPUT:   X      = an nxm matrix (stored in b[][])
//           A^{-1} = an nxn matrix (stored in a[][])
//           returns determinant of a[][]

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const double EPS = 1e-10;

typedef vector<int> VI;
typedef double T;
typedef vector<T> VT;
typedef vector<VT> VVT;

T GaussJordan(VVT &a, VVT &b) {
    const int n = a.size();
    const int m = b[0].size();
    VI irow(n), icol(n), ipiv(n);
    T det = 1;
    
    for (int i = 0; i < n; i++) {
        int pj = -1, pk = -1;
        for (int j = 0; j < n; j++) 
            if (!ipiv[j])
                for (int k = 0; k < n; k++) 
                    if (!ipiv[k])
                        if (pj == -1 || fabs(a[j][k]) > fabs(a[pj][pk])) { pj = j; pk = k; }
        if (fabs(a[pj][pk]) < EPS) { cerr << "Matrix is singular." << endl; exit(0); }
        ipiv[pk]++;
        swap(a[pj], a[pk]);
        swap(b[pj], b[pk]);
        if (pj != pk) det *= -1;
        irow[i] = pj;
        icol[i] = pk;

        T c = 1.0 / a[pk][pk];
        det *= a[pk][pk];
        a[pk][pk] = 1.0;
        for (int p = 0; p < n; p++) a[pk][p] *= c;
        for (int p = 0; p < m; p++) b[pk][p] *= c;
        for (int p = 0; p < n; p++) 
            if (p != pk) {
                c = a[p][pk];
                a[p][pk] = 0;
                for (int q = 0; q < n; q++) a[p][q] -= a[pk][q] * c;
                for (int q = 0; q < m; q++) b[p][q] -= b[pk][q] * c;      
            }
    }
    for (int p = n-1; p >= 0; p--) {
        if (irow[p] != icol[p]) {
            for (int k = 0; k < n; k++) 
                swap(a[k][irow[p]], a[k][icol[p]]);
        }
    }
    return det;
}

//////////////////////////////////////
//  ReducedRowEchelonForm/ Find Rank
//
//  can be modified to find a normal rank of an nxn matrix
//////////////////////////////////////

template<typename T>
int findRank(vector<vector<T> > &a, vector<T> &b, T p) {
    // M equations, N variables, modulo p
    int M = a.size();
    int N = a[0].size();
    int ans = 0;     
     
    // Using Gauss-Jordan in [ A : b ] to make it in row-echellon form.
    // So that we can find the rank. We also need to make sure that the
    // rank of [ A ] is equal to the rank of [A : b], else there are
    // no solutions
    for (int c = 0; c < N; c++) {
        int j = ans;
        for (int i = ans + 1; i < M; i++) {
            if (a[i][c]) {
                j = i;
                break;
            }
        }
        if (a[j][c] == 0) continue;
        swap(a[j], a[ans]);
        swap(b[j], b[ans]);
        // s = 1 / a[ans][c] (in mod p)
        T s = mod_inverse(a[ans][c], p);
        for (int k = 0; k < N; k++) 
            a[ans][k] = (a[ans][k] * s) % p;
        b[ans] = (b[ans] * s) % p;
        for (int k = ans + 1; k < M; k++) {
            int coef = -a[k][c];
            for (int l = 0; l < N; l++) 
                a[k][l] = ((a[k][l] + a[ans][l] * coef) % p + p) % p;
            b[k] = (b[k] + b[ans] * coef) % p;
        }
        ans++;
    }
    
    // If rank of [A] is not equal to the rank of [A : b]
    for (int i = ans; i < M; i++) {
        if (b[i]) {
            return -1;
        }
    }
          
    return ans;
}

int main() {
    const int n = 4;
    const int m = 2;
    double A[n][n] = { {1,2,3,4},{1,0,1,0},{5,3,2,4},{6,1,4,6} };
    double B[n][m] = { {1,2},{4,3},{5,6},{8,7} };
    VVT a(n), b(n);
    for (int i = 0; i < n; i++) {
        a[i] = VT(A[i], A[i] + n);
        b[i] = VT(B[i], B[i] + m);
    }
  
    double det = GaussJordan(a, b);
  
    // expected: 60  
    cout << "Determinant: " << det << endl;

    // expected: -0.233333 0.166667 0.133333 0.0666667
    //           0.166667 0.166667 0.333333 -0.333333
    //           0.233333 0.833333 -0.133333 -0.0666667
    //           0.05 -0.75 -0.1 0.2
    cout << "Inverse: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << a[i][j] << ' ';
        cout << endl;
    }
    
    // expected: 1.63333 1.3
    //           -0.166667 0.5
    //           2.36667 1.7
    //           -1.85 -1.35
    cout << "Solution: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            cout << b[i][j] << ' ';
        cout << endl;
    }

    for (int i = 0; i <= 10; i++) cout << catalan_number(i) << endl;
    return 0;

}
