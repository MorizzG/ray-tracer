#pragma once

#include "raytracer.h"
class Interval {
   public:
    static const Interval kEmpty, kFull, kPositive;

    constexpr Interval() = default;
    constexpr Interval(double min, double max) : min_{min}, max_{max} {}

    constexpr double min() const { return min_; }
    constexpr double max() const { return max_; }

    constexpr bool contains(double x) const { return min_ <= x && x <= max_; }
    constexpr bool surronds(double x) const { return min_ < x && x < max_; }

   private:
    double min_ = kInf;
    double max_ = -kInf;
};

constexpr Interval Interval::kEmpty{kInf, -kInf};
constexpr Interval Interval::kFull{-kInf, kInf};
constexpr Interval Interval::kPositive{0.0, kInf};
