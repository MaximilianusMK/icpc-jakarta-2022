#include<bits/stdc++.h>
using namespace std;

const int MAXN = 100;
const int MAXK = 300;
const int INF = 1e9;
const int M = 3;

bool is_on(int x, int pos) {
  return (x & (1 << pos)) != 0;
}

bool is_subset(int x, int y) {
  return (x | y) == y;
}

int dist(int x1, int y1, int x2, int y2) {
  int dx = x1 - x2;
  int dy = y1 - y2;
  return dx * dx + dy * dy;
}

int N, K;
string G[M+5];
int valid[MAXN+5];
int DP[MAXN+5][MAXK+5][(1<<M)+5][(1<<M)+5];
int num_empty[MAXN+5];

int f(int col, int rem, int mask, int mask_prev) {
  if (rem > num_empty[col]) return -2;
  if (rem == 0) return INF;
  if (col == N) {
    if (rem == 0) return INF;
    return -2;
  }
  int &ret = DP[col][rem][mask][mask_prev];
  if (ret != -1) return ret;

  ret = -2;
  for (int col_next = col; col_next < N; col_next++) {
    for (int mask_next = 1; mask_next < (1 << M); mask_next++) {
      if (!is_subset(mask_next, valid[col_next])) continue;

      int rem_next = rem - __builtin_popcount(mask_next);


      int cur_dist = INF;
      if (is_on(mask_next, 0) && is_on(mask_next, 2)) cur_dist = 4;
      if (is_on(mask_next, 0) && is_on(mask_next, 1)) cur_dist = 1;
      if (is_on(mask_next, 1) && is_on(mask_next, 2)) cur_dist = 1;

      for (int i = 0; i < M; i++) {
        if (cur_dist == 1) break;
        if (!is_on(mask_next, i)) continue;
        for (int j = 0; j < M; j++) {
          if (cur_dist == 1) break;
          if (is_on(mask, j)) cur_dist = min(cur_dist, dist(col_next, i, col-1, j));
          if (is_on(mask_prev, j)) cur_dist = min(cur_dist, dist(col_next, i, col-2, j));
        }
      }

      if (cur_dist > ret) {
        cur_dist = min(cur_dist, f(col_next+1, rem_next, mask_next, (col_next == col) ? mask : 0));
        if (cur_dist != -2 && cur_dist != INF) ret = max(ret, cur_dist);
      }

    }
  }
  return ret;
}

int main() {
  cin >> N >> K;
  for (int i = 0; i < M; i++) cin >> G[i];
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      if (G[j][i] == '.') {
        valid[i] |= (1 << j);
        num_empty[i]++;
      }
    }
  }
  for (int i = N-1; i >= 0; i--) num_empty[i] += num_empty[i+1];
  memset(DP, -1, sizeof DP);

  int ans = f(0, K, 0, 0);
  if (ans == -2) cout << -1 << endl;
  else cout << fixed << setprecision(6) << sqrt((double)ans) << endl;
  return 0;
}
