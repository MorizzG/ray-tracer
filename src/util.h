#pragma once

#include <cstdint>
constexpr uint32_t FToU8(double x) {
    return static_cast<uint16_t>(255.999 * x);
}