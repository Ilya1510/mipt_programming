#include <iostream>
#include <algorithm>

using namespace std;

const double EPS = 0.000001;

struct Point {
    double x, y;
    explicit Point() = default;
    explicit Point(double x, double y) : x(x), y(y) {}
    double operator^(const Point& other) const {
        return x * other.y - other.x * y;
    }
    const Point operator*(double k) const {
        return Point(x * k, y * k);
    }
    double operator*(const Point& other) const {
        return x * other.x + y * other.y;
    }
    const Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    const Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
    }
    double getLen() const {
        return sqrt(x * x + y * y);
    }
    double getDistToPoint(const Point& other) const {
        Point d = *this - other;
        return d.getLen();
    }
};

struct Ray {
    Point center;
    Point point;
    double A, B, C;
    Ray(const Point& a, const Point& b) : center(a), point(b) {
        A = b.y - a.y;
        B = a.x - b.x;
        C = -A * a.x - B * a.y;
    }
    bool isOn(const Point& other) const {
        Point p1 = point - center;
        Point p2 = other - center;
        return abs((p1 * p2) - p1.getLen() * p2.getLen()) < EPS;
    }
    double getDistFromPoint(const Point& other) const {
        Point v1 = other - center;
        Point v2 = point - center;
        double pr = (v1 * v2) / v2.getLen();
        Point res = center + (v2 * (pr / v2.getLen()));
        if (isOn(res)) {
            return other.getDistToPoint(res);
        } else {
            return other.getDistToPoint(center);
        }
    }
    bool isRaysIntersects(const Ray& other) const {
        if (abs(A * other.B - B * other.A) < EPS) {
            return false;
        } else {
            double x = - (C * other.B - other.C * B) / (A * other.B - B * other.A);
            double y = - (A * other.C - C * other.A) / (A * other.B - B * other.A);
            Point p(x, y);
            return isOn(p) && other.isOn(p);
        }
    }
};

double getDistBetweenRays(Point a, Point b, Point c, Point d) {
    Ray r1(a, b);
    Ray r2(c, d);
    if (r1.isRaysIntersects(r2)) {
        return 0;
    } else {
        return min(r1.getDistFromPoint(c), r2.getDistFromPoint(a));
    }
}

int main() {
    cout.precision(20);
    Point a, b, c, d;
    cin >> a.x >> a.y >> b.x >> b.y >> c.x >> c.y >> d.x >> d.y;
    cout << getDistBetweenRays(a, b, c, d) << endl;
    return 0;
}