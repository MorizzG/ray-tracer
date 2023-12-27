#pragma once

#include <cassert>
#include <cmath>
#include <optional>

#include "ray.h"
#include "renderobject.h"
#include "vec3.h"

class Sphere : public RenderObject {
   public:
    constexpr Sphere() = default;
    constexpr Sphere(Point3 centre, double radius) : centre_{centre}, radius_{radius} {
        assert(radius >= 0);
    }

    constexpr Point3 centre() const { return centre_; }
    constexpr double radius() const { return radius_; }

    std::optional<HitRecord> hit(const Ray& ray, double t_min, double t_max) const override {
        HitRecord hit_record;

        Vec3 oc = ray.origin() - centre_;

        double a = ray.direction().squared();
        double b_half = dot(oc, ray.direction());
        double c = oc.squared() - radius_ * radius_;

        auto discr = b_half * b_half - a * c;

        if (discr < 0) {
            // miss
            return std::nullopt;
        }

        // hit

        // smaller of the ts that hit the sphere
        double t = (-b_half - std::sqrt(discr)) / a;

        if (t > t_max) {
            // if the smaller t is already too large, we don't need to check the second one since it
            // will be even larger

            return std::nullopt;
        }

        if (t < t_min) {
            // if the smaller t is too small, check the larger one

            t = (-b_half + std::sqrt(discr)) / a;

            if (t < t_min || t > t_max) {
                // larger t also out of range: return false

                return std::nullopt;
            }

            // smaller t hits front side, smaller t hits back side
            // TODO: what if our hit t is less than 0? is that case even relevant?
            hit_record.front_face = false;
        }

        hit_record.t = t;
        hit_record.p = ray.At(t);
        hit_record.normal = (hit_record.p - centre_) / radius_;

        if (!hit_record.front_face) {
            hit_record.normal = -hit_record.normal;
        }

        return hit_record;
    }

   private:
    Point3 centre_{};
    double radius_ = 0.0;
};
