#include <iostream>
#include <memory>

#include "camera.h"
#include "image.h"
#include "raytracer.h"
#include "renderobject.h"
#include "renderobjectlist.h"
#include "sphere.h"
#include "vec3.h"

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

    camera.Render(img, world);

    std::cout << img;

    return 0;
}
