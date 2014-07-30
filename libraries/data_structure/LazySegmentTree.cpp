///////////////////////////////////////////////////////////////////////////////
//  Lazy Segment Tree (lazy propagation)
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <cassert>

using namespace std;

template<class Value_t, class Lazy_t>
class LazySegmentTree {
public:
    typedef struct tree_st {
        Value_t value;
        Lazy_t lazy;
        int left, right;
    } TreeNode;

    vector<TreeNode> tree;
    int N; // the number of data
    int tree_size; // the number of tree nodes
    LazySegmentTree(int n) { vector<Value_t> v(n); build_tree(n, v); }
    LazySegmentTree(int n, vector<Value_t> &v) { build_tree(n, v); }
    Value_t query(int ll, int rr) { return query(1, ll, rr); }
    void update(int ll, int rr, Value_t val) { update(1, ll, rr, val); }
private:
    void build_tree(int n, vector<Value_t> &v){
        N = n; int base = 1; while (base < N) base *= 2; tree_size = base * 2;
        tree = vector<TreeNode> (tree_size);
        init(1, 1, n, v);
    }

    void init(int at, int ll, int rr, vector<Value_t> &v) {
        tree[at].left = ll; tree[at].right = rr;
        initialize_laziness(at);
        if(ll == rr) tree[at].value = v[ll - 1];
        else {
            int mid = (tree[at].left + tree[at].right) / 2;
            init(at * 2, tree[at].left, mid, v);
            init(at * 2 + 1, mid + 1, tree[at].right, v);
            update_up(at);
        }
    }

    // Initialize node's laziness
    void initialize_laziness (int at) {
        tree[at].lazy = 0;
    }    
    
    // Return the node's value (considering the given node's value and laziness)
    Value_t node_to_value(int at) {
        return tree[at].value + (tree[at].right - tree[at].left + 1) * tree[at].lazy;
    }

    // Update the node laziness
    void update_laziness(int at, Lazy_t lazy) {
        tree[at].lazy += lazy;
    }
    
    // Recalculate the parent's value without considering the parent's laziness
    void update_up(int at){
        tree[at].value = node_to_value(at * 2) + node_to_value(at * 2 + 1);
    }

    // Update the childrens' laziness
    void update_down(int at){
        if (tree[at].lazy == 0) return ; // no need to update
        update_laziness(at * 2, tree[at].lazy);
        update_laziness(at * 2 + 1, tree[at].lazy);
        tree[at].lazy = 0;
    }

    // Update the result
    void update_result(Value_t &res, Value_t val) {
        res += val;
    }
    
    Value_t query(int at, int ll, int rr){
        if(tree[at].left == ll && tree[at].right == rr){
            return node_to_value(at);
        } else {
            update_down(at);
            int mid = (tree[at].left + tree[at].right) / 2;
            Value_t res = 0;
            if(ll <= mid) update_result(res, query(at * 2, ll, min(rr, mid)));
            if(rr > mid) update_result(res, query(at * 2 + 1, max(mid + 1, ll), rr));
            update_up(at);
            return res;
        }
    }

    void update(int at, int ll, int rr, Value_t val){
        if(tree[at].left == ll && tree[at].right == rr){
            update_laziness(at, val);
        } else {
            update_down(at);
            int mid = (tree[at].left + tree[at].right) / 2;
            if(ll <= mid) update(at * 2, ll, min(rr, mid), val);
            if(rr > mid) update(at * 2 + 1, max(mid + 1, ll), rr, val);
            update_up(at);
        }
    }
};

int main(){
    int N = 7;
    LazySegmentTree<int, int> tree(N);
    tree.update(1, 7, 4);
    assert(tree.query(1, 7) == 28);
    tree.update(3, 5, 2);
    assert(tree.query(3, 3) == 6);
    assert(tree.query(1, 7) == 34);
    assert(tree.query(5, 6) == 10);
    assert(tree.query(1, 4) == 20);
    return 0;
}
