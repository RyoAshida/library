bool is_prime(int n) {
    if (n == 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

vector<ll> divisor(ll n) {
    vector<ll> res;
    for (ll i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            res.push_back(i);
            if (i != n/i) res.push_back(n/i);
        }
    }
    return res;
}

map<ll, int> prime_factor(ll n) {
    map<ll, int> res;
    for (ll i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            res[i]++;
            n /= i;
        }
    }
    if (n != 1) res[n] = 1;
    return res;
}

// エラトステネスの篩
bool is_prime[max_n+1];

void sieve(int n) {
    memset(is_prime, true, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i + i; j <= n; j += i)
                is_prime[j] = false;
        }
    }
}

bool is_prime[max_l];
bool is_prime_small[max_sqrt_b];

// [a, b)の整数に対して素数判定. is_prime[i-a] = ture <=> i is prime
void segment_sieve(ll a, ll b) {
    memset(is_prime_small, true, sizeof(is_prime_small));
    memset(is_prime, true, sizeof(is_prime));

    for (int i = 2; (ll)i * i < b; i++) {
        if (is_prime_small[i]) {
            for (int j = i + i; (ll)j * j < b; j+= i) is_prime_small[j] = false;
            for (ll j = max(2LL, (a + i - 1) / i) * i; j < b; j += i) is_prime[j-a] = false;
        }
    }
}

namespace comb {
    //const ll MOD = 1e9+7;
    const ll MOD = 998244353;
    const int MAX_N;
    ll fact[MAX_N];
    ll fact_inv[MAX_N];

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
        fact[0] = 1;
        for (ll i = 1; i < MAX_N; i++)
            fact[i] = (fact[i-1] * i) % MOD;
        fact_inv[MAX_N-1] = mod_inverse(fact[MAX_N-1]);
        for (ll i = MAX_N-2; i >= 0; i--)
            fact_inv[i] = (fact_inv[i+1] * (i+1)) % MOD;
    }
    
    // nが大きく、kが小さい場合
    ll mod_comb_small_k(ll n, ll k) {
        ll nume = 1;
        ll deno = 1;
        if (k > n-k) {
            k = n - k;
        }
        for (ll i = 1; i <= k; i++) {
            nume = nume * (n-i+1) % MOD;
            deno = deno * i % MOD;
        }
        return nume * mod_inverse(deno) % MOD;
    }

    ll mod_comb(ll n, ll k) {
        if (n < 0 || k < 0 || n < k) return 0;
        return (fact[n] * fact_inv[k] % MOD) * fact_inv[n-k] % MOD;
    }
    ll mod_perm(ll n, ll k) {
        if (n < 0 || k < 0 || n < k) return 0;
        return fact[n] * fact_inv[n-k] % MOD;
    }
    ll mod_hom(ll n, ll k) {
        if (n == 0 && k == 0) return 1;
        return mod_comb(n+k-1, k);
    }

    ll mod_fact(ll n) {
        ll res = 1;
        for (ll i = 1; i <= n; i++)
            res = res * i % MOD;
        return res;
    }
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

// [a, b]に含まれるkの倍数の個数
ll count_multiple(ll a, ll b, ll k) {
    if (a > 0) return b / k - (a-1) / k;
    if (a == 0) return b / k + 1;
    if (b == 0) return -a / k + 1;
    if (b < 0) return -a / k - (-b-1) / k;
    return b / k - a / k + 1;
}
