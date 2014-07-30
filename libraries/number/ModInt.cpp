///////////////////////////////////////////////////////////////////////////////
//  Modulo Integer
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

using namespace std;

template<typename Int,Int mod>
class ModInt {
public:
    static const Int Mod = mod;
    Int x;
    ModInt(): x(0) {}
    ModInt(int a) { Int t = a % mod; if(t < 0) t += mod; x = t; }
    ModInt(long long a) { Int t = a % mod; if(t < 0) t += mod; x = t; }
    Int get() const { return x; }
    
    ModInt &operator += (ModInt that) { if((x += that.x) >= mod) x -= mod; return *this; }
    ModInt &operator -= (ModInt that) { if((x += mod - that.x) >= mod) x -= mod; return *this; }
    ModInt &operator *= (ModInt that) { x = (long long)(x) * that.x % mod; return *this; }
    bool operator == (ModInt that) const { return x == that.x; }
    ModInt operator + (ModInt that) const { return ModInt(*this) += that; }
    ModInt operator - (ModInt that) const { return ModInt(*this) -= that; }
    ModInt operator * (ModInt that) const { return ModInt(*this) *= that; }
    ModInt operator - () const { return ModInt(-this->x); }
    inline friend ostream& operator << (ostream &out, ModInt m) {return out << m.x;}
    
    ModInt power(long long k) const {
        ModInt r(1); ModInt b = *this;
        if (k <= 0) return r; while (k) { if (k & 1) r *= b; b *= b; k >>= 1; }
        return r;
    }
    ModInt inverse() const {
        long long a = x, b = mod, u = 1, v = 0;
        while(b) { long long t = a / b; a -= t * b; swap(a, b); u -= t * v; swap(u, v); }
        return ModInt(u);
    }
};

#define MOD (1000000007)
typedef ModInt<int, MOD> Mint;

int main() {
    Mint a(7);
    for (int i = -10; i < 10; i++) {
        cout << a << " power of " << i << " : " << a.power(i) << endl;
        Mint t(i);
        cout << t << " (inverse): " << t.inverse() << endl;
    }
    return 0;
}
