#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

const int N = 100001;
vector <int> g[N];
set <int> cutpoints;
int color[N];
int pred[N];
int timer = 0, tin[N], fup[N];

void dfs(int s, int p = -1) {
    color[s] = 1;
    tin[s] = fup[s] = timer++;
    int childrenCount = 0;
    for (size_t i = 0; i < g[s].size(); ++i) {
        int v = g[s][i];
        if (v == p) continue;
        if (color[v]) { //back edge
            fup[s] = min(fup[s], tin[v]);
        } else {
            dfs(v, s);
            fup[s] = min(fup[s], fup[v]);
            if (fup[v] >= tin[s] && p != -1) {
                cutpoints.insert(s);
            }
            ++childrenCount;
        }
    }
    if (p == -1 && childrenCount > 1) {
        cutpoints.insert(s);
    }
    color[s] = 2;
}

int main() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    for (int i = 1; i <= n; ++i) {
        if (!color[i]) {
            dfs(i);
        }
    }
    cout << cutpoints.size() << endl;
    for (auto x : cutpoints) {
        cout << x << endl;
    }
    return 0;
}