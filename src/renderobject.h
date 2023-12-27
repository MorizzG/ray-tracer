#pragma once

#include <memory>
#include <optional>

#include "colour.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t = 0.0;
    bool front_face = true;
};

class RenderObject {
   public:
    RenderObject() = default;

    RenderObject(const RenderObject& other) = default;
    RenderObject& operator=(const RenderObject&) = default;

    RenderObject(RenderObject&& other) = default;
    RenderObject& operator=(RenderObject&&) = default;

    virtual ~RenderObject() = default;

    virtual std::optional<HitRecord> hit(const Ray& ray, Interval ts) const = 0;
};

using SharedRenderObject = std::shared_ptr<RenderObject>;

constexpr Colour Cast(const Ray& ray, const RenderObject& world) {
    auto hit_record = world.hit(ray, Interval::kPositive);
    // auto hit_record = world.hit(ray, Interval{0.505, kInf});

    if (hit_record.has_value()) {
        // assert(hit_record.front_face);

        Vec3 n = hit_record->normal;

        if (!hit_record->front_face) {
            // return Colour{0.0, 0.0, 0.0};
            return Colour::kBlack;
        }

        return Colour{0.5 * (n + Vec3{1, 1, 1})};
    }

    auto unit_dir = ray.direction().normed();

    double a = 0.5 * (unit_dir.y() + 1.0);

    return (1.0 - a) * Colour{1.0, 1.0, 1.0} + a * Colour{0.5, 0.7, 1.0};
}
