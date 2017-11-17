#include <iostream>

using namespace std;

struct Point {
    long long x, y;
    explicit Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
    double operator^(const Point& other) {
        return x * other.y - other.x * y;
    }
    const Point operator-(const Point& other) {
        return Point(x - other.x, y - other.y);
    }
    bool operator<(const Point& other) {
        return x < other.x || (x == other.x && y < other.y);
    }
};

bool PointOnSegment(Point a, Point b, Point c) {
    long long A = a.y - b.y;
    long long B = b.x - a.x;
    long long C = -A * a.x - B * a.y;
    if (A * c.x + B * c.y + C != 0) {
        return false;
    }
    // c on line a, b
    return c.x >= min(a.x, b.x) && c.x <= max(a.x, b.x) &&
                                  c.y >= min(a.y, b.y) && c.y <= max(a.y, b.y);
}

int main() {
    Point a, b, c;
    cin >> c.x >> c.y >> a.x >> a.y >> b.x >> b.y;
    if (PointOnSegment(a, b, c)) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }
    return 0;
}