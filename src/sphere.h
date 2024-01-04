#pragma once

#include <cassert>
#include <memory>
#include <optional>

#include "interval.h"
#include "material.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"
#include "vec3.h"

class Sphere : public RenderObject {
   public:
    constexpr Sphere() = default;

    Sphere(Point3 centre, f64 radius, const std::shared_ptr<Material>& mat)
        : centre_{centre}, radius_{radius}, mat_{mat} {
        assert(radius >= 0);
    }

    constexpr Point3 centre() const { return centre_; }
    constexpr f64 radius() const { return radius_; }

    std::optional<HitRecord> hit(const Ray& ray, Interval ts) const override {
        HitRecord hit_record{};

        auto t_low_high = ray.HitSphere(centre_, radius_);

        if (!t_low_high.has_value()) {
            // miss
            return std::nullopt;
        }

        // hit
        auto [t_low, t_high] = t_low_high.value();

        // we need to only distinguish 4 cases here:
        // 1. t_low higher than interval automatically implies t_high is also higher than interval,
        //    so return
        // 2. t_low is in the interval, so return the t_low hit (t_high doesn't matter)
        // 3. t_low is too low, t_high is in interval, so return t_high
        // 4. both t_low is too low and t_high is too high

        if (t_low >= ts.max()) {
            // if the smaller t is already too large, we don't need to check the second one since it
            // will be even larger

            return std::nullopt;
        }

        if (t_low > ts.min()) {
            // t_min is hit

            hit_record.t = t_low;
            // hit_record.front_face = true;
        } else {
            // if the smaller t is too small, check the larger one

            if (!ts.surronds(t_high)) {
                // larger t also out of range: return false

                return std::nullopt;
            }

            // t_low hits front side, t_high hits back side
            hit_record.t = t_high;
            hit_record.front_face = false;
        }

        hit_record.p = ray.At(hit_record.t);
        hit_record.mat = mat_.get();

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

    std::shared_ptr<Material> mat_;
};
