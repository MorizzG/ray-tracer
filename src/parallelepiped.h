#pragma once

#include <cassert>
#include <cmath>
#include <optional>
#include <utility>

#include "interval.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"
#include "vec3.h"

class Parallelepiped : public RenderObject {
   public:
    constexpr Parallelepiped() = default;

    constexpr Parallelepiped(Point3 centre, Vec3 u, Vec3 v, Vec3 w, f64 a = 1.0, f64 b = 1.0,
                             f64 c = 1.0)
        : centre_{centre}, u_{u.normed()}, v_{v.normed()}, w_{w.normed()}, a_{a}, b_{b}, c_{c} {
        assert(a >= 0);
        assert(b >= 0);
        assert(c >= 0);

        // make sure all axes lie in unit cube

        if (dot(u_, v_) < 0) {
            v_ = -v_;
        }

        if (dot(u_, w_) < 0) {
            w_ = -w_;
        }

        // make u, v, w are right-handed (not sure if neccessary, but why not)
        if (dot(cross(u_, v_), w_) < 0.0) {
            std::swap(v_, w_);
        }
    }

    constexpr Vec3 u() const { return u_; }
    constexpr Vec3 v() const { return v_; }
    constexpr Vec3 w() const { return w_; }

    constexpr f64 a() const { return a_; }
    constexpr f64 b() const { return b_; }
    constexpr f64 c() const { return c_; }

    std::optional<HitRecord> hit(const Ray& ray, Interval ts) const override {
        // check if ray intersects outsphere first
        const f64 radius_ = (a_ * u_ + b_ * v_ + c_ * w_).norm() / 2;

        Vec3 oc = ray.origin() - centre_;

        f64 a = ray.direction().squared();
        f64 b_half = dot(oc, ray.direction());
        f64 c = oc.squared() - radius_ * radius_;

        f64 discr = b_half * b_half - a * c;

        if (discr < 0) {
            // miss
            return std::nullopt;
        }

        // since the sphere completely envelops the cuboid

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
        }
    }

   private:
    Point3 centre_;

    // face normals
    Vec3 u_;
    Vec3 v_;
    Vec3 w_;

    // face distances
    f64 a_ = 0.0;
    f64 b_ = 0.0;
    f64 c_ = 0.0;
};
