#pragma once

#include <optional>
#include <tuple>

#include "colour.h"
#include "rand.h"
#include "ray.h"
#include "renderobject.h"

class Material {
   public:
    constexpr Material() = default;

    constexpr Material(const Material&) = default;
    Material& operator=(const Material&) = default;

    constexpr Material(Material&&) = default;
    Material& operator=(Material&&) = default;

    virtual ~Material() = default;

    RandomGen& rand() const { return rand_; }

    virtual std::optional<std::tuple<Colour, Ray>> Scatter(const Ray& in,
                                                           const HitRecord& hit_record) const = 0;

   private:
    mutable RandomGen rand_{};
};

class Lambertian : public Material {
   public:
    constexpr explicit Lambertian(Colour albedo) : albedo_{albedo} {}

    std::optional<std::tuple<Colour, Ray>> Scatter(const Ray& /* in */,
                                                   const HitRecord& hit_record) const override {
        auto scatter_dir = hit_record.normal + rand().GenOnUnitSphere();

        // TODO: enable
        /* if (scatter_dir.almost_zero()) {
            scatter_dir = hit_record.normal;
        } */

        Ray scattered{hit_record.p, scatter_dir};

        return std::make_tuple(albedo_, scattered);
    }

   private:
    Colour albedo_;
};
