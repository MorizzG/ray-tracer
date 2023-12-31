#pragma once

#include <cmath>

#include "raytracer.h"
#include "vec3.h"

class Camera {
   public:
    constexpr Camera(Point3 centre, u32 image_width, u32 image_height, f64 focal_length)
        : centre_{centre},
          focal_length_{focal_length},
          image_width_{image_width},
          image_height_{image_height} {
        Recalculate();
    }

    // can be changed, need to recalculate parameters

    constexpr Point3 centre() const { return centre_; }
    constexpr void centre(Point3 centre) {
        centre_ = centre;
        Recalculate();
    }

    constexpr Vec3 u() const { return u_; }
    constexpr Vec3 v() const { return v_; }
    constexpr Vec3 w() const { return w_; }

    constexpr void look_at(Point3 look_at, Vec3 up = Vec3::e_y) {
        w_ = (look_at - centre_).normed();

        v_ = (up - dot(up, w_) * w_).normed();

        u_ = cross(v_, w_);

        focal_length_ = (look_at - centre_).norm();

        Recalculate();
    }

    constexpr void rotate(f64 deg) {
        f64 rad = deg2rad(deg);

        Vec3 u = std::cos(rad) * u_ + std::sin(rad) * v_;
        Vec3 v = -std::sin(rad) * u_ + std::cos(rad) * v_;

        u_ = u;
        v_ = v;

        Recalculate();
    }

    constexpr f64 fov_angle_v() const { return fov_deg_v_; }
    constexpr void fov_angle_v(f64 fov) {
        fov_deg_v_ = fov;
        Recalculate();
    }

    // const-only accessors

    constexpr Vec3 d_u_pixel() const { return d_u_pixel_; }
    constexpr Vec3 d_v_pixel() const { return d_v_pixel_; }

    constexpr u32 image_width() const { return image_width_; }
    constexpr u32 image_height() const { return image_height_; }

    constexpr Vec3 PixelToWorld(u32 i, u32 j) const {
        return pixel_00_ + i * d_u_pixel_ + j * d_v_pixel_;
    }

   private:
    constexpr void Recalculate() {
        const f64 aspect_ratio = static_cast<f64>(image_width_) / image_height_;

        f64 fov_rad_v = deg2rad(fov_deg_v_);
        f64 h = std::tan(fov_rad_v / 2);

        // constexpr f64 viewport_height = 2.0;
        const f64 viewport_height = 2 * h * focal_length_;

        f64 viewport_width = viewport_height * aspect_ratio;

        Vec3 u_viewport = viewport_width * u_;
        Vec3 v_viewport = -viewport_height * v_;

        d_u_pixel_ = u_viewport / image_width_;
        d_v_pixel_ = v_viewport / image_height_;

        // upper left of viewport: move from camera centre half the viewport length in u and v
        // direction respectively and focal_length` in negative w direction
        auto viewport_upper_left = centre_ - u_viewport / 2 - v_viewport / 2 - focal_length_ * w_;

        pixel_00_ = viewport_upper_left + 0.5 * d_u_pixel_ + 0.5 * d_v_pixel_;
    }

    // settable camera parameters

    Point3 centre_ = Vec3::origin;

    Vec3 u_ = Vec3::e_x;  // points to right of camera
    Vec3 v_ = Vec3::e_y;  // points to up of camera
    Vec3 w_ = Vec3::e_z;  // points backward of camera

    f64 focal_length_;

    f64 fov_deg_v_ = 80;

    // saved camera properties

    // position of pixel 0, 0
    Vec3 pixel_00_;

    // Vec3 pointing from pixel to right/lower neighbour resp.
    Vec3 d_u_pixel_;
    Vec3 d_v_pixel_;

    u32 image_width_;
    u32 image_height_;
};
