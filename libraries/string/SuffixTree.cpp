/////////////////////////////////////////////////////////////////////////////////
//  Suffix Tree
//  by Ukkonen's algorithm in O(N * alphabetSize)
//
//  There are 2 more methods added to solve this problem:
//  http://codeforces.com/contest/452/problem/E
//    - For each valid l, find the number of common substrings of lengh l
//      that are in all three given strings
/////////////////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

using namespace std;

#define x first
#define y second
#define SZ(x) ((int)((x).size()))
#define PB(x) push_back(x);
#define INF (0x3f3f3f3f)
#define MEMSET(x,v) memset(x,v,sizeof(x));

typedef long long LL;
typedef pair<int, int> PII; typedef pair<PII, int> PII2;
typedef vector<int> VI; typedef vector<VI> VVI;

typedef pair<LL, LL> PLL; typedef pair<PLL, LL> PLL2;
#define MOD (1000000007)

// The number of delimiter the class will use is equal to the number of strings added
// Please make sure that the delimiters won't appear in the input strings
// '\1' -> '\1', '\2', '\3', '\4', ... (these characters are invisible)
// '#'  -> '#', '$', '%', '&', ...
template<char FIRST_DELIMITER>
class SuffixTree {
public:
    class Node {
    public:
        int begin, end; // s[begin...end-1]
        int depth; // depth by chars from the root to this node
        Node *parent;
        vector<Node*> children;
        Node *suffix_link;
        Node(int b, int e, int d, Node *p, int alpha_size) : begin(b), end(e), depth(d) , parent(p) {
            children = vector<Node*>(alpha_size);
            for (int i = 0; i < alpha_size; i++) children[i] = NULL;
            suffix_link = NULL;
        }
        bool contains(int d) { return depth <= d && d < depth + (end - begin); }
    };
    Node *root;
    vector<string> strings;
    set<char> alphabets;
    string concat_s;
    SuffixTree() { root = NULL; concat_s = ""; }
    void add_string(string s) {
        strings.PB(s); concat_s += s;
        for (char c : s) { alphabets.insert(c); }
        concat_s += FIRST_DELIMITER + (SZ(strings) - 1); // delimiter
        alphabets.insert(FIRST_DELIMITER + (SZ(strings) - 1));
    }
    void build_suffix_tree() {
        int alphabet_size = SZ(alphabets);
        int N = SZ(concat_s);
        VI a(N);
        for (int i = 0; i < N; i++) {
            int j = 0;
            for (char c : alphabets) { if (concat_s[i] == c) { a[i] = j; break; } j++; }
        }
        root = new Node(0, 0, 0, NULL, alphabet_size);
        Node *cn = root; // root.suffixLink must be null, but that way it gets more convenient processing
        root->suffix_link = root;
        Node *needs_suffix_link = NULL;
        int last_rule = 0;
        int j = 0;
        for (int i = -1; i < N - 1; i++) { // strings s[j..i] already in tree, add s[i+1] to it
            int cur = a[i + 1]; // last char of current string
            for (; j <= i + 1; j++) {
                int cur_depth = i + 1 - j;
                if (last_rule != 3) {
                    cn = cn->suffix_link != NULL ? cn->suffix_link : cn->parent->suffix_link;
                    int k = j + cn->depth;
                    while (cur_depth > 0 && !cn->contains(cur_depth - 1)) {
                        k += cn->end - cn->begin;
                        cn = cn->children[a[k]];
                    }
                }
                if (!cn->contains(cur_depth)) { // explicit node
                    if (needs_suffix_link != NULL) {
                        needs_suffix_link->suffix_link = cn;
                        needs_suffix_link = NULL;
                    }
                    if (cn->children[cur] == NULL) {
                        // no extension - add leaf
                        cn->children[cur] = new Node(i + 1, N, cur_depth, cn, alphabet_size);
                        last_rule = 2;
                    } else {
                        cn = cn->children[cur];
                        last_rule = 3; // already exists
                        break;
                    }
                } else { // implicit node
                    int end = cn->begin + cur_depth - cn->depth;
                    if (a[end] != cur) { // split implicit node here
                        Node *newn = new Node(cn->begin, end, cn->depth, cn->parent, alphabet_size);
                        newn->children[cur] = new Node(i + 1, N, cur_depth, newn, alphabet_size);
                        newn->children[a[end]] = cn;
                        cn->parent->children[a[cn->begin]] = newn;
                        if (needs_suffix_link != NULL) {
                            needs_suffix_link->suffix_link = newn;
                        }
                        cn->begin = end;
                        cn->depth = cur_depth;
                        cn->parent = newn;
                        cn = needs_suffix_link = newn;
                        last_rule = 2;
                    } else if (cn->end != N || cn->begin - cn->depth < j) {
                        last_rule = 3;
                        break;
                    } else {
                        last_rule = 1;
                    }
                }

            }
        }
        root->suffix_link = NULL;
    }
    
    void print_preorder() {
        cout << "Preorder Traversal: " << endl;
        print_preorder(root);
    }
    void print_suffix_array() {
        cout << "Suffix Array: " << endl;
        print_suffix_array(root);
    }

    int max_common_len;
    int max_common_begin;
    // Maximum Common Substring of the first two strings added
    void find_max_common_substring() {
        assert(SZ(strings) >= 2);
        max_common_len = -1;
        int len1 = SZ(strings[0]);
        int len2 = SZ(strings[1]);
        find_max_common_substring(root, len1, len1 + len2 + 1);
        cout << "Max Common Substring Length: " << max_common_len << endl;
        if (max_common_len) {
            cout << concat_s.substr(max_common_begin, max_common_len) << endl;
        }
    }
private:
    pair<bool, bool> find_max_common_substring(Node *cur, int i1, int i2) {
        if (cur->begin <= i1 && i1 < cur->end) {
            return make_pair(true, false); // found a suffix of the first string
        }
        if (cur->begin <= i2 && i2 < cur->end) {
            return make_pair(false, true); // found a suffix of the second string
        }
        pair<bool, bool> ret = make_pair(false, false);
        for (int i = 0; i < SZ(alphabets); i++) {
            if (cur->children[i]) {
                pair<bool, bool> temp = find_max_common_substring(cur->children[i], i1, i2);
                ret.x |= temp.x;
                ret.y |= temp.y;
            }
        }
        if (ret.x && ret.y) { // found a common substring
            int len = cur->depth + cur->end - cur->begin;
            if (len > max_common_len) {
                max_common_len = len;
                max_common_begin = cur->begin - cur->depth; // give the starting position of the suffix
            }
        }
        return ret;
    }
    void print_preorder(Node *cur) {
        if (cur == root) cout << "ROOT" << endl;
        else cout << concat_s.substr(cur->begin, cur->end - cur->begin) << endl;
        for (int i = 0; i < SZ(alphabets); i++) {
            if (cur->children[i] != NULL) {
                print_preorder(cur->children[i]);
            }
        }
    }

    void print_suffix_array(Node *cur) {
       bool is_leaf = true;
        for (int i = 0; i < SZ(alphabets); i++) {
            if (cur->children[i] != NULL) {
                print_suffix_array(cur->children[i]);
                is_leaf = false;
            }
        }
        if (is_leaf) {
            cout << concat_s.substr(cur->begin - cur->depth, cur->end - (cur->begin - cur->depth)) << endl;
        }
    }
public:
    // Add more methods below
    PII2 dfs(Node *cur, int i1, int i2, int i3, vector<LL> &ans) {
        if (cur->begin <= i1 && i1 < cur->end) {
            return PLL2(PLL(1, 0), 0);
        }
        if (cur->begin <= i2 && i2 < cur->end) {
            return PLL2(PLL(0, 1), 0);
        }
        if (cur->begin <= i3 && i3 < cur->end) {
            return PLL2(PLL(0, 0), 1);
        }
        PLL2 ret = PLL2(PLL(0, 0), 0);
        for (int i = 0; i < SZ(alphabets); i++) {
            if (cur->children[i]) {
                PLL2 temp = dfs(cur->children[i], i1, i2, i3, ans);
                ret.x.x += temp.x.x;
                ret.x.y += temp.x.y;
                ret.y += temp.y;
                ret.x.x %= MOD; ret.x.y %= MOD; ret.y %= MOD;
            }
        }
        LL x = (((ret.x.x * ret.x.y) % MOD) * ret.y) % MOD;
        int len = cur->depth + cur->end - cur->begin;
        ans[cur->depth] += x; ans[cur->depth] %= MOD;
        ans[len] -= x; if (ans[len] < 0) ans[len] += MOD;
        return ret;
    }
    
    void solve() {
        vector<LL> ans(SZ(concat_s), 0);
        int len1 = SZ(strings[0]), len2 = SZ(strings[1]), len3 = SZ(strings[2]);
        dfs(root, len1, len1 + len2 + 1, len1 + len2 + len3 + 2, ans);
        int min_len = min(len1, min(len2, len3));
        int res = 0;
        for (int i = 0; i < min_len; i++) {
            res += ans[i]; res %= MOD;
            printf("%d ", (int)res);
        }
    }
};

int main() {
    string s1, s2, s3;
    SuffixTree<'#'> tree;
    cin >> s1 >> s2 >> s3;
    tree.add_string(s1);
    tree.add_string(s2);
    tree.add_string(s3);
    tree.build_suffix_tree();
    tree.solve();
    /*
    {
        SuffixTree<'#'> tree;
        tree.add_string("baab");
        tree.build_suffix_tree();
        tree.print_preorder();
        tree.print_suffix_array();
    }

    {
        SuffixTree<'#'> tree;
        tree.add_string("banana");
        tree.add_string("xana");
        tree.build_suffix_tree();
        tree.find_max_common_substring();
    }
    */
    return 0;
}
