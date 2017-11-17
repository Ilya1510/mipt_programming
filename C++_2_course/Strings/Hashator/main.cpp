#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Hashator {
    int p;
    vector<long long> pPow;
    vector<long long> prefHash;
    void calculatePow(size_t n) {
        pPow.push_back(1);
        for (size_t i = 0; i < n; ++i) {
            pPow.push_back(pPow.back() * p);
        }
    }
    void calculatePrefHashes(const string& s) {
        prefHash.push_back(0);
        for (auto& c : s) {
            prefHash.push_back(prefHash.back() * p + c);
        }
    }
public:
    Hashator(const string& s, int p) : p(p) {
        calculatePow(s.size());
        calculatePrefHashes(s);
    }
    // j - не включая
    long long getSubstringHash(size_t i, size_t j) {
        return prefHash[j] - prefHash[i] * pPow[j - i];
    }
};

map<long long, size_t> rightBorder;
set<long long> answer;

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string s;
    cin >> s;
    size_t n = s.size();
    Hashator hashator(s, 37);
    size_t ans = 0;
    for (size_t l = 1; l <= n / 2; ++l) {
        rightBorder.clear();
        ans += answer.size();
        answer.clear();
        for (size_t i = 0; i + l <= n; ++i) {
            size_t j = i + l;
            long long hash = hashator.getSubstringHash(i, j);
            if (rightBorder.count(hash) == 0) {
                rightBorder[hash] = j;
            } else {
                size_t predRight = rightBorder[hash];
                if (predRight <= i) {
                    answer.insert(hash);
                    rightBorder[hash] = n + 1;
                } else if (predRight != n + 1) {
                    rightBorder[hash] = min(predRight, j);
                }
            }
        }
    }
    cout << ans + answer.size() << '\n';
    return 0;
}