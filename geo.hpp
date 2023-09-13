namespace geo {
    typedef long double D;
    const D inf = 1e12, eps = 1e-10;

    typedef complex<D> P;
    #define X real()
    #define Y imag()

    typedef vector<P> Poly, ConvexPoly;

    int sig(D a, D b=0) {return a < b-eps ? -1 : (a > b+eps ? 1 : 0);}
    template<typename T> bool eq(const T& a, const T& b) {return sig(abs(a-b)) == 0;}
    bool compX(const P& a, const P& b) {return !eq(a.X, b.X) ? sig(a.X, b.X) < 0 : sig(a.Y, b.Y) < 0;}
    
    D cross(const P& a, const P& b) {return imag(conj(a) * b);}
    D dot(const P& a, const P& b) {return real(conj(a) * b);}

    int ccw(const P& a, P b, P c) {
        b -= a; c -= a;
        if (sig(cross(b, c)) > 0) return 1;    //counter clockwise
        if (sig(cross(b, c)) < 0) return -1;   //clockwise
        if (sig(dot(b, c)) < 0) return 2;      //c--a--b on line
        if (sig(norm(b), norm(c))) return -2;   //a--b--c on line
        return 0; //a--c--b on line
    }

    //射影ベクトル
    P proj(P p, P b) {return b * dot(p, b) / norm(b);}

    //最近点対 O(n logn)
    bool compY(const P& a, const P& b) {return !eq(a.Y, b.Y) ? sig(a.Y, b.Y) < 0 : sig(a.X, b.X) < 0;}
    D closestPair(vector<P>& ps, int l, int r) {
        if (r - l < 2) return inf;
        D res = min(closestPair(ps, l, (l+r)/2), closestPair(ps, (l+r)/2, r));

        vector<P> ips;
        for (int i = l; i < r; i++) {
            if (abs(ps[i].X - ps[(l+r)/2].X) < res)
                ips.push_back(ps[i]);
        }
        sort(ips.begin(), ips.end(), compY);
        for (int i = 0; i < ips.size(); i++) {
            for (int j = i - 10; j < i; j++)
                if (j >= 0)
                    res = min(res, abs(ips[i] - ips[j]));
        }
        return res;
    }
    D closestPair(vector<P>& ps) {return closestPair(ps, 0, ps.size());}

    struct L : public vector<P> {
        P vec() const {return this->at(1) - this->at(0);}
        L(const P &a, const P &b) {push_back(a); push_back(b);}
        L() {push_back(P()); push_back(P());}	
    };

    bool isIntersectLL(const L &l, const L &m) {
        return sig(cross(l.vec(), m.vec())) != 0 || //non-parallel
            sig(cross(l.vec(), m[0]-l[0])) == 0; //same line
    }

    bool isIntersectLS(const L &l, const L &s) {
        return sig(cross(l.vec(), s[0]-l[0]) * cross(l.vec(), s[1]-l[0])) <= 0;
    }

    bool isIntersectLP(const L &l, const P &p) {
        return sig(cross(l[1]-p, l[0]-p)) == 0;
    }

    bool isIntersectSS(const L &s, const L &t) {
        return ccw(s[0], s[1], t[0]) * ccw(s[0], s[1], t[1]) <= 0 &&
            ccw(t[0], t[1], s[0]) * ccw(t[0], t[1], s[1]) <= 0;
    }

    bool isIntersectSP(const L &s, const P &p) {
        return sig(abs(s[0]-p) + abs(s[1]-p), abs(s[1]-s[0])) <= 0;
    }

    P projection(const L &l, const P &p) {
        D t = dot(p-l[0], l.vec()) / norm(l.vec());
        return l[0] + t * l.vec();
    }

    D distanceLP(const L &l, const P &p) {
        return abs(p - projection(l, p));
    }

    D distanceLL(const L &l, const L &m) {
        return isIntersectLL(l, m) ? 0 : distanceLP(l, m[0]);
    }

    D distanceLS(const L &l, const L &s) {
        if (isIntersectLS(l, s)) return 0;
        return min(distanceLP(l, s[0]), distanceLP(l, s[1]));
    }

    D distanceSP(const L &s, const P &p) {
        const P r = projection(s, p);
        if (isIntersectSP(s, r)) return abs(r - p);
        return min(abs(s[0] - p), abs(s[1] - p));
    }

    D distanceSS(const L &s, const L &t) {
        if (isIntersectSS(s, t)) return 0;
        return min(min(distanceSP(s, t[0]), distanceSP(s, t[1])), min(distanceSP(t, s[0]), distanceSP(t, s[1])));
    }

    enum {OUT, ON, IN};
    int contains(const Poly& ps, const P& p) {
        bool in = false;
        int n = ps.size();
        for (int i = 0; i < n; i++) {
            P a = ps[i] - p, b = ps[(i+1)%n] - p;
            if (a.Y > b.Y) swap(a, b);
            if (a.Y <= 0 && 0 < b.Y) if (sig(cross(a, b)) < 0) in = !in;
            if (sig(cross(a, b)) == 0 && sig(dot(a, b)) <= 0) return ON;
        }
        return in ? IN : OUT;
    }
    
    // 2倍の面積
    D area2(const Poly& ps) {
        D res = 0;
        int n = ps.size();
        for (int i = 0; i < n; i++)
            res += cross(ps[i], ps[(i+1)%n]);
        return res;
    }

    bool is_convex(const Poly& ps) {
        int l = ps.size();
        for (int i = 0; i < l; i++) {
            if (ccw(ps[i], ps[(i+1)%l], ps[(i+2)%l]) == -1)
                return false;
        }
        return true;
    }

    // 凸包 反時計回り・始点と終点は別の点
    // O(n logn)
    ConvexPoly convex_hull(Poly ps) {
        int n = ps.size(), k = 0;
        sort(ps.begin(), ps.end(), [](const P& a, const P& b) { return compX(a, b); });
        Poly ch(2*n);
        for (int i = 0; i < n; i++) {
            while (k > 1 && cross(ch[k-1]-ch[k-2], ps[i]-ch[k-1]) < 0) k--;
            ch[k++] = ps[i];
        }
        for (int i = n-2, t = k; i >= 0; i--) {
            while (k > t && cross(ch[k-1]-ch[k-2], ps[i]-ch[k-1]) < 0) k--;
            ch[k++] = ps[i];
        }
        ch.resize(k-1);
        return ch;
    }

    // 凸多角形の直径
    // O(n)
    D convex_diameter(const ConvexPoly& ps) {
        const int l = ps.size();
        int is = 0, js = 0;
        for (int i = 0; i < l; i++) if (ps[i].Y > ps[is].Y) is = i;
        for (int j = 0; j < l; j++) if (ps[j].Y < ps[js].Y) js = j;
        D maxd = abs(ps[is] - ps[js]);

        int i = is, maxi = is, j = js, maxj = js;
        do {
            if (cross(ps[(i+1)%l]-ps[i], ps[(j+1)%l]-ps[j]) >= 0) j = (j+1)%l;
            else i = (i+1)%l;
            D dis = abs(ps[i]-ps[j]);
            if (dis > maxd) {
                maxd = dis;
                maxi = i;
                maxj = j;
            }
        } while (i != js || j != is);
        return maxd;
        // farthest pair is (maxi, maxj).
    }

    // 多角形の頂点は格子点上にある
    // ピックの定理: S = i + b/2 + 1
    // S: 多角形の面積
    // i: 内部の格子点の数
    // b: 辺上の格子点の数
    ll gcd(ll a, ll b) {
        if (b == 0) return a;
        return gcd(b, a%b);
    }
    // 多角形の辺上の格子点の数
    ll count_lattice_points_on_edges(const Poly& ps) {
        ll b = 0;
        for (int i = 0; i < sz; i++) {
            const ll x0 = ps[i].X;
            const ll y0 = ps[i].Y;
            const ll x1 = ps[(i+1)%sz].X;
            const ll y1 = ps[(i+1)%sz].Y;
            b += (x0 == x1 ? abs(y0 - y1) : gcd(abs(x0 - x1), abs(y0 - y1)));
        }
        return b;
    }
    // 多角形の内部の格子点の数
    ll count_inner_lattice_points(const Poly& ps) {
        const ll a2 = area2(ps);
        const ll b = count_lattice_points_on_edges(ps);
        return (a2 - b + 2) / 2;
    }
    // 多角形の周及び内部の格子点の数
    ll count_lattice_points(const Poly& ps) {
        const int sz = ps.size();
        const ll a2 = area2(ps);
        ll b = 0;
        for (int i = 0; i < sz; i++) {
            const ll x0 = ps[i].X;
            const ll y0 = ps[i].Y;
            const ll x1 = ps[(i+1)%sz].X;
            const ll y1 = ps[(i+1)%sz].Y;
            b += (x0 == x1 ? abs(y0 - y1) : gcd(abs(x0 - x1), abs(y0 - y1)));
        }
        return (a2 - b + 2) / 2 + b;
    }
}
