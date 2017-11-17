#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

const int MAX_SYMBOLS = 30;
const char MIN_SYMBOL = 'a';

class Bor {
    struct Vertex {
        int indNext[MAX_SYMBOLS];
        mutable int go[MAX_SYMBOLS];
        int indParent;
        char parentChar;
        mutable int link;
        mutable int up;
        bool leaf;
        mutable int countLeaf;
        mutable bool isCertainLeaf;
        Vertex() : leaf(false), indParent(-1), countLeaf(0), isCertainLeaf(false),
                   parentChar(-1), link(-1), up(-1) {
            fill(indNext, indNext + MAX_SYMBOLS, -1);
            fill(go, go + MAX_SYMBOLS, -1);
        }
        Vertex(int indParent, char parentChar) :
                indParent(indParent), parentChar(parentChar), isCertainLeaf(false),
                leaf(false), link(-1), up(-1), countLeaf(0) {
            fill(indNext, indNext + MAX_SYMBOLS, -1);
            fill(go, go + MAX_SYMBOLS, -1);
        }
    };
    vector <Vertex> tree;
    int getSuffixLink(int v) const {
        if (tree[v].link == -1) {
            if (v == 0 || tree[v].indParent == 0) {
                tree[v].link = 0;
            } else {
                tree[v].link = go(getSuffixLink(tree[v].indParent), tree[v].parentChar);
            }
        }
        return tree[v].link;
    }
    int go(int v, char c) const {
        if (tree[v].go[c] == -1) {
            if (tree[v].indNext[c] != -1) {
                tree[v].go[c] = tree[v].indNext[c];
            } else if (v == 0) {
                tree[v].go[c] = 0;
            } else {
                tree[v].go[c] = go(getSuffixLink(v), c);
            }
        }
        return tree[v].go[c];
    }
    int getUp(int v) const {
        if (tree[v].up == -1) {
            if (v == 0 || tree[v].indParent == 0) {
                tree[v].up = 0;
            } else if (tree[getSuffixLink(v)].leaf) {
                tree[v].up = getSuffixLink(v);
            } else if (getSuffixLink(v) == 0) {
                tree[v].up = 0;
            } else {
                tree[v].up = getUp(getSuffixLink(v));
            }
        }
        return tree[v].up;
    }
    bool isContainPatternAsSuffix(int v) const {
        return tree[v].leaf || getUp(v) != 0;
    }
    void dfs(int s, bool& isCycled) const {
        static char* color = new char[tree.size()];
        static bool flag = true;
        if (flag) {
            for (size_t i = 0; i < tree.size(); ++i) {
                color[i] = 0;
            }
            flag = false;
        }
        color[s] = 1;
        for (char i = 0; i < MAX_SYMBOLS; ++i) {
            int v = go(s, i);
            if (!isContainPatternAsSuffix(v)) {
                if (color[v] == 0) {
                    dfs(v, isCycled);
                } else if (color[v] == 1) {
                    isCycled = true;
                }
            }
        }
        color[s] = 2;
    }
    int getCountLeaf(int v) const {
        if (!tree[v].isCertainLeaf) {
            tree[v].isCertainLeaf = true;
            if (v == 0) {
                tree[v].countLeaf = 0;
            } else {
                tree[v].countLeaf += getCountLeaf(getSuffixLink(v));
            }
        }
        return tree[v].countLeaf;
    }
    long long jointDfs(const vector<pair<int, char> >* v1, int ind, int curV) const {
        long long sum = getCountLeaf(curV);
        for (auto x : v1[ind]) {
            sum += jointDfs(v1, x.first, go(curV, x.second));
        }
        return sum;
    }
public:
    Bor() {
        tree.clear();
        tree.push_back(Vertex());
    }
    void addString(const string& s) {
        int curVertex = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i] - MIN_SYMBOL;
            if (tree[curVertex].indNext[c] == -1) {
                tree[curVertex].indNext[c] = static_cast<int>(tree.size());
                tree.push_back(Vertex(curVertex, c));
            }
            curVertex = tree[curVertex].indNext[c];
        }
        tree[curVertex].leaf = true;
        tree[curVertex].countLeaf += 1;
    }
    bool isContainPattern(const string& text) const {
        int curV = 0;
        for (size_t i = 0; i < text.size(); ++i) {
            char c = text[i] - MIN_SYMBOL;
            curV = go(curV, c);
            if (isContainPatternAsSuffix(curV)) {
                return true;
            }
        }
        return false;
    }
    bool isGraphWithoutPatternCycled() const {
        bool isCycled = false;
        dfs(0, isCycled);
        return isCycled;
    }
    long long getNumbersOccur(vector<pair<int, char> >* vector1) const {
        return jointDfs(vector1, 1, 0);
    }
};

const int N = 100000;
vector <pair <int, char> > G[N];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n;
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        int k;
        cin >> k;
        for (int j = 0; j < k; ++j) {
            int x;
            char c;
            cin >> x >> c;
            G[i].push_back(make_pair(x, c - MIN_SYMBOL));
        }
    }

    int m;
    cin >> m;
    Bor bor;
    for (size_t i = 0; i < m; ++i) {
        string s;
        cin >> s;
        bor.addString(s);
    }

    cout << bor.getNumbersOccur(G) << '\n';

    return 0;
}