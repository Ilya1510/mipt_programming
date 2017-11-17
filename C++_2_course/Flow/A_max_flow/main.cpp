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
    for (int i = 1; i <= n; ++i) {
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
        for (int j = 1; j <= n; ++j) {
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

int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    cin >> n;
    s = 1, t = n;
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        addEdge(a, b, c);
    }

    cout << get_max_flow() << endl;
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        int c, x, y;
        cin >> c >> x >> y;
        for (int j = 0; j < G[x].size(); ++j) {
            if (G[x][j].v == y) {
                G[x][j].cap += (c == 1 ? 1 : -1);
                break;
            }
        }
        for (int v = 1; v <= n; ++v) {
            for (int j = 0; j < G[v].size(); ++j) {
                G[v][j].flow = 0;
            }
        }
        cout << get_max_flow() << endl;
    }
    return 0;
}