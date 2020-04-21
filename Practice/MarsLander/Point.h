#pragma once

#include <math.h>

class Point {
public:
    Point() noexcept {

    }

    Point(float x, float y) noexcept
        : x(x)
        , y(y) {
    }

    bool operator == (const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }

    Point operator + (const Point& other) const noexcept {
        return Point(x + other.x, y + other.y);
    }

    Point& operator += (const Point& other) noexcept {
        x += other.x;
        y += other.y;

        return *this;
    }

    Point operator - (const Point& other) const noexcept {
        return Point(x - other.x, y - other.y);
    }

    Point& operator -= (const Point& other) noexcept {
        x -= other.x;
        y -= other.y;

        return *this;
    }

    Point operator * (const Point& other) const noexcept {
        return Point(x * other.x, y * other.y);
    }

    Point& operator *= (const Point& other) noexcept {
        x *= other.x;
        y *= other.y;

        return *this;
    }

    Point operator / (const Point& other) const {
        return Point(x / other.x, y / other.y);
    }

    Point& operator /= (const Point& other) {
        x /= other.x;
        y /= other.y;

        return *this;
    }

    bool valid() const noexcept {
        return x != COORDINATE_INVALID && y != COORDINATE_INVALID;
    }

public:
    float x = COORDINATE_INVALID;
    float y = COORDINATE_INVALID;

public:
    static constexpr float COORDINATE_INVALID = -1;
};

inline float distance(const Point& p0, const Point& p1) noexcept {
    auto dx = p0.x - p1.x;
    auto dy = p0.y - p1.y;

    return sqrt(dx*dx + dy*dy);
}

inline float area(const Point& p0, const Point& p1, const Point& p2) noexcept {
    return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
}

