#pragma once

#include <cassert>
#include <optional>
#include <tuple>

#include "colour.h"
#include "rand.h"
#include "ray.h"
#include "raytracer.h"
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

        if (scatter_dir.almost_zero()) {
            scatter_dir = hit_record.normal;
        }

        Ray scattered{hit_record.p, scatter_dir};

        return std::make_tuple(albedo_, scattered);
    }

   private:
    Colour albedo_;
};

class Metal : public Material {
   public:
    constexpr Metal(Colour albedo, f64 fuzz) : albedo_{albedo}, fuzz_{fuzz} { assert(fuzz <= 1); }

    std::optional<std::tuple<Colour, Ray>> Scatter(const Ray& in,
                                                   const HitRecord& hit_record) const override {
        auto reflect_dir = in.direction().reflect(hit_record.normal).normed();

        reflect_dir += fuzz_ * rand().GenOnUnitSphere();

        Ray scattered{hit_record.p, reflect_dir};

        return std::make_tuple(albedo_, scattered);
    }

   private:
    Colour albedo_;
    f64 fuzz_;
};
