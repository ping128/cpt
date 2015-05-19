///////////////////////////////////////////////////////////////////////////////
//  Manacher's algorithm
//  - Finding the longest substring that is also a palindrome in linear time
//  - It also finds the length of longest palindrome "centered" at each position
/////////////////////////////////////////////////////////////////////////////////

// Transform S into T.
// For example, S = "abba", T = "^#a#b#b#a#$".
// ^ and $ signs are sentinels appended to each end to avoid bounds checking
// 
// For example: S = "abaaba", T = "^#a#b#a#a#b#a#$".
// T = ^ # a # b # a # a # b # a # $
// P = 0 0 1 0 3 0 1 6 1 0 3 0 1 0 0

#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

class ManacherAlgorithm {
public:
    char *T;
    string s;
    
    ManacherAlgorithm(string _s) {
        s = _s;
        int n = s.size();
        int n2 = 0;
        T = new char[2 * n + 5];
        T[n2++] = '^';
        if(n == 0){
            T[n2++] = '$';
            T[n2] = '\0';
        } else {
            for (int i = 0; i < n; i++ ){
                T[n2++] = '#';
                T[n2++] = s[i];
            }
            T[n2++] = '#';
            T[n2++] = '$';
            T[n2] = '\0';
        }
    }
 
    string get_longest_palindrome() {
        int n = strlen(T);
        int *P = new int[n];
        int C = 0, R = 0;
        for (int i = 1; i < n - 1; i++ ){
            int i_mirror = 2 * C - i; // equals to i' = C - (i-C)
    
            P[i] = (R > i) ? min(R-i, P[i_mirror]) : 0;
    
            // Attempt to expand palindrome centered at i
            while(T[i + 1 + P[i]] == T[i - 1 - P[i]])
                P[i]++;
 
            // If palindrome centered at i expand past R,
            // adjust center based on expanded palindrome.
            if(i + P[i] > R){
                C = i;
                R = i + P[i];
            }
        }
 
        // Find the maximum element in P.
        int maxLen = 0;
        int centerIndex = 0;
        for (int i = 1; i < n-1; i++){
            if (P[i] > maxLen){
                maxLen = P[i];
                centerIndex = i;
            }
        }

        delete[] P;
        return s.substr((centerIndex - 1 - maxLen)/2, maxLen);
    }
};

int main() {
    ManacherAlgorithm solver("abaaba");
    cout << solver.get_longest_palindrome() << endl;

    solver = ManacherAlgorithm("abba");
    cout << solver.get_longest_palindrome() << endl;

    solver = ManacherAlgorithm("xabac");
    cout << solver.get_longest_palindrome() << endl;

    return 0;
}
