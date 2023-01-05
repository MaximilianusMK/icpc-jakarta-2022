#pragma GCC optimize "-O2"
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 131072;
const int OFFSET = MAXN;
const int INF = 1000000;

int N;
int A[MAXN];

struct tdata {
  int index;
  vector<int> p;
};
bool operator < (const tdata &a, const tdata &b) { return a.p < b.p; }

vector<tdata> get_all_points(int pivot) {
  vector<tdata> ret;
  ret.push_back({-1, {0, 0, 0}});
  vector<int> f(4, {0});
  for (int i = 0; i < N; ++i) {
    tdata one;
    for (auto value : {0, 1, 2, 3}) if (value != pivot) {
      if (A[i] == pivot) ++f[value];
      if (A[i] == value) --f[value];
      one.p.push_back(f[value]);
    }
    one.index = i;
    ret.push_back(one);
  }
  for (auto &one : ret)
    for (auto &p : one.p)
      p += OFFSET;
  return ret;
}

struct tnode {
  int score;
  int le, ri, yt;
};

int n_nodey = 0;
tnode nodex[MAXN * 4 + 5];
tnode nodey[MAXN * 64 + 5];

inline int LE(int k) { return (k << 1); }
inline int RI(int k) { return (k << 1) + 1; }

void reset() {
 memset(nodex, 0, sizeof(nodex));
 memset(nodey, 0, sizeof(nodey));  
 n_nodey = 0;
}

int alloc_y() {
  nodey[++n_nodey].score = INF;
  return n_nodey;
}

int update_y(int idy, int y, int score, int lo=0, int hi=OFFSET+MAXN) {
  int mid = ((lo + hi) >> 1);
  if (lo < hi) {
    if (y <= mid) {
      if (!nodey[idy].le) nodey[idy].le = alloc_y();
      score  = min(score, update_y(nodey[idy].le, y, score, lo, mid));
    }
    else {
      if (!nodey[idy].ri) nodey[idy].ri = alloc_y();
      score  = min(score, update_y(nodey[idy].ri, y, score, mid+1, hi));
    }
  }
  nodey[idy].score = min(nodey[idy].score, score);
  return nodey[idy].score;
}

void update(int x, int y, int score, int lo=0, int hi=OFFSET+MAXN, int idx=1) {
  int mid = ((lo + hi) >> 1);
  if (lo < hi) {
    if (x <= mid)
      update(x, y, score, lo, mid, LE(idx));
    else
      update(x, y, score, mid+1, hi, RI(idx));
  }
  if (!nodex[idx].yt) nodex[idx].yt = alloc_y();
  update_y(nodex[idx].yt, y, score);
}

int query_y(int idy, int y, int lo=0, int hi=OFFSET+MAXN) {
  if (y <= lo) return INF;
  if (hi < y) return nodey[idy].score;
  int ret = INF;
  int mid = ((lo + hi) >> 1);
  if (nodey[idy].le) ret = min(ret, query_y(nodey[idy].le, y, lo, mid));
  if (nodey[idy].ri) ret = min(ret, query_y(nodey[idy].ri, y, mid+1, hi));
  return ret;
}

int query(int x, int y, int lo=0, int hi=OFFSET+MAXN, int idx=1) {
  if (x <= lo) return INF;
  if (hi < x) return nodex[idx].yt ? query_y(nodex[idx].yt, y) : INF;
  int ret = INF;
  int mid = ((lo + hi) >> 1);
  ret = min(ret, query(x, y, lo, mid, LE(idx)));
  ret = min(ret, query(x, y, mid+1, hi, RI(idx)));
  return ret;
}


int solve(int pivot) {
  int ans = 0;

  vector<tdata> data = get_all_points(pivot);
  sort(data.begin(), data.end());

  reset();

  int last = 0;
  for (int i = 0; i < data.size(); ++i) {
    while (last < i and data[last].p[0] < data[i].p[0]) {
      update(data[last].p[1], data[last].p[2], data[last].index);
      ++last;
    }
    ans = max(ans, data[i].index - query(data[i].p[1], data[i].p[2]));
  }

  return ans;
}


int main(int argc, char *argv[]) {
  scanf("%d", &N);
  for (int i = 0; i < N; ++i) {
    scanf("%d", &A[i]);
  }

  for (int k = 0; k <= 3; ++k) {
    printf("%d%c", solve(k), k==3?'\n':' ');
  }
  
  return 0;
}
