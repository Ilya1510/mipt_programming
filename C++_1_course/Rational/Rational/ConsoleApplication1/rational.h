#include <iostream>
#include <vector>
#include <string>

namespace numbers {

    using std::vector;
    using std::string;
    using std::cin;
    using std::cout;

    typedef unsigned int uint;

    template <class T>
    void Swap(T &a, T &b) {
        T t = a;
        a = b;
        b = t;
    }

    template <class BidirectionalIterator>
    void reverse(BidirectionalIterator begin, BidirectionalIterator end) {
        --end;
        while (begin < end) {
            Swap(*begin, *end);
            ++begin;
            --end;
        }
    }

    class BigInteger;

    bool operator<(const BigInteger & a, const BigInteger& b);
    bool operator>(const BigInteger& a, const BigInteger& b);
    bool operator==(const BigInteger& a, const BigInteger& b);
    bool operator!=(const BigInteger& a, const BigInteger& b);
    bool operator<=(const BigInteger& a, const BigInteger& b);
    bool operator>=(const BigInteger& a, const BigInteger& b);
    const BigInteger operator+(const BigInteger& a, const BigInteger& b);
    const BigInteger operator-(const BigInteger& a, const BigInteger& b);
    const BigInteger operator*(const BigInteger& a, const BigInteger& b);
    const BigInteger operator/(const BigInteger& a, const BigInteger& b);
    const BigInteger operator%(const BigInteger& a, const BigInteger& b);

    class BigInteger{
    private:
        vector <int> digits;
        uint len;
        bool isNegative;

        void addZeros(int count) {
            if (count < 0) {
                return;
            }
            numbers::reverse(digits.begin(), digits.end());
            for (int i = 0; i < count; ++i) {
                digits.push_back(0);
            }
            len += count;
            numbers::reverse(digits.begin(), digits.end());
        }
        //! function which make all digits 0..9, make number correct
        void toRightForm() {
            numbers::reverse(digits.begin(), digits.end());
            for (uint i = 0; i < len; ++i) {
                if (abs(digits[i]) >= 10) {
                    if (i + 1 == len) {
                        digits.push_back(0);
                        ++len;
                    }
                    digits[i + 1] += digits[i] / 10;
                    digits[i] %= 10;
                }
            }
            int indMostDigit = len - 1;
            while (indMostDigit >= 0 && !digits[indMostDigit]) {
                --indMostDigit;
            }
            if (indMostDigit == -1) {
                *this = 0;
                return;
            }
            if (digits[indMostDigit] < 0) {
                isNegative = !isNegative;
                for (int i = 0; i <= indMostDigit; ++i) {
                    digits[i] *= -1;
                }
            }
            for (int i = 0; i <= indMostDigit; ++i) {
                if (digits[i] < 0) {
                    digits[i + 1] -= 1;
                    digits[i] += 10;
                }
            }
            while (!digits.back()) {
                digits.pop_back();
                --len;
            }
            numbers::reverse(digits.begin(), digits.end());
        }
        void pushDigit(size_t ind) {
            if (digits[ind] > 9) {
                if (ind == 0) {
                    numbers::reverse(digits.begin(), digits.end());
                    digits.push_back(digits.back() / 10);
                    digits[digits.size() - 2] %= 10;
                    len++;
                    numbers::reverse(digits.begin(), digits.end());
                }
                else {
                    digits[ind - 1] += digits[ind] / 10;
                    digits[ind] %= 10;
                }
            }
        }
        BigInteger(const vector <int> &x, bool isNegativeNumber) {
            digits = x;
            len = digits.size();
            isNegative = isNegativeNumber;
        }
        void changeSign() {
            if (!*this) {
                isNegative = !isNegative;
            }
        }
        void popDigitEnd() {
            if (*this) {
                digits.pop_back();
                if (digits.size() == 0) {
                    digits.push_back(0);
                }
                else {
                    --len;
                }
            }
            toRightForm();
        }
        void toNext() {
            if (!*this) {
                *this = 1;
                return;
            }
            digits.back() += isNegative ? -1 : 1;
            for (int i = len - 1; i >= 0 && digits[i] == 10; --i) {
                pushDigit(i);
            }
        }

        void toPrevious() {
            if (!*this) {
                *this = -1;
            }
            digits.back() += isNegative ? 1 : -1;
            for (int i = len - 1; i >= 0 && digits[i] == 10; --i) {
                pushDigit(i);
            }
        }
    public:
        BigInteger() : len(1), isNegative(false) {
            digits.push_back(0);
        }
        BigInteger(int d) {
            isNegative = !(d >= 0);
            digits.clear();
            d = abs(d);
            if (d == 0) {
                digits.push_back(0);
                len = 1;
                return;
            }
            while (d) {
                digits.push_back(d % 10);
                d /= 10;
            }
            len = digits.size();
            numbers::reverse(digits.begin(), digits.end());
        }
        BigInteger(const string& s) {
            if (s == "") {
                *this = 0;
                return;
            }
            int cur = 0;
            if (s[0] == '-') {
                isNegative = true;
                ++cur;
            }
            else {
                isNegative = false;
            }
            for (uint i = cur; i < s.size(); ++i) {
                digits.push_back(s[i] - '0');
            }
            len = digits.size();
        }

        int getLen() const {
            return static_cast<int>(len);
        }
        int getDigit(uint ind) const {
            return digits[ind];
        }
        bool isPositive() const {
            return !isNegative;
        }
        string toString() const {
            string res;
            if (isNegative) {
                res.push_back('-');
            }
            for (uint i = 0; i < len; ++i) {
                res += '0' + digits[i];
            }
            return res;
        }
        const BigInteger operator-() const {
            BigInteger res = *this;
            res *= -1;
            return res;
        }
        BigInteger& operator+=(const BigInteger& numberToAdd) {
            addZeros(numberToAdd.getLen() - len);
            int lenAdd = numberToAdd.getLen();
            int ind1 = len - 1, ind2 = lenAdd - 1;
            int signAdd = numberToAdd.isPositive() == isPositive() ? 1 : -1;
            for (int i = 0; i < lenAdd; ++i, --ind1, --ind2) {
                digits[ind1] += numberToAdd.getDigit(ind2) * signAdd;
            }
            toRightForm();
            return *this;
        }
        BigInteger& operator-=(const BigInteger& numberToSub) {
            isNegative = !isNegative;
            *this += numberToSub;
            //number is not zero
            if (*this) {
                isNegative = !isNegative;
            }
            return *this;
        }
        BigInteger& operator*=(const BigInteger& numberToMultiply) {
            BigInteger numberToMultiplyCopy = numberToMultiply;
            int beginLen = len;
            addZeros(numberToMultiplyCopy.getLen());
            int order1 = beginLen - 1;
            // important that we multiply the first high-order bits, so we save memory
            for (unsigned int i = len - beginLen; i < len; ++i, --order1) {
                int curDigit = digits[i];
                for (int j = 0, order2 = numberToMultiplyCopy.getLen() - 1; j < numberToMultiplyCopy.getLen(); ++j, --order2) {
                    digits[i - order2] += curDigit * numberToMultiplyCopy.getDigit(j);
                    pushDigit(i - order2);
                }
                // We should sub begin number, as the record to the new array
                digits[i] -= curDigit;
            }
            isNegative = numberToMultiplyCopy.isPositive() != isPositive();
            toRightForm();
            return *this;
        }
        BigInteger& operator/=(const BigInteger& numberToDivide) {
            bool isResNegative = isPositive() ^ numberToDivide.isPositive();
            BigInteger divPositive = BigInteger(numberToDivide.digits, false);
            int zerosCount = 0;
            isNegative = false;
            while (*this >= divPositive) {
                divPositive *= 10;
                ++zerosCount;
            }
            vector <int> res;
            while (zerosCount >= 0) {
                int count = 0;
                while (*this >= divPositive) {
                    *this -= divPositive;
                    count++;
                }
                res.push_back(count);
                divPositive.popDigitEnd();
                --zerosCount;
            }
            *this = BigInteger(res, isResNegative);
            toRightForm();
            return *this;
        }
        BigInteger& operator%=(const BigInteger& numberToDivide) {
            *this -= (*this / numberToDivide) * numberToDivide;
            return *this;
        }
        BigInteger& operator++() {
            toNext();
            return *this;
        }
        BigInteger& operator--() {
            toPrevious();
            return *this;
        }
        const BigInteger operator++(int) {
            BigInteger cur = *this;
            toNext();
            return cur;
        }
        const BigInteger operator--(int) {
            BigInteger cur = *this;
            toPrevious();
            return cur;
        }
        explicit operator bool() const {
            return len != 1 || digits[0];
        }
    };

    bool operator<(const BigInteger& a, const BigInteger& b) {
        if (a.isPositive() < b.isPositive()) {
            return true;
        }
        else if (a.isPositive() > b.isPositive()) {
            return false;
        }
        if (a.getLen() < b.getLen())
            return a.isPositive();
        if (a.getLen() > b.getLen())
            return !a.isPositive();
        string aStringForm = a.toString();
        string bStringForm = b.toString();
        int eachLen = a.getLen();
        for (int i = 0; i < eachLen; ++i) {
            if (aStringForm[i] < bStringForm[i]) {
                return a.isPositive();
            }
            else if (aStringForm[i] > bStringForm[i]) {
                return !a.isPositive();
            }
        }
        return false;
    }
    bool operator>(const BigInteger& a, const BigInteger& b) {
        return b < a;
    }
    bool operator==(const BigInteger& a, const BigInteger& b) {
        if (a.getLen() != b.getLen())
            return false;
        if (a.isPositive() != b.isPositive()) {
            return false;
        }
        int eachLen = a.getLen();
        string aStringForm = a.toString();
        string bStringForm = b.toString();
        for (int i = 0; i < eachLen; i++) {
            if (aStringForm[i] != bStringForm[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const BigInteger& a, const BigInteger& b) {
        return !(a == b);
    }
    bool operator<=(const BigInteger& a, const BigInteger& b) {
        return !(b < a);
    }
    bool operator>=(const BigInteger& a, const BigInteger& b) {
        return !(a < b);
    }

    const BigInteger operator+(const BigInteger& a, const BigInteger& b) {
        BigInteger sum = a;
        sum += b;
        return sum;
    }
    const BigInteger operator*(const BigInteger& a, const BigInteger& b) {
        BigInteger res = a;
        res *= b;
        return res;
    }
    const BigInteger operator-(const BigInteger& a, const BigInteger& b) {
        BigInteger res = a;
        res -= b;
        return res;
    }
    const BigInteger operator/(const BigInteger& a, const BigInteger& b) {
        BigInteger res = a;
        res /= b;
        return res;
    }
    const BigInteger operator%(const BigInteger& a, const BigInteger& b) {
        BigInteger res = a;
        res %= b;
        return res;
    }

    std::istream& operator>>(std::istream& is, BigInteger &b) {
        string s;
        is >> s;
        b = BigInteger(s);
        return is;
    }
    std::ostream& operator<<(std::ostream& os, const BigInteger &b) {
        os << b.toString();
        return os;
    }

    const BigInteger gcd(BigInteger a, BigInteger b) {
        while (a != 0 && b != 0) {
            if (a > b) {
                a %= b;
            }
            else {
                b %= a;
            }
        }
        return a + b;
    }

    class Rational{
    private:
        BigInteger num, denom;
        void reduceFraction() {
            BigInteger c = 1;
            if (num < 0) {
                c = gcd(-num, denom);
            }
            else {
                c = gcd(num, denom);
            }
            num /= c;
            denom /= c;
        }
    public:
        Rational(const Rational& a) {
            num = a.num;
            denom = a.denom;
            reduceFraction();
        }
        Rational(const BigInteger& num = 0, const BigInteger& denom = 1)
            :num(num), denom(denom) {
            reduceFraction();
        }
        Rational(int num) : num(num), denom(1) {
            reduceFraction();
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
            BigInteger newDenom = denom * numberToAdd.getDenom();
            num = newNum;
            denom = newDenom;
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
            num = newNum;
            denom *= numberToSub.denom;
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

}

using numbers::BigInteger;
using numbers::Rational;