#pragma once

#include "Point.h"

#include <algorithm>

class Segment {
public:
    Segment(const Point &p0, const Point &p1) noexcept
        : p0(p0)
        , p1(p1) {
        length = distance(p0, p1);
    }

public:
    Point p0;
    Point p1;

    float length = 0;
};

inline bool bounding_box(float a0, float a1, float b0, float b1) noexcept {
    if (a0 > a1) {
        std::swap(a0, a1);
    }
    if (b0 > b1) {
        std::swap(b0, b1);
    }

    return std::max(a0, b0) <= std::min(a1, b1);
}

inline bool cross(const Segment& s0, const Segment& s1) noexcept {
    return bounding_box(s0.p0.x, s0.p1.x, s1.p0.x, s1.p1.x) &&
           bounding_box(s0.p0.y, s0.p1.y, s1.p0.y, s1.p1.y) &&
           area(s0.p0, s0.p1, s1.p0) * area(s0.p0, s0.p1, s1.p1) <= 0 &&
           area(s1.p0, s1.p1, s0.p0) * area(s1.p0, s1.p1, s0.p1) <= 0;
}

