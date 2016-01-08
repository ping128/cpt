///////////////////////////////////////////////////////////////////////////////
//  Splay Tree with lazy propagation
//      - values on left subtree <= x
//      - values on right subtree > x
//
///////////////////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>

using namespace std;

#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

// left: <= x
// right: > x
template<class T>
class SplayTree {
public:
    struct Node {
        Node *left, *right, *par;
        int sz;
        T key;
        Node () {}
        Node (T v) {
            left = right = par = NULL;
            sz = 1;
            key = v;
        }
        void push() {}
    };
    typedef Node *NPtr;
    NPtr root;
    SplayTree() { root = NULL; }
    SplayTree(NPtr r) { root = r; if(root) root->par = NULL; }
    void update_node (NPtr x) {
        x->sz = (x->left == NULL ? 0 : x->left->sz) +
                (x->right == NULL ? 0 : x->right->sz) + 1;
    }
    // Rotate x up one level
    void rotate(NPtr x) {
        assert(x->par != NULL);
        NPtr p_x = x->par;
        NPtr pp_x = x->par->par;
        p_x->push();
        x->push();
        x->par = pp_x;
        if (pp_x) {
            if (pp_x->left == p_x) pp_x->left = x;
            else pp_x->right = x;
        }
        p_x->par = x;
        if (x == p_x->left) { // x is the left child
            if (x->right) x->right->par = p_x;
            p_x->left = x->right;
            x->right = p_x;
        } else { // x is the right child
            if (x->left) x->left->par = p_x;
            p_x->right = x->left;
            x->left = p_x;
        }
        update_node(p_x);
        update_node(x);
    }
    // If p == NULL, splay to the root. Otherwise, splay to the child of p
    void splay(NPtr x) { splay(x, NULL); }
    void splay(NPtr x, NPtr p) {
        while (x->par != p) {
            NPtr p_x = x->par;
            NPtr pp_x = x->par->par;
            if (pp_x == p) // zig
                rotate(x);
            else if ((pp_x->left == p_x) == (p_x->left == x)) // zig-zig
                rotate(p_x), rotate(x);
            else // zig-zag
                rotate(x), rotate(x);
        }
        if (!p) root = x;
    }
    void splayMax() {
        NPtr temp = root;
        while (temp && temp->right) temp = temp->right;
        if (temp) splay(temp);
    }
    void insert(T key) {
        NPtr n = new Node(key);
        if (!root) {
            root = n;
        } else {
            NPtr t = root;
            while(t) {
                if (key > t->key) {
                    if (t->right) t = t->right;
                    else {
                        t->right = n;
                        break;
                    }
                } else {
                    if (t->left) t = t->left;
                    else {
                        t->left = n;
                        break;
                    }
                }
            }
            n->par = t;
        }
        splay(n, NULL);
    }
    int size() { return root ? root->sz : 0; }
    void join(SplayTree<T> &tree) {
        if (root) {
            splayMax();
            root->right = tree.root;
            if (tree.root) tree.root->par = root;
            update_node(root);
        } else {
            root = tree.root;
        }
    }

    void erase(T k) {
        if (contains(k)) {
            SplayTree<T> lc(root->left);
            SplayTree<T> rc(root->right);
            delete root;
            root = lc.root;
            join(rc);
        }
    }
    bool contains(T k) {
        NPtr t = find(root, k);
        if (t) {
            splay(t);
            return t->key == k;
        }
        return false;
    }
    T get_kth_smallest(int k) {
        assert(k >= 1 && k <= size());
        k--;
        NPtr t = root;
        while (t) {
            int x = size(t->left);
            if (k == x) {
                splay(t);
                return t->key;
            }
            else if (k < x) t = t->left;
            else {
                k -= x + 1;
                t = t->right;
            }
        }
        assert(false);
    }
    // Return the number of nodes whose key <= k
    int get_rank(T k) {
        NPtr t = find(root, k);
        if (t == NULL) return 0;
        else {
            splay(t);
            return size(root->left) + 1;
        }
    }
    void print() {
        if (root) {
            print(root);
            cout << ": " << root->sz << endl;
            printf("\n");
        }
    }
    void print(NPtr t) {
        if (t) {
            print(t->left);
            printf("%d,", t->key);
            print(t->right);
        }
    }
private:
    int size(NPtr t) { return t ? t->sz : 0; }
    // Return the right most node whose key <= k of the subtree t
    NPtr find(NPtr t, T k) {
        NPtr ret = NULL;
        while(t) {
            if (k == t->key) return t;
            else if (k < t->key) t = t->left;
            else {
                ret = t;
                t = t->right;
            }
        }
        return ret;
    }
};

int main() {
    SplayTree<int> tree;
    srand(time(NULL));
    REP(i, 20) {
        int x = rand() % 20;
        printf("Insert: %d\n", x);
        tree.insert(x);
        tree.print();
    }
    REP(i, 10) {
        int x = rand() % 20;
        printf("Erase: %d\n", x);
        tree.erase(x);
        tree.print();
    }
    return 0;
}
