template<typename T>
class LKH {
    const int N;
    vector<vector<T>> d;
    vector<T> penalty;

    unsigned long xor128(void) {
        static unsigned long x=123456789,y=362436069,z=521288629,w=88675123;
        unsigned long t;
        t=(x^(x<<11));x=y;y=z;z=w; return( w=(w^(w>>19))^(t^(t>>8)) );
    }

    // [0, k)
    int rand(int k) {
        return xor128() % k;
    }
    
public:
    vector<vector<int>> cand_set;
    using Tree = vector<vector<int>>;  // 隣接リスト
    
    LKH(const int N):
        N(N),
        d(N, vector<T>(N)),
        cand_set(N, vector<int>()) {}

    void set_distances(const vector<vector<T>>& _d) {
        d = _d;
    }

    tuple<double, Tree, pair<int, int>> calc_minimum_1_tree(const vector<T>& pi) {
        // O(N^2) の Prim法
        bool used[N];
        T min_cost[N];
        int source[N];
        memset(used, false, sizeof(used));
        used[0] = true;
        for (int i = 0; i < N; i++) min_cost[i] = d[0][i] + pi[0] + pi[i];
        memset(source, 0, sizeof(source));

        Tree m1tree(N, vector<int>());
        T w = 0;
        for (int iter = 0; iter < N-1; iter++) {
            int v_next, v_source;
            T min_c = numeric_limits<T>::infinity();
            for (int i = 0; i < N; i++) {
                if (!used[i] && min_c > min_cost[i]) {
                    min_c = min_cost[i];
                    v_next = i;
                    v_source = source[i];
                }
            }

            m1tree[v_source].push_back(v_next);
            m1tree[v_next].push_back(v_source);
            used[v_next] = true;
            w += min_c;
            for (int i = 0; i < N; i++) {
                if (!used[i] && d[v_next][i] + pi[v_next] + pi[i] < min_cost[i]) {
                    min_cost[i] = d[v_next][i] + pi[v_next] + pi[i];
                    source[i] = v_next;
                }
            }
        }

        // 辺を1本追加する
        // 追加する辺はある葉ノードから出るMSTに含まれない最小の辺
        // 葉ノードの選び方は、上記の辺の長さが最大のもの
        int u, v;
        T max_cost = 0;
        for (int i = 0; i < N; i++) {
            if ((int)m1tree[i].size() > 1) continue;

            T c = numeric_limits<T>::infinity();
            int k;
            for (int j = 0; j < N; j++) {
                if (!(j == i || j == m1tree[i][0]) && c > d[i][j] + pi[i] + pi[j]) {
                    c = d[i][j] + pi[i] + pi[j];
                    k = j;
                }
            }

            if (max_cost < c) {
                max_cost = c;
                u = i;
                v = k;
            }
        }
        m1tree[u].push_back(v);
        m1tree[v].push_back(u);
        w += max_cost;
        w -= 2 * accumulate(pi.begin(), pi.end(), (T)0);

        return make_tuple(w, m1tree, make_pair(u, v));
    }

    tuple<double, Tree, pair<int, int>> ascent() {
        vector<T> pi(N), best_pi;
        vector<int> last_deg(N);
        T best_w = 0;

        const int INITIAL_STEP_SIZE = 1;
        const int INITIAL_PERIOD = N/2;
        bool initial_phase = true;
        for (int t = INITIAL_STEP_SIZE, period = INITIAL_PERIOD; t > 0; period /= 2, t /= 2) {
            for (int p = 1; t > 0 && p <= period; p++) {
                cerr << "t: " << t << ", p: " << p << endl;
                const auto res = calc_minimum_1_tree(pi);
                const T w = get<0>(res);
                if (w > best_w) {
                    best_w = w;
                    best_pi = pi;
                    if (initial_phase) t *= 2;
                    if (p == period) period *= 2;
                } else if (initial_phase && p > INITIAL_PERIOD/2) {
                    initial_phase = false;
                    p = 0;
                    t = 3 * t / 4;
                }

                const Tree& tree = get<1>(res);
                for (int i = 0; i < N; i++) {
                    const int degree = (int)tree[i].size() - 2;
                    if (degree != 0) {
                        pi[i] += t * (7 * degree + 3 * last_deg[i]) / 10;
                    }
                    last_deg[i] = degree;
                }
            }
        }

        penalty = best_pi;
        return calc_minimum_1_tree(best_pi);
    }

    void dfs_candset(const int v, const Tree& tree, const pair<int, int>& no_use_edge, vector<T>& beta) {
        for (const int u : tree[v]) {
            if (beta[u] >= 0 ||
                no_use_edge == make_pair(v, u) || no_use_edge == make_pair(u, v)) continue;
            beta[u] = max(beta[v], d[v][u] + penalty[v] + penalty[u]);
            dfs_candset(u, tree, no_use_edge, beta);
        }
    }
    
    void create_candidate_set() {
        const auto ret = ascent();
        const Tree tree = get<1>(ret);
        const auto no_use_edge = get<2>(ret);
        vector<T> beta(N);
        vector<pair<T, int>> alpha(N);
        
        for (int i = 0; i < N; i++) {
            fill(beta.begin(), beta.end(), (T)-1);
            beta[i] = 0;
            dfs_candset(i, tree, no_use_edge, beta);

            for (int j = 0; j < N; j++) {
                if (j == i) {
                    alpha[j] = make_pair(numeric_limits<T>::infinity(), j);
                } else {
                    alpha[j] = make_pair(d[i][j] + penalty[i] + penalty[j] - beta[j], j);
                }
            }
            sort(alpha.begin(), alpha.end());
            for (int j = 0; j < 5; j++) {
                cand_set[i].push_back(alpha[j].second);
            }
        }
    }

    vector<int> make_initial_tour() {
        vector<int> rem(N), idx(N);
        for (int i = 0; i < N; i++) {
            rem[i] = idx[i] = i;
        }

        vector<int> p(N);
        p[0] = rand(N);
        swap(rem[N-1], rem[p[0]]);
        idx[N-1] = p[0];
        idx[p[0]] = N-1;
        for (int i = 1; i < N; i++) {
            const int R = N-i;
            bool selected = false;
            for (const int j : cand_set[p[i-1]]) {
                if (idx[j] >= R) continue;
                p[i] = j;
                const int temp0 = rem[R-1];
                const int temp1 = idx[j];
                swap(rem[R-1], rem[idx[j]]);
                idx[j] = R-1;
                idx[temp0] = temp1;
                selected = true;
                break;
            }
            if (!selected) {
                const int r = rand(R);
                p[i] = rem[r];
                idx[rem[r]] = R-1;
                idx[rem[R-1]] = r;
                swap(rem[R-1], rem[r]);
            }
        }

        return p;
    }

    void best_move(const int t0, vector<int>& p, vector<int>& idx) {
        T G0, G1, G2;
        for (int d0 = 0; d0 < 2; d0++) {
            const int t1 = (d0 == 0 ? p[(idx[t0]+1)%N] : p[(idx[t0]+N-1)%N]);
            G0 = d[t0][t1];
            for (const int t2 : cand_set[t1]) {
                if (t2 == p[(idx[t1]+1)%N] || t2 == p[(idx[t1]+N-1)%N] ||
                    (G1 = G0 - d[t1][t2]) <= 0) {
                    continue;
                }
                for (int d1 = 0; d1 < 2; d1++) {
                    const int t3 = (d1 == 0 ? p[(idx[t2]+1)%N] : p[(idx[t2]+N-1)%N]);
                    G2 = G1 + d[t2][t3];
                    // 2-opt
                    if (d0 != d1 && t3 != t0 && G2 - d[t3][t0] > 0) {
                        int s, e;
                        if (d0 == 0) {
                            if (idx[t1] < idx[t3]) {
                                s = idx[t1];
                                e = idx[t3] + 1;
                            } else {
                                s = idx[t2];
                                e = idx[t0] + 1;
                            }
                        } else {
                            if (idx[t3] < idx[t1]) {
                                s = idx[t3];
                                e = idx[t1] + 1;
                            } else {
                                s = idx[t0];
                                e = idx[t2] + 1;
                            }
                        }
                        reverse(p.begin()+s, p.begin()+e);
                        for (int i = s; i < e; i++) {
                            idx[p[i]] = i;
                        }
                        return;
                    }
                    for (const int t4 : cand_set[t3]) {
                        if (abs(idx[t4] - idx[t0]) <= 1 ||
                            abs(idx[t4] - idx[t1]) <= 1 ||
                            abs(idx[t4] - idx[t2]) <= 1 ||
                            abs(idx[t4] - idx[t3]) <= 1 ||
                            (G3 = G2 - d[t3][t4]) <= 0) {
                            continue;
                        }
                        for (int d2 = 0; d2 < 2; d2++) {
                            const int t5 = (d2 == 0 ? p[(idx[t4]+1)%N] : p[(idx[t4]+N-1)%N]);
                            G4 = G3 + d[t4][t5];
                            if (G4 - d[t5][t0] > 0) {
                                
                            }
                        }
                    }
                }
            }
        }
    }

    vector<int> calc() {
        create_candidate_set();
        vector<int> p = make_initial_tour();
        vector<int> idx(N);
        for (int i = 0; i < N; i++) {
            idx[p[i]] = i;
        }
        for (int iter = 0; iter < 10000; iter++) {
            const int t0 = rand(N);
            best_move(t0, p, idx);
        }
        return p;
    }
};
