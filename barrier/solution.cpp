// Solution O(N^2 log N)

#include <bits/stdc++.h>

using namespace std;

typedef long long LD;

inline bool eq(LD x, LD y) { return x == y; }
inline bool le(LD x, LD y) { return x <= y; }
inline bool lt(LD x, LD y) { return x < y; }
inline int sign(LD x) { return eq(x, 0) ? 0 : (x < 0 ? -1 : 1); }

struct point {
  LD x, y;
  int id;
  point(LD _x = 0, LD _y = 0, int _id = -1) : x(_x), y(_y), id(_id) {}
  point operator+(point p) { return point(x+p.x, y+p.y, id); }
  point operator-(point p) { return point(x-p.x, y-p.y, id); }
  point operator*(LD s) { return point(x * s, y * s, id); }
  point operator/(LD s) { return point(x / s, y / s, id); }
  LD operator*(point p) { return x * p.x + y * p.y; }
  LD operator%(point p) { return x * p.y - y * p.x; }
  LD norm2() { return *this * *this; }
  bool operator<(point p) const { return eq(y, p.y) ? x < p.x : y < p.y; }
  bool operator==(point p) { return eq(x, p.x) && eq(y, p.y); }
};

ostream& operator<<(ostream & os, point p) {
  return os << "(" << p.x << ", " << p.y << ")";
}

// 1 : ccw, -1 : cw, 0 : colinear
int ccw(point a, point b, point c) {
  return sign((b-a) % (c-b));
}

void sort_circular(vector<point>& vp) {
  vector<point> upper, lower;
  point origin;
  for (point p : vp) {
    if (origin < p) {
      upper.push_back(p);
    } else {
      lower.push_back(p);
    }
  }
  sort(lower.begin(), lower.end(), [&](point& le, point& ri) {
    return (le % ri) > 0;
  });
  sort(upper.begin(), upper.end(), [&](point& le, point& ri) {
    return (le % ri) > 0;
  });
  upper.insert(upper.end(), lower.begin(), lower.end());
  vp.swap(upper);
}

int main() {
  int n;
  cin >> n;
  vector<point> vp(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y;
    vp[i] = point(x, y, i);
  }
  vector<vector<int>> cnt(n, vector<int>(n, 0));
  for (int i = 0; i < n; ++i) {
    vector<point> others;
    others.reserve(n-1);
    for (int j = 0; j < n; ++j) {
      if (j == i) continue;
      others.push_back(vp[j] - vp[i]);
    }
    sort_circular(others);
    const int sz = n-1;
    for (int j = 0, cur = 0; j < sz; ++j) {
      while (cur < sz-1 && (others[j] % others[(j+cur+1) % sz]) > 0) ++cur;
      cnt[i][others[j].id] = cur;
      cur = max(0, cur-1);
    }
  }
  vector<vector<int>> ray_intersects(n, vector<int>(n, 0));
  for (int i = 0; i < n; ++i) {
    vector<point> others;
    others.reserve(2 * (n-1));
    for (int j = 0; j < n; ++j) {
      if (j == i) continue;
      others.push_back(vp[j] - vp[i]);
      others.push_back(vp[i] - vp[j]);
      others.back().id = ~j;
    }
    sort_circular(others);
    const int sz = others.size();
    int start = 0;
    while (others[start].id < 0) ++start;
    int cur = 0, added = 0;
    for (int j = start+1; j < sz && (others[start] % others[j]) > 0; ++j) {
      if (others[j].id >= 0) {
        auto id = others[j].id;
        cur += cnt[i][id];
        cur -= added;
        ++added;
      }
    }
    for (int j = start; j < sz; ++j) {
      int id = others[j].id;
      if (id < 0) {
        id = ~id;
        cur += cnt[i][id] - cnt[id][i];
      } else {
        ray_intersects[i][id] = cur;
      }
    }
  }
  int best = n * (n-1) / 2;
  for (int i = 0; i < n; ++i) {
    for (int j = i+1; j < n; ++j) {
      best = min(best, ray_intersects[i][j] + ray_intersects[j][i] +
             cnt[i][j] * (cnt[i][j] - 1) / 2 + cnt[j][i] * (cnt[j][i]-1) / 2);
    }
  }
  best = n * (n-1) / 2 - (2 * (n-1)-1) - best;
  cout << best << endl;
  return 0;
}
