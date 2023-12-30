#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>

#include "colour.h"
#include "raytracer.h"

class Image {
   public:
    constexpr Image(u32 width, u32 height)
        // NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
        : buf_(std::make_unique_for_overwrite<Colour[]>(static_cast<std::size_t>(width) * height)),
          width_{width},
          height_{height} {}

    constexpr u32 width() const { return width_; }
    constexpr u32 height() const { return height_; }

    Colour& operator[](u32 x, u32 y) {
        assert(x < width_);
        assert(y < height_);

        assert(buf_.get() != nullptr);

        return buf_[y * width_ + x];
    }

    Colour operator[](u32 x, u32 y) const {
        assert(x < width_);
        assert(y < height_);

        assert(buf_.get() != nullptr);

        return buf_[y * width_ + x];
    }

   private:
    // NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    std::unique_ptr<Colour[]> buf_;

    u32 width_;
    u32 height_;
};

constexpr std::ostream& operator<<(std::ostream& os, const Image& img) {
    os << "P3" << newline;
    os << img.width() << ' ' << img.height() << newline;
    os << "255" << newline;

    for (u32 j = 0; j < img.height(); j++) {
        for (u32 i = 0; i < img.width(); i++) {
            const Colour col = img[i, j];

            col.CheckValid();

            // apply gamma 2 transform
            os << col.to_gamma2() << newline;
        }
    }

    return os;
}
