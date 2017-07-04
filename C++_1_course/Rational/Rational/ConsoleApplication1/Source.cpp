#include "rational.h"

#include <random>
#include <assert.h>
using std::cout;
void test(int n) {
    for (int i = 0; i < n; i++) {
        int a = rand() % 1000 - 500;
        int b = rand() % 1000 + 1;
        int c = rand() % 1000 - 500;
        int d = rand() % 1000 + 1;
        Rational x = BigInteger(a);
        x /= b;
        Rational y = c;
        y /= b;
        //cout << x.toString() << " " << y.toString() << '\n';
        assert(abs(double(x + y) - double(x) - double(y)) < 0.01);
        assert(abs(double(x - y) - double(x) + double(y)) < 0.01);
        assert(abs(double(x * y) - double(x) * double(y)) < 0.01);
        assert(abs(double(x / y) - double(x) / double(y)) < 0.01);
        assert(x < y == double(x) < double(y));
        assert(x <= y == double(x) <= double(y));
        assert(x > y == double(x) > double(y));
        assert(x >= y == double(x) >= double(y));
        assert((x == y) == (double(x) == double(y)));
        assert((x != y) == (double(x) != double(y)));
        assert(double(-x) == -double(x));
        assert(x.getDenom() > 0 && y.getDenom() > 0);
        if (x.getNum() >= 0) {
            assert(gcd(x.getDenom(), x.getNum()) == 1);
        }
        if (y.getNum() >= 0) {
            assert(gcd(y.getDenom(), y.getNum()) == 1);
        }
        cout << x.asDecimal(1) << '\n';
    }
}

int main() {
    for (BigInteger i = 99; i < 1000; ++i) {
        cout << i;
    }
    for (;;);
    //test(1000);
}
