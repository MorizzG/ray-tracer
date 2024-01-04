#pragma once

#include <cassert>
#include <cmath>

#include "raytracer.h"
#include "vec3.h"

class Camera {
   public:
    constexpr Camera(u32 image_width, u32 image_height)
        : image_width_{image_width}, image_height_{image_height} {}

    // change paramters

    constexpr Point3 centre() const { return centre_; }
    constexpr void centre(Point3 centre) {
        centre_ = centre;

        focal_length_ = (look_at_ - centre_).norm();
    }

    constexpr Point3 look_at() const { return look_at_; }
    constexpr void look_at(Point3 look_at) {
        look_at_ = look_at;

        focal_length_ = (look_at_ - centre_).norm();
    }

    constexpr Vec3 up() const { return up_; }
    constexpr void up(Vec3 up) { up_ = up; }

    constexpr f64 fov() const { return fov_; }
    constexpr void fov(f64 fov) { fov_ = fov; }

    constexpr f64 focal_length() const { return focal_length_; }
    constexpr void focal_length(f64 focal_length) { focal_length_ = focal_length; }

    constexpr f64 defocus_angle() const { return defocus_angle_; }
    constexpr void defocus_angle(f64 defocus_angle) { defocus_angle_ = defocus_angle; }

    // const-only accessors

    constexpr Vec3 u() const { return u_; }
    constexpr Vec3 v() const { return v_; }
    constexpr Vec3 w() const { return w_; }

    constexpr Vec3 d_u_pixel() const { return d_u_pixel_; }
    constexpr Vec3 d_v_pixel() const { return d_v_pixel_; }

    constexpr u32 image_width() const { return image_width_; }
    constexpr u32 image_height() const { return image_height_; }

    constexpr Vec3 PixelToWorld(f64 i, f64 j) const {
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
    constexpr void Update() {
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
    }

   private:
    // fundamental camera parameters

    Point3 centre_ = -Point3::e_z;
    Point3 look_at_ = Point3::origin;
    Vec3 up_ = Vec3::e_y;

    f64 fov_ = 80;

    f64 defocus_angle_ = 0;

    // saved camera properties, need to be recalculated after updating parameters

    Vec3 u_ = Vec3::e_x;  // points to right of camera
    Vec3 v_ = Vec3::e_y;  // points to up of camera
    Vec3 w_ = Vec3::e_z;  // points backward of camera

    f64 focal_length_ = 1.0;

    // position of pixel 0, 0
    Vec3 pixel_00_;

    // Vec3 pointing from pixel to right/lower neighbour resp.
    Vec3 d_u_pixel_;
    Vec3 d_v_pixel_;

    // const

    u32 image_width_;
    u32 image_height_;
};
