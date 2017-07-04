//https://github.com/Ilya1510/Deque
#include <iostream>
#include <algorithm>
#include "deque.h"
#include <deque>
#include <vector>
#include "gtest/gtest.h"
#include <string>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;

//методы back(), back() const, front(), front() const,
//Оба вида конструкторов : обычный, конструктор копирования.
//operator[], operator[] const,
//методы bool empty() const, size() const
TEST(DEQUE_TEST_MAIN_WORK, testInts) {
    Deque <int> a;
    for (int i = 0; i < 100; i++) {
        a.push_back(i);
        a.push_front(i);
    }
    ASSERT_EQ(a.size(), 200);
    for (int i = 0; i < 80; i++) {
        ASSERT_EQ(a.front(), a.back());
        a.pop_back();
        a.pop_front();
    }
    Deque <int>::iterator it = a.begin();
    Deque <int>::reverse_iterator rit = a.rbegin();
    ASSERT_EQ(it[10], rit[10]);
    ASSERT_EQ(it[36], rit[36]);
    int ind = 0;
    while (it != a.end()) {
        ASSERT_EQ(*it, a[ind++]);
        ASSERT_EQ(*it++, *rit++);
    }
    Deque <int> b(a);
    Deque <int> c;
    const Deque <int> d = a;
    const Deque <int>::const_iterator itc = d.cbegin();
    ASSERT_TRUE(a[10] == b[10]);
    ASSERT_TRUE(a[20] == 0);
    ASSERT_TRUE(a.end() - a.begin() == a.size());
    ASSERT_TRUE(c.begin() - c.end() == c.size());
    ASSERT_TRUE(b.end() - b.begin() == a.end() - a.begin());
    ASSERT_EQ(d.size(), b.size());
}

//методы begin(), begin() const, cbegin() const, end(), end() const, cend() const, 
//rbegin(), rbegin() const, crbegin() const, rend(), rend() const, crend() const,
TEST(DEQUE_TEST_ITERATORS, test1) {
    Deque <pair<string, int> > d;
    vector<pair<string, int> > v;
    unsigned int TEST_SIZE = 1000;
    for (unsigned int i = 0; i < TEST_SIZE; ++i) {
        string s = "123" + (i % 100 + 10);
        d.push_back(make_pair(s, i * i));
        v.push_back(make_pair(s, i * i));
    }

    Deque<pair<string, int> >::const_iterator cbd = d.cbegin();
    vector<pair<string, int> >::const_iterator cbv = v.cbegin();
    for (int i = 0; i < 1000; ++i) {
        cbd++;
    }
    while (cbd != d.cend()) {
        ASSERT_EQ(*cbd, *cbv);
        ASSERT_TRUE(cbd->second == cbv->second);
        cbd++;
        ++cbv;
    }
}

TEST(DEQUE_TEST_ITERATORS, test2) {
    Deque<int> d;
    for (int i = 0; i < 10; ++i) {
        d.push_back(i);
    }
    std::deque<int> sd(d.begin(), d.end());
    for (Deque<int>::iterator it = d.begin(); it < d.end(); it++) {
        ASSERT_EQ(*it, it - d.begin());
    }
    for (Deque<int>::const_iterator it = d.cbegin(); it < d.cend(); ++it) {
        ASSERT_EQ(*it, it - d.cbegin());
    }
    int j = 9;
    for (Deque<int>::const_reverse_iterator it = d.crbegin(); it < d.crend(); ++it) {
        ASSERT_EQ(*it, j--);
    }
    j = 9;
    for (Deque<int>::reverse_iterator it = d.rbegin(); it < d.rend(); it++) {
        ASSERT_EQ(*it, j--);
        (*it)--;
        ASSERT_EQ(*it, j);
    }
}

TEST(DEQUE_IN_DEQUE_TEST, test) {
    Deque<Deque<int> > d;
    unsigned int MAX_SIZE = 20000;
    for (unsigned int i = 0; i < MAX_SIZE; ++i) {
        Deque <int> c;
        c.push_front(i);
        d.push_back(c);
    }
    ASSERT_EQ(d.size(), MAX_SIZE);
    ASSERT_EQ(d[100].size(), 1);
    int i = 0;
    for (Deque<Deque<int> >::iterator cbd = d.begin(); cbd != d.end(); cbd++, ++i) {
        ASSERT_EQ(cbd->front(), i);
    }
    
}

TEST(DEQUE_MAX_TEST, test) {
    Deque<int> d;
    unsigned int MAX_SIZE = 1000000;
    for (unsigned int i = 0; i < MAX_SIZE; ++i) {
        d.push_back(i);
        d.push_front(i);
    }
    for (unsigned int i = 0; i < MAX_SIZE; ++i) {
        ASSERT_EQ(d.front(), d.back());
        ASSERT_EQ(d.front(), MAX_SIZE - i - 1);
        d.pop_back();
        d.pop_front();
    }
    ASSERT_EQ(d.size(), 0);
}

#include <time.h>
TEST(DEQUE_TEST_COMPLEXITY, test1) {
    Deque <int> c;
    typedef unsigned int uint;
    typedef long long ll;
    const ll MAX_SIZE = 1000000000;
    const ll BIN_COUNT = 10000000;
    const ll INF = 1000000000000;
    ll time = clock(), min_time = INF, max_time = -INF;
    c.push_back(0);
    for (uint i = 1; i < MAX_SIZE; ++i) {
        if (c.size() % BIN_COUNT == 0) {
            ll cur_time = (clock() - time);
            std::cout << cur_time << std::endl;
            time = clock();
            min_time = std::min(min_time, cur_time);
            max_time = std::max(max_time, cur_time);
        }
        c.push_back(i);
    }
    ASSERT_TRUE(static_cast<double>(max_time) / min_time < 3);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
