#pragma once

#include <initializer_list>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "interval.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"

class RenderObjectList : public RenderObject {
   public:
    constexpr RenderObjectList() = default;
    // explicit RenderObjectList(const SharedRenderObject& obj) { Append(obj); }
    // RenderObjectList(std::initializer_list<RenderObject> objs) : objs_{objs} {}

    constexpr auto begin() { return objs_.begin(); }
    constexpr auto end() { return objs_.end(); }

    constexpr auto begin() const { return objs_.begin(); }
    constexpr auto end() const { return objs_.end(); }

    void Add(std::unique_ptr<RenderObject> obj) { objs_.emplace_back(std::move(obj)); }

    void Clear() { objs_.clear(); }

    std::optional<HitRecord> hit(const Ray& ray, Interval ts) const override {
        std::optional<HitRecord> closest_hit_record = std::nullopt;

        f64 closest_t = ts.max();

        for (const auto& obj : objs_) {
            Interval _ts{ts.min(), closest_t};
            auto hit_record = obj->hit(ray, _ts);

            if (hit_record.has_value()) {
                closest_hit_record = hit_record;
                closest_t = hit_record->t;
            }
        }

        return closest_hit_record;
    }

   private:
    std::vector<std::unique_ptr<RenderObject>> objs_;
};
