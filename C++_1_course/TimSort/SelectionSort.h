#pragma once

#include <typeinfo>
#include <stack>
#include <iostream>
#include "ITimSortParams.h"

namespace timsort {
    template <class RandomAccessIterator, class Compare>
    void selectionSort(RandomAccessIterator begin, RandomAccessIterator end, Compare compare) {
        size_t numbersCount = end - begin;
        for (size_t i = 0; i < numbersCount; ++i) {
            size_t currentIndexMinimum = i;
            for (size_t j = i; j < numbersCount; ++j) {
                if (compare(begin[j], begin[currentIndexMinimum])) {
                    currentIndexMinimum = j;
                }
            }
            swap(begin[i], begin[currentIndexMinimum]);
        }
    }

    template <class RandomAccessIterator>
    void selectionSort(RandomAccessIterator begin, RandomAccessIterator end) {
        selectionSort(begin, end, std::less<typename iterator_traits<RandomAccessIterator>::value_type>());
    }
}

using timsort::selectionSort;