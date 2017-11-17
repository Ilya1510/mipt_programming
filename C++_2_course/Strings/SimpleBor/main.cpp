#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Bor {
    static const char MIN_SYMBOL = '0';
    static const int MAX_SYMBOLS = 10;
    struct Node {
        int indNext[MAX_SYMBOLS];
        int countLeaf;
        int curLeaf;
        Node() : countLeaf(0), curLeaf(0) {
            fill(indNext, indNext + MAX_SYMBOLS, -1);
        }
    };
    vector<Node> tree;
    void addSymbol(int v, char c) {
        tree[v].indNext[c] = static_cast<int>(tree.size());
        tree.push_back(Node());
    }
    //! k >= 1
    const string getKthString(int v, int k) const {
        int count = tree[v].curLeaf;
        size_t i;
        for (i = 0; count < k && i < MAX_SYMBOLS; ++i) {
            if (tree[v].indNext[i] != -1) {
                count += tree[tree[v].indNext[i]].countLeaf;
            }
        }
        if (i == 0) {
            return "";
        }
        int next = tree[v].indNext[i - 1];
        return char(i - 1 + MIN_SYMBOL) +
                getKthString(next, k - count + tree[next].countLeaf);
    }
public:
    Bor() : tree(1, Node())
    {}
    template <class T>
    void addString(const T& s) {
        int curV = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i] - MIN_SYMBOL;
            if (tree[curV].indNext[c] == -1) {
                addSymbol(curV, c);
            }
            tree[curV].countLeaf += 1;
            curV = tree[curV].indNext[c];
        }
        tree[curV].countLeaf += 1;
        tree[curV].curLeaf += 1;
    }
    bool isContainString(const string& s) const {
        int v = 0;
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i] - MIN_SYMBOL;
            if (tree[v].indNext[c] == -1) {
                return false;
            }
            v = tree[v].indNext[c];
        }
        return tree[v].curLeaf > 0;
    }
    const string getKthString(int k) const {
        return getKthString(0, k);
    }
};

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

const string toNumbers(const string& s) {
    string res;
    char toNum[] = {2, 2, 2, 3, 3, 3, 4, 4, 1, 1, 5, 5,
                   6, 6, 0, 7, 0, 7, 7, 8, 8, 8, 9, 9, 9, 0};
    for (auto x : s) {
        res.push_back(toNum[x - 'a'] + '0');
    }
    return res;
}

const int N = 1000;
int dp[N];
int pred[N];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string tmp;
    while (getline(cin, tmp)) {
        if (tmp == "-1") {
            break;
        }
        map<string, string> numberToWord;
        Bor bor;
        string number = tmp;
        getline(cin, tmp);
        int k = atoi(tmp.c_str());
        for (int i = 0; i < k; ++i) {
            getline(cin, tmp);
            numberToWord[toNumbers(tmp)] = tmp;
            bor.addString(toNumbers(tmp));
        }
        fill(dp, dp + number.size() + 1, -1);
        fill(pred, pred + number.size() + 1, -1);
        dp[0] = 0;
        for (size_t i = 1; i <= number.size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (dp[j] != -1 && bor.isContainString(number.substr(j, i - j))) {
                    if (dp[i] == -1 || dp[i] > dp[j] + 1) {
                        dp[i] = dp[j] + 1;
                        pred[i] = static_cast<int>(j);
                    }
                }
            }
        }
        if (dp[number.size()] == -1) {
            cout << "No solution.\n";
        } else {
            vector <string> ans;
            size_t curState = number.size();
            while (curState) {
                size_t predState = static_cast<size_t>(pred[curState]);
                ans.push_back(number.substr(predState, curState - predState));
                curState = predState;
            }
            reverse(ans.begin(), ans.end());
            for (auto x : ans) {
                cout << numberToWord[x] << " ";
            }
            cout << '\n';
        }
    }
    return 0;
}