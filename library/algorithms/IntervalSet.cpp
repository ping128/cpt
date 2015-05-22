////////////////////////////////////////////////////////////
//  Interval Set
//  All intervals are [a, b)
//  For Double type, use EPS to deal with precision error
////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

#include <cassert>

using namespace std;

#define x first
#define y second
#define SZ(x) ((int)((x).size()))

template<class T>
class IntervalSet {
public:
    typedef pair<T, T> Inter;

    set<Inter> S;
    IntervalSet () {}
    IntervalSet (vector<Inter> &v) { for (auto e : v) { assert(e.x < e.y); S.insert(e); } }
    Inter get_intersection() {
        assert(SZ(S) > 0);
        Inter ret = *S.begin();
        for (auto e : S) { ret.x = max(ret.x, e.x); ret.y = min(ret.y, e.y); }
        return ret;
    }
    long long length() {
        long long ret = 0;
        for (auto e : S) ret += e.y - e.x;
        return ret;
    }
    void union_intervals() {
        if (SZ(S) == 0) return ;
        vector<pair<T, int>> events;
        for (auto e : S) { events.push_back(make_pair(e.x, -1)); events.push_back(make_pair(e.y, 1)); }
        S.clear();
        sort(events.begin(), events.end());
        int open_interval = 1;
        T last_open = events[0].x;
        for (int i = 1; i < SZ(events); i++) {
            if (!open_interval) last_open = events[i].x;
            open_interval -= events[i].y;
            if (!open_interval) S.insert(Inter(last_open, events[i].x));
        }
    }
    void add(T a, T b) {
        assert(a < b);
        Inter inter = Inter(a, b);
        if (!S.empty()) {
            auto it1 = S.lower_bound(Inter(inter.x, inter.x));
             if (it1 != S.begin()) {
                auto it2 = it1; it2--;
                if (it2->y >= inter.x) { inter = union_overlap(inter, *it2); S.erase(it2); }
            }
            while (it1 != S.end()) {
                if (inter.y < it1->x) break;
                inter = union_overlap(inter, *it1);
                auto temp = it1; temp++; S.erase(it1); it1 = temp;
            }
        }
        S.insert(inter);
    }
    void print() {
        cout << "{"; for (auto e : S) cout << " (" << e.x << "," << e.y << ")"; cout << " }" << endl;
    }
private:
    Inter union_overlap(Inter a, Inter b) { return Inter(min(a.x, b.x), max(a.y, b.y)); }
};

typedef pair<int, int> PII;
int main () {
    IntervalSet<int> myset;
    myset.add(1, 2); myset.print();

    myset.add(2, 5); myset.print();
    myset.add(1, 2); myset.print();
    myset.add(5, 6); myset.print();

    myset.add(10, 12); myset.print();
    myset.add(7, 10); myset.print();
    myset.add(0, 20); myset.print();
    myset.add(20, 22); myset.print();
    cout << myset.length() << endl;

    vector<PII> v{PII(1, 2), PII(3, 5), PII(5, 7), PII(12, 15), PII(10, 13), PII(5, 8)};
    IntervalSet<int> myset2 (v);
    myset2.union_intervals();
    myset2.print();
    cout << myset2.length() << endl;
    return 0;
}
