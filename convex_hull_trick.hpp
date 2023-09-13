template<typename T>
class ConvexHullTrick {
    class Node {
    public:
        ll l, r; // [l, r]
        T a, b;  // 直線 ax+b
        Node* child_L;
        Node* child_R;
        Node(ll l, ll r) {
            this->l = l;
            this->r = r;
            this->a = 0;
            this->b = init;
            this->child_L = nullptr;
            this->child_R = nullptr;
        }
    };

    Node* root;
    ll N;
    //const T init = -2e18;  // 最大値を求める
    static const T init = 2e18;  // 最小値を求める
    static const ll MAX_N;
    static bool op(const T& a, const T& b) {
        //return a >= b;  // 最大値を求める
        return a <= b;  // 最小値を求める
    }
public:
    ConvexHullTrick(ll n = MAX_N) {
        this->N = n;
        this->root = new Node(0, n);
    }

    void reset(ll n) {
        this->N = n;
        this->root = new Node(0, n);
    }

    void add(Node* node, T a, T b) {
        const ll xl = node->l;
        const ll xr = node->r;
        const ll xm = (xl + xr) / 2;
        T vl = node->a * xl + node->b;
        T vr = node->a * xr + node->b;
        T vm = node->a * xm + node->b;
        T nvl = a * xl + b;
        T nvr = a * xr + b;
        T nvm = a * xm + b;
        // (最大値の場合)3点とも元の直線の方が大きいなら何もせず終了
        if (op(vl, nvl) && op(vr, nvr) && op(vm, nvm)) return;
        // (最大値の場合)3点とも新しい直線の方が大きいなら直線を書き換えて終了
        if (!op(vl, nvl) && !op(vr, nvr) && !op(vm, nvm)) {
            node->a = a;
            node->b = b;
            return;
        }
        // (最大値の場合)真ん中の点で新しい直線の方が大きい場合スワップ
        // 新しい直線が左端か右端でのみ大きくなる
        if (!op(vm, nvm)) {
            swap(node->a, a);
            swap(node->b, b);
            swap(vl, nvl);
            swap(vr, nvr);
        }
        // (最大値の場合)新しい直線の方が左端で大きくなる場合
        // 左の子に降りる
        if (!op(vl, nvl)) {
            if (node->child_L == nullptr) {
                ll mid = (node->l + node->r) / 2;
                node->child_L = new Node(node->l, mid);
            }
            add(node->child_L, a, b);
        }
        // (最大値の場合)新しい直線の方が右端で大きくなる場合
        // 右の子に降りる
        else {
            if (node->child_R == nullptr) {
                ll mid = (node->l + node->r) / 2 + 1;
                node->child_R = new Node(mid, node->r);
            }
            add(node->child_R, a, b);
        }
    }
    void add(T a, T b) {
        add(this->root, a, b);
    }

    T query(Node* node, ll x) {
        if (node == nullptr || !(node->l <= x && x <= node->r)) return init;
        T val = node->a * x + node->b;
        T val_l = query(node->child_L, x);
        T val_r = query(node->child_R, x);
        if (op(val_l, val)) val = val_l;
        if (op(val_r, val)) val = val_r;
        return val;
    }
    T query(ll x) {
        return query(this->root, x);
    }
};

// 線分
template<typename T>
class ConvexHullTrick {
    class Node {
    public:
        ll l, r; // [l, r]
        T a, b;  // 直線 ax+b
        Node* child_L;
        Node* child_R;
        Node(ll l, ll r) {
            this->l = l;
            this->r = r;
            this->a = 0;
            this->b = init;
            this->child_L = nullptr;
            this->child_R = nullptr;
        }
    };

    Node* root;
    ll N;
    //const T init = -2e18;  // 最大値を求める
    static const T init = 2e18;  // 最小値を求める
    static const ll MAX_N;
    static bool op(const T& a, const T& b) {
        //return a >= b;  // 最大値を求める
        return a <= b;  // 最小値を求める
    }
public:
    ConvexHullTrick(ll n = MAX_N) {
        this->N = n;
        this->root = new Node(0, n);
    }

    void reset(ll n) {
        this->N = n;
        this->root = new Node(0, n);
    }

    void add(Node* node, T a, T b, ll l, ll r) {
        // 追加区間と被りがなければ何もしない
        if (node->r < l || r < node->l) return;
        // 追加区間に完全に含まれてはいないなら子供に行く
        if (!(l <= node->l && node->r <= r)) {
            if (node->child_L == nullptr) {
                ll mid = (node->l + node->r) / 2;
                node->child_L = new Node(node->l, mid);
            }
            if (node->child_R == nullptr) {
                ll mid = (node->l + node->r) / 2 + 1;
                node->child_R = new Node(mid, node->r);
            }
            add(node->child_L, a, b, l, r);
            add(node->child_R, a, b, l, r);
            return;
        }
        const ll xl = node->l;
        const ll xr = node->r;
        const ll xm = (xl + xr) / 2;
        T vl = node->a * xl + node->b;
        T vr = node->a * xr + node->b;
        T vm = node->a * xm + node->b;
        T nvl = a * xl + b;
        T nvr = a * xr + b;
        T nvm = a * xm + b;
        // (最大値の場合)3点とも元の直線の方が大きいなら何もせず終了
        if (op(vl, nvl) && op(vr, nvr) && op(vm, nvm)) return;
        // (最大値の場合)3点とも新しい直線の方が大きいなら直線を書き換えて終了
        if (!op(vl, nvl) && !op(vr, nvr) && !op(vm, nvm)) {
            node->a = a;
            node->b = b;
            return;
        }
        // (最大値の場合)真ん中の点で新しい直線の方が大きい場合スワップ
        // 新しい直線が左端か右端でのみ大きくなる
        if (!op(vm, nvm)) {
            swap(node->a, a);
            swap(node->b, b);
            swap(vl, nvl);
            swap(vr, nvr);
        }
        // (最大値の場合)新しい直線の方が左端で大きくなる場合
        // 左の子に降りる
        if (!op(vl, nvl)) {
            if (node->child_L == nullptr) {
                ll mid = (node->l + node->r) / 2;
                node->child_L = new Node(node->l, mid);
            }
            add(node->child_L, a, b, l, r);
        }
        // (最大値の場合)新しい直線の方が右端で大きくなる場合
        // 右の子に降りる
        else {
            if (node->child_R == nullptr) {
                ll mid = (node->l + node->r) / 2 + 1;
                node->child_R = new Node(mid, node->r);
            }
            add(node->child_R, a, b, l, r);
        }
    }
    void add(T a, T b, ll l, ll r) {
        add(this->root, a, b, l, r);
    }

    T query(Node* node, ll x) {
        if (node == nullptr || !(node->l <= x && x <= node->r)) return init;
        T val = node->a * x + node->b;
        T val_l = query(node->child_L, x);
        T val_r = query(node->child_R, x);
        if (op(val_l, val)) val = val_l;
        if (op(val_r, val)) val = val_r;
        return val;
    }
    T query(ll x) {
        return query(this->root, x);
    }
};
