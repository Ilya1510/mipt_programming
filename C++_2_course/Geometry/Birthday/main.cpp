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

const int N = 1000;
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

int cnt[N];
int color[N];
int ans[N];

vector <int> birth[N];
vector <int> minCover;

void dfs1(int s) {
    color[s] = 1;
    for (int i = 0; i < birth[s].size(); ++i) {
        int v = birth[s][i];
        if (color[v] == 0) {
            dfs1(v);
        }
    }
    color[s] = 2;
}

int main() {
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        int m;
        cin >> m >> n;
        for (int j = 1; j <= m; ++j) {
            for (int k = 1; k <= n; ++k) {
                cnt[k] = 0;
            }
            int x;
            cin >> x;
            while (x) {
                cnt[x] = 1;
                cin >> x;
            }
            for (int k = 1; k <= n; ++k) {
                if (!cnt[k]) {
                    addEdge(j, m + k, 1);
                }
            }
        }
        for (int j = 1; j <= m; ++j) {
            addEdge(0, j, 1);
        }
        for (int j = m + 1; j <= m + n; ++j) {
            addEdge(j, m + n + 1, 1);
        }
        s = 0;
        t = m + n + 1;
        n = m + n + 1;
        int maxMatch = get_max_flow();

        n -= m + 1;

        //! get graph for min vertices cover
        for (int j = 0; j <= m + n; ++j) {
            birth[j].clear();
        }
        for (int j = 1; j <= m; ++j) {
            for (int k = 0; k < G[j].size(); ++k) {
                if (G[j][k].flow == 1) {
                    birth[G[j][k].v].push_back(j);
                } else {
                    birth[j].push_back(G[j][k].v);
                }
            }
        }

        for (int j = 1; j <= m + n; ++j) {
            color[j] = 0;
        }
        for (int k = 0; k < G[0].size(); ++k) {
            if (!G[0][k].flow && !color[G[0][k].v]) {
                dfs1(G[0][k].v);
            }
        }
        //! get min vertices cover
        for (int j = 1; j <= m + n; ++j) {
            ans[j] = 1;
        }
        int countBoys = m;
        int countGirls = n;
        for (int j = 1; j <= m + n; ++j) {
            if (color[j] && j > m || !color[j] && j <= m) {
                ans[j] = 0;
                if (j <= m) {
                    countBoys -= 1;
                } else {
                    countGirls -= 1;
                }
            }
        }

        cout << countBoys + countGirls << endl;
        cout << countBoys << " " << countGirls << endl;
        for (int j = 1; j <= m; ++j) {
            if (ans[j]) {
                cout << j << " ";
            }
        }
        cout << endl;
        for (int j = m + 1; j <= m + n; ++j) {
            if (ans[j]) {
                cout << j - m << " ";
            }
        }
        for (int j = 0; j <= m + n + 1; ++j) {
            G[j].clear();
        }
        cout << endl;
        cout << endl;


    }

    return 0;
}