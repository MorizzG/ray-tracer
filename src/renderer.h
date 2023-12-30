#pragma once

#include <sys/sysinfo.h>

#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

#include "camera.h"
#include "colour.h"
#include "image.h"
#include "interval.h"
#include "material.h"
#include "rand.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"
#include "vec3.h"

class Renderer {
   public:
    constexpr explicit Renderer(Camera camera, u32 samples_per_pixel = 100, u32 max_bounces = 50)
        : camera_{camera}, samples_per_pixel_{samples_per_pixel}, max_bounces_{max_bounces} {}

    constexpr u32& samples_per_pixel() { return samples_per_pixel_; }
    constexpr u32 samples_per_pixel() const { return samples_per_pixel_; }

    constexpr u32& max_bounces() { return max_bounces_; }
    constexpr u32 max_bounces() const { return max_bounces_; }

    Image Render(const RenderObject& world) {
        Image img{camera_.image_width(), camera_.image_height()};

        constexpr u32 kNumThreads = 16;

        std::atomic_uint32_t lines_done = 0;

        auto render_thread = [this, &world, &img, &lines_done](u32 thread) {
            for (u32 j = thread; j < img.height(); j += kNumThreads) {
                for (u32 i = 0; i < img.width(); i++) {
                    Colour colour_sum{0.0, 0.0, 0.0};

                    for (u32 num_sample = 0; num_sample < samples_per_pixel_; num_sample++) {
                        auto ray = SampleRay(i, j);

                        colour_sum += Cast(ray, world, max_bounces_);
                    }

                    img[i, j] = colour_sum / samples_per_pixel_;
                }

                lines_done.fetch_add(1);
            }
        };

        // render_thread(0);

        std::array<std::thread, kNumThreads> render_threads;

        for (u32 i = 0; i < kNumThreads; i++) {
            render_threads[i] = std::thread{render_thread, i};
        }

        std::atomic_bool done = false;

        auto writer_thread = std::thread([&done, &lines_done, total_lines = img.height()] {
            using std::literals::chrono_literals::operator""ms;

            while (!done.load()) {
                std::cerr << "\rWriting line " << lines_done.load() << " of " << total_lines
                          << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            std::clog << "\rDone                                                " << newline;
        });

        for (auto& t : render_threads) {
            t.join();
        }

        done.store(true);

        writer_thread.join();

        return img;
    }

   private:
    Ray SampleRay(u32 i, u32 j) {
        auto pixel_centre = camera_.PixelToWorld(i, j);

        auto& rand = RandomGen::GenInstance();

        Vec3 random_shift = rand.GenUniform(-0.5, 0.5) * camera_.d_u_pixel() +
                            rand.GenUniform(-0.5, 0.5) * camera_.d_v_pixel();

        pixel_centre += random_shift;

        auto ray_direction = (pixel_centre - camera_.centre()).normed();

        return Ray{camera_.centre(), ray_direction};
    }

    constexpr Colour Cast(const Ray& ray, const RenderObject& world, u32 bounces) {
        auto hit_record = world.hit(ray, Interval{0.001, kInf});

        // background
        if (!hit_record.has_value()) {
            auto unit_dir = ray.direction();

            f64 a = 0.5 * (unit_dir.y() + 1.0);

            return (1.0 - a) * Colour{1.0, 1.0, 1.0} + a * Colour{0.5, 0.7, 1.0};
        }

        // hit

        if (bounces == 0) {
            return Colour::kBlack;
        }

        auto res = hit_record->mat->Scatter(ray, hit_record.value());

        if (!res.has_value()) {
            // absorped
            return Colour::kBlack;
        }

        auto [albedo, out_ray] = res.value();

        assert(bounces > 0);
        return albedo * Cast(out_ray, world, bounces - 1);
    }

    Camera camera_;

    u32 samples_per_pixel_;
    u32 max_bounces_;
};
