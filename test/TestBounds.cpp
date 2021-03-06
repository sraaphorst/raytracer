/**
 * TestBounds.cpp
 *
 * By Sebastian Raaphorst, 2019.
 */

#include <catch.hpp>

#include <array>

#include "affine_transform.h"
#include "common.h"
#include "ray.h"
#include "vec.h"
#include "bounding_box.h"

using namespace raytracer;
using namespace raytracer::impl;

TEST_CASE("BoundingBox: Creating an empty BoundingBox") {
    constexpr BoundingBox box{};
    REQUIRE(box.getMinPoint() == predefined_tuples::min_point);
    REQUIRE(box.getMaxPoint() == predefined_tuples::max_point);
}

TEST_CASE("BoundingBox: Create a BoundingBow with volume") {
    constexpr BoundingBox box{make_point(-1, -2, -3), make_point(3, 2, 1)};
    REQUIRE(box.getMinPoint() == make_point(-1, -2, -3));
    REQUIRE(box.getMaxPoint() == make_point(3, 2, 1));
}

TEST_CASE("BoundingBox: Adding points to an empty BoundingBox") {
    BoundingBox box{};
    box.addPoint(make_point(-5, 2, 0));
    box.addPoint(make_point(7, 0, -3));
    REQUIRE(box.getMinPoint() == make_point(-5, 0, -3));
    REQUIRE(box.getMaxPoint() == make_point(7, 2, 0));
}

TEST_CASE("BoundingBox:: Adding one bounding box to another") {
    BoundingBox box1{make_point(-5, -2, 0), make_point(7, 4, 4)};
    const BoundingBox box2{make_point(8, -7, -2), make_point(14, 2, 8)};
    box1.addBox(box2);
    REQUIRE(box1.getMinPoint() == make_point(-5, -7, -2));
    REQUIRE(box1.getMaxPoint() == make_point(14, 4, 8));
}

TEST_CASE("BoundingBox: Checking to see if a box contains a given point") {
    BoundingBox box{make_point(5, -2, 0), make_point(11, 4, 7)};
    std::array<Tuple, 9> points {{
        make_point( 5, -2, 0),
        make_point(11,  4,  7),
        make_point( 8,  1,  3),
        make_point( 3,  0,  3),
        make_point( 8, -4,  3),
        make_point( 8,  1, -1),
        make_point(13,  1,  3),
        make_point( 8,  5,  3),
        make_point( 8,  1,  8)
    }};
    std::array<bool, 9> incidence {{
        true, true, true,
        false, false, false,
        false, false, false
    }};

    for (size_t i = 0; i < 9; ++i)
        REQUIRE(box.containsPoint(points[i]) == incidence[i]);
}

TEST_CASE("BoundingBox: Checking to see if a box contains a given box") {
    BoundingBox box{make_point(5, -2, 0), make_point(11, 4, 7)};
    std::array<BoundingBox, 4> boxes {{
        BoundingBox{make_point(5, -2,  0), make_point(11, 4, 7)},
        BoundingBox{make_point(6, -1,  1), make_point(10, 3, 6)},
        BoundingBox{make_point(4, -3, -1), make_point(11, 4, 7)},
        BoundingBox{make_point(6, -1,  1), make_point(12, 5, 8)}
    }};
    std::array<bool, 4> incidence {{
        true, true, false, false
    }};

    for (size_t i = 0; i < 4; ++i)
        REQUIRE(box.containsBox(boxes[i]) == incidence[i]);
}

TEST_CASE("BoundingBox: Transforming a bounding box") {
    const BoundingBox box{make_point(-1, -1, -1), make_point(1, 1, 1)};
    const auto trans = rotation_x(math_constants::pi_by_four<>) * rotation_y(math_constants::pi_by_four<>);
    const auto box2 = box.transform(trans);
    REQUIRE(box2.getMinPoint() == make_point(-1.4142, -1.7071, -1.7071));
    REQUIRE(box2.getMaxPoint() == make_point( 1.4142,  1.7071,  1.7071));
}

TEST_CASE("BoundingBox: Intersecting a ray with a bounding box at the origin") {
    const BoundingBox box{make_point(-1, -1, -1), make_point(1, 1, 1)};

    std::array<Tuple, 13> origins {{
        make_point( 5,    0.5,  0),
        make_point(-5,    0.5,  0),
        make_point( 0.5,  5,    0),
        make_point( 0.5, -5,    0),
        make_point( 0.5,  0,    5),
        make_point( 0.5,  0,   -5),
        make_point( 0,    0.5,  0),
        make_point(-2,    0,    0),
        make_point( 0,   -2,    0),
        make_point( 0,    0,   -2),
        make_point( 2,    0,    2),
        make_point( 0,    2,    2),
        make_point( 2,    2,    0)
    }};

    std::array<Tuple, 13> directions {{
        make_vector(-1,  0,  0),
        make_vector( 1,  0,  0),
        make_vector( 0, -1,  0),
        make_vector( 0,  1,  0),
        make_vector( 0,  0, -1),
        make_vector( 0,  0,  1),
        make_vector( 0,  0,  1),
        make_vector( 2,  4,  6),
        make_vector( 6,  2,  4),
        make_vector( 4,  6,  2),
        make_vector( 0,  0, -1),
        make_vector( 0, -1,  0),
        make_vector(-1,  0,  0)
    }};

    std::array<bool, 13> results {{
        true, true, true, true, true, true, true,
        false, false, false, false, false, false
    }};

    for (size_t i = 0; i < 13; ++i) {
        const Ray r{origins[i], directions[i].normalize()};
        REQUIRE(box.intersects(r) == results[i]);
    }
}

TEST_CASE("BoundingBox: Intersecting a ray with a non-cubic bounding box") {
    const BoundingBox box{make_point(5, -2, 0), make_point(11, 4, 7)};

    std::array<Tuple, 13> origins {{
        make_point(15,  1,  2),
        make_point(-5, -1,  4),
        make_point( 7,  6,  5),
        make_point( 9, -5,  6),
        make_point( 8,  2, 12),
        make_point( 6,  0, -5),
        make_point( 8,  1,  3.5),
        make_point( 9, -1, -8),
        make_point( 8,  3, -4),
        make_point( 9, -1, -2),
        make_point( 4,  0,  9),
        make_point( 8,  6, -1),
        make_point(12,  5,  4)
    }};

    std::array<Tuple, 13> directions {{
        make_vector(-1,  0,  0),
        make_vector( 1,  0,  0),
        make_vector( 0, -1,  0),
        make_vector( 0,  1,  0),
        make_vector( 0,  0, -1),
        make_vector( 0,  0,  1),
        make_vector( 0,  0,  1),
        make_vector( 2,  4,  6),
        make_vector( 6,  2,  4),
        make_vector( 4,  6,  2),
        make_vector( 0,  0, -1),
        make_vector( 0, -1,  0),
        make_vector(-1,  0,  0)
    }};

    std::array<bool, 13> results {{
        true, true, true, true, true, true, true,
        false, false, false, false, false, false
    }};

    for (size_t i = 0; i < 13; ++i) {
        const Ray r{origins[i], directions[i].normalize()};
        REQUIRE(box.intersects(r) == results[i]);
    }
}
