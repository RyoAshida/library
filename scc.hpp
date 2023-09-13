class SCC {
public:
    static const int MAX_V;
    int N;
    vector<int> G[MAX_V];
    vector<int> rG[MAX_V];
    vector<int> vs;
    bool used[MAX_V];
    int cmp[MAX_V];              // 頂点が所属する強連結成分のインデックス
    vector<int> cmp_rev[MAX_V];  // 各強連結成分に所属する頂点
    vector<int> dG[MAX_V];       // 強連結成分分解後のDAG

    SCC(const int N): N(N) {}

    void add_edge(int from, int to) {
        G[from].push_back(to);
        rG[to].push_back(from);
    }

    void dfs(int v) {
        used[v] = true;
        for (int i = 0; i < G[v].size(); i++) {
            if (!used[G[v][i]]) dfs(G[v][i]);
        }
        vs.push_back(v);
    }

    void rdfs(int v, int k) {
        used[v] = true;
        cmp[v] = k;
        cmp_rev[k].push_back(v);
        for (int i = 0; i < rG[v].size(); i++) {
            if (!used[rG[v][i]]) rdfs(rG[v][i], k);
        }
    }

    // 強連結成分の個数を計算する
    int calc() {
        memset(used, 0, sizeof(used));
        vs.clear();
        for (int v = 0; v < N; v++) {
            if (!used[v]) dfs(v);
        }
        memset(used, 0, sizeof(used));
        int k = 0;
        for (int i = vs.size() -1; i >= 0; i--) {
            if (!used[vs[i]]) rdfs(vs[i], k++);
        }

        // DAGを作成する
        for (int i = 0; i < k; i++) {
            set<int> s;
            for (const int v : cmp_rev[i]) {
                for (const int u : G[v]) {
                    s.insert(cmp[u]);
                }
            }
            for (const int idx : s) {
                if (i == idx) continue;
                dG[i].push_back(idx);
            }
        }
        
        return k;
    }
};
