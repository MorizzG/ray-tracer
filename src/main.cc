#include <cstdint>
#include <iostream>

#include "colour.h"
#include "image.h"
#include "ray.h"
#include "vec3.h"

bool IntersectsSphere(const Ray& r, const Point3& centre, double radius) {
    Vec3 oc = r.origin() - centre;

    double a = r.direction().squared();
    double b = 2.0 * dot(oc, r.direction());
    double c = oc.squared() - radius * radius;

    auto discr = b * b - 4 * a * c;

    return discr >= 0;
}

Colour RayColour(const Ray& ray) {
    if (IntersectsSphere(ray, -Vec3::e_z, 0.5)) {
        return {1.0, 0.0, 0.0};
    }

    auto unit_dir = ray.direction().normed();

    double a = 0.5 * (unit_dir.y() + 1.0);

    return (1.0 - a) * Colour{1.0, 1.0, 1.0} + a * Colour{0.5, 0.7, 1.0};
}

int main(int /* argc */, char* /* argv */[]) {
    // image properties

    constexpr double aspect_ratio = 16.0 / 9.0;

    constexpr uint32_t image_width = 800;

    constexpr auto image_height = static_cast<uint32_t>(image_width / aspect_ratio);

    static_assert(image_height >= 1);

    // camera

    constexpr double focal_length = 1.0;

    constexpr double viewport_height = 2.0;
    constexpr double viewport_width =
        viewport_height * static_cast<double>(image_width) / image_height;

    Point3 camera_centre{0.0, 0.0, 0.0};

    // viewport

    constexpr Vec3 u_viewport = viewport_width * Vec3::e_x;
    constexpr Vec3 v_viewport = -viewport_height * Vec3::e_y;

    constexpr Vec3 d_u_pixel = u_viewport / image_width;
    constexpr Vec3 d_v_pixel = v_viewport / image_height;

    auto viewport_upper_left =
        camera_centre - focal_length * Vec3::e_z - u_viewport / 2 - v_viewport / 2;

    auto pixel_00 = viewport_upper_left + 0.5 * d_u_pixel + 0.5 * d_v_pixel;

    // render

    Image img{image_width, image_height};

    for (uint32_t j = 0; j < img.height(); j++) {
        std::clog << "\rWriting line " << j << " of " << img.height() << std::flush;

        for (uint32_t i = 0; i < img.width(); i++) {
            auto pixel_centre = pixel_00 + i * d_u_pixel + j * d_v_pixel;

            auto ray_direction = pixel_centre - camera_centre;

            Ray r{camera_centre, ray_direction};

            img[i, j] = RayColour(r);
        }
    }

    std::cout << img;

    return 0;
}
