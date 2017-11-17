#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int v, cap, flow;
    size_t backEdgeNumber;
    Edge(int v, int cap, int flow, size_t backEdgeNumber) :
            v(v), cap(cap), flow(flow), backEdgeNumber(backEdgeNumber) {}
};

const int N = 100000;
const int INF = 1000000000;
vector <Edge> G[N];
int d[N];
int ptr[N];
int n, s, t;

void addEdge(int a, int b, int c) {
    G[a].push_back(Edge(b, c, 0, G[b].size()));
    G[b].push_back(Edge(a, 0, 0, G[a].size() - 1));
}

bool bfs() {
    for (int i = 0; i <= n; ++i) {
        d[i] = INF;
    }
    d[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < G[u].size(); ++i) {
            Edge e = G[u][i];
            int v = e.v;
            if (e.flow < e.cap && d[v] == INF) {
                d[v] = d[u] + 1;
                q.push(v);
            }
        }
    }
    return d[t] != INF;
}

int dfs(int u, int minC) {
    if (u == t || minC == 0) {
        return minC;
    }
    for (int j = ptr[u]; j < G[u].size(); ++j) {
        Edge e = G[u][j];
        int v = e.v;
        int cuv = e.cap;
        int fuv = e.flow;
        size_t backEdgeNumber = e.backEdgeNumber;
        if (d[v] == d[u] + 1) {
            int delta = dfs(v, min(minC, cuv - fuv));
            if (delta != 0) {
                G[u][j].flow += delta;
                G[v][backEdgeNumber].flow -= delta;
                return delta;
            }
        }
        ptr[u]++;
    }
    return 0;
}

int get_max_flow() {
    int max_flow = 0;
    while (bfs()) {
        for (int j = 0; j <= n; ++j) {
            ptr[j] = 0;
        }
        int flow = dfs(s, INF);
        while (flow != 0) {
            max_flow += flow;
            flow = dfs(s, INF);
        }
    }
    return max_flow;
}

string str[N];

int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    int m, a, b;
    cin >> n >> m >> a >> b;
    swap(a, b);
    int countBad = 0;
    for (int i = 0; i < n; ++i) {
        cin >> str[i];
        for (int j = 0; j < m; ++j) {
            countBad += (str[i][j] == '*');
        }
    }
    if (2 * a <= b) {
        cout << a * countBad << endl;
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m - 1; ++j) {
            if (str[i][j] != '.' && str[i][j + 1] != '.' && (i + j) % 2 == 0) {
                addEdge(i * m + j, i * m + j + 1, 1);
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = m - 1; j > 0; --j) {
            if (str[i][j] != '.' && str[i][j - 1] != '.' && (i + j) % 2 == 0) {
                addEdge(i * m + j, i * m + j - 1, 1);
            }
        }
    }
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < m; ++j) {
            if (str[i][j] != '.' && str[i + 1][j] != '.' && (i + j) % 2 == 0) {
                addEdge(i * m + j, (i + 1) * m + j, 1);
            }
        }
    }
    for (int i = n - 1; i > 0; --i) {
        for (int j = 0; j < m; ++j) {
            if (str[i][j] != '.' && str[i - 1][j] != '.' && (i + j) % 2 == 0) {
                addEdge(i * m + j, (i - 1) * m + j, 1);
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if ((i + j) % 2 == 0 && str[i][j] == '*') {
                addEdge(m * n, i * m + j, 1);
            } else if (str[i][j] == '*') {
                addEdge(i * m + j, m * n + 1, 1);
            }
        }
    }

    s = m * n;
    t = m * n + 1;
    n = m * n + 1;
    int max_pairs = get_max_flow();
    cout << max_pairs * b + (countBad - 2 * max_pairs) * a << endl;

    return 0;
}