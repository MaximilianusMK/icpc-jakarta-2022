#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <stack>
#include <utility>
#include <vector>

void ac() {
  std::cout << "AC\n";
  exit(0);
}

void wa(std::string reason = "") {
  std::cout << "WA\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

int readInt(std::ifstream &ifs) {
  int ret;
  if (!(ifs >> ret)) wa();
  return ret;
}

bool eof(std::ifstream &ifs) {
  std::string dummy;
  if (ifs >> dummy) return false;
  return true;
}

using edge = std::pair<int, int>;
std::vector<edge> readEdges(std::ifstream &ifs, int n, int prev = -1) {
  std::vector<edge> e;
  int i = 0;
  if (prev != -1) i = 1;
  for (; i < 2 * n - 2; i++) {
    int v = readInt(ifs);
    if (i % 2) {
      e.push_back({prev - 1, v - 1});  // we use 0-based index in dsu rollback
    } else {
      prev = v;
    }
  }
  return e;
}

/* dsu rollback from cp-algorithms*/
struct dsu_save {
  int v, rnkv, u, rnku;

  dsu_save() {}

  dsu_save(int _v, int _rnkv, int _u, int _rnku)
      : v(_v), rnkv(_rnkv), u(_u), rnku(_rnku) {}
};

struct dsu_with_rollbacks {
  std::vector<int> p, rnk;
  int comps;
  std::stack<dsu_save> op;

  void init(int n) {
    p.resize(n);
    rnk.resize(n);
    for (int i = 0; i < n; i++) {
      p[i] = i;
      rnk[i] = 0;
    }
    comps = n;
  }

  int find_set(int v) { return (v == p[v]) ? v : find_set(p[v]); }

  bool unite(int v, int u) {
    v = find_set(v);
    u = find_set(u);
    if (v == u) return false;
    comps--;
    if (rnk[v] > rnk[u]) std::swap(v, u);
    op.push(dsu_save(v, rnk[v], u, rnk[u]));
    p[v] = u;
    if (rnk[u] == rnk[v]) rnk[u]++;
    return true;
  }

  void rollback() {
    if (op.empty()) return;
    dsu_save x = op.top();
    op.pop();
    comps++;
    p[x.v] = x.v;
    rnk[x.v] = x.rnkv;

    p[x.u] = x.u;
    rnk[x.u] = x.rnku;
  }

  bool is_all_connected() { return comps == 1; }
};

dsu_with_rollbacks dsu;

/*segment tree - stabbing query*/
const int N = 100000;
// store list of edges that can be used in this range query
std::vector<edge> segment_tree[4 * N + 5];

void insert(int idx, int l, int r, int le, int ri, const edge &e) {
  if (le > ri || r < le || ri < l) return;
  if (le <= l && r <= ri) {
    segment_tree[idx].push_back(e);
    return;
  }
  int mid = (l + r) / 2;
  insert(2 * idx, l, mid, le, ri, e);
  insert(2 * idx + 1, mid + 1, r, le, ri, e);
}

bool inRange(int x, int lo, int hi) { return lo <= x && x <= hi; }

void traverse(int idx, int l, int r) {
  for (const auto &e : segment_tree[idx]) {
    if (!dsu.unite(e.first, e.second)) {
      wa();  // cycle detected
    }
  }

  if (l == r) {
    if (!dsu.is_all_connected()) {
      wa();  // not a tree
    }
  } else {
    int mid = (l + r) / 2;
    traverse(2 * idx, l, mid);
    traverse(2 * idx + 1, mid + 1, r);
  }

  for (int i = 0; i < segment_tree[idx].size(); i++) {
    dsu.rollback();
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Must be run with arguments [input-file] [output-file] "
                 "[contestant-output]\n";
    exit(0);
  }

  std::ifstream inp(argv[1]);
  std::ifstream out(argv[2]);
  std::ifstream con(argv[3]);

  int N = readInt(inp);
  int judgeAns = readInt(out);
  int conAns = readInt(con);

  if (judgeAns == -1) {
    if (conAns != -1) wa();
    if (!eof(con)) wa();
    ac();
  }

  auto inp_edges = readEdges(inp, N);
  auto con_edges = readEdges(con, N, conAns);
  if (!eof(con)) wa();

  // check if contestant's edges duplicate or not
  // check if contestant's edge out of bound or not
  std::set<edge> used_set(inp_edges.begin(), inp_edges.end());
  for (const auto &e : con_edges) {
    if (used_set.count(e)) {
      // u and v already exists
      wa();
    } else if (!inRange(e.first, 0, N - 1) || !inRange(e.second, 0, N - 1)) {
      // node is not in range [0, N - 1]
      wa();
    }
    used_set.insert(e);
  }

  // build dsu and segment tree - stabbing query
  dsu.init(N);
  for (int i = 1; i <= N - 1; i++) {
    // input edges used from disaster level 1 to i - 1
    insert(1, 1, N - 1, 1, i - 1, inp_edges[i - 1]);

    // contestant edges used from disaster level i to N - 1
    insert(1, 1, N - 1, i, N - 1, con_edges[i - 1]);
  }

  traverse(1, 1, N - 1);

  ac();
}
