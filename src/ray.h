#pragma once

#include "vec3.h"

class Ray {
   public:
    Ray() = default;

    Ray(const Point3& origin, const Vec3& direction) : orig_{origin}, dir_{direction} {}

    Point3 origin() const { return orig_; }
    Vec3 direction() const { return dir_; }

    Point3 At(double t) { return orig_ + t * dir_; }

   private:
    Point3 orig_;
    Vec3 dir_;
};