///////////////////////////////////////////////////////////////////////////////
//  Treap - randomized binary search tree
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdio>
#include <cassert>

using namespace std;

template<class T1, class T2>
ostream& operator << (ostream &out, pair<T1, T2> pair) { return out << "(" << pair.first << ", " << pair.second << ")"; }

typedef long long LL;
typedef pair<int, int> PII;

/*
 * Treap Class Declaration
 * 
 * DO NOT FORGET TO CALL SRAND IN MAIN: srand(time(NULL));
 */
template<class T>
class Treap {
    typedef struct Treapnode {
        T key; // (x = key, y = id) can differentiate the duplicate keys
        int fix;
        Treapnode *left, *right;
        int subtree_size;

        LL sum; // sum of all keys
        Treapnode() {
            left = right = NULL;
            subtree_size = 0;
            sum = 0;
        }
    } *Treapnode_ptr;
    
public:
    Treapnode_ptr root;

    Treap() {
        root = NULL;
    }
    
    void insert(T key) {
        insert(this->root, key);
    }
    
    void remove(T key) {
        remove(this->root, key);
    }

    void display() {
        display(this->root, 1);
    }

    bool find(T key) {
        return find(this->root, key);
    }

    T find_ksmallest(int k) {
        assert(k >= 1 && k <= this->root->subtree_size);
        return find_ksmallest(this->root, k);
    }

    LL find_ksmallest_sum(int k) {
        assert(k >= 1 && k <= this->root->subtree_size);
        return find_ksmallest_sum(this->root, k);
    }
        
private:
    void insert(Treapnode_ptr &node, T key) {
        if (node == NULL) {
            node = new Treapnode();
            node->left = node->right = NULL;
            node->key = key;
            node->fix = rand();
        } else {
            if (node->key == key) {
                // do nothing
            } else if (node->key > key) {
                insert(node->left, key);
                if (node->left->fix > node->fix)
                    rotate_right(node);
            } else {
                insert(node->right, key);
                if (node->right->fix > node->fix)
                    rotate_left(node);
            }
        }
        update_size(node);
        update_sum(node);
    }

    void remove(Treapnode_ptr &node, T key) {
        if (node) {
            if (key > node->key) {
                remove(node->right, key);
                update_size(node);
                update_sum(node);
            } else if (key < node->key) {
                remove(node->left, key);
                update_size(node);
                update_sum(node);
            } else {
                if (node->left == NULL && node->right == NULL) {
                    delete node;
                    node = NULL;
                } else if (node->left == NULL) {
                    Treapnode_ptr temp = node;
                    node = node->right;
                    delete temp;
                } else if (node->right == NULL) {
                    Treapnode_ptr temp = node;
                    node = node->left;
                    delete temp;
                } else {
                    if (node->left->fix < node->right->fix) {
                        rotate_left(node);
                        remove(node->left, key);
                    } else {
                        rotate_right(node);
                        remove(node->right, key);
                    }
                    update_size(node);
                    update_sum(node);
                }
            }
        }
    }

    bool find(Treapnode_ptr node, T key) {
        if (node) {
            if (node->key == key) return true;
            else if (key < node->key) return find(node->left, key);
            else return find(node->right, key);
        }
        return false;
    }

    void rotate_left(Treapnode_ptr &node) {
        Treapnode_ptr temp;
        temp = node->right;
        node->right = temp->left;
        temp->left = node;
        node = temp;
        update_size(node->left);
        update_size(node);
        update_sum(node->left);
        update_sum(node);
    }

    void rotate_right(Treapnode_ptr &node) {
        Treapnode_ptr temp;
        temp = node->left;
        node->left = temp->right;
        temp->right = node;
        node = temp;
        update_size(node->right);
        update_size(node);
        update_sum(node->right);
        update_sum(node);
    }

    void display(Treapnode_ptr node, int level) {
        if (node) {
            display(node->right, level + 1);
            if (node == this->root)
                cout << "Root->: ";
            else
                for (int i = 0; i < level; i++)
                    cout << "\t";
            cout << "key: " << node->key << ", sz: " << node->subtree_size << endl;
            display(node->left, level + 1);
        }
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
};


////////////////
//  USAGE
////////////////

int main() {
    srand(time(NULL));
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
