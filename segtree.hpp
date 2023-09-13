template<typename T>
class SegmentTree {
    class Node {
    public:
        ll l, r;
        T data;
        Node* child_L;
        Node* child_R;
        Node(ll l, ll r, T data) {
            this->l = l;
            this->r = r;
            this->data = data;
            this->child_L = nullptr;
            this->child_R = nullptr;
        }
    };

    Node* root;
    ll N;
    const T init;
    static const ll MAX_N;
    static T op(const T& a, const T& b) {
        return a+b;
    }
public:
    SegmentTree(ll n = MAX_N) {
        this->N = n;
        this->root = new Node(0, n, init);
    }

    void reset(ll n) {
        this->N = n;
        this->root = new Node(0, n, init);
    }

    ll size() {
        return this->N;
    }

    void update(Node* node, ll k, T a) {
        if (node->l <= k && k < node->r) {
            if (node->l == k && node->r == k+1) {
                node->data = a;
            } else {
                if (node->child_L == nullptr) {
                    ll mid = (node->l + node->r) / 2;
                    node->child_L = new Node(node->l, mid, init);
                    node->child_R = new Node(mid, node->r, init);
                }
                update(node->child_L, k, a);
                update(node->child_R, k, a);
                node->data = op(node->child_L->data, node->child_R->data);
            }
        }
    }
    void update(ll k, T a) {
        update(this->root, k, a);
    }

    void add(Node* node, ll k, T a) {
        if (node->l <= k && k < node->r) {
            if (node->l == k && node->r == k+1) {
                node->data += a;
            } else {
                if (node->child_L == nullptr) {
                    ll mid = (node->l + node->r) / 2;
                    node->child_L = new Node(node->l, mid, init);
                    node->child_R = new Node(mid, node->r, init);
                }
                add(node->child_L, k, a);
                add(node->child_R, k, a);
                node->data = op(node->child_L->data, node->child_R->data);
            }
        }
    }
    void add(ll k, T a) {
        add(this->root, k, a);
    }

    T query(Node* node, ll l, ll r) {
        if (node == nullptr || node->r <= l || r <= node->l) return init;
        if (l <= node->l && node->r <= r) return node->data;
        return op(query(node->child_L, l, r), query(node->child_R, l, r));
    }
    
    T query(ll l, ll r) {
        return query(this->root, l, r);
    }

    T get(ll i) {
        return query(this->root, i, i+1);
    }

    static void replace(Node* node0, Node* node1, ll l, ll r) {
        if (node0->child_L != nullptr) {
            // left child
            if (l <= node0->child_L->l && node0->child_L->r <= r) swap(node0->child_L, node1->child_L);
            else if (node0->child_L->l < r && node0->child_L->r > l) {
                replace(node0->child_L, node1->child_L, l, r);
            }
            // right child
            if (l <= node0->child_R->l && node0->child_R->r <= r) swap(node0->child_R, node1->child_R);
            else if (node0->child_R->l < r && node0->child_R->r > l) {
                replace(node0->child_R, node1->child_R, l, r);
            }

            node0->data = op(node0->child_L->data, node0->child_R->data);
            node1->data = op(node1->child_L->data, node1->child_R->data);
        }
    }

    static void replace(SegmentTree& s0, SegmentTree& s1, ll l, ll r) {
        if (l == 0 && r == s0.size()) {
            swap(s0.root, s1.root);
        } else {
            replace(s0.root, s1.root, l, r);
        }
    }
};

// Range Add&Update Query
template<typename T>
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
    static const T init = 0;  // モノイドの単位元
    // モノイドの演算
    static T op(const T& a, const T& b) {
        return a + b;
    }
    // op(a, a,..., a) (aがkコ) の演算
    static T prop(const T& a, int k) {
        return a * k;
    }
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
