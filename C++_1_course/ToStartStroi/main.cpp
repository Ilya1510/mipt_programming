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
    int rootNumber = getSize(root->left) + 1;
    if (rootNumber <= key) {
        std::pair<TreeNode*, TreeNode*> splitted =
                split(root->right, key - rootNumber);
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

void insert(TreeNode *&root, int ind, int value) {
    TreeNode* newNode = new TreeNode(value);
    std::pair<TreeNode*, TreeNode*> a = split(root, ind - 1);
    root = merge(merge(a.first, newNode), a.second);
}

void toStart(TreeNode *&root, int l, int r) {
    std::pair<TreeNode*, TreeNode*> a = split(root, r);
    std::pair<TreeNode*, TreeNode*> b = split(a.first, l - 1);
    root = merge(merge(b.second, b.first), a.second);
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
    int n, m;
    std::cin >> n >> m;
    TreeNode *root = nullptr;
    for (int i = 1; i <= n; ++i) {
        insert(root, i, i);
    }
    for (int i = 1; i <= m; ++i) {
        int l, r;
        std::cin >> l >> r;
        toStart(root, l, r);
    }
    print(root);
    std::cout << std::endl;
    return 0;
}