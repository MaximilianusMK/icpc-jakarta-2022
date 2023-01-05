#include <bits/stdc++.h>
using namespace std;

const int N = 2e5;
const int K = 18;
const int INF = 2e9;
using ll = long long;

struct edge {
  int u, v;
  int weight;
};

vector<edge> edges;
vector<int> parents;
vector<pair<int, int>> lst[N + 5];
int sparse[K + 1][N + 5];
int max_val[K + 1][N + 5];
int depth[N + 5];

int findDSU(int x) {
  if (parents[x] == x) return x;
  return parents[x] = findDSU(parents[x]);
}

vector<int> findMSTEdgeIdx(const int &n, const int &m, const set<int> &banned) {
  vector<int> rnk(m, 0);
  iota(begin(rnk), end(rnk), 0);
  iota(begin(parents), end(parents), 0);

  sort(rnk.begin(), rnk.end(), [&](const int &l, const int &r) {
    if (edges[l].weight < edges[r].weight) {
      return true;
    }
    return false;
  });

  vector<int> used;
  for (int i = 0; i < m; i++) {
    int rank = rnk[i];
    if (banned.count(rank)) continue;

    int pu = findDSU(edges[rank].u);
    int pv = findDSU(edges[rank].v);
    if (pu == pv) continue;

    parents[pu] = pv;
    used.push_back(rank);
  }

  return used;
}

void dfs(int current, int previous, int d) {
  depth[current] = d;
  for (const auto &e : lst[current]) {
    if (e.first == previous) continue;
    sparse[0][e.first] = current;
    max_val[0][e.first] = e.second;
    dfs(e.first, current, d + 1);
  }
}

void buildLCA(int n, const vector<int> &edges_idx) {
  for (const int &id : edges_idx) {
    auto &edge = edges[id];
    lst[edge.u].push_back({edge.v, edge.weight});
    lst[edge.v].push_back({edge.u, edge.weight});
  }

  sparse[0][0] = 0, max_val[0][0] = 0;
  dfs(0, 0, 0);

  for (int i = 1; i <= K; i++) {
    for (int j = 0; j < n; j++) {
      int nx = sparse[i - 1][j];
      sparse[i][j] = sparse[i - 1][nx];
      max_val[i][j] = max(max_val[i - 1][j], max_val[i - 1][nx]);
    }
  }
}

int query(int u, int v) {
  if (depth[u] < depth[v]) swap(u, v);
  int diff = depth[u] - depth[v];
  int mx = 0;
  for (int i = K; i >= 0; i--) {
    if ((1 << i) & diff) {
      mx = max(mx, max_val[i][u]);
      u = sparse[i][u];
    }
  }

  if (u == v) return mx;

  for (int i = K; i >= 0; i--) {
    if (sparse[i][u] == sparse[i][v]) continue;
    mx = max({mx, max_val[i][u], max_val[i][v]});
    u = sparse[i][u];
    v = sparse[i][v];
  }

  mx = max({mx, max_val[0][u], max_val[0][v]});
  return mx;
}

int main() {
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
  int n, m;
  cin >> n >> m;
  for (int i = 0; i < m; i++) {
    edge tmp;
    cin >> tmp.u >> tmp.v >> tmp.weight;
    tmp.u--, tmp.v--;
    edges.emplace_back(tmp);
  }

  parents.resize(n, 0);

  auto initial_edges_mst_idx = findMSTEdgeIdx(n, m, set<int>());
  auto banned_edges =
      set<int>(initial_edges_mst_idx.begin(), initial_edges_mst_idx.end());

  auto edges_mst_idx = findMSTEdgeIdx(n, m, banned_edges);

  if (edges_mst_idx.size() != n - 1) {
    cout << -1 << endl;
    return 0;
  }

  // build LCA
  buildLCA(n, edges_mst_idx);

  // query LCA
  ll total = 0;
  for (const int &idx : initial_edges_mst_idx) {
    total += query(edges[idx].u, edges[idx].v) - edges[idx].weight + 1;
  }

  cout << total << endl;

  return 0;
}
