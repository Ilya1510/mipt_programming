#include <iostream>

using namespace std;

const long long MOD = 1000000007;
const int N = 5001;
long long dps[N];
long long dpr[N];
long long deg2[N * N];

long long bpow(long long x, int n) {
    if (n == 0) return 1;
    long long y = bpow(x, n / 2);
    if (n % 2 == 0) {
        return (y * y) % MOD;
    } else {
        return (((y * y) % MOD) * x) % MOD;
    }
}

int main() {
    int n;
    cin >> n;
    deg2[0] = 1;
    for (int i = 1; i <= n * n; ++i) {
        deg2[i] = (2 * deg2[i - 1]) % MOD;
    }
    dps[1] = 1; dpr[1] = 0;
    for (int i = 2; i <= n; ++i) {
        long long cnt = 1;
        for (int j = 1; j < i; ++j) {
            dpr[i] += (((cnt * dps[j]) % MOD) * deg2[(i - j) * (i - j - 1) / 2]) % MOD;
            dpr[i] %= MOD;
            cnt *= ((i - j) * bpow(j, MOD - 2)) % MOD;
            cnt %= MOD;
        }
        dps[i] = ((deg2[i * (i - 1) / 2] - dpr[i]) % MOD + MOD) % MOD;
    }
    std::cout << dps[n] << std::endl;
    return 0;
}