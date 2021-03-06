///////////////////////////////////////////////////////////////////////////////
//  Range Minimum Query
//  <O(N lg N, lg N or O(1)>
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

class RangeMinimumQuery {
public:
    vector<vector<int> > M;
    vector<int> A;
    int N;
    RangeMinimumQuery(const vector<int>& in) {
        N = in.size();
        M = vector<vector<int> >(N + 5);
        A = vector<int> (N + 5);
        int b; for (b = 0; 1<<b <= N; b++);
        for (int i = 0; i < N + 5; i++)
            M[i] = vector<int> (b);

        //initialize M for the intervals with length 1
        for (int i = 0; i < N; i++) {
            M[i][0] = i;
            A[i] = in[i];
        }
        //compute values from smaller to bigger intervals
        for (int j = 1; 1<<j <= N; j++){
            for (int i = 0; i + (1<<j) - 1 < N; i++)
                if (A[M[i][j - 1]] < A[M[i + (1 << (j - 1))][j - 1]]) M[i][j] = M[i][j - 1];
                else M[i][j] = M[i + (1 << (j - 1))][j - 1];
        }
    }

    // returns min(A[left...right])
    // also can be modified to return the index
    int get_min(int left, int right) {
        int num = right - left + 1;
        int e = 1, k = 0;
        while (e * 2 <= num) e *= 2, k++;
        int temp = right - e + 1;
        if (A[M[left][k]] <= A[M[temp][k]]) return A[M[left][k]];
        else return A[M[temp][k]];
    }
};

int bruteforce_rmq(vector<int>& a, int left, int right) {
    int ret = a[left];
    for (int i = left + 1; i <= right; i++) {
        ret = min(ret, a[i]);
    }
    return ret;
}

int main() {
    int N = 15;
    vector<int> in = {-1, 4, 1, 6, 2, -10, 11, 3, 7, 100, 4, -123123123, 19, 1, -5};
    RangeMinimumQuery rmq(in);

    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            assert(rmq.get_min(i, j) == bruteforce_rmq(in, i, j));
        }
    }
    return 0;
}
