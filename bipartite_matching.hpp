class BipartiteMatching {
public:
    static const int MAX_N = 10;
    int N;
    std::vector<int> G[MAX_N];
    int match[MAX_N];
    bool used[MAX_N];

    BipartiteMatching(const int N): N(N) {}

    void add_edge(const int u, const int v) {
        G[u].push_back(v);
        G[v].push_back(u);
    }

    bool dfs(const int v) {
        used[v] = true;
        for (int i = 0; i < G[v].size(); i++) {
            const int u = G[v][i], w = match[u];
            if (w < 0 || (!used[w] && dfs(w))) {
                match[v] = u;
                match[u] = v;
                return true;
            }
        }
        return false;
    }

    int calc() {
        int res = 0;
        memset(match, -1, sizeof(match));
        for (int v = 0; v < N; v++) {
            if (match[v] < 0) {
                memset(used, 0, sizeof(used));
                if (dfs(v)) 
                    res++;
            }
        }
        return res;
    }
};
