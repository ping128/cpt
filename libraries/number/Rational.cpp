#include <iostream>
#include <algorithm>

#include <cassert>

using namespace std;

#define x first
#define y second
#define SZ(x) ((int)((x).size()))

typedef long long LL;
typedef pair<int, int> PII; typedef pair<PII, int> PII2;
typedef vector<int> VI; typedef vector<VI> VVI;

class Rational {
public:
    LL p, q;
    Rational() {}
    Rational(LL p_): p(p_), q(1) {}
    Rational(LL p_, LL q_): p(p_), q(q_) { reduce(); }
    bool operator == (const Rational& rhs) const { return p == rhs.p && q == rhs.q; }
    bool operator != (const Rational& rhs) const { return p != rhs.p || q != rhs.q; }
    bool operator < (const Rational& rhs) const { return p * rhs.q < rhs.p * q; }
    Rational operator + (const Rational& rhs) const { return Rational(p * rhs.q + q * rhs.p, q * rhs.q); }
    Rational operator - (const Rational& rhs) const { return Rational(p * rhs.q - q * rhs.p, q * rhs.q); }
    Rational operator * (const Rational& rhs) const { return Rational(p * rhs.p, q * rhs.q); }
    Rational operator / (const Rational& rhs) const { return Rational(p * rhs.q, q * rhs.p); }
    inline friend ostream& operator << (ostream &out, Rational m) { return out << m.p << "/" << m.q; }
private:
    LL gcd(LL a, LL b) { if (b == 0) return a; return gcd(b, a % b); }
    void reduce() {
        if (q < 0) { p *= -1; q *= -1; }
        LL t = gcd((p >= 0 ? p : -p), q);
        p /= t; q /= t;
    }
};

int main() {
    Rational x(4, 6);
    Rational y(5, 6);
    cout << x << endl;
    cout << y << endl;
    cout << x + y << endl;
    cout << x - y << endl;
    Rational z(0, -1);
    cout << z << endl;
    return 0;
}
