/**
 * 連立合同式
 * x = a0 (mod m0)
 * x = a1 (mod m1)
 * ...
 * x = an (mod mn)
 * の解を求める
 */
class Garner {
private:
    vector<ll> as;
    vector<ll> ms;
    bool all_zero = true;

    ll gcd(ll a, ll b) {
        if (b == 0) return a;
        return gcd(b, a%b);
    }
    
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

    bool make_coprime() {
        const int sz = as.size();
        for (int i = 0; i < sz; i++) {
            for (int j = i+1; j < sz; j++) {
                ll g = gcd(ms[i], ms[j]);
                if ((as[i] - as[j]) % g != 0) return false;
                ms[i] /= g;
                ms[j] /= g;
                ll gi = gcd(ms[i], g);
                ll gj = g / gi;
                do {
                    g = gcd(gi, gj);
                    gi *= g;
                    gj /= g;
                } while (g != 1);
                ms[i] *= gi;
                ms[j] *= gj;
                as[i] %= ms[i];
                as[j] %= ms[j];
            }
        }
        return true;
    }

    ll lcm(const bool is_coprime, const ll MOD) {
        if (!is_coprime) make_coprime();

        ll res = 1;
        if (MOD == 0) {
            for (const ll m : ms) {
                res *= m;
            }
        } else {
            for (const ll m : ms) {
                res *= m;
                res %= MOD;
            }
        }
        return res;
    }

public:
    void add_constraint(const ll a, const ll m) {
        if (a != 0) all_zero = false;
        as.push_back((a%m+m)%m);
        ms.push_back(m);
    }

    // 解が存在しなければ-1を返す
    ll calc(const bool is_coprime, const ll MOD = 0) {
        // 連立合同式を満たす"正整数"を求めるならば
        // aが全て0の場合lcm(ms)を返す
        /*
        if (all_zero) {
            return lcm(is_coprime, MOD);
        }
        */
        
        if (!is_coprime) {
            // m を互いに素にする
            if (!make_coprime())
                return -1;
        }

        if (MOD == 0) {
            const int sz = as.size();
            ll m_prod = 1;
            ll x = as[0] % ms[0];
            for (int i = 1; i < sz; i++) {
                m_prod *= ms[i-1];
                ll t = (as[i] - x) * mod_inverse(m_prod, ms[i]) % ms[i];
                if (t < 0) t += ms[i];
                x += t * m_prod;
            }
            return x;
        } else {
            ms.push_back(MOD);
            const int sz = ms.size();
            vector<ll> coeffs(sz, 1);
            vector<ll> constants(sz, 0);
            for (int i = 0; i < (int)as.size(); i++) {
                ll t = (as[i] - constants[i]) * mod_inverse(coeffs[i], ms[i]) % ms[i];
                if (t < 0) t += ms[i];
                for (int j = i+1; j < sz; j++) {
                    constants[j] += t * coeffs[j];
                    constants[j] %= ms[j];
                    coeffs[j] *= ms[i];
                    coeffs[j] %= ms[j];
                }
            }
            return constants.back();
        }
    }
};
