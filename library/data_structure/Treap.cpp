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

typedef long long LL;
typedef pair<int, int> PII; typedef pair<PII, int> PII2;

template<class T1, class T2>
ostream& operator << (ostream &out, pair<T1, T2> pair) { return out << "(" << pair.first << ", " << pair.second << ")"; }


template<class T>
class Treap {
    typedef struct Treapnode {
        T key; // (x = key, y = id) can differentiate the duplicate keys
        int fix;
        Treapnode *left, *right;
        int subtree_size;
        LL sum; // sum of all keys
        Treapnode() { left = right = NULL; subtree_size = 0; sum = 0; }
    } *Treapnode_ptr;
public:
    Treapnode_ptr root;
    int tree_size;
    Treap() { root = NULL; srand(time(NULL)); tree_size = 0; }
    void insert(T key) { root = insert(root, key); tree_size = root->subtree_size; }
    int size() { return tree_size; }
    T find_ksmallest(int k) {
        assert(k >= 1 && k <= this->root->subtree_size);
        return find_ksmallest(this->root, k);
    }
    LL find_ksmallest_sum(int k) {
        assert(k >= 1 && k <= this->root->subtree_size);
        return find_ksmallest_sum(this->root, k);
    }
    void remove(T key) { root = remove(root, key); tree_size = (root != NULL) ? root->subtree_size : 0; }
    void display() { display(this->root, 1); }
    bool find(T key) { return find(this->root, key); }
private:
    Treapnode_ptr insert(Treapnode_ptr node, T key) {
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

    Treapnode_ptr remove(Treapnode_ptr node, T key) {
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
                    Treapnode_ptr temp = node->right;
                    delete node;
                    node = temp;
                } else if (node->right == NULL) {
                    Treapnode_ptr temp = node->left;
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

    void rotate_left(Treapnode_ptr &node) {
        Treapnode_ptr temp;
        temp = node->right; node->right = temp->left;
        temp->left = node; node = temp;
        update_size(node->left); update_size(node);
        update_sum(node->left); update_sum(node);
    }

    void rotate_right(Treapnode_ptr &node) {
        Treapnode_ptr temp;
        temp = node->left; node->left = temp->right;
        temp->right = node; node = temp;
        update_size(node->right); update_size(node);
        update_sum(node->right); update_sum(node);
    }

    void update_size(Treapnode_ptr node) {
        node->subtree_size = 1;
        if (node->left) node->subtree_size += node->left->subtree_size;
        if (node->right) node->subtree_size += node->right->subtree_size;
    }

    void update_sum(Treapnode_ptr node) {
        node->sum = node->key.first;
        if (node->left) node->sum += node->left->sum;
        if (node->right) node->sum += node->right->sum;
    }

    T find_ksmallest(Treapnode_ptr node, int k) {
        if (node->left == NULL) {
            if (k == 1)
                return node->key;
            return find_ksmallest(node->right, k - 1);
        } else {
            int num_left = node->left->subtree_size;
            if (k <= num_left)
                return find_ksmallest(node->left, k);
            else if (k == num_left + 1)
                return node->key;
            else
                return find_ksmallest(node->right, k - num_left - 1);
        }
    }

    LL find_ksmallest_sum(Treapnode_ptr node, int k) {
        LL res = 0;
        if (node && k) {
            if (node->left == NULL) {
                if (k == node->subtree_size)
                    res = node->sum;
                else
                    res = node->key.first + find_ksmallest_sum(node->right, k - 1);
            } else {
                int num_left = node->left->subtree_size;
                if (k <= num_left)
                    res = find_ksmallest_sum(node->left, k);
                else if (k == num_left + 1)
                    res = node->key.first + node->left->sum;
                else
                    res = node->key.first + node->left->sum + find_ksmallest_sum(node->right, k - num_left - 1);
            }
        }
        return res;
    }
    bool find(Treapnode_ptr node, T key) {
        if (node) {
            if (node->key == key) return true;
            else if (key < node->key) return find(node->left, key);
            else return find(node->right, key);
        }
        return false;
    }
    void display(Treapnode_ptr node, int level) {
        if (node) {
            display(node->right, level + 1);
            if (node == this->root)
                cout << "(Size: " << tree_size << " Root->: ";
            else
                for (int i = 0; i < level; i++)
                    cout << "\t";
            cout << "key: " << node->key << ", sz: " << node->subtree_size << endl;
            display(node->left, level + 1);
        }
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
    tree.display();

    for (int i = 0; i <= 11; i++) cout << tree.find(PII(i, 0)) << endl;

    cout << endl;

    for (int i = 1; i <= 10; i++)
        cout << tree.find_ksmallest(i) << endl;

    cout << endl;
    for (int i = 1; i <= 10; i++)
        cout << tree.find_ksmallest_sum(i) << endl;
    return 0;
}
