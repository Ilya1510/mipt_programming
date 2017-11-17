#include <iostream>
#include <vector>

using namespace std;

struct Point {
    long long x, y;
    Point() = default;
    Point(long long x, long long y) : x(x), y(y) {}
    double operator^(const Point& other) {
        return x * other.y - other.x * y;
    }
    const Point operator-(const Point& other) {
        return Point(x - other.x, y - other.y);
    }
};

const long long N = 100000;
Point arr[N];

long long n, m, k;

bool inAngle(Point center, Point left, Point right, Point point) {
    Point v1 = left - center;
    Point v2 = point - center;
    Point v3 = right - center;
    return (v2 ^ v1) * (v2 ^ v3) <= 0;
}

bool inTriangle(Point a, Point b, Point c, Point p) {
    return inAngle(a, b, c, p) && inAngle(b, a, c, p) && inAngle(c, a, b, p);
}

bool inPolygon(Point p) {
    long long l = 1;
    long long r = n - 1;
    while (r - l > 1) {
        long long t = (r + l) / 2;
        if (inAngle(arr[0], arr[1], arr[t], p)) {
            r = t;
        } else {
            l = t;
        }
    }
    return inTriangle(arr[0], arr[l], arr[r], p);
}

int main() {
    cin >> n >> m >> k;
    for (long long i = 0; i < n; ++i) {
        cin >> arr[i].x >> arr[i].y;
    }
    int count = 0;
    for (int i = 0; i < m; ++i) {
        long long x, y;
        cin >> x >> y;
        count += inPolygon(Point(x, y));
    }
    if (count >= k) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }
    return 0;
}