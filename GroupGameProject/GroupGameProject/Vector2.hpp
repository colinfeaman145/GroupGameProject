#ifndef VECTOR2HPP
#define VECTOR2HPP
#include <math.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>

#define PI 3.14159265


using namespace std;
class Vector2 {
public:
    float x;
    float y;

    inline Vector2();
    inline Vector2(float a, float b);

    inline const Vector2& operator+() const { return *this; }
    inline Vector2 operator-() const { return Vector2(-x, -y); }

    inline Vector2& operator+=(const Vector2& v2);
    inline Vector2& operator-=(const Vector2& v2);
    inline Vector2& operator*=(const Vector2& v2);
    inline Vector2& operator/=(const Vector2& v2);
    inline Vector2& operator+=(const float t);
    inline Vector2& operator-=(const float t);
    inline Vector2& operator*=(const float t);
    inline Vector2& operator/=(const float t);

    inline Vector2 Normalized();
    inline float LengthSquared() const { return ((x * x) + (y * y)); }
    inline float Length() const { return sqrtf(LengthSquared()); }
    inline string toString() const;
    inline bool IsNear(Vector2 v2, float delta);
};

inline Vector2::Vector2() {
    x = y = 0;
}

inline Vector2::Vector2(float a, float b) {
    x = a;
    y = b;
}

inline Vector2& Vector2::operator+=(const Vector2& v) {
    x += v.x;
    y += v.y;
    return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

inline Vector2& Vector2::operator*=(const Vector2& v) {
    x *= v.x;
    y *= v.y;
    return *this;
}

inline Vector2& Vector2::operator/=(const Vector2& v) {
    x /= v.x;
    y /= v.y;
    return *this;
}

inline Vector2& Vector2::operator+=(const float t) {
    x += t;
    y += t;
    return *this;
}

inline Vector2& Vector2::operator-=(const float t) {
    x -= t;
    y -= t;
    return *this;
}

inline Vector2& Vector2::operator*=(const float t) {
    x *= t;
    y *= t;
    return *this;
}

inline Vector2& Vector2::operator/=(const float t) {
    x /= t;
    y /= t;
    return *this;
}

inline string Vector2::toString() const {
    ostringstream s;
    s << "[" << x << ", " << y << "]";
    return s.str();
}

// for comparison of float values
inline bool Vector2::IsNear(Vector2 v2, float delta) {
    return std::fabs(x - v2.x) <= delta &&
        std::fabs(y - v2.y) <= delta;
}

inline Vector2 operator+(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}
inline Vector2 operator-(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}
inline Vector2 operator*(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x * v2.x, v1.y * v2.y);
}
inline Vector2 operator/(const Vector2& v1, const Vector2& v2) {
    return Vector2(v1.x / v2.x, v1.y / v2.y);
}

inline Vector2 operator+(const Vector2 v1, float t) {
    return Vector2(v1.x + t, v1.y + t);
}
inline Vector2 operator+(float t, const Vector2 v1) {
    return Vector2(v1.x + t, v1.y + t);
}
inline Vector2 operator-(const Vector2 v1, float t) {
    return Vector2(v1.x - t, v1.y - t);
}
inline Vector2 operator*(const Vector2 v1, float t) {
    return Vector2(v1.x * t, v1.y * t);
}
inline Vector2 operator*(float t, const Vector2 v1) {
    return Vector2(v1.x * t, v1.y * t);
}
inline Vector2 operator/(const Vector2 v1, float t) {
    return Vector2(v1.x * t, v1.y * t);
}

inline Vector2 Vector2::Normalized() {
    float length = Length();
    if (length == 0) return Vector2();
    return Vector2(x / length, y / length);
}

inline static float DotProduct(const Vector2& v1, const Vector2& v2) {
    return (v1.x * v2.x) + (v1.y * v2.y);
}

inline static Vector2 Reflect(const Vector2& v, const Vector2& n) {
    return v - (2 * DotProduct(v, n) * n);
}

inline static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float time) {
    return (Vector2(v1 + time * (v2 - v1)));
}

inline static float Distance(const Vector2& v1, const Vector2 v2) {
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    return sqrt(dx * dx + dy * dy);
}

#endif