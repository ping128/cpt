///////////////////////////////////////////////////////////////////////////////
//  Closest Pair Example
//  Link: http://www.spoj.com/problems/CLOPPAIR/
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>

using namespace std;


typedef long long LL;
#define INF (1LL<<62)

class ClosestPair {
public:
    typedef pair<LL, LL> Point;
    typedef pair<Point, Point> PPP;
    typedef pair<double, pair<Point, Point> > PDPP;
    typedef pair<LL, pair<Point, Point> > PLPP;
    typedef pair<LL, LL> PLL;
    vector<Point> all_points;
    int N;

    ClosestPair () {
        N = 0;
    }
    
    void add_point(Point p) {
        N++; all_points.push_back(p);
    }

    // Return the closest distance and the pair of the two points
    PDPP get_closest_pair() {
        assert(N >= 2);
        sort(all_points.begin(), all_points.end());
        PLPP ret = get_closest_pair(0, N - 1);
        return PDPP(sqrt(ret.first), ret.second);
    }

private:
    LL dist2(int id1, int id2) {
        LL dx = all_points[id1].first - all_points[id2].first;
        LL dy = all_points[id1].second - all_points[id2].second;
        return dx * dx + dy * dy;
    }

    PLPP get_closest_pair(int left, int right) {
        if (left >= right) {
            return PLPP(INF, PPP(Point(0, 0), Point(0, 0)));
        }
        int mid = (left + right) / 2;
        PLPP ret_left = get_closest_pair(left, mid);
        PLPP ret_right = get_closest_pair(mid + 1, right);
        LL minn = INF;

        PPP ret;
        if (ret_left.first < minn) {
            minn = ret_left.first;
            ret = ret_left.second;
        }
        if (ret_right.first < minn) {
            minn = ret_right.first;
            ret = ret_right.second;
        }
        double minn2 = sqrt(minn);
        
        vector<PLL> left_part;
        vector<PLL> right_part;
        for (int i = left; i <= right; i++) {
            if (i <= mid) {
                if (all_points[mid].first - all_points[i].first <= minn2) {
                    // ....|.X..|....|....
                    left_part.push_back(PLL(all_points[i].second, i));
                }
            } else {
                if (all_points[i].first - all_points[mid].first <= minn2) {
                    // ....|....|.X..|...
                    right_part.push_back(PLL(all_points[i].second, i));
                }
            }
        }

        // sort by y-coordinates
        sort(left_part.begin(), left_part.end());
        sort(right_part.begin(), right_part.end());
    
        int left_sz = left_part.size();
        int right_sz = right_part.size();
        int lower = 0, upper = 0;
        for (int i = 0; i < left_sz; i++) {
            while (upper < right_sz && all_points[right_part[upper].second].second - all_points[left_part[i].second].second <= minn2) upper++;
            while (lower < right_sz && all_points[left_part[i].second].second - all_points[right_part[lower].second].second > minn2) lower++;
            for (int j = lower; j < upper; j++) {
                LL temp = dist2(left_part[i].second, right_part[j].second);
                if (temp < minn) {
                    minn = temp;
                    ret = PPP(all_points[left_part[i].second], all_points[right_part[j].second]);
                }
            }
        }
        return PLPP(minn, ret);
    }
};
    
#define MAXN (50005)
typedef long long LL;
typedef pair<LL, LL> Point;

Point in[MAXN];
    
int main() {
    int N;
    cin >> N;
    ClosestPair solver;
    int x, y;
    for (int i = 0; i < N; i++) {
        scanf("%d %d", &x, &y);
        in[i] = Point(x, y);
        solver.add_point(in[i]);
    }

    pair<double, pair<Point, Point> > ret = solver.get_closest_pair();

    int ans1, ans2;
    for (int i = 0; i < N; i++) {
        if (in[i] == ret.second.first) ans1 = i;
        if (in[i] == ret.second.second) ans2 = i;
    }
    if (ans1 > ans2) swap(ans1, ans2);
    printf("%d %d %.6lf\n", ans1, ans2, ret.first);
    return 0;
}
