#include <iostream>
#include <stack>
#include <algorithm>

using namespace std;

struct Footballer{
    long long effic;
    int ind;
};

bool comp(const Footballer& a, const Footballer& b) {
    return a.effic < b.effic || a.effic == b.effic && a.ind < b.ind;
}

enum EWhatMerge {
    WM_NoMerge, WM_MergeXY, WM_MergeYZ
};

typedef unsigned int ui32;

struct ITimSortParams {
    // Вычисление minRun по n.
    virtual ui32 minRun(ui32 n) const = 0;
    // Нужно ли выполнять слияние, если в стеке ровно два рана размеров lenX и lenY.
    virtual bool needMerge(ui32 lenX, ui32 lenY) const = 0;
    // Как сливать, если в стеке три последних рана имеют размеры lenX, lenY и lenZ.
    virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const = 0;
    // Когда в merge искать бинпоиском, сколько элементов меньше заданного.           
    virtual ui32 getGallop() const = 0;
};

struct ITimSortParamsDefault : public ITimSortParams {
    ui32 minRun(ui32 n) const override {
        return 50;
    }
    bool needMerge(ui32 lenX, ui32 lenY) const override {
        return false;
    }
    EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const override {
        if (lenX >= lenY) {
            return WM_MergeXY;
        }
        else if (lenZ <= lenX + lenY) {
            return WM_MergeYZ;
        }
        return WM_NoMerge;
    }
    ui32 getGallop() const override {
        return 100000;
    }
};

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
        }
        else if (secondBegin == secondEnd) {
            std::swap(*bufferBegin++, *firstBegin++);
        }
        else if (compare(*secondBegin, *bufferBegin)) {
            std::swap(*secondBegin++, *firstBegin++);
        }
        else {
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
};


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
                }
                else {
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

int main() {
    int n;
    scanf("%d\n", &n);
    Footballer *people = new Footballer[n];
    bool *used = new bool[n];
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &people[i].effic);
        people[i].ind = i;
        used[i] = false;
    }
    timSort(people, people + n, comp);
    long long ans = 0;
    int ans_left = 0, ans_right = 0;
    long long sum_eff = people[n - 1].effic;
    int ind_min = n - 2; // ind people, who we don't take
    for (int ind_max = n - 1; ind_max >= 0; --ind_max) {
        while (ind_min >= 0 && people[ind_min].effic + people[ind_min + 1].effic >= people[ind_max].effic) {
            sum_eff += people[ind_min].effic;
            --ind_min;
        }
        if (ans < sum_eff) {
            ans = sum_eff;
            ans_left = ind_min + 1;
            ans_right = ind_max;
        }
        sum_eff -= people[ind_max].effic;
    }
    printf("%lld\n", ans);
    for (int i = ans_left; i <= ans_right; ++i) {
        used[people[i].ind] = true;
    }
    for (int i = 0; i < n; ++i) {
        if (used[i]) {
            printf("%d ", i + 1);
        }
    }
    delete[] people;
    delete[] used;
    return 0;
}