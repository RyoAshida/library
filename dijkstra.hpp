template<typename T>
class Dijkstra {
public:
    using U = pair<T, int>;
    static constexpr T INF = 1e9+9;
    static const int MAX_N;
    struct edge {
        int to;
        T cost;
    };

    vector<vector<edge>> G;
    vector<T> dist;
    Dijkstra(): G(MAX_N, vector<edge>()), dist(MAX_N) {}

    void add_edge(const int from, const int to, const T cost) {
        G[from].push_back(edge{to, cost});
    }
    
    void calc(int s) {
        priority_queue<U, vector<U>, greater<U>> que;
        for (int i = 0; i < MAX_N; i++) dist[i] = INF;
        dist[s] = 0;
        que.push(U(0, s));
        while (!que.empty()) {
            const U p = que.top(); que.pop();
            const int v = p.second;
            if (dist[v] < p.first) continue;
            for (const edge& e : G[v]) {
                if (dist[e.to] > dist[v] + e.cost) {
                    dist[e.to] = dist[v] + e.cost;
                    que.push(U(dist[e.to], e.to));
                }
            }
        }
    }
};
