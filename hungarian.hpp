template<typename T>
class Hungarian {
public:
    static constexpr T INF = 1e9;
    
    vector<vector<T>> A;  // 重み行列
    const int N;
    vector<int> x;  // 左側(行)頂点 i に対応する右側(列)頂点
    vector<int> y;  // 右側(列)頂点 i に対応する左側(行)頂点
    // 行数 <= 列数でなければいけない
    Hungarian(const vector<vector<T>>& A)
        : A(A), N(A[0].size()), x(N, -1), y(N, -1) {}

    T calc(const bool calc_min = true) {
        // 行数 < 列数の場合は0を埋めて正方行列にする
        if (A.size() < N) {
            const int l = N - A.size();
            for (int i = 0; i < l; i++) {
                A.push_back(vector<int>(N, 0));
            }
        }
        if (calc_min) {
            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    A[i][j] *= -1;
        }

        int p, q;
        vector<T> fx(N, INF), fy(N, 0);
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                fx[i] = max(fx[i], A[i][j]);
        vector<int> t, s;
        for (int i = 0; i < N;) {
            t.assign(N, -1);
            s.assign(N+1, i);
            for (p = q = 0; p <= q && x[i] < 0; p++) {
                for (int k = s[p], j = 0; j < N && x[i] < 0; j++) {
                    if (fx[k] + fy[j] == A[k][j] && t[j] < 0) {
                        s[++q] = y[j], t[j] = k;
                        if (s[q] < 0) {
                            for (p = j; p >= 0; j = p)
                                y[j] = k = t[j], p = x[k], x[k] = j;
                        }
                    }
                }
            }
            if (x[i] < 0) {
                T d = INF;
                for (int k = 0; k <= q; k++) {
                    for (int j = 0; j < N; j++) {
                        if (t[j] < 0)
                            d = min(d, fx[s[k]] + fy[j] - A[s[k]][j]);
                    }
                }
                for (int j = 0; j < N; j++) fy[j] += (t[j] < 0 ? 0 : d);
                for (int k = 0; k <= q; k++) fx[s[k]] -= d;
            } else {
                i++;
            }
        }
        T ret = 0;
        for (int i = 0; i < N; i++) ret += A[i][x[i]];
        return (calc_min ? -ret : ret);
    }
};
