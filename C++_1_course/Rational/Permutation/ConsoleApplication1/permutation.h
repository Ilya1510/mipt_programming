#include <iostream>

namespace permutation {

    using std::cin;
    using std::cout;

    size_t MergeSort(int *start, int *finish, int *buffer);

    template <typename T>
    void swap(T &a, T &b) {
        T t = a;
        a = b;
        b = t;
    }

    template <class BiDirectionalIterator>
    void reverse(BiDirectionalIterator begin, BiDirectionalIterator end) {
        --end;
        while (begin < end) {
            swap(*begin, *end);
            ++begin;
            --end;
        }
    }

    class Permutation{
    private:
        int* per_;
        size_t size_;

        const Permutation splitCycles() const {
            int *res = new int[size_];
            bool *used = new bool[size_];
            for (size_t i = 0; i < size_; ++i) {
                used[i] = 0;
            }
            size_t ind = 0;
            for (size_t i = 0; i < size_; ++i) {
                if (!used[i]) {
                    size_t v = i;
                    used[v] = true;
                    while (per_[v] != static_cast<int>(i)) {
                        res[ind++] = v;
                        v = per_[v];
                        used[v] = true;
                    }
                    //special mark for end cycle
                    res[ind++] = v + size_;
                }
            }
            Permutation ans = Permutation(size_, res);
            delete[] res;
            delete[] used;
            return ans;
        }

        void print() const {
            for (size_t i = 0; i < size_; ++i) {
                cout << per_[i] << " ";
            }
            cout << '\n';
        }
        void changeByOne(bool increase) {
            int toAdd = increase ? 1 : -1;
            size_t bord = size_ - 1;
            while (bord > 0 && (per_[bord - 1] - per_[bord]) * toAdd > 0) {
                --bord;
            }
            if (!bord) {
                return;
            }
            reverse(per_ + bord, per_ + size_);
            --bord;
            for (size_t i = bord + 1; i < size_; ++i) {
                if ((per_[i] - per_[bord]) * toAdd > 0) {
                    swap(per_[bord], per_[i]);
                    break;
                }
            }
        }
    public:
        Permutation(const Permutation& a)
                :per_(new int[a.size_]), size_(a.size_) {
            for (size_t i = 0; i < a.size_; ++i) {
                per_[i] = a[i];
            }
        }
        explicit Permutation(unsigned int n)
                :per_(new int[n]), size_(n) {
            for (unsigned int i = 0; i < n; ++i) {
                per_[i] = i;
            }
        }
        Permutation(unsigned int n, int *arr)
                :per_(new int[n]), size_(n) {
            for (unsigned int i = 0; i < n; ++i) {
                per_[i] = arr[i];
            }
        }
        Permutation& operator=(const Permutation &a) {
            if (this != &a) {
                delete[] per_;
                size_ = a.size_;
                per_ = new int[a.size_];
                for (size_t i = 0; i < a.size_; ++i) {
                    per_[i] = a[i];
                }
            }
            return *this;
        }
        ~Permutation() {
            delete[] per_;
        }
        size_t getLen() const {
            return size_;
        }
        int operator[](int i) const {
            return per_[i];
        }
        Permutation& operator*=(const Permutation& a) {
            int* comp = new int[size_];
            for (size_t i = 0; i < size_; ++i) {
                comp[i] = a[per_[i]];
            }
            for (size_t i = 0; i < size_; ++i) {
                per_[i] = comp[i];
            }
            delete[] comp;
            return *this;
        }
        Permutation& operator++() {
            changeByOne(true);
            return *this;
        }
        const Permutation operator++(int) {
            Permutation pred = *this;
            ++*this;
            return pred;
        }
        Permutation& operator--() {
            changeByOne(false);
            return *this;
        }
        const Permutation operator--(int) {
            Permutation pred = *this;
            --*this;
            return pred;
        }
        const Permutation next() const {
            Permutation res = *this;
            ++res;
            return res;
        }
        const Permutation previous() const {
            Permutation res = *this;
            --res;
            return res;
        }
        const Permutation inverse() const {
            Permutation res = *this;
            for (size_t i = 0; i < size_; ++i) {
                res.per_[per_[i]] = i;
            }
            return res;
        }
        void operator()(int* arr) const {
            int *res = new int[size_];
            for (size_t i = 0; i < size_; ++i) {
                res[per_[i]] = arr[i];
            }
            for (size_t i = 0; i < size_; ++i) {
                arr[i] = res[i];
            }
            delete[] res;
        }
        size_t derangementsCount() const {
            Permutation tmp = *this;
            int* buffer = new int[size_];
            size_t res = MergeSort(tmp.per_, tmp.per_ + size_, buffer);
            delete[] buffer;
            return res;
        }

        bool isOdd() const {
            Permutation cycles = splitCycles();
            size_t evenCyclesCount = 0;
            size_t curCycle = 0;
            for (size_t i = 0; i < size_; ++i) {
                ++curCycle;
                if (cycles[i] >= static_cast<int>(size_)) {
                    evenCyclesCount += 1 - curCycle % 2;
                    curCycle = 0;
                }
            }
            return evenCyclesCount % 2;
        }
        bool isEven() const {
            return !isOdd();
        }
        const Permutation pow(int degree) const {
            Permutation cycles = splitCycles();
            int *res = new int[size_];
            for (size_t i = 0; i < size_; ++i) {
                size_t l = i;
                while (cycles[i] < static_cast<int>(size_)) {
                    ++i;
                }
                size_t r = i;
                int cyclesCount = r - l + 1;
                for (size_t j = l; j <= r; ++j) {
                    size_t pred = (j - l + cyclesCount + degree % cyclesCount) % cyclesCount;
                    res[cycles[j] % size_] = cycles[l + pred] % size_;
                }
            }
            Permutation ans = Permutation(size_, res);
            delete[] res;
            return ans;
        }
    };

    const Permutation operator*(const Permutation& a, const Permutation& b) {
        Permutation res = a;
        res *= b;
        return res;
    }

    bool operator==(const Permutation& a, const Permutation& b) {
        if (a.getLen() != b.getLen()) {
            return false;
        }
        for (size_t i = 0; i < a.getLen(); ++i) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Permutation& a, const Permutation& b) {
        return !(a == b);
    }
    bool operator<(const Permutation& a, const Permutation& b) {
        size_t minLen = a.getLen() < b.getLen() ? a.getLen() : b.getLen();
        for (size_t i = 0; i < minLen; ++i) {
            if (a[i] < b[i]) {
                return true;
            }
            else if (a[i] > b[i]) {
                return false;
            }
        }
        return a.getLen() < b.getLen();
    }
    bool operator>(const Permutation& a, const Permutation& b) {
        return b < a;
    }
    bool operator<=(const Permutation& a, const Permutation& b) {
        return !(b < a);
    }
    bool operator>=(const Permutation& a, const Permutation& b) {
        return !(a < b);
    }

    size_t MergeSort(int *start, int *finish, int *buffer) {
        if (finish - start <= 1) {
            return 0;
        }
        int *middle = start + (finish - start) / 2;
        size_t ans = 0;
        ans += MergeSort(start, middle, buffer);
        ans += MergeSort(middle, finish, buffer);
        int *ind1 = start;
        int *ind2 = middle;
        size_t count = finish - start;
        for (size_t i = 0; i < count; ++i) {
            if (ind1 == middle) {
                buffer[i] = *ind2++;
                ans += middle - ind1;
            }
            else if (ind2 == finish) {
                buffer[i] = *ind1++;
            }
            else if (*ind1 < *ind2) {
                buffer[i] = *ind1++;
            }
            else {
                buffer[i] = *ind2++;
                ans += middle - ind1;
            }
        }
        for (size_t i = 0; i < count; ++i) {
            start[i] = buffer[i];
        }
        return ans;
    }

}

using permutation::Permutation;