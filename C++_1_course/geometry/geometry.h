#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>

using std::cout;
using std::endl;
using std::vector;

const double PI = acos(-1);
const double EPS = 0.000001;

struct Point;
class Line;

struct Point { // and vector in one time
    double x, y;
    Point(double x, double y);
    Point();
    Point& operator/=(double k);
    Point& operator*=(double k);
    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);
    //angle in radiant
    void rotate(const Point& center, double angle);
    double len() const;
    double distance(const Point& other) const;
    //return oriented dist to line
    double distance(const Line& other) const;
    //return new Point parrallel your vect and with a^2 + b^2 = 1;
    const Point getNormalize() const;
    const Point getProjectionToLine(const Line& line) const;
    void reflex(const Point& center);
    void reflex(const Line& axis);
    void scale(const Point& center, double coefficient);
};
bool operator==(const Point& a, const Point& b);
bool operator!=(const Point& a, const Point& b);
double operator^(const Point& a, const Point& b);
const Point operator-(const Point& a, const Point& b);
const Point operator+(const Point& a, const Point& b);
const Point operator/(const Point& a, double k);
const Point operator*(const Point& a, double k);
double operator*(const Point& a, const Point& b);

class Line {
private:
    //a * x + b * y + c = 0;
    double a, b, c;
public:
    //through p1 and p2
    Line(const Point& p1, const Point& p2);
    //through p1 and with direction vector a = a and b = b;
    Line(const Point& p1, double a, double b);
    Line(double k, double d);
    Line(const Point& p1, double k);
    double getA() const;
    double getB() const;
    double getC() const;
    //return vector(Point) dist = 1, direct at line
    const Point direction() const;
    const Point crossingWithLine(const Line& line) const;
};
bool operator==(const Line& l1, const Line& l2);
bool operator!=(const Line& l1, const Line& l2);

Point::Point(double x, double y)
        :x(x), y(y) {}
Point::Point() :x(0), y(0) {}
Point& Point::operator/=(double k) {
    x /= k;
    y /= k;
    return *this;
}
Point& Point::operator*=(double k) {
    x *= k;
    y *= k;
    return *this;
}
Point& Point::operator+=(const Point& other) {
    x += other.x;
    y += other.y;
    return *this;
}
Point& Point::operator-=(const Point& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}
//angle in radiant
void Point::rotate(const Point& center, double angle) {
    *this -= center;
    double x1 = x * cos(angle) - y * sin(angle);
    double y1 = x * sin(angle) + y * cos(angle);
    x = x1;
    y = y1;
    *this += center;
}
double Point::len() const {
    return sqrt(x * x + y * y);
}
double Point::distance(const Point& other) const {
    return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
}
//return oriented dist to line
double Point::distance(const Line& other) const {
    double a = other.getA(), b = other.getB(), c = other.getC();
    return (a * x + b * y + c) / sqrt(a * a + b * b);
}
//return new Point parrallel your vect and with a^2 + b^2 = 1;
const Point Point::getNormalize() const {
    double k = sqrt(x * x + y * y);
    if (fabs(k) < EPS) {
        return Point(0, 0);
    }
    return Point(x / k, y / k);
}
const Point Point::getProjectionToLine(const Line& line) const {
    //a(x0 + at) + b(y0 + bt) + c = 0; 
    double t = -distance(line) / sqrt(line.getA() * line.getA() + line.getB() * line.getB());
    return *this + Point(line.getA(), line.getB()) * t;
}
//we expect that line and *this are crossing in one Point
const Point Line::crossingWithLine(const Line& line) const {
    double x = (line.getC() * b - c * line.getB()) / (a * line.getB() - b * line.getA());
    double y = (c * line.getA() - a * line.getC()) / (a * line.getB() - b * line.getA());
    return Point(x, y);
}
void Point::reflex(const Point& center) {
    *this = center + (center - *this);
}
void Point::reflex(const Line& axis) {
    // (a, b) oriented on positive half-plane
    Point vectNormal(axis.getA(), axis.getB());
    vectNormal = vectNormal.getNormalize();
    double distToLine = distance(axis);
    vectNormal *= distToLine;
    *this = *this - vectNormal * 2;
}
void Point::scale(const Point& center, double coefficient) {
    Point directionalVector = *this - center;
    directionalVector *= coefficient;
    *this = center + directionalVector;
}

bool operator==(const Point& a, const Point& b) {
    return a.distance(b) < EPS;
}
bool operator!=(const Point& a, const Point& b) {
    return !(a == b);
}
double operator^(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}
const Point operator-(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y);
}
const Point operator+(const Point& a, const Point& b) {
    Point res = a;
    res += b;
    return res;
}
const Point operator/(const Point& a, double k) {
    Point res = a;
    res /= k;
    return res;
}
const Point operator*(const Point& a, double k) {
    Point res = a;
    res *= k;
    return res;
}
double operator*(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

//through p1 and p2
Line::Line(const Point& p1, const Point& p2) {
    a = p2.y - p1.y;
    b = p1.x - p2.x;
    c = -a * p1.x - b * p1.y;
}
//through p1 and with direction vector a = a and b = b;
Line::Line(const Point& p1, double a, double b) :a(a), b(b) {
    c = -a * p1.x - b * p1.y;
}
Line::Line(double k, double d) {
    //y = k * x + d => k * x + (-1) * y + d = 0;
    a = k;
    b = -1;
    c = d;
}
Line::Line(const Point& p1, double k) {
    a = k;
    b = -1;
    c = -a * p1.x - b * p1.y;
}
double Line::getA() const {
    return a;
}
double Line::getB() const {
    return b;
}
double Line::getC() const {
    return c;
}
//return vector(Point) dist = 1, direct at line
const Point Line::direction() const {
    return Point(b, -a).getNormalize();
}

bool operator==(const Line& l1, const Line& l2) {
    return (fabs(l1.getA() * l2.getB() - l1.getB() * l2.getA()) < EPS &&
        (l1.getA() * l2.getC() - l1.getC() * l2.getA()) < EPS &&
        (l1.getB() * l2.getC() - l1.getC() * l2.getB()) < EPS);
}
bool operator!=(const Line& l1, const Line& l2) {
    return !(l1 == l2);
}

class Shape;
class Polygon;
class Rectangle;
class Square;
class Triangle;
class Ellipse;
class Circle;

class Shape{
public:
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool operator==(const Shape& other) const = 0;
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(const Point& point) const = 0;

    virtual void rotate(const Point& center, double angle) = 0;
    virtual void reflex(const Point& center) = 0;
    virtual void reflex(const Line& axis) = 0;
    virtual void scale(const Point& center, double coefficient) = 0;
    virtual ~Shape() {}
};

class Polygon : public Shape {
private:
    bool equalWithFixDirection(const vector<Point>& other) const;
    bool isCongruentWithFixDirection(const vector<Point>& other) const;
    bool isSimilarWithFixDirection(const vector<Point>& other) const;
protected:
    vector <Point> vertices;
public:
    Polygon(const vector<Point>& vertices);
    template <class... Args>
    Polygon(Args... args);
    template <class... Args>
    void init(const Point& p, Args... args);
    void init();
    size_t verticesCount() const;
    const std::vector<Point> getVertices() const;
    bool isConvex() const;

    double perimeter() const override;
    double area() const override;
    bool operator==(const Shape& other) const override;
    bool operator!=(const Shape& other) const;
    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;

    bool containsPoint(const Point& point) const override;
    void rotate(const Point& center, double angle) override;
    void reflex(const Point& center) override;
    void reflex(const Line& axis) override;
    void scale(const Point& center, double coefficient) override;
};

class Rectangle : public Polygon {
public:
    Rectangle(const Point& a, const Point& b, double k);
    Rectangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4);
    const Point center() const;
    const std::pair<Line, Line> diagonals() const;
};

class Square : public Rectangle {
public:
    Square(const Point &p1, const Point& p2);
    const Circle circumscribedCircle() const;
    const Circle inscribedCircle() const;
};

class Ellipse : public Shape {
protected:
    double a, b;
    Line axis;
    Point pointCenter;
    std::pair<Point, Point> pointFocuses;
public:
    Ellipse(const Point& f1, const Point& f2, double dist);
    const Point center() const;
    const std::pair<Point, Point> focuses() const;
    double eccentricity() const;
    const std::pair<Line, Line> directrixes() const;

    double perimeter() const override;
    double area() const override;
    bool operator==(const Shape& other) const override;
    bool operator!=(const Shape& other) const;
    bool isCongruentTo(const Shape& another) const override;
    bool isSimilarTo(const Shape& another) const override;

    bool containsPoint(const Point& point) const override;
    void rotate(const Point& center, double angle) override;
    void reflex(const Point& center) override;
    void reflex(const Line& axis) override;
    void scale(const Point& center, double coefficient) override;
};

class Triangle : public Polygon {
public:
    template <class... Args>
    Triangle(Args... args);
    const Circle circumscribedCircle() const;
    const Circle inscribedCircle() const;
    const Point centroid() const;
    const Point orthocenter() const;
    const Line EulerLine() const;
    const Circle ninePointsCircle() const;
};

class Circle : public Ellipse {
public:
    Circle(const Point& center, double r);
    double radius() const;
};

Polygon::Polygon(const vector<Point>& vertices) :vertices(vertices) {

}

template <class... Args>
Polygon::Polygon(Args... args) {
    init(args...);
}

template <class... Args>
void Polygon::init(const Point& p, Args... args) {
    //делаем что-то с p
    vertices.push_back(p);
    //вызываем себя же, но без первого аргумента
    init(args...);
}

void Polygon::init() {
    return;
}

size_t Polygon::verticesCount() const {
    return vertices.size();
}

const std::vector<Point> Polygon::getVertices() const {
    return vertices;
}

bool Polygon::isConvex() const {
    unsigned int countPositiveMultiply = 0;
    unsigned int countNegativeMultiply = 0;
    //for every vertice check angle between neigbour sides
    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t predVertice = (i + vertices.size() - 1) % vertices.size();
        size_t nextVertice = (i + 1) % vertices.size();
        Point vec1 = vertices[predVertice] - vertices[i];
        Point vec2 = vertices[nextVertice] - vertices[i];
        if ((vec1 ^ vec2) > 0) {
            ++countPositiveMultiply;
        }
        else if ((vec1 ^ vec2) < 0) {
            ++countNegativeMultiply;
        }
    }
    //if all product < 0 or all product > 0, then convex
    return !(countNegativeMultiply * countPositiveMultiply);
}

double Polygon::perimeter() const {
    double sum = 0;
    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t next = (i + 1) % vertices.size();
        sum += vertices[i].distance(vertices[next]);
    }
    return sum;
}

double Polygon::area() const {
    double sum = 0;
    //from begin of coordinate
    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t next = (i + 1) % vertices.size();
        sum += vertices[i] ^ vertices[next];
    }
    return fabs(sum) / 2;
}

bool Polygon::equalWithFixDirection(const vector<Point>& other) const {
    size_t verticesCnt = verticesCount();
    if (verticesCnt != other.size()) {
        return false;
    }
    for (size_t i = 0; i < verticesCnt; ++i) {
        bool prov = true;
        for (size_t j = 0; j < verticesCnt; ++j) {
            size_t ind = (j + i) % verticesCnt;
            if (vertices[j] != other[ind]) {
                prov = false;
            }
        }
        if (prov) {
            return true;
        }
    }
    return false;
}

bool Polygon::operator==(const Shape& other) const {
    try {
        const Polygon& polygon = dynamic_cast<const Polygon&>(other);
        //we think that no three vertices lie on one line!!!
        vector <Point> otherPoly = polygon.getVertices();
        bool equalRight = equalWithFixDirection(otherPoly);
        std::reverse(otherPoly.begin(), otherPoly.end());
        bool equalReverse = equalWithFixDirection(otherPoly);
        return equalRight || equalReverse;
    }
    catch (std::bad_cast&) {
        return false;
    }
    return false;
}

bool Polygon::operator!=(const Shape& other) const {
    return !(*this == other);
}

bool Polygon::isCongruentWithFixDirection(const vector<Point>& other) const {
    size_t verticesCnt = verticesCount();
    if (verticesCnt != other.size()) {
        return false;
    }
    for (size_t i = 0; i < verticesCnt; ++i) {
        bool prov = true;
        for (size_t j = 0; j < verticesCnt; ++j) {
            size_t pred = (j + verticesCnt - 1) % verticesCnt;
            size_t cur = j;
            size_t next = (j + 1) % verticesCnt;
            Point vectLeft1 = vertices[pred] - vertices[cur];
            Point vectRight1 = vertices[next] - vertices[cur];
            size_t pred2 = (i + j + verticesCnt - 1) % verticesCnt;
            size_t cur2 = (i + j) % verticesCnt;
            size_t next2 = (i + j + 1) % verticesCnt;
            Point vectLeft2 = other[pred2] - other[cur2];
            Point vectRight2 = other[next2] - other[cur2];
            if (fabs(vectLeft1.len() - vectLeft2.len()) > EPS || fabs(vectRight1.len() - vectRight2.len()) > EPS ||
                    fabs((vectLeft1 * vectRight1 - vectLeft2 * vectRight2)) > EPS) {
                prov = false;
            }
        }
        if (prov) {
            return true;
        }
    }
    return false;
}

bool Polygon::isCongruentTo(const Shape& another) const {
    try {
        const Polygon& polygon = dynamic_cast<const Polygon&>(another);
        //we think that no three vertices lie on one line!!!
        vector <Point> otherPoly = polygon.getVertices();
        bool congruentRight = isCongruentWithFixDirection(otherPoly);
        std::reverse(otherPoly.begin(), otherPoly.end());
        bool congruentReverse = isCongruentWithFixDirection(otherPoly);
        return congruentRight || congruentReverse;
    }
    catch (std::bad_cast&) {
        return false;
    }
    return false;
}

bool Polygon::isSimilarWithFixDirection(const vector<Point>& other) const {
    size_t verticesCnt = verticesCount();
    if (verticesCnt != other.size()) {
        return false;
    }
    for (size_t i = 0; i < verticesCnt; ++i) {
        bool prov = true;
        for (size_t j = 0; j < verticesCnt; ++j) {
            size_t pred = (j + verticesCnt - 1) % verticesCnt;
            size_t next = (j + 1) % verticesCnt;
            Point vectLeft1 = vertices[pred] - vertices[j];
            Point vectRight1 = vertices[next] - vertices[j];
            double s1 = vertices[j].distance(vertices[pred]) * vertices[j].distance(vertices[next]);
            double k1 = vertices[j].distance(vertices[pred]) / vertices[j].distance(vertices[next]);
            size_t pred2 = (i + j + verticesCnt - 1) % verticesCnt;
            size_t cur = (i + j) % verticesCnt;
            size_t next2 = (i + j + 1) % verticesCnt;
            Point vectLeft2 = other[pred2] - other[cur];
            Point vectRight2 = other[next2] - other[cur];
            double s2 = other[cur].distance(other[pred2]) * other[cur].distance(other[next2]);
            double k2 = other[cur].distance(other[pred2]) / other[cur].distance(other[next2]);
            if (fabs(fabs((vectLeft1 ^ vectRight1) / s1) - fabs((vectLeft2 ^ vectRight2) / s2)) > EPS ||
                    fabs((vectLeft1 * vectRight1) / s1 - (vectLeft2 * vectRight2) / s2) > EPS ||
                    fabs(k1 - k2) > EPS) {
                prov = false;
            }
            if (!prov) {
                break;
            }
        }
        if (prov) {
            return true;
        }
    }
    return false;
}

bool Polygon::isSimilarTo(const Shape& another) const {
    try {
        const Polygon& polygon = dynamic_cast<const Polygon&>(another);
        //we think that no three vertices lie on one line!!!
        vector <Point> otherPoly = polygon.getVertices();
        bool similarRight = isSimilarWithFixDirection(otherPoly);
        std::reverse(otherPoly.begin(), otherPoly.end());
        bool similarReverse = isSimilarWithFixDirection(otherPoly);
        return similarRight || similarReverse;
    }
    catch (std::bad_cast&) {
        return false;
    }
    return false;
}

bool Polygon::containsPoint(const Point& point) const {
    double xMax = vertices[0].x;
    for (size_t i = 1; i < verticesCount(); ++i) {
        xMax = std::max(xMax, vertices[i].x);
    }
    Point finish(xMax + 1, point.y + 1);
    size_t cnt = 0;
    for (size_t i = 0; i < verticesCount(); ++i) {
        size_t next = (i + 1) % verticesCount();
        Point p1 = vertices[i];
        Point p2 = vertices[next];
        if (((p1 - point) ^ (finish - point)) * ((p2 - point) ^ (finish - point)) < 0 &&
            (((point - p1) ^ (p2 - p1)) * ((finish - p1) ^ (p2 - p1))) < 0) {
            ++cnt;
        }
    }
    if (cnt % 2) {
        return true;
    }
    else {
        return false;
    }
}

//angle in gradus
void Polygon::rotate(const Point& center, double angle) {
    angle = angle / 180 * PI;
    Polygon prev = *this;
    for (size_t i = 0; i < verticesCount(); ++i) {
        vertices[i].rotate(center, angle);
    }
}

void Polygon::reflex(const Point& center) {
    Polygon prev = *this;
    for (size_t i = 0; i < verticesCount(); ++i) {
        vertices[i].reflex(center);
    }
}

void Polygon::reflex(const Line& axis) {
    Polygon prev = *this;
    for (size_t i = 0; i < verticesCount(); ++i) {
        vertices[i].reflex(axis);
    }
}

void Polygon::scale(const Point& center, double coefficient) {
    Polygon prev = *this;
    for (size_t i = 0; i < verticesCount(); ++i) {
        vertices[i].scale(center, coefficient);
    }
}

Ellipse::Ellipse(const Point& f1, const Point& f2, double dist)
        :axis(f1, f2), pointFocuses(f1, f2) {
    pointCenter = (f1 + f2) / 2;
    a = dist / 2;
    // c^2 = a^2 - b^2
    double c = f1.distance(f2) / 2;
    b = sqrt(a * a - c * c);
}

const Point Ellipse::center() const {
    return pointCenter;
}

const std::pair<Point, Point> Ellipse::focuses() const {
    return pointFocuses;
}

double Ellipse::eccentricity() const {
    double c = sqrt(a * a - b * b);
    return c / a;
}

const std::pair<Line, Line> Ellipse::directrixes() const {
    double displaceDist = a / eccentricity();
    Point displaceFromCenter = axis.direction().getNormalize() * displaceDist;
    //(a, b) perpendicular (b, -a)
    Line directrixOne = Line(pointCenter - displaceFromCenter, axis.getB(), -axis.getA());
    Line directrixTwo = Line(pointCenter + displaceFromCenter, axis.getB(), -axis.getA());
    return std::make_pair(directrixOne, directrixTwo);
}

double Ellipse::perimeter() const {
    return PI * (3 * (a + b) - sqrt((3 * a + b) * (a + 3 * b)));
}

double Ellipse::area() const {
    return PI * a * b;
}

bool Ellipse::operator==(const Shape& other) const {
    try {
        const Ellipse& ellipse = dynamic_cast<const Ellipse&>(other);
        return (((pointFocuses.first == ellipse.pointFocuses.first && pointFocuses.second == ellipse.pointFocuses.second) ||
            (pointFocuses.first == ellipse.pointFocuses.second && pointFocuses.second == ellipse.pointFocuses.first)) &&
            a == ellipse.a);
    } catch (std::bad_cast&) {
        return false;
    }
}

bool Ellipse::operator!=(const Shape& other) const {
    return !(*this == other);
}

bool Ellipse::isCongruentTo(const Shape& another) const {
    try {
        const Ellipse& ellipse = dynamic_cast<const Ellipse&>(another);
        return a == ellipse.a && b == ellipse.b;
    }
    catch (std::bad_cast&) {
        return false;
    }
}

bool Ellipse::isSimilarTo(const Shape& another) const {
    try {
        const Ellipse& ellipse = dynamic_cast<const Ellipse&>(another);
        return a / ellipse.a == b / ellipse.b;
    }
    catch (std::bad_cast&) {
        return false;
    }
}

bool Ellipse::containsPoint(const Point& point) const {
    return point.distance(pointFocuses.first) + point.distance(pointFocuses.second) <= 2 * a;
}

void Ellipse::rotate(const Point& center, double angle) {
    angle = angle / 180 * PI;
    pointFocuses.first.rotate(center, angle);
    pointFocuses.second.rotate(center, angle);
    axis = Line(pointFocuses.first, pointFocuses.second);
    pointCenter = (pointFocuses.first + pointFocuses.second) / 2;
}

void Ellipse::reflex(const Point& center) {
    pointFocuses.first.reflex(center);
    pointFocuses.second.reflex(center);
    axis = Line(pointFocuses.first, pointFocuses.second);
    pointCenter = (pointFocuses.first + pointFocuses.second) / 2;
}

void Ellipse::reflex(const Line& axis) {
    pointFocuses.first.reflex(axis);
    pointFocuses.second.reflex(axis);
    Ellipse::axis = Line(pointFocuses.first, pointFocuses.second);
    pointCenter = (pointFocuses.first + pointFocuses.second) / 2;
}

void Ellipse::scale(const Point& center, double coefficient) {
    pointFocuses.first.scale(center, coefficient);
    pointFocuses.second.scale(center, coefficient);
    Ellipse::axis = Line(pointFocuses.first, pointFocuses.second);
    pointCenter = (pointFocuses.first + pointFocuses.second) / 2;
    a = std::fabs(a * coefficient);
    b = std::fabs(b * coefficient);
}

Circle::Circle(const Point& center, double r)
        :Ellipse(center, center, 2 * r) {}

double Circle::radius() const {
    return a;
}

const Circle Triangle::circumscribedCircle() const {
    Point a = vertices[0];
    Point b = vertices[1];
    Point c = vertices[2];
    Point medianaBC = (b + c) / 2;
    Point medianaAC = (a + c) / 2;
    Line AC(a, c);
    Line BC(b, c);
    Line serPerB(medianaAC, AC.getB(), -AC.getA());
    Line serPerA(medianaBC, BC.getB(), -BC.getA());
    Point center = serPerA.crossingWithLine(serPerB);
    return Circle(center, center.distance(a));
}
const Circle Triangle::inscribedCircle() const {
    Point a = vertices[0];
    Point b = vertices[1];
    Point c = vertices[2];
    Point c1 = a + (b - a) * c.distance(a) / (c.distance(b) + c.distance(a));
    Point b1 = a + (c - a) * b.distance(a) / (b.distance(c) + b.distance(a));
    Line bisectrixB(b, b1);
    Line bisectrixC(c, c1);
    Point center = bisectrixB.crossingWithLine(bisectrixC);
    Circle circumCircle = circumscribedCircle();
    double radius = (circumCircle.radius() * circumCircle.radius() -
        center.distance(circumCircle.center()) * center.distance(circumCircle.center())) /
        (2 * circumCircle.radius());
    return Circle(center, radius);
}
const Point Triangle::centroid() const {
    Point center = (vertices[0] + vertices[1] + vertices[2]) / 3;
    return center;
}
const Point Triangle::orthocenter() const {
    Point a = vertices[0];
    Point b = vertices[1];
    Point c = vertices[2];
    Line AB(a, b);
    Line AC(a, c);
    Point hc = c.getProjectionToLine(AB);
    Point hb = b.getProjectionToLine(AC);
    Line BH(b, hb);
    Line CH(c, hc);
    return BH.crossingWithLine(CH);
}
const Line Triangle::EulerLine() const {
    return Line(centroid(), orthocenter());
}
const Circle Triangle::ninePointsCircle() const {
    Point ninePointsCenter = (circumscribedCircle().center() + orthocenter()) / 2;
    Point middleSide = (vertices[0] + vertices[1]) / 2;
    return Circle(ninePointsCenter, ninePointsCenter.distance(middleSide));
}

Rectangle::Rectangle(const Point& a, const Point& b, double k) {
    double angle = atan(k);
    angle = angle > PI / 4 ? angle : PI / 2 - angle;
    Point verticeSecond = b;
    Point verticeFourth = b;
    verticeSecond.rotate(a, angle);
    verticeSecond.scale(a, cos(angle));
    verticeFourth.rotate(a, -(PI / 2 - angle));
    verticeFourth.scale(a, sin(angle));
    vertices.push_back(a);
    vertices.push_back(verticeSecond);
    vertices.push_back(b);
    vertices.push_back(verticeFourth);
}

Rectangle::Rectangle(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    Polygon(p1, p2, p3, p4);
}

const Point Rectangle::center() const {
    Point centerMass;
    for (size_t i = 0; i < 4; ++i) {
        centerMass += vertices[i];
    }
    centerMass /= 4;
    return centerMass;
}
const std::pair<Line, Line> Rectangle::diagonals() const {
    Line diagonalOne(vertices[0], vertices[2]);
    Line diagonalTwo(vertices[1], vertices[3]);
    return std::make_pair(diagonalOne, diagonalTwo);
}

Square::Square(const Point &p1, const Point& p2)
        :Rectangle(p1, p2, static_cast<double>(1)) {
}

const Circle Square::circumscribedCircle() const {
    return Circle(center(), perimeter() / (4 * sqrt(2)));
}
const Circle Square::inscribedCircle() const {
    return Circle(center(), perimeter() / 8);
}

template <class... Args>
Triangle::Triangle(Args... args) : Polygon(args...) {

}