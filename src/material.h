#pragma once

#include <cassert>
#include <cmath>
#include <optional>
#include <tuple>

#include "colour.h"
#include "rand.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"
#include "vec3.h"

class Material {
   public:
    constexpr Material() = default;

    constexpr Material(const Material&) = default;
    Material& operator=(const Material&) = default;

    constexpr Material(Material&&) = default;
    Material& operator=(Material&&) = default;

    virtual ~Material() = default;

    virtual std::optional<std::tuple<Colour, Ray>> Scatter(const Ray& in,
                                                           const HitRecord& hit_record) const = 0;

   private:
};

class Lambertian : public Material {
   public:
    constexpr explicit Lambertian(Colour albedo) : albedo_{albedo} {}

    std::optional<std::tuple<Colour, Ray>> Scatter(const Ray& /* in */,
                                                   const HitRecord& hit_record) const override {
        auto scatter_dir = hit_record.normal + RandomGen::GenInstance().GenOnUnitSphere();

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

        reflect_dir += fuzz_ * RandomGen::GenInstance().GenOnUnitSphere();

        Ray scattered{hit_record.p, reflect_dir};

        return std::make_tuple(albedo_, scattered);
    }

   private:
    Colour albedo_;
    f64 fuzz_;
};

class Dielectric : public Material {
   public:
    constexpr explicit Dielectric(f64 eta, f64 fuzz) : eta_{eta}, fuzz_{fuzz} {}

    std::optional<std::tuple<Colour, Ray>> Scatter(const Ray& in,
                                                   const HitRecord& hit_record) const override {
        auto& rand = RandomGen::GenInstance();

        f64 cos_theta = dot(-in.direction(), hit_record.normal);
        f64 sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        f64 eta = hit_record.front_face ? 1 / eta_ : eta_;

        Vec3 out_dir;

        constexpr auto reflectance = [](f64 cos, f64 eta) {
            auto r0 = (1 - eta) / (1 + eta);
            r0 = r0 * r0;
            return r0 * (1 - r0) * std::pow(1 - cos, 5);
        };

        if (eta * sin_theta > 1.0 || reflectance(cos_theta, eta) > rand.GenUniform()) {
            // can't refract, must reflect or Schlick approximation
            out_dir = in.direction().reflect(hit_record.normal);
        } else {
            // reflect
            out_dir = in.direction().refract(hit_record.normal, eta);
        }

        out_dir += fuzz_ * rand.GenOnUnitSphere();

        Ray out{hit_record.p, out_dir};

        return std::make_tuple(Colour::kWhite, out);
    }

   private:
    f64 eta_;  // index of refraction inside over outside
    f64 fuzz_;
};
