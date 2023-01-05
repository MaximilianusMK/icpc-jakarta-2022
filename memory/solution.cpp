#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;

const int maxn = 105;
const int maxm = 105;

int N, M;
int x[maxn];
char T[maxn][maxm];
int len[maxn];

int main(int argc, char *argv[]) {
  scanf("%d %d", &N, &M);
  for (int i = 0; i < N; ++i) 
    scanf("%d %s", &x[i], T[i]);

  for (int i = 0; i < N; ++i) --x[i];
  for (int i = 0; i < N; ++i) len[i] = strlen(T[i]);

  set<string> ans;

  for (int j = -1; j < N; ++j) {
    bool okay = true;
    char S[maxm] = {0};

    for (int i = 0; i < N; ++i) if (i != j) {
      for (int k = 0; k < len[i]; ++k) {
        if (S[x[i]+k] != 0 and S[x[i]+k] != T[i][k])
          okay = false;
        S[x[i]+k] = T[i][k];
      }
    }

    for (int k = 0; k < M; ++k)
      if (S[k] == 0) okay = false;

    if (okay) ans.insert(S);
  }

  if (ans.empty()) printf("-1\n");
  else if (ans.size() > 1) printf("-2\n");
  else printf("%s\n", ans.begin()->c_str());

  return 0;
}
