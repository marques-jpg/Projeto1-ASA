#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using namespace std;

int main() {
  ios::sync_with_stdio(0);
  cin.tie(nullptr);

  int n;
  if (!(cin >> n)) return 0;
  vector<unsigned long long> P(n + 2);
  vector<char> C(n + 2);
  P[0] = P[n+1] = 1;
  C[0] = C[n+1] = 'T';

  for (int i = 1; i <= n; ++i) cin >> P[i];
  string s; cin >> s;
  for (int i = 1; i <= n; ++i) C[i] = s[i-1];

  auto Af = [&](char a, char b) -> long long {
    if (a == 'T' || b == 'T') return 1;
    int ra, rb;
    auto idx = [&](char x)->int{
      if (x=='P') return 0;
      if (x=='N') return 1;
      if (x=='A') return 2;
      return 3;
    };
    ra = idx(a); rb = idx(b);
    static const int T[4][4] = {
      {1,3,1,3},
      {5,1,0,1},
      {0,1,0,4},
      {1,3,2,3}
    };
    return T[ra][rb];
  };

  int N = n + 2;
  vector<vector<long long>> dp(N, vector<long long>(N, 0));
  vector<vector<vector<int>>> seq(N, vector<vector<int>>(N));

  for (int i = 0; i < N; ++i) {
    dp[i][i] = 0;
    if (i+1 < N) dp[i][i+1] = 0;
  }

  for (int len = 2; len < N; ++len) {
    for (int l = 0; l + len < N; ++l) {
      int r = l + len;
      long long best = 0;
      vector<int> bestSeq;
      for (int k = l + 1; k <= r - 1; ++k) {
        long long gain = P[l] * Af(C[l], C[k]) * P[k] + P[k] * Af(C[k], C[r]) * P[r];
        long long candVal = dp[l][k] + dp[k][r] + gain;
        vector<int> candSeq;
        candSeq.reserve((int)seq[l][k].size() + (int)seq[k][r].size() + 1);
        candSeq.insert(candSeq.end(), seq[l][k].begin(), seq[l][k].end());
        candSeq.insert(candSeq.end(), seq[k][r].begin(), seq[k][r].end());
        candSeq.push_back(k);
        if (candVal > best || (candVal == best && candSeq < bestSeq)) {
          best = candVal;
          bestSeq = move(candSeq);
        }
      }
      if (best != 0) {
        dp[l][r] = best;
        seq[l][r] = move(bestSeq);
      }
    }
  }

  long long result = dp[0][n+1];
  cout << result << "\n";
  for (size_t i = 0; i < seq[0][n+1].size(); ++i) {
    if (i) cout << ' ';
    cout << seq[0][n+1][i];
  }
  cout << "\n";
  return 0;
}