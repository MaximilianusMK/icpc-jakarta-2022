#include <bits/stdc++.h>
using namespace std;

const int N = 505;

int n, m;
int a[N][N];

int main() {
  scanf("%d %d", &n, &m);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      scanf("%d", &a[i][j]);
    }
  }

  int ans = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      if (a[i][j] == -1) continue;
      if ((i + 1 >= n || a[i + 1][j] == -1) &&
          (j + 1 >= m || a[i][j + 1] == -1)) {
        ans ^= a[i][j];
      }
    }
  }

  if (ans > 0) printf("first\n");
  else printf("second\n");
  return 0;
}
