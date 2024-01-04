#pragma once

#include <philox.hh>

#include "raytracer.h"
#include "vec3.h"

class RandomGen {
   public:
    constexpr RandomGen(const RandomGen&) = delete;
    constexpr RandomGen& operator=(const RandomGen&) = delete;

    constexpr RandomGen(RandomGen&&) = delete;
    constexpr RandomGen& operator=(RandomGen&&) = delete;

    constexpr ~RandomGen() = default;

    static RandomGen& GenInstance() {
        static thread_local RandomGen rand{};

        return rand;
    }

    constexpr u64 U64() { return philox_.Next(); }
    // constexpr u32 GenU32() { return static_cast<u32>(GenU64()); }

    constexpr f64 Uniform() { return philox_.NextF64(); }

    constexpr f64 Uniform(f64 min, f64 max) { return min + (max - min) * Uniform(); }

    constexpr Vec3 UniformVec3() { return Vec3{Uniform(), Uniform(), Uniform()}; }

    constexpr Vec3 UniformVec3(f64 min, f64 max) {
        return Vec3{Uniform(min, max), Uniform(min, max), Uniform(min, max)};
    }

    constexpr Vec3 UnitBallVec3() {
        while (true) {
            Vec3 v = UniformVec3(-1.0, 1.0);

            if (v.norm() < 1.0) {
                return v;
            }
        }
    }

    constexpr Vec3 UnitSphereVec3() { return UnitBallVec3().normed(); }

    constexpr Vec3 HemisphereVec3(Vec3 normal) {
        Vec3 v = UnitSphereVec3();

        if (dot(v, normal) < 0) {
            v = -v;
        }

        return v;
    }

    constexpr Vec3 UnitDiskVec3(Vec3 normal) {
        while (true) {
            Vec3 v = UniformVec3();
            v -= dot(v, normal) * normal;

            if (v.norm() < 1) {
                return v;
            }
        }
    }

   private:
    RandomGen() = default;

    Philox philox_;
};
