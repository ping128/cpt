////////////////////////////////////////////////////////////
//  Interval Set
//  All intervals are [a, b).
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
typedef pair<int, int> PII;
typedef vector<PII> VP;

class IntervalSet {
public:
    set<PII> S;
    IntervalSet () {}
    IntervalSet (VP &v) { for (auto e : v) { assert(e.x < e.y); S.insert(e); } }
    PII get_intersection() {
        assert(SZ(S) > 0);
        PII ret = *S.begin();
        for (auto e : S) { ret.x = max(ret.x, e.x); ret.y = min(ret.y, e.y); }
        return ret;
    }
    long long length() {
        long long ret = 0;
        for (auto e : S) ret += e.y - e.x;
        return ret;
    }
    void union_intervals() {
        VP events;
        for (auto e : S) { events.push_back(PII(e.x, -1)); events.push_back(PII(e.y, 1)); }
        S.clear();
        sort(events.begin(), events.end());
        int open_interval = 1, last_open = events[0].x;
        for (int i = 1; i < SZ(events); i++) {
            if (!open_interval) last_open = events[i].x;
            open_interval -= events[i].y;
            if (!open_interval) S.insert(PII(last_open, events[i].x));
        }
    }
    void add(PII inter) {
        assert(inter.x < inter.y);
        if (!S.empty()) {
            auto it1 = S.lower_bound(PII(inter.x, inter.x));
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
        printf("{"); for (auto e : S) printf(" (%d,%d)", e.x, e.y); printf(" }\n");
    }
private:
    PII union_overlap(PII a, PII b) { return PII(min(a.x, b.x), max(a.y, b.y)); }
};

int main () {
    IntervalSet myset;
    myset.add(PII(1, 2)); myset.print();
    
    myset.add(PII(2, 5)); myset.print();
    myset.add(PII(1, 2)); myset.print();
    myset.add(PII(5, 6)); myset.print();

    myset.add(PII(10, 12)); myset.print();
    myset.add(PII(7, 10)); myset.print();
    myset.add(PII(0, 20)); myset.print();
    myset.add(PII(20, 22)); myset.print();
    cout << myset.length() << endl;

    vector<PII> v{PII(1, 2), PII(3, 5), PII(5, 7), PII(12, 15), PII(10, 13), PII(5, 8)};
    IntervalSet myset2 (v);
    myset2.union_intervals();
    myset2.print();
    cout << myset2.length() << endl;
    return 0;
}
