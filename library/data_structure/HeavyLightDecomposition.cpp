///////////////////////////////////////////////////////////////////////////////
//  Heavy Light Decompostion (only support a single tree node updating)
//  An example of how to solve a problem where there are 2 types of queries
//      1. change an edge's weight
//      2. find the max weight among all edges alone the path (u, v)
//
//  Link: http://www.spoj.com/problems/QTREE/
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstring>
#include <vector>
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

    // Return the node's value
    Value_t node_to_value(int at) {
        return tree[at].value;
    }

    // Recalculate the parent's value
    void update_up(int at){
        tree[at].value = max(node_to_value(at * 2), node_to_value(at * 2 + 1));
    }

    // Update the result
    void update_result(Value_t &res, Value_t val) {
        res = max(res, val);
    }

    // Update leaf's value for internal update
    void update_leaf_value(int at, Value_t val) {
        tree[at].value = val;
    }

    Value_t query(int at, int ll, int rr){
        if(tree[at].left == ll && tree[at].right == rr){
            return node_to_value(at);
        } else {
            int mid = (tree[at].left + tree[at].right) / 2;
            Value_t res = 0;
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


// Value's type, Max number of nodes, true if values on vertices
template <class Value_t, int MN, bool VALUE_ON_VERTICES>
class HeavyLightDecompositon {
public:
    static const bool ON_VERTICES = VALUE_ON_VERTICES;
    int N, M; // number of vertices, edges
    int head[MN], weight[MN*2], to[MN*2], nxt[MN*2], ecnt;
    int edge_id[MN]; // i-th edge (u,v,c) = (to[id], to[id^1], weight[id]), id = edge_id[i]
    Value_t node_value[MN]; // node value is initialized in tree_setup
    int tree_size[MN], parent[MN], dep[MN], heaviest_child[MN];
    int chain_head[MN], dfs_clock, num_chain;
    int pos_on_base_array[MN]; // 1-based
    int vertex_on_base_array[MN]; // 1-based
    vector<Value_t> base_array_value; // 0-based
    // Vertices and Edges are 1-based indexed.
    HeavyLightDecompositon(int n) {
        N = n; ecnt = 2, M = 0; base_array_value.resize(N+5);
        memset(head, 0, sizeof(head));
    }
    // Returns base_array_value
    vector<Value_t> decompose() {
        initialize_node_value(1, -1);
        internal_tree_setup(1, 0, 0);
        dfs_clock = 0; num_chain = 1;
        internal_dfs_heavy_edge(1, 1);
        return base_array_value;
    }
    // Initialize node's value ***
    void initialize_node_value(int u, int par) {
        node_value[u] = 1;
        for (int p = head[u]; p; p = nxt[p]) {
            int v = to[p]; if (v == par) continue;
            initialize_node_value(v, u);
            if (ON_VERTICES) {  // Value on vertices case
                node_value[u] += node_value[v];
            } else {            // Value on edges case
                node_value[v] = weight[p];
            }
        }
    }
    // Set base_array_value[ind] by considering the current node
    void set_base_array_value(int ind, int cur_node) {
        base_array_value[ind] = node_value[cur_node];
    }
    void add_edge(int u, int v, int c) {
        edge_id[++M] = ecnt;
        to[ecnt] = v; weight[ecnt] = c; nxt[ecnt] = head[u]; head[u] = ecnt++;
        to[ecnt] = u; weight[ecnt] = c; nxt[ecnt] = head[v]; head[v] = ecnt++;
    }
private:
    void internal_tree_setup(int u, int par, int depth) {
        int maxsize = 0;
        parent[u] = par; dep[u] = depth;
        tree_size[u] = 1; heaviest_child[u] = 0;
        for (int p = head[u]; p; p = nxt[p]) {
            int v = to[p]; if (v == par) continue;
            internal_tree_setup(v, u, depth + 1);
            tree_size[u] += tree_size[v];
            if (tree_size[v] > maxsize) { maxsize = tree_size[v]; heaviest_child[u] = v; }
        }
    }
    void internal_dfs_heavy_edge(int u, int chain_head_id) {
        pos_on_base_array[u] = ++dfs_clock; chain_head[u] = chain_head_id;
        vertex_on_base_array[dfs_clock] = u;
        set_base_array_value(dfs_clock - 1, u);
        if(heaviest_child[u])
            internal_dfs_heavy_edge(heaviest_child[u], chain_head_id); // Expand the chain
        for(int p = head[u]; p; p = nxt[p]) {
            int v = to[p]; if(v == parent[u] || v == heaviest_child[u]) continue;
            num_chain++; // Create a new chain
            internal_dfs_heavy_edge(v, v);
        }
    }
};

const int MAXN = 10010;
const int INF = 0x7fffffff;
typedef HeavyLightDecompositon<int, MAXN, false> HLD;
typedef SegmentTree<int> SegTree;

//////////////////////////////////////////////////////////////////////
// Using HLD and Segment tree to process query and update
//////////////////////////////////////////////////////////////////////

void update_result (int &res, int val) {
    res = max(res, val);
}

int query(SegTree &seg_tree, HLD &hld, int x, int y) {
    int res = 0;
    while(hld.chain_head[x] != hld.chain_head[y]) {
        int head_x = hld.chain_head[x], head_y = hld.chain_head[y];
        if(hld.dep[head_x] < hld.dep[head_y]) swap(x, y), swap(head_x, head_y);
        update_result(res, seg_tree.query(hld.pos_on_base_array[head_x],
                                          hld.pos_on_base_array[x]));
        x = hld.parent[head_x];
    }
    if(hld.dep[x] > hld.dep[y]) swap(x, y);
    int from = hld.pos_on_base_array[x] + (hld.ON_VERTICES == false);
    int to = hld.pos_on_base_array[y];
    update_result(res, seg_tree.query(from, to));
    return res;
}

void update(SegTree &seg_tree, HLD &hld, int e, int y) {
    if (hld.ON_VERTICES) { // Value on vertices
        seg_tree.update(hld.pos_on_base_array[e], y);
    } else {               // Value on edges
        e = hld.edge_id[e];
        int u = hld.to[e], v = hld.to[e ^ 1];
        if(hld.parent[v] == u) swap(u, v);
        // Update the lower end of this edge e
        seg_tree.update(hld.pos_on_base_array[u], y);
    }
}

char str[15];
int main() {
    int n;
    int T; scanf("%d", &T);
    for(int t = 1; t <= T; ++t) {
        scanf("%d", &n);
        HLD hld(n);
        for(int i = 1; i < n; ++i) {
            int u, v, c;
            scanf("%d %d %d", &u, &v, &c);
            hld.add_edge(u, v, c);
        }
        vector<int> base_array_value = hld.decompose();
        SegTree seg_tree(n, base_array_value);
        while(scanf("%s", str) && *str != 'D') {
            int x, y;
            scanf("%d %d", &x, &y);
            if(*str == 'C') update(seg_tree, hld, x, y);
            else printf("%d\n", query(seg_tree, hld, x, y));
        }
    }
    return 0;
}
