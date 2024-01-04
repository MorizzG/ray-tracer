#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <ostream>

#include "raytracer.h"

class Vec3 {
   public:
    static const Vec3 origin;

    static const Vec3 e_x;
    static const Vec3 e_y;
    static const Vec3 e_z;

    constexpr Vec3(f64 x, f64 y, f64 z) : xyz_{x, y, z} {}
    constexpr Vec3() : Vec3{0, 0, 0} {}

    constexpr f64& x() { return xyz_[0]; }
    constexpr f64& y() { return xyz_[1]; }
    constexpr f64& z() { return xyz_[2]; }

    constexpr f64 x() const { return xyz_[0]; }
    constexpr f64 y() const { return xyz_[1]; }
    constexpr f64 z() const { return xyz_[2]; }

    constexpr Vec3 operator-() const { return Vec3{-x(), -y(), -z()}; }

    constexpr f64& operator[](std::size_t i) {
        assert(i <= 3);
        return xyz_[i];
    }

    constexpr f64 operator[](std::size_t i) const {
        assert(i <= 3);
        return xyz_[i];
    }

    constexpr Vec3& operator+=(Vec3 v) {
        x() += v.x();
        y() += v.y();
        z() += v.z();

        return *this;
    }

    constexpr Vec3& operator-=(Vec3 v) {
        x() -= v.x();
        y() -= v.y();
        z() -= v.z();

        return *this;
    }

    constexpr Vec3& operator*=(f64 t) {
        x() *= t;
        y() *= t;
        z() *= t;

        return *this;
    }

    constexpr Vec3& operator/=(f64 t) {
        *this *= 1 / t;

        return *this;
    }

    constexpr bool almost_zero() const {
        f64 eps = 1e-8;

        return std::fabs(x()) < eps && std::fabs(y()) < eps && std::fabs(z()) < eps;
    }

    constexpr f64 squared() const;

    constexpr f64 norm() const;

    constexpr Vec3 normed() const;

    constexpr Vec3 reflect(Vec3 normal) const;

    constexpr Vec3 refract(Vec3 normal, f64 eta) const;

   private:
    std::array<f64, 3> xyz_;
};

constexpr Vec3 Vec3::origin{0.0, 0.0, 0.0};
constexpr Vec3 Vec3::e_x{1.0, 0.0, 0.0};
constexpr Vec3 Vec3::e_y{0.0, 1.0, 0.0};
constexpr Vec3 Vec3::e_z{0.0, 0.0, 1.0};

using Point3 = Vec3;

constexpr std::ostream& operator<<(std::ostream& out, Vec3 v) {
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

constexpr Vec3 operator+(Vec3 u, Vec3 v) {
    Vec3 out = u;
    out += v;
    return out;
}

constexpr Vec3 operator-(Vec3 u, Vec3 v) { return u + (-v); }

constexpr Vec3 operator*(Vec3 u, Vec3 v) { return {u.x() * v.x(), u.y() * v.y(), u.z() * v.z()}; }

constexpr Vec3 operator*(f64 t, Vec3 v) {
    Vec3 out = v;
    out *= t;
    return out;
}

constexpr Vec3 operator/(Vec3 v, f64 t) { return (1 / t) * v; }

constexpr f64 dot(Vec3 u, Vec3 v) {
    Vec3 tmp = u * v;
    return tmp.x() + tmp.y() + tmp.z();
}

constexpr Vec3 cross(Vec3 u, Vec3 v) {
    return {u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
            u.x() * v.y() - u.y() * v.x()};
}

constexpr f64 Vec3::squared() const { return dot(*this, *this); }

constexpr f64 Vec3::norm() const { return std::sqrt(squared()); }

constexpr Vec3 Vec3::normed() const { return *this / norm(); }

constexpr Vec3 Vec3::reflect(Vec3 normal) const { return *this - 2 * dot(*this, normal) * normal; }

constexpr Vec3 Vec3::refract(Vec3 normal, f64 eta) const {
    f64 cos_theta = dot(-*this, normal);

    Vec3 out_orth = eta * (*this + cos_theta * normal);
    Vec3 out_par = -std::sqrt(std::fabs(1.0 - out_orth.squared())) * normal;

    return out_orth + out_par;
}
