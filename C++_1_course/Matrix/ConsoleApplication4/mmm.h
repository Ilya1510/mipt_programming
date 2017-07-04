#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::swap;

template<typename T>
const T& max__(const T& a, const T& b) {
    return a < b ? b : a;
}

template<typename T>
const T& min__(const T& a, const T& b) {
    return a > b ? b : a;
}

template<typename T>
const T abs__(const T& a) {
    return a < 0 ? -a : a;
}

template<class it>
void reverse__(it l, it r) {
   --r;
    while (l < r) {
        swap(*l, *r);
        ++l;
        --r;
    }
    return;
}

class BigInteger {
public:
    static const long long BASE = 100000000;
    static const long long BASE_LEN = 8;
    static const int BASE_LOG = 27;
    
    BigInteger() {
        num.clear();
        sign = 1;
    }

    BigInteger(long long x) {
        num.clear();
        sign = x < 0 ? -1 : 1;
        long long curx = abs__(x);
        while (curx) {
            num.push_back(curx % BASE);
            curx /= BASE;
        }
    }

    BigInteger(string st) {
        num.clear();
        sign = 1;
        if (st.empty())
            return;
        sign = st[0] == '-' ? -1 : 1;
        long long carry = 0;
        for (int i = int(st.size()) - 1; i >= (sign == -1); i -= BASE_LEN) {
            carry = 0;
            for (int j = max__((long long)(sign == -1), i - BASE_LEN + 1); j <= i; ++j) {
                carry *= 10;
                carry += st[j] - '0';
            }
            num.push_back(carry);
        }
        for (int i = int(num.size()) - 1; i >= 0 && num[i] == 0; --i)
            num.pop_back();
        if (num.empty())
            sign = 1;
    }


private:
    BigInteger& positiveAdd(const BigInteger& other) {
        long long carry = 0;
        for (size_t i = 0; i < max__(num.size(), other.num.size()) || carry; ++i) {
            if (num.size() <= i)
                num.push_back(0);
            num[i] += (i < other.num.size() ? other.num[i] : 0) + carry;
            carry = 0;
            if (num[i] >= BASE) {
                carry = 1;
                num[i] -= BASE;
            }
        }
        return *this;
    }

    BigInteger& positiveSub(const BigInteger& other) {
        long long carry = 0;
        for (size_t i = 0; i < num.size() || carry; ++i) {
            num[i] -= (i < other.num.size() ? other.num[i] : 0) + carry;
            carry = 0;
            if (num[i] < 0) {
                carry = 1;
                num[i] += BASE;
            }
        }
        for (int i = int(num.size()) - 1; i >= 0 && num[i] == 0; --i) 
            num.pop_back();
        return *this;
    }

    BigInteger& revPositiveSub(const BigInteger& other) {
        long long carry = 0;
        for (size_t i = 0; i < other.num.size() || carry; ++i) {
            if (i <= num.size())
                num.push_back(0);
            num[i] = other.num[i] - num[i] - carry;
            carry = 0;
            if (num[i] < 0) {
                carry = 1;
                num[i] += BASE;
            }
        }
        for (int i = int(num.size()) - 1; i >= 0 && num[i] == 0; --i) 
            num.pop_back();
        return *this;
    }


    BigInteger& multByLong(long long x) {
        long long carry = 0;
        for (size_t i = 0; i < num.size() || carry; ++i) {
            if (i >= num.size())
                num.push_back(0);
            num[i] = num[i] * x + carry;
            carry = 0;
            if (num[i] >= BASE) {
                carry = num[i] / BASE; 
                num[i] %= BASE;
            }
        }
        for (int i = (int)(num.size()) - 1; i >= 0 && num[i] == 0; --i)
            num.pop_back();
        return *this;
    }

public:

    bool isNeg() const {
        return sign == -1;
    }
    
    BigInteger& operator = (const BigInteger& x) {
        num = x.num;
        sign = x.sign;
        return *this;
    }

    string toString() const {
        string res = sign == -1 ? "-" : "";
        if (num.empty()) 
            res += "0";
        long long x;
        string tmp;
        for (int i = int(num.size()) - 1; i >= 0; --i) {
            x = num[i];
            tmp = "";
            while (tmp.size() < BASE_LEN) {
                tmp += char('0' + x % 10);
                x /= 10;
            }
            int flag = 0;
            for (int j = int(tmp.size()) - 1; j >= 0; --j) {
                if (i == int(num.size()) - 1 && tmp[j] == '0' && !flag)
                    continue;
                flag = 1;
                res += tmp[j];
            }
        }
        return res;
    }

    explicit operator bool() const {
        return num.size() != 0;
    }

    explicit operator double() const {
        double ans = 0;
        for (size_t i = 0; i < num.size(); i++) {
            ans *= BASE;
            ans += num[i];
        }
        ans *= sign;
        return ans;
    }

    int operator [] (int x) const {
        return x >= 0 && x < (int)num.size() ? num[x] : 0;
    }

    unsigned int size() const {
        return num.size();
    }

    int getSign() const {
        return sign;
    }

    void changeSign() {
        if (num.size())
            sign *= -1;
        else
            sign = 1;
    }

    bool operator<(const BigInteger& other) const {
        if (sign != other.sign)
            return sign < other.sign;
        bool ans = 0;
        if (num.size() != other.num.size())
            ans = (num.size() < other.num.size()) ^ (sign == -1);
        else {
            for (int i = int(num.size()) - 1; i >= 0; --i) {
                if (num[i] != other.num[i]) {
                    ans = (num[i] < other.num[i]) ^ (sign == -1);
                    break;
                }
            }
        }
        return ans;
    }

    bool operator==(const BigInteger& other) const {
        return !(*this < other || other < *this);
    }

    bool operator!=(const BigInteger& other) const {
        return *this < other || other < *this;
    }

    bool operator>(const BigInteger& other) const {
        return other < *this;
    }

    bool operator<=(const BigInteger& other) const {
        return !(other < *this);
    }

    bool operator>=(const BigInteger& other) const {
        return !(*this < other);
    }

    const BigInteger operator-() const {
        BigInteger a = *this;
        if (a.num.size())
            a.changeSign();
        return a;
    }

    const BigInteger operator+(const BigInteger& other) const {
        BigInteger result = *this;
        return result += other;
    }

    BigInteger& operator+=(const BigInteger& other) {
        if (sign == 1 && other.sign == 1) {
            positiveAdd(other);
        } else if (sign == -1 && other.sign == -1) {
            positiveAdd(other);
        } else if (sign == -1 && other.sign == 1) {
            if (-*this <= other)
                revPositiveSub(other), changeSign();
            else 
                positiveSub(other);
        } else {
            if (*this >= -other)
                positiveSub(other);
            else
                revPositiveSub(other), changeSign();
        }
        return *this;
    }

    BigInteger& operator-=(const BigInteger& other) {
        return *this += -other;
    }

    const BigInteger operator-(const BigInteger& other) const {
        BigInteger result = *this;
        return result -= other;
    }


    BigInteger& operator++() {
        return *this += 1;
    }

    BigInteger& operator--() {
        return *this -= 1;
    }

    const BigInteger operator++(int) {
        return (*this += 1) - 1;
    }

    const BigInteger operator--(int) {
        return (*this -= 1) + 1;
    }
    
    const BigInteger operator*(const BigInteger& other) const {
        if (num.empty() || other.num.empty())
            return 0;
        BigInteger ans = 0;
        BigInteger z = *this;
        BigInteger tmp;
        z.sign = 1;
        for (size_t i = 0; i < other.num.size(); ++i) {
            tmp = z;
            ans += tmp.multByLong(other.num[i]);
            z.multByLong(BASE);
        }
        ans.sign = sign * other.sign;
        return ans;
    }

    BigInteger& operator*=(const BigInteger& other) {
        return *this = *this * other;
    }

    const BigInteger operator/(const BigInteger& other) const {
        if (num.empty() || other.num.empty())
            return 0;
        BigInteger ans = 0;
        BigInteger z = 0;
        BigInteger abs__x = abs__(other);
        BigInteger x2[BASE_LOG + 1];
        x2[0] = abs__x;
        for (int i = 1; i <= BASE_LOG; ++i) {
            x2[i] = x2[i - 1];
            x2[i].multByLong(2);
        }
        int flag = -1;
        for (int i = int(num.size()) - 1; i >= 0; --i) {
            z.multByLong(BASE);
            z += num[i];
            if (flag == -1 && z >= abs__(other)) 
                flag = i;
            if (flag != -1) {
                ans.num.push_back(0);
                if (z >= abs__x) {
                    for (int j = BASE_LOG; j >= 0; --j) 
                        while (z >= x2[j]) {
                            ans.num[flag - i] += (1 << j);
                            z -= x2[j];
                        }
                }
            }
        }
        if (ans.num.size()) {
            reverse__(ans.num.begin(), ans.num.end());
            ans.sign = sign * other.sign;
        }
        return ans;
    }

    BigInteger& operator/=(const BigInteger& other) {
        return *this = *this / other;
    }

    const BigInteger operator%(const BigInteger& other) const {
        if (num.empty() || other.num.empty())
            return 0;
        BigInteger z = 0;
        BigInteger abs__x = abs__(other);
        BigInteger x2[BASE_LOG + 1];
        x2[0] = abs__x;
        for (int i = 1; i <= BASE_LOG; ++i) {
            x2[i] = x2[i - 1];
            x2[i].multByLong(2);
        }
        for (int i = int(num.size()) - 1; i >= 0; --i) {
            z.multByLong(BASE);
            z += num[i];
            if (z >= abs__x) {
                for (int j = BASE_LOG; j >= 0; --j) 
                    while (z >= x2[j]) 
                        z -= x2[j];
            }
        }
        if (z.num.size()) 
            z.sign = sign;
        return z;
    }

    BigInteger& operator %= (const BigInteger& x) {
        return *this = *this % x;
    }

private:
    vector<long long> num;
    int sign;
};

std::ostream& operator << (std::ostream& out, const BigInteger& x) {
    out << x.toString();
    return out;
}

std::istream& operator >> (std::istream& in, BigInteger& x) {
    string st;
    in >> st;
    x = st;
    return in;
}

const BigInteger gcd(const BigInteger& x, const BigInteger& y) {
    return y ? gcd(y, x % y) : x;
}


class Rational {
private:
    BigInteger num, den;

    BigInteger gcd(BigInteger a, BigInteger b) const {
        if (a.isNeg())
            a = -a;
        if (b.isNeg())
            b = -b;
        while (a > 0 && b > 0) 
            if (a > b)
                a %= b;
            else
                b %= a;
        return a + b;
    }

    void normalise() {
        if ((num.isNeg() && den.isNeg()) || (!num.isNeg() && den.isNeg()))
            num = -num, den = -den;
        if (num == 0)
            den = 1;
        BigInteger g = gcd(num, den);
        num /= g, den /= g;
    }

public:
    Rational() {
        num = 0;
        den = 1;
    }

    Rational(const BigInteger& x) {
        num = x;
        den = 1;
    }

    Rational(const int n) {
        num = BigInteger(n);
        den = 1;
    }

    Rational& operator=(const Rational& rn) {
        num = rn.num;
        den = rn.den;
        normalise();
        return *this;
    }

    const BigInteger& getNum() const {
        return num;
    }

    const BigInteger& getDen() const {
        return den;
    }

    void setNum(const BigInteger& newNum) {
        num = newNum;
    }

    void setDen(const BigInteger& newDen) {
        den = newDen;
    }

    std::string toString() const {
        std::string res = num.toString();
        if (den != 1)
             res += "/" + den.toString();
        return res;
    }

    Rational& operator+=(const Rational& x) {
        Rational xx = x;
        xx.num *= den;
        xx.den *= den;
        num *= x.den;
        den *= x.den;
        num += xx.num;
        normalise();
        return *this;
    }

    Rational& operator-=(const Rational& x) {
        return *this += (-x);
    }

    Rational& operator*=(const Rational& x) {
        num *= x.num;
        den *= x.den;
        normalise();
        return *this;
    }

    Rational& operator/=(const Rational& x) {
        num *= x.den;
        den *= x.num;
        normalise();
        return *this;
    }

    Rational operator-() const {
        Rational res = *this;
        res.num = -res.num;
        res.normalise();
        return res;
    }

    std::string asDecimal(size_t precision = 0) const {
        BigInteger a = num, b = den;
        bool sign = (a.isNeg() ^ b.isNeg());
        if (a.isNeg())
            a = -a;
        if (b.isNeg())
            b = -b;
        BigInteger integer = a / b;
        a %= b;
        std::string s = (sign ? "-" : "");
        s += integer.toString();
        if (precision > 0)
            s.push_back('.');
        for (size_t i = 0; i < precision; ++i) {
            a *= 10;
            BigInteger div = a / b;
            s += div.toString();
            a %= b;
        }
        return s;
    }

    explicit operator double() const {
        std::string asd = asDecimal(10);
        double integer = 0.0;
        int ppos = 0;
        for (size_t i = 0; i < asd.size() && asd[i] != '.'; ++i, ++ppos)
            if (asd[i] >= '0' && asd[i] <= '9')
                integer = 10.0 * integer + double(asd[i] - '0');
        double frac = 0;
        for (int i = asd.size() - 1; i > ppos; --i)
            if (asd[i] >= '0' && asd[i] <= '9')
                frac = 0.1 * frac + double(asd[i] - '0');
        if (asd[0] == '-')
            integer = -integer, frac = -frac;
        return integer + 0.1 * frac;
    }
    
    double toDouble() const {
        return (double)*this;
    }
};

std::istream& operator>>(std::istream& is, Rational& x) {
    BigInteger num;
    is >> num;
    x.setNum(num);
    x.setDen(1);
    return is;
}

bool operator==(const Rational& x, const Rational& y) {
    Rational xx = x, yy = y;
    xx.setNum(x.getNum() * y.getDen());
    yy.setNum(y.getNum() * x.getDen());
    return xx.getNum() == yy.getNum();
}

bool operator<(const Rational& x, const Rational& y) {
    if (x.getNum().isNeg() && !y.getNum().isNeg())
        return true;
    if (!x.getNum().isNeg() && y.getNum().isNeg())
        return false;
    BigInteger num1 = x.getNum() * y.getDen();
    BigInteger num2 = y.getNum() * x.getDen();
    return num1 < num2;
}

bool operator>(const Rational& x, const Rational& y) {
    return y < x;
}

bool operator<=(const Rational& x, const Rational& y) {
    return !(x > y);
}

bool operator>=(const Rational& x, const Rational& y) {
    return !(x < y);
}

bool operator!=(const Rational& x, const Rational& y) {
    return !(x == y);
}

const Rational operator+(const Rational& a, const Rational& b) {
    Rational res = a;
    return res += b;
}

const Rational operator-(const Rational& a, const Rational& b) {
    Rational res = a;
    return res -= b;
}

const Rational operator*(const Rational& a, const Rational& b) {
    Rational res = a;
    return res *= b;
}

const Rational operator/(const Rational& a, const Rational& b) {
    Rational res = a;
    return res /= b;
}


int multMod(int x, int y, const int MOD) {
    return ((long long)x * y) % (long long)MOD;
}

int binPow(int val, int pw, const int MOD) {
    if (pw == 0)
        return 1;
    int sqroot = binPow(val, pw / 2, MOD);
    sqroot = multMod(sqroot, sqroot, MOD);
    if (pw & 1)
        sqroot = multMod(sqroot, val, MOD);
    return sqroot;
}

int rev(int x, const int MOD) {
    return binPow(x, MOD - 2, MOD);
}

int mod(int x, const int MOD) {
    if (x < 0) 
        x += MOD * (-x / MOD + ((-x % MOD) != 0));
    if (x >= MOD)
        x %= MOD;
    return x;
}

template<int N>
class Finite {
private:
    int val;
    
    constexpr static bool isPrimeN() {
        for (size_t i = 2; i * i <= N; ++i)
            if (N % i == 0)
                return false;
        return true;
    }

public:
    Finite(int x = 0): val(mod(x, N)) {}
    
    Finite(const Finite& other): val(mod(other.val, N)) { }
    
    int getVal() const {
        return val;
    }
    
    Finite& operator+=(const Finite& other) {
        val += other.val;
        if (val >= N)
            val -= N;
        return *this;
    }
    
    Finite& operator-=(const Finite& other) {
        val -= other.val;
        if (val < 0)
            val += N;
        return *this;
    }
   
    Finite& operator*=(const Finite& other) {
        val = ((long long)val * other.val) % N;
        return *this;
    }
    
    Finite& operator/=(const Finite& other) {
        //static_assert(isPrimeN(), "Modulo isn't prime");
        return *this *= Finite(rev(other.val, N));
    }
    
    Finite& operator++(int) {
        Finite res = Finite(val);
        val += 1;
        if (val >= N)
            val -= N;
        return res;
    }
    
    Finite& operator++() {
        val += 1;
        if (val >= N)
            val -= N;
        return *this;
    }
    
    Finite& operator--(int) {
        Finite res = Finite(val);
        val -= 1;
        if (val < 0)
            val += N;
        return res;
    }
    
    Finite& operator--() {
        val -= 1;
        if (val < 0)
            val += N;
        return *this;
    }
    
    Finite& operator=(const Finite& other) {
        val = other.val;
        return *this;
    }
    
    const Finite operator-() {
        Finite f(*this);
        int dec = 2 * val;
        if (dec >= N)
            dec -= N;
        f -= dec;
        return f;
    }
    
    friend const Finite operator-(const Finite& finite) {
        Finite res = Finite(finite);
        res.val = -res.val;
        if (res.val < 0)
            res.val += N;
        return res;
    }
    
    friend const Finite operator+(const Finite& finite1, const Finite& finite2) {
        Finite res = finite1;
        return res += finite2;
    }
   
    friend const Finite operator-(const Finite& finite1, const Finite& finite2) {
        Finite res = finite1;
        return res -= finite2;
    }
    
    friend const Finite operator*(const Finite& finite1, const Finite& finite2) {
        Finite res = finite1;
        return res *= finite2;
    }
   
    friend const Finite operator/(const Finite& finite1, const Finite& finite2) {
        Finite res = finite1;
        return res /= finite2;
    }
   
    explicit operator int() {
        return val;
    }
    
    bool operator==(const Finite& other) const {
        return val == other.val;
    }
    
    bool operator!=(const Finite& other) const {
        return val != other.val;
    }
    
    bool operator==(const int num) const {
        return val == num;
    }
    
    bool operator!=(const int num) const {
        return val != num;
    }
};

template<int N>
std::istream& operator>>(std::istream& is, Finite<N>& x) {
    int val;
    is >> val;
    x = Finite<N>(val);
    return is;
}

template<unsigned N, unsigned M, typename Field>
class Matrix;

//template<unsigned N, unsigned M, unsigned K, typename Field>
//Matrix<N, K, Field> operator*(const Matrix<N, M, Field>& matrix1, const Matrix<M, K, Field>& matrix2);

template<unsigned N, unsigned M, typename Field = Rational>
class Matrix {
private:
    Field **arr;
    
    
    void swapRows(int r1, int r2) {
        if (r1 == r2)
            return;
        Field* t = arr[r1];
        arr[r1] = arr[r2];
        arr[r2] = t;
       // if (correctDet)
       //     cdet = -cdet;
    }
    
    const std::pair<Matrix, Matrix> gauss(bool needRev = false) const {
        Matrix diag(*this), rev;
        size_t to = std::min(N, M);
        int putRowTo = -1;
        for (size_t i = 0; i < to; ++i) {
            int row = -1;
            for (size_t j = i; j < N; ++j)
                if (diag.arr[j][i] != 0) {
                    row = j;
                    break;
                }
            if (row == -1)
                continue;
            ++putRowTo;
            for (int j = row; j > putRowTo; --j) {
                diag.swapRows(j, j - 1);
                if (needRev)
                    rev.swapRows(j, j - 1);
            }
            for (size_t j = 0; j < N; ++j)
                if (j != static_cast<size_t>(putRowTo) && diag.arr[j][i] != 0) {
                    Field coef = diag.arr[j][i] / diag.arr[putRowTo][i];
                    for (size_t x = 0; x < M; ++x) {
                        diag.arr[j][x] -= diag.arr[putRowTo][x] * coef;
                        if (needRev)
                            rev.arr[j][x] -= rev.arr[putRowTo][x] * coef;
                    }
                }
        }
        if (N == M) {
           // cdet = 1;
           /* for (size_t i = 0; i < N; ++i)
                cdet *= diag.arr[i][i];
            correctDet = true;*/
        }
        return {diag, rev};
    }
public:
     //mutable Field cdet;
     //mutable bool correctDet;
    
     Matrix() {
        arr = new Field*[N];
        for (size_t i = 0; i < N; ++i) {
            arr[i] = new Field[M];
            for (size_t j = 0; j < M; ++j)
                arr[i][j] = 0;
            if (N == M) 
                arr[i][i] = Field(1);
        }
        if (N == M) {
            //correctDet = true;
            //cdet = Field(1);
        }
    }
    
    Matrix(const Matrix& other) {
        arr = new Field*[N];
        for (size_t i = 0; i < N; ++i) {
            arr[i] = new Field[M];
            for (size_t j = 0; j < M; ++j)
                arr[i][j] = other.arr[i][j];
        }
    }
    
    Matrix(const std::vector<std::vector<int>>& vec) {
        //static_assert(N == vec.size() && M == vec[0].size(), "Invalid sizes\n");

        arr = new Field*[N];
        for (size_t i = 0; i < N; ++i) {
            arr[i] = new Field[M];
            for (size_t j = 0; j < M; ++j)
                arr[i][j] = vec[i][j];
        }
    }
    
    ~Matrix() {
        for (size_t i = 0; i < N; ++i)
            delete[] arr[i];
        delete[] arr;
        //correctDet = false;
    }
    
    Matrix& operator+=(const Matrix& other) {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j)
                arr[i][j] += other.arr[i][j];
        //correctDet = false;
        return *this;
    }
    
    Matrix& operator-=(const Matrix& other) {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j)
                arr[i][j] -= other.arr[i][j];
        //correctDet = false;
        return *this;
    }
    
    Matrix& operator/=(const Field& to) {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j)
                arr[i][j] /= to;
        //correctDet = false;
        return *this;
    }
    
    Matrix& operator*=(const Field& alpha) {
         for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j)
                 arr[i][j] *= alpha;
        //correctDet = false;
        return *this;
    }
    
    const Matrix operator+(const Matrix& other) const {
        Matrix res = *this;
        return res += other;
    }
    
    const Matrix operator-(const Matrix& other) const {
        Matrix res = *this;
        return res -= other;
    }
    
    const Matrix operator/(const Field& to) const {
        Matrix res = *this;
        return res /= to;
    }
    
    const Matrix operator*(const Field& to) const {
        Matrix res = *this;
        return res *= to;
    }
    
    Field* operator[](int pos) {
        //correctDet = false;
        return arr[pos];
    }
    
    const Field* operator[](int pos) const {
        return arr[pos];
    }
    
    const Field trace() const {
        static_assert(N == M, "Given matrix isn't a square matrix(N != M).");
        Field res;
        for (size_t i = 0; i < N; ++i)
            res += arr[i][i];
        return res;
    }
    
    const Field det() const {
        static_assert(N == M, "Given matrix isn't a square matrix(N != M).");
//        if (correctDet)
//            return cdet;
        Matrix diag = gauss().first;
        Field res = 1;
        for (size_t i = 0; i < N; ++i)
            res *= diag.arr[i][i];
        return res;
    }
    
    unsigned rank() const {
        Matrix diag = gauss().first;
        unsigned res = 0;
        for (size_t i = 0; i < N; ++i) {
            bool isNull = true;
            for (size_t j = 0; j < M; ++j)
                isNull &= (diag.arr[i][j] == 0);
            res += (!isNull);
        }
        return res;
    }
    
    const Matrix inverted() const {
        static_assert(N == M, "Given matrix isn't a square matrix(N != M).");
        std::pair<Matrix, Matrix> res = gauss(true);
        for (size_t i = 0; i < N; ++i) 
            for (size_t j = 0; j < M; ++j) {
              //  std::cerr << res.second[i][j].getVal() << ' ' << res.first[i][i].getVal() << ' ' << (res.second[i][j] / res.first[i][j]).getVal() << '\n';
                if (res.first[i][i] != 0)
                    res.second[i][j] /= res.first[i][i];
            }
        return res.second;
    }
    
    void invert() {
        *this = inverted();
        //correctDet = false;
    }
    
    const Matrix<M, N, Field> transposed() const {
        Matrix<M, N, Field> res;
        for (size_t i = 0; i < M; ++i)
            for (size_t j = 0; j < N; ++j) 
                res[i][j] = arr[j][i];
        //res.correctDet = correctDet;
        //if (correctDet)
        //    res.cdet = cdet;
        return res;
    }
    
    Matrix& operator*=(const Matrix& other) {
        static_assert(N == M, "Given matrix isn't a square matrix(N != M).");
        Matrix res = *this * other;
        /*correctDet = false;
        if (correctDet && other.correctDet) {
            res.correctDet = true;
            res.cdet = cdet * other.cdet;
        }*/
        return *this = res;
    }
    
    std::vector<Field> getRow(unsigned rowNum) const {
        std::vector<Field> res;
        for (size_t i = 0; i < M; ++i)
            res.push_back(arr[rowNum][i]);
        return res;
    }
    
    std::vector<Field> getColumn(unsigned columnNum) const {
        std::vector<Field> res;
        for (size_t i = 0; i < N; ++i)
            res.push_back(arr[i][columnNum]);
        return res;
    }
    
    Matrix& operator=(const Matrix& other) {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j)
                arr[i][j] = other.arr[i][j];
        //correctDet = false;
        return *this;
    }
    
    bool operator==(const Matrix& other) const {
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < M; ++j)
                if (arr[i][j] != other[i][j])
                    return false;
        return true;
    }
    
    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }
    
    void print() const {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < M; ++j)
                std::cout << arr[i][j].getVal() << ' ';
            std::cout << '\n';
        }
    }
};

template<unsigned N, unsigned M, typename Field = Rational> 
const Matrix<N, M, Field> operator*(const Field& coeff, const Matrix<N, M, Field>& matrix) {
    Matrix<N, M, Field> res = matrix;
    return res *= coeff;
}

template<unsigned N, unsigned M, unsigned K, typename Field = Rational>
const Matrix<N, K, Field> operator*(const Matrix<N, M, Field>& matrix1, const Matrix<M, K, Field>& matrix2) {
    Matrix<N, K, Field> res;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < K; ++j)
            res[i][j] = 0;
    //if (max(max(N, M), K) <= 64) {    
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < K; ++j)
                for (size_t k = 0; k < M; ++k)
                    res[i][j] += matrix1[i][k] * matrix2[k][j];
        return res;
    //}
    const int NN = N + (N % 2 != 0);
    const int MM = M + (M % 2 != 0);
    const int KK = K + (K % 2 != 0);
    Matrix<NN, MM, Field> a;
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < M; ++j)
            a[i][j] = matrix1[i][j];
    Matrix<MM, KK, Field> b;
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < K; ++j)
            b[i][j] = matrix2[i][j];
    Matrix<NN, KK, Field> c;
    Matrix<NN / 2, MM / 2, Field> a11, a12, a21, a22;
    for (size_t i = 0; i < NN / 2; ++i)
        for (size_t j = 0; j < MM / 2; ++j)
            a11[i][j] = a[i][j];
    for (size_t i = 0; i < NN / 2; ++i)
        for (size_t j = 0; j < MM / 2; ++j)
            a12[i][j] = a[i][j + MM / 2];
    for (size_t i = 0; i < NN / 2; ++i)
        for (size_t j = 0; j < MM / 2; ++j)
            a21[i][j] = a[i + NN / 2][j];
    for (size_t i = 0; i < NN / 2; ++i)
        for (size_t j = 0; j < MM / 2; ++j)
            a22[i][j] = a[i + NN / 2][j + MM / 2];
    Matrix<MM / 2, KK / 2, Field> b11, b12, b21, b22;
    for (size_t i = 0; i < MM / 2; ++i)
        for (size_t j = 0; j < KK / 2; ++j)
            b11[i][j] = b[i][j];
    for (size_t i = 0; i < MM / 2; ++i)
        for (size_t j = 0; j < KK / 2; ++j)
            b12[i][j] = b[i][j + KK / 2];
    for (size_t i = 0; i < MM / 2; ++i)
        for (size_t j = 0; j < KK / 2; ++j)
            b21[i][j] = b[i + MM / 2][j];
    for (size_t i = 0; i < MM / 2; ++i)
        for (size_t j = 0; j < KK / 2; ++j)
            b22[i][j] = b[i + MM / 2][j + KK / 2];
    Matrix<NN / 2, KK / 2, Field> p1, p2, p3, p4, p5, p6, p7;
    p1 = (a11 + a22) * (b11 + b22);
    p2 = (a21 + a22) * b11;
    p3 = a11 * (b12 - b22);
    p4 = a22 * (b21 - b11);
    p5 = (a11 + a12) * b22;
    p6 = (a21 - a11) * (b11 + b12);
    p7 = (a12 - a22) * (b21 + b22);
    for (size_t i = 0; i < NN / 2; ++i)
        for (size_t j = 0; j < KK / 2; ++j)
            c[i][j] = p1[i][j] + p4[i][j] - p5[i][j] + p7[i][j];
    for (size_t i = 0; i < NN / 2; ++i)
        for (size_t j = 0; j < KK / 2; ++j)
            c[i][j + KK / 2] = p3[i][j] + p5[i][j];
    for (size_t i = 0; i < NN / 2; ++i)
        for (size_t j = 0; j < KK / 2; ++j)
            c[i + NN / 2][j] = p2[i][j] + p4[i][j];
    for (size_t i = 0; i < NN / 2; ++i)
        for (size_t j = 0; j < KK / 2; ++j)
            c[i + NN / 2][j + KK / 2] = p1[i][j] - p2[i][j] + p3[i][j] + p6[i][j];
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < K; ++j)
            res[i][j] = c[i][j];
    return res;
}

template<unsigned N, typename Field = Rational>
using SquareMatrix = Matrix<N, N, Field>;

#include <stdexcept>