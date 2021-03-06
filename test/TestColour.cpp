/**
 * TestColour.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "common.h"
#include "vec.h"

using namespace raytracer;

TEST_CASE("Colour: Colours are RGB tuples") {
    constexpr Colour rgb{-0.5, 0.4, 1.7};
    REQUIRE(ALMOST_EQUALS(rgb[colour_constants::r], -0.5));
    REQUIRE(ALMOST_EQUALS(rgb[colour_constants::g],  0.4));
    REQUIRE(ALMOST_EQUALS(rgb[colour_constants::b],  1.7));
}

TEST_CASE("Colour: Colours can be added") {
    constexpr Colour c1{0.9, 0.6, 0.75};
    constexpr Colour c2{0.7, 0.1, 0.25};
    [[maybe_unused]] constexpr auto res1 = c1 + c2;
    [[maybe_unused]] constexpr auto res2 = c1 + c2 == Colour{1.6, 0.7, 1.0};
    REQUIRE(c1 + c2 == Colour{1.6, 0.7, 1.0});
}

TEST_CASE("Colour: Colours can be subtracted") {
    constexpr Colour c1{0.9, 0.6, 0.75};
    constexpr Colour c2{0.7, 0.1, 0.25};
    [[maybe_unused]] constexpr auto res1 = c1 - c2;
    [[maybe_unused]] constexpr auto res2 = c1 - c2 == Colour{0.2, 0.5, 0.5};
    REQUIRE(c1 - c2 == Colour{0.2, 0.5, 0.5});
}

TEST_CASE("Colour: Colours can be multiplied by a scalar") {
    constexpr Colour c{0.2, 0.3, 0.4};
    [[maybe_unused]] constexpr auto res1 = 2 * c;
    [[maybe_unused]] constexpr auto res2 = 2 * c == Colour{0.4, 0.6, 0.8};
    REQUIRE(2 * c == Colour{0.4, 0.6, 0.8});
}

TEST_CASE("Colour: Colours can be multiplied by the hadamard product") {
    constexpr Colour c1{1  , 0.2, 0.4};
    constexpr Colour c2{0.9, 1,   0.1};
    [[maybe_unused]] constexpr auto res1 = c1 * c2;
    [[maybe_unused]] constexpr auto res2 = c1 * c2 == Colour{0.9, 0.2, 0.04};
    REQUIRE(c1 * c2 == Colour{0.9, 0.2, 0.04});
}

TEST_CASE("Colour: Colours should be able to determine when they are valid") {
    constexpr Colour c{0.5, 0, 1};
    [[maybe_unused]] constexpr auto res = c.isValidColour();
    REQUIRE(c.isValidColour());
}

TEST_CASE("Colour: Colours should be able to determine when they are invalid") {
    constexpr Colour c{0.5, 0, 1.1};
    [[maybe_unused]] constexpr auto res = c.isValidColour();
    REQUIRE_FALSE(c.isValidColour());
}