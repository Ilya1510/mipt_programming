#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int v, cap;
    int flow;
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

int color[N];
vector <pair <int, int> > ans;
const int M = 100;
int board[M][M];

inline int getInVert(int x, int y) {
    return x * M + y;
}

inline int getOutVert(int x, int y) {
    return M * M + x * M + y;
}

inline pair<int, int> getPointFromVert(int num) {
    num %= M * M;
    return make_pair(num / M, num % M);
}

void dfs1(int s) {
    color[s] = 1;
    for (int i = 0; i < G[s].size(); ++i) {
        if (G[s][i].flow < G[s][i].cap) {
            if (color[G[s][i].v] == 0) {
                dfs1(G[s][i].v);
            }
        }
    }
    color[s] = 2;
}

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

int main() {
    int m;
    cin >> m >> n;
    int k, l;
    cin >> k >> l;
    for (int i = 0; i < k; ++i) {
        int x, y;
        cin >> x >> y;
        board[x][y] = 2; // mountain
    }
    for (int i = 0; i < l; ++i) {
        int x, y;
        cin >> x >> y;
        board[x][y] = 1;
    }
    int xa, ya, xb, yb;
    cin >> xa >> ya >> xb >> yb;
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (board[i][j] == 1) {
                addEdge(getInVert(i, j), getOutVert(i, j), 1);
            } else if (board[i][j] == 0) {
                addEdge(getInVert(i, j), getOutVert(i, j), INF);
            }
            for (int q = 0; q < 4; ++q) {
                int xn = i + dx[q];
                int yn = j + dy[q];
                addEdge(getOutVert(i, j), getInVert(xn, yn), INF);
            }
        }
    }
    int predN = n;
    s = getInVert(xa, ya);
    t = getOutVert(xb, yb);
    n = 2 * M * M + 1;
    int maxFlow = get_max_flow();
    if (maxFlow == INF) {
        cout << -1 << endl;
    } else {
        dfs1(s);
        if (maxFlow == 0) {
            cout << 0 << endl;
        } else {
            for (int i = 1; i <= m; ++i) {
                for (int j = 1; j <= predN; ++j) {
                    if (board[i][j] == 1) {
                        int vIn = getInVert(i, j);
                        int vOut = getOutVert(i, j);
                        if (color[vIn] && !color[vOut]) {
                            ans.push_back(make_pair(i, j));
                        }
                    }
                }
            }
            cout << ans.size() << endl;
            for (int i = 0; i < ans.size(); i++) {
                pair <int, int> p = ans[i];
                cout << p.first << " " << p.second << endl;
            }
        }
    }

}