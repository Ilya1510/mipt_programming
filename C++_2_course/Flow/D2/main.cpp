#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class MaxMatch {
    static const int N = 10100;
    
    struct Edge {
        int from;
        int to;
        int flow;
        int cap;
    };

    vector<Edge> edges;
    vector<size_t> graph[N];

    int n, m;
    bool used[N];
    size_t predEdge[N];
    bool have[N];
public:
    int result = 0;
    int costV[N];

    MaxMatch(int n, int m) : n(n), m(m)
    {}

    void addEdge(int from, int to, int cap) {
        graph[from].push_back(edges.size());
        edges.push_back(Edge{from, to, 0, cap});
        graph[to].push_back(edges.size());
        edges.push_back(Edge{to, from, 0, 0});
    }

    void addFlow(size_t edgeNum, int flow) {
        edges[edgeNum].flow += flow;
        edges[edgeNum ^ 1].flow -= flow;
    }

    void addLeftVertexToMatch(int v) {
        fill(used, used + n + m + 1, false);
        queue<int> q;
        q.push(v);
        while (!q.empty()) {
            int curV = q.front();
            q.pop();
            if (used[curV])
                continue;
            used[curV] = true;
            for (auto to: graph[curV])
                if (edges[to].cap > edges[to].flow && !used[edges[to].to]) {
                    predEdge[edges[to].to] = to;
                    q.push(edges[to].to);
                }
        }
        int res = -1;
        for (int i = n + 1; i <= n + m; ++i) {
            if (used[i] && !have[i] && (res == -1 || costV[i] > costV[res])) {
                res = i;
            }
        }
        if (res != -1) {
            have[res] = true;
            result += costV[v] + costV[res];
            for (int curv = res; curv != v; curv = edges[predEdge[curv]].from) {
                addFlow(predEdge[curv], 1);
            }
        }
    }
    const vector <size_t> getMatch() const {
        vector <size_t> match;
        for (size_t i = 0; i < edges.size(); ++i) {
            Edge ed = edges[i];
            if (ed.flow > 0 && ed.from >= 1
                && ed.from <= n && ed.to >= n + 1
                && ed.to <= n + m)
                match.push_back(i / 2);
        }
        return match;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n, m, e;
    cin >> n >> m >> e;

    MaxMatch maxMatch(n, m);
    for (int i = 1; i <= n + m; ++i) {
        cin >> maxMatch.costV[i];
    }

    for (int i = 0; i < e; ++i) {
        int from, to;
        cin >> from >> to;
        to += n;
        maxMatch.addEdge(from, to, 1);
    }

    vector<pair<int, int>> leftV;
    for (int i = 1; i <= n; ++i) {
        leftV.push_back({maxMatch.costV[i], i});
    }
    sort(leftV.rbegin(), leftV.rend());
    for (int i = 0; i < n; ++i) {
        maxMatch.addLeftVertexToMatch(leftV[i].second);
    }

    cout << maxMatch.result << endl;
    auto match = maxMatch.getMatch();
    cout << match.size() << endl;
    for (auto x: match) {
        cout << x + 1 << " ";
    }
    cout << endl;
    return 0;
}