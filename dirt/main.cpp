#include <iostream>
#include <deque>
#include <queue>

using namespace std;

const int N = 500;
const int INF = 10000000;
char tab[N][N];
int color[N][N];
int dn[] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dm[] = {-1, 0, 1, 1, 1, 0, -1, -1};
int dp[N][N];
int n, m;

bool inTable(int a, int b) {
    return a >= 0 && b >= 0 && a < n && b < m;
}

void bfs01(int as, int bs) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0 ; j < m;++j) {
            dp[i][j] = INF;
        }
    }
    queue <pair<int, int> > q[2];
    q[0].push({as, bs});
    dp[as][bs] = 0;
    while (!q[0].empty() || !q[1].empty()) {
        pair <int, int> cur;
        if (!q[0].empty()) {
            cur = q[0].front();
            q[0].pop();
        } else {
            cur = q[1].front();
            q[1].pop();
        }
        int a = cur.first;
        int b = cur.second;
        if (color[a][b])
            continue;
        color[cur.first][cur.second] = 1;
        for (int i = 0; i < 8; ++i) {
            int an = a + dn[i];
            int bn = b + dm[i];
            if (inTable(an, bn) && tab[an][bn] != '0' && color[an][bn] == 0) {
                if (tab[an][bn] == tab[a][b]) {
                    q[0].push({an, bn});
                    dp[an][bn] = dp[a][b];
                } else {
                    q[1].push({an, bn});
                    dp[an][bn] = dp[a][b] + 1;
                }
            }
        }
    }
}

int dw[N][N];

void bfs2(int as, int bs) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            dw[i][j] = INF;
            color[i][j] = 0;
        }
    }
    queue <pair <int, int> > q;
    q.push({as, bs});
    dw[as][bs] = 1;
    color[as][bs] = 1;

    while (!q.empty()) {
        auto&& cur = q.front();
        q.pop();
        int a = cur.first;
        int b = cur.second;
        for (int i = 0; i < 8; ++i) {
            int an = a + dn[i];
            int bn = b + dm[i];
            if (inTable(an, bn) && tab[an][bn] != '0' && color[an][bn] == 0) {
                if (dp[an][bn] == (dp[a][b] + (tab[an][bn] != tab[a][b]))) {
                    dw[an][bn] = min(dw[an][bn], dw[a][b] + 1);
                    color[an][bn] = 1;
                    q.push({an, bn});
                }
            }
        }
    }
}

int main() {
    cin >> n >> m;
    int a[2], b[2];
    for (int i = 0; i < 2; ++i) {
        cin >> a[i] >> b[i];
        a[i]--;
        b[i]--;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> tab[i][j];
        }
    }
    bfs01(a[0], b[0]);
    bfs2(a[0], b[0]);
    if (dp[a[1]][b[1]] == INF) {
        cout << 0 << " " << 0 << endl;
    } else {
        cout << dw[a[1]][b[1]] << " " << dp[a[1]][b[1]] << endl;
    }
    return 0;
}
 /*
5 9
1 1
2 9
121102221
121210221
111212220
222211221
111112020
  */