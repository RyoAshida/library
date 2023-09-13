template<typename T>
class Kruskal {
public:
    static const int MAX;
    int par[MAX];
    int r[MAX];

    struct edge {
        int u, v;
        T cost;
    };

    static bool compare(const edge &e1, const edge &e2) {
        return e1.cost < e2.cost;
    }

    vector<edge> es;
    int N = 0;
    void add_edge(const int u, const int v, const T cost) {
        es.push_back(edge{u, v, cost});
        N = max(N, max(u, v)+1);
    }

    void init() {
        for (int i = 0; i < N; i++) {
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

    T calc() {
        sort(es.begin(), es.end(), compare);
        init();
        T res = 0;
        for (const edge& e : es) {
            if (!same(e.u, e.v)) {
                unite(e.u, e.v);
                res += e.cost;
            }
        }
        return res;
    }
};
