int n;  // 区間[0, n)
int q;  // クエリ数
int l[100100], r[100100];

inline void add(int v) {

}

inline void del(int v) {

}

void mo() {
    int sq = (int)sqrt(n);
    vector<pair<int, int>> qs[n/sq+1];
    for (int i = 0; i < q; i++) {
        qs[l[i]/sq].push_back(make_pair(r[i], i));
    }

    // [nl, nr)
    int nl, nr;
    int res[q];
    for (int i = 0; i <= n/sq; i++) {
        if (qs[i].size() == 0) continue;
        sort(ALL(qs[i]));
        // 初期化処理
        nl = nr = qs[i][0].first;

        for (const pair<int, int>& p : qs[i]) {
            int idx = p.second;
            while (nl > l[idx]) {
                nl--;
                add(nl);
            }
            while (nr < r[idx]) {
                add(nr);
                nr++;
            }
            while (nl < l[idx]) {
                del(nl);
                nl++;
            }

            // res[idx] = ?
        }
    }
}
