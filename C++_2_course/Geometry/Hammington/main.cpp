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

int failors[N][2];
int color[N];

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

int main() {
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    string a;
    string b;
    cin >> b >> a;
    int l1 = (int)a.length();
    int l2 = (int)b.length();
    int minErros = 0;
    for (int i = 0; i < a.length(); ++i) {
        for (int j = i; j <= b.length() - l1 + i; ++j) {
            if (a[i] == '0' && b[j] == '1' ||
                    a[i] == '1' && b[j] == '0') {
                minErros++;
            }
            if (a[i] == '?') {
                if (b[j] == '0') { //fail 1
                    failors[i][1]++;
                } else if (b[j] == '1') {
                    failors[i][0]++;
                } else {
                    addEdge(i, l1 + j, 1);
                    addEdge(l1 + j, i, 1);
                }
            } else if (b[j] == '?') {
                failors[l1 + j]['1' - a[i]]++;
            }
        }
    }
    for (int i = 0; i < l1 + l2; ++i) {
        addEdge(l1 + l2, i, failors[i][1]);
        addEdge(i, l1 + l2 + 1, failors[i][0]);
    }
    n = l1 + l2 + 1;
    s = l1 + l2;
    t = l1 + l2 + 1;
    int addErrors = get_max_flow();
    dfs1(s);
    for (int i = 0; i < l1 + l2; ++i) {
        if (i < l1) {
            if (a[i] == '?') {
                if (color[i]) {
                    a[i] = '0';
                }
                if (a[i] == '?') {
                    a[i] = '1';
                }
            }
        } else {
            if (b[i - l1] == '?') {
                if (color[i]) {
                    b[i - l1] = '0';
                }
                if (b[i - l1] == '?') {
                    b[i - l1] = '1';
                }
            }
        }
    }
    cout << minErros + addErrors << endl;
    cout << b << endl;
    cout << a << endl;
    return 0;
}