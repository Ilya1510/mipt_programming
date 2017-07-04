#pragma once

#include <typeinfo>
#include <stack>
#include <iostream>
#include "ITimSortParams.h"
#include "SelectionSort.h"

namespace timsort {
    
    class RunManager {
        size_t n_;
        size_t minDistRun_;
        const ITimSortParams& params_;
    public:
        RunManager(const ITimSortParams& params, size_t n) 
                :params_(params), n_(n) {
            minDistRun_ = getMinDistRun();
        }
        size_t getMinDistRun() const {
            return params_.minRun(n_);
        }

        //create new run in array of type T
        //and return dist of new run
        template <class RandomAccessIterator, class Compare>
        size_t makeRun(RandomAccessIterator begin, RandomAccessIterator end, Compare compare) {
            size_t indEnd = 0;
            //while run increase we continue rise him
            bool increase = true;
            while (begin + indEnd != end && (indEnd < minDistRun_ || increase)) {
                ++indEnd;
                //if next number will not save increase then increase = false;
                if (begin + indEnd != end) {
                    increase &= !compare(begin[indEnd], begin[indEnd - 1]);
                }
            }
            selectionSort(begin, begin + indEnd, compare);
            //cout <<"!!!"<< indEnd << endl;
            //print(begin, end);
            return indEnd;
        }
        template <class RandomAccessIterator>
        size_t makeRun(RandomAccessIterator begin, RandomAccessIterator end) {
            return makeRun(begin, end, std::less<typename iterator_traits<RandomAccessIterator>::value_type>());
        }

        template <class RandomAccessIterator>
        std::stack <std::pair <RandomAccessIterator, RandomAccessIterator> > 
                splitIntoRuns(RandomAccessIterator begin, RandomAccessIterator end) {
            std::stack <std::pair <RandomAccessIterator, RandomAccessIterator> > stackRuns;
            while (begin != end) {
                size_t distRun = makeRun(begin, end);
                stackRuns.push(make_pair(begin, begin + distRun));
                begin += distRun;
            }
            return stackRuns;
        }
    };

}

using timsort::RunManager;