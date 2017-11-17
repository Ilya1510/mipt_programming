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

const int N = 1009;
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
        while (flow) {
            max_flow += flow;
            flow = dfs(s, INF);
        }
    }
    return max_flow;
}

vector <int> likes[N];
bool used[N];
bool comeOut[N];
vector <string> ans;
vector <int> group;

int main() {
    ios_base::sync_with_stdio(0);
    int k;
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) {
        int q;
        cin >> q;
        for (int j = 0; j < q; ++j) {
            int x;
            cin >> x;
            likes[i].push_back(x);
        }
    }
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        int size;
        cin >> size;
        group.clear();
        for (int j = 0; j < size; ++j) {
            int x;
            cin >> x;
            group.push_back(x);
        }
        for (int j = 1; j <= k; ++j) {
            used[j] = false;
            comeOut[j] = false;
        }
        for (int j = 0; j < group.size(); ++j) {
            int boy = group[j];
            for (int p = 0; p < likes[boy].size(); ++p) {
                used[likes[boy][p]] = true;
            }
        }
        int girlsCount = 0;
        for (int g = 1; g <= k; ++g) {
            girlsCount += used[g];
        }
        int ind = 0;
        for (int b = 1; b <= n; ++b) {
            if (group[ind] == b) {
                for (int p = 0; p < likes[b].size(); ++p) {
                    comeOut[likes[b][p]] = true;
                }
                ind++;
                if (ind == group.size()) {
                    break;
                }
            } else {
                for (int p = 0; p < likes[b].size(); ++p) {
                    int g = likes[b][p];
                    if (used[g] && !comeOut[g]) {
                        addEdge(b, n + g, 1);
                    }
                }
            }
        }
        for (int b = 1; b <= n; ++b) {
            addEdge(0, b, 1);
        }
        for (int g = 1; g <= k; ++g) {
            addEdge(n + g, n + k + 1, 1);
        }
        s = 0;
        t = n + k + 1;
        n = n + k + 1;
        int maxMatch = get_max_flow();
        if (maxMatch == girlsCount) {
            ans.push_back("Yes");
        } else {
            ans.push_back("No");
        }
        for (int j = 0; j <= n; ++j) {
            G[j].clear();
        }
        n -= k + 1;
    }
    for (auto s: ans) {
        cout << s << endl;
    }
}