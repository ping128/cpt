/////////////////////////////////////////////////////////////////////////////
// Maximum Rectangle Under Histogram
//
// # #    # # 
// ### -> MMM -> ans: 3
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

#include <cassert>

using namespace std;

#define x first
#define y second
#define SZ(x) ((int)((x).size()))

template <class Value_t>
class MaxRecUnderHistogram {
    typedef pair<Value_t, int> PVI;
    typedef vector<int> VI;
public:
    Value_t get_max(vector<Value_t> &h) {
        int N = SZ(h); VI left(N), right(N);
        stack<PVI> l_sk;
        for (int i = 0; i < N; i++) {
            int x = i;
            while (!l_sk.empty()) {
                if (l_sk.top().x >= h[i]) { x = l_sk.top().y; l_sk.pop(); }
                else break;
            }
            left[i] = x; l_sk.push(PVI(h[i], x));
        }
        stack<PVI> r_sk;
        for (int i = N - 1; i >= 0; i--) {
            int x = i;
            while (!r_sk.empty()) {
                if (r_sk.top().x >= h[i]) { x = r_sk.top().y; r_sk.pop(); }
                else break;
            }
            right[i] = x; r_sk.push(PVI(h[i], x));
        }
        Value_t ret = 0;
        for (int i = 0; i < N; i++) {
            ret = max(ret, h[i] * (right[i] - left[i] + 1));
        }
        return ret;
    }
};


int main() {
    MaxRecUnderHistogram<int> max_his;
    vector<int> v = vector<int>({2, 1, 2});
    assert(max_his.get_max(v) == 3);

    v = vector<int>({4, 2, 1, 2, 1, 4, 2, 3, 2, 2, 1, 9});
    assert(max_his.get_max(v) == 12);

    v = vector<int>({4, 2, 1, 2, 0, 4, 2, 3, 2, 2, 1, 9});
    assert(max_his.get_max(v) == 10);

    return 0;
}
