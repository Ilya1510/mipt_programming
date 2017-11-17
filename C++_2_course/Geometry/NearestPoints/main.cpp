#include <iostream>
#include <algorithm>

using namespace std;

const double INF = 100000000000;

struct Point {
    double x, y;
    explicit Point() = default;
    Point(double x, double y) : x(x), y(y) {}
    double operator^(const Point& other) const {
        return x * other.y - other.x * y;
    }
    const Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
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

inline bool cmp_y(const Point& a, const Point& b) {
    return a.y < b.y;
}

const int N = 200000;
Point arr[N];

double minDist;

inline void updAns(const Point & a, const Point & b) {
    minDist = min(minDist, a.getDistToPoint(b));
}

void step(int l, int r) {
    if (r - l <= 3) {
        for (int i = l; i <= r; ++i)
            for (int j = i + 1; j <= r; ++j)
                updAns(arr[i], arr[j]);
        sort(arr + l, arr + r + 1, &cmp_y);
        return;
    }
    int m = (l + r) / 2;
    double midX = arr[m].x;
    step(l, m), step(m + 1, r);
    static Point buffer[N];
    merge(arr + l, arr + m + 1, arr + m + 1, arr + r + 1, buffer, &cmp_y);
    copy(buffer, buffer + r - l + 1, arr + l);

    int tsz = 0;
    for (int i = l; i <= r; ++i) {
        if (abs(arr[i].x - midX) < minDist) {
            for (int j = tsz - 1; j >= 0 && arr[i].y - buffer[j].y < minDist; --j)
                updAns(arr[i], buffer[j]);
            buffer[tsz++] = arr[i];
        }
    }
}


int main() {
    cout.precision(20);
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> arr[i].x >> arr[i].y;
    }
    sort (arr, arr + n);
    minDist = INF;
    step(0, n - 1);
    cout << minDist << endl;
    return 0;
}