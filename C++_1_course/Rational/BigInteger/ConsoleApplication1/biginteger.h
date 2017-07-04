#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cin;
using std::cout;

class BigInteger;

bool operator<(const BigInteger & a, const BigInteger& b);
bool operator>(const BigInteger& a, const BigInteger& b);
bool operator==(const BigInteger& a, const BigInteger& b);
bool operator!=(const BigInteger& a, const BigInteger& b);
bool operator<=(const BigInteger& a, const BigInteger& b);
bool operator>=(const BigInteger& a, const BigInteger& b);
BigInteger operator+(const BigInteger& a, const BigInteger& b);
BigInteger operator-(const BigInteger& a, const BigInteger& b);
BigInteger operator*(const BigInteger& a, const BigInteger& b);
BigInteger operator/(const BigInteger& a, const BigInteger& b);
BigInteger operator%(const BigInteger& a, const BigInteger& b);

class BigInteger{
private:
    vector <int> digits;
    unsigned int len;
    bool neg;
    void swap(int &a, int &b) {
        int t = a;
        a = b;
        b = t;
    }
    int min(int a, int b) {
        return a < b ? a : b;
    }
    void reverse() {
        for (unsigned int i = 0; i < len / 2; ++i) {
            swap(digits[i], digits[len - i - 1]);
        }
    }
    void addZeros(int count) {
        if (count < 0) {
            return;
        }
        reverse();
        for (int i = 0; i < count; ++i) {
            digits.push_back(0);
        }
        len += count;
        reverse();
    }
    void toRightForm() { //! function which make all digits 0..9
        reverse();
        for (unsigned int i = 0; i < len; ++i) {
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
            neg = !neg;
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
        reverse();
    }
    void pushDigit(int ind) {
        if (digits[ind] > 9) {
            digits[ind - 1] += digits[ind] / 10;
            digits[ind] %= 10;
        }
    }
    BigInteger(const vector <int> &x, bool negative) {
        digits = x;
        len = digits.size();
        neg = negative;
    }
    void changeSign() {
        if (*this != 0) {
            neg = !neg;
        }
    }
public:
    BigInteger() {
        *this = 0;
    }
    BigInteger(int d) {
        neg = d >= 0 ? false : true;
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
        reverse();
    }
    BigInteger(string s) {
        if (s == "") {
            *this = 0;
            return;
        }
        int cur = 0;
        if (s[0] == '-') {
            neg = true;
            ++cur;
        }
        else {
            neg = false;
        }
        for (unsigned int i = cur; i < s.size(); ++i) {
            digits.push_back(s[i] - '0');
        }
        len = digits.size();
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
    int getLen() const {
        return len;
    }
    int getDigit(int ind) const {
        return digits[ind];
    }
    int getSign() const {
        return neg == true ? -1 : 1;
    }
    string toString() const {
        string res;
        if (neg) {
            res.push_back('-');
        }
        for (unsigned int i = 0; i < len; ++i) {
            res += '0' + digits[i];
        }
        return res;
    }
    BigInteger operator-() const {
        BigInteger res = *this;
        res *= -1;
        return res;
    }
    BigInteger& operator+=(const BigInteger& add) {
        addZeros(add.getLen() - len);
        int lenAdd = add.getLen();
        int ind1 = len - 1, ind2 = lenAdd - 1;
        int signAdd = add.getSign() == getSign() ? 1 : -1;
        for (int i = 0; i < lenAdd; ++i, --ind1, --ind2) {
            digits[ind1] += add.getDigit(ind2) * signAdd;
        }
        toRightForm();
        return *this;
    }
    BigInteger& operator-=(const BigInteger& sub) {
        neg = !neg;
        *this += sub;
        if (*this) { //number is not zero
            neg = !neg;
        }
        return *this;
    }
    BigInteger& operator*=(const BigInteger& fact) {
        BigInteger fact_tmp = fact;
        int beginLen = len;
        addZeros(fact_tmp.getLen());
        int order1 = beginLen - 1;
        for (unsigned int i = len - beginLen; i < len; ++i, --order1) {
            // important that we multiply the first high-order bits, so we save memory
            int cur_digit = digits[i];
            for (int j = 0, order2 = fact_tmp.getLen() - 1; j < fact_tmp.getLen(); ++j, --order2) {
                digits[i - order2] += cur_digit * fact_tmp.getDigit(j);
                pushDigit(i - order2);
            }
            digits[i] -= cur_digit; // We should sub begin number, as the record to the new array
        }
        neg = fact_tmp.getSign() != getSign();
        toRightForm();
        return *this;
    }
    BigInteger& operator/=(const BigInteger& div) {
        int sign = getSign() * div.getSign();
        BigInteger divPositive = BigInteger(div.digits, false);
        int zerosCount = 0;
        this->neg = false;
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
        *this = BigInteger(res, sign == -1);
        this->toRightForm();
        return *this;
    }
    BigInteger& operator%=(const BigInteger& div) {
        *this -= (*this / div) * div;
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
    int operator[](unsigned int ind) const {
        if (ind < len) {
            return digits[ind];
        }
        return -1;
    }
    explicit operator bool() const {
        if (len == 1 && digits[0] == 0) {
            return false;
        }
        return true;
    }
};

bool operator<(const BigInteger& a, const BigInteger& b) {
    if (a.getSign() < b.getSign()) {
        return true;
    }
    else if (a.getSign() > b.getSign()) {
        return false;
    }
    if (a.getLen() < b.getLen())
        return a.getSign() == 1;
    if (a.getLen() > b.getLen())
        return a.getSign() == -1;
    int len = a.getLen();
    for (int i = 0; i < len; ++i) {
        if (a[i] < b[i]) {
            return a.getSign() == 1;
        }
        else if (a[i] > b[i]) {
            return a.getSign() == -1;
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
    if (a.getSign() != b.getSign()) {
        return false;
    }
    int len = a.getLen();
    for (int i = 0; i < len; i++) {
        if (a[i] != b[i]) {
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

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
    BigInteger sum = a;
    sum += b;
    return sum;
}
BigInteger operator*(const BigInteger& a, const BigInteger& b) {
    BigInteger res = a;
    res *= b;
    return res;
}
BigInteger operator-(const BigInteger& a, const BigInteger& b) {
    BigInteger res = a;
    res -= b;
    return res;
}
BigInteger operator/(const BigInteger& a, const BigInteger& b) {
    BigInteger res = a;
    res /= b;
    return res;
}
BigInteger operator%(const BigInteger& a, const BigInteger& b) {
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