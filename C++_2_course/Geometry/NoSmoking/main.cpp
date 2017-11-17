#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <assert.h>

bool flag;
int ind1;
int ind2;

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
    int ind;
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
    Segment(const Segment& other) : a(other.a), b(other.b), ind(other.ind) {
        if (ComparatorXY()(b, a)) {
            std::swap(a, b);
        }
    }
    //! Intersect by OX
    bool isIntersect(const Segment& other) const {
        NonVerticalLine line = getLine();
        NonVerticalLine lineOther = other.getLine();
        if (getA().getX() == getB().getX()) {
            if (other.getA().getX() == other.getB().getX()) {
                //Both Segments Vertical
                double y1 = getA().getY();
                double y2 = getB().getY();
                double y3 = other.getA().getY();
                double y4 = other.getB().getY();
                assert(y4 >= y3 && y2 >= y1);
                return (y1 >= y3 && y1 <= y4) ||
                       (y2 >= y3 && y2 <= y4) ||
                       (y3 >= y1 && y3 <= y2) ||
                       (y4 >= y1 && y4 <= y2);
            }
            // NonVertical Line lineOther
            double yIntersect = lineOther.getY(getA().getX());
            return getA().getY() <= yIntersect && yIntersect <= getB().getY();
        } else if (other.getA().getX() == other.getB().getX()) {
            //NonVertical Line line
            double yIntersect = line.getY(other.getA().getX());
            return other.getA().getY() <= yIntersect && yIntersect <= other.getB().getY();
        } else {
            //Both Segments are not vertical
            double x1 = std::min(getB().getX(), other.getB().getX());
            double y1 = line.getY(x1);
            double yOther1 = lineOther.getY(x1);

            double x2 = std::max(getA().getX(), other.getA().getX());
            double y2 = line.getY(x2);
            double yOther2 = lineOther.getY(x2);
            return (y1 <= yOther1 && y2 >= yOther2) ||
                   (y1 >= yOther1 && y2 <= yOther2);
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
        if ((s.isIntersect(other) || other.isIntersect(s)) && s.ind != other.ind) {
            flag = true;
            ind1 = s.ind;
            ind2 = other.ind;
            return false;
        }
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

using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int n;
    std::cin >> n;
    std::vector<Segment> segs;
    std::vector<std::pair<double, int> > line;
    for (int i = 0; i < n; ++i) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        segs.push_back(Segment(x1, y1, x2, y2));
        segs.back().ind = i + 1;
        line.push_back(std::make_pair(segs.back().getA().getX(), -(i + 1)));
        line.push_back(std::make_pair(segs.back().getB().getX(), i + 1));
    }
    sort(line.begin(), line.end());
    std::multiset <Segment, ComparatorSegmentsByHeight> s;
    vector <pair <pair<double, double>, int> >vector1;
    bool isSorted = false;
    for (auto p : line) {
        if (p.second < 0) {
            int ind = -p.second - 1;
            Segment curS = segs[ind];
            if (curS.getA().getX() == curS.getB().getX()) {
                vector1.push_back(make_pair(make_pair(curS.getA().getY(), curS.getB().getY()), ind + 1));
                isSorted = false;
            } else {
                s.insert(segs[ind]);
            }
        }
        else {
            int ind = p.second - 1;
            Segment curS = segs[ind];
            if (curS.getA().getX() == curS.getB().getX()) {
                if (!isSorted) {
                    isSorted = true;
                    sort(vector1.begin(), vector1.end());
                    double maxRight = vector1[0].first.second;
                    int indRight = 0;
                    for (int i = 1; i < vector1.size(); ++i) {
                        if (maxRight >= vector1[i].first.first) {
                            flag = true;
                            ind1 = ind + 1;
                            ind2 = vector1[indRight].second;
                        } else {
                            maxRight = vector1[i].first.second;
                            indRight = i;
                        }
                    }
                    vector1.clear();
                }
            }
            s.erase(segs[ind]);
        }
        if (flag) {
            break;
        }
    }
    if (flag) {
        std::cout << "YES" << std::endl;
        std::cout << ind1 << " " << ind2 << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}