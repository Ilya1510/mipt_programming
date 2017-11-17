#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

using namespace std;

vector <vector<int> > ans;

class MinCostFlow {
    static const int N = 300;
    static const int INF = 2000000001;

    struct Edge {
        int v, cap, flow, cost, ind;
        size_t backEdgeNumber;
        Edge(int v, int cap, int flow, size_t backEdgeNumber, int cost, int ind) :
                v(v), cap(cap), flow(flow), backEdgeNumber(backEdgeNumber), cost(cost), ind(ind) {}
    };

    vector <Edge> G[N];
    int d[N];
    pair<int, int> pred[N];
    int n, s, t;
    int curFlow;
    int curMinCost;

    bool bfs() {
        for (int i = 1; i <= n; ++i) {
            d[i] = INF;
        }
        d[s] = 0;
        for (;;) {
            bool any = false;
            for (int i = 1; i <= n; ++i) {
                for (int j = 0; j < G[i].size(); ++j) {
                    Edge& edge = G[i][j];
                    if (edge.flow < edge.cap) {
                        if (d[i] < INF && d[edge.v] > d[i] + edge.cost) {
                            d[edge.v] = d[i] + edge.cost;
                            pred[edge.v] = make_pair(i, j);
                            any = true;
                        }
                    }
                }
            }
            if (!any) {
                break;
            }
        }

        if (d[t] == INF) {
            return false;
        }
        curFlow += 1;
        for (int j = n; j != 1; j = pred[j].first) {
            Edge& edge = G[pred[j].first][pred[j].second];
            curMinCost += edge.cost;
            edge.flow += 1;
            G[edge.v][edge.backEdgeNumber].flow -= 1;
        }
        return true;
    }
public:
    MinCostFlow(int n, int s, int t)
            : n(n), s(s), t(t) {
        curFlow = 0;
        curMinCost = 0;
    }

    void addEdge(int a, int b, int c, int cost = 1, int ind = 0) {
        G[a].push_back(Edge(b, c, 0, G[b].size(), cost, ind));
        G[b].push_back(Edge(a, 0, 0, G[a].size() - 1, -cost, ind));
    }

    int do_min_cost_max_flow() {
        do_min_cost_k_flow(INF);
        return curFlow;
    }

    bool do_min_cost_k_flow(int k) {
        curFlow = 0;
        curMinCost = 0;
        for (int i = 0; i < k; ++i) {
            if (!bfs()) {
                return false;
            }
        }
        return true;
    }

    int getFlow() const {
        return curFlow;
    }

    int getCost() const {
        return curMinCost;
    }

    vector<Edge>* getGraph() {
        return G;
    }

    int used[N];
    void dfs(int v) {
        used[v] = true;
        if (v == t) {
            return;
        }
        for (int i = 0; i < G[v].size(); ++i) {
            Edge& edge = G[v][i];
            if (edge.flow > 0 && !used[edge.v]) {
                edge.flow -= 1;
                G[edge.v][edge.backEdgeNumber].flow += 1;
                ans.back().push_back(edge.ind);
                return dfs(edge.v);
            }
        }
    }

    void createAnswer(int k) {
        for (int i = 0; i < k; ++i) {
            fill(used, used + n + 1, false);
            ans.push_back(vector<int>());
            dfs(s);
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
    int n, m, k;
    cin >> n >> m >> k;
    MinCostFlow minCostFlow(n, 1, n);
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        minCostFlow.addEdge(a, b, 1, c, i + 1);
        minCostFlow.addEdge(b, a, 1, c, i + 1);
    }
    minCostFlow.do_min_cost_k_flow(k);
    if (minCostFlow.getFlow() != k) {
        cout << -1 << endl;
        return 0;
    } else {
        cout.precision(10);
        minCostFlow.createAnswer(k);
        cout << 1.0 * minCostFlow.getCost() / k << endl;
        for (int i = 0; i < k; ++i) {
            cout << ans[i].size() << " ";
            for (auto x : ans[i]) {
                cout << x << " ";
            }
            cout << '\n';
        }
    }
    return 0;
}