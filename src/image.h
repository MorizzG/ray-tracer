#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>

#include "colour.h"
#include "util.h"

class Image {
   public:
    constexpr Image(uint32_t width, uint32_t height)
        // NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
        : buf_(std::make_unique_for_overwrite<Colour[]>(static_cast<std::size_t>(width) * height)),
          width_{width},
          height_{height} {}

    constexpr uint32_t width() const { return width_; }
    constexpr uint32_t height() const { return height_; }

    Colour& operator[](uint32_t x, uint32_t y) {
        assert(x < width_);
        assert(y < height_);

        assert(buf_.get() != nullptr);

        return buf_[y * width_ + x];
    }

    Colour operator[](uint32_t x, uint32_t y) const {
        assert(x < width_);
        assert(y < height_);

        assert(buf_.get() != nullptr);

        return buf_[y * width_ + x];
    }

   private:
    // NOLINTNEXTLINE(modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    std::unique_ptr<Colour[]> buf_;

    uint32_t width_;
    uint32_t height_;
};

constexpr std::ostream& operator<<(std::ostream& os, const Image& img) {
    os << "P3\n" << img.width() << ' ' << img.height() << "\n255\n";

    for (uint32_t j = 0; j < img.height(); j++) {
        std::clog << "\rWriting line " << j << " of " << img.height() << std::flush;

        for (uint32_t i = 0; i < img.width(); i++) {
            const Colour col = img[i, j];

            os << FToU8(col.r()) << ' ' << FToU8(col.g()) << ' ' << FToU8(col.b()) << '\n';
        }
    }

    std::clog << "\rDone                                                " << std::endl;

    return os;
}