#include <iostream>
#include <vector>
#include <>

using namespace std;

typedef long long ll;

const int N = 105;
const ll INF = 1e17;

struct Edge {
    int flow;
    int cap;
    ll cost;
};

Edge eds[N][N];
ll phi[N];
ll dst[N];
bool used[N];
int pr[N];

void addEdge(int from, int to, int cap, ll cost) {
    eds[from][to] = Edge{0, cap, cost};
    eds[to][from] = Edge{0, 0, -cost};
}

void addFlow(int from, int to, int flow) {
    eds[from][to].flow += flow;
    eds[to][from].flow -= flow;
}

void bellman(int s, int t) {
    fill(phi, phi + t + 1, INF);
    phi[s] = 0LL;
    for (int iter = 0; iter <= t; ++iter)
        for (int i = 0; i <= t; ++i)
            for (int j = 0; j <= t; ++j)
                if (eds[i][j].cap > eds[i][j].flow && phi[j] > phi[i] + eds[i][j].cost) {
                    phi[j] = phi[i] + eds[i][j].cost;
                    pr[j] = i;
                }
}

void dijkstra(int s, int t) {
    fill(dst, dst + t + 1, INF);
    fill(used, used + t + 1, false);
    dst[s] = 0;
    for (int iter = 0; iter <= t; ++iter) {
        int v = -1;
        for (int i = 0; i <= t; ++i)
            if (!used[i] && (v == -1 || dst[i] < dst[v]))
                v = i;
        if (dst[v] >= INF)
            break;
        used[v] = true;
        for (int to = 0; to <= t; ++to)
            if (eds[v][to].cap > eds[v][to].flow && dst[to] > dst[v] + eds[v][to].cost + phi[v] - phi[to]) {
                dst[to] = dst[v] + eds[v][to].cost + phi[v] - phi[to];
                pr[to] = v;
            }
    }
    for (int i = 0; i <= t; ++i)
        phi[i] = dst[i] + phi[i] - phi[s];
}

int main() {
    int n;
    scanf("%d", &n);
    int s = 0;
    int t = 2 * n + 1;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) {
            ll curcost;
            scanf("%lld", &curcost);
            addEdge(i, n + j, 1, curcost);
        }
    for (int i = 1; i <= n; ++i) {
        addEdge(s, i, k, 0);
        addEdge(n + i, t, k, 0);
    }
    bellman(s, t);
    int flow = 0;
    ll cost = 0;
    while (flow < k * n) {
        int canadd = 1e9;
        for (int curv = t; curv != s; curv = pr[curv])
            canadd = min(canadd, eds[pr[curv]][curv].cap - eds[pr[curv]][curv].flow);
        for (int curv = t; curv != s; curv = pr[curv]) {
            addFlow(pr[curv], curv, canadd);
            cost += (ll)canadd * eds[pr[curv]][curv].cost;
        }
        flow += canadd;
        dijkstra(s, t);
    }
    printf("%lld\n", cost);

    return 0;
}