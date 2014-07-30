////////////////////////////////////////////////////////////////////////////////
// Prime Generator
// Use Sieve Eratosthenes - can find all the primes within range (1, 2^31 - 1)
// Reference: http://zobayer.blogspot.in/2009/09/segmented-sieve.html
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <iostream>
#include <cassert>

using namespace std;

template <int MAX_RANGE>
class PrimeGenerator {
#define SQ(x) ((x)*(x))
#define MSET(x,v) memset(x,v,sizeof(x))
#define CHKC(x,n) (x[n>>6]&(1<<((n>>1)&31)))
#define SETC(x,n) (x[n>>6]|=(1<<((n>>1)&31)))
    static const int MAX = 46656; // around sqrt(2^31 - 1)
    static const int LMT = 216;  // sqrt(MAX)
    static const int LEN = 4830; // number of necessary primes (primes under MAX)
    unsigned base[MAX/64], segment[MAX_RANGE/64], primes[LEN];    
public:
    PrimeGenerator() { internal_sieve(); }
    // Returns the number of primes within range [a, b] and marks them in segment[]
    // Stores all the primes in the user array (it not NULL)
    int segmented_sieve(int a, int b, int *user) {
        unsigned i, j, k, cnt = (a <= 2 && 2 <= b)? 1 : 0;
        if (a <= 2 && 2 <= b) { if (user) user[cnt - 1] = 2; }
        if (b < 2) return 0;
        if (a < 3) a = 3;
        if (a % 2 == 0) a++;
        MSET(segment, 0);
        for (i = 0; SQ(primes[i]) <= b; i++) {
            j = primes[i] * ((a + primes[i] - 1) / primes[i]);
            if (j % 2 == 0) j += primes[i];
            for (k = primes[i]<<1; j <= b; j += k)
                if (j != primes[i]) SETC(segment, (j - a));
        }
        if (b - a >= 0) {
            for(i = 0; i <= b - a; i += 2) {
                if(!CHKC(segment, i)) {
                    if (user) user[cnt] = i + a;
                    cnt++;
                }
            }
        }
        return cnt;
    }
private:
    // Generates all the necessary prime numbers and marks them in base[]
    void internal_sieve() {
        unsigned i, j, k;
        for (i = 3; i < LMT; i += 2)
            if (!CHKC(base, i)) for (j = i * i, k = i<<1; j < MAX; j += k) SETC(base, j);
        for (i = 3, j = 0; i < MAX; i += 2) if (!CHKC(base, i)) primes[j++] = i;
    }
};

int primes[1000005];

int main() {
    PrimeGenerator<10000000> pg; // range 10,000,000 for testing - might wanna change it
    int num_p = pg.segmented_sieve(1, 100, primes);
    for (int i = 0; i < num_p; i++) {
        cout << primes[i] << endl;
    }
    cout << "count: " << num_p << endl;    

    assert(pg.segmented_sieve(2, 2, NULL) == 1);
    assert(pg.segmented_sieve(2, 3, NULL) == 2);
    assert(pg.segmented_sieve(5, 10, NULL) == 2);
    assert(pg.segmented_sieve(1, 10, NULL) == 4);
    assert(pg.segmented_sieve(1, 50, NULL) == 15);
    assert(pg.segmented_sieve(1, 100, NULL) == 25);
    assert(pg.segmented_sieve(1, 1000000, NULL) == 78498);
    assert(pg.segmented_sieve(1, 10000000, NULL) == 664579);
    assert(pg.segmented_sieve(2000000000, 2000000000, NULL) == 0);
    assert(pg.segmented_sieve(2000000001, 2000000001, NULL) == 0);
    assert(pg.segmented_sieve(2000000000, 2010000000, NULL) == 467612);

    return 0;
}
