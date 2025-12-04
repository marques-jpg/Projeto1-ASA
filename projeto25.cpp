#include <iostream>
#include <vector>
#include <functional>

using namespace std;

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);

  int n;
  if (!(cin >> n)) return 0;
  vector<unsigned long long> P(n + 2);
  vector<char> C(n + 2);
  P[0] = P[n+1] = 1;
  C[0] = C[n+1] = 'T';

  for (int i = 1; i <= n; ++i) {
    if (!(cin >> P[i])) {
      return 0;
    }
  }
  string s;
  if (!(cin >> s)) return 0;
  if ((int)s.size() != n) return 0;
  for (int i = 1; i <= n; ++i) C[i] = s[i-1];

  int N = n + 2;
  vector<int> type(N);
  auto mapChar = [](char x)->int {
    if (x == 'P') return 0;
    if (x == 'N') return 1;
    if (x == 'A') return 2;
    if (x == 'B') return 3;
    return 4;
  };
  for (int i = 0; i < N; ++i) type[i] = mapChar(C[i]);

  static const int AfTable[5][5] = {
    {1,3,1,3,1},
    {5,1,0,1,1},
    {0,1,0,4,1},
    {1,3,2,3,1},
    {1,1,1,1,1}
  };

  auto Af = [&](int a, int b)->int { return AfTable[a][b]; };

  auto idx = [&](int l, int r)->size_t { return (size_t)l * (size_t)N + (size_t)r; };
  vector<unsigned long long> dp((size_t)N * N, 0);
  vector<int> parent((size_t)N * N, -1);

  function<void(int,int,vector<int>&)> collect = [&](int l, int r, vector<int> &out) {
    int p = parent[idx(l,r)];
    if (p == -1) return;
    collect(l, p, out);
    collect(p, r, out);
    out.push_back(p);
  };

  for (int len = 2; len < N; ++len) {
    for (int l = 0; l + len < N; ++l) {
      int r = l + len;
      unsigned long long best = 0;
      int bestK = -1;
      for (int k = l + 1; k <= r - 1; ++k) {
        __int128 g1 = (__int128)P[l] * Af(type[l], type[k]) * (__int128)P[k];
        __int128 g2 = (__int128)P[k] * Af(type[k], type[r]) * (__int128)P[r];
        unsigned long long gain = (unsigned long long)(g1 + g2);

        unsigned long long cand = dp[idx(l,k)] + dp[idx(k,r)] + gain;

        if (cand > best) {
          best = cand;
          bestK = k;
        } else if (cand == best) {
          vector<int> candSeq;
          candSeq.reserve(len - 1);
          collect(l, k, candSeq);
          collect(k, r, candSeq);
          candSeq.push_back(k);

          if (bestK == -1) {
            bestK = k;
          } else {
            vector<int> curBestSeq;
            curBestSeq.reserve(len - 1);
            collect(l, bestK, curBestSeq);
            collect(bestK, r, curBestSeq);
            curBestSeq.push_back(bestK);
            if (candSeq < curBestSeq) bestK = k;
          }
        }
      }
      dp[idx(l,r)] = best;
      parent[idx(l,r)] = bestK;
    }
  }

  unsigned long long result = dp[idx(0, n+1)];
  cout << result << "\n";
  vector<int> out;
  out.reserve(n);
  collect(0, n+1, out);
  for (size_t i = 0; i < out.size(); ++i) {
    if (i) cout << ' ';
    cout << out[i];
  }
  cout << "\n";
  return 0;
}