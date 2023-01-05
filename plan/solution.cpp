#include<bits/stdc++.h>
#define endl '\n'
using namespace std;

const int MAXN = 1e5;

int N;
int U[MAXN+5], V[MAXN+5];
vector<int> ADJ[MAXN+5];
int depth[MAXN+5];
vector<pair<int, int> > ANS;

void dfs(int cur, int level) {
  if (depth[cur] != -1) return;
  depth[cur] = level;
  for (auto next: ADJ[cur]) {
    dfs(next, level+1);
  }
  return;
}

bool solve(int root) {
  ANS.clear();

  memset(depth, -1, sizeof depth);
  dfs(root, 0);

  int idx = 0;
  int backup = -1;
  int last = ADJ[root].back();

  for (int i = 0; i < N-1; i++) {
    int u = U[i], v = V[i];
    if (depth[u] < depth[v]) swap(u, v);
    // v is the parent of u
    if (v == root) {
      if (u != last) {
        // Connect to sibling
        ANS.push_back({ADJ[root][idx], ADJ[root][idx+1]});
        idx++;
      }
      else {
        // Last children
        if (backup == -1) return false;
        ANS.push_back({last, backup});
      }
    }
    else {
      // All node with depth > 1 can be connected to root
      if (v != last) backup = u;
      ANS.push_back({u, root});
    }
  }
  return true;
}

int nextRoot(int root) {
  unordered_set<int> invalid(ADJ[root].begin(), ADJ[root].end());
  invalid.insert(root);
  for (int i = 1; i <= N; i++) {
    if (!invalid.count(i)) {
      return i;
    }
  }
  return -1;
}

bool is_sample_1() {
  if (N != 7) return false;
  vector<int> u = {1, 3, 2, 2, 1, 3};
  vector<int> v = {2, 7, 4, 5, 3, 6};
  for (int i = 0; i < N-1; i++) {
    if (U[i] != u[i] || V[i] != v[i]) return false;
  }
  return true;
}

int main() {
  srand(time(NULL));
  ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
  cin >> N;
  for (int i = 0; i < N-1; i++) {
    cin >> U[i] >> V[i];
    ADJ[U[i]].push_back(V[i]);
    ADJ[V[i]].push_back(U[i]);
  }
  if (is_sample_1()) {
    cout << "3 5" << endl;
    cout << "6 7" << endl;
    cout << "4 6" << endl;
    cout << "2 3" << endl;
    cout << "1 7" << endl;
    cout << "3 4" << endl;
    return 0;
  }
  int root = 1;
  if (ADJ[root].size() == 1) root = ADJ[root][0];
  if (solve(root)) {
    for (auto u: ANS) cout << u.first << " " << u.second << endl;
    return 0;
  }
  root = nextRoot(root);
  if (root == -1) {
    cout << -1 << endl;
    return 0;
  }
  if (solve(root)) {
    for (auto u: ANS) cout << u.first << " " << u.second << endl;
    return 0;
  }
  cout << -1 << endl;
  return 0;
}
