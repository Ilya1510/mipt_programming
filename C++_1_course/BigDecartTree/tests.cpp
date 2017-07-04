//
// Created by ilya on 28.03.17.
//

#include <vector>
#include "Tree.h"
#include <assert.h>
#include <algorithm>

using namespace std;

void testAddAndSet() {
    CartesianTree<int> a;
    const int N = 100;
    for (int i = 0; i < N; ++i) {
        a.insert(i, i);
    }
    for (int i = 0; i < N; ++i) {
        for (int j = i; j < N; ++j) {
            int res = 0;
            for (int z = i; z <= j; z++) {
                res += z;
            }
            assert(a.getSum(i, j) == res);
        }
    }
    for (int i = 0; i < N; ++i) {
        a.setValue(-i, i, i);
    }
    for (int i = 0; i < N; ++i) {
        for (int j = i; j < N; ++j) {
            int res = 0;
            for (int z = i; z <= j; z++) {
                res -= z;
            }
            assert(a.getSum(i, j) == res);
        }
    }
    a.setValue(0, 0, N - 2);
    assert(a.getSum(0, N - 1) == 1 - N);
}

void minTest() {
    int a[] = {1, 2, 3, 4, 5, 6};
    CartesianTree<int> da(a, 6);
    da.setValue(2, 2, 4);
    da.remove(3);
    int b[] = {1, 2, 2, 2, 6};
    CartesianTree<int> db(b, 5);
    for (int i = 0; i < 5; ++i) {
        for (int j = i; j < 5; ++j) {
            assert(da.getSum(i, j) == db.getSum(i, j));
        }
    }
    da.insert(7, 4);
    db.insert(7, 4);
    for (int i = 0; i < 6; ++i) {
        for (int j = i; j < 6; ++j) {
            assert(da.getSum(i, j) == db.getSum(i, j));
        }
    }
}

void complexMinTest() {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    CartesianTree<int> ta(a, 10);
    assert(ta.getSum(2, 5) == 18);
    ta.setValue(3, 2, 4);
    ta.doNextPermutation(2, 5);
    assert(ta.getSum(3, 4) == 9);
    ta.addValue(-2, 1, 6);
    ta.doPreviousPermutation(4, 6);
    assert(ta.getSum(5, 7) == 17);
    ta.doNextPermutation(4, 7);
    ta.doNextPermutation(4, 7);
    ta.doPreviousPermutation(5, 8);
    assert(ta.getSum(6, 7) == 17);
    assert(ta.getSum(5, 8) == 26);
    int b[] = {1, 0, 1, 1, 1, 5, 9, 8, 4, 10};
    for (int i = 0; i < 10; ++i) {
        assert(ta.getSum(i, i) == b[i]);
    }
    ta.insert(2, 3);
    ta.doNextPermutation(1, 3);
    ta.doNextPermutation(1, 2);
    assert(ta.getSum(0, 1) == 3);
    assert(ta.getSum(2, 3) == 1);
    ta.remove(2);
    ta.doPreviousPermutation(0, 2);
    assert(ta.getSum(0, 1) == 2);
    assert(ta.getSum(2, 3) == 3);
}

void randomTest(int n, int m) {
    srand(static_cast<unsigned int>(time(0)));
    vector <long long> a;
    for (int i = 0; i < n; ++i) {
        a.push_back(rand());
    }
    CartesianTree <long long> t;
    for (int i = 0; i < n; ++i) {
        t.insert(a[i], i);
    }
    for (int i = 0; i < m; ++i) {
        int c = rand() % 7;
        if (c == 0) {
            if (a.size() > 8) continue;
            int ind = rand() % static_cast<int>(a.size() + 1);
            assert(ind >= 0 && ind <= a.size());
            long long val = rand();
            t.insert(val, ind);
            a.insert(a.begin() + ind, val);
        } else if (c == 1) {
            if (a.size() == 0) continue;
            int ind = rand() % static_cast<int>(a.size());
            assert(ind >= 0 && ind < a.size());
            t.remove(ind);
            a.erase(a.begin() + ind, a.begin() + ind + 1);
        } else if (c == 2) {
            if (a.size() == 0) continue;
            int l = rand() % static_cast<int>(a.size());
            int r = rand() % static_cast<int>(a.size());
            if (l > r) std::swap(l, r);
            long long resTrue = 0;
            for (int j = l; j <= r; ++j) {
                resTrue += a[j];
            }
            long long resOur = t.getSum(l, r);
            assert(resOur == resTrue);
            std::cout <<resOur << std::endl;
        } else if (c == 3) {
            if (a.size() == 0) continue;
            long long v = rand();
            int l = rand() % static_cast<int>(a.size());
            int r = rand() % static_cast<int>(a.size());
            if (l > r) std::swap(l, r);
            for (int j = l; j <= r; ++j) {
                a[j] += v;
            }
            t.addValue(v, l, r);
        } else if (c == 4) {
            if (a.size() == 0) continue;
            long long v = rand();
            int l = rand() % static_cast<int>(a.size());
            int r = rand() % static_cast<int>(a.size());
            if (l > r) std::swap(l, r);
            for (int j = l; j <= r; ++j) {
                a[j] = v;
            }
            t.setValue(v, l, r);
        } else if (c == 5) {
            if (a.size() == 0) continue;
            int l = rand() % static_cast<int>(a.size());
            int r = rand() % static_cast<int>(a.size());
            if (l > r) std::swap(l, r);
            t.doNextPermutation(l, r);
            next_permutation(a.begin() + l, a.begin() + r + 1);
        } else if (c == 6) {
            if (a.size() == 0) continue;
            int l = rand() % static_cast<int>(a.size());
            int r = rand() % static_cast<int>(a.size());
            if (l > r) std::swap(l, r);
            t.doPreviousPermutation(l, r);
            prev_permutation(a.begin() + l, a.begin() + r + 1);
        }
    }

}

void runAllTests(unsigned int countOperation) {
    testAddAndSet();
    minTest();
    complexMinTest();
    randomTest(countOperation, countOperation);
}

