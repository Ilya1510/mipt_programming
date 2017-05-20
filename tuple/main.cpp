#include <iostream>
#include <tuple>
#include <functional>
#include "tuple.h"

//! beginTests
struct A {
    int x;
    A() : x(8) {}
};
#include <assert.h>

using namespace std;

void testConstructors() {
    Tuple <int, int, std::string> t1;
    A a;
    const int x = 9;
    Tuple<int, int, A> tp1(x, 7, a);
    Tuple<int, int, A> tp2(9, x, a);
    Tuple<int, int, A> tp(7, 7, std::move(a));
    Tuple<int, int, int, int, int, int> tpb(x, 5, x, 5, x, x);
    Tuple<int, int, int, int, int> tpf(4, 4, 4, 4, 4);
    Tuple<int, int, A> tp3(tp2);
    Tuple<int, int, A> tp4(std::move(tp3));
    Tuple<int, const int, int*> b(10, 100, new int(66));
    Tuple<int, const int, int*> c(std::move(b));
    return;
}

void testTypeAt() {
    Tuple <int, const int, A> t1;

    static_assert(std::is_same<typename Tuple<int, const int, A> ::template TypeAt<0>, int>::value, "TypeAt");
    static_assert(std::is_same<typename Tuple<int, const int, A> ::template TypeAt<1>, const int>::value, "TypeAt");
    static_assert(std::is_same<typename Tuple<int, const int, A> ::template TypeAt<2>, A>::value, "TypeAt");
    static_assert(std::is_same<typename Tuple<int*> ::template TypeAt<0>, int*>::value, "TypeAt");

    Tuple <int, int> t2(2, 5);
    assert(get<0>(t2) == 2);
    assert(get<1>(t2) == 5);
    get<0>(t2) = 1000;
    assert(get<0>(t2) == 1000);
    get<1>(t2) = -3;
    assert(get<1>(t2) == -3);
    const Tuple <int, char, A> t3;
    assert(get<0>(t3) == 0);
    //assert(get<2>(t3).x == 8);
    int c = get<1>(std::move(t2));
    assert(c == -3);
    int x = 10;
    const int y = 100;
    int q = 15;
    auto z = makeTuple(1, 4, x, y, std::move(q));
    assert(get<0>(z) == 1);
    assert(get<1>(z) == 4);
    assert(get<2>(z) == 10);
    assert(get<3>(z) == 100);
    assert(get<4>(z) == 15);
    Tuple <int, char> ic;
    get<0>(ic) = 10;
    get<1>(ic) = '6';
    assert(get<int>(ic) == 10);
    assert(get<char>(ic) == '6');
    get<char>(ic) = '9';
    get<int>(ic) = 100;
    assert(get<int>(ic) == 100);
    assert(get<char>(ic) == '9');
}

void testTupleCat() {
    Tuple<int, char> x(7, 's');
    Tuple<char, int> y('q', 10);
    auto z = tupleCat(x, y);
    assert(get<0>(z) == 7);
    assert(get<1>(z) == 's');
    assert(get<2>(z) == 'q');
    assert(get<3>(z) == 10);
    auto w = tupleCat(z, z);
    assert(get<6>(w) == 'q');
    auto qq = tupleCat(w, z, w, x);
    assert(get<3>(qq) == 10);
    assert(get<5>(qq) == 's');
    assert(get<12>(qq) == 7);
    assert(get<21>(qq) == 's');
    get<12>(qq) = 9;
    assert(get<12>(qq) = 9);

    Tuple<> a;
    Tuple<> b;
    Tuple<> c = tupleCat(a, b);

    Tuple<int> ma(4);
    Tuple<int> mb(5);
    Tuple<int> mc(6);
    Tuple<char> md('3');
    Tuple<int, int, int, char> tmc = tupleCat(std::move(ma), std::move(mb), std::move(mc), std::move(md));
}

void testTupleComp() {
    Tuple<int, int> a(3, 4);
    Tuple<int, int> b(3, 5);
    assert(a < b);
    assert(a != b);
    get<1>(b) = 4;
    assert(a == b);
    assert(a <= b);
    assert(a >= b);
    get<0>(a) = 1;
    assert(a < b);
    assert(b > a);
}

#include <vector>
#include <algorithm>

template <class T>
class Wonderful {
public:
    Wonderful() = delete;
};

void test_tuple() {

    {
        Tuple<int, std::string, long> tuple;
    }

    {
        int test_int = 5;
        std::string test_string = "Hello";
        Tuple<int, std::string> tuple(test_int, test_string);
    }

    {
        int test_int = 5;
        std::string test_string = "Hello";
        std::vector<int> test_vector(10, 5);
        Tuple<int, std::string, std::vector<int>> tuple(test_int, test_string, std::move(test_vector));
    }

    {
        std::string test_string = "Hello";
        std::vector<int> test_vector(10, 5);
        Tuple<std::string, std::vector<int>> tuple(std::move(test_string), std::move(test_vector));
    }

    {
        Tuple<int> first(5);
        auto second = first;
        auto third = std::move(first);

        first.swap(second);
        assert(third == second);
    }

    {
        Tuple<int> first(12);
        auto second = first;
        assert(get<0>(first) == get<0>(second));

        second = Tuple<int>(14);
        assert(get<0>(second) == 14);

        first.swap(second);
        assert(get<0>(second) == 12);
        assert(get<0>(first) == 14);

        int&& test_int = get<0>(std::move(first));
        assert(test_int == 14);
    }

    {
        Tuple<int, long> tuple(12, 16);
        assert(get<int>(tuple) == 12);
        assert(get<long>(tuple) == 16);

        int test_int_lv = get<int>(tuple);
        int&& test_int_rv = get<int>(std::move(tuple));
        assert(test_int_lv == test_int_rv);
    }

    {
        Tuple<int, std::string, std::vector<int>> tuple = makeTuple(5, std::string("test"), std::vector<int>(2, 5));
        get<2>(tuple)[1] = 2;
        assert(get<2>(tuple)[1] == 2);
    }

    {
        int test_int = 1;
        auto tuple = makeTuple(test_int, std::ref(test_int));
        test_int = 2;
        assert(get<0>(tuple) == 1);
        assert(get<1>(tuple) == 2);
    }

    {
        std::vector<std::tuple<int, std::string, float>> v;
        v.emplace_back(2, "baz", -0.1);
        v.emplace_back(2, "bar", 3.14);
        v.emplace_back(1, "foo", 100.1);
        std::sort(v.begin(), v.end());

        assert(get<0>(v[0]) == 1);
        assert(get<0>(v[1]) == 2);
        assert(get<0>(v[2]) == 2);

        assert(get<1>(v[0]) == std::string("foo"));
        assert(get<1>(v[1]) == std::string("bar"));
        assert(get<1>(v[2]) == std::string("baz"));
    }

    {
        auto tuple = makeTuple(1, "hello");
        auto test_tuple = tupleCat(tuple, tuple, makeTuple(5, 10));

        assert(get<0>(test_tuple) == get<2>(test_tuple));
        assert(get<4>(test_tuple) == 5);
    }


    for (int i = 0; i < 10000; ++i) {
        Tuple<int, std::vector<int>> tuple(4, std::vector<int>(10000, 5));
        assert(get<int>(tuple) == 4);
    }

}

int main() {

    auto tuple = makeTuple(1, "hello");
    auto test_tuple1 = tupleCat(tuple, tuple, makeTuple(5, 10));
    //Wonderful<decltype(test_tuple1)>();

    assert(get<0>(test_tuple1) == get<2>(test_tuple1));
    assert(get<4>(test_tuple1) == 5);
    /*Tuple<> b;
    tuple_cat();
    testConstructors();
    testTypeAt();
    testTupleCat();
    testTupleComp();*/
    test_tuple();
    std::cout << "Hello, World!" << std::endl;
    return 0;
}