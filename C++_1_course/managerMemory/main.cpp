#include <iostream>
#include <set>

using namespace std;

const int N = 100001;

int query[N][3];
set <pair <int, int> > s;
set <pair <int, int> > otr;

void get(int i, int x) {
    if (s.size() == 0) {
        std::cout << -1 << std::endl;
        return;
    }
    auto it = s.end();
    it--;
    std::pair<int, int> p = *it;
    if (it->first >= x) {
        int l = -it->second;
        int r = -it->second + it->first - 1;
        int len = it->first;
        query[i][0] = x;
        query[i][1] = l;
        query[i][2] = l + x - 1;
        s.erase(it);
        otr.erase({l, r});
        if (len > x) {
            s.insert({len - x, -(l + x)});
            otr.insert({l + x, r});
        }
        std::cout << l << endl;
    } else {
        std::cout << -1 << endl;
    }
}

void del(int i) {
    if (query[i][0] == 0) {
        return;
    }
    int len = query[i][0];
    int l = query[i][1];
    int r = query[i][2];
    int resR = r;
    int resL = l;
    otr.insert({l, r});
    auto it = otr.find({l, r});
    it++;
    if (it != otr.end()) {
        if (it->first == r + 1) {
            s.erase({it->second - it->first + 1, -it->first});
            resR = it->second;
            otr.erase(it);
        }
    }
    it = otr.find({l, r});
    it--;
    if (it != otr.end()) {
        if (it->second == l - 1) {
            s.erase({it->second - it->first + 1, -it->first});
            resL = it->first;
            otr.erase(it);
        }
    }
    otr.erase({l, r});
    otr.insert({resL, resR});
    s.insert({resR - resL + 1, -resL});
}

int main() {
    int n, m;
    std::cin >> n >> m;
    s.insert({n, -1});
    for (int i = 1; i <= m; ++i) {
        int x;
        std::cin >> x;
        if (x > 0) {
            get(i, x);
        } else {
            del(-x);
        }
    }
    return 0;
}