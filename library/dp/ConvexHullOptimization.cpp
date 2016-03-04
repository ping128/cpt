//////////////////////////////////////////////////////////////////////////////////////////////////
//  Convex Hull Optimization
//
//  dp[i] = min(dp[j] + b[j] * a[i]), j < i
//  y     = min(c     + m    * x)
//
//  Requires b[j] >= b[j + 1] (lines are added in decreasing order of slopes
//  Requires for this implementation: a[i] <= a[i + 1] (query x in increasing order)
// 
//  Link: http://codeforces.com/contest/319/problem/C
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

template<class T, int MAX_SIZE>
class ConvexHullOptimization {
public:
    typedef struct line_st {
        T c, m;
    } Line;
    vector<Line> sk;
    int front, rear;
    ConvexHullOptimization () { front = rear = 0; sk.resize(MAX_SIZE); }
    void add_line(T m, T c) {
        Line new_line = {c, m};
        if (front < rear && sk[rear - 1].m == new_line.m) {
            sk[rear - 1].c = min(sk[rear - 1].c, new_line.c);
            return ;
        }
        while (rear - front >= 2 && cross_left(sk[rear - 2], sk[rear - 1], new_line)) rear--;
        sk[rear++] = new_line;
    }
    // Returns the minimum value of y
    T minimize(T x) {
        if (front == rear) return 0;
        while (rear - front >= 2 && cal_y(sk[front], x) > cal_y(sk[front + 1], x)) front++;
        return cal_y(sk[front], x);
    }
private:
    // Returns true if x_ab > x_bc
    //   x_ab = the x-coordinate of the intersection of A and B
    //   x_bc = the x-coordinate of the intersection of B and new_line
    bool cross_left(Line A, Line B, Line new_line) {
        long double x_ab = (long double)(B.c - A.c) / (A.m - B.m);
        long double x_bc = (long double)(new_line.c - B.c) / (B.m - new_line.m);
        return x_ab > x_bc;
    }
    T cal_y(Line A, T x) {
        return A.m * x + A.c;
    }
};
                              
#define MAXN (100005)
typedef long long LL;
int heights[MAXN];
int costs[MAXN];
LL dp[MAXN];

int main(){
    int n;
    scanf("%d", &n);
    for(int i = 1; i <= n; i++ ) scanf("%d", &heights[i]);
    for(int i = 1; i <= n; i++ ) scanf("%d", &costs[i]);
    
    // dp[i] = min({dp[k] + costs[k] * heights[i]})
    // y = dp[i], c = dp[k], m = costs[k], x = heights[i]
    // costs is decreasing, heights is increasing
    ConvexHullOptimization<LL, MAXN> solver;
    for (int i = 1; i <= n; i++ ) {
        dp[i] = solver.minimize(heights[i]);
        solver.add_line(costs[i], dp[i]);
    }
    cout << dp[n] << endl;
    return 0;
}
