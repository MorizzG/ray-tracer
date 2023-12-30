#pragma once

#include <philox.hh>

#include "raytracer.h"
#include "vec3.h"

class RandomGen {
   public:
    constexpr u64 GenU64() { return philox_.Next(); }
    // constexpr u32 GenU32() { return static_cast<u32>(GenU64()); }

    constexpr f64 GenUniform() { return philox_.NextF64(); }

    constexpr f64 GenUniform(f64 min, f64 max) { return min + (max - min) * GenUniform(); }

    constexpr Vec3 GenVec3() { return Vec3{GenUniform(), GenUniform(), GenUniform()}; }

    constexpr Vec3 GenVec3(f64 min, f64 max) {
        return Vec3{GenUniform(min, max), GenUniform(min, max), GenUniform(min, max)};
    }

    constexpr Vec3 GenInUnitBall() {
        while (true) {
            Vec3 v = GenVec3(-1.0, 1.0);

            if (v.norm() < 1.0) {
                return v;
            }
        }
    }

    constexpr Vec3 GenOnUnitSphere() { return GenInUnitBall().normed(); }

    constexpr Vec3 GenOnHemisphere(const Vec3& normal) {
        Vec3 v = GenOnUnitSphere();

        if (dot(v, normal) < 0) {
            v = -v;
        }

        return v;
    }

   private:
    Philox philox_;
};
