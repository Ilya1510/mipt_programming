#include <iostream>
#include <ctime>
#include <vector>

struct TreeNode {
    int key;
    int prior;
    int size;
    TreeNode *left, *right;
    TreeNode(int value) {
        key = value;
        prior = (rand() << 16) + rand();
        size = 1;
    }
};

void print(TreeNode *root);

int getSize(TreeNode *root) {
    if (root == nullptr) {
        return 0;
    }
    return root->size;
}

void update(TreeNode *root) {
    root->size = getSize(root->left) + getSize(root->right) + 1;
}

std::pair<TreeNode*, TreeNode*> split(TreeNode* root, int key) {
    if (root == nullptr) {
        return {nullptr, nullptr};
    }
    if (root->key <= key) {
        std::pair<TreeNode*, TreeNode*> splitted =
                split(root->right, key);
        root->right = splitted.first;
        update(root);
        return {root, splitted.second};
    } else {
        std::pair<TreeNode*, TreeNode*> splitted =
                split(root->left, key);
        root->left = splitted.second;
        update(root);
        return {splitted.first, root};
    }
}

TreeNode* merge(TreeNode *left, TreeNode *right) {
    if (left == nullptr || right == nullptr) {
        return left == nullptr ? right : left;
    }
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

void insert(TreeNode *&root, int key) {
    std::pair<TreeNode*, TreeNode*> sp = split(root, key);
    TreeNode* newNode = new TreeNode(key);
    root = merge(merge(sp.first, newNode), sp.second);
}

void remove(TreeNode *&root, int key) {
    std::pair<TreeNode*, TreeNode*> sp = split(root, key);
    std::pair<TreeNode*, TreeNode*> spl = split(sp.first, key - 1);
    root = merge(spl.first, sp.second);
}

int kmax(TreeNode *root, int k) {
    while (getSize(root->right) != k - 1) {
        if (getSize(root->right) >= k) {
            root = root->right;
        } else {
            k -= getSize(root->right) + 1;
            root = root->left;
        }
    }
    return root->key;
}

void print(TreeNode *root) {
    if (root == nullptr) {
        return;
    }
    print(root->left);
    std::cout << root->key << " ";
    print(root->right);
}

int main() {
    int n;
    std::cin >> n;
    TreeNode *root = nullptr;
    for (int i = 0; i < n; ++i) {
        int c, k;
        std::cin >> c >> k;
        if (c == 1) {
            insert(root, k);
        } else if (c == -1) {
            remove(root, k);
        } else {
            std::cout << kmax(root, k) << std::endl;
        }
        //print(root);
        //std::cout << std::endl;
    }
    return 0;
}