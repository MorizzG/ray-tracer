#pragma once

#include <initializer_list>
#include <optional>
#include <vector>

#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"

class RenderObjectList : public RenderObject {
   public:
    constexpr RenderObjectList() = default;
    // explicit RenderObjectList(const SharedRenderObject& obj) { Append(obj); }
    RenderObjectList(std::initializer_list<SharedRenderObject> objs) : objs_{objs} {}

    constexpr auto begin() { return objs_.begin(); }
    constexpr auto end() { return objs_.end(); }

    constexpr auto begin() const { return objs_.begin(); }
    constexpr auto end() const { return objs_.end(); }

    void Add(const SharedRenderObject& obj) { objs_.push_back(obj); }

    void Clear() { objs_.clear(); }

    std::optional<HitRecord> hit(const Ray& ray, double t_min, double t_max) const override {
        std::optional<HitRecord> closest_hit_record = std::nullopt;

        double closest_t = kInf;

        for (const auto& obj : objs_) {
            auto hit_record = obj->hit(ray, t_min, t_max);

            if (hit_record.has_value() && hit_record->t < closest_t) {
                closest_hit_record = hit_record;
                closest_t = hit_record->t;
            }
        }

        return closest_hit_record;
    }

   private:
    std::vector<SharedRenderObject> objs_;
};
