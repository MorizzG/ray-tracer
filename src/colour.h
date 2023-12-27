#pragma once

#include <cassert>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "raytracer.h"
#include "vec3.h"

class Colour {
   public:
    static const Colour kBlack;
    static const Colour kWhite;

    constexpr Colour() = default;
    constexpr Colour(f64 r, f64 g, f64 b) : r_{r}, g_{g}, b_{b} {}

    constexpr void CheckValid() const {
        assert(0.0 <= r() && r() <= 1.0);
        assert(0.0 <= g() && g() <= 1.0);
        assert(0.0 <= b() && b() <= 1.0);
    }

    constexpr explicit Colour(Vec3 v) : Colour{v.x(), v.y(), v.z()} {}

    constexpr f64& r() { return r_; }
    constexpr f64& g() { return g_; }
    constexpr f64& b() { return b_; }

    constexpr f64 r() const { return r_; }
    constexpr f64 g() const { return g_; }
    constexpr f64 b() const { return b_; }

    constexpr Colour& operator+=(const Colour& c) {
        r_ += c.r();
        g_ += c.g();
        b_ += c.b();

        return *this;
    }

    std::string to_string() const {
        std::stringstream ss;

        ss << '#';

        ss << std::hex << std::setfill('0') << std::setw(2) << FToU8(r());
        ss << std::hex << std::setfill('0') << std::setw(2) << FToU8(g());
        ss << std::hex << std::setfill('0') << std::setw(2) << FToU8(b());

        return ss.str();
    }

   private:
    f64 r_ = 0.0;
    f64 g_ = 0.0;
    f64 b_ = 0.0;
};

constexpr Colour Colour::kBlack{0.0, 0.0, 0.0};

constexpr Colour operator*(f64 t, Colour col) { return {t * col.r(), t * col.g(), t * col.b()}; }
constexpr Colour operator/(Colour col, f64 t) { return (1.0 / t) * col; }

constexpr Colour operator+(Colour c1, Colour c2) {
    Colour out{c1};

    out += c2;

    return out;
}

inline std::ostream& operator<<(std::ostream& os, Colour colour) {
    os << FToU8(colour.r()) << ' ' << FToU8(colour.g()) << ' ' << FToU8(colour.b());

    return os;
}
