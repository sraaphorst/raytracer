/**
 * canvas.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "canvas.h"

namespace raytracer {
    Canvas::Canvas(size_t width, size_t height): width{width}, height{height},
        pixels{width, std::vector<Colour>{height, predefined_colours::black}} {}
}