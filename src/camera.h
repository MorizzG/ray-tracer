#pragma once

#include "ray.h"
#include "raytracer.h"
#include "vec3.h"

class Camera {
   public:
    constexpr Camera(Vec3 centre, u32 image_width, u32 image_height, double focal_length)
        : centre_{centre} {
        constexpr double viewport_height = 2.0;
        double viewport_width = viewport_height * static_cast<double>(image_width) / image_height;

        Vec3 u_viewport = viewport_width * Vec3::e_x;
        Vec3 v_viewport = -viewport_height * Vec3::e_y;

        d_u_pixel_ = u_viewport / image_width;
        d_v_pixel_ = v_viewport / image_height;

        // upper left of viewport: move from camera centre `focal_length` in z direction, then half
        // of the viewport length in x and y direction respectively
        auto viewport_upper_left =
            centre_ - focal_length * Vec3::e_z - u_viewport / 2 - v_viewport / 2;

        pixel_00_ = viewport_upper_left + 0.5 * d_u_pixel_ + 0.5 * d_v_pixel_;
    }

    constexpr Vec3 centre() const { return centre_; }

    constexpr Vec3 Image2World(u32 i, u32 j) const {
        return pixel_00_ + i * d_u_pixel_ + j * d_v_pixel_;
    }

    constexpr Ray CastRay(u32 i, u32 j) const {
        auto pixel_centre = Image2World(i, j);

        auto ray_direction = pixel_centre - centre();

        return Ray{centre(), ray_direction};
    }

   private:
    Vec3 centre_;

    // position of pixel 0, 0
    Vec3 pixel_00_;

    // Vec3 pointing from pixel to right/lower neighbour resp.
    Vec3 d_u_pixel_;
    Vec3 d_v_pixel_;
};
