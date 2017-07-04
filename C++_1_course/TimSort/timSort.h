#pragma once

#include <iostream>
#include "ITimSortParams.h"
#include "Runs.h"

namespace timsort {
    template <class RandomAccessIterator, class Compare>
    void timSort(RandomAccessIterator begin, RandomAccessIterator end, Compare comp, 
            const ITimSortParams& params = ITimSortParamsDefault()) {
        typedef std::pair <RandomAccessIterator, RandomAccessIterator> PairIter;
        typedef stack <PairIter> StackRun;
        StackRun stackRuns;
        RandomAccessIterator begin1 = begin;
        RunManager runManager = RunManager(ITimSortParamsDefault(), end - begin);
        while (begin != end) {
            size_t curLenRun = runManager.makeRun(begin, end, comp);
            stackRuns.push(make_pair(begin, begin + curLenRun));
            begin += curLenRun;
            while (true) {
                if (stackRuns.size() == 1) {
                    break;
                }
                PairIter X = stackRuns.top();
                stackRuns.pop();
                PairIter Y = stackRuns.top();
                stackRuns.pop();
                size_t lenX = X.second - X.first, lenY = Y.second - Y.first;
                if (stackRuns.empty()) {
                    if (params.needMerge(lenX, lenY)) {
                        inplaceMerge(Y.first, Y.second, X.second, comp);
                        stackRuns.push(make_pair(Y.first, X.second));
                        continue;
                    }
                    else {
                        stackRuns.push(Y);
                        stackRuns.push(X);
                        break;
                    }
                }
                else {
                    PairIter Z = stackRuns.top();
                    stackRuns.pop();
                    size_t lenZ = Z.second - Z.first;
                    EWhatMerge whatMerge = params.whatMerge(lenX, lenY, lenZ);
                    if (whatMerge == WM_MergeXY) {
                        inplaceMerge(Y.first, Y.second, X.second, comp);
                        stackRuns.push(Z);
                        stackRuns.push(make_pair(Y.first, X.second));
                        continue;
                    }
                    else if (whatMerge == WM_MergeYZ) {
                        inplaceMerge(Z.first, Z.second, Y.second, comp);
                        stackRuns.push(make_pair(Z.first, Y.second));
                        stackRuns.push(X);
                        continue;
                    } else {
                        stackRuns.push(Z);
                        stackRuns.push(Y);
                        stackRuns.push(X);
                        break;
                    }
                }
            }
        }
        while (stackRuns.size() >= 2) {
            PairIter X = stackRuns.top();
            stackRuns.pop();
            PairIter Y = stackRuns.top();
            stackRuns.pop();
            inplaceMerge(Y.first, Y.second, X.second, comp);
            stackRuns.push(make_pair(Y.first, X.second));
        }
        //конец!)
    }
    template <class RandomAccessIterator>
    void timSort(RandomAccessIterator first, RandomAccessIterator last, 
             const ITimSortParams& params = ITimSortParamsDefault()) {
        timSort(first, last, std::less<typename std::iterator_traits<
                RandomAccessIterator>::value_type>(), params);
    }
}

using timsort::timSort;