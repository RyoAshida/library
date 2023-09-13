template<typename T>
class Dinic {
public:
    static constexpr T INF = 1e9+9;
    static const int MAX_N = 110;
    struct edge {
        int to;
        T cap;
        int rev;
    };

    vector<edge> G[MAX_N];
    int level[MAX_N];
    int iter[MAX_N];

    void add_edge(int from, int to, T cap) {
        G[from].push_back((edge){to, cap, (int)G[to].size()});
        G[to].push_back((edge){from, 0, (int)G[from].size()-1});
    }

    void bfs(int s) {
        memset(level, -1, sizeof(level));
        queue<int> que;
        level[s] = 0;
        que.push(s);
        while (!que.empty()) {
            int v = que.front(); que.pop();
            for (int i = 0; i < G[v].size(); i++) {
                edge &e = G[v][i];
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[v] + 1;
                    que.push(e.to);
                }
            }
        }
    }
    T dfs(int v, int t, T f) {
        if (v == t) return f;
        for (int &i = iter[v]; i < G[v].size(); i++) {
            edge &e = G[v][i];
            if (e.cap > 0 and level[v] < level[e.to]) {
                T d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    G[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    T calc(int s, int t) {
        T flow = 0;
        while (true) {
            bfs(s);
            if (level[t] < 0) return flow;
            memset(iter, 0, sizeof(iter));
            T f;
            while ((f = dfs(s, t, INF)) > 0) {
                flow += f;
            }
        }
    }
};
