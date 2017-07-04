//we garantee that value >= 0
template <int N>
class Finite {
    int value_;
public:
    Finite() : value(0) {}
    Finite(int value) {
        value_ = (value % N + N) % N;
    }
    int value() const {
        return value_;
    }
    Finite& operator+=(const Finite<N>& a) {
        value_ = (value_ + a.value_) % N;
        return *this;
    }
    Finite& operator-=(const Finite<N>& a) {
        value_ = (value_ + N - a.value_) % N;
        return *this;
    }
    Finite& operator*=(const Finite<N>& a) {
        value_ = (value_ * a.value_) % N;
        return *this;
    }
    Finite& operator/=(const Finite<N>& a) {
        //trivial
        for (int i = 0; i < N - 2; ++i) {
            *this *= a;
        }
        return *this;
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

template<int N>
std::ostream& operator<<(std::ostream &out, const Finite<N>& a) {
    out << a.value();
    return out;
}