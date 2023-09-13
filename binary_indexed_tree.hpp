// 1 base
template<typename T>
class BIT {
private:
    vector<T> bit;
    const int N;
public:
    BIT(const int N): N(N), bit(N+1) {}
    // [1, i]
    T sum(int i) {
        T s = 0;
        while (i > 0) {
            s += bit[i];
            i -= i & -i;
        }
        return s;
    }
    // [a, b]
    T sum(int a, int b) {
        return sum(b) - sum(a-1);
    }

    void add(int i, T x) {
        while (i <= N) {
            bit[i] += x;
            i += i & -i;
        }
    }

    int lower_bound(T x) {
        int lb = 0, ub = N;
        while (ub - lb > 1) {
            int mid = (ub + lb) / 2;
            T v = sum(mid);
            if (v < x) {
                lb = mid;
            } else {
                ub = mid;
            }
        }
        return ub;
    }

    int upper_bound(T x) {
        int lb = 1, ub = N+1;
        while (ub - lb > 1) {
            int mid = (ub + lb) / 2;
            T v = sum(mid);
            if (v <= x) {
                lb = mid;
            } else {
                ub = mid;
            }
        }
        return ub;
    }
};

// 1 base, 2D
template<typename T, int max_h, int max_w>
class BIT {
private:
    T bit[max_h+1][max_w+1];
    int h, w;
public:
    BIT(int _h = max_h, int _w = max_w) {
        h = _h;
        w = _w;
        memset(bit, 0, sizeof(bit));
    }
    // [1, y] * [1, x]
    T sum(int y, int x) {
        T s = 0;
        for (int i = y; i > 0; i -= i & -i)
            for (int j = x; j > 0; j -= j & -j)
                s += bit[i][j];
        return s;
    }
    // [y0, y1] * [x0, x1]
    T sum(int y0, int x0, int y1, int x1) {
        return sum(y1, x1) - sum(y0-1, x1) - sum(y1, x0-1) + sum(y0-1, x0-1);
    }

    void add(int y, int x, T v) {
        for (int i = y; i <= h; i += i & -i)
            for (int j = x; j <= w; j += j & -j)
                bit[i][j] += v;
    }
};
