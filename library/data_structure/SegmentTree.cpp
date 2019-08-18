///////////////////////////////////////////////////////////////////////////////
//  Segment Tree
//  Only work for single element updating
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <cassert>
#include <iostream>

using namespace std;

class SegmentTree {
public:
    typedef struct segment_st {
        int value;
        int l, r;
    } TreeNode;

    vector<TreeNode> tree;
    int N; // the number of data
    int tree_size; // the number of tree nodes
    vector<int> leaves; // leaves' indices
    SegmentTree(int n) { build_tree(n); }
    int query(int ll, int rr) { return query(1, ll, rr); }
    void update(int at, int val) { internal_update(at, val); }
private:
    // Initialize node
    void initialize_node (int at) {
        tree[at].value = 0;
    }
    // Recalculate the parent's value
    void update_up(int at){
        int left = at + at;
        int right = at + at + 1;
        tree[at].value = tree[left].value + tree[right].value;
    }
    // Update the result
    void update_result(int &res, int val) {
        res += val;
    }
    // Update leaf's value for internal update
    void update_leaf_value(int at, int &val) {
        tree[at].value += val;
    }
    int query(int at, int ll, int rr){
        if (ll > tree[at].r || rr < tree[at].l) return 0;
        if (ll <= tree[at].l && tree[at].r <= rr) {
            return tree[at].value;
        } else {
            int res = 0;
            update_result(res, query(at + at, ll, rr));
            update_result(res, query(at + at + 1, ll, rr));
            return res;
        }
    }
    void internal_update(int at, int &val) {
        at = leaves[at];
        update_leaf_value(at, val);
        at >>= 1;
        while (at) { update_up(at); at >>= 1; }
    }
    void build_tree(int n) {
        N = n; int base = 1; while (base < N) base <<= 1; tree_size = base << 1;
        leaves.resize(N + 1); tree = vector<TreeNode> (tree_size);
        init(1, 1, n);
    }
    void init(int at, int ll, int rr) {
        tree[at].l = ll; tree[at].r = rr;
        initialize_node(at);
        if(ll == rr) { leaves[ll] = at; }
        else {
            int mid = (tree[at].l + tree[at].r) >> 1;
            init(at + at, tree[at].l, mid);
            init(at + at + 1, mid + 1, tree[at].r);
            update_up(at);
        }
    }
};

int main(){
    int N = 7;
    SegmentTree tree(N);
    int sum = 0;
    for (int i = 1; i <= N; i++) {
        tree.update(i, i);
        sum += i;
        assert(tree.query(1, N) == sum);
        assert(tree.query(i, i) == i);
    }
    sum += 10;
    tree.update(4, 10);
    assert(tree.query(1, N) == sum);
    assert(tree.query(1, 3) == 6);
    assert(tree.query(4, 5) == 19);
    assert(tree.query(5, 6) == 11);
    return 0;
}
