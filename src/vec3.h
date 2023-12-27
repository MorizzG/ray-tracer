#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <ostream>

class Vec3 {
   public:
    static const Vec3 e_x;
    static const Vec3 e_y;
    static const Vec3 e_z;

    constexpr Vec3(double x, double y, double z) : xyz_{x, y, z} {}
    constexpr Vec3() : Vec3{0, 0, 0} {}

    constexpr double& x() { return xyz_[0]; }
    constexpr double& y() { return xyz_[1]; }
    constexpr double& z() { return xyz_[2]; }

    constexpr double x() const { return xyz_[0]; }
    constexpr double y() const { return xyz_[1]; }
    constexpr double z() const { return xyz_[2]; }

    constexpr Vec3 operator-() const { return Vec3{-x(), -y(), -z()}; }

    constexpr double& operator[](std::size_t i) {
        assert(i <= 3);
        return xyz_[i];
    }

    constexpr double operator[](std::size_t i) const {
        assert(i <= 3);
        return xyz_[i];
    }

    constexpr Vec3& operator+=(const Vec3& v) {
        x() += v.x();
        y() += v.y();
        z() += v.z();

        return *this;
    }

    constexpr Vec3& operator*=(double t) {
        x() *= t;
        y() *= t;
        z() *= t;

        return *this;
    }

    constexpr Vec3& operator/=(double t) {
        *this *= 1 / t;

        return *this;
    }

    constexpr double squared() const { return x() * x() + y() * y() + z() * z(); }

    constexpr double norm() const { return std::sqrt(squared()); }

    constexpr Vec3 normed() const {
        auto r = *this;
        r /= norm();
        return r;
    }

   private:
    std::array<double, 3> xyz_;
};

constexpr Vec3 Vec3::e_x{1.0, 0.0, 0.0};
constexpr Vec3 Vec3::e_y{0.0, 1.0, 0.0};
constexpr Vec3 Vec3::e_z{0.0, 0.0, 1.0};

using Point3 = Vec3;

constexpr std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

constexpr Vec3 operator+(const Vec3& u, const Vec3& v) {
    Vec3 out = u;
    out += v;
    return out;
}

constexpr Vec3 operator-(const Vec3& u, const Vec3& v) { return u + (-v); }

constexpr Vec3 operator*(const Vec3& u, const Vec3& v) {
    return {u.x() * v.x(), u.y() * v.y(), u.z() * v.z()};
}

constexpr Vec3 operator*(double t, const Vec3& v) {
    Vec3 out = v;
    out *= t;
    return out;
}

constexpr Vec3 operator/(const Vec3& v, double t) { return (1 / t) * v; }

constexpr double dot(const Vec3& u, const Vec3& v) {
    Vec3 tmp = u * v;
    return tmp.x() + tmp.y() + tmp.z();
}

constexpr Vec3 cross(const Vec3& u, const Vec3& v) {
    return {u.y() * v.z() - u.z() - v.y(), u.z() * v.x() - u.x() * v.z(),
            u.x() * v.y() - u.y() - v.x()};
}

constexpr Vec3 normed(const Vec3& v) { return v.normed(); }
