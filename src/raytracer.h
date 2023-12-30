#pragma once

#include <cstdint>
#include <limits>
#include <numbers>

using u32 = uint32_t;
using u64 = uint64_t;

using f64 = double;

constexpr f64 kInf = std::numeric_limits<f64>::infinity();
constexpr f64 kNan = std::numeric_limits<f64>::signaling_NaN();

constexpr f64 kPi = std::numbers::pi_v<f64>;

constexpr char newline = '\n';

constexpr u32 FToU8(f64 x) { return static_cast<uint16_t>(255.999 * x); }

constexpr f64 deg2rad(f64 deg) { return deg * kPi / 180.0; }
