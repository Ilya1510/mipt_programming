#include <iostream>
#include <vector>

struct node {
    int number;
    int size;
    int prior;
    bool reverse;
    node *left, *right, *pred;
    node(int value = 0) {
        number = value;
        size = 1;
        prior = rand() << 16 + rand();
        reverse = false;
        left = right = pred = nullptr;
    }
};

void push(node *root);
int getIndex(node *a);
int getSize(node *root);

const int N = 100001;
node arr[N];
bool cycle[N];

node* getRoot(node* a) {
    while (a->pred != nullptr) {
        a = a->pred;
    }
    return a;
}

bool isRight(node *a) {
    return getIndex(a) == getSize(getRoot(a)) - 1;
}

void reverseTree(node *root) {
    root->reverse = true;
}

void push(node *root) {
    if (root->reverse) {
        if (root->left != nullptr) {
            root->left->reverse ^= 1;
        }
        if (root->right != nullptr) {
            root->right->reverse ^= 1;
        }
        std::swap(root->left, root->right);
        root->reverse = false;
    }
}

int getSize(node *root) {
    if (root == nullptr) {
        return 0;
    }
    return root->size;
}

void update(node *root) {
    root->size = getSize(root->left) + getSize(root->right) + 1;
    if (root->left != nullptr) {
        root->left->pred = root;
    }
    if (root->right != nullptr) {
        root->right->pred = root;
    }
    root->pred = nullptr;
}

node* merge(node *left, node *right) {
    if (left == nullptr || right == nullptr) {
        return left == nullptr ? right : left;
    }
    push(left);
    push(right);
    if (left->prior > right->prior) {
        left->right = merge(left->right, right);
        left->right->pred = left;
        update(left);
        return left;
    } else {
        right->left = merge(left, right->left);
        right->left->pred = right;
        update(right);
        return right;
    }
}

std::pair<node*, node*> split(node* root, int ind) {
    if (!root) {
        return {nullptr, nullptr};
    }
    push(root);
    int rootKey = getSize(root->left);
    if (rootKey <= ind) {
        std::pair<node*, node*> splitted = split(root->right, ind - rootKey - 1);
        root->right = splitted.first;
        update(root);
        return {root, splitted.second};
    } else {
        std::pair <node*, node*> splitted = split(root->left, ind);
        root->left = splitted.second;
        update(root);
        return {splitted.first, root};
    }
}

void add(int a, int b) {
    node* na = &arr[a];
    node* nb = &arr[b];
    bool isRightA = isRight(na);
    bool isRightB = isRight(nb);
    node* ra = getRoot(na);
    node* rb = getRoot(nb);
    if (ra->number != rb->number) {
        if (!isRightA) {
            reverseTree(ra);
        }
        if (isRightB) {
            reverseTree(rb);
        }
        merge(ra, rb);
    } else {
        cycle[ra->number] = true;
    }
}

int getIndex(node *a) {
    std::vector <node*> v;
    node *b = a;
    while (b) {
        v.push_back(b);
        b = b->pred;
    }
    for (auto it = v.rbegin(); it != v.rend(); it++) {
        push(*it);
    }
    int res = getSize(a->left);
    while (a->pred != nullptr) {
        if (a->pred->right && a->pred->right->number == a->number) {
            res += getSize(a->pred->left) + 1;
        }
        a = a->pred;
    }
    return res;
}

int dist(int a, int b) {
    if (a == b) {
        return 0;
    }
    node* na = &arr[a];
    node* nb = &arr[b];
    node* ra = getRoot(na);
    node* rb = getRoot(nb);
    if (ra->number != rb->number) {
        return -1;
    } else {
        int indA = getIndex(na);
        int indB = getIndex(nb);
        int d = abs(indA - indB) - 1;
        if (cycle[ra->number]) {
            d = std::min(d, ra->size - d - 2);
        }
        return d;
    }
}

void del(int a, int b) {
    node* na = &arr[a];
    node* nb = &arr[b];
    node* root = getRoot(na);
    int indA = getIndex(na);
    int indB = getIndex(nb);
    if (!cycle[root->number]) {
        split(root, std::min(indA, indB));
    } else {
        cycle[root->number] = false;
        int id1 = std::min(indA, indB);
        int id2 = std::max(indA, indB);
        if (id1 == 0 && id2 == getSize(root) - 1) {
            return;
        }
        std::pair<node*, node*> p = split(root, id1);
        merge(p.second, p.first);
    }
}

int n, m, q;
void print() {
    std::cout << "***" << std::endl;
    for (int i = 1; i <= n; ++i) {
        std::cout << cycle[i] << " ";
    }
    std::cout << std::endl;
    for (int i = 1; i <= n; ++i) {
        std::cout << i << " " << (arr[i].pred ? arr[i].pred->number : 0) << " " << (arr[i].left ? arr[i].left->number : 0)
                  << " " << (arr[i].right ? arr[i].right->number : 0) << std::endl;
    }
    std::cout << "***" << std::endl;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    std::cin >> n >> m >> q;
    for (int i = 1; i <= n; ++i) {
        arr[i].number = i;
    }

    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        add(a, b);
    }
    for (int i = 0; i < q; ++i) {
        char c; int a, b;
        std::cin >> c >> a >> b;
        if (c == '+') {
            add(a, b);
        } else if (c == '-') {
            del(a, b);
        } else {
            std::cout << dist(a, b) << std::endl;
        }
    }
    return 0;
}