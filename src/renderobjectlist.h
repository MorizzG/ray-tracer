#pragma once

#include <initializer_list>
#include <optional>
#include <vector>

#include "interval.h"
#include "ray.h"
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

    std::optional<HitRecord> hit(const Ray& ray, Interval ts) const override {
        std::optional<HitRecord> closest_hit_record = std::nullopt;

        double closest_t = ts.max();

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
    std::vector<SharedRenderObject> objs_;
};
