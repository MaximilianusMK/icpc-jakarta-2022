#include <bits/stdc++.h>

std::vector<long long> dijkstra(int u, const std::vector<int> &h,
                                const std::vector<std::vector<int>> &adj) {
  int N = h.size();
  std::vector<long long> dist(N, 1e18);
  dist[u] = 0;
  std::priority_queue<std::pair<long long, int>> pq;
  pq.emplace(0LL, u);

  while (!pq.empty()) {
    long long d; int u;
    std::tie(d, u) = pq.top();
    pq.pop();
    d = -d;

    if (d != dist[u]) continue;

    for (int v : adj[u]) {
      long long nd = d + 1LL * (h[u] - h[v]) * (h[u] - h[v]);
      if (nd >= dist[v]) continue;
      dist[v] = nd;
      pq.emplace(-nd, v);
    }
  }
  return dist;
}

int main() {
  int N, M, S, T;
  scanf("%d %d %d %d", &N, &M, &S, &T);
  --S, --T;
  std::vector<int> H(N);
  for (int i = 0; i < N; ++i) {
    scanf("%d", &H[i]);
  }
  std::vector<std::vector<int>> adj(N);
  for (int j = 0; j < M; ++j) {
    int U, V;
    scanf("%d %d", &U, &V);
    --U, --V;

    adj[U].push_back(V);
    adj[V].push_back(U);
  }

  std::vector<long long> distS = dijkstra(S, H, adj);
  std::vector<long long> distT = dijkstra(T, H, adj);

  long long ans = distS[T];
  for (int v : adj[S]) ans = std::min(ans, distT[v]);  // Change S altitude
  for (int v : adj[T]) ans = std::min(ans, distS[v]);  // Change T altitude
  ans *= 2;

  int maxHull = 0;
  for (int u = 0; u < N; ++u) {
    // 2distS[x] + 2distT[y] + (H[x] - H[y])^2
    // 2distS[x] + H[x]^2 - 2H[x]H[y] + 2distT[y] + H[y]^2

    using Line = std::pair<int, long long>;

    std::vector<Line> lines, hull;
    for (int v : adj[u]) {
      lines.emplace_back(-2 * H[v], 2 * distS[v] + 1LL * H[v] * H[v]);
    }
    std::sort(lines.begin(), lines.end(), std::greater<Line>());

    auto cross = [&](const Line &p, const Line &q, const Line &r) {
      return (q.second - p.second) * (q.first - r.first) - (r.second - q.second) * (p.first - q.first);
    };

    for (Line line : lines) {
      while (!hull.empty() && hull.back().first == line.first) hull.pop_back();
      while (hull.size() >= 2 && cross(hull.end()[-2], hull.end()[-1], line) >= 0) hull.pop_back();
      hull.push_back(line);
    }

    auto eval = [&](const Line &l, int x) { return 1LL * l.first * x + l.second; };

    for (int v : adj[u]) {
      int l = 0, r = hull.size() - 1;
      while (l <= r) {
        int mid = (l + r) / 2;
        if (mid + 1 < hull.size() && eval(hull[mid + 1], H[v]) < eval(hull[mid], H[v])) {
          l = mid + 1;
        } else if (mid > 0 && eval(hull[mid - 1], H[v]) < eval(hull[mid], H[v])) {
          r = mid - 1;
        } else {
          ans = std::min(ans, eval(hull[mid], H[v]) + 2 * distT[v] + 1LL * H[v] * H[v]);
          break;
        }
      }
    }
  }

  printf("%lld.%06d\n", ans / 2, ans % 2 == 0 ? 0 : 500000);
  return 0;
}
