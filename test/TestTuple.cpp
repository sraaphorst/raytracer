/**
 * TestTuple.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "catch.hpp"

#include "common.h"
#include "tup.h"

using namespace raytracer;

TEST_CASE("tup should be constructible via initializer list", "[tup][initializer_list]") {
    tup t1{1, 2, 3, tup::vector_flag};
    tup t2 = {1, 2, 3, tup::vector_flag};
    REQUIRE(t1 == t2);
}

TEST_CASE("tup should be indexable", "[tup][index]") {
    tup t = tup::point(3, 1, 2);
    REQUIRE(t[0] == 3);
    REQUIRE(t[1] == 1);
    REQUIRE(t[2] == 2);
}

TEST_CASE("tup with w=1 is a point", "[tup][point]") {
    tup t{1, 1, 1, tup::point_flag};
    REQUIRE(t.isPoint());
}

TEST_CASE("tup with w=0 is a vector", "[tup][vector]") {
    tup t{1, 1, 1, tup::vector_flag};
    REQUIRE(t.isVector());
}

TEST_CASE("tup should be able to be multiplied by a factor on the left", "[tup][scalar_multiplication]") {
    tup t{1, 2, 3, tup::vector_flag};
    tup t2{2, 4, 6, tup::vector_flag};

    REQUIRE(2 * t == t2);
}

TEST_CASE("tup should be able to be multiplied by a factor on the right", "[tup][scalar_multiplication]") {
    tup t{1, 2, 3, tup::vector_flag};
    tup t2{2, 4, 6, tup::vector_flag};

    REQUIRE(t * 2 == t2);
}

TEST_CASE("tup should be able to be added to itself", "[tup][addition]") {
    tup t{1, 2, 3, tup::vector_flag};
    REQUIRE(2 * t == t + t);
}

TEST_CASE("tup should be able to be negated", "[tup][negation]") {
    tup t{1, 2, 3, tup::vector_flag};
    REQUIRE(-t == -1 * t);
}

TEST_CASE("tup addition should be commutative", "[tup][addition][commutativity]") {
    tup t1{1, 2, 3, tup::vector_flag};
    tup t2{2, 4, 6, tup::vector_flag};

    REQUIRE(t1 + t2 == t2 + t1);
}

TEST_CASE("tup addition should be associative", "[tup][addition][associativity]") {
    const tup &tx = tup_constants::x1;
    const tup &ty = tup_constants::y1;
    const tup &tz = tup_constants::z1;

    REQUIRE(tx + (ty + tz) == (tx + ty) + tz);
}

TEST_CASE("tup should have inverses", "[tup][inverse]") {
    tup t{1, 2, 3, tup::vector_flag};
    REQUIRE(t - t == tup_constants::zero_vector);
    REQUIRE(t + (-t) == tup_constants::zero_vector);
}

TEST_CASE("tup subtraction should not be commutative", "[tup][subtraction][commutativity]") {
    tup t1{1, 2, 3, tup::vector_flag};
    tup t2{2, 4, 6, tup::vector_flag};

    REQUIRE(t1 - t2 != t2 - t1);
}

TEST_CASE("tup subtraction should not be associative", "[tup][subtraction][associativity]") {
    const tup &tx = tup_constants::x1;
    const tup &ty = tup_constants::y1;
    const tup &tz = tup_constants::z1;

    REQUIRE(tx - (ty - tz) != (tx - ty) - tz);
}

TEST_CASE("tup subtraction of two points should result in a vector", "[tup][subtraction][point]") {
    tup t1 = tup::point(1, 0, 0);
    tup t2 = tup::point(0, 1, 0);
    REQUIRE((t1 - t2).isVector());
}

TEST_CASE("tup subtraction of a vector from a point should result in a point", "[tup][subtraction][point][vector]") {
    tup p = tup::point(1, 0, 0);
    tup v = tup::vector(0, 1, 0);
    REQUIRE((p - v).isPoint());
}

TEST_CASE("tup subtraction of two vectors should result in a vector", "[tup][subtraction][vector]") {
    tup v1 = tup::vector(1, 0, 0);
    tup v2 = tup::vector(0, 1, 0);
    REQUIRE((v1 - v2).isVector());
}

TEST_CASE("tup should be able to calculate dot product", "[tup][dot_product][vector]") {
    tup t1{1, 1, 1, tup::vector_flag};
    tup t2{2, 4, 8, tup::vector_flag};
    REQUIRE(t1.dot_product(t2) == 14);
}

TEST_CASE("tup dot product should be commutative", "[tup][dot_product][commutativity]") {
    tup t1{1, 2, 3, tup::vector_flag};
    tup t2{2, 4, 6, tup::vector_flag};
    REQUIRE(t1.dot_product(t2) == t2.dot_product(t1));
}

TEST_CASE("tup dot product should be distributive", "[tup][dot_product][distributivity]") {
    tup t1{1, 2, 3, tup::vector_flag};
    tup t2{2, 4, 6, tup::vector_flag};
    tup t3{-1, -2, -3, tup::vector_flag};
    REQUIRE(t1.dot_product(t2 + t3) == t1.dot_product(t2) + t1.dot_product(t3));
}

TEST_CASE("tup should be able to calculate cross product", "[tup][cross_product][vector]") {
    const tup &tx = tup_constants::x1;
    const tup &ty = tup_constants::y1;
    const tup &tz = tup_constants::z1;

    REQUIRE(tx.cross_product(ty) == tz);
    REQUIRE(ty.cross_product(tx) == -tz);
    REQUIRE(tx.cross_product(tz) == -ty);
    REQUIRE(tz.cross_product(tx) == ty);
    REQUIRE(ty.cross_product(tz) == tx);
    REQUIRE(tz.cross_product(ty) == -tx);
}

TEST_CASE("tup cross product should be self-inverting", "[tup][cross_product][inverse]") {
    const tup &tx = tup_constants::x1;
    const tup &ty = tup_constants::y1;
    const tup &tz = tup_constants::z1;

    REQUIRE(tx.cross_product(tx) == tup_constants::zero_vector);
    REQUIRE(ty.cross_product(ty) == tup_constants::zero_vector);
    REQUIRE(tz.cross_product(tz) == tup_constants::zero_vector);
}

TEST_CASE("tup cross product is not associative", "[tup][cross_product][associativity]") {
    const tup txy = tup_constants::x1 + tup_constants::y1;
    const tup &ty = tup_constants::y1;
    const tup &tz = tup_constants::z1;

    REQUIRE(txy.cross_product(ty.cross_product(tz)) != txy.cross_product(ty).cross_product(tz));
}

TEST_CASE("tup cross product is not commutative", "[tup][cross_product][commutativity]") {
    const tup &tx = tup_constants::x1;
    const tup &ty = tup_constants::y1;
    const tup &tz = tup_constants::z1;

    REQUIRE(tx.cross_product(ty) != ty.cross_product(tx));
    REQUIRE(tx.cross_product(tz) != tz.cross_product(tx));
    REQUIRE(ty.cross_product(tz) != tz.cross_product(ty));
}

TEST_CASE("tup point should not be able to calculate cross product", "[tup][cross_product][point]") {
    REQUIRE_THROWS(tup{1, 0, 0, tup::vector_flag}.cross_product(tup{0, 1, 0, tup::point_flag}));
}

TEST_CASE("tup point should create a point", "[tup][point]") {
    REQUIRE(tup::point(0, 0, 0).isPoint());
}

TEST_CASE("tup vector should create a vector", "[tup][vector]") {
    REQUIRE(tup::vector(0, 0, 0).isVector());
}

TEST_CASE("tup comparison equality should be able to tolerate slight offsets", "[tup][equality]") {
    tup t1 = tup::point(1, 0, 0);
    tup t2 = tup::point(1 - EPSILON/2, EPSILON/2, -EPSILON/2);
    REQUIRE(t1 == t2);
}

TEST_CASE("tup should calculate the magnitude of a vector", "[tup][magnitude]") {
    tup t = tup::vector(1, 4, 8);
    REQUIRE(t.magnitude() == 9);
}

TEST_CASE("tup normalization should produce a vector of magnitude 1", "[tup][normalization]") {
    tup t = tup::vector(1, 4, 8);
    REQUIRE(t.normalize().magnitude() == 1);
}
