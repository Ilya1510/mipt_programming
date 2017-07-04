#include <iostream>
#include <vector>

using namespace std;

int getColor(int ind, char bun) {
    if (bun == 'R') {
        return ind + 1;
    } else if (bun == 'G') {
        return ind == 0 ? 0 : 2;
    } else {
        return ind;
    }
}

char tc[3] = {'R', 'G', 'B'};

enum Color {
    R = 0,
    G = 1,
    B = 2
};

const int N = 1001;
vector <int> g[2 * N];
vector <int> gt[2 * N];
bool used[2 * N];
int comp[2 * N];
vector<int> order;

void dfs1 (int v) {
    used[v] = true;
    for (size_t i = 0; i < g[v].size(); ++i) {
        int to = g[v][i];
        if (!used[to])
            dfs1 (to);
    }
    order.push_back(v);
}

void dfs2 (int v, int num) {
    comp[v] = num;
    for (size_t i = 0; i < gt[v].size(); ++i) {
        int to = gt[v][i];
        if (comp[to] == -1)
            dfs2 (to, num);
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    string s;
    cin >> s;
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        a--; b--;
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                if (getColor(j, s[a]) == getColor(k, s[b])) {
                    g[2 * a + j].push_back(2 * b + 1 - k);
                    g[2 * b + k].push_back(2 * a + 1 - j);
                    gt[2 * b + 1 - k].push_back(2 * a + j);
                    gt[2 * a + 1 - j].push_back(2 * b + k);
                }
            }
        }
    }

    for (int i = 0; i < 2 * n; ++i) {
        used[i] = 0;
    }
    for (int i = 0; i < 2 * n; ++i) {
        if (!used[i]) {
            dfs1(i);
        }
    }
    for (int i = 0; i < 2 * n; ++i) {
        comp[i] = -1;
    }
    for (int i = 0, j = 0; i < 2 * n; ++i) {
        int v = order[2 * n - i - 1];
        if (comp[v] == -1)
            dfs2(v, j++);
    }

    for (int i = 0; i < n; ++i) {
        if (comp[2 * i] == comp[2 * i + 1]) {
            cout << "Impossible" << endl;
            return 0;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (comp[2 * i] > comp[2 * i + 1]) {
            cout << tc[getColor(0, s[i])];
        } else {
            cout << tc[getColor(1, s[i])];
        }
    }
    cout << endl;
    return 0;
}