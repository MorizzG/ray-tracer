#pragma once

#include <cstdint>
#include <limits>
#include <numbers>

using u32 = uint32_t;

constexpr double kInf = std::numeric_limits<double>::infinity();
constexpr double kPi = std::numbers::pi_v<double>;

constexpr char newline = '\n';

constexpr u32 FToU8(double x) { return static_cast<uint16_t>(255.999 * x); }

constexpr double deg2rad(double deg) { return deg * kPi / 180.0; }
