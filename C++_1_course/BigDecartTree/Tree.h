//
// Created by ilya on 26.03.17.
//

#ifndef BIGDECART_CartesianTree_H
#define BIGDECART_CartesianTree_H

#include <cstddef>
#include <functional>
#include <random>

template <class T>
class CartesianTree {
    static const long long INF = 100000000000000000;

    struct Node {
        T key;
        //! sum keep sum of keys in subtree with root
        T sum;
        bool hasAssignedValue;
        T assignedValue;
        //! addValue need add to all vertices in subtree without root
        T addValue;
        T prior;
        //! size of subtree with root
        size_t size;
        bool isIncreasing;
        bool isDecreasing;
        bool needReverse;
        //! the rightest and leftest numbers in subtree
        T rightNumber;
        T leftNumber;
        Node *left;
        Node *right;

        explicit Node (T value);
    };
    Node *root;
    static size_t getSize(Node *root);

    static T getSum(Node *root);

    static T getPrior(Node *root);

    static bool getIsIncreasing(Node *root);

    static bool getIsDecreasing(Node *root);

    static std::pair<Node*, Node*> split(Node* root, int ind);

    static void setValue(Node *root, T value);

    static void addValue(Node *root, T value);

    static void changeReverse(Node *root);

    static void push(Node *root);

    static void update(Node *root);

    static void doNextPermutation(Node *&root);

    static void doPreviousPermutation(Node *&root);

    static void doNearPermutation(Node *&root, bool next);

    size_t findBound(T value, bool IsupperBound);

    size_t upperBound(T value);

    size_t lowerBound(T value);

    static size_t getDistMaxDecreasingSuffix(Node *root);

    static size_t getDistMaxIncreasingSuffix(Node *root);

    static size_t getDistMonotoneSuffix(Node* root, bool isIncreasing);

    static Node* merge(Node* left, Node* right);

    void printAllInformation(Node *root);

    void pushAll(Node *root);

    void modifySubTree(size_t l, size_t r, std::function<void(Node*&)>);

public:
    CartesianTree();

    CartesianTree(Node *node) : CartesianTree() {
        root = node;
    }

    CartesianTree(T *numbers, size_t n);

    void insert(T value, size_t ind);

    void remove(size_t ind);

    T getSum(size_t l, size_t r);

    void addValue(T value, size_t l, size_t r);

    void setValue(T value, size_t l, size_t r);

    void printTree();

    void pushTree();

    void doNextPermutation(size_t l , size_t r);

    void doPreviousPermutation(size_t l, size_t r);

    size_t getSize();
};

//
// Created by ilya on 26.03.17.
//

#include <iostream>
#include <vector>
#include "Tree.h"
#include <random>

template <class T>
CartesianTree<T>::Node::Node (T value) {
    key = value;
    size = 1;
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(1, (1 << 30));
    prior = distribution(generator);
    sum = value;
    assignedValue = 0;
    hasAssignedValue = false;
    addValue = 0;
    isIncreasing = isDecreasing = true;
    leftNumber = rightNumber = value;
    needReverse = false;
    right = left = nullptr;
}

template <class T>
size_t CartesianTree<T>::getSize(Node *root) {
    if (!root) {
        return 0;
    }
    return root->size;
}

template <class T>
T CartesianTree<T>::getSum(Node *root) {
    if (!root) {
        return 0;
    }
    return root->sum;
}

template <class T>
T CartesianTree<T>::getPrior(Node *root) {
    if (!root) {
        return -1;
    }
    return root->prior;
}

template <class T>
bool CartesianTree<T>::getIsIncreasing(Node *root) {
    if (!root) {
        return true;
    }
    return root->isIncreasing;
}

template <class T>
bool CartesianTree<T>::getIsDecreasing(Node *root) {
    if (!root) {
        return true;
    }
    return root->isDecreasing;
}

template <class T>
std::pair<typename CartesianTree<T>::Node*, typename CartesianTree<T>::Node*>
CartesianTree<T>::split(CartesianTree<T>::Node* root, int ind) {
    if (!root) {
        return {nullptr, nullptr};
    }
    push(root);
    //At left tree 0 .. (ind - 1) elements so < ind and at right >= ind
    int indRoot = getSize(root->left);
    if (indRoot >= ind) {
        std::pair<Node*, Node*> splitted = split(root->left, ind);
        root->left = splitted.second;
        update(root);
        return {splitted.first, root};
    } else {
        std::pair<Node*, Node*> splitted = split(root->right, ind - indRoot - 1);
        root->right = splitted.first;
        update(root);
        return {root, splitted.second};
    }
};

template <class T>
void CartesianTree<T>::setValue(Node *root, T value) {
    if (!root) {
        return;
    }
    root->hasAssignedValue = true;
    root->assignedValue = value;
    root->sum = root->size * root->assignedValue;
    root->addValue = 0;
    root->key = value;
    root->leftNumber = root->rightNumber = value;
    root->isDecreasing = root->isIncreasing = true;
}

template <class T>
void CartesianTree<T>::addValue(Node *root, T value) {
    if (!root) {
        return;
    }
    root->addValue += value;
    root->sum += value * root->size;
    root->rightNumber += value;
    root->leftNumber += value;
    root->key += value;
}

template <class T>
void CartesianTree<T>::changeReverse(Node *root) {
    if (!root) {
        return;
    }
    root->needReverse = !root->needReverse;
    std::swap(root->leftNumber, root->rightNumber);
    std::swap(root->isIncreasing, root->isDecreasing);
};

//! There are three deferred operations: reverse[l, r], add[l, r], set[l, r]
//! After push children of this vertex contain true information their fields
template <class T>
void CartesianTree<T>::push(Node *root) {
    if (!root) {
        return;
    }
    if (root->needReverse) {
        changeReverse(root->left);
        changeReverse(root->right);
        std::swap(root->left, root->right);
        root->needReverse = false;
    }
    if (root->hasAssignedValue) {
        root->assignedValue += root->addValue;
        root->addValue = 0;
    }
    if (root->hasAssignedValue) {
        setValue(root->left, root->assignedValue);
        setValue(root->right, root->assignedValue);
    } else {
        addValue(root->left, root->addValue);
        addValue(root->right, root->addValue);
    }
    root->hasAssignedValue = false;
    root->assignedValue = 0;
    root->addValue = 0;
}

//! we guarantee that update calls when vertex is pushed
template <class T>
void CartesianTree<T>::update(Node *root) {
    root->size = getSize(root->left) + getSize(root->right) + 1;
    root->sum = getSum(root->left) + getSum(root->right) + root->key;
    root->rightNumber = root->right ? root->right->rightNumber : root->key;
    root->leftNumber = root->left ? root->left->leftNumber : root->key;
    T rightCenter = root->right ? root->right->leftNumber : root->key;
    T leftCenter = root->left ? root->left->rightNumber : root->key;
    root->isIncreasing = getIsIncreasing(root->left) &&
                         leftCenter <= root->key && root->key <= rightCenter &&
                         getIsIncreasing(root->right);
    root->isDecreasing = getIsDecreasing(root->left) &&
                         leftCenter >= root->key && root->key >= rightCenter &&
                         getIsDecreasing(root->right);
}

template <class T>
typename CartesianTree<T>::Node* CartesianTree<T>::merge(CartesianTree<T>::Node* left,
                                                   CartesianTree<T>::Node* right) {
    if (!left || !right) {
        return left ? left : right;
    }
    push(left);
    push(right);
    if (left->prior > right->prior) {
        left->right = merge(left->right, right);
        update(left);
        return left;
    } else {
        right->left = merge(left, right->left);
        update(right);
        return right;
    }
}

template <class T>
void CartesianTree<T>::printAllInformation(Node *root) {
    if (!root) {
        return;
    }
    printAllInformation(root->left);
    std::cout << root->key << " ";
    printAllInformation(root->right);
}

template <class T>
CartesianTree<T>::CartesianTree() {
    root = nullptr;
}

template <class T>
CartesianTree<T>::CartesianTree(T *numbers, size_t n) : CartesianTree() {
    root = nullptr;
    for (int i = 0; i < n; ++i) {
        insert(numbers[i], i);
        //printTree();
    }
}

template <class T>
void CartesianTree<T>::insert(T value, size_t ind) {
    Node* newNode = new Node(value);
    std::pair<Node*, Node*> splitOne = split(root, ind);
    root = merge(merge(splitOne.first, newNode), splitOne.second);
}

template <class T>
void CartesianTree<T>::remove(size_t ind) {
    std::pair<Node*, Node*> splitOne = split(root, ind);
    std::pair<Node*, Node*> splitTwo = split(splitOne.second, 1);
    root = merge(splitOne.first, splitTwo.second);
}

template <class T>
void CartesianTree<T>::modifySubTree(size_t l, size_t r, std::function<void(Node*&)> f) {
    std::pair<Node*, Node*> splitRight = split(root, r + 1);
    std::pair<Node*, Node*> splitLeft = split(splitRight.first, l);
    f(splitLeft.second);
    root = merge(merge(splitLeft.first, splitLeft.second), splitRight.second);
}


template <class T>
T CartesianTree<T>::getSum(size_t l, size_t r) {
    T result;
    modifySubTree(l, r, [&result](Node*& node){result = getSum(node);});
    return result;
}

template <class T>
void CartesianTree<T>::addValue(T value, size_t l, size_t r) {
    modifySubTree(l, r, [value](Node *&node){addValue(node, value);});
}

template <class T>
void CartesianTree<T>::setValue(T value, size_t l, size_t r) {
    modifySubTree(l, r, [value](Node *&node){setValue(node, value);});
}

template <class T>
void CartesianTree<T>::doNextPermutation(size_t l, size_t r) {
    modifySubTree(l, r, [](Node *&node){doNextPermutation(node);});
}

template <class T>
void CartesianTree<T>::doNearPermutation(Node *&root, bool next) {
    int lenSuffix = next ? getDistMaxDecreasingSuffix(root) :
                    getDistMaxIncreasingSuffix(root);
    //! next permutation from decreasing array is increasing array
    if (lenSuffix == getSize(root)) {
        changeReverse(root);
        return;
    }
    std::pair<CartesianTree::Node*, CartesianTree::Node*> splitSuffix =
            split(root, root->size - lenSuffix);
    if (next) {
        changeReverse(splitSuffix.second);
    }
    T lastValuePreSuffix = splitSuffix.first->rightNumber;
    int ind = next ? CartesianTree(splitSuffix.second).
            upperBound(lastValuePreSuffix) : CartesianTree(splitSuffix.second).
            lowerBound(lastValuePreSuffix);
    if (!next) {
        changeReverse(splitSuffix.second);
        ind = getSize(splitSuffix.second) - ind;
    }
    std::pair<CartesianTree::Node*, CartesianTree::Node*> splitPrefix =
            split(splitSuffix.first, getSize(splitSuffix.first) - 1);
    std::pair<CartesianTree::Node*, CartesianTree::Node*> splitValue =
            split(splitSuffix.second, ind);
    std::pair<CartesianTree::Node*, CartesianTree::Node*> splitThree =
            split(splitValue.second, 1);
    root = merge(splitPrefix.first,
                 merge(splitThree.first,
                       merge(splitValue.first,
                             merge(splitPrefix.second, splitThree.second))));
}

template <class T>
void CartesianTree<T>::doNextPermutation(CartesianTree::Node *&root) {
    doNearPermutation(root, true);
}

template <class T>
void CartesianTree<T>::doPreviousPermutation(Node *&root) {
    doNearPermutation(root, false);
}

template <class T>
size_t CartesianTree<T>::findBound(T value, bool IsupperBound) {
    CartesianTree::Node *node = root;
    size_t ind = 0;
    while (node) {
        push(node);
        if (value > node->key || value == node->key && IsupperBound) {
            ind += getSize(node->left) + 1;
            node = node->right;
        } else {
            node = node->left;
        }
    }
    return ind;
}

template <class T>
size_t CartesianTree<T>::upperBound(T value) {
    return findBound(value, true);
}

template <class T>
size_t CartesianTree<T>::lowerBound(T value) {
    return findBound(value, false);
}

template <class T>
size_t CartesianTree<T>::getDistMaxDecreasingSuffix(Node *root) {
    return getDistMonotoneSuffix(root, false);
}

template <class T>
size_t CartesianTree<T>::getDistMaxIncreasingSuffix(Node *root) {
    return getDistMonotoneSuffix(root, true);
}

template <class T>
size_t CartesianTree<T>::getDistMonotoneSuffix(Node *root, bool isIncreasing) {
    size_t result = 0;
    long long val = isIncreasing ? INF : -INF;
    while (root) {
        push(root);
        if (!root->right) {
            if (isIncreasing && root->key <= val ||
                !isIncreasing && root->key >= val) {
                result += 1;
                val = root->key;
                root = root->left;
                continue;
            } else {
                return result;
            }
        } else if (isIncreasing && root->right->rightNumber > val ||
                   !isIncreasing && root->right->rightNumber < val) {
            return result;
        } else if (isIncreasing && (!root->right->isIncreasing || root->key > root->right->leftNumber) ||
                   !isIncreasing && (!root->right->isDecreasing || root->key < root->right->leftNumber) ) {
            root = root->right;
        } else {
            result += getSize(root->right) + 1;
            val = root->key;
            root = root->left;
        }
    }
    return result;
}

template <class T>
void CartesianTree<T>::doPreviousPermutation(size_t l, size_t r) {
    modifySubTree(l, r, [](Node *&node){doPreviousPermutation(node);});
}

template <class T>
void CartesianTree<T>::printTree() {
    printAllInformation(root);
    std::cout << std::endl;
}

template <class T>
void CartesianTree<T>::pushAll(Node *root) {
    if (!root) {
        return;
    }
    push(root);
    pushAll(root->left);
    pushAll(root->right);
}

template <class T>
size_t CartesianTree<T>::getSize() {
    return getSize(root);
}

template <class T>
void CartesianTree<T>::pushTree() {
    pushAll(root);
}

#endif //BIGDECART_CartesianTree_H
