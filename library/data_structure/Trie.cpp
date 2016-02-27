/////////////////////////////////////////////////////////
//  Trie
//  Link: http://codeforces.com/contest/455/problem/B
/////////////////////////////////////////////////////////

#include <bits/stdc++.h>

using namespace std;
class TrieNode {
public:
    TrieNode *next[26];
    // any other fields here
    int val; // 0 W, 1 L
    int val2;
    TrieNode () {
        for (int i = 0; i < 26; i++) {
            next[i] = NULL;
        }
        val = val2 = 0;
    }
};
typedef TrieNode* TrieNodePtr;

class Trie {
public:
    TrieNodePtr root;
    Trie() { root = new TrieNode(); }
    void insert(string &s) {
        TrieNodePtr cur = root;
        for (int j = 0; j < (int)s.size(); j++) {
            insert(cur, s[j]);
        }
    }
    bool insert(TrieNodePtr &cur, char c) {
        assert(c >= 'a' && c <= 'z');
        bool ret = false;
        if (cur->next[c - 'a'] == NULL) {
            ret = true;
            cur->next[c - 'a'] = new TrieNode();
        }
        cur = cur->next[c - 'a'];
        return ret;
    }
};
    
void dfs(TrieNodePtr cur) {
    int sz = 0;
    int cnt_l = 0;
    int cnt_w = 0;
    for (int i = 0; i < 26; i++) {
        if (cur->next[i] != NULL) {
            dfs(cur->next[i]);
            sz++;
            if (cur->next[i]->val == 1) cnt_l++;
            if (cur->next[i]->val2 == 0) cnt_w++;
        }
    }

    if (sz == 0) {
        cur->val = 1;
        cur->val2 = 1;
    } else {
        if (cnt_l) cur->val = 0;
        else cur->val = 1;
        if (cnt_w) cur->val2 = 1;
        else cur->val2 = 0;
    }
}

int main() {
    string s;
    int N, K;
    cin >> N >> K;
    Trie tree;
    for (int i = 0; i < N; i++) {
        cin >> s;
        tree.insert(s);
    }
    dfs(tree.root);
    if (tree.root->val == 1) printf("Second\n");
    else if ((K % 2 == 0) && tree.root->val2 == 0) printf("Second\n");
    else printf("First\n");
    return 0;
}
