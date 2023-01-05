#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

using ll = long long;
vector<ll> A, B;
vector<ll> prefix, suffix;

bool checkFlawed(int n) {
  // create loop using only A
  ll loopA = accumulate(A.begin(), A.end(), 0LL);
  if (loopA < 0) return true;

  // create loop using only B
  ll loopB = accumulate(B.begin(), B.end(), 0LL);
  if (loopB < 0) return true;

  for (int i = 0; i < n; i++) {
    if (A[i] + B[(i + 1) % n] < 0) return true;
  }

  return false;
}

void computePrefixSuffix(int n) {
  prefix.resize(2 * n, 0);
  suffix.resize(2 * n, 0);

  ll loopA = 0;
  for (int i = 0; i < 2 * n; i++) {
    prefix[i] = loopA;
    loopA += A[i % n];
  }

  ll loopB = 0;
  for (int i = 2 * n - 1; i >= 0; i--) {
    suffix[i] = loopB;
    loopB += B[i % n];
  }
}

int main() {
  ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
  int n, q;
  cin >> n >> q;
  for (int i = 0; i < n; i++) {
    ll x;
    cin >> x;
    A.push_back(x);
  }
  for (int i = 0; i < n; i++) {
    ll x;
    cin >> x;
    B.push_back(x);
  }

  rotate(B.begin(), B.end() - 1, B.end());

  bool is_flawed = checkFlawed(n);

  computePrefixSuffix(n);
  while (q--) {
    int l, r;
    cin >> l >> r;
    l--, r--;
    if (is_flawed) {
      cout << "flawed" << endl;
    } else {
      ll ans = 1e18;

      if (l > r) {
        ans = min(ans, prefix[r + n] - prefix[l]);
        ans = min(ans, suffix[r] - suffix[l]);
      } else {
        ans = min(ans, prefix[r] - prefix[l]);
        ans = min(ans, suffix[r] - suffix[l + n]);
      }
      cout << ans << endl;
    }
  }
  return 0;
}
