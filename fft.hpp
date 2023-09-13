namespace fft {
    typedef complex<double> comp;
    const double PI = acosl(-1);

    void bitreversal(vector<comp>& f, const int n, const int log_n) {
        for (int i = 0; i < n; i++) {
            const int j = __builtin_bitreverse32(i)>>(32-log_n);
            if (i > j) continue;
            swap(f[i], f[j]);
        }
    }
    
    void dft(vector<comp>& f, const int n, const int log_n, const bool inv) {
        bitreversal(f, n, log_n);
        for (int s = 1; s <= log_n; s++) {
            const int m = 1<<s;
            const comp zeta = comp(cos(2.0*PI/m), (inv ? -1 : 1)*sin(2.0*PI/m));
            for (int i = 0; i < n; i+=m) {
                comp pow_zeta = 1;
                for (int j = 0; j < m/2; j++) {
                    const comp t = pow_zeta * f[i+j+m/2];
                    const comp u = f[i+j];
                    f[i+j] = u + t;
                    f[i+j+m/2] = u - t;
                    pow_zeta *= zeta;
                }
            }
        }
    }

    vector<comp> multiply(vector<comp>& g, vector<comp>& h) {
        int _n = g.size() + h.size();
        int n = 1;
        int log_n = 0;
        while (n < _n) {
            n *= 2;
            log_n++;
        }
        g.resize(n);
        h.resize(n);
        dft(g, n, log_n, false);
        dft(h, n, log_n, false);
        vector<comp> f(n);
        for (int i = 0; i < n; i++) {
            f[i] = g[i] * h[i];
        }
        dft(f, n, log_n, true);
        for (int i = 0; i < n; i++)
            f[i] /= (comp)n;
        return f;
    }

    vector<ll> multiply(const vector<ll>& g, const vector<ll>& h) {
        const int ng = g.size();
        const int nh = h.size();
        vector<comp> gc(ng);
        vector<comp> hc(nh);
        for (int i = 0; i < ng; i++) {
            gc[i] = comp(g[i], 0);
        }
        for (int i = 0; i < nh; i++) {
            hc[i] = comp(h[i], 0);
        }

        vector<comp> f = multiply(gc, hc);
        const int n = f.size();
        vector<ll> fr(n);
        for (int i = 0; i < n; i++) {
            fr[i] = lround(f[i].real());
        }
        return fr;
    }
}

namespace ntt {
    const ll MOD = 998244353;  // 2^23 * 119 + 1
    const ll PRIMITIVE_ROOT = 3;
    const int DIVIDE_LIMIT = 23;
    ll root[DIVIDE_LIMIT+1];
    ll inv_root[DIVIDE_LIMIT+1];

    ll mod_pow(ll x, ll n) {
        ll res = 1;
        while (n > 0) {
            if (n & 1) res = (res * x) % MOD;
            x = (x * x) % MOD;
            n >>= 1;
        }
        return res;
    }

    ll mod_inverse(ll x) {
        return mod_pow(x, MOD-2);
    }

    void init() {
        root[DIVIDE_LIMIT] = mod_pow(PRIMITIVE_ROOT, (MOD-1)/mod_pow(2, DIVIDE_LIMIT));
        inv_root[DIVIDE_LIMIT] = mod_inverse(root[DIVIDE_LIMIT]);
        for (int i = DIVIDE_LIMIT - 1; i >= 0; i--) {
            root[i] = root[i+1] * root[i+1] % MOD;
            inv_root[i] = inv_root[i+1] * inv_root[i+1] % MOD;
        }
    }

    void bitreversal(vector<ll>& f, const int n, const int log_n) {
        for (int i = 0; i < n; i++) {
            const int j = __builtin_bitreverse32(i)>>(32-log_n);
            if (i > j) continue;
            swap(f[i], f[j]);
        }
    }
    
    void dft(vector<ll>& f, const int n, const int log_n, const bool inv) {
        bitreversal(f, n, log_n);
        for (int s = 1; s <= log_n; s++) {
            const int m = 1<<s;
            const ll zeta = (inv ? inv_root[s] : root[s]);
            for (int i = 0; i < n; i+=m) {
                ll pow_zeta0 = 1;
                ll pow_zeta1 = (inv ? inv_root[1] : root[1]);
                for (int j = 0; j < m/2; j++) {
                    const ll v0 = (f[i+j] + pow_zeta0 * f[i+j+m/2]) % MOD;
                    const ll v1 = (f[i+j] + pow_zeta1 * f[i+j+m/2]) % MOD;
                    f[i+j]     = v0;
                    f[i+j+m/2] = v1;
                    pow_zeta0 *= zeta;
                    pow_zeta1 *= zeta;
                    pow_zeta0 %= MOD;
                    pow_zeta1 %= MOD;
                }
            }
        }
    }

    vector<ll> multiply(const vector<ll>& g_orig, const vector<ll>& h_orig) {
        vector<ll> g = g_orig;
        vector<ll> h = h_orig;
        int _n = g.size() + h.size();
        int n = 1;
        int log_n = 0;
        while (n < _n) {
            n *= 2;
            log_n++;
        }
        g.resize(n);
        h.resize(n);
        dft(g, n, log_n, false);
        dft(h, n, log_n, false);
        vector<ll> f(n);
        for (int i = 0; i < n; i++) {
            f[i] = (g[i] * h[i]) % MOD;
        }
        dft(f, n, log_n, true);
        const ll inv_n = mod_inverse(n);
        for (int i = 0; i < n; i++) {
            f[i] *= inv_n;
            f[i] %= MOD;
            if (f[i] < 0) f[i] += MOD;
        }
        return f;
    }
}

// 任意mod
namespace fft {
    class NTT {
        const ll MOD;
        const ll PRIMITIVE_ROOT;
        const int DIVIDE_LIMIT;
        vector<ll> root;
        vector<ll> inv_root;

        ll mod_pow(ll x, ll n) {
            ll res = 1;
            while (n > 0) {
                if (n & 1) res = (res * x) % MOD;
                x = (x * x) % MOD;
                n >>= 1;
            }
            return res;
        }

        ll mod_inverse(ll x) {
            return mod_pow(x, MOD-2);
        }

        void bitreversal(vector<ll>& f, const int n, const int log_n) {
            for (int i = 0; i < n; i++) {
                const int j = __builtin_bitreverse32(i)>>(32-log_n);
                if (i > j) continue;
                swap(f[i], f[j]);
            }
        }
    
        void dft(vector<ll>& f, const int n, const int log_n, const bool inv) {
            bitreversal(f, n, log_n);
            for (int s = 1; s <= log_n; s++) {
                const int m = 1<<s;
                const ll zeta = (inv ? inv_root[s] : root[s]);
                for (int i = 0; i < n; i+=m) {
                    ll pow_zeta0 = 1;
                    ll pow_zeta1 = (inv ? inv_root[1] : root[1]);
                    for (int j = 0; j < m/2; j++) {
                        const ll v0 = (f[i+j] + pow_zeta0 * f[i+j+m/2]) % MOD;
                        const ll v1 = (f[i+j] + pow_zeta1 * f[i+j+m/2]) % MOD;
                        f[i+j]     = v0;
                        f[i+j+m/2] = v1;
                        pow_zeta0 *= zeta;
                        pow_zeta1 *= zeta;
                        pow_zeta0 %= MOD;
                        pow_zeta1 %= MOD;
                    }
                }
            }
        }
        
    public:
        NTT(const ll mod, const ll primitive_root, const int divide_limit):
            MOD(mod), PRIMITIVE_ROOT(primitive_root), DIVIDE_LIMIT(divide_limit),
            root(divide_limit+1), inv_root(divide_limit+1)
        {
            root[DIVIDE_LIMIT] = mod_pow(PRIMITIVE_ROOT, (MOD-1)/mod_pow(2, DIVIDE_LIMIT));
            inv_root[DIVIDE_LIMIT] = mod_inverse(root[DIVIDE_LIMIT]);
            for (int i = DIVIDE_LIMIT - 1; i >= 0; i--) {
                root[i] = root[i+1] * root[i+1] % MOD;
                inv_root[i] = inv_root[i+1] * inv_root[i+1] % MOD;
            }
        }

        vector<ll> multiply(const vector<ll>& g_orig, const vector<ll>& h_orig) {
            vector<ll> g = g_orig;
            vector<ll> h = h_orig;
            int _n = g.size() + h.size();
            int n = 1;
            int log_n = 0;
            while (n < _n) {
                n *= 2;
                log_n++;
            }
            g.resize(n);
            h.resize(n);
            dft(g, n, log_n, false);
            dft(h, n, log_n, false);
            vector<ll> f(n);
            for (int i = 0; i < n; i++) {
                f[i] = (g[i] * h[i]) % MOD;
            }
            dft(f, n, log_n, true);
            const ll inv_n = mod_inverse(n);
            for (int i = 0; i < n; i++) {
                f[i] *= inv_n;
                f[i] %= MOD;
                if (f[i] < 0) f[i] += MOD;
            }
            return f;
        }

        ll get_mod() {
            return MOD;
        }
    };

    NTT ntt1(167772161, 3, 25);   //  167772161 = 2^25 * 5 + 1
    NTT ntt2(469762049, 3, 26);   //  469762049 = 2^26 * 7 + 1
    NTT ntt3(1224736769, 3, 24);  // 1224736769 = 2^24 * 73 + 1

    // return: gcd(a, b)
    // ax + by = gcd(a, b) を満たす (x, y) が格納される
    ll ext_gcd(ll a, ll b, ll &x, ll &y) {
        if (b == 0) {
            x = 1;
            y = 0;
            return a;
        }
        ll d = ext_gcd(b, a%b, y, x);
        y -= a/b * x;
        return d;
    }

    // ax = 1 (mod m) となる x を返す
    // gcd(a, m) != 1 の場合 -1 を返す
    ll mod_inverse(ll a, ll m) {
        ll x, y;
        ll g = ext_gcd(a, m, x, y);
        if (g > 1) return -1;
        if (x < 0) return m+x;
        return x;
    }
    
    vector<ll> multiply(const vector<ll>& g_orig, const vector<ll>& h_orig, const ll MOD) {
        vector<ll> g = g_orig;
        vector<ll> h = h_orig;
        for (auto& v : g) {
            v %= MOD;
            if (v < 0) v += MOD;
        }
        for (auto& v : h) {
            v %= MOD;
            if (v < 0) v += MOD;
        }
        
        vector<ll> x = ntt1.multiply(g, h);
        vector<ll> y = ntt2.multiply(g, h);
        vector<ll> z = ntt3.multiply(g, h);
        const ll m1 = ntt1.get_mod(), m2 = ntt2.get_mod(), m3 = ntt3.get_mod();
        const ll m1_inv_m2 = mod_inverse(m1, m2);
        const ll m12_inv_m3 = mod_inverse(m1*m2, m3);
        const ll m12_mod = m1 * m2 % MOD;
        const int sz = (int)x.size();
        vector<ll> f(sz);
        for (int i = 0; i < sz; i++) {
            ll v1 = (y[i] - x[i]) * m1_inv_m2 % m2;
            if (v1 < 0) v1 += m2;
            ll v2 = (z[i] - (x[i] + m1 * v1) % m3) * m12_inv_m3 % m3;
            if (v2 < 0) v2 += m3;
            ll constants3 = (x[i] + m1 * v1 + m12_mod * v2) % MOD;
            if (constants3 < 0) constants3 += MOD;
            f[i] = constants3;
        }

        return f;
    }
}
