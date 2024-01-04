#include <iostream>
#include <memory>

#include "2dshapes.h"
#include "camera.h"
#include "colour.h"
#include "image.h"
#include "material.h"
#include "raytracer.h"
#include "renderer.h"
#include "renderobjectlist.h"
#include "sphere.h"
#include "vec3.h"

int main(int /* argc */, char* /* argv */[]) {
    // world

    RenderObjectList world;

    constexpr f64 focal_length = 5.0;

    /* auto lamb = std::make_shared<Lambertian>(Colour{0.7, 0.0, 0.5});
    auto metal = std::make_shared<Metal>(Colour{0.3, 0.3, 0.3});

    auto ground = std::make_unique<Sphere>(Point3{0, -100.5, -1}, 100, metal);
    auto sphere = std::make_unique<Sphere>(-Vec3::e_z, 0.5, lamb);

    world.Add(std::move(ground));
    world.Add(std::move(sphere)); */

    auto mat_ground = std::make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
    auto mat_lamb = std::make_shared<Lambertian>(Colour(1.0, 0.0, 0.0));
    auto mat_metal = std::make_shared<Metal>(Colour(0.9, 0.4, 0.6), 0.0);
    auto mat_metal2 = std::make_shared<Metal>(Colour(0.3, 0.5, 0.9), 0.0);
    // auto mat_dielec = std::make_shared<Dielectric>(1.5, 0.0);
    // auto mat_dielec2 = std::make_shared<Dielectric>(0.66, 0.0);

    world.Add(std::make_unique<Sphere>(Point3(0, -100, 0), 100, mat_ground));

    world.Add(std::make_unique<Rectangle>(Point3(-2.5, -2.5, 50), 5 * Vec3::e_x, 5 * Vec3::e_y,
                                          mat_lamb));

    /* constexpr i32 N = 5;

    for (i32 x = -N; x <= N; x++) {
        for (i32 y = 0; y <= N; y++) {
            world.Add(std::make_unique<Rectangle>(Point3(x, y, 50), Vec3::e_x, Vec3::e_y,
                                                  (x + y) % 2 == 0 ? mat_lamb : mat_lamb2));
        }
    } */

    // world.Add(std::make_unique<Sphere>(Point3(-1.0, 0.0, 0.0), 0.5, mat_dielec));
    // world.Add(std::make_unique<Sphere>(Point3(-1.0, 0.0, 0.0), 0.4, mat_dielec2));

    // world.Add(std::make_unique<Sphere>(Point3{-1, 0, 0}, 0.5, mat_metal2));

    // world.Add(std::make_unique<Sphere>(Point3(1, 0, 0), 0.5, mat_metal));

    /* for (f64 z = 0.5; z < 10; z += 0.5) {
        world.Add(std::make_unique<Sphere>(Point3(1, 0, z), 1 * z / focal_length, mat_metal));
    } */

    // camera

    constexpr f64 aspect_ratio = 16.0 / 9.0;

    constexpr u32 image_width = 1200;
    constexpr auto image_height = static_cast<u32>(image_width / aspect_ratio);

    // constexpr u32 image_width = 1920;
    // constexpr u32 image_height = 1080;

    static_assert(image_height >= 1);

    constexpr Point3 camera_centre{0, 1, -1};

    Camera camera{camera_centre, image_width, image_height};

    camera.look_at(Vec3{0, 0, 1});
    camera.focal_length(focal_length);

    camera.defocus_angle(1);
    // camera.focal_length(1);

    // camera.fov(40);
    // camera.centre(Point3{-2, 2, 1});

    // render

    Renderer renderer{camera};

    auto img = renderer.Render(world);

    std::cout << img;

    return 0;
}
