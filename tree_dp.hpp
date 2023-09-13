// 全方位木DP 

// verified by abc160_f
// 辺の重みなし
// モノイドの単位元(id)と演算(op)、
// 子供の値を計算した後の最終的な頂点の値の計算(op_last)を記述
template<typename T>
class AllDirTreeDP {
public:
    static const int MAX_N;  // 最大頂点数
    static const ll MOD = 1e9 + 7;
    vector<vector<int>> G;      // グラフ
    vector<ll> sz;              // 部分木のサイズ

    const T id;                          // 単位元
    vector<T> val;                       // 部分木の値
    vector<vector<vector<T>>> val_acc;   // 子供の左右からの累積の値
    vector<T> ans;                       // 頂点の値

    AllDirTreeDP():
        G(MAX_N, vector<int>()),
        sz(MAX_N),
        val(MAX_N),
        val_acc(MAX_N, vector<vector<T>>(2, vector<T>())),
        ans(MAX_N) {}
    
    void calc_subtree_size(const int v, const int parent) {
        sz[v] = 1;
        for (const int u : G[v]) {
            if (u == parent) continue;
            calc_subtree_size(u, v);
            sz[v] += sz[u];
        }
    }

    T op(const T& v, const T& u) {
    }

    void op_last(const int v, T& val) {
    }

    void dfs0(const int v, const int parent) {
        for (const int u : G[v]) {
            if (u == parent) continue;
            dfs0(u, v);
        }

        const int l = (int)G[v].size();
        val[v] = id;
        val_acc[v][0].push_back(id);
        for (int i = 0; i < l; i++) {
            const int u = G[v][i];
            if (u == parent) continue;

            val[v] = op(val[v], val[u]);
            val_acc[v][0].push_back(val[v]);
        }

        val[v] = id;
        val_acc[v][1].push_back(id);
        for (int i = l-1; i >= 0; i--) {
            const int u = G[v][i];
            if (u == parent) continue;

            val[v] = op(val[v], val[u]);
            val_acc[v][1].push_back(val[v]);
        }

        op_last(v, val[v]);
    }

    void dfs1(const int v, const int parent, const T& p_val) {
        ans[v] = op(val_acc[v][0].back(), p_val);
        op_last(v, ans[v]);
        
        int idx = 0;
        const int l = (int)G[v].size() + (parent < 0 ? 1 : 0);
        for (const int u : G[v]) {
            if (u == parent) continue;
            
            T cp_val = p_val;
            cp_val = op(cp_val, val_acc[v][0][idx]);
            cp_val = op(cp_val, val_acc[v][1][l-2-idx]);
            op_last(v, cp_val);
            dfs1(u, v, cp_val);
            idx++;
        }
    }

    void add_edge(const int u, const int v) {
        G[u].push_back(v);
        G[v].push_back(u);
    }
    
    void calc() {
        calc_subtree_size(0, -1);
                
        dfs0(0, -1);
        dfs1(0, -1, id);
    }
};

// 辺の重みあり
// モノイドの単位元(id)と演算(op)、
// 子供の値を計算した後の最終的な頂点の値の計算(op_last)を記述
template<typename T>
class AllDirTreeDP {
public:
    static const int MAX_N;  // 最大頂点数
    static const ll MOD = 1e9 + 7;

    struct edge {
        int to;
        ll cost;
    };
    vector<vector<edge>> G;     // グラフ
    vector<ll> sz;              // 部分木のサイズ

    const T id = make_pair(0, 1);        // 単位元
    vector<T> val;                       // 部分木の値
    vector<vector<vector<T>>> val_acc;   // 子供の左右からの累積の値
    vector<T> ans;                       // 頂点の値

    AllDirTreeDP():
        G(MAX_N, vector<edge>()),
        sz(MAX_N),
        val(MAX_N),
        val_acc(MAX_N, vector<vector<T>>(2, vector<T>())),
        ans(MAX_N) {}
    
    void calc_subtree_size(const int v, const int parent) {
        sz[v] = 1;
        for (const int u : G[v]) {
            if (u == parent) continue;
            calc_subtree_size(u, v);
            sz[v] += sz[u];
        }
    }

    T op(const T& v, const T& u) {
    }

    void op_last(const int v, T& val) {
    }

    void dfs0(const int v, const int parent) {
        for (const int u : G[v]) {
            if (u == parent) continue;
            dfs0(u, v);
        }

        const int l = (int)G[v].size();
        val[v] = id;
        val_acc[v][0].push_back(id);
        for (int i = 0; i < l; i++) {
            const int u = G[v][i];
            if (u == parent) continue;

            val[v] = op(val[v], val[u]);
            val_acc[v][0].push_back(val[v]);
        }

        val[v] = id;
        val_acc[v][1].push_back(id);
        for (int i = l-1; i >= 0; i--) {
            const int u = G[v][i];
            if (u == parent) continue;

            val[v] = op(val[v], val[u]);
            val_acc[v][1].push_back(val[v]);
        }

        op_last(v, val[v]);
    }

    void dfs1(const int v, const int parent, const T& p_val) {
        ans[v] = op(val_acc[v][0].back(), p_val);
        op_last(v, ans[v]);
        
        int idx = 0;
        const int l = (int)G[v].size() + (parent < 0 ? 1 : 0);
        for (const int u : G[v]) {
            if (u == parent) continue;
            
            T cp_val = p_val;
            cp_val = op(cp_val, val_acc[v][0][idx]);
            cp_val = op(cp_val, val_acc[v][1][l-2-idx]);
            op_last(v, cp_val);
            dfs1(u, v, cp_val);
            idx++;
        }
    }

    void add_edge(const int u, const int v, const ll cost) {
        G[u].push_back(edge{v, cost});
        G[v].push_back(edge{u, cost});
    }
    
    void calc() {
        calc_subtree_size(0, -1);
                
        dfs0(0, -1);
        dfs1(0, -1, id);
    }
};
