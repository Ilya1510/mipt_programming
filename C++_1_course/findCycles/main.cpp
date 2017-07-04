#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int N = 100001;
vector <int> g[N];
vector <int> cycle;
int color[N];
int pred[N];

void dfs(int s) {
    color[s] = 1;
    for (size_t i = 0; i < g[s].size(); ++i) {
        int v = g[s][i];
        if (!color[v]) {
            pred[v] = s;
            dfs(v);
        } else if (color[v] == 1) {
            cycle.clear();
            cycle.push_back(s);
            int x = s;
            while (x != v) {
                x = pred[x];
                cycle.push_back(x);
            }
            reverse(cycle.begin(), cycle.end());
        }
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
    }
    for (int i = 1; i <= n; ++i) {
        if (!color[i]) {
            dfs(i);
        }
    }
    if (cycle.empty()) {
        cout << "NO" << endl;
    } else {
        cout << "YES" << endl;
        for (size_t i = 0; i < cycle.size(); ++i) {
            cout << cycle[i] << " ";
        }
        cout << endl;
    }
    return 0;
}