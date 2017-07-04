#include <iostream>
#include "Tree.h"
#include "tests.cpp"

const int N = 100000;
long long a[N];

int main() {
    //freopen("../input.txt", "r", stdin);
    //freopen("../output.txt", "w", stdout);
    runAllTests(100000);
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    CartesianTree<long long> tree(a, n);
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        char c;
        cin >> c;
        if (c == '2') { // 'i'
            int val, ind;
            cin >> val >> ind;
            tree.insert(val, ind);
        } else if (c == '3') { // 'r'
            int ind;
            cin >> ind;
            tree.remove(ind);
        } else if (c == '1') { // 's'
            int l, r;
            cin >> l >> r;
            std::cout << tree.getSum(l, r) << std::endl;
        } else if (c == '5') { // '+'
            int v, l, r;
            cin >> v >> l >> r;
            tree.addValue(v, l, r);
        } else if (c == '4') { //'a'
            int v, l, r;
            cin >> v >> l >> r;
            tree.setValue(v, l, r);
        } else if (c == '6') { // 'n'
            int l, r;
            cin >> l >> r;
            tree.doNextPermutation(l, r);
        } else if (c == '7') { // 'p'
            int l, r;
            cin >> l >> r;
            tree.doPreviousPermutation(l, r);
        }
    }
    for (int i = 0; i < tree.getSize(); ++i) {
        cout << tree.getSum(i, i) << " ";
    }
    cout << endl;

    return 0;
}