#include <iostream>
#include <vector>
#include <string>

namespace numbers {

    using std::vector;
    using std::string;
    using std::cin;
    using std::cout;
    typedef unsigned uint;

    template <class T>
    void mySwap(T &a, T &b) {
        T t = a;
        a = b;
        b = t;
    }

    template <class T>
    void reverse(T begin, T end) {
        --end;
        while (begin < end) {
            mySwap(*begin, *end);
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
        static const long long MOD = 1000000000;
        vector <long long> digits;
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
            for (unsigned int i = 0; i < len; ++i) {
                if (abs(digits[i]) >= MOD) {
                    if (i + 1 == len) {
                        digits.push_back(0);
                        ++len;
                    }
                    digits[i + 1] += digits[i] / MOD;
                    digits[i] %= MOD;
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
                    digits[i] += MOD;
                }
            }
            while (!digits.back()) {
                digits.pop_back();
                --len;
            }
            numbers::reverse(digits.begin(), digits.end());
        }
        void pushDigit(size_t ind) {
            if (digits[ind] >= MOD) {
                digits[ind - 1] += digits[ind] / MOD;
                digits[ind] %= MOD;
            }
        }
        BigInteger(const vector <long long> &x, bool negative) {
            digits = x;
            len = digits.size();
            isNegative = negative;
        }
        void changeSign() {
            if (*this != 0) {
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
    public:
        BigInteger() {
            *this = 0;
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
                digits.push_back(d % MOD);
                d /= MOD;
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
            for (unsigned int i = cur; i < s.size(); ++i) {
                digits.push_back(s[i] - '0');
            }
            len = digits.size();
        }

        int getLen() const {
            return len;
        }
        int getDigit(int ind) const {
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
                long long div = MOD / 10;
                long long curDigit = digits[i];
                while (div) {
                    res += '0' + curDigit / div;
                    div /= 10;
                    curDigit -= div * (curDigit / div);
                }
            }
            numbers::reverse(res.begin(), res.end());
            while (res.back() == '0') {
                res.pop_back();
            }
            numbers::reverse(res.begin(), res.end());
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
            int beginLen = len;
            addZeros(numberToMultiply.getLen());
            int order1 = beginLen - 1;
            // important that we multiply the first high-order bits, so we save memory
            for (unsigned int i = len - beginLen; i < len; ++i, --order1) {
                long long curDigit = digits[i];
                for (int j = 0, order2 = numberToMultiply.getLen() - 1; j < numberToMultiply.getLen(); ++j, --order2) {
                    digits[i - order2] += curDigit * numberToMultiply.getDigit(j);
                    pushDigit(i - order2);
                }
                // We should sub begin number, as the record to the new array
                digits[i] -= curDigit;
            }
            isNegative = numberToMultiply.isPositive() != isPositive();
            toRightForm();
            return *this;
        }
        BigInteger& operator/=(const BigInteger& numberToDivide) {
            bool isResNegative = isPositive() ^ numberToDivide.isPositive();
            BigInteger divPositive = BigInteger(numberToDivide.digits, false);
            int zerosCount = 0;
            isNegative = false;
            while (*this >= divPositive) {
                divPositive *= MOD;
                ++zerosCount;
            }
            vector <long long> res;
            while (zerosCount >= 0) {
                long long count = 0;
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
            return *this += 1;
        }
        BigInteger& operator--() {
            return *this -= 1;
        }
        const BigInteger operator++(int) {
            BigInteger pred = *this;
            *this += 1;
            return pred;
        }
        const BigInteger operator--(int) {
            BigInteger pred = *this;
            *this -= 1;
            return pred;
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
        while (a && b) {
            if (a > b) {
                a %= b;
            }
            else {
                b %= a;
            }
        }
        return a + b;
    }

}

using numbers::BigInteger;
using numbers::gcd;