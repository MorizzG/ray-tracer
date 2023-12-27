#pragma once

#include <iostream>

#include "camera.h"
#include "colour.h"
#include "image.h"
#include "interval.h"
#include "rand.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"
#include "vec3.h"

class Renderer {
   public:
    constexpr explicit Renderer(Camera camera, u32 samples_per_pixel = 10)
        : camera_{camera}, rand_{0}, samples_per_pixel_{samples_per_pixel} {}

    constexpr Image Render(const RenderObject& world) {
        Image img{camera_.image_width(), camera_.image_height()};

        for (u32 j = 0; j < img.height(); j++) {
            std::clog << "\rWriting line " << j << " of " << img.height() << std::flush;

            for (u32 i = 0; i < img.width(); i++) {
                Colour colour_sum{0.0, 0.0, 0.0};

                for (u32 num_sample = 0; num_sample < samples_per_pixel_; num_sample++) {
                    auto ray = SampleRay(i, j);

                    colour_sum += Cast(ray, world);
                }

                img[i, j] = colour_sum / samples_per_pixel_;
            }
        }

        return img;
    }

   private:
    constexpr Ray SampleRay(u32 i, u32 j) {
        auto pixel_centre = camera_.PixelToWorld(i, j);

        pixel_centre += rand_.GenF64(-0.5, 0.5) * camera_.d_u_pixel();
        pixel_centre += rand_.GenF64(-0.5, 0.5) * camera_.d_v_pixel();

        auto ray_direction = pixel_centre - camera_.centre();

        return Ray{camera_.centre(), ray_direction};
    }

    static constexpr Colour Cast(const Ray& ray, const RenderObject& world) {
        auto hit_record = world.hit(ray, Interval::kPositive);

        if (hit_record.has_value()) {
            Vec3 n = hit_record->normal;

            if (!hit_record->front_face) {
                return Colour::kBlack;
            }

            return Colour{0.5 * (n + Vec3{1, 1, 1})};
        }

        auto unit_dir = ray.direction().normed();

        f64 a = 0.5 * (unit_dir.y() + 1.0);

        return (1.0 - a) * Colour{1.0, 1.0, 1.0} + a * Colour{0.5, 0.7, 1.0};
    }

    Camera camera_;

    Random rand_;

    u32 samples_per_pixel_;
};
