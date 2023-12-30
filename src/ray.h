#pragma once

#include "raytracer.h"
#include "vec3.h"

class Ray {
   public:
    constexpr Ray() = default;

    constexpr Ray(const Point3& origin, const Vec3& direction)
        : orig_{origin}, dir_{direction.normed()} {}

    Point3 origin() const { return orig_; }
    Vec3 direction() const { return dir_; }

    constexpr Point3 At(f64 t) const { return orig_ + t * dir_; }

   private:
    Point3 orig_;
    Vec3 dir_;
};
