#pragma once

#include <cassert>
#include <cmath>

#include "raytracer.h"
#include "vec3.h"

class Camera {
   public:
    constexpr Camera(Point3 centre, u32 image_width, u32 image_height)
        : centre_{centre}, image_width_{image_width}, image_height_{image_height} {
        Update();
    }

    // change paramters

    constexpr Point3 centre() const { return centre_; }
    constexpr void centre(Point3 centre) {
        centre_ = centre;

        focal_length_ = (look_at_ - centre_).norm();

        need_update_ = true;
    }

    constexpr Point3 look_at() const { return look_at_; }
    constexpr void look_at(Point3 look_at) {
        look_at_ = look_at;

        focal_length_ = (look_at_ - centre_).norm();

        need_update_ = true;
    }

    constexpr Vec3 up() const { return up_; }
    constexpr void up(Vec3 up) {
        up_ = up;

        need_update_ = true;
    }

    constexpr f64 fov() const { return fov_; }
    constexpr void fov(f64 fov) {
        fov_ = fov;

        need_update_ = true;
    }

    constexpr f64 focal_length() const { return focal_length_; }
    constexpr void focal_length(f64 focal_length) {
        focal_length_ = focal_length;

        need_update_ = true;
    }

    constexpr f64 defocus_angle() const { return defocus_angle_; }
    constexpr void defocus_angle(f64 defocus_angle) { defocus_angle_ = defocus_angle; }

    // const-only accessors

    constexpr Vec3 u() const {
        Update();

        return u_;
    }
    constexpr Vec3 v() const {
        Update();

        return v_;
    }
    constexpr Vec3 w() const {
        Update();

        return w_;
    }

    constexpr Vec3 d_u_pixel() const {
        Update();

        return d_u_pixel_;
    }
    constexpr Vec3 d_v_pixel() const {
        Update();

        return d_v_pixel_;
    }

    constexpr u32 image_width() const { return image_width_; }
    constexpr u32 image_height() const { return image_height_; }

    constexpr Vec3 PixelToWorld(f64 i, f64 j) const {
        Update();

        return pixel_00_ + i * d_u_pixel_ + j * d_v_pixel_;
    }

    constexpr f64 defocus_radius() const {
        return focal_length() * std::tan(deg2rad(defocus_angle()) / 2);
    }

    //

    constexpr void rotate(f64 deg) {
        Update();

        f64 rad = deg2rad(deg);

        Vec3 u = std::cos(rad) * u_ + std::sin(rad) * v_;
        Vec3 v = -std::sin(rad) * u_ + std::cos(rad) * v_;

        u_ = u;
        v_ = v;
    }

   private:
    constexpr void Update() const {
        if (!need_update_) {
            return;
        }

        f64 aspect_ratio = static_cast<f64>(image_width_) / image_height_;

        w_ = -(look_at_ - centre_).normed();

        v_ = (up_ - dot(up_, w_) * w_).normed();

        u_ = cross(v_, w_);

        assert(is_zero(dot(u_, v_)));
        assert(is_zero(dot(u_, w_)));
        assert(is_zero(dot(v_, w_)));

        f64 h = focal_length_ * std::tan(deg2rad(fov_) / 2);

        const f64 viewport_height = 2 * h;

        f64 viewport_width = viewport_height * aspect_ratio;

        Vec3 u_viewport = viewport_width * u_;
        Vec3 v_viewport = -viewport_height * v_;

        d_u_pixel_ = u_viewport / image_width_;
        d_v_pixel_ = v_viewport / image_height_;

        // upper left of viewport: move from camera centre half the viewport length in u and v
        // direction respectively and focal_length` in negative w direction
        auto viewport_upper_left = centre_ - u_viewport / 2 - v_viewport / 2 - focal_length_ * w_;

        pixel_00_ = viewport_upper_left + 0.5 * d_u_pixel_ + 0.5 * d_v_pixel_;

        need_update_ = false;
    }

    // fundamental camera parameters

    Point3 centre_ = -Point3::e_z;
    Point3 look_at_ = Point3::origin;
    Vec3 up_ = Vec3::e_y;

    f64 fov_ = 80;

    f64 defocus_angle_ = 0;

    // saved camera properties, need to be recalculated after updating parameters

    mutable bool need_update_ = true;

    mutable Vec3 u_ = Vec3::e_x;  // points to right of camera
    mutable Vec3 v_ = Vec3::e_y;  // points to up of camera
    mutable Vec3 w_ = Vec3::e_z;  // points backward of camera

    mutable f64 focal_length_ = 1.0;

    // position of pixel 0, 0
    mutable Vec3 pixel_00_;

    // Vec3 pointing from pixel to right/lower neighbour resp.
    mutable Vec3 d_u_pixel_;
    mutable Vec3 d_v_pixel_;

    // const

    u32 image_width_;
    u32 image_height_;
};
