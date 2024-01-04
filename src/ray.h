#pragma once

#include <cmath>
#include <iostream>
#include <optional>
#include <utility>

#include "raytracer.h"
#include "vec3.h"

class Ray {
   public:
    constexpr Ray() = default;

    constexpr Ray(const Point3& origin, Vec3 direction) : orig_{origin}, dir_{direction.normed()} {}

    Point3 origin() const { return orig_; }
    Vec3 direction() const { return dir_; }

    constexpr Point3 At(f64 t) const { return orig_ + t * dir_; }

    // return ts at which the ray intersects sphere or nullopt if no intersection
    // ts may the the same if tangential
    constexpr std::optional<std::pair<f64, f64>> HitSphere(Point3 centre, f64 radius) const {
        Vec3 oc = origin() - centre;

        f64 a = 1.0;  // direction().squared() is always 1
        f64 b_half = dot(oc, direction());
        f64 c = oc.squared() - radius * radius;

        f64 discr = b_half * b_half - a * c;

        if (discr < 0) {
            // miss
            return std::nullopt;
        }

        // hit

        return std::make_pair((-b_half - std::sqrt(discr)) / a, (-b_half + std::sqrt(discr)) / a);
    }

    // return t a which the ray intersects the plane containing `v` with normal `normal`
    std::optional<f64> HitPlane(Point3 v, Vec3 normal) const {
        // if the ray is parallel to the plane, return nullopt, even  if the ray runs inside the
        // plane!
        if (is_zero(dot(direction(), normal))) {
            return std::nullopt;
        }

        f64 t = dot(v - origin(), normal) / dot(direction(), normal);

        return t;
    }

   private:
    Point3 orig_;
    Vec3 dir_;  // unit vector
};
