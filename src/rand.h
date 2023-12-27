#pragma once

#include <cassert>

#include "raytracer.h"
class Random {
   public:
    constexpr explicit Random(u64 j) {
        assert(j != v_);

        u_ = j ^ v_;
        GenU64();

        v_ = u_;
        GenU64();

        w_ = v_;
        GenU64();
    }

    constexpr u64 GenU64() {
        u_ = u_ * 2862933555777941757ULL + 7046029254386353087ULL;

        v_ ^= v_ >> 17;
        v_ ^= v_ << 31;
        v_ ^= v_ >> 8;

        w_ = 4294957665ULL * (w_ & 0xffffffffULL) + (w_ >> 32);

        u64 x = u_ ^ (u_ << 21);
        x ^= x >> 35;
        x ^= x << 4;

        return (x + v_) ^ w_;
    }

    constexpr u32 GenU32() { return static_cast<u32>(GenU64()); }

    constexpr f64 GenF64() { return 5.42101086242752217E-20 * static_cast<f64>(GenU64()); }

    constexpr f64 GenF64(f64 min, f64 max) { return min + (max - min) * GenF64(); }

   private:
    u64 u_ = 0ULL;
    u64 v_ = 4101842887655102017ULL;
    u64 w_ = 1ULL;
};
