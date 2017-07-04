//https://github.com/Ilya1510/Deque
#include <iostream>
#include <algorithm>
#include <stdio.h>

template <class T>
class Deque;

template <class T, class Pointer, class Reference>
class IteratorDeque : public std::iterator<std::random_access_iterator_tag, T, std::ptrdiff_t, Pointer, Reference> {
private:
    const Deque<T>* dequeReference;
    size_t index_;
public:
    typedef IteratorDeque <T, Pointer, Reference> Iterator;
    IteratorDeque() :dequeReference(nullptr), index_(0) {}
    IteratorDeque(const Iterator& a)
        :dequeReference(a.dequeReference) {
        index_ = a.index_;
    }
    IteratorDeque(const Deque<T>* deque, size_t index)
        :dequeReference(deque), index_(index) {}
    Iterator& operator=(const Iterator& a) {
        dequeReference = a.dequeReference;
        index_ = a.index_;
        return *this;
    }
    size_t index() const {
        return index_;
    }
    Iterator& operator++() {
        index_ = (index_ + 1) % dequeReference->sizeBuffer_;
        return *this;
    }
    const Iterator operator++(int) {
        Iterator tmp(dequeReference, index_);
        ++*this;
        return tmp;
    }
    Iterator& operator--() {
        index_ = (index_ + dequeReference->sizeBuffer_ - 1) % dequeReference->sizeBuffer_;
        return *this;
    }
    const Iterator operator--(int) {
        Iterator tmp(dequeReference, index_);
        --*this;
        return tmp;
    }
    
    Iterator& operator+=(int n) {
        index_ = (index_ + n) % dequeReference->sizeBuffer_;
        return *this;
    }
    Iterator& operator-=(int n) {
        index_ = (index_ - n % dequeReference->sizeBuffer_ +
            dequeReference->sizeBuffer_) % dequeReference->sizeBuffer_;
        return *this;
    }
    const Iterator operator+(int n) const {
        Iterator tmp = *this;
        tmp += n;
        return tmp;
    }
    const Iterator operator-(int n) const {
        Iterator tmp = *this;
        tmp -= n;
        return tmp;
    }
    int operator-(const Iterator& a) const {
        int firstIndex = static_cast<int>(index()) + (index() < dequeReference->left_ ? dequeReference->sizeBuffer_ : 0);
        int secondIndex = static_cast<int>(a.index()) + (a.index() < dequeReference->left_ ? dequeReference->sizeBuffer_ : 0);
        return firstIndex - secondIndex;
    }
    bool operator==(const Iterator& a) const {
        return (dequeReference == dequeReference && index() == a.index());
    }
    bool operator!=(const Iterator& a) const {
        return !(*this == a);
    }
    bool operator<(const Iterator& a) const {
        return (dequeReference < dequeReference) ||
            (dequeReference == dequeReference && index() < a.index());
    }
    bool operator>(const Iterator &a) const {
        return a < *this;
    }
    bool operator>=(const Iterator &a) const {
        return !(*this < a);
    }
    bool operator<=(const Iterator &a) const {
        return !(a < *this);
    }
    Reference operator[](int n) const {
        std::cerr << n << " " << dequeReference->sizeBuffer_ << " " << index_ << std::endl;
        return *(*this + n);
    }
    Reference operator*() const {
        return dequeReference->arr_[index_];
    }
    Pointer operator->() const {
        return &**this;
    }
};
template <class T, class Pointer, class Reference>
const IteratorDeque<T, Pointer, Reference> operator+(int n, const IteratorDeque<T, Pointer, Reference>& a) {
    return a + n;
}
template <class T>
class Deque {
private:
    T* arr_;
    // element consist in [l..r)
    size_t left_, right_;
    size_t sizeBuffer_;
    size_t size_;
    static const size_t minSize_ = 4;

    void changeBuffer(size_t sizeBuffer) {
        T* newBuffer = new T[sizeBuffer];
        size_t index = 0;
        for (size_t i = left_; !index || i != right_; i = (i + 1) % sizeBuffer_) {
            newBuffer[index++] = arr_[i];
        }
        left_ = 0;
        right_ = size_;
        delete arr_;
        arr_ = newBuffer;
        sizeBuffer_ = sizeBuffer;
    }
    void expand() {
        changeBuffer(static_cast<size_t>(sizeBuffer_ * 2));
    }
    void shrink() {
        if (sizeBuffer_ > 4) {
            changeBuffer(static_cast<size_t>(1.0 * sizeBuffer_ / 2));
        }
    }

public:
    void print() const {
        if (sizeBuffer_) {
            size_t index = 0;
            for (size_t i = left_; index == 0 || i != right_; i = (i + 1) % sizeBuffer_) {
                std::cout << arr_[i] << " ";
                index++;
            }
        }
        std::cout << std::endl;
    }
    Deque<T>(const Deque<T> & a)
            :arr_(new T[a.sizeBuffer_]) {
        size_ = a.size_;
        sizeBuffer_ = a.sizeBuffer_;
        left_ = a.left_;
        right_ = a.right_;
        std::copy(a.arr_, a.arr_ + sizeBuffer_, arr_);
    }
    Deque<T>() :arr_(new T[minSize_]) {
        size_ = 0;
        left_ = 0;
        right_ = 0;
        sizeBuffer_ = minSize_;
    }
    /*Deque<T>& operator=(const Deque<T> & a) {
        if (this != &a) {
            delete arr_;
            arr_ = new T[a.size()];
            size_ = a.size_;
            sizeBuffer_ = a.sizeBuffer_;
            left_ = a.left_;
            right_ = a.right_;
            std::copy(a.arr_, a.arr_ + sizeBuffer_);
        }
    }*/
    const T& back() const {
        return right_ ? arr_[right_ - 1] : arr_[sizeBuffer_ - 1];
    }
    T& back() {
        return right_ ? arr_[right_ - 1] : arr_[sizeBuffer_ - 1];
    }
    const T& front() const {
        return arr_[left_];
    }
    T& front() {
        return arr_[left_];
    }
    const T& operator[](size_t i) const {
        return arr_[(i + left_) % sizeBuffer_];
    }
    T& operator[](size_t i) {
        return arr_[(i + left_) % sizeBuffer_];
    }
    bool empty() const {
        return !size_;
    }
    size_t size() const {
        return size_;
    }
    void push_back(const T& a) {
        if (size_ == sizeBuffer_) {
            expand();
        }
        arr_[right_] = a;
        right_ = (right_ + 1) % sizeBuffer_;
        ++size_;
        if (size_ == sizeBuffer_) {
            expand();
        }
    }
    void pop_back() {
        right_ = (right_ + sizeBuffer_ - 1) % sizeBuffer_;
        --size_;
        if (size_ < 1.0 * sizeBuffer_ / 4) {
            shrink();
        }
    }
    void push_front(const T& a) {
        if (size_ == sizeBuffer_) {
            expand();
        }
        left_ = (left_ + sizeBuffer_ - 1) % sizeBuffer_;
        arr_[left_] = a;
        ++size_;
        if (size_ == sizeBuffer_) {
            expand();
        }
    }
    void pop_front() {
        left_ = (left_ + 1) % sizeBuffer_;
        --size_;
        if (size_ < 1.0 * sizeBuffer_ / 4) {
            shrink();
        }
    }

    template <class Type, class Pointer, class Reference>
    friend class IteratorDeque;

    typedef IteratorDeque<T, T*, T&> iterator;
    typedef IteratorDeque<T, const T*, const T&> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    const iterator begin() {
        return iterator(this, left_);
    }
    const const_iterator begin() const {
        return const_iterator(this, left_);
    }
    const const_iterator cbegin() const {
        return const_iterator(this, left_);
    }
    const iterator end() {
        return iterator(this, right_);
    }
    const const_iterator end() const {
        return const_iterator(this, right_);
    }
    const const_iterator cend() const {
        return const_iterator(this, right_);
    }
    const reverse_iterator rbegin() {
        return reverse_iterator(iterator(this, right_));
    }
    const const_reverse_iterator rbegin() const {
        return const_reverse_iterator(const_iterator(this, right_));
    }
    const const_reverse_iterator crbegin() const {
        return const_reverse_iterator(const_iterator(this, right_));
    }
    const reverse_iterator rend() {
        return reverse_iterator(iterator(this, left_));
    }
    const const_reverse_iterator rend() const {
        return const_reverse_iterator(const_iterator(this, left_));
    }
    const const_reverse_iterator crend() const {
        return const_reverse_iterator(const_iterator(this, left_));
    }
};