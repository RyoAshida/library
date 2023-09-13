// 無向オイラー路
class EulerPath {
public:
    const int N;
    vector<vector<int>> G;
    vector<vector<int>> adj;

    EulerPath(const int N): N(N), G(N, vector<int>()), adj(N, vector<int>(N)) {}

    void add_edge(const int v, const int u) {
        G[v].push_back(u);
        G[u].push_back(v);
        adj[v][u]++;
        adj[u][v]++;
    }

    void visit(const int s, vector<int>& path) {
        for (const int v : G[s]) {
            if (adj[s][v] > 0) {
                adj[s][v]--;
                adj[v][s]--;
                visit(v, path);
            }
        }
        path.push_back(s);
    }

    bool calc(const int s, vector<int>& path) {
        int odd = 0, m = 0;
        for (int i = 0; i < N; i++) {
            if (G[i].size() % 2 == 1) odd++;
            m += G[i].size();
        }
        m /= 2;
        if (odd == 0 || (odd == 2 && G[s].size % 2 == 1)) {
            path.clear();
            visit(s, path);
            reverse(path.begin(), path.end());
            return path.size() == m + 1;
        }
        return false;
    }
};
