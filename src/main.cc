#include <iostream>
#include <memory>

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

    /* auto lamb = std::make_shared<Lambertian>(Colour{0.7, 0.0, 0.5});
    auto metal = std::make_shared<Metal>(Colour{0.3, 0.3, 0.3});

    auto ground = std::make_unique<Sphere>(Point3{0, -100.5, -1}, 100, metal);
    auto sphere = std::make_unique<Sphere>(-Vec3::e_z, 0.5, lamb);

    world.Add(std::move(ground));
    world.Add(std::move(sphere)); */

    auto mat_ground = std::make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
    auto mat_lamb = std::make_shared<Lambertian>(Colour(0.1, 0.2, 0.5));
    auto mat_metal = std::make_shared<Metal>(Colour(0.9, 0.4, 0.6), 0.0);
    auto mat_dielec = std::make_shared<Dielectric>(1.5, 0.0);
    auto mat_dielec2 = std::make_shared<Dielectric>(0.66, 0.0);

    world.Add(std::make_unique<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, mat_ground));

    world.Add(std::make_unique<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, mat_lamb));

    world.Add(std::make_unique<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, mat_dielec));
    world.Add(std::make_unique<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, mat_dielec2));

    world.Add(std::make_unique<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, mat_metal));

    // camera

    constexpr f64 aspect_ratio = 16.0 / 9.0;

    constexpr u32 image_width = 800;
    constexpr auto image_height = static_cast<u32>(image_width / aspect_ratio);

    // constexpr u32 image_width = 1920;
    // constexpr u32 image_height = 1080;

    static_assert(image_height >= 1);

    constexpr f64 focal_length = 1.0;

    constexpr Point3 camera_centre{0.0, 0.0, 0.0};

    Camera camera{camera_centre, image_width, image_height, focal_length};

    std::clog << camera.u() << newline;
    std::clog << camera.v() << newline;
    std::clog << camera.w() << newline << newline;

    camera.look_at(Point3{0.8, 0.0, 1.0}, Vec3::e_x);

    std::clog << camera.u() << newline;
    std::clog << camera.v() << newline;
    std::clog << camera.w() << newline << newline;

    // render

    Renderer renderer{camera};

    auto img = renderer.Render(world);

    std::cout << img;

    return 0;
}
