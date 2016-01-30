///////////////////////////////////////////////////////////////////////////////
//  Persistent Segment Tree
//  Single-element updating, interval query
//
//  Problem: query: return the k-th smallest number in a given interval
//  Link: http://www.spoj.com/problems/MKTHNUM/
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <vector>
#include <map>

using namespace std;

#define PB(x) push_back(x)
#define REP(i,n) for(int (i)=0;(i)<(n);++(i))

class PersistentTree {
public:
    typedef struct Node {
        Node *l, *r;
        int cnt;
        Node () { l = r = NULL; cnt = 0; }
        Node (Node *ll, Node *rr) {
            l = ll, r = rr;
            update_up();
        }
        // Update node's value
        void update_up() {
            cnt = 0;
            if (l) cnt += l->cnt;
            if (r) cnt += r->cnt;
        }
    } *NPtr;
    int N;
    vector<NPtr> roots;
    PersistentTree(int n) {
        N = n;
        roots.PB(build(1, N));
    }
    void update_leaf(NPtr prev, NPtr now) {
        now->cnt = prev->cnt + 1;
    }
    void update(int at) {
        roots.PB(update(roots.back(), 1, N, at));
    }
    int query(int l, int r, int val) {
        return query(roots[l - 1], roots[r], 1, N, val);
    }
    int query(NPtr left_t, NPtr right_t, int l, int r, int val) {
        if (l == r) {
            return l;
        }
        int m = (l + r) >> 1;
        int cnt = right_t->l->cnt - left_t->l->cnt;
        if (val <= cnt) return query(left_t->l, right_t->l, l, m, val);
        val -= cnt;
        return query(left_t->r, right_t->r, m + 1, r, val);
    }
    NPtr update(NPtr ptree, int l, int r, int at) {
        if (l <= at && at <= r) {
            if (l == r) {
                NPtr node = new Node();
                update_leaf(ptree, node);
                return node;
            }
            int m = (l + r) >> 1;
            return new Node(update(ptree->l, l, m, at), update(ptree->r, m + 1, r, at));
        }
        return ptree;
    }
private:
    NPtr build(int l, int r) {
        if (l == r) {
            return new Node();
        }
        int m = (l + r) >> 1;
        return new Node(build(l, m), build(m + 1, r));
    }
};

#define MAXN (100005)

map<int, int> order;
int inversed_order[MAXN];

int in[MAXN];

int main() {
    int N, M;
    scanf("%d %d", &N, &M);
    for (int i = 1; i <= N; i++) {
        scanf("%d", &in[i]);
        order[in[i]]++;
    }
    N = 0;
    for (map<int, int>::iterator it = order.begin(); it != order.end(); it++) {
        it->second = ++N;
        inversed_order[N] = it->first;
    }
    PersistentTree tree(N);
    for (int i = 1; i <= N; i++) {
        tree.update(order[in[i]]);
    }
    int l, r, k;
    REP(m, M) {
        scanf("%d %d %d", &l, &r, &k);
        int res = tree.query(l, r, k);
        printf("%d\n", inversed_order[res]);
    }

    return 0;
}
