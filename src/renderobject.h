#pragma once

#include <memory>
#include <optional>

#include "interval.h"
#include "ray.h"
#include "raytracer.h"
#include "vec3.h"

class Material;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    const Material* mat = nullptr;

    f64 t = kNan;
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
