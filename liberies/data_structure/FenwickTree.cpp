///////////////////////////////////////////////////////////////////////////////
//  Fenwick Tree
//  An example of sum fenwick tree
//
//  Note: use map<int, T> instead of vector<T> for Sparse Fenwick Tree
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <vector>
#include <iostream>

using namespace std;

template<class T>
class FenwickTree {
public:
    vector<T> data;
    int N;

    // Indices are 1-based.
    FenwickTree(int n) {
        N = n;
        data = vector<T>(N + 5);
    }

    // Return the prefix sum XXXXXXat....
    T query(int at) {
        T res = 0;
        for (int i = at; i; i -= (i & (-i)))
            res += data[i];
        return res;
    }

    // Add a value to a single element
    void update(int at, T value) {
        for (int i = at; i <= N; i += (i & (-i)))
            data[i] += value;
    }
};

int main() {
    FenwickTree<int> tree(10);
    int sum = 0;
    for (int i = 1; i <= 10; i++) tree.update(i, i);
    for (int i = 1; i <= 10; i++) {
        sum += i;
        assert(tree.query(i) == sum);
    }
    for (int i = 1; i <= 10; i++) tree.update(i, i * i);
    sum = 0;
    for (int i = 1; i <= 10; i++) {
        sum += i * i + i;
        assert(tree.query(i) == sum);
    }
    return 0;
}
    
    

