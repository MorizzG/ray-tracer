#pragma once

#include <cassert>
#include <cmath>
#include <optional>

#include "interval.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"
#include "vec3.h"

class Sphere : public RenderObject {
   public:
    constexpr Sphere() = default;
    constexpr Sphere(Point3 centre, f64 radius) : centre_{centre}, radius_{radius} {
        assert(radius >= 0);
    }

    constexpr Point3 centre() const { return centre_; }
    constexpr f64 radius() const { return radius_; }

    std::optional<HitRecord> hit(const Ray& ray, Interval ts) const override {
        HitRecord hit_record;

        Vec3 oc = ray.origin() - centre_;

        f64 a = ray.direction().squared();
        f64 b_half = dot(oc, ray.direction());
        f64 c = oc.squared() - radius_ * radius_;

        f64 discr = b_half * b_half - a * c;

        if (discr < 0) {
            // miss
            return std::nullopt;
        }

        // hit

        // smaller of the ts that hit the sphere
        f64 t = (-b_half - std::sqrt(discr)) / a;

        if (t >= ts.max()) {
            // if the smaller t is already too large, we don't need to check the second one since it
            // will be even larger

            return std::nullopt;
        }

        if (t <= ts.min()) {
            // if the smaller t is too small, check the larger one

            t = (-b_half + std::sqrt(discr)) / a;

            if (!ts.surronds(t)) {
                // larger t also out of range: return false

                return std::nullopt;
            }

            // smaller t hits front side, smaller t hits back side
            // TODO: what if our hit t is less than 0? is that case even relevant?
            hit_record.front_face = false;
        }

        hit_record.t = t;
        hit_record.p = ray.At(t);

        if (hit_record.front_face) {
            hit_record.normal = (hit_record.p - centre_) / radius_;
        } else {
            hit_record.normal = -(hit_record.p - centre_) / radius_;
        }

        return hit_record;
    }

   private:
    Point3 centre_{};
    f64 radius_ = 0.0;
};
