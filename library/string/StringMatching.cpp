///////////////////////////////////////////////////////////////////////////////
//  Z-functions
/////////////////////////////////////////////////////////////////////////////// 

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// z-functions: return z, z[i] is the length of the longest substring
//              starting from S[i] which is also a prefix of S

vector<int> get_ZFunction(string s){
    int len = s.length();
    vector<int> z(len);
    int L = 0, R = 0;
    for(int i = 1; i < len; i++ ){
        if (i > R){
            L = R = i;
            while (R < len && s[R-L] == s[R]) R++;
            z[i] = R - L; R--;
        } else {
            int k = i - L;
            if(z[k] < R - i + 1) z[i] = z[k];
            else {
                L = i;
                while (R < len && s[R - L] == s[R]) R++;
                z[i] = R - L; R--;
            }
        }
    }
    z[0] = len;
    return z;
}


///////////////////////////////////////////////////////////////////////////////
//  Knuth-Morris-Pratt
///////////////////////////////////////////////////////////////////////////////

/* 
  Searches for the string key in the string s (of length k). Returns the 
  0-based index of the first match (k if no match is found). Algorithm 
  runs in O(k) time. 
*/ 

#include <cassert>

using namespace std;
typedef vector<int> VI;

class KnuthMorrisPratt {
public:
    void build_table(string& key, VI& t){
        int sz = key.size();
        t = VI(sz);
        int i = 2, j = 0;
        t[0] = -1; t[1] = 0;
        while (i < sz) {
            if (key[i - 1] == key[j]) { t[i] = j + 1; i++; j++; }
            else if (j > 0) j = t[j];
            else { t[i] = 0; i++; }
        }
    }

    // return the index of the first match, or the length of s if key is not found
    int KMP(string s, string key){
        int m = 0, i = 0;
        VI t;
        build_table(key, t);
        int sz_s = s.size();
        int sz_w = key.size();
        while (m+i < sz_s) {
            if (key[i] == s[m+i]) {
                i++;
                if (i == sz_w) return m;
            } else {
                m += i - t[i];
                if (i > 0) i = t[i];
            }
        }
        return s.length();
    }
};

int main(){
    KnuthMorrisPratt solve;
    assert(solve.KMP("win", "I wanna win.") == 3);
    assert(solve.KMP("I wanna win.", "win") == 8);
    return 0;
}  

///////////////////////////////////////////////////////////////////////////////
//  Prefix Function
//  Given a string. Return an array of numbers, 
//  where is defined as follows: it is a maximum length of the longest 
//  proper suffix substring that matches the prefix (suffix own - so not 
//  the entire line). In particular, the value is set equal to zero.
//  For example, the string "abcabcd" prefix function is: [1, 0, 0, 1, 2, 3, 0]
///////////////////////////////////////////////////////////////////////////////

vector<int> prefix_function (string s){
    int n = (int) s.length();
    vector<int> pi (n);
    for (int i=1; i<n; ++i){
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j])
            j = pi[j-1];
        if (s[i] == s[j])  ++j;
        pi[i] = j;
    }
    return pi;
}
