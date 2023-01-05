#include <bits/stdc++.h>
using namespace std;

const int N = 100000;
const int SQRT = 400;

int rnk[3][N / SQRT + 5];
string str[3][N / SQRT + 5];

void convertToNumber(int block) {
  vector<int> idx(3);
  iota(begin(idx), end(idx), 0);
  sort(idx.begin(), idx.end(), [&](const int &l, const int &r) {
    return str[l][block] < str[r][block];
  });

  int sz = 0;
  rnk[idx[0]][block] = sz;
  for (int i = 1; i < 3; i++) {
    if (str[idx[i]][block] != str[idx[i - 1]][block]) sz++;
    rnk[idx[i]][block] = sz;
  }
}

vector<vector<int>> getNumbers(int _l, int _r) {
  vector<vector<int>> numbers;
  for (int i = 0; i < 3; i++) {
    int l = _l, r = _r;
    vector<int> num;
    while (l % SQRT != 0 && l <= r) {
      num.push_back(str[i][l / SQRT][l % SQRT] - 'a');
      l++;
    }
    while (l + SQRT <= r) {
      num.push_back(rnk[i][l / SQRT]);
      l += SQRT;
    }
    while (l <= r) {
      num.push_back(str[i][l / SQRT][l % SQRT] - 'a');
      l++;
    }
    numbers.emplace_back(num);
  }
  return numbers;
}

vector<int> getRank(int _l, int _r) {
  auto numbers = getNumbers(_l, _r);

  vector<int> rnk(3);
  iota(begin(rnk), end(rnk), 0);
  sort(rnk.begin(), rnk.end(),
       [&](const int &l, const int &r) { return numbers[l] < numbers[r]; });

  return rnk;
}

void charSwap(int idx, const vector<int> &r) {
  char tmp[3];
  for (int i = 0; i < 3; i++) {
    tmp[i] = str[i][idx / SQRT][idx % SQRT];
  }
  for (int i = 0; i < 3; i++) {
    str[i][idx / SQRT][idx % SQRT] = tmp[r[i]];
  }
}

void stringSwap(int idx, const vector<int> &r) {
  string tmp[3];

  int tmp_rnk[3];
  for (int i = 0; i < 3; i++) {
    tmp[i].swap(str[i][idx]);
    tmp_rnk[i] = rnk[i][idx];
  }
  for (int i = 0; i < 3; i++) {
    str[i][idx].swap(tmp[r[i]]);
    rnk[i][idx] = tmp_rnk[r[i]];
  }
}

void swapBasedonRank(int _l, int _r, const vector<int> &rnk) {
  int l = _l, r = _r;
  while (l % SQRT != 0 && l <= r) {
    charSwap(l, rnk);
    l++;
  }
  while (l + SQRT <= r) {
    stringSwap(l / SQRT, rnk);
    l += SQRT;
  }
  while (l <= r) {
    charSwap(l, rnk);
    l++;
  }

  convertToNumber(_l / SQRT);
  convertToNumber(_r / SQRT);
}

int main() {
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
  int n, q;
  cin >> n >> q;
  for (int i = 0; i < 3; i++) {
    for (int idx = 0; idx < n; idx++) {
      char c;
      cin >> c;
      str[i][idx / SQRT] += c;
    }
  }

  for (int idx = 0; idx < n; idx += SQRT) {
    convertToNumber(idx / SQRT);
  }

  for (int t = 0; t < q; t++) {
    int l, r;
    cin >> l >> r;
    l--, r--;

    auto rnk = getRank(l, r);
    swapBasedonRank(l, r, rnk);
  }

  for (int i = 0; i < 3; i++) {
    for (int idx = 0; idx < n; idx += SQRT) {
      cout << str[i][idx / SQRT];
    }
    cout << '\n';
  }
  return 0;
}
