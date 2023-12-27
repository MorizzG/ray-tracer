#pragma once

#include <algorithm>

#include "raytracer.h"
class Interval {
   public:
    static const Interval kEmpty, kFull, kPositive;

    constexpr Interval() = default;
    constexpr Interval(f64 min, f64 max) : min_{min}, max_{max} {}

    constexpr f64 min() const { return min_; }
    constexpr f64 max() const { return max_; }

    constexpr bool contains(f64 x) const { return min_ <= x && x <= max_; }
    constexpr bool surronds(f64 x) const { return min_ < x && x < max_; }

    constexpr double clamp(f64 x) const { return std::clamp(x, min_, max_); }

   private:
    f64 min_ = kInf;
    f64 max_ = -kInf;
};

constexpr Interval Interval::kEmpty{kInf, -kInf};
constexpr Interval Interval::kFull{-kInf, kInf};
constexpr Interval Interval::kPositive{0.0, kInf};
