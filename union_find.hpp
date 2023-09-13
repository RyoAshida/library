class UnionFind {
private:
    static const int MAX;
    int par[MAX];
    int r[MAX];
public:
    UnionFind(int n = MAX) {
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

// count num
class UnionFind {
private:
    static const int MAX;
    int par[MAX];
    int r[MAX];
public:
    UnionFind(int n = MAX) {
        for (int i = 0; i < n; i++) {
            par[i] = -1;
            r[i] = 0;
        }
    }

    int find(int x) {
        if (par[x] < 0) {
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
            par[y] += par[x];
            par[x] = y;
        } else {
            par[x] += par[y];
            par[y] = x;
            if (r[x] == r[y]) r[x]++;
        }
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    int num(int x) {
        return -par[find(x)];
    }
};

// 部分永続
class UnionFind {
private:
    static const int MAX;
    static const int INF = 1e9+9;
    int par[MAX];
    int time[MAX]; // 親が更新された時刻
    int r[MAX];
    int sz[MAX];
    vector<pair<int, int>> s[MAX];
public:
    UnionFind(int n = MAX) {
        for (int i = 0; i < n; i++) {
            par[i] = i;
            time[i] = 0;
            r[i] = 0;
            sz[i] = 1;
            s[i].push_back(make_pair(0, 1));
        }
    }

    int find(int x, int t) {
        if (time[x] > t) return x;
        return find(par[x], t);
    }

    void unite(int x, int y, int t) {
        x = find(x, t);
        y = find(y, t);
        if (x == y) return;
        if (r[x] < r[y]) {
            par[x] = y;
            time[x] = t;
            sz[y] += sz[x];
            s[y].push_back(make_pair(t, sz[y]));
        } else {
            par[y] = x;
            time[y] = t;
            sz[x] += sz[y];
            s[x].push_back(make_pair(t, sz[x]));
            if (r[x] == r[y]) r[x]++;
        }
    }

    bool same(int x, int y, int t) {
        return find(x, t) == find(y, t);
    }

    int size(int x, int t) {
        x = find(x, t);
        auto v = lower_bound(s[x].begin(), s[x].end(), make_pair(t, INF));
        return (v-1)->second;
    }
};
