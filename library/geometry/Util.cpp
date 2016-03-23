///////////////////////////////////////////////////////////////////////////////
//  Geometry
///////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <cassert>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

using namespace std;

#define SZ(x) ((int)((x).size()))

// -- Adapted from Stanford ACM team notebook --
double INF = 1e100;
double EPS = 1e-9;

typedef double T;
struct Point {
    T x, y;
    Point() {}
    Point(T x, T y) : x(x), y(y) {}
    Point(const Point &p) : x(p.x), y(p.y) {}
    Point operator + (const Point &p) const { return Point(x+p.x, y+p.y); }
    Point operator - (const Point &p) const { return Point(x-p.x, y-p.y); }
    Point operator * (T c)            const { return Point(x*c,   y*c  ); }
    Point operator / (T c)            const { return Point(x/c,   y/c  ); }
    bool  operator < (const Point &p) const { return make_pair(y,x) < make_pair(p.y,p.x); }
    bool  operator ==(const Point &p) const { return make_pair(y,x) == make_pair(p.y,p.x); }
};

T cross(Point p, Point q) { return p.x*q.y-p.y*q.x; }
T area2(Point a, Point b, Point c) { return cross(a,b) + cross(b,c) + cross(c,a); }
T dot(Point p, Point q) { return p.x*q.x+p.y*q.y; }
T dist2(Point p, Point q) { return dot(p-q,p-q); }

ostream &operator<<(ostream &os, const Point &p) {
    return os << "(" << p.x << "," << p.y << ")";
}
// rotate a point CCW or CW around the origin
Point RotateCCW90(Point p) { return Point(-p.y,p.x); }
Point RotateCW90(Point p) { return Point(p.y,-p.x); }

Point RotateCCW(Point p, double t) { return Point(p.x*cos(t)-p.y*sin(t), p.x*sin(t)+p.y*cos(t)); }


////////////////////
//  ConvexHull
////////////////////

// Compute the 2D convex hull of a set of points using the monotone chain
// algorithm. Eliminate redundant points from the hull if REMOVE_REDUNDANT is
// #defined.
//
// Running time: O(n log n)
//
// INPUT: a vector of input points, unordered.
// OUTPUT: a vector of points in the convex hull, counterclockwise, starting
// with bottommost/leftmost point

#define REMOVE_REDUNDANT
#ifdef REMOVE_REDUNDANT
bool between(const Point &a, const Point &b, const Point &c) {
    return (fabs(area2(a,b,c)) < EPS && (a.x-b.x)*(c.x-b.x) <= 0 &&
            (a.y-b.y)*(c.y-b.y) <= 0);
}
#endif

void ConvexHull(vector<Point> &pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    vector<Point> up, dn;
    for (int i = 0; i < SZ(pts); i++) {
        while (SZ(up) > 1 && area2(up[SZ(up)-2], up.back(), pts[i]) >= 0) // up = concave down (top convex)
            up.pop_back();
        while (SZ(dn) > 1 && area2(dn[SZ(dn)-2], dn.back(), pts[i]) <= 0) // dn = concave up (bot convex)
            dn.pop_back();
        up.push_back(pts[i]);
		dn.push_back(pts[i]);
    }
    pts = dn;
    for (int i = (int) SZ(up) - 2; i >= 1; i--) pts.push_back(up[i]);
#ifdef REMOVE_REDUNDANT
    if (SZ(pts) <= 2) return;
    dn.clear();
    dn.push_back(pts[0]);
    dn.push_back(pts[1]);
    for (int i = 2; i < SZ(pts); i++){
        if (between(dn[SZ(dn)-2], dn[SZ(dn)-1], pts[i])) dn.pop_back();
		dn.push_back(pts[i]);
    }
    if (SZ(dn) >= 3 && between(dn.back(), dn[0], dn[1])) {
        dn[0] = dn.back();
        dn.pop_back();
    }
    pts = dn;
#endif
}

// project point c onto line through a and b
// assuming a != b
Point ProjectPointLine(Point a, Point b, Point c) {
    return a + (b-a)*dot(c-a, b-a)/dot(b-a, b-a);
}

// project point c onto line segment through a and b
Point ProjectPointSegment(Point a, Point b, Point c) {
    double r = dot(b-a,b-a);
    if (fabs(r) < EPS) return a;
    r = dot(c-a, b-a)/r;
    if (r < 0) return a;
    if (r > 1) return b;
    return a + (b-a)*r;
}
// compute distance from c to line between a and b
double DistancePointLine(Point a, Point b, Point c) {
    Point ac = c - a;
    Point ab = b - a;
    return abs(cross(ac, ab) / sqrt(dot(ab, ab)));
}
// compute distance from c to segment between a and b
double DistancePointSegment(Point a, Point b, Point c) {
    return sqrt(dist2(c, ProjectPointSegment(a, b, c)));
}
// compute distance between point (x,y,z) and plane ax+by+cz=d
double DistancePointPlane(double x, double y, double z,
                          double a, double b, double c, double d){
    return fabs(a*x+b*y+c*z-d)/sqrt(a*a+b*b+c*c);
}
// determine if lines from a to b and c to d are parallel or collinear
bool LinesParallel(Point a, Point b, Point c, Point d) {
    return fabs(cross(b-a, c-d)) < EPS;
}

bool LinesCollinear(Point a, Point b, Point c, Point d) {
    return LinesParallel(a, b, c, d) && fabs(cross(a-b, a-c)) < EPS &&
        fabs(cross(c-d, c-a)) < EPS;
}

// determine if line segment from a to b intersects with
// line segment from c to d
bool SegmentsIntersect(Point a, Point b, Point c, Point d) {
    if (LinesCollinear(a, b, c, d)) {
        if (dist2(a, c) < EPS || dist2(a, d) < EPS ||
            dist2(b, c) < EPS || dist2(b, d) < EPS)
            return true;
        if (dot(c-a, c-b) > 0 && dot(d-a, d-b) > 0 && dot(c-b, d-b) > 0)
            return false;
        return true;
    }
    if (cross(d-a, b-a) * cross(c-a, b-a) > 0) return false;
    if (cross(a-c, d-c) * cross(b-c, d-c) > 0) return false;
    return true;
}
// compute intersection of line passing through a and b
// with line passing through c and d, assuming that unique
// intersection exists; for segment intersection, check if
// segments intersect first
// if they are colinear, return only a point
Point ComputeLineIntersection(Point a, Point b, Point c, Point d) {
    b=b-a; d=c-d; c=c-a;
    assert(dot(b, b) > EPS && dot(d, d) > EPS);
    if (fabs(cross(b, d)) < EPS) { // colinear
        if (min(c.x, d.x) <= b.x && b.x <= max(c.x, d.x)) return b;
        else return a;
    }
    return a + b*cross(c, d)/cross(b, d);
}
// compute center of circle given three points
Point ComputeCircleCenter(Point a, Point b, Point c) {
    b=(a+b)/2;
    c=(a+c)/2;
    return ComputeLineIntersection(b, b+RotateCW90(a-b), c, c+RotateCW90(a-c));
}

// compute center of circle given 2 points and its radius
// idea:
//   1. Find the vector passing between a and b -> (x2-x1, y2-y1)
//   2. Find the mirror vector between a and b -> (y2-y1, x1-x2)
//   3. Normalize the mirror vector ((y2-y1)/d, (x1-x2)/d)
//   4. Use the distance between the center and the midpoint and 3.
//      to compute the center.
// Returns two possible centers
vector<Point> ComputeCircleCenter(Point a, Point b, double r) {
    vector<Point> res;
    Point mid = (a + b) / 2;
    double d = sqrt(dist2(a, b));
    double d_sq = dist2(a, b);
    if (d_sq < EPS || d_sq > 4 * r * r - EPS) return res;
    Point v_ab = b - a;
    Point v1 = RotateCW90(v_ab) / d;
    double d2 = sqrt(r*r - (d / 2) * (d / 2));
    res.push_back(mid + v1 * d2);
    res.push_back(mid - v1 * d2);
    return res;
}

// determine if point is in a possibly non-convex polygon (by William
// Randolph Franklin); returns 1 for strictly interior points, 0 for
// strictly exterior points, and 0 or 1 for the remaining points.
// Note that it is possible to convert this into an *exact* test using
// integer arithmetic by taking care of the division appropriately
// (making sure to deal with signs properly) and then by writing exact
// tests for checking point on polygon boundary
bool PointInPolygon(const vector<Point> &p, Point q) {
    bool c = 0;
    for (int i = 0; i < SZ(p); i++){
        int j = (i+1)%SZ(p);
        if ((p[i].y <= q.y && q.y < p[j].y || p[j].y <= q.y && q.y < p[i].y) &&
            q.x < p[i].x + (p[j].x - p[i].x) * (q.y - p[i].y) / (p[j].y - p[i].y))
            c = !c;
    }
    return c;
}

// determine if point is on the boundary of a polygon
bool PointOnPolygon(const vector<Point> &p, Point q) {
    for (int i = 0; i < SZ(p); i++)
        if (dist2(ProjectPointSegment(p[i], p[(i+1)%SZ(p)], q), q) < EPS)
            return true;
    return false;
}

// compute intersection of line through points a and b with
// circle centered at c with radius r > 0
vector<Point> CircleLineIntersection(Point a, Point b, Point c, double r) {
    vector<Point> ret;
    b = b-a;
    a = a-c;
    double A = dot(b, b);
    double B = dot(a, b);
    double C = dot(a, a) - r*r;
    double D = B*B - A*C;
    if (D < -EPS) return ret;
    ret.push_back(c+a+b*(-B+sqrt(D+EPS))/A);
    if (D > EPS)
	ret.push_back(c+a+b*(-B-sqrt(D))/A);
    return ret;
}

// compute intersection of circle centered at a with radius r
// with circle centered at b with radius R
vector<Point> CircleCircleIntersection(Point a, Point b, double r, double R) {
    vector<Point> ret;
    double d = sqrt(dist2(a, b));
    if (d > r+R || d+min(r, R) < max(r, R)) return ret;
    double x = (d*d-R*R+r*r)/(2*d);
    double y = sqrt(r*r-x*x);
    Point v = (b-a)/d;
    ret.push_back(a+v*x + RotateCCW90(v)*y);
    if (y > 0)
        ret.push_back(a+v*x - RotateCCW90(v)*y);
    return ret;
}

// This code computes the area or centroid of a (possibly nonconvex)
// polygon, assuming that the coordinates are listed in a clockwise or
// counterclockwise fashion. Note that the centroid is often known as
// the "center of gravity" or "center of mass".
double ComputeSignedArea(const vector<Point> &p) {
    double area = 0;
    for(int i = 0; i < SZ(p); i++) {
        int j = (i+1) % SZ(p);
        area += p[i].x*p[j].y - p[j].x*p[i].y;
    }
    return area / 2.0;
}

double ComputeArea(const vector<Point> &p) {
    return fabs(ComputeSignedArea(p));
}

Point ComputeCentroid(const vector<Point> &p) {
    Point c(0,0);
    double scale = 6.0 * ComputeSignedArea(p);
    for (int i = 0; i < SZ(p); i++){
        int j = (i+1) % SZ(p);
        c = c + (p[i]+p[j])*(p[i].x*p[j].y - p[j].x*p[i].y);
    }
    return c / scale;
}

// tests whether or not a given polygon (in CW or CCW order) is simple
bool IsSimple(const vector<Point> &p) {
    for (int i = 0; i < SZ(p); i++) {
        for (int k = i+1; k < SZ(p); k++) {
            int j = (i+1) % SZ(p);
            int l = (k+1) % SZ(p);
            if (i == l || j == k) continue;
            if (SegmentsIntersect(p[i], p[j], p[k], p[l]))
                return false;
        }
    }
    return true;
}

int main() {
	// expected: (-5,2)
	cerr << RotateCCW90(Point(2,5)) << endl;
	// expected: (5,-2)
	cerr << RotateCW90(Point(2,5)) << endl;
	// expected: (-5,2)
	cerr << RotateCCW(Point(2,5),M_PI/2) << endl;
	// expected: (5,2)
	cerr << ProjectPointLine(Point(-5,-2), Point(10,4), Point(3,7)) << endl;
	// expected: (5,2) (7.5,3) (2.5,1)
	cerr << ProjectPointSegment(Point(-5,-2), Point(10,4), Point(3,7)) << " "
	<< ProjectPointSegment(Point(7.5,3), Point(10,4), Point(3,7)) << " "
	<< ProjectPointSegment(Point(-5,-2), Point(2.5,1), Point(3,7)) << endl;
	// expected: 6.78903
	cerr << DistancePointPlane(4,-4,3,2,-2,5,-8) << endl;
	// expected: 1 0 1
	cerr << LinesParallel(Point(1,1), Point(3,5), Point(2,1), Point(4,5)) << " "
	<< LinesParallel(Point(1,1), Point(3,5), Point(2,0), Point(4,5)) << " "
	<< LinesParallel(Point(1,1), Point(3,5), Point(5,9), Point(7,13)) << endl;
	// expected: 0 0 1
	cerr << LinesCollinear(Point(1,1), Point(3,5), Point(2,1), Point(4,5)) << " "
	<< LinesCollinear(Point(1,1), Point(3,5), Point(2,0), Point(4,5)) << " "
	<< LinesCollinear(Point(1,1), Point(3,5), Point(5,9), Point(7,13)) << endl;
	// expected: 1 1 1 0
	cerr << SegmentsIntersect(Point(0,0), Point(2,4), Point(3,1), Point(-1,3)) << " "
	<< SegmentsIntersect(Point(0,0), Point(2,4), Point(4,3), Point(0,5)) << " "
	<< SegmentsIntersect(Point(0,0), Point(2,4), Point(2,-1), Point(-2,1)) << " "
	<< SegmentsIntersect(Point(0,0), Point(2,4), Point(5,5), Point(1,7)) << endl;
	// expected: (1,2)
	cerr << ComputeLineIntersection(Point(0,0), Point(2,4), Point(3,1), Point(-1,3)) << endl;
	// expected: (1,1)
	cerr << ComputeCircleCenter(Point(-3,4), Point(6,1), Point(4,5)) << endl;
	vector<Point> v;
	v.push_back(Point(0,0));
	v.push_back(Point(5,0));
	v.push_back(Point(5,5));
	v.push_back(Point(0,5));
	// expected: 1 1 1 0 0
	cerr << PointInPolygon(v, Point(2,2)) << " "
	<< PointInPolygon(v, Point(2,0)) << " "
	<< PointInPolygon(v, Point(0,2)) << " "
	<< PointInPolygon(v, Point(5,2)) << " "
	<< PointInPolygon(v, Point(2,5)) << endl;
	// expected: 0 1 1 1 1
	cerr << PointOnPolygon(v, Point(2,2)) << " "
	<< PointOnPolygon(v, Point(2,0)) << " "
	<< PointOnPolygon(v, Point(0,2)) << " "
	<< PointOnPolygon(v, Point(5,2)) << " "
	<< PointOnPolygon(v, Point(2,5)) << endl;
	// expected: (1,6)
	// (5,4) (4,5)
	// blank line
	// (4,5) (5,4)
	// blank line
	// (4,5) (5,4)
	vector<Point> u = CircleLineIntersection(Point(0,6), Point(2,6), Point(1,1), 5);
	for (int i = 0; i < SZ(u); i++) cerr << u[i] << " "; cerr << endl;
	u = CircleLineIntersection(Point(0,9), Point(9,0), Point(1,1), 5);
	for (int i = 0; i < SZ(u); i++) cerr << u[i] << " "; cerr << endl;
	u = CircleCircleIntersection(Point(1,1), Point(10,10), 5, 5);
	for (int i = 0; i < SZ(u); i++) cerr << u[i] << " "; cerr << endl;
	u = CircleCircleIntersection(Point(1,1), Point(8,8), 5, 5);
	for (int i = 0; i < SZ(u); i++) cerr << u[i] << " "; cerr << endl;
	u = CircleCircleIntersection(Point(1,1), Point(4.5,4.5), 10, sqrt(2.0)/2.0);
	for (int i = 0; i < SZ(u); i++) cerr << u[i] << " "; cerr << endl;
	u = CircleCircleIntersection(Point(1,1), Point(4.5,4.5), 5, sqrt(2.0)/2.0);
	for (int i = 0; i < SZ(u); i++) cerr << u[i] << " "; cerr << endl;
	// area should be 5.0
	// centroid should be (1.1666666, 1.166666)
	Point pa[] = { Point(0,0), Point(5,0), Point(1,1), Point(0,5) };
	vector<Point> p(pa, pa+4);
	Point c = ComputeCentroid(p);
	cerr << "Area: " << ComputeArea(p) << endl;
	cerr << "Centroid: " << c << endl;
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//  Pick's Theorem
//  On a simple polygon constructed on a grid of equal-distanced points,
//  for area A, the number of interior points I, number of boundary points B,
//  we have A = I + B / 2 - 1.
/////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Returns true if Rec(a, b) can fit in Rec(c, d)
//////////////////////////////////////////////////////////////////////////
bool can_ab_fit_in_cd(double a, double b, double c, double d){
	if(a * b > c * d) return false;
	if(a > b) swap(a, b);
	if(c > d) swap(c, d);
	if(a > c) return false;
	if(a <= c && b <= d) return true;
	double dis = sqrt(a * a + b * b);
	double P = asin(c / dis);
	double Q = asin(a / dis);
	double theta = P - Q;
	double dd = b * cos(theta) + a * sin(theta);
	return dd <= d;
}

