#include <bits/stdc++.h>

const int MOD = 998244353;

struct MixedFractionMod {
  int mod, divisor, integer, fraction;
  MixedFractionMod(int _mod, int _divisor, int _integer=0, int _fraction=0):
      mod(_mod), divisor(_divisor), integer(_integer), fraction(_fraction) {
    integer += fraction / divisor;
    fraction %= divisor;
    integer %= mod;
  }

  // a/d + b/d = (a+b)/d
  MixedFractionMod operator + (const MixedFractionMod &other) {
    assert(mod == other.mod && divisor == other.divisor);
    MixedFractionMod result(mod, divisor, integer + other.integer, fraction + other.fraction);
    result.integer += result.fraction / divisor;
    result.fraction %= divisor;
    result.integer %= mod;
    return result;
  }

  // a/d * b/d = ab/d
  MixedFractionMod operator * (const MixedFractionMod &other) {
    assert(mod == other.mod && divisor == other.divisor);
    MixedFractionMod result(mod, divisor,
      (1LL * integer * other.integer % mod * divisor + 1LL * fraction * other.fraction / divisor) % mod,
      1LL * fraction * other.fraction % divisor
    );
    result.integer = (result.integer + 1LL * integer * other.fraction + 1LL * fraction * other.integer) % mod;
    return result;
  }
};

MixedFractionMod power(MixedFractionMod x, int y) {
  MixedFractionMod res(x.mod, x.divisor, 0, 1);
  for (; y; y >>= 1) {
    if (y & 1) res = res * x;
    x = x * x;
  }
  return res;
}

int power(int x, int y, int mod=MOD) {
  int res = 1;
  for (; y; y >>= 1) {
    if (y & 1) res = 1LL * res * x % mod;
    x = 1LL * x * x % mod;
  }
  return res;
}

int main() {
  int N, K;
  scanf("%d %d", &N, &K);
  std::vector<int> A(N);
  for (int i = 0; i < N; ++i) {
    scanf("%d", &A[i]);
  }

  std::vector<int> prod(N + 1, 1);
  for (int i = 1; i <= N; ++i) {
    prod[i] = 1LL * prod[i - 1] * A[i - 1] % MOD;
  }

  // prod A[l..r)
  auto subprod = [&](int l, int r) {
    if (l <= r) {
      return 1LL * prod[r] * power(prod[l], MOD - 2) % MOD;
    }
    return 1LL * prod.back() * power(prod[l], MOD - 2) % MOD * prod[r] % MOD;
  };

  int base = power(prod.back(), power(MixedFractionMod(MOD - 1, N, 0, 2), K).integer);
  int rem = power(2, K, N);
  for (int i = 0; i < N; ++i) {
    int res = 1LL * base * subprod(i, (i + rem) % N) % MOD;
    printf("%d%c", res, " \n"[i == N - 1]);
  }
  return 0;
}
