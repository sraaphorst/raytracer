/**
 * pattern.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "affine_transform.h"
#include "matrix.h"
#include "pattern.h"
#include "shape.h"

using namespace raytracer::shapes;

namespace raytracer {
    Pattern::Pattern():
        transformation{predefined_matrices::I<>},
        transformationInverse{predefined_matrices::I<>} {}
    Pattern::Pattern(const Transformation &t):
        transformation{t},
        transformationInverse{predefined_matrices::I<>} {}
    Pattern::Pattern(Transformation &&t):
        transformation{t},
        transformationInverse{predefined_matrices::I<>} {}

    const Transformation &Pattern::getTransformation() const noexcept {
        return transformation;
    }
    void Pattern::setTransformation(const Transformation &t) noexcept {
        transformation = t;
        transformationInverse = transformation.invert();
    }
    void Pattern::setTransformation(Transformation &&t) noexcept {
        transformation = std::move(t);
        transformationInverse = transformation.invert();
    }

    bool Pattern::operator==(const Pattern &other) const noexcept {
        return typeid(*this) == typeid(other) && doCompare(other);
    }

    bool Pattern::operator!=(const Pattern &other) const noexcept {
        return !(*this == other);
    }

    const Colour Pattern::colourAtObject(const std::shared_ptr<const Shape> &shape,
            const Tuple &world_point) const noexcept {
        const auto object_point = shape->worldToObject(world_point);
        const auto pattern_point = transformationInverse * object_point;
        return colourAt(pattern_point);
    }

    bool Pattern::doCompare(const Pattern&) const noexcept {
        return true;
    }
}