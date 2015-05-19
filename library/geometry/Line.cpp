///////////////////////////////////////////////////////////////////////////////
//  Line
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>

using namespace std;

class Line {
public:
    typedef long long LL;
    LL a, b, c;
    Line() {}
    Line(LL a_, LL b_, LL c_) { init(a_, b_, c_); }
    Line(LL x1, LL y1, LL x2, LL y2) {
        LL A = y2 - y1, B = x1 - x2, C = -A * x1 - B * y1;
        init(A, B, C);
    }
    bool operator == (const Line &o) const { return a == o.a && b == o.b && c == o.c; }
    bool operator != (const Line &o) const { return !(*this == o); }
    bool operator < (const Line &o) const {
        if(a != o.a) return a < o.a;
        if(b != o.b) return b < o.b;
        return c < o.c;
    }
private:
    void init(LL A, LL B, LL C){
        a = A, b = B, c = C;
        if(a < 0 || (a == 0 && b < 0)) a = -a, b = -b, c = -c;
        reduce();
    }
    void reduce(){
        LL g = gcd(labs(a), gcd(labs(b), labs(c)));
        a /= g, b /= g, c /= g;       
    }
    LL gcd(LL x, LL y) {
        if (x == 0 || y == 0) return x + y;
        if (x % y == 0) return y;
        return gcd(y, x % y);
    }
};
