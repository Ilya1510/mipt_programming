//
// Created by ilya on 20.05.17.
//
#include <iostream>
#ifndef TUPLE_TUPLE_H
#define TUPLE_TUPLE_H

template <class... Types>
class Tuple;

template <class FirstType, class... Types>
class Tuple<FirstType, Types...> {
    FirstType first;
    Tuple <Types...> tail;
public:
    template <size_t i>
    using TypeAt = typename std::conditional<i == 0, FirstType, typename Tuple<Types...> :: template TypeAt<i - 1> > :: type;

    Tuple() : first(), tail() {}
    template <class... U>
    Tuple(const FirstType& x, U&&... t) : first(x), tail(std::forward<U>(t)...) {}
    template <class... U>
    Tuple(FirstType&& x, U&&... t) : first(std::forward<FirstType>(x)), tail(std::forward<U>(t)...) {}

    Tuple(const Tuple<FirstType, Types...>& other) = default;
    Tuple(Tuple<FirstType, Types...>&& other) = default;
    Tuple& operator=(const Tuple& other) = default;
    Tuple& operator=(Tuple&& other) = default;
    ~Tuple() = default;

    void swap(Tuple& other) {
        std::swap(first, other.first);
        tail.swap(other.tail);
    }

    template <size_t i, class beginType, class... Args>
    friend class GetImpl;
    template <class Type, class beginType, class... Args>
    friend class GetTypeImpl;
    template <class T1, class T2>
    friend class TupleAppendImpl;
    template <class U, class T>
    friend struct TupleAddElemImpl;
    template <class U1, class U2>
    friend struct OperatorCompImpl;
};

template <>
class Tuple<> {
public:
    void swap(Tuple&) {}
    template <size_t i>
    using TypeAt = int;
    template <size_t i, class FirstType, class... Args>
    friend class GetImpl;
    template <class Type, class FirstType, class... Args>
    friend class GetTypeImpl;
    template <class T1, class T2>
    friend class TupleAppendImpl;
    template <class U, class T>
    friend struct TupleAddElemImpl;
    template <class U1, class U2>
    friend struct OperatorCompImpl;
};

//! Сорян за копипасту
//! Всё из-за того, что не существует частичной перегрузки функций
//!--------- get<size_t>
template <size_t i, class FirstType, class... Args>
struct GetImpl {
    static const typename Tuple<FirstType, Args...>::template TypeAt <i>& get(const Tuple<FirstType, Args...>& t) {
        return GetImpl<i - 1, Args...>::get(t.tail);
    }
    static typename Tuple<FirstType, Args...>::template TypeAt <i>& get(Tuple<FirstType, Args...>& t) {
        return GetImpl<i - 1, Args...>::get(t.tail);
    }
    static typename Tuple<FirstType, Args...>::template TypeAt <i>&& get(Tuple<FirstType, Args...>&& t) {
        return GetImpl<i - 1, Args...>::get(std::move(t.tail));
    }
};

template <class FirstType, class... Args>
struct GetImpl<0, FirstType, Args...> {
    static const typename Tuple<FirstType, Args...>::template TypeAt <0>& get(const Tuple<FirstType, Args...>& t) {
        return t.first;
    }
    static typename Tuple<FirstType, Args...>::template TypeAt <0>& get(Tuple<FirstType, Args...>& t) {
        return t.first;
    }
    static typename Tuple<FirstType, Args...>::template TypeAt <0>&& get(Tuple<FirstType, Args...>&& t) {
        return std::move(t.first);
    }
};

template <size_t i, class... Args>
const typename Tuple<Args...>::template TypeAt <i>& get(const Tuple<Args...>& t) {
    return GetImpl<i, Args...>::get(t);
};

template <size_t i, class... Args>
typename Tuple<Args...>::template TypeAt <i>& get(Tuple<Args...>& t) {
    return GetImpl<i, Args...>::get(t);
};

template <size_t i, class... Args>
typename Tuple<Args...>::template TypeAt <i>&& get(Tuple<Args...>&& t) {
    return GetImpl<i, Args...>::get(std::move(t));
};

//!--------- makeTuple
template <class T>
using decayAndStrip = typename std::reference_wrapper<typename std::decay<T>::type>::type;

template<class... Args>
constexpr Tuple<decayAndStrip <Args>...> makeTuple(Args&&... args ) {
    return Tuple<decayAndStrip <Args>...> (std::forward<Args>(args)...);
}
//!------- get<Type>
template <class Type, class FirstType, class... Args>
struct GetTypeImpl {
    static const Type& get(const Tuple<FirstType, Args...>& t) {
        return GetTypeImpl<Type, Args...>::get(t.tail);
    }
    static Type& get(Tuple<FirstType, Args...>& t) {
        return GetTypeImpl<Type, Args...>::get(t.tail);
    }
    static Type&& get(Tuple<FirstType, Args...>&& t) {
        return GetTypeImpl<Type, Args...>::get(std::move(t.tail));
    }
};

template <class Type, class... Args>
struct GetTypeImpl<Type, Type, Args...> {
    static const Type& get(const Tuple<Type, Args...>& t) {
        return t.first;
    }
    static Type& get(Tuple<Type, Args...>& t) {
        return t.first;
    }
    static Type&& get(Tuple<Type, Args...>&& t) {
        return std::move(t.first);
    }
};

template <class Type, class FirstType, class... Args>
const Type& get(const Tuple<FirstType, Args...>& t) {
    return GetTypeImpl<Type, FirstType, Args...>::get(t);
};

template <class Type, class FirstType, class... Args>
Type& get(Tuple<FirstType, Args...>& t) {
    return GetTypeImpl<Type, FirstType, Args...>::get(t);
};

template <class Type, class FirstType, class... Args>
Type&& get(Tuple<FirstType, Args...>&& t) {
    return GetTypeImpl<Type, FirstType, Args...>::get(std::move(t));
};

//!--------- TupleCat
template <class... Args>
struct TupleCatImpl;
template <class U, class T>
struct TupleAppendImpl;

template <class U, class T>
struct TupleAddElemImpl {
    template <class FirstType, class... Types, class E>
    static auto addElem(const Tuple<FirstType, Types...>& tuple, E&& elem) {
        auto&& newTail = TupleAddElemImpl<Tuple<Types...>, E>::addElem(tuple.tail, std::forward<E>(elem));
        Tuple<FirstType, Types..., std::decay_t <E>> res;
        res.first = tuple.first;
        res.tail = newTail;
        return res;
    }
    template <class FirstType, class... Types, class E>
    static auto addElem(Tuple<FirstType, Types...>&& tuple, E&& elem) {
        auto&& newTail = TupleAddElemImpl<Tuple<Types...>, E>::addElem(std::move(tuple.tail), std::forward<E>(elem));
        Tuple<FirstType, Types..., std::decay_t <E>> res;
        res.first = std::move(tuple.first);
        res.tail = std::move(newTail);
        return res;
    }
};

template <class T>
struct TupleAddElemImpl<Tuple<>, T> {
    template <class U>
    static auto addElem(const Tuple<>&, U&& elem) {
        return Tuple<std::decay_t<U> >(std::forward<U>(elem));
    }
    template <class U>
    static auto addElem(Tuple<>&&, U&& elem) {
        return Tuple<std::decay_t<U> >(std::forward<U>(elem));
    }
};

template <class U, class T>
auto tupleAddElem(U&& tuple, T&& elem) {
    return TupleAddElemImpl<U, T>::addElem(std::forward<U>(tuple), std::forward<T>(elem));
};

template <class T1, class T2>
struct TupleAppendImpl {
    template <class U, class FirstType, class... Args>
    static auto tupleAppend(U&& t1, const Tuple<FirstType, Args...>& t2) {
        auto& elem = t2.first;
        auto t1WithAddElem = tupleAddElem(std::forward<U>(t1), elem);
        return TupleAppendImpl<decltype(t1WithAddElem), decltype(t2.tail)>::tupleAppend(t1WithAddElem, t2.tail);
    }
    template <class U, class FirstType, class... Args>
    static auto tupleAppend(U&& t1, Tuple<FirstType, Args...>&& t2) {
        auto&& elem = t2.first;
        auto t1WithAddElem = tupleAddElem(std::forward<U>(t1), std::move(elem));
        return TupleAppendImpl<decltype(t1WithAddElem), decltype(t2.tail)>::tupleAppend(t1WithAddElem, std::move(t2.tail));
    }
};

template <class T>
struct TupleAppendImpl<T, Tuple<>> {
    template <class U>
    static auto tupleAppend(U&& t, const Tuple<>&) {
        return std::forward<U>(t);
    }
    template <class U>
    static auto tupleAppend(U&& t, Tuple<>&&) {
        return std::forward<U>(t);
    }
};

//! Копипаста
template <class T>
struct TupleAppendImpl<T, Tuple<>&> {
    template <class U>
    static auto tupleAppend(U&& t, const Tuple<>& t2) {
        return std::forward<U>(t);
    }
    template <class U>
    static auto tupleAppend(U&& t, Tuple<>&& t2) {
        return std::forward<U>(t);
    }
};

template <class T1, class T2>
auto tupleAppend(T1&& t1, T2&& t2) {
    return TupleAppendImpl<T1, T2>::tupleAppend(std::forward<T1>(t1), std::forward<T2>(t2));
};

template <class Tuple1, class Tuple2, class... Args>
struct TupleCatImpl<Tuple1, Tuple2, Args...> {
    template <class U1, class U2, class... Types>
    static auto tupleCat(U1&& t1, U2&& t2, Types&&... args) {
        auto&& t = tupleAppend(std::forward<U1>(t1), std::forward<U2>(t2)); //auto or auto&&
        return TupleCatImpl<decltype(t), Types...>::tupleCat(std::forward<decltype(t)>(t), std::forward<Args>(args)...);
    }
};

template <class Tuple>
struct TupleCatImpl<Tuple> {
    template <class U>
    static auto tupleCat(U&& t) {
        return std::forward<U>(t);
    }
};

template <>
struct TupleCatImpl<> {
    static Tuple<> tupleCat() {
        return Tuple<>();
    }
};

template <class... Tuples> //return auto or decltype(auto)
auto tupleCat(Tuples&&... args) {
    return TupleCatImpl<Tuples...>::tupleCat(std::forward<Tuples>(args)...);
}

//!--------- Операторы сравнения
template <class U1, class U2>
struct OperatorCompImpl {
    static bool operatorLess(const U1& a, const U2& b) {
        if (get<0>(a) < get<0>(b)) {
            return true;
        } else if (get<0>(a) > get<0>(b)) {
            return false;
        }
        return a.tail < b.tail;
    }
    static bool operatorEqual(const U1& a, const U2& b) {
        if (get<0>(a) != get<0>(b)) {
            return false;
        }
        return a.tail == b.tail;
    }
};

template <class U1>
struct OperatorCompImpl<U1, Tuple<> > {
    static constexpr bool operatorLess(const U1& a, const Tuple<>& b) {
        return false;
    }
    static constexpr bool operatorEqual(U1& a, const Tuple<>& b) {
        return false;
    }
};

template <class U2>
struct OperatorCompImpl<Tuple<>, U2> {
    static constexpr bool operatorLess(const Tuple<>& a, const U2& b) {
        return true;
    }
    static constexpr bool operatorEqual(const Tuple<>& a, const U2& b) {
        return false;
    }
};

template <>
struct OperatorCompImpl<Tuple<>, Tuple<> > {
    static constexpr bool operatorLess(const Tuple<>&, const Tuple<>&) {
        return false;
    }
    static constexpr bool operatorEqual(const Tuple<>&, const Tuple<>&) {
        return true;
    }
};

template <class... T1, class... T2>
constexpr bool operator==(const Tuple<T1...>& a, const Tuple<T2...>& b) {
    return OperatorCompImpl<Tuple<T1...>, Tuple<T2...> >::operatorEqual(a, b);
}

template <class... T1, class... T2>
constexpr bool operator!=(const Tuple<T1...>& a, const Tuple<T2...>& b) {
    return !(a == b);
};

template <class... T1, class... T2>
constexpr bool operator<(const Tuple<T1...>& a, const Tuple<T2...>& b) {
    return OperatorCompImpl<Tuple<T1...>, Tuple<T2...> >::operatorLess(a, b);
}

template <class... T1, class... T2>
constexpr bool operator>(const Tuple<T1...>& a, const Tuple<T2...>& b) {
    return b < a;
}

template <class... T1, class... T2>
constexpr bool operator<=(const Tuple<T1...>& a, const Tuple<T2...>& b) {
    return !(b < a);
}

template <class... T1, class... T2>
constexpr bool operator>=(const Tuple<T1...>& a, const Tuple<T2...>& b) {
    return !(a < b);
}

#endif //TUPLE_TUPLE_H
