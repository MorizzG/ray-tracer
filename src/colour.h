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
    constexpr Colour(double r, double g, double b) : r_{r}, g_{g}, b_{b} {
        assert(0.0 <= r && r <= 1.0);
        assert(0.0 <= g && g <= 1.0);
        assert(0.0 <= b && b <= 1.0);
    }

    constexpr explicit Colour(Vec3 v) : Colour{v.x(), v.y(), v.z()} {}

    constexpr double& r() { return r_; }
    constexpr double& g() { return g_; }
    constexpr double& b() { return b_; }

    constexpr double r() const { return r_; }
    constexpr double g() const { return g_; }
    constexpr double b() const { return b_; }

    std::string to_string() const {
        std::stringstream ss;

        ss << '#';

        ss << std::hex << std::setfill('0') << std::setw(2) << FToU8(r());
        ss << std::hex << std::setfill('0') << std::setw(2) << FToU8(g());
        ss << std::hex << std::setfill('0') << std::setw(2) << FToU8(b());

        return ss.str();
    }

   private:
    double r_;
    double g_;
    double b_;
};

constexpr Colour Colour::kBlack{0.0, 0.0, 0.0};

constexpr Colour operator*(double t, Colour col) { return {t * col.r(), t * col.g(), t * col.b()}; }

constexpr Colour operator+(Colour c1, Colour c2) {
    return {c1.r() + c2.r(), c1.g() + c2.g(), c1.b() + c2.b()};
}

inline std::ostream& operator<<(std::ostream& os, Colour colour) {
    os << FToU8(colour.r()) << ' ' << FToU8(colour.g()) << ' ' << FToU8(colour.b());

    return os;
}
