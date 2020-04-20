#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>

#define INVALID_COORDINATE -1
#define INVALID_LINE_ID -1

using namespace std;

class Point {
public:
    Point(float x = INVALID_COORDINATE, float y = INVALID_COORDINATE)
        : x(x)
        , y(y) {
    }

    bool operator == (const Point& other) {
        return x == other.x && y == other.y;
    }

    Point operator + (const Point& other) {
        return Point(x + other.x, y + other.y);
    }

    Point& operator += (const Point& other) {
        x += other.x;
        y += other.y;

        return *this;
    }

public:
    float x;
    float y;
};

inline float distance(const Point& p0, const Point& p1) {
    auto dx = p0.x - p1.x;
    auto dy = p0.y - p1.y;

    return sqrt(dx*dx + dy*dy);
}

inline float area(const Point& p0, const Point& p1, const Point& p2) {
    return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
}

class Segment {
public:
    Segment(const Point &p0, const Point &p1)
        : p0(p0)
        , p1(p1) {
        length = distance(p0, p1);
    }

public:
    Point p0;
    Point p1;

    float length = 0;
};

inline bool bounding_box(float a0, float a1, float b0, float b1) {
    if (a0 > a1) {
        swap(a0, a1);
    }
    if (b0 > b1) {
        swap(b0, b1);
    }

    return max(a0, b0) <= min(a1, b1);
}

inline bool cross(const Segment& s0, const Segment& s1) {
    return bounding_box(s0.p0.x, s0.p1.x, s1.p0.x, s1.p1.x) &&
           bounding_box(s0.p0.y, s0.p1.y, s1.p0.y, s1.p1.y) &&
           area(s0.p0, s0.p1, s1.p0) * area(s0.p0, s0.p1, s1.p1) <= 0 &&
           area(s1.p0, s1.p1, s0.p0) * area(s1.p0, s1.p1, s0.p1) <= 0;
}

class Surface {
public:
    void addSegment(const Point& p0, const Point& p1) {
        return addSegment(Segment(p0, p1));
    }

    void addSegment(const Segment& line) {
        segments.push_back(line);
    }

    void findLandingSegment() {
        landing_line_idx = INVALID_LINE_ID;

        for (size_t i = 0; i < segments.size(); ++i) {
            if (segments[i].p0.x == segments[i].p1.x) {
                landing_line_idx = i;

                break;
            }
        }
    }

    inline int landingSegment() const noexcept {
        return landing_line_idx;
    }

    int collide(const Segment& trajectory) const {
        for (size_t i = 0; i < segments.size(); ++i) {
            if (cross(trajectory, segments[i])) {
                return i;
            }
        }

        return INVALID_LINE_ID;
    }

public:
    vector < Segment > segments;

    int landing_line_idx = INVALID_LINE_ID;
};

class Lander {
public:
    Lander() {}

public:
    Point position;
    Point speed;

    int fuel;
    int rotate;
    int power;
};

int main() {
    cout << "Hello world!" << endl;
    return 0;
}
