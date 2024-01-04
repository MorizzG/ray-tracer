#pragma once

#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>

using u32 = uint32_t;
using u64 = uint64_t;

using i32 = int32_t;
using i64 = int64_t;

using f64 = double;

constexpr f64 kInf = std::numeric_limits<f64>::infinity();
constexpr f64 kNan = std::numeric_limits<f64>::signaling_NaN();

constexpr f64 kPi = std::numbers::pi_v<f64>;

constexpr char newline = '\n';

constexpr u32 FToU8(f64 x) { return static_cast<u32>(255.999 * x); }

constexpr f64 deg2rad(f64 deg) { return deg * kPi / 180.0; }

constexpr bool is_zero(f64 x, f64 atol = 1e-5) {
    assert(atol >= 0.0);
    return std::fabs(x) < atol;
}
