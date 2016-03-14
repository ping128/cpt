///////////////////////////////////////////////////////////////////////////////
//  Treap - randomized binary search tree
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdio>
#include <cassert>

using namespace std;

#define SZ(x) ((int)((x).size()))
#define PB(x) push_back(x)
#define MEMSET(x,v) memset(x,v,sizeof(x))
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

#define x first
#define y second

typedef long long LL;
typedef pair<int, int> PII; typedef pair<PII, int> PII2;

template<class T>
class Treap {
    typedef struct Treapnode {
        T key; // (x = key, y = id) can differentiate the duplicate keys
        int fix;
        Treapnode *left, *right;
        int sz;
        LL sum; // sum of all keys
        Treapnode() { left = right = NULL; sz = 0; sum = 0; }
    } *NPtr;
public:
    NPtr root;
    Treap() { root = NULL; srand(time(NULL)); }
    void insert(T key) { root = insert(root, key); }
    int size() { return size(root); }
    int size(Treapnode node) { return (node == NULL) ? 0 : node->sz; }
    T find_ksmallest(int k) {
        assert(k >= 1 && k <= this->root->sz);
        return find_ksmallest(this->root, k);
    }
    LL find_ksmallest_sum(int k) {
        assert(k >= 1 && k <= this->root->sz);
        return find_ksmallest_sum(this->root, k);
    }
    void remove(T key) { root = remove(root, key); }
    bool find(T key) { return find(this->root, key); }
private:
    NPtr insert(NPtr node, T key) {
        if (node == NULL) {
            node = new Treapnode();
            node->left = node->right = NULL;
            node->key = key;
            node->fix = rand();
        } else {
            if (node->key == key) {
                // do nothing
            } else if (node->key > key) {
                node->left = insert(node->left, key);
                if (node->left->fix > node->fix)
                    rotate_right(node);
            } else {
                node->right = insert(node->right, key);
                if (node->right->fix > node->fix)
                    rotate_left(node);
            }
        }
        update_size(node);
        update_sum(node);
        return node;
    }

    NPtr remove(NPtr node, T key) {
        if (node) {
            if (key > node->key) {
                node->right = remove(node->right, key);
                update_size(node);
                update_sum(node);
            } else if (key < node->key) {
                node->left = remove(node->left, key);
                update_size(node);
                update_sum(node);
            } else {
                if (node->left == NULL && node->right == NULL) {
                    delete node;
                    node = NULL;
                } else if (node->left == NULL) {
                    NPtr temp = node->right;
                    delete node;
                    node = temp;
                } else if (node->right == NULL) {
                    NPtr temp = node->left;
                    delete node;
                    node = temp;
                } else {
                    if (node->left->fix < node->right->fix) {
                        rotate_left(node);
                        node->left = remove(node->left, key);
                    } else {
                        rotate_right(node);
                        node->right = remove(node->right, key);
                    }
                    update_size(node);
                    update_sum(node);
                }
            }
        }
        return node;
    }

    void rotate_left(NPtr &node) {
        NPtr temp;
        temp = node->right; node->right = temp->left;
        temp->left = node; node = temp;
        update_size(node->left); update_size(node);
        update_sum(node->left); update_sum(node);
    }

    void rotate_right(NPtr &node) {
        NPtr temp;
        temp = node->left; node->left = temp->right;
        temp->right = node; node = temp;
        update_size(node->right); update_size(node);
        update_sum(node->right); update_sum(node);
    }

    void update_size(NPtr node) {
        node->sz = 1;
        if (node->left) node->sz += node->left->sz;
        if (node->right) node->sz += node->right->sz;
    }

    void update_sum(NPtr node) {
        node->sum = node->key.x;
        if (node->left) node->sum += node->left->sum;
        if (node->right) node->sum += node->right->sum;
    }

    T find_ksmallest(NPtr node, int k) {
        if (node->left == NULL) {
            if (k == 1)
                return node->key;
            return find_ksmallest(node->right, k - 1);
        } else {
            int num_left = node->left->sz;
            if (k <= num_left)
                return find_ksmallest(node->left, k);
            else if (k == num_left + 1)
                return node->key;
            else
                return find_ksmallest(node->right, k - num_left - 1);
        }
    }

    LL find_ksmallest_sum(NPtr node, int k) {
        LL res = 0;
        if (node && k) {
            if (node->left == NULL) {
                if (k == node->sz)
                    res = node->sum;
                else
                    res = node->key.x + find_ksmallest_sum(node->right, k - 1);
            } else {
                int num_left = node->left->sz;
                if (k <= num_left)
                    res = find_ksmallest_sum(node->left, k);
                else if (k == num_left + 1)
                    res = node->key.x + node->left->sum;
                else
                    res = node->key.x + node->left->sum + find_ksmallest_sum(node->right, k - num_left - 1);
            }
        }
        return res;
    }
    bool find(NPtr node, T key) {
        if (node) {
            if (node->key == key) return true;
            else if (key < node->key) return find(node->left, key);
            else return find(node->right, key);
        }
        return false;
    }
};

////////////////
//  USAGE
////////////////

int main() {
    Treap<PII> tree;
    tree.insert(PII(1, 0));
    tree.insert(PII(5, 0));
    tree.insert(PII(2, 0));
    tree.insert(PII(3, 0));
    tree.insert(PII(10, 0));
    tree.insert(PII(8, 0));
    tree.insert(PII(4, 0));
    tree.insert(PII(7, 0));
    tree.insert(PII(6, 0));
    tree.insert(PII(9, 0));

    for (int i = 0; i <= 11; i++) cout << tree.find(PII(i, 0)) << endl;
    cout << endl;

    for (int i = 1; i <= 10; i++)
        cout << tree.find_ksmallest(i).x << endl;
    cout << endl;
    for (int i = 1; i <= 10; i++)
        cout << tree.find_ksmallest_sum(i) << endl;
    return 0;
}
