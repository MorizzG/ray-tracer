#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <utility>

#include "interval.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"
#include "vec3.h"

class Rectangle : public RenderObject {
   public:
    Rectangle() = default;

    Rectangle(Point3 origin, Vec3 a, Vec3 b, std::shared_ptr<Material> mat)
        : origin_{origin}, a_{a}, b_{b}, normal_{cross(a, b).normed()}, mat_{std::move(mat)} {
        assert(is_zero(dot(a_, b_)));
    }

    std::optional<HitRecord> hit(const Ray& ray, Interval ts) const override {
        auto t_maybe = ray.HitPlane(origin_, normal_);

        if (!t_maybe.has_value()) {
            return std::nullopt;
        }

        f64 t = t_maybe.value();

        if (!ts.contains(t)) {
            return std::nullopt;
        }

        Point3 p = ray.At(t);

        f64 _a = dot(p - origin_, a_);
        f64 _b = dot(p - origin_, b_);

        if (_a <= 0.0 || _b <= 0.0 || _a >= a_.norm() || _b >= b_.norm()) {
            return std::nullopt;
        }

        HitRecord hit_record;

        hit_record.t = t;
        hit_record.front_face = dot(normal_, ray.direction()) > 0;
        hit_record.p = p;
        hit_record.mat = mat_.get();
        hit_record.normal = normal_;

        return hit_record;
    }

   private:
    Point3 origin_;

    Vec3 a_;
    Vec3 b_;

    Vec3 normal_;

    std::shared_ptr<Material> mat_;
};
