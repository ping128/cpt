/////////////////////////////////////////////////////////////////////////////////////////////
//  Aho-Corasick Algorithm
//  Prefix tree with suffix link
//
//  Link: http://acm.timus.ru/problem.aspx?space=1&num=1269
/////////////////////////////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>

using namespace std;

#define SZ(x) ((int)(x).size())
#define PB(x) push_back(x)
#define MEMSET(x,v) memset(x,v,sizeof(x))
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))
#define x first
#define y second
#define INF (0x3f3f3f3f)

typedef long long LL;
typedef pair<int, int> P2;
template<class A, class B> inline bool mina(A &x, B y) {return (x > y)?(x=y,1):0;}
template<class A, class B> inline bool maxa(A &x, B y) {return (x < y)?(x=y,1):0;}

class Node {
public:
    char c;
    int parent;
    bool isWord;
    int suffLink;
    int num_child;
    int *children;
    int len = 0;
    Node () {
        parent = -1;
        isWord = false;
        suffLink = -1;
        num_child = 0;
        children = NULL;
        len = -1;
    }
};

class AhoCorasick {
public:
    Node *nodes;
    int N;
    AhoCorasick (int maxNode) {
        N = 1;
        nodes = new Node[maxNode];
        nodes[0].suffLink = 0;
    }
    void add_string(char *s) {
        int cur = 0;
        int len = strlen(s);
        REP(i, len) {
            int nxt = -1;
            REP(j, nodes[cur].num_child) {
                if (nodes[nodes[cur].children[j]].c == s[i]) {
                    nxt = nodes[cur].children[j];
                    break;
                }
            }
            if (nxt == -1) {
                int *nc = new int[nodes[cur].num_child + 1];
                REP(j, nodes[cur].num_child) nc[j] = nodes[cur].children[j];
                nc[nodes[cur].num_child] = N++;
                delete [] nodes[cur].children;
                nodes[cur].children = nc;
                nodes[cur].num_child++;
                nodes[N - 1].parent = cur;
                nodes[N - 1].c = s[i];
                nxt = N - 1;
            }
            cur = nxt;
        }
        nodes[cur].isWord = true;
        nodes[cur].len = len;
    }

    int find_suffix_link(int cur) {
        if (nodes[cur].suffLink == -1) {
            if (nodes[cur].parent == 0) return 0;
            return nodes[cur].suffLink = transition(find_suffix_link(nodes[cur].parent), nodes[cur].c);
        }
        return nodes[cur].suffLink;
    }
    int transition(int cur, char c) {
        REP(i, nodes[cur].num_child) {
            if (nodes[nodes[cur].children[i]].c == c) {
                return nodes[cur].children[i];
            }
        }
        if (cur == 0) return 0;
        return transition(find_suffix_link(cur), c);
    }
    // Find the first word in the tree that matches the given string s
    int find(char *s) {
        int ans = -1;
        int cur = 0;
        int len = strlen(s);
        REP(i, len) {
            cur = transition(cur, s[i]);
            // Check if this word is a substring of the given text
            int temp = cur;
            while (temp && nodes[temp].suffLink == -1) {
                if (nodes[temp].isWord) {
                    int dd = i - nodes[temp].len + 1;
                    if (ans == -1 || dd < ans) {
                        ans = dd;
                    }
                }
                temp = find_suffix_link(temp);
            }
        }
        return ans;
    }
};

const int MAXL = 1e6 + 5;
char S[MAXL];

int main() {
    int N;
    scanf("%d", &N);
    AhoCorasick aho(110000);
    gets(S);
    REP(i, N) {
        gets(S);
        aho.add_string(S);
    }
   // REP(i, aho.N) {
   //     cout << i << "#" << aho.find_suffix_link(i) << endl;
   // }
    int M;
    scanf("%d", &M);
    gets(S);
    REP(i, M) {
        gets(S);
        int temp = aho.find(S);
        if (temp != -1) {
            printf("%d %d\n", i + 1, temp + 1);
            return 0;
        }
    }
    printf("Passed\n");
    return 0;
}
