#pragma once

#include <algorithm>
namespace timsort {

    template <class RandomAccessIterator>
    void print(RandomAccessIterator begin, RandomAccessIterator end) {
        while (begin != end) {
            cout << *begin << " ";
            ++begin;
        }
        cout << endl;
    }


    //garanted that part divides group evenly
    template <class RandomAccessIterator, class Compare>
    void sortGroupsByFirstValue(RandomAccessIterator begin, RandomAccessIterator part, 
            RandomAccessIterator end, size_t lenGroup, Compare compare) {
        size_t groupsCount = (end - begin) / lenGroup;
        for (size_t i = 0; i < groupsCount; ++i) {
            size_t indexMinimGroup = i;
            for (size_t j = i + 1; j < groupsCount; ++j) {
                size_t indexFirstElementGroup = j * lenGroup;
                size_t indexLastElemetnGroup = indexFirstElementGroup + lenGroup - 1;
                if (compare(begin[indexMinimGroup * lenGroup], begin[indexFirstElementGroup])) {
                    continue;
                }
                if (compare(begin[indexFirstElementGroup], begin[indexMinimGroup * lenGroup]) ||
                    compare(begin[indexLastElemetnGroup], begin[indexMinimGroup * lenGroup + lenGroup - 1])) {
                    indexMinimGroup = j;
                }
            }
            std::swap_ranges(begin + i * lenGroup, 
                begin + (i + 1) * lenGroup, begin + indexMinimGroup * lenGroup);

        }
    }

    //buffer must consist >= size and size1 == size2 and firstEnd = secondBegin
    //garanted that numbers in buffer can only swap between themselves
    template <class RandomAccessIterator, class Compare>
    void mergeSortWithBuffer(RandomAccessIterator firstBegin, RandomAccessIterator secondBegin, 
            RandomAccessIterator bufferBegin, Compare compare) {
        size_t len = secondBegin - firstBegin;
        RandomAccessIterator secondEnd = secondBegin + len;
        RandomAccessIterator bufferEnd = bufferBegin + len;
        std::swap_ranges(firstBegin, secondBegin, bufferBegin);
        while (bufferBegin != bufferEnd || secondBegin != secondEnd) {
            if (bufferBegin == bufferEnd) {
                std::swap(*secondBegin++, *firstBegin++);
            } else if (secondBegin == secondEnd) {
                std::swap(*bufferBegin++, *firstBegin++);
            } else if (compare(*secondBegin, *bufferBegin)) {
                std::swap(*secondBegin++, *firstBegin++);
            } else {
                std::swap(*bufferBegin++, *firstBegin++);
            }
        }
    }

    //last group is used as buffer for merge and size divisible lenGroup
    template <class RandomAccessIterator, class Compare>
    void sortAdjacentGroupsUp(RandomAccessIterator begin, 
            RandomAccessIterator end, size_t lenGroup, Compare compare) {
        size_t groupCount = (end - begin) / lenGroup - 1;
        RandomAccessIterator lastGroup = end - lenGroup;
        for (size_t i = 0; i < groupCount - 1; ++i) {
            mergeSortWithBuffer(begin + i * lenGroup, begin + (i + 1) * lenGroup,
                end - lenGroup, compare);
        }
    }

    template <class RandomAccessIterator, class Compare>
    void sortAdjacentGroupsDown(RandomAccessIterator begin, 
            RandomAccessIterator end, size_t lenGroup, Compare compare) {
        RandomAccessIterator currentGroupEnd = end - lenGroup;
        while (currentGroupEnd >= begin + 2 * lenGroup) {
            RandomAccessIterator currentGroupBegin = currentGroupEnd - 2 * lenGroup;
            mergeSortWithBuffer(currentGroupBegin, currentGroupBegin + lenGroup, end - lenGroup, compare);
            currentGroupEnd -= lenGroup;
        }
        selectionSort(begin, currentGroupEnd, compare);
    }


    template <class RandomAccessIterator, class Compare>
    void inplaceMerge(RandomAccessIterator begin, 
            RandomAccessIterator part, RandomAccessIterator end, Compare compare) {
        size_t numbersCountAll = end - begin;
        if (numbersCountAll <= 50) {
            selectionSort(begin, end, compare);
            return;
        }
        size_t indBeginSecondArray = part - begin;
        size_t lenGroup = static_cast<int>(sqrt(numbersCountAll));
        size_t reminder = end - begin - lenGroup * lenGroup;
        //print(begin, end);
        //remove yellow block;
        size_t indBeginSecondArrayGroup = indBeginSecondArray / lenGroup;
        size_t indLastGroup = (end - begin) / lenGroup - 1;
        if (indBeginSecondArrayGroup > indLastGroup) {
            indBeginSecondArrayGroup = indLastGroup;
        }
        RandomAccessIterator beginSecondArrayGroup = 
            begin + indBeginSecondArrayGroup * lenGroup;
        RandomAccessIterator lastGroup = begin + indLastGroup * lenGroup;

        std::swap_ranges(beginSecondArrayGroup, 
            beginSecondArrayGroup + lenGroup, lastGroup);
        //print(begin, end);
        //selection sort for red an blue group with selection sort
        sortGroupsByFirstValue(begin, part, end - lenGroup - reminder, lenGroup, compare);
        //print(begin, end);

        //sort adjacent groups, so after array is sorted except last group and reminder
        sortAdjacentGroupsUp(begin, end - reminder, lenGroup, compare);
        //print(begin, end);

        //sort elements in lastGroup and reminder O(n) because 2 * len <= 4 * sqrt(n)
        lenGroup = lenGroup + reminder;
        RandomAccessIterator beginSort = max(begin, end - 2 * lenGroup);
        selectionSort(beginSort, end, compare);
        //print(begin, end);

        //protalk numbers from reminder and last group to begin
        sortAdjacentGroupsDown(begin, end, lenGroup, compare);
        //print(begin, end);

        //and sort buffer, because he can be repositoned
        selectionSort(end - lenGroup, end, compare);
        //print(begin, end);
    }

    template <class RandomAccessIterator>
    void inplaceMerge(RandomAccessIterator begin,
        RandomAccessIterator part, RandomAccessIterator end) {
        inplaceMerge(begin, part, end, std::less<typename iterator_traits<RandomAccessIterator>::value_type>());
    }
}

using timsort::print;