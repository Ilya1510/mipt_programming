#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <class T>
vector<int> zFunction(const T& s) {
    size_t n = s.size();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    z[0] = static_cast<int>(n);
    return z;
}

vector <int> prefixFunction(const string& s) {
    size_t n = s.size();
    vector<int> pi(n, 0);
    for (size_t i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            j++;
        }
        pi[i] = j;
    }
    return pi;
}

size_t getMaxRetroPrefix(const string& s) {
    size_t n = s.size();
    vector<int> pi(n, 0);
    vector<int> d(n, 0);
    int maxNumberReccurence = 0;
    size_t indMax = 0;
    for (size_t i = 1; i < n; ++i) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            d[i] = d[j] + 1;
            j++;
        } else {
            d[i] = 0;
        }
        pi[i] = j;
        if (maxNumberReccurence < d[i]) {
            maxNumberReccurence = d[i];
            indMax = i;
        }
    }
    return indMax;
}

vector <int> findOccur(const string& s, const string& p) {
    string text = p + '#' + s;
    vector<int> z = zFunction(text);
    vector<int> occur;
    int pLen = static_cast<int>(p.size());
    for (int i = pLen; i < text.size(); ++i) {
        if (z[i] == p.size()) {
            occur.push_back(i - pLen - 1);
        }
    }
    return occur;
}

vector<int> convertZToPrefix(const vector<int>& z) {
    vector<int> pi(z.size(), 0);
    int maxRight = 0;
    for (int l = 1; l < z.size(); ++l) {
        if (l + z[l] - 1 > maxRight) {
            for (int j = maxRight + 1; j < l + z[l]; ++j) {
                pi[j] = j - l + 1;
            }
            maxRight = l + z[l] - 1;
        }
    }
    return pi;
}

vector<int> getStringFromPrefix(const vector<int>& pi) {
    vector<int> result(pi.size(), 0);
    for (int i = 1; i < pi.size(); ++i) {
        if (!pi[i]) {
            result[i] = i;
        } else {
            result[i] = result[pi[i] - 1];
        }
    }
    return result;
}

vector <int> convertPrefToZ(const vector<int>& pi) {
    vector<int> string = getStringFromPrefix(pi);
    return zFunction(string);
}

const string getZipString(const string& s) {
    vector <int> pi = prefixFunction(s);
    size_t n = s.size();
    return s.substr(0, n - pi[n - 1]);
}

bool isContainContinious(const string& t, string p) {
    while (2 * t.size() > p.size()) {
        p = p + p;
    }
    return findOccur(p, t).size() > 0;
}

int getMaxPrefixSameSuffix(const string& s) {
    return prefixFunction(s).back();
}

const string getGoodString(const string& s, const string& t) {
    if (isContainContinious(t, getZipString(s))) {
        return getZipString(s);
    } else if (isContainContinious(s, getZipString(t))) {
        return getZipString(t);
    }
    int d1 = getMaxPrefixSameSuffix(t + '#' + s);
    int d2 = getMaxPrefixSameSuffix(s + '#' + t);
    string res1 = s + t.substr(d1, t.size() - d2);
    string res2 = t + s.substr(d2, s.size() - d1);
    if (res1.size() < res2.size()) {
        return res1;
    } else {
        return res2;
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string s, t;
    cin >> s >> t;
    cout << getGoodString(s, t) << endl;

    return 0;
}