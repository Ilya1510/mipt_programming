#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>

//we garantee that value >= 0
int binPow(int a, int n, int mod) {
    if (n == 0) {
        return 1;
    }
    long long x = binPow(a, n / 2, mod);
    long long y = (x * x) % mod;
    return static_cast<int>(n % 2 == 0 ? y : (y * a) % mod);
}

template <int N>
class Finite {
    int value_;
public:
    Finite() : value_(0) {}
    Finite(int value) {
        value_ = (value % N + N) % N;
    }
    int value() const {
        return value_;
    }
    Finite& operator+=(const Finite& a) {
        value_ = (value_ + a.value_) % N;
        return *this;
    }
    Finite& operator-=(const Finite& a) {
        value_ = (value_ + N - a.value_) % N;
        return *this;
    }
    Finite& operator*=(const Finite& a) {
        value_ = (static_cast<long long>(value_)* a.value_) % N;
        return *this;
    }
    Finite& operator/=(const Finite& a) {
        *this *= Finite(binPow(a.value_, N - 2, N));
        return *this;
    }
    Finite& operator=(const Finite& a) {
        if (this != &a) {
            value_ = a.value();
        }
        return *this;
    }
    Finite& operator++() {
        return *this += 1;
    }
    Finite& operator--() {
        return *this -= 1;
    }
    const Finite operator++(int) {
        Finite tmp = *this;
        *this += 1;
        return tmp;
    }
    const Finite operator--(int) {
        Finite tmp = *this;
        *this -= 1;
        return tmp;
    }
    explicit operator bool() const {
        return value_ != 0;
    }
    const std::string toString() const {
        int x = value_;
        std::string res = "";
        while (x) {
            res.push_back('0' + x % 10);
            x /= 10;
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
};
template <int N>
const Finite<N> operator+(const Finite<N>& a, const Finite<N> &b) {
    Finite<N> tmp = a;
    tmp += b;
    return tmp;
}
template <int N>
const Finite<N> operator-(const Finite<N>& a, const Finite<N> &b) {
    Finite<N> tmp = a;
    tmp -= b;
    return tmp;
}
template <int N>
const Finite<N> operator*(const Finite<N>& a, const Finite<N> &b) {
    Finite<N> tmp = a;
    tmp *= b;
    return tmp;
}
template <int N>
const Finite<N> operator/(const Finite<N>& a, const Finite<N> &b) {
    Finite<N> tmp = a;
    tmp /= b;
    return tmp;
}
template <int N>
bool operator==(const Finite<N>& a, const Finite<N> &b) {
    return a.value() == b.value();
}
template <int N>
bool operator!=(const Finite<N>& a, const Finite<N> & b) {
    return a.value() != b.value();
}

template<int N>
std::ostream& operator<<(std::ostream &out, const Finite<N>& a) {
    out << a.value();
    return out;
}

namespace numbers {

    using std::vector;
    using std::string;
    using std::cin;
    using std::cout;
    using std::swap;

    typedef unsigned int uint;
    typedef long long ll;

    template<typename T>
    const T max(const T& a, const T& b) {
        return a < b ? b : a;
    }

    template<typename T>
    const T abs(const T& a) {
        return a < 0 ? -a : a;
    }

    class BigInteger {
    private:
        vector<long long> digits_;
        bool isPositive_;

        BigInteger& unsignedAdd_(const BigInteger& other) {
            long long carry = 0;
            for (size_t i = 0; i < max(digits_.size(), other.digits_.size()) || carry; ++i) {
                if (digits_.size() <= i)
                    digits_.push_back(0);
                digits_[i] += (i < other.digits_.size() ? other.digits_[i] : 0) + carry;
                carry = 0;
                if (digits_[i] >= BASE) {
                    carry = 1;
                    digits_[i] -= BASE;
                }
            }
            return *this;
        }

        BigInteger& unsignedSub_(const BigInteger& other) {
            long long carry = 0;
            for (size_t i = 0; i < digits_.size() || carry; ++i) {
                digits_[i] -= (i < other.digits_.size() ? other.digits_[i] : 0) + carry;
                carry = 0;
                if (digits_[i] < 0) {
                    carry = 1;
                    digits_[i] += BASE;
                }
            }
            for (int i = static_cast<int>(digits_.size()) - 1; i >= 0 && digits_[i] == 0; --i)
                digits_.pop_back();
            return *this;
        }

        BigInteger& unsignedSubRev_(const BigInteger& other) {
            long long carry = 0;
            for (size_t i = 0; i < other.digits_.size() || carry; ++i) {
                if (i <= digits_.size())
                    digits_.push_back(0);
                digits_[i] = other.digits_[i] - digits_[i] - carry;
                carry = 0;
                if (digits_[i] < 0) {
                    carry = 1;
                    digits_[i] += BASE;
                }
            }
            for (int i = static_cast<int>(digits_.size()) - 1; i >= 0 && digits_[i] == 0; --i)
                digits_.pop_back();
            return *this;
        }

        BigInteger& multiplyByLong(long long x) {
            long long carry = 0;
            for (size_t i = 0; i < digits_.size() || carry; ++i) {
                if (i >= digits_.size())
                    digits_.push_back(0);
                digits_[i] = digits_[i] * x + carry;
                carry = 0;
                if (digits_[i] >= BASE) {
                    carry = digits_[i] / BASE;
                    digits_[i] %= BASE;
                }
            }
            for (int i = static_cast<int>(digits_.size()) - 1; i >= 0 && digits_[i] == 0; --i)
                digits_.pop_back();
            return *this;
        }
    public:
        static const long long BASE = 100000000;
        static const long long BASE_LEN = 8;
        static const int BASE_LOG = 27;

        BigInteger() {
            digits_.clear();
            isPositive_ = true;
        }

        BigInteger(long long x) {
            digits_.clear();
            isPositive_ = (x >= 0);
            long long curx = numbers::abs(x);
            while (curx) {
                digits_.push_back(curx % BASE);
                curx /= BASE;
            }
        }

        BigInteger(const std::string& str) {
            digits_.clear();
            isPositive_ = true;
            if (str.empty())
                return;
            isPositive_ = !(str[0] == '-');
            long long carry = 0;
            for (int i = static_cast<int>(str.size()) - 1; i >= (isPositive_ ? 0 : 1); i -= BASE_LEN) {
                carry = 0;
                for (ll j = max(static_cast<ll>(!isPositive_), i - BASE_LEN + 1); j <= i; ++j) {
                    carry *= 10;
                    carry += str[j] - '0';
                }
                digits_.push_back(carry);
            }
            for (int i = static_cast<int>(digits_.size()) - 1; i >= 0 && digits_[i] == 0; --i)
                digits_.pop_back();
            if (digits_.empty())
                isPositive_ = true;
        }

        bool isNeg() const {
            return !isPositive_;
        }

        bool isPositive() const {
            return isPositive_;
        }

        uint getLen() const {
            return digits_.size();
        }

        BigInteger& operator = (const BigInteger& x) {
            digits_ = x.digits_;
            isPositive_ = x.isPositive_;
            return *this;
        }

        string toString() const {
            string res = (!isPositive_ ? "-" : "");
            if (digits_.empty())
                res += "0";
            long long x;
            string tmp;
            for (int i = static_cast<int>(digits_.size()) - 1; i >= 0; --i) {
                x = digits_[i];
                tmp = "";
                while (tmp.size() < BASE_LEN) {
                    tmp += char('0' + x % 10);
                    x /= 10;
                }
                int flag = 0;
                for (int j = static_cast<int>(tmp.size()) - 1; j >= 0; --j) {
                    if (i == static_cast<int>(digits_.size()) - 1 && tmp[j] == '0' && !flag)
                        continue;
                    flag = 1;
                    res += tmp[j];
                }
            }
            return res;
        }

        explicit operator bool() const {
            return digits_.size() != 0;
        }

        explicit operator double() const {
            double ans = 0;
            for (size_t i = 0; i < digits_.size(); i++) {
                ans *= BASE;
                ans += digits_[i];
            }
            ans *= getSign();
            return ans;
        }

        int operator[] (uint x) const {
            return x < digits_.size() ? digits_[x] : 0;
        }

        unsigned int size() const {
            return digits_.size();
        }

        int getSign() const {
            return isPositive_ ? 1 : -1;
        }

        void changeSign() {
            if (digits_.size())
                isPositive_ = !isPositive_;
            else
                isPositive_ = true;
        }

        bool operator<(const BigInteger& other) const {
            if (isPositive_ != other.isPositive_)
                return isPositive_ < other.isPositive_;
            bool ans = 0;
            if (digits_.size() != other.digits_.size())
                ans = (digits_.size() < other.digits_.size()) ^ (!isPositive_);
            else {
                for (int i = static_cast<int>(digits_.size()) - 1; i >= 0; --i) {
                    if (digits_[i] != other.digits_[i]) {
                        ans = (digits_[i] < other.digits_[i]) ^ (!isPositive_);
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
            if (a.digits_.size())
                a.changeSign();
            return a;
        }

        const BigInteger operator+(const BigInteger& other) const {
            BigInteger result = *this;
            return result += other;
        }

        BigInteger& operator+=(const BigInteger& other) {
            if (isPositive_ && other.isPositive_) {
                unsignedAdd_(other);
            }
            else if (!isPositive_ && !other.isPositive_) {
                unsignedAdd_(other);
            }
            else if (!isPositive_ && other.isPositive_) {
                if (-*this <= other)
                    unsignedSubRev_(other), changeSign();
                else
                    unsignedSub_(other);
            }
            else {
                if (*this >= -other)
                    unsignedSub_(other);
                else
                    unsignedSubRev_(other), changeSign();
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
            if (digits_.empty() || other.digits_.empty())
                return 0;
            BigInteger ans = 0;
            BigInteger z = *this;
            BigInteger tmp;
            z.isPositive_ = true;
            for (size_t i = 0; i < other.digits_.size(); ++i) {
                tmp = z;
                ans += tmp.multiplyByLong(other.digits_[i]);
                z.multiplyByLong(BASE);
            }
            ans.isPositive_ = (isPositive_ != other.isPositive_);
            return ans;
        }

        BigInteger& operator*=(const BigInteger& other) {
            return *this = *this * other;
        }

        const BigInteger operator/(const BigInteger& other) const {
            if (digits_.empty() || other.digits_.empty())
                return 0;
            BigInteger ans = 0;
            BigInteger z = 0;
            BigInteger absx = numbers::abs(other);
            BigInteger x2[BASE_LOG + 1];
            x2[0] = absx;
            for (int i = 1; i <= BASE_LOG; ++i) {
                x2[i] = x2[i - 1];
                x2[i].multiplyByLong(2);
            }
            int flag = -1;
            for (int i = static_cast<int>(digits_.size()) - 1; i >= 0; --i) {
                z.multiplyByLong(BASE);
                z += digits_[i];
                if (flag == -1 && z >= numbers::abs(other))
                    flag = i;
                if (flag != -1) {
                    ans.digits_.push_back(0);
                    if (z >= absx) {
                        for (int j = BASE_LOG; j >= 0; --j)
                        while (z >= x2[j]) {
                            ans.digits_[flag - i] += (1 << j);
                            z -= x2[j];
                        }
                    }
                }
            }
            if (ans.digits_.size()) {
                reverse(ans.digits_.begin(), ans.digits_.end());
                ans.isPositive_ = (isPositive_ != other.isPositive_);
            }
            return ans;
        }

        BigInteger& operator/=(const BigInteger& other) {
            return *this = *this / other;
        }

        const BigInteger operator%(const BigInteger& other) const {
            if (digits_.empty() || other.digits_.empty())
                return 0;
            BigInteger z = 0;
            BigInteger absx = numbers::abs(other);
            BigInteger x2[BASE_LOG + 1];
            x2[0] = absx;
            for (int i = 1; i <= BASE_LOG; ++i) {
                x2[i] = x2[i - 1];
                x2[i].multiplyByLong(2);
            }
            for (int i = static_cast<int>(digits_.size()) - 1; i >= 0; --i) {
                z.multiplyByLong(BASE);
                z += digits_[i];
                if (z >= absx) {
                    for (int j = BASE_LOG; j >= 0; --j)
                    while (z >= x2[j])
                        z -= x2[j];
                }
            }
            if (z.digits_.size())
                z.isPositive_ = isPositive_;
            return z;
        }

        BigInteger& operator%=(const BigInteger& x) {
            return *this = *this % x;
        }

        explicit operator bool() {
            return digits_.size();
        }
    };

    std::ostream& operator<<(std::ostream& out, const BigInteger& x) {
        out << x.toString();
        return out;
    }

    std::istream& operator>>(std::istream& in, BigInteger& x) {
        string st;
        in >> st;
        x = st;
        return in;
    }

    const BigInteger gcd(const BigInteger& x, const BigInteger& y) {
        return y ? gcd(y, x % y) : x;
    }

    class Rational{
    private:
        BigInteger num, denom;
        void reduceFraction() {
            BigInteger c = 1;
            if (num.isNeg()) {
                c = gcd(-num, denom);
            }
            else {
                c = gcd(num, denom);
            }
            if (c != 1) {
                num /= c;
                denom /= c;
            }
        }
    public:
        Rational(const Rational& a) {
            num = a.num;
            denom = a.denom;
        }
        Rational(const BigInteger& num = 0, const BigInteger& denom = 1)
            :num(num), denom(denom) {
            reduceFraction();
        }
        Rational(int num) : num(num), denom(1) {}
        Rational(const std::string& s) {
            uint indDiv = 0;
            string numString = "";
            for (; indDiv < s.size() && s[indDiv] != '/'; ++indDiv) {
                numString.push_back(s[indDiv]);
            }
            string denomString = "";
            for (++indDiv; indDiv < s.size(); ++indDiv) {
                denomString.push_back(s[indDiv]);
            }
            if (denomString == "") {
                denomString.push_back('1');
            }
            BigInteger bNum(numString);
            BigInteger bDenom(denomString);
            num = bNum;
            denom = bDenom;
            //reduceFraction();
        }
        const BigInteger& getNum() const {
            return num;
        }
        const BigInteger& getDenom() const {
            return denom;
        }
        string toString() const {
            if (denom == 1) {
                return num.toString();
            }
            return num.toString() + '/' + denom.toString();
        }
        string asDecimal(size_t precision = 0) const {
            Rational tmp = *this;
            string res;
            res += (tmp.num / tmp.denom).toString();
            if (precision == 0) {
                return res;
            }
            if (tmp.num < 0 && res[0] == '0') {
                res.push_back('0');
                res[0] = '-';
            }
            tmp.num %= tmp.denom;
            res += '.';
            if (tmp.num < 0) {
                tmp.num *= -1;
            }
            for (size_t i = 0; i < precision; ++i) {
                tmp.num *= 10;
                res += (tmp.num / tmp.denom).toString();
                tmp.num %= tmp.denom;
            }
            return res;
        }
        const Rational operator-() const {
            Rational res = *this;
            res *= Rational(-1);
            return res;
        }
        Rational& operator+=(const Rational& numberToAdd) {
            BigInteger newNum = num * numberToAdd.getDenom() + denom * numberToAdd.getNum();
            denom *= numberToAdd.denom;
            num = newNum;
            reduceFraction();
            return *this;
        }
        Rational& operator*=(const Rational& numberToMultiply) {
            num *= numberToMultiply.getNum();
            denom *= numberToMultiply.getDenom();
            reduceFraction();
            return *this;
        }
        Rational& operator-=(const Rational& numberToSub) {
            BigInteger newNum = num * numberToSub.getDenom() - denom * numberToSub.getNum();
            denom *= numberToSub.denom;
            num = newNum;
            reduceFraction();
            return *this;
        }
        Rational& operator/=(const Rational& numberToDivide) {
            if (numberToDivide.getNum() <= 0) {
                num *= -numberToDivide.getDenom();
                denom *= -numberToDivide.getNum();
            }
            else {
                num *= numberToDivide.getDenom();
                denom *= numberToDivide.getNum();
            }
            reduceFraction();
            return *this;
        }
        explicit operator bool() const {
            return static_cast<bool>(num);
        }
        explicit operator double() const {
            string stringRepresentNumber = asDecimal(100);
            double res = 0;
            int bord = stringRepresentNumber.size();
            for (unsigned int i = 0; i < stringRepresentNumber.size(); ++i) {
                if (stringRepresentNumber[i] == '.') {
                    bord = i;
                }
            }
            bool neg = stringRepresentNumber[0] == '-';
            double deg = 1;
            for (int i = bord - 1; i >= 0 + neg; --i, deg *= 10) {
                res += deg * (stringRepresentNumber[i] - '0');
            }
            deg = 0.1;
            for (unsigned int i = bord + 1; i < stringRepresentNumber.size(); ++i, deg /= 10) {
                res += deg * (stringRepresentNumber[i] - '0');
            }
            return neg ? -res : res;
        }
    };

    const Rational operator+(const Rational& a, const Rational& b) {
        Rational res = a;
        res += b;
        return res;
    }
    const Rational operator-(const Rational& a, const Rational& b) {
        Rational res = a;
        res -= b;
        return res;
    }
    const Rational operator*(const Rational& a, const Rational& b) {
        Rational res = a;
        res *= b;
        return res;
    }
    const Rational operator/(const Rational& a, const Rational& b) {
        Rational res = a;
        res /= b;
        return res;
    }

    bool operator==(const Rational& a, const Rational& b) {
        return a.getNum() == b.getNum() && a.getDenom() == b.getDenom();
    }
    bool operator!=(const Rational& a, const Rational& b) {
        return !(a == b);
    }
    bool operator<(const Rational& a, const Rational& b) {
        return a.getNum() * b.getDenom() < a.getDenom() * b.getNum();
    }
    bool operator>(const Rational& a, const Rational& b) {
        return b < a;
    }
    bool operator<=(const Rational& a, const Rational& b) {
        return !(b < a);
    }
    bool operator>=(const Rational& a, const Rational& b) {
        return !(a < b);
    }
    std::istream& operator>>(std::istream& is, Rational &r) {
        string s;
        is >> s;
        r = Rational(s);
        return is;
    }
    std::ostream& operator<<(std::ostream& os, const Rational &r) {
        os << r.toString();
        return os;
    }

}

using numbers::BigInteger;
using numbers::Rational;

typedef unsigned uint;

template <typename Field>
class RowMatrix {
    Field* row_;
public:
    RowMatrix(Field* a) : row_(a) {}
    const Field& operator[](unsigned j) const {
        return row_[j];
    }
    Field& operator[](unsigned j) {
        return row_[j];
    }
};

template <unsigned M, unsigned N, typename Field = Rational>
class Matrix {
private:
    Field matrix_[M][N];
    const std::pair<Matrix, Matrix> gaussMethod(bool needInvert) const {
        unsigned currentHeight = 0;
        int changeSignDet = 0;
        Matrix diag = *this;
        Matrix inverted;
        for (uint i = 0; i < N; ++i) { //choose stolbik
            std::cout << 1 << std::endl;
            bool findNotNullElement = true;
            if (currentHeight < M && !diag.matrix_[currentHeight][i]) {
                findNotNullElement = false;
                for (uint j = currentHeight + 1; j < M; ++j) {
                    if (diag.matrix_[j][i]) {
                        std::swap(diag.matrix_[currentHeight], diag.matrix_[j]);
                        changeSignDet ^= (j - currentHeight) % 2;
                        if (needInvert) {
                            std::swap(inverted.matrix_[currentHeight], inverted.matrix_[j]);
                        }
                        findNotNullElement = true;
                        break;
                    }
                }
            }
            if (!findNotNullElement) {
                continue;
            }
            for (uint j = 0; j < M; ++j) {
                if (j != currentHeight) {
                    Field coeff = diag.matrix_[j][i] / diag.matrix_[currentHeight][i];
                    //matrix_[j] -= matrix_[currentHeight] * coeff
                    for (uint t = 0; t < N; ++t) {
                        diag.matrix_[j][t] -= diag.matrix_[currentHeight][t] * coeff;
                    }
                    if (needInvert) {
                        for (uint t = 0; t < N; ++t) {
                            inverted.matrix_[j][t] -= inverted.matrix_[currentHeight][t] * coeff;
                        }
                    }
                }
            }
            ++currentHeight;
        }

        if (changeSignDet) {
            for (uint t = 0; t < N; ++t) {
                diag.matrix_[currentHeight - 1][t] *= -1;
            }
            if (needInvert) {
                for (uint t = 0; t < N; ++t) {
                    inverted.matrix_[currentHeight - 1][t] *= -1;
                }
            }
        }
        return std::make_pair(diag, inverted);
    }
public:
    static const unsigned WIDTH = N;
    static const unsigned HEIGHT = M;

    Matrix() {
        //static_assert(N == M, "try create E with not square matrix");
        if (N == M) {
            for (uint i = 0; i < M; ++i) {
                for (uint j = 0; j < N; ++j) {
                    matrix_[i][j] = i == j ? Field(1) : Field(0);
                }
            }
        }
        else {
            for (uint i = 0; i < M; ++i) {
                for (uint j = 0; j < N; ++j) {
                    matrix_[i][j] = i == j ? Field(1) : Field(0);
                }
            }
        }
    }
    Matrix(const std::vector <std::vector<Field> >& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] = a[i][j];
            }
        }
    }
    Matrix(const std::vector<std::vector<int> >& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] = Field(a[i][j]);
            }
        }
    }
    Matrix(const Matrix& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] = a.matrix_[i][j];
            }
        }
    }
    explicit Matrix(const Field& a) {
        for (uint i = 0; i < N && i < M; ++i) {
            matrix_[i][i] = a;
        }
    }

    static const Matrix getZeroMatrix() {
        Matrix a;
        for (uint i = 0; i < N && i < M; ++i) {
            a[i][i] = 0;
        }
        return a;
    }
    //fill matrix from matrix less size
    template<unsigned H, unsigned W>
    Matrix(const Matrix<H, W, Field>& a, unsigned up, unsigned left) {
        for (uint i = 0; i < M && i + up < H; ++i) {
            for (uint j = 0; j < N && j + left < W; ++j) {
                matrix_[i][j] = a[i + up][j + left];
            }
        }
    }
    Matrix& operator=(const Matrix& a) {
        if (this != &a) {
            for (uint i = 0; i < M; ++i) {
                for (uint j = 0; j < N; ++j) {
                    matrix_[i][j] = a.matrix_[i][j];
                }
            }
        }
        return *this;
    }
    const Field trace() const {
        static_assert(N == M, "try get trace from not square matrix");
        Field res = 0;
        for (uint i = 0; i < N; ++i) {
            res += matrix_[i][i];
        }
        return res;
    }

    Matrix& operator+=(const Matrix& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] += a.matrix_[i][j];
            }
        }
        return *this;
    }
    Matrix& operator-=(const Matrix& a) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] -= a.matrix_[i][j];
            }
        }
        return *this;
    }
    Matrix& operator*=(const Matrix& a) {
        static_assert(M == N, "matrix are not square in *=");
        return *this = *this * a;
    }
    Matrix& operator*=(const Field& k) {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                matrix_[i][j] *= k;
            }
        }
        return *this;
    }
    const Matrix<N, M, Field> transposed() const {
        Matrix<N, M, Field> res;
        for (uint i = 0; i < N; ++i) {
            for (uint j = 0; j < M; ++j) {
                res[i][j] = matrix_[j][i];
            }
        }
        return res;
    }
    uint rank() const {
        Matrix diag = gaussMethod(false).first;
        unsigned currentHeight = 0;
        for (uint i = 0; i < N; ++i) {
            if (diag[currentHeight][i]) {
                ++currentHeight;
            }
        }
        return currentHeight;
    }
    const Field det() const {
        static_assert(M == N, "try get det from not square matrix");
        Matrix diag = gaussMethod(false).first;
        unsigned currentHeight = 0;
        Field res = 1;
        for (uint i = 0; i < N; ++i) {
            if (currentHeight < M && diag[currentHeight][i]) {
                res *= diag[currentHeight][i];
                ++currentHeight;
            }
        }
        return res;
    }
    const Matrix inverted() const {
        static_assert(M == N, "try get inverted from not square matrix");
        std::pair<Matrix, Matrix> gaussMatrix = gaussMethod(true);
        Matrix& diag = gaussMatrix.first;
        Matrix& inverted = gaussMatrix.second;
        unsigned currentHeight = 0;
        for (uint i = 0; i < N; ++i) {
            if (currentHeight < M && diag[currentHeight][i]) {
                for (uint t = 0; t < N; ++t) {
                    inverted.matrix_[currentHeight][t] /= diag[currentHeight][i];
                }
                ++currentHeight;
            }
        }
        return inverted;
    }
    void invert() {
        static_assert(M == N, "try invert not square matrix");
        *this = inverted();
    }
    const std::vector<Field> getRow(unsigned i) const {
        std::vector<Field> res;
        for (uint j = 0; j < N; ++j) {
            res.push_back(matrix_[i][j]);
        }
        return res;
    }
    const std::vector<Field> getColumn(unsigned j) const {
        std::vector<Field> res;
        for (uint i = 0; i < M; ++i) {
            res.push_back(matrix_[i][j]);
        }
        return res;
    }
    const RowMatrix<const Field> operator[](unsigned i) const {
        return RowMatrix<const Field>(matrix_[i]);
    }
    RowMatrix<Field> operator[](unsigned i) {
        return RowMatrix<Field>(matrix_[i]);
    }
    void print() const {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                std::cerr << matrix_[i][j].toString() << " ";
            }
            std::cout << std::endl;
        }
    }
    void printForFinite() const {
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                std::cout << matrix_[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    template <unsigned H, unsigned W>
    void fillFromMatrix(const Matrix<H, W, Field>& a, unsigned up, unsigned left) {
        for (uint i = 0; i < H; ++i) {
            for (uint j = 0; j < W; ++j) {
                matrix_[i + up][j + left] = a[i][j];
            }
        }
    }
    bool operator==(const Matrix& a) const {
        bool isEqual = true;
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < N; ++j) {
                if (matrix_[i][j] != a[i][j]) {
                    isEqual = false;
                }
            }
        }
        return isEqual;
    }
    bool operator!=(const Matrix& a) const {
        return !(*this == a);
    }
};

template <unsigned M, unsigned N, typename Field = Rational>
const Matrix<M, N, Field> operator+(const Matrix<M, N, Field>& a, const Matrix<M, N, Field>& b) {
    Matrix<M, N, Field> tmp = a;
    tmp += b;
    return tmp;
}
template <unsigned M, unsigned N, typename Field = Rational>
const Matrix<M, N, Field> operator-(const Matrix<M, N, Field>& a, const Matrix<M, N, Field>& b) {
    Matrix<M, N, Field> tmp = a;
    tmp -= b;
    return tmp;
}
template <unsigned M, unsigned N, typename Field = Rational>
const Matrix<M, N, Field> operator*(const Matrix<M, N, Field>& a, const Field& k) {
    Matrix<M, N, Field> tmp = a;
    tmp *= k;
    return tmp;
}
template <unsigned M, unsigned N, typename Field = Rational>
const Matrix<M, N, Field> operator*(const Field& k, const Matrix<M, N, Field>& a) {
    Matrix<M, N, Field> tmp = a;
    tmp *= k;
    return tmp;
}
#include <algorithm>
template <unsigned M, unsigned N, unsigned K, typename Field = Rational>
const Matrix<M, K, Field> operator*(const Matrix<M, N, Field>& a, const Matrix<N, K, Field>& b) {
    //naive algorithm
    if (M <= >= 16 || N <= 16 || K <= 16) {
        Matrix<M, K, Field> res = Matrix<M, K, Field>::getZeroMatrix();
        for (uint i = 0; i < M; ++i) {
            for (uint j = 0; j < K; ++j) {
                for (uint t = 0; t < N; ++t) {
                    res[i][j] += a[i][t] * b[t][j];
                }
            }
        }
        return res;
    }
    //make matrix with n, m, k : 2; Supplemented
    const unsigned mUp = M + M % 2;
    const unsigned nUp = N + N % 2;
    const unsigned kUp = K + K % 2;
    Matrix<mUp, nUp, Field> aSupplemented(a, 0, 0);
    Matrix<nUp, kUp, Field> bSupplemented(b, 0, 0);
    //Cut into pieces of the matrix
    const unsigned mSmall = mUp / 2;
    const unsigned nSmall = nUp / 2;
    const unsigned kSmall = kUp / 2;
    Matrix<mSmall, nSmall, Field> a11(aSupplemented, 0, 0);
    Matrix<mSmall, nSmall, Field> a12(aSupplemented, 0, nUp / 2);
    Matrix<mSmall, nSmall, Field> a21(aSupplemented, mUp / 2, 0);
    Matrix<mSmall, nSmall, Field> a22(aSupplemented, mUp / 2, nUp / 2);

    Matrix<nSmall, kSmall, Field> b11(bSupplemented, 0, 0);
    Matrix<nSmall, kSmall, Field> b12(bSupplemented, 0, kUp / 2);
    Matrix<nSmall, kSmall, Field> b21(bSupplemented, nUp / 2, 0);
    Matrix<nSmall, kSmall, Field> b22(bSupplemented, nUp / 2, kUp / 2);

    //Strassen algorithm
    Matrix<mSmall, kSmall, Field> P1 = a11 * (b12 - b22);
    Matrix<mSmall, kSmall, Field> P2 = (a11 + a12) * b22;
    Matrix<mSmall, kSmall, Field> P3 = (a21 + a22) * b11;
    Matrix<mSmall, kSmall, Field> P4 = a22 * (b21 - b11);
    Matrix<mSmall, kSmall, Field> P5 = (a11 + a22) * (b11 + b22);
    Matrix<mSmall, kSmall, Field> P6 = (a12 - a22) * (b21 + b22);
    Matrix<mSmall, kSmall, Field> P7 = (a11 - a21) * (b11 + b12);

    Matrix<mUp, kUp, Field> res = Matrix<mUp, kUp, Field>::getZeroMatrix();
    res.fillFromMatrix(P5 + P4 - P2 + P6, 0, 0);
    res.fillFromMatrix(P1 + P2, 0, kSmall);
    res.fillFromMatrix(P3 + P4, mSmall, 0);
    res.fillFromMatrix(P1 + P5 - P3 - P7, mSmall, kSmall);

    Matrix<M, K, Field> ans(res, 0, 0);
    return ans;
}

template <unsigned Size, typename Field = Rational>
using SquareMatrix = Matrix<Size, Size, Field>;
