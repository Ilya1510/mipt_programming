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

const long long N = 100000;
Point arr[N];
int color[N];
int curColor = 1;
int n;

bool isMoreGoodSecond(Point v1, Point v2) {
    return ((v1 ^ v2) < 0) ||
            (((v1 ^ v2) == 0) && v1.x * v1.x + v1.y * v1.y > v2.x * v2.x + v2.y * v2.y &&
            v2.x * v2.x + v2.y + v2.y != 0);
}

bool findShell() {
    int indBegin = -1;
    for (int i = 0; i < n; ++i) {
        if (color[i] == 0 && (indBegin == -1 || arr[i] < arr[indBegin])) {
            indBegin = i;
        }
    }
    if (indBegin == -1) {
        return false;
    }
    int indCur = indBegin;
    int indNext = -1;
    color[indCur] = curColor;
    while (indNext != indBegin) {
        indNext = -1;
        for (int i = 0; i < n; ++i) {
            if (color[i] == 0 || color[i] == curColor) {
                if (indNext == -1 || indNext == indCur ||
                        isMoreGoodSecond(arr[indNext] - arr[indCur], arr[i] - arr[indCur])) {
                    indNext = i;
                }
            }
        }
        indCur = indNext;
        color[indCur] = curColor;
    }
    return true;
}

int main() {

    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> arr[i].x >> arr[i].y;
    }
    while (findShell()) {
        curColor+= 1;
    }
    for (int i = 0; i < n; ++i) {
        cout << color[i] << endl;
    }
    return 0;
}