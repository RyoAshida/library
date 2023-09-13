template<typename T>
class HLDecomposition {
    static const T init = 0;  // モノイドの単位元
    // モノイドの演算
    static T op(const T& a, const T& b) {
        return a+b;
    }
    // op(a, a,..., a) (aがkコ)の演算
    static T prop(const T& a, int k) {
        return a*k;
    }
    
    // Range Add&Update Query
    class SegmentTree {
        class Node {
        public:
            ll l, r;
            T data;
            T lazy;
            bool wait_add, wait_update; // 同時にtrueになることはない
            Node* child_L;
            Node* child_R;
            Node(ll l, ll r, T data) {
                this->l = l;
                this->r = r;
                this->data = data;
                this->lazy = 0;
                this->wait_add = false;
                this->wait_update = false;
                this->child_L = nullptr;
                this->child_R = nullptr;
            }

            void make_children() {
                if (this->child_L == nullptr) {
                    ll mid = (this->l + this->r) / 2;
                    this->child_L = new Node(this->l, mid, init);
                    this->child_R = new Node(mid, this->r, init);
                }
            }

            void lazy_evaluate() {
                if (this->wait_add) {
                    this->data += prop(this->lazy, this->r - this->l);

                    if (this->r - this->l > 1) {
                        this->make_children();

                        // 左の子の遅延評価に加算
                        if (!this->child_L->wait_add && !this->child_L->wait_update) {
                            this->child_L->wait_add = true;
                        }
                        this->child_L->lazy += this->lazy;
                        // 右の子の遅延評価に加算
                        if (!this->child_R->wait_add && !this->child_R->wait_update) {
                            this->child_R->wait_add = true;
                        }
                        this->child_R->lazy += this->lazy;
                    }
                    this->lazy = 0;
                    this->wait_add = false;
                } else if (this->wait_update) {
                    this->data = prop(this->lazy, this->r - this->l);

                    if (this->r - this->l > 1) {
                        this->make_children();

                        // 左の子の更新を上書き
                        this->child_L->wait_add = false;
                        this->child_L->wait_update = true;
                        this->child_L->lazy = this->lazy;
                        // 右の子の更新を上書き
                        this->child_R->wait_add = false;
                        this->child_R->wait_update = true;
                        this->child_R->lazy = this->lazy;
                    }
                    this->lazy = 0;
                    this->wait_update = false;
                }
            }
        };

        Node* root;
        ll N;
    public:
        SegmentTree(ll n) {
            this->N = n;
            this->root = new Node(0, n, init);
        }

        void update(Node* node, ll k, T a) {
            node->lazy_evaluate();
            if (node->l <= k && k < node->r) {
                if (node->l == k && node->r == k+1) {
                    node->data = a;
                } else {
                    node->make_children();
                    update(node->child_L, k, a);
                    update(node->child_R, k, a);
                    node->data = op(node->child_L->data, node->child_R->data);
                }
            }
        }
        void update(ll k, T a) {
            update(this->root, k, a);
        }

        void update_range(Node* node, ll l, ll r, T a) {
            node->lazy_evaluate();
            if (node->r <= l || r <= node->l) return;

            if (l <= node->l && node->r <= r) {
                node->lazy = a;
                node->wait_update = true;
                node->lazy_evaluate();
            } else {
                node->make_children();
                update_range(node->child_L, l, r, a);
                update_range(node->child_R, l, r, a);
                node->data = op(node->child_L->data, node->child_R->data);
            }
        }
        void update_range(ll l, ll r, T a) {
            if (l < r)
                update_range(this->root, l, r, a);
        }

        void add(Node* node, ll k, T a) {
            node->lazy_evaluate();
            if (node->l <= k && k < node->r) {
                if (node->l == k && node->r == k+1) {
                    node->data += a;
                } else {
                    node->make_children();
                    add(node->child_L, k, a);
                    add(node->child_R, k, a);
                    node->data = op(node->child_L->data, node->child_R->data);
                }
            }
        }
        void add(ll k, T a) {
            add(this->root, k, a);
        }

        void add_range(Node* node, ll l, ll r, T a) {
            node->lazy_evaluate();
            if (node->r <= l || r <= node->l) return;

            if (l <= node->l && node->r <= r) {
                node->lazy = a;
                node->wait_add = true;
                node->lazy_evaluate();
            } else {
                node->make_children();
                add_range(node->child_L, l, r, a);
                add_range(node->child_R, l, r, a);
                node->data = op(node->child_L->data, node->child_R->data);
            }
        }
        void add_range(ll l, ll r, T a) {
            if (l < r)
                add_range(this->root, l, r, a);
        }

        T query(Node* node, ll l, ll r) {
            if (node == nullptr || node->r <= l || r <= node->l) return init;
            node->lazy_evaluate();
            if (l <= node->l && node->r <= r) return node->data;
            return op(query(node->child_L, l, r), query(node->child_R, l, r));
        }
    
        T query(ll l, ll r) {
            return query(this->root, l, r);
        }

        T get(ll i) {
            return query(this->root, i, i+1);
        }
    };
    
    const int N;
    int root = 0;
    vector<vector<int>> G;
    vector<vector<int>> children;
    vector<int> sz;          // 部分木のサイズ
    vector<int> depth;       // 根からの深さ
    vector<int> hl_idx;      // HL分解後のインデックス
    vector<int> inv_idx;     // hl_idxの逆写像
    vector<int> hl_idx_out;  // 部分木の終わりのインデックス
    /*** 以下HL分解後のインデックスで管理 ***/
    vector<int> head;        // heavy-pathの根に一番近い頂点のインデックス
    vector<int> parent;      // 親のインデックス

    SegmentTree seg_tree;

    void dfs(int v, int prev, int d) {
        sz[v] = 1;
        depth[v] = d;
        for (int _u : G[v]) {
            if (_u == prev) continue;
            children[v].push_back(_u);
            int& u = children[v].back();
            dfs(u, v, d+1);
            sz[v] += sz[u];
            if (sz[u] > sz[children[v][0]]) {
                swap(u, children[v][0]);
            }
        }
    }

    void dfs_hld(int v, int prev, int& t) {
        hl_idx[v] = t;
        inv_idx[t] = v;
        parent[t] = (prev >= 0 ? hl_idx[prev] : -1);
        t++;
        for (int u : children[v]) {
            // hl_idx[u] = t;
            head[t] = (u == children[v][0] ? head[hl_idx[v]] : t);
            dfs_hld(u, v, t);
        }
        hl_idx_out[v] = t;
    }

public:
    HLDecomposition(int sz, int rt = 0):
        root(rt), N(sz), G(N), children(N), sz(N), depth(N),
        hl_idx(N), inv_idx(N), hl_idx_out(N), head(N), parent(N), seg_tree(N){}

    void add_edge(int u, int v) {
        G[u].push_back(v);
        G[v].push_back(u);
    }

    void build() {
        dfs(this->root, -1, 0);
        head[0] = this->root;
        int t = 0;
        dfs_hld(this->root, -1, t);
    }

    void update_vertex(int v, T a) {
        seg_tree.update(hl_idx[v], a);
    }

    void add_vertex(int v, T a) {
        seg_tree.add(hl_idx[v], a);
    }

    void update_subtree(int v, T a) {
        seg_tree.update_range(hl_idx[v], hl_idx_out[v], a);
    }

    void add_subtree(int v, T a) {
        seg_tree.add_range(hl_idx[v], hl_idx_out[v], a);
    }

    void update_path(int u, int v, T a) {
        u = hl_idx[u];
        v = hl_idx[v];
        while (true) {
            if (u > v) swap(u, v);
            seg_tree.update_range(max(head[v], u), v+1, a);
            if (head[u] == head[v]) break;
            v = parent[head[v]];
        }
    }

    void add_path(int u, int v, T a) {
        u = hl_idx[u];
        v = hl_idx[v];
        while (true) {
            if (u > v) swap(u, v);
            seg_tree.add_range(max(head[v], u), v+1, a);
            if (head[u] == head[v]) break;
            v = parent[head[v]];
        }
    }

    T query_subtree(int u) {
        return seg_tree.query(hl_idx[u], hl_idx_out[u]);
    }

    T query_path(int u, int v) {
        u = hl_idx[u];
        v = hl_idx[v];
        T res = init;
        while (true) {
            if (u > v) swap(u, v);
            res = op(res, seg_tree.query(max(head[v], u), v+1));
            if (head[u] == head[v]) break;
            v = parent[head[v]];
        }
        return res;
    }
    
    T get(int v) {
        return seg_tree.get(hl_idx[v]);
    }

    int lca(int u, int v) {
        u = hl_idx[u];
        v = hl_idx[v];
        while (true) {
            if (u > v) swap(u, v);
            if (head[u] == head[v]) break;
            v = parent[head[v]];
        }
        return inv_idx[u];
    }

    int distance(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u,v)];
    }
};
