#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

const int N = 1000;



struct Point {
    double x;
    double y;
    int ind;
    Point() = default;
    Point(const Point& other) = default;
    Point(double x, double y) : x(x), y(y) {};
    bool operator<(const Point& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
    const Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    const Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    const Point getNorm() const {
        return Point(x / sqrt(x * x + y * y), y / sqrt(x * x + y * y));
    }
};

bool upper(Point a, Point b, Point c) {
    return b.x * (a.y - c.y) + b.y * (c.x - a.x) + (a.x * c.y - a.y * c.x) > 0;
}

bool downer(Point a, Point b, Point c) {
    return b.x * (a.y - c.y) + b.y * (c.x - a.x) + (a.x * c.y - a.y * c.x) < 0;
}
vector <Point> getShell(vector <Point>& mas) {
    sort(mas.begin(), mas.end());
    Point p1 = mas[0], p2 = mas.back();

    vector <Point> up, down;
    vector <Point> shell;


    up.push_back(p1);
    down.push_back(p1);
    for (int i = 1; i < (int) mas.size(); i++) {
        if ((i == (int) mas.size() - 1) || (upper(p1, mas[i], p2))) {
            while (((int) up.size() >= 2) && (!upper(up[(int) up.size() - 2], up.back(), mas[i]))) {
                up.pop_back();
            }
            up.push_back(mas[i]);
        }
        if ((i == (int) mas.size() - 1) || (downer(p1, mas[i], p2))) {
            while (((int) down.size() >= 2) && (!downer(down[(int) down.size() - 2], down.back(), mas[i]))) {
                down.pop_back();
            }
            down.push_back(mas[i]);
        }
    }
    for (int i = 0; i < (int) up.size(); i++) {
        shell.push_back(up[i]);
    }
    for (int i = (int) down.size() - 2; i > 0; i--) {
        shell.push_back(down[i]);
    }
    return shell;
}

double getDist(Point a, Point b) {
    Point c = a - b;
    return sqrt(c.x * c.x + c.y * c.y);
}

pair <Point, Point> getBoardPoints(Point p1, Point p2, double L) {
    double a = p2.y - p1.y;
    double b = p1.x - p2.x;
    Point norm = Point(a, b).getNorm();
    Point r1 = Point(p1.x + L * norm.x, p1.y + L * norm.y);
    Point r2 = Point(p1.x - L * norm.x, p1.y - L * norm.y);
    return make_pair(r1, r2);
};

Point arr[N];

int main() {
    cout.precision(20);
    int n, L;
    cin >> n >> L;
    for (int i = 0; i < n; ++i) {
        cin >> (arr[i].x) >> (arr[i].y);
        arr[i].ind = i;
    }
    vector <Point> board;
    for (int i = 0; i < n; ++i) {
        board.push_back(arr[i]);
    }
    vector <Point> shell = getShell(board);
    double ans = 0;
    for (int i = 0; i < (int)shell.size(); ++i) {
        int next = (i + 1) % (int)(shell.size());
        ans += getDist(shell[i], shell[next]);
    }
    cout << ans + 2 * acos(-1) * L << endl;
    return 0;
}