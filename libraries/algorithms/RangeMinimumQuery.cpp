///////////////////////////////////////////////////////////////////////////////
//  Range Minimum Query
//  <O(N lg N, lg N or O(1)>
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

template<class T>
class RangeMinimumQuery {
public:
    vector<vector<T> > M;
    vector<T> A;
    int N;
    RangeMinimumQuery(T in[], int n) {
        N = n;
        M = vector<vector<T> >(N + 5);
        A = vector<T> (N + 5);
        for (int i = 0; i < N + 5; i++)
            M[i] = vector<T> (25);

        //initialize M for the intervals with length 1
        for(int i = 0; i < N; i++ ) {
            M[i][0] = i;
            A[i] = in[i];
        }
        //compute values from smaller to bigger intervals
        for(int j = 1; 1<<j <= N; j++ ){
            for(int i = 0; i + (1<<j) - 1 < N; i++ )
                if(A[M[i][j - 1]] < A[M[i + (1 << (j - 1))][j - 1]]) M[i][j] = M[i][j - 1];
                else M[i][j] = M[i + (1 << (j - 1))][j - 1];
        }
    }

    // returns min(A[left...right])
    // also can be modified to return the index
    int get_min(int left, int right) {
        int num = right - left + 1;
        int e = 1, k = 0;
        while(e * 2 <= num) e *= 2, k++;
        int temp = right - e + 1;
        if(A[M[left][k]] <= A[M[temp][k]]) return A[M[left][k]];
        else return A[M[temp][k]];
    }
};

int main() {
    int in[] = {-1, 4, 1, 6, 2, -10, 11, 3, 7, 100};
    RangeMinimumQuery<int> rmq(in, 10);
    
    assert(rmq.get_min(0, 0) == -1);
    assert(rmq.get_min(0, 9) == -10);
    assert(rmq.get_min(0, 2) == -1);
    assert(rmq.get_min(4, 9) == -10);
    assert(rmq.get_min(3, 3) == 6);
    assert(rmq.get_min(6, 8) == 3);
    return 0;
}
