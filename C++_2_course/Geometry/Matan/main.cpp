#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>

using namespace std;

struct Edge {
    int v, cap;
    int flow;
    size_t backEdgeNumber;
    Edge(int v, int cap, int flow, size_t backEdgeNumber) :
            v(v), cap(cap), flow(flow), backEdgeNumber(backEdgeNumber) {}
};

const int N = 1009;
const int INF = 1e6;
vector <Edge> G[N];
int d[N];
int ptr[N];
int PIn[N];
int POut[N];
bool used[N];
int e[N];
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

void push(int s, int t, int flow, bool isBack) {
    queue <int> q;
    q.push(s);
    e[s] = flow * (isBack ? -1 : 1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int i = 0; i < G[v].size(); ++i) {
            Edge& edge = G[v][i];
            int to = edge.v;
            Edge& backEdge = G[to][edge.backEdgeNumber];
            int add;
            if (edge.cap > 0) {
                if (!isBack) {
                    add = min(abs(e[v]), edge.cap - edge.flow);
                } else {
                    add = min(abs(e[v]), abs(edge.flow));
                }
            } else {
                if (isBack) {
                    add = min(abs(e[v]), backEdge.cap - backEdge.flow);
                } else {
                    add = min(abs(e[v]), abs(backEdge.flow));
                }
            }
            if (!used[to] && d[to] == d[v] + (isBack ? -1 : 1) && add) {
                if (isBack) { //! Входящее ребро
                    edge.flow -= add;
                    backEdge.flow += add;
                    e[to] -= add;
                    e[v] += add;
                } else if (!isBack) {
                    edge.flow += add;
                    backEdge.flow -= add;
                    e[to] += add;
                    e[v] -= add;
                }
                if (e[to] == add * (isBack ? -1 : 1)) {
                    q.push(to);
                }
                assert(edge.cap >= edge.flow);
                assert(backEdge.cap >= backEdge.flow);
                if (isBack) {
                    PIn[v] -= add;
                    POut[to] -= add;
                    assert(PIn[v] >= 0);
                    assert(POut[to] >= 0);
                } else {
                    POut[v] -= add;
                    PIn[to] -= add;
                    assert(POut[v] >= 0);
                    assert(PIn[to] >= 0);
                }
                if (!e[v]) {
                    break;
                }
            }
        }
    }
    e[t] = 0;
}

void delVertex(int s) {
    used[s] = true;
    for (int i = 0; i < G[s].size(); ++i) {
        Edge e = G[s][i];
        int v = e.v;
        if (e.cap > 0) { //! Исходящее ребро
            if (used[v] || d[v] != d[s] + 1) {
                continue;
            }
            PIn[v] -= e.cap - e.flow;
            assert(PIn[v] >= 0);
        } else { //! Обратное ребро
            if (used[v] || d[v] != d[s] - 1) {
                continue;
            }
            Edge backEdge = G[v][e.backEdgeNumber];
            POut[v] -= backEdge.cap - backEdge.flow;
            assert(POut[v] >= 0);
        }
        if (min(PIn[v], POut[v]) == 0) {
            delVertex(v);
        }
    }
}

void init() {
    for (int i = 0; i <= n; ++i) {
        used[i] = false;
        PIn[i] = 0;
        POut[i] = 0;
    }
    PIn[s] = INF;
    POut[t] = INF;
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j < G[i].size(); ++j) {
            Edge e = G[i][j];
            int v = e.v;
            if (d[v] == d[i] + 1) {
                if (e.cap > 0) {
                    PIn[v] += e.cap - e.flow;
                    POut[i] += e.cap - e.flow;
                }
            }
        }
    }
}

int getMaxFlow() {
    int maxFlow = 0;
    while (bfs()) {
        init();
        while (true) {

            for (int i = 0; i <= n; ++i) {
                if (!used[i] && !min(PIn[i], POut[i])) {
                    delVertex(i);
                }
            }
            int minP = 1e9;
            int idVertex = -1;
            for (int i = 0; i <= n; ++i) {
                if (!used[i] && min(PIn[i], POut[i]) < minP) {
                    minP = min(PIn[i], POut[i]);
                    idVertex = i;
                }
            }
            if (idVertex == -1) {
                break;
            }

            push(idVertex, t, minP, 0);
            push(idVertex, s, minP, 1);
            assert(minP > 0);
            assert(min(PIn[idVertex], POut[idVertex]) == 0);
            assert(maxFlow < INF);

            maxFlow += minP;

        }
    }
    return maxFlow;
}

int profit[N];

int main() {
    ios_base::sync_with_stdio(0);
    cin >> n;
    cerr << n << endl;
    int sumPositive = 0;
    for (int i = 1; i <= n; ++i) {
        cin >> profit[i];
        if (profit[i] > 0) {
            sumPositive += profit[i];
        }
    }
    for (int i = 1; i <= n; ++i) {
        int k;
        cin >> k;
        for (int j = 0; j < k; ++j) {
            int x;
            cin >> x;
            addEdge(i, x, INF);
        }
        if (profit[i] >= 0) {
            addEdge(0, i, profit[i]);
        } else {
            addEdge(i, n + 1, -profit[i]);
        }
    }
    s = 0;
    t = n + 1;
    n = n + 1;
    int maxFlow = getMaxFlow();
    assert(maxFlow <= sumPositive);
    cout << sumPositive - maxFlow << endl;
}