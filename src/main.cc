#include <iostream>
#include <memory>

#include "camera.h"
#include "colour.h"
#include "image.h"
#include "ray.h"
#include "raytracer.h"
#include "renderobject.h"
#include "renderobjectlist.h"
#include "sphere.h"
#include "vec3.h"

Colour RayColour(const Ray& ray, const RenderObject& world) {
    auto hit_record = world.hit(ray, 0.0, kInf);

    if (hit_record.has_value()) {
        // assert(hit_record.front_face);

        Vec3 n = hit_record->normal;

        if (!hit_record->front_face) {
            // return Colour{0.0, 0.0, 0.0};
            return Colour{0.5 * (n + Vec3{1, 1, 1})};
        }

        return Colour{0.5 * (n + Vec3{1, 1, 1})};
    }

    auto unit_dir = ray.direction().normed();

    double a = 0.5 * (unit_dir.y() + 1.0);

    return (1.0 - a) * Colour{1.0, 1.0, 1.0} + a * Colour{0.5, 0.7, 1.0};
}

int main(int /* argc */, char* /* argv */[]) {
    // image

    constexpr double aspect_ratio = 16.0 / 9.0;

    constexpr u32 image_width = 800;

    constexpr auto image_height = static_cast<u32>(image_width / aspect_ratio);

    static_assert(image_height >= 1);

    Image img{image_width, image_height};

    // world

    RenderObjectList world;

    world.Add(std::make_shared<Sphere>(Vec3{0, -100.5, -1}, 100));
    world.Add(std::make_shared<Sphere>(-Vec3::e_z, 0.5));

    // camera

    constexpr double focal_length = 1.0;

    constexpr Point3 camera_centre{0.0, 0.0, 0.0};

    Camera camera{camera_centre, img.width(), img.height(), focal_length};

    // render

    for (u32 j = 0; j < img.height(); j++) {
        std::clog << "\rWriting line " << j << " of " << img.height() << std::flush;

        for (u32 i = 0; i < img.width(); i++) {
            auto ray = camera.CastRay(i, j);

            img[i, j] = RayColour(ray, world);
        }
    }

    std::cout << img;

    return 0;
}
