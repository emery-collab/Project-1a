#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <cmath>
#include <iostream>

namespace CMPUT350 {

struct Point2D {
    float x, y;
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}
    double Distance(const Point2D &other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        double d = double(sqrt((dx*dx) + (dy*dy)));
        return d;
    }
    Point2D operator+(const Point2D &other) const {
        
        return Point2D(x + other.x, y + other.y);
    }
    Point2D operator+(const float &other) const {
       
        return Point2D(x + other, y + other);
    }
    Point2D operator-(const Point2D &other) const {
        return Point2D(x - other.x, y - other.y);
    }
    Point2D operator-(const float &other) const {
        return Point2D(x - other, y - other);
    }
    Point2D operator*(const float &scalar) const {
        return Point2D(x * scalar, y * scalar);
    }
    Point2D &operator+=(const float &scalar) {
        x += scalar;
        y += scalar;
        return *this;
    }
    Point2D &operator+=(const Point2D &other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Point2D &operator-=(const Point2D &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    bool operator==(const Point2D &other) const {
        if(x == other.x && y == other.y){
            return true;
        }
        return false;
    }
    Point2D &operator*=(const int &scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Point2D &operator/=(const int &scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    float operator*(const Point2D &other) const {
        return x * other.x + y * other.y;
    }
    float Dot(Point2D b) const {
        return x * b.x + y * b.y;
    }
    static float Dot(Point2D a, Point2D b) {
        return a.x * b.x + a.y * b.y;
    }
    static float Cross(Point2D a, Point2D b) {
        return a.x * b.y - a.y * b.x;
    }    
    float Cross(Point2D b) const{
        return x * b.y - y * b.x;
    }
    void Normalize() {
        if(x != 0 || y != 0){
            double dist = Distance(Point2D(0,0));
            x = x/dist;
            y = y/dist;
        }
    }
};

static std::ostream &operator<<(std::ostream &os, const Point2D &p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

static Point2D operator*(float number, const Point2D &rhs) {
    return Point2D(number * rhs.x , number * rhs.y);
}

struct Line {
    Point2D p1, p2;

    Line(Point2D p1 = {0, 0}, Point2D p2 = {0, 0}) : p1(p1), p2(p2) {

    }
    Line(float x1, float y1, float x2, float y2) : p1(x1, y1), p2(x2, y2) {

    }
    float Length() const {
       return p1.Distance(p2);
    }
    Point2D ClosestPoint(const Point2D &p) const {

        Point2D direction = p2 - p1;
        Point2D fromStart = p - p1;

        float lengthSquared = direction.Dot(direction);

        // A segment with a length of 0 is just a single point
        if (lengthSquared == 0.0f) {
            return p1;
        }

        float t = fromStart.Dot(direction) / lengthSquared;

        // Restrict the projection to the segment [p1, p2]
        t = std::max(0.0f, std::min(1.0f, t));
        
        return p1 + direction * t;

        /*
        if(p.Distance(p1)< p.Distance(p2)){
            return p1;
        }
        else{
            return p2;
        }
        */
    }
    bool Crosses(Line other, Point2D &crossingPoint) const {

        // Get the variables in the equation we derived from class "((c-a) x cd / ab x cd) = t or ((a-c) x ab / cd x ab) = t"
        // Take the cross for the denominator in both
        Point2D ab = p2 - p1;
        Point2D cd = other.p2 - other.p1;
        Point2D cMinusA = other.p1 - p1;

        float denominator = ab.Cross(cd);

        // This section checks for the denominator being 0, if so they are parellel and never cross
        // The epsilon is a tiny number used so that in case floating point arithmetic gives us something
        // like 0.000000000001 when it should be 0, we still compare correctly. Checking less than epsilon
        // instead of denominator == 0 was recommended by AI
        constexpr float epsilon = 1e-6f;
        if (std::abs(denominator) < epsilon) {
            return false;
        }

        // Here we plug into the formulas written in the initial comment
        float u = cMinusA.Cross(ab) / denominator;
        float t = cMinusA.Cross(cd) / denominator;

        // Check if the intersection lies on both segments
        // If we don't check both, we are only checking line to segment intersection, not segment to segment
        if (t < 0.0f || t > 1.0f || u < 0.0f || u > 1.0f) {
            return false;
        }

        // We know there is collision now
        // Set the crossing point equal to the starting point of line one + the scalar t applied to the direction vector between a and b
        crossingPoint = p1 + ab * t;
        return true;

    }
};

static std::ostream &operator<<(std::ostream &os, const Line &l) {
    os << "(" << l.p1 << " -> " << l.p2 << ")";
    return os;
}

struct Circle {
    Point2D center;
    float radius;

    Circle(Point2D c = {0, 0}, float r = 0) : center(c), radius(r) {

    }

    Circle(float x, float y, float r) : center(x, y), radius(r) {
        
    }
};

struct Rect {
    Point2D topLeft;
    float width, height;

    Rect(float left, float top, float width, float height)
        : topLeft(Point2D(left,top )), width(width), height(height) {//switched left and top
            }

    Rect(Point2D tl = {0, 0}, int w = 0, int h = 0) : topLeft(tl), width(w), height(h) {}

    // Creates bounding box around p1 and p2 with positive width/height
    Rect(Point2D p1, Point2D p2)
        : topLeft(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
          width(fabs(p1.x - p2.x)),
          height(fabs(p1.y - p2.y)) {}

    Rect(Point2D center, float radius)
        : topLeft(center.x - radius, center.y - radius), width(2 * radius), height(2 * radius) {}

    Rect &operator|=(const Rect &other) {
        float left =  std::min(topLeft.x, other.topLeft.x);
        float top =  std::min(topLeft.y, other.topLeft.y);
        float right = std::max(topLeft.x + width, other.topLeft.x + other.width);
        float bottom = std::max(topLeft.y + height, other.topLeft.y + other.height);
        topLeft = Point2D(left, top);
        width = (right -left);
        height = (bottom - top);
        return *this;
    }
    Rect &operator|=(const Point2D &other) {
        float left =  std::min(topLeft.x, other.x);
        float top =  std::min(topLeft.y, other.y);
        float right = std::max(topLeft.x + width, other.x);
        float bottom = std::max(topLeft.y + height, other.y);
        topLeft = Point2D(left, top);
        width = (right -left);
        height = (bottom - top);
        return *this;
    }
    Rect &operator|=(const Line &other) {
        *this |= other.p1;
        *this |= other.p2;
        return *this;
    }
    Rect &operator&=(const Rect &other) {
        float left =  std::max(topLeft.x, other.topLeft.x);
        float top =  std::max(topLeft.y, other.topLeft.y);
        float right = std::min(topLeft.x + width, other.topLeft.x + other.width);
        float bottom = std::min(topLeft.y + height, other.topLeft.y + other.height);
        topLeft = Point2D(left, top);
        width = (right -left);
        height = (bottom - top);
        if(width < 0 ||height < 0){
            width = 0;
            height = 0;
        }
        return *this;
    }
    Rect &operator+=(const Point2D &other) {
        topLeft += other;
        return *this;
    }
    Rect operator+(const Point2D &other) const {
        Rect result = *this;
        result += other;
        return *this;
    }
    void Inset(int inset) {
        if(inset > 0)
        {
            topLeft = Point2D(topLeft.x +inset, topLeft.y + inset);
            //each side lost the inset amount (right and left lose inset etc)
            width = width-(2*inset);
            height = height-(2*inset);
        }

    }
    bool IsInside(const Point2D &p) const {
        if(topLeft.x <= p.x && p.x<= topLeft.x + width && 
            topLeft.y <= p.y && p.y <= topLeft.y + height ){
            return true;
        }
        return false;
    }
};

static std::ostream &operator<<(std::ostream &os, const Rect &l) {
    os << "Rect(topLeft=" << l.topLeft << ", w: " << l.width << ", h: " << l.height << ")";
    return os;
}

}  // namespace CMPUT350

#endif  // MATHUTIL_H
