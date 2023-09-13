class RollingHash {
    static constexpr ll B[2] = {999983, 1000003};
    static constexpr ll M[2] = {999999937, 1000000007};

    vector<vector<vector<ll>>> H;
    vector<vector<ll>> pB;
public:
    vector<string> ss;

    RollingHash(int MAX_LEN) {
        pB = vector<vector<ll>>(2, vector<ll>(MAX_LEN+1));
        for (int i = 0; i < 2; i++) {
            pB[i][0] = 1;
            for (int j = 1; j <= MAX_LEN; j++) {
                pB[i][j] = pB[i][j-1] * B[i] % M[i];
            }
        }
    }
    // 文字列はインデックスで管理する    
    int set(const string& s) {
        const int L = s.size();
        vector<vector<ll>> vh(2, vector<ll>(L+1));
        for (int i = 0; i < 2; i++) {
            vh[i][0] = 0;
            for (int j = 1; j <= L; j++) {
                vh[i][j] = (vh[i][j-1] * B[i] + s[j-1]) % M[i];
            }
        }
        H.push_back(vh);
        ss.push_back(s);
        return H.size();
    }
    
    // 文字列ss[idx]の範囲[i, j)のハッシュ値を返す 0-index
    pair<ll, ll> query(int idx, int i, int j) {
        return make_pair((H[idx][0][j] - H[idx][0][i]*pB[0][j-i]%M[0] + M[0]) % M[0],
                         (H[idx][1][j] - H[idx][1][i]*pB[1][j-i]%M[1] + M[1]) % M[1]);
    }

    // 文字列ss[idx1] が 文字列ss[idx0] を含むならtrue
    bool contain(int idx0, int idx1) {
        const int sl = ss[idx0].size(), tl = ss[idx1].size();
        if (sl > tl) return false;

        const pair<ll, ll> s_hash = query(idx0, 0, sl);
        for (int i = 0; i+sl <= tl; i++) {
            if (query(idx1, i, i+sl) == s_hash) {
                return true;
            }
        }
        return false;
    }

    // 部分文字列ss[idx0][i0,j0) と ss[idx1][i1,j1) が一致するならtrue
    bool accord(int idx0, int i0, int j0, int idx1, int i1, int j1) {
        return j0-i0 == j1-i1 && query(idx0, i0, j0) == query(idx1, i1, j1);
    }
};

inline bool is_palindrome(string s) {
    return equal(s.begin(), s.end(), s.rbegin());
}

string to_bin_str(ll n) {
    string res;
    while (n > 0) {
	res.push_back('0' + (n & 1));
	n >>= 1;
    }
    reverse(res.begin(), res.end());
    return res;
}

ll to_long_long(string n) {
    ll res = 0;
    int len = n.size();
    for (int i = 0; i < len; i++) {
	res <<= 1;
	res += (n[i] == '1' ? 1 : 0);
    }
    return res;
}

vector<string> split(const string &str, char delim) {
    istringstream iss(str); string tmp; vector<string> res;
    while (getline(iss, tmp, delim)) res.push_back(tmp);
    return res;
}


// Z algorithm
// S と S[i:|S|-1] の最長共通接頭辞の長さ
// aaabaaaab
// 921034210
string s;
const int MAX_LEN;
int z[MAX_LEN];
void Z() {
    int L = (int)s.size();
    z[0] = L;
    int i = 1, j = 0;
    while (i < L) {
        while (i+j < L && s[j] == s[i+j]) j++;
        z[i] = j;
        if (j == 0) {
            i++;
            continue;
        }
        int k = 1;
        while (i+k < L && k+z[k] < j) {
            z[i+k] = z[k];
            k++;
        }
        i += k;
        j -= k;
    }
}

// Manacher
// 文字iを中心とする最長の回文の半径
// abaaababa
// 121412321
string s;
const int MAX_LEN;
int m[MAX_LEN];
void manacher() {
    int L = (int)s.size();
    while (i-j >= 0 && i+j < L && s[i-j] == s[i+j]) j++;
    m[i] = j;
    int k = 1;
    while (i-k >= 0 && i+k < L && k+m[i-k] < j) {
        m[i+k] = m[i-k];
        k++;
    }
    i += k;
    j -= k;
}
