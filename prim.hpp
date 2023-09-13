template<typename T>
class Prim {
public:
    static const int MAX_N;
    using P = pair<T, int>;
    struct edge {
        int to;
        T cost;
    };

    vector<vector<edge>> G;
    bool used[MAX_N];
    Prim(): G(MAX_N, vector<edge>()) {}

    void add_edge(const int from, const int to, const T cost) {
        G[from].push_back(edge{to, cost});
    }
    
    T calc() {
        priority_queue<P, vector<P>, greater<P>> que;
        fill(used, used+MAX_N, false);
        for (int i = 0; i < G[0].size(); i++) {
            const edge& e = G[0][i];
            que.push(P(e.cost, e.to));
        }
        used[0] = true;
        T res = 0;
        while (!que.empty()) {
            const P p = que.top(); que.pop();
            const int v = p.second;
            if (used[v]) continue;
            used[v] = true;
            res += p.first;
            for (int i = 0; i < G[v].size(); i++) {
                const edge& e = G[v][i];
                if (!used[e.to]) {
                    que.push(P(e.cost, e.to));
                }
            }
        }
        return res;
    }
};
