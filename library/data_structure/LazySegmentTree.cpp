
///////////////////////////////////////////////////////////////////////////////
//  Lazy Segment Tree (lazy propagation)
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <cassert>

using namespace std;


class LazySegmentTree {
public:
    typedef int Update_t;
    typedef int Query_t;
    typedef int Value_t;
    typedef int Lazy_t;

    typedef struct tree_st {
        Value_t value;
        Lazy_t lazy;
        int left, right;
    } TreeNode;

    vector<TreeNode> tree;
    int N; // the number of data
    int tree_size; // the number of tree nodes
    LazySegmentTree(int n) { vector<Value_t> v; build_tree(n, v); }
    LazySegmentTree(int n, vector<Value_t> &v) { build_tree(n, v); }
    Query_t query(int ll, int rr) { return value_to_result(query(1, ll, rr)); }
    void update(int ll, int rr, Update_t val) { update(1, ll, rr, val); }
private:
    Lazy_t DEF_LAZY = 0;
    Value_t DEF_VALUE = 0;
    // Initialize node
    void initialize_node (int at) {
        tree[at].lazy = DEF_LAZY;
        tree[at].value = DEF_VALUE;
    }

    // Node's value to query's result
    Query_t value_to_result(Value_t val) {
        return val;
    }

    // Return node's value (considering the given node's value and lazy)
    Value_t node_to_value(int at) {
        return tree[at].value + (tree[at].right - tree[at].left + 1) * tree[at].lazy;
    }

    // Update node's lazy
    void update_lazy(int at, Update_t up_val) {
        tree[at].lazy += up_val;
    }

    // Recalculate the parent's value without considering the parent's lazy
    void update_up(int at){
        tree[at].value = node_to_value(at * 2) + node_to_value(at * 2 + 1);
    }

    // Update children's lazy
    void update_down(int at){
        if (tree[at].lazy == DEF_LAZY) return ; // no need to update
        tree[at * 2].lazy += tree[at].lazy;
        tree[at * 2 + 1].lazy += tree[at].lazy;
        tree[at].lazy = DEF_LAZY;
    }

    // Update the result
    void update_result(Value_t &res, Value_t q) {
        res += q;
    }

    Value_t query(int at, int ll, int rr){
        if (ll == tree[at].left && tree[at].right == rr) {
            return node_to_value(at);
        } else {
            Value_t res = DEF_VALUE;
            update_down(at);
            int mid = (tree[at].left + tree[at].right) >> 1;
            if(ll <= mid) update_result(res, query(at * 2, ll, min(rr, mid)));
            if(rr > mid) update_result(res, query(at * 2 + 1, max(mid + 1, ll), rr));
            update_up(at);
            return res;
        }
    }

    void update(int at, int ll, int rr, Update_t up_val){
        if (ll == tree[at].left && tree[at].right == rr) {
            update_lazy(at, up_val);
        } else {
            update_down(at);
            int mid = (tree[at].left + tree[at].right) >> 1;
            if(ll <= mid) update(at * 2, ll, min(rr, mid), up_val);
            if(rr > mid) update(at * 2 + 1, max(mid + 1, ll), rr, up_val);
            update_up(at);
        }
    }

    void build_tree(int n, vector<Value_t> &v){
        N = n; int base = 1; while (base < N) base *= 2; tree_size = base * 2;
        tree = vector<TreeNode> (tree_size);
        init(1, 1, n, v);
    }

    void init(int at, int ll, int rr, vector<Value_t> &v) {
        tree[at].left = ll; tree[at].right = rr;
        initialize_node(at);
        if (ll == rr) { if (!v.empty()) tree[at].value = v[ll - 1]; }
        else {
            int mid = (tree[at].left + tree[at].right) >> 1;
            init(at * 2, tree[at].left, mid, v);
            init(at * 2 + 1, mid + 1, tree[at].right, v);
            update_up(at);
        }
    }
};

int main(){
    int N = 7;
    LazySegmentTree tree(N);
    tree.update(1, 7, 4);
    assert(tree.query(1, 7) == 28);
    tree.update(3, 5, 2);
    assert(tree.query(3, 3) == 6);
    assert(tree.query(1, 7) == 34);
    assert(tree.query(5, 6) == 10);
    assert(tree.query(1, 4) == 20);
    return 0;
}
