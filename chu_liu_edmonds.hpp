template<typename T>
class ChuLiuEdmonds {
private:
    const T INF;
    
    class UnionFind {
    private:
        vector<int> par;
        vector<int> r;
    public:
        UnionFind(int n) {
            par = vector<int>(n);
            r   = vector<int>(n);
            for (int i = 0; i < n; i++) {
                par[i] = i;
                r[i] = 0;
            }
        }

        int find(int x) {
            if (par[x] == x) {
                return x;
            } else {
                return par[x] = find(par[x]);
            }
        }

        void unite(int x, int y) {
            x = find(x);
            y = find(y);
            if (x == y) return;
            if (r[x] < r[y]) {
                par[x] = y;
            } else {
                par[y] = x;
                if (r[x] == r[y]) r[x]++;
            }
        }

        bool same(int x, int y) {
            return find(x) == find(y);
        }
    };

public:
    const int N;
    struct edge {
        int from, to;
        T cost;
    };

    vector<edge> edges;
    vector<vector<int>> F_in;   // 頂点に入ってくる辺のインデックス
    vector<vector<int>> F_out;  // 頂点から出ていく辺のインデックス

    ChuLiuEdmonds(const int N): N(N), F_in(N, vector<int>()), F_out(N, vector<int>()) {}

    void add_edge(const int from, const int to, const T cost) {
        const int idx = (int)edges.size();
        F_out[from].push_back(idx);
        F_in[to].push_back(idx);
        edges.push_back(edge{from, to, cost});
    }

    pair<T, vector<int>> calc(const int r) {
        UnionFind uf(N);
        vector<int> parents(N, -1);
        edge cycle_edge = edge{-1, -1, -1};
        T weight = 0;
        for (int i = 0; i < N; i++) {
            if (i == r) continue;
            edge min_edge = edge{-1, -1, INF};
            for (const int idx : F_in[i]) {
                const edge& e = edges[idx];
                if (e.cost < min_edge.cost) {
                    min_edge = e;
                }
            }
            if (uf.same(i, min_edge.from)) {
                cycle_edge = min_edge;
            }
            uf.unite(i, min_edge.from);
            parents[i] = min_edge.from;
            weight += min_edge.cost;
        }

        if (cycle_edge.from >= 0) {
            vector<int> cycle_v;
            bool on_cycle[N];
            memset(on_cycle, false, sizeof(on_cycle));
            int v = cycle_edge.to;
            do {
                cycle_v.push_back(v);
                on_cycle[v] = true;
                v = parents[v];
            } while (v != cycle_edge.to);

            T cost_from_cycle[N];
            T cost_to_cycle[N];
            int sink_idx[N];
            int source_idx[N];
            for (int i = 0; i < N; i++) cost_from_cycle[i] = cost_to_cycle[i] = INF;
            memset(sink_idx, -1, sizeof(sink_idx));
            memset(source_idx, -1, sizeof(source_idx));
            for (int i = 0; i < N; i++) {
                if (!on_cycle[i]) continue;
                T min_cost = INF;
                for (const int idx : F_in[i]) {
                    min_cost = min(min_cost, edges[idx].cost);
                }
                for (const int idx : F_in[i]) {
                    if (cost_to_cycle[edges[idx].from] > edges[idx].cost-min_cost) {
                        cost_to_cycle[edges[idx].from] = edges[idx].cost-min_cost;
                        sink_idx[edges[idx].from] = i;
                    }
                }
                for (const int idx : F_out[i]) {
                    if (cost_from_cycle[edges[idx].to] > edges[idx].cost) {
                        cost_from_cycle[edges[idx].to] = edges[idx].cost;
                        source_idx[edges[idx].to] = i;
                    }
                }
            }
            
            int new_idx[N];
            int new_idx_rev[N];
            memset(new_idx, -1, sizeof(new_idx));
            memset(new_idx_rev, -1, sizeof(new_idx_rev));
            int idx = 0;
            for (int i = 0; i < N; i++) {
                if (on_cycle[i]) continue;
                new_idx[i] = idx;
                new_idx_rev[idx] = i;
                idx++;
            }

            ChuLiuEdmonds cle(idx+1);
            bool used[2][N];
            memset(used, false, sizeof(used));
            for (const edge& e : edges) {
                if (on_cycle[e.from] && on_cycle[e.to]) continue;
                if (on_cycle[e.from]) {
                    if (used[0][e.to]) continue;
                    const int to_idx = new_idx[e.to];
                    cle.add_edge(idx, to_idx, cost_from_cycle[e.to]);
                    used[0][e.to] = true;
                } else if (on_cycle[e.to]) {
                    if (used[1][e.from]) continue;
                    const int from_idx = new_idx[e.from];
                    cle.add_edge(from_idx, idx, cost_to_cycle[e.from]);
                    used[1][e.from] = true;
                } else {
                    cle.add_edge(new_idx[e.from], new_idx[e.to], e.cost);
                }
            }

            const vector<int>& ret = cle.calc(new_idx[r]).second;
            const int to_cycle_idx = new_idx_rev[ret[idx]];
            const int target_cycle_idx = sink_idx[to_cycle_idx];
            weight = 0;
            for (int i = 0; i < N; i++) {
                if (i == r) continue;
                if (on_cycle[i]) {
                    if (i == target_cycle_idx) {
                        parents[i] = to_cycle_idx;
                    }
                    for (const int j : F_in[i]) {
                        if (edges[j].from == parents[i]) {
                            weight += edges[j].cost;
                            break;
                        }
                    }
                } else {
                    const int nidx = new_idx[i];
                    if (ret[nidx] == idx) {
                        parents[i] = source_idx[i];
                        weight += cost_from_cycle[i];
                    } else {
                        parents[i] = new_idx_rev[ret[nidx]];
                        for (const int j : F_in[i]) {
                            if (edges[j].from == parents[i]) {
                                weight += edges[j].cost;
                                break;
                            }
                        }
                    }
                }
            }
        }
        
        return make_pair(weight, parents);
    }
};
