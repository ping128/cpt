///////////////////////////////////////////////////////////////////////////////
//  Segment Tree
//  Only work for single element updating
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <cassert>
#include <iostream>

using namespace std;

template<class Value_t>
class SegmentTree {
public:
    typedef struct segment_st {
        Value_t value;
        int left, right;
    } TreeNode;

    vector<TreeNode> tree;
    int N; // the number of data
    int tree_size; // the number of tree nodes
    vector<int> leaves; // leaves' indices
    SegmentTree(int n) { vector<Value_t> v(n); build_tree(n, v); }
    SegmentTree(int n, vector<Value_t> &v) { build_tree(n, v); }
    Value_t query(int ll, int rr) { return query(1, ll, rr); }
    void update(int at, Value_t val) { internal_update(at, val); }
private:
    void build_tree(int n, vector<Value_t> &v){
        N = n; int base = 1; while (base < N) base *= 2; tree_size = base * 2;
        leaves.resize(N + 1); tree = vector<TreeNode> (tree_size);
        init(1, 1, n, v);
    }

    void init(int at, int ll, int rr, vector<Value_t> &v) {
        tree[at].left = ll; tree[at].right = rr;
        if(ll == rr) { tree[at].value = v[ll - 1]; leaves[ll] = at; }
        else {
            int mid = (tree[at].left + tree[at].right) / 2;
            init(at * 2, tree[at].left, mid, v);
            init(at * 2 + 1, mid + 1, tree[at].right, v);
            update_up(at);
        }
    }
    // Set the initial value
    Value_t initial_value() {
        return 0;
    }
    // Return the node's value
    Value_t node_to_value(int at) {
        return tree[at].value;
    }
    // Recalculate the parent's value
    void update_up(int at){
        tree[at].value = node_to_value(at * 2) + node_to_value(at * 2 + 1);
    }
    // Update the result
    void update_result(Value_t &res, Value_t val) {
        res += val;
    }
    // Update leaf's value for internal update
    void update_leaf_value(int at, Value_t val) {
        tree[at].value += val;
    }
    Value_t query(int at, int ll, int rr){
        if(tree[at].left == ll && tree[at].right == rr){
            return node_to_value(at);
        } else {
            int mid = (tree[at].left + tree[at].right) / 2;
            Value_t res = initial_value();
            if(ll <= mid) update_result(res, query(at * 2, ll, min(rr, mid)));
            if(rr > mid) update_result(res, query(at * 2 + 1, max(mid + 1, ll), rr));
            return res;
        }
    }

    void internal_update(int at, Value_t val) {
        at = leaves[at];
        update_leaf_value(at, val);
        at /= 2;
        while (at) { update_up(at); at /= 2; }
    }
};

int main(){
    int N = 7;
    SegmentTree<int> tree(N);
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
