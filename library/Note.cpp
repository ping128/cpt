///////////////////////////////////////////////////////////////////////////////
//  C++ Syntax
//
//  Note:
//    alias g++="g++ -O2 -std=gnu++0x -Wall -Wshadow"
///////////////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>

using namespace std;

typedef pair<int, int> PII;
// Set Compare Function Example
typedef struct shape_st {
    PII points[4];
    shape_st(){}
    shape_st(PII in[4]){
        for(int i = 0; i < 4; i++ )
            points[i] = in[i];
        sort(points, points + 4);
    }

}Shape;

struct ShapeCompare{
    bool operator() (const Shape s1, const Shape s2) const {
        for(int i = 0; i < 4; i++ ){
            if(s1.points[i] != s2.points[i])
                return s1.points[i] < s2.points[i];
        }
        return false;
    }
};

set<Shape, ShapeCompare> setShape;

int main() {
    char temp[100];
    string temp2;
    // scan until reaching \n
    scanf("%[^\n]", temp);
    // scan until reaching :
    scanf("%[^:]", temp);
    // get a line for string
    getline(cin, temp2);
    // int to string
    cout << to_string(1) << endl;
    // string to int, stod, stol, stold, stoll
    // int stoi (const string&  str, size_t* idx = 0, int base = 10);
    cout << stoi("1") << endl;
    // 1000 (base 2) -> 8
    cout << stoi("1000", NULL, 2) << endl;
    // string to char[]
    string s = "123";
    strcpy(temp, s.c_str());
    cout << temp << endl;
    // char[] to string
    char t[100] = "123";
    cout << string(t) << endl;
    return 0;
}
