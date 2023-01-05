#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e5;

int N;
long long A[MAXN+5];
long long P[MAXN+5];
priority_queue<long long, vector<long long>, greater<long long> > PQ;

int main() {
  cin >> N;
  for (int i = 0; i < N; i++) cin >> A[i];

  long long g = A[0];
  for (int i = 1; i < N; i++) g = __gcd(g, A[i]);

  for (int i = 0; i < N; i++) A[i] /= g;

  memset(P, 0, sizeof P);
  for (int i = 0; i < N; i++) {
    while (A[i] % 2 == 0) {
      P[i]++;
      A[i] /= 2;
    }
    PQ.push(P[i]);
  }

  while (PQ.size() > 1) {
    long long p1 = PQ.top();
    PQ.pop();

    long long p2 = PQ.top();
    PQ.pop();

    PQ.push(1 + min(p1, p2));
  }

  cout << g * (1LL << PQ.top()) << endl;

}
