#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

class Point {
    double x, y;
public:
    Point() : x(0), y(0)
    {}
    Point(double x, double y) : x(x), y(y)
    {}
    Point(const Point& other) : x(other.x), y(other.y)
    {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    const double getX() const {
        return x;
    }
    const double getY() const {
        return y;
    }
};

class ComparatorXY {
public:
    bool operator()(const Point& a, const Point& b) const {
        return a.getX() < b.getX() || (a.getX() == b.getX() && a.getY() < b.getY());
    }
};

class NonVerticalLine {
    double a, b, c;
public:
    NonVerticalLine(double a, double b, double c) :
            a(a), b(b), c(c) {}
    NonVerticalLine(const Point& p1, const Point& p2) {
        a = p2.getY() - p1.getY();
        b = p1.getX() - p2.getX();
        c = -a * p1.getX() - b * p1.getY();
    }
    bool isOn(const Point& p) const {
        return a * p.getX() + b * p.getY() + c == 0;
    }
    //guarantee that line is not vertical
    double getY(double x) const {
        return (-c - a * x) / b;
    }
};

// Point(a) <= Point(b)
class Segment {
    Point a, b;
public:
    Segment() : a(0, 0), b(0, 0)
    {}
    Segment(const Point& p1, const Point& p2)
            : a(p1), b(p2) {
        if (ComparatorXY()(b, a)) {
            std::swap(a, b);
        }
    }
    Segment(double x1, double y1, double x2, double y2)
            :a(x1, y1), b(x2, y2) {
        if (ComparatorXY()(b, a)) {
            std::swap(a, b);
        }
    }
    Segment(const Segment& other) : a(other.a), b(other.b) {
        if (ComparatorXY()(b, a)) {
            std::swap(a, b);
        }
    }
    const Point getA() const {
        return a;
    }
    const Point getB() const {
        return b;
    }
    const NonVerticalLine getLine() const {
        return NonVerticalLine(a, b);
    }
    bool operator==(const Segment& other) const {
        return a == other.a && b == other.b;
    }
    bool isPointOnSegment(const Point& p) const {
        double maxY = std::max(b.getY(), a.getY());
        double minY = std::min(b.getY(), a.getY());
        bool inRect = p.getX() >= a.getX() &&
                      p.getX() <= b.getX() && p.getY() <= maxY && p.getY() >= minY;
        return getLine().isOn(p) && inRect;
    }
};

// Segments don't intersects by inner Point
class ComparatorSegmentsByHeight {
public:
    bool operator()(const Segment& s, const Segment& other) const {
        NonVerticalLine line = s.getLine();
        NonVerticalLine lineOther = other.getLine();
        if (s.getA().getX() == s.getB().getX()) {
            if (other.getA().getX() == other.getB().getX()) {
                //Both Segments Vertical
                return s.getB().getY() < other.getB().getY();
            }
            // NonVertical Line lineOther
            double yIntersect = lineOther.getY(s.getA().getX());
            return s.getB().getY() <= yIntersect;
        } else if (other.getA().getX() == other.getB().getX()) {
            //NonVertical Line line
            double yIntersect = line.getY(other.getA().getX());
            return yIntersect < other.getB().getY();
        } else {
            //Both Segments are not vertical
            double x1 = std::min(s.getB().getX(), other.getB().getX());
            double y1 = line.getY(x1);
            double yOther1 = lineOther.getY(x1);

            double x2 = std::max(s.getA().getX(), other.getA().getX());
            double y2 = line.getY(x2);
            double yOther2 = lineOther.getY(x2);
            return y1 < yOther1 || y2 < yOther2;
        }
    }
};

enum class PointType : int {
    ptFinish = 0,
    ptOpen = 1,
    ptInterest = 2
};

class EventPoint {
    Point p_;
    PointType event_;
    size_t ind_;
    size_t indSecond_;
public:
    EventPoint(const Point& p, PointType event, size_t ind, size_t indSecond = 0)
            :p_(p), event_(event), ind_(ind), indSecond_(indSecond) {}
    bool operator<(const EventPoint& other) const {
        return p_.getX() < other.p_.getX() ||
               (p_.getX() == other.p_.getX() && event_ < other.event_);
    }
    const Point getPoint() const {
        return p_;
    }
    const PointType getEvent() const {
        return event_;
    }
    const size_t getInd() const {
        return ind_;
    }
    const size_t getIndSecond() const {
        return indSecond_;
    }
};

class CartesianTree {
    struct Node {
        Segment key;
        size_t size;
        int prior;
        Node* left;
        Node* right;
        explicit Node(const Segment& segment) : key(segment) {
            size = 1;
            left = right = nullptr;
            static std::default_random_engine generator;
            static std::uniform_int_distribution<int> distribution(1, (1 << 30));
            prior = distribution(generator);
        }
    };
    Node* root_;
    static size_t getSize(Node* v) {
        if (v) {
            return v->size;
        }
        return 0;
    }
    static void update(Node *v) {
        if (!v)
            return;
        v->size = getSize(v->left) + getSize(v->right) + 1;
    }
    static std::pair<Node*, Node*> split(Node* root, Segment key) {
        if (root == nullptr) {
            return std::make_pair(static_cast<Node*>(nullptr), static_cast<Node*>(nullptr));
        }
        if (ComparatorSegmentsByHeight()(root->key, key)) {
            std::pair <Node*, Node*> splitted = split(root->right, key);
            root->right = splitted.first;
            update(root);
            return std::make_pair(root, splitted.second);
        }
        else {
            std::pair <Node*, Node*> splitted = split(root->left, key);
            root->left = splitted.second;
            update(root);
            return std::make_pair(splitted.first, root);
        }
    }

    static std::pair<Node*, Node*> splitBySize(Node* root, size_t ind) {
        if (root == nullptr) {
            return std::make_pair(static_cast<Node*>(nullptr), static_cast<Node*>(nullptr));
        }
        size_t rootInd = getSize(root->left);
        if (rootInd < ind) {
            std::pair <Node*, Node*> splitted = splitBySize(root->right, ind - rootInd - 1 );
            root->right = splitted.first;
            update(root);
            return std::make_pair(root, splitted.second);
        }
        else {
            std::pair <Node*, Node*> splitted = splitBySize(root->left, ind);
            root->left = splitted.second;
            update(root);
            return std::make_pair(splitted.first, root);
        }
    }

    static Node* merge(Node *left, Node *right) {
        if (left == nullptr || right == nullptr) {
            return left == nullptr ? right : left;
        }
        if (left->prior > right->prior) {
            left->right = merge(left->right, right);
            update(left);
            return left;
        }
        else {
            right->left = merge(left, right->left);
            update(right);
            return right;
        }
    }
public:
    CartesianTree() {
        root_ = nullptr;
    }

    size_t getCountDown(Segment key) {
        size_t predSize = getSize(root_);
        auto x = split(root_, key);
        size_t sizeLeft = getSize(x.first);
        root_ = merge(x.first, x.second);
        return sizeLeft;
    }

    bool isOnSegments(const Point& p) {
        size_t predSize = getSize(root_);
        Segment s(p, p);
        auto x = split(root_, s);
        auto y = splitBySize(x.second, 1);
        bool res = y.first && y.first->key.isPointOnSegment(p);
        root_ = merge(x.first, merge(y.first, y.second));
        return res;
    }

    //delete elem key if exist in tree
    void erase(Segment key) {
        size_t predSize = getSize(root_);
        auto x = split(root_, key);
        auto y = splitBySize(x.second, 1);
        if (y.first && y.first->key == key) {
            delete y.first;
            root_ = merge(x.first, y.second);
        } else {
            root_ = merge(x.first, merge(y.first, y.second));
        }
    }

    void insert(Segment key) {
        size_t predSize = getSize(root_);
        Node* newNode = new Node(key);
        auto x = split(root_, key);
        root_ = merge(x.first, merge(newNode, x.second));
    }
};

enum class AnswerType {
    atInside,
    atBorder,
    atOutside
};

std::string toString(AnswerType ans) {
    switch (ans) {
        case AnswerType::atInside:
            return "INSIDE";
        case AnswerType::atBorder:
            return "BORDER";
        case AnswerType::atOutside:
            return "OUTSIDE";
    }
}

std::set<Point, ComparatorXY> createSetVertices(const std::vector<Point>& polygon) {
    std::set<Point, ComparatorXY> vertices;
    for (auto x : polygon) {
        vertices.insert(x);
    }
    return vertices;
}

void closeOldSegments(std::vector<Segment>& waitingClose,
                      CartesianTree& openSegments) {
    while (!waitingClose.empty()) {
        Segment s = waitingClose.back();
        waitingClose.pop_back();
        openSegments.erase(s);
    }
}

void stepToNewX(CartesianTree& openSegments, std::vector<Segment>& waitingClose,
                std::vector<double>& verticalSegmentsY, bool& isVerticalSegmentsYSorted) {
    closeOldSegments(waitingClose, openSegments);
    verticalSegmentsY.clear();
    isVerticalSegmentsYSorted = false;
}

void processSegmentClose(Segment key, CartesianTree& openSegments,
                         std::vector<Segment>& waitingClose, std::vector<double>& verticalSegmentsY) {
    openSegments.erase(key);
    if (key.getB().getX() == key.getA().getX()) { //vertical segment
        waitingClose.push_back(key);
        verticalSegmentsY.push_back(key.getB().getY());
    }
}

AnswerType getAnswerInterestPoint(Point p, CartesianTree& openSegments,
                                  std::vector<double>& verticalSegmentsY,
                          bool& isVerticalSegmentsYSorted,
                          const std::set<Point, ComparatorXY>& vertices) {
    Segment segmentPoint(p, p);
    if (openSegments.isOnSegments(p) || vertices.find(p) != vertices.end()) {
        return AnswerType::atBorder;
    } else {
        if (!isVerticalSegmentsYSorted) {
            sort(verticalSegmentsY.begin(), verticalSegmentsY.end());
            isVerticalSegmentsYSorted = true;
        }
        long pos = lower_bound(verticalSegmentsY.begin(), verticalSegmentsY.end(), p.getY()) - verticalSegmentsY.begin();
        if ((pos + openSegments.getCountDown(segmentPoint)) % 2 == 0) {
            return AnswerType::atOutside;
        } else {
            return AnswerType::atInside;
        }
    }
}

const std::vector<AnswerType> processPoints(const std::vector<EventPoint>& scanLine,
                                const std::vector<Point>& polygon, const std::vector<Point>& points) {
    std::set <Point, ComparatorXY> vertices = createSetVertices(polygon);
    std::vector <AnswerType> ans(points.size());
    CartesianTree openSegments;
    double predX = 0;
    std::vector <Segment> waitingClose;
    std::vector <double> verticalSegmentsY;
    bool isVerticalSegmentsYSorted = false;
    for (auto eventPoint: scanLine) {
        Point p = eventPoint.getPoint();
        PointType event = eventPoint.getEvent();
        size_t ind = eventPoint.getInd();
        size_t indSecond = eventPoint.getIndSecond();
        if (p.getX() != predX) {
            stepToNewX(openSegments, waitingClose, verticalSegmentsY, isVerticalSegmentsYSorted);
        }
        predX = p.getX();
        Segment key(polygon[ind], polygon[indSecond]);
        if (event == PointType::ptFinish) {
            processSegmentClose(key, openSegments, waitingClose, verticalSegmentsY);
        } else if (event == PointType::ptOpen) {
            openSegments.insert(key);
        } else if (event == PointType::ptInterest) {
            ans[ind] = getAnswerInterestPoint(p, openSegments, verticalSegmentsY,
                                              isVerticalSegmentsYSorted, vertices);
        }
    }
    return ans;
}

void readData(std::vector<Point>& polygon, std::vector<Point>& points) {
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        polygon.push_back(Point(x, y));
    }
    int k;
    std::cin >> k;
    for (int i = 0; i < k; ++i) {
        int x, y;
        std::cin >> x >> y;
        points.push_back(Point(x, y));
    }
}

std::vector<EventPoint> createScanLine(const std::vector<Point>& polygon,
                             const std::vector<Point>& points) {
    size_t n = polygon.size();
    std::vector <EventPoint> scanLine;
    for (size_t i = 0; i < polygon.size(); ++i) {
        size_t indNext = (i + 1) % n;
        if (ComparatorXY()(polygon[i], polygon[indNext])) {
            scanLine.push_back(EventPoint(polygon[i], PointType::ptOpen, i, indNext));
            scanLine.push_back(EventPoint(polygon[indNext], PointType::ptFinish, indNext, i));
        } else {
            scanLine.push_back(EventPoint(polygon[indNext], PointType::ptOpen, indNext, i));
            scanLine.push_back(EventPoint(polygon[i], PointType::ptFinish, i, indNext));
        }
    }
    for (size_t i = 0; i < points.size(); ++i) {
        scanLine.push_back(EventPoint(points[i], PointType::ptInterest, i));
    }
    sort(scanLine.begin(), scanLine.end());
    return scanLine;
}

std::vector<AnswerType> getPointsAnswerType(const std::vector<Point>& polygon,
                                       const std::vector<Point>& points) {
    std::vector<EventPoint> scanLine = createScanLine(polygon, points);
    return processPoints(scanLine, polygon, points);
}

void printAnswer(const std::vector<AnswerType>& ans) {
    for (auto x : ans) {
        std::cout << toString(x) << std::endl;
    }
}

int main() {
    int testsCount;
    std::cin >> testsCount;
    for (int curTest = 0; curTest < testsCount; ++curTest) {
        std::vector<Point> polygon, points;
        readData(polygon, points);
        std::vector<AnswerType> ans = getPointsAnswerType(polygon, points);
        printAnswer(ans);
    }
    return 0;
}