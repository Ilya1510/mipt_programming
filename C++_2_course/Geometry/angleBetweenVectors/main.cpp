#include <iostream>
#include <algorithm>

using namespace std;

struct Vector {
    double x, y;
    Vector (double x, double y) : x(x), y(y) {}
    double operator*(Vector other) const {
        return x * other.x + y * other.y;
    }
    double getLen() const {
        return sqrt(x * x + y * y);
    }
};

double getAngle(Vector a, Vector b) {
    return acos((a * b) / a.getLen() / b.getLen());
}

int main() {
    cout.precision(20);
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    cout << getAngle(Vector(x1, y1), Vector(x2, y2)) << endl;
    return 0;
}