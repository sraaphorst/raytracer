/**
 * solidpattern.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "pattern.h"
#include "vector.h"

namespace raytracer {
    class SolidPattern final: public Pattern {
        Colour colour;

    public:
        /// Default colour is white.
        SolidPattern() noexcept;
        explicit SolidPattern(const Colour&) noexcept;
        explicit SolidPattern(Colour&&) noexcept;
        SolidPattern(const SolidPattern&) noexcept = default;
        SolidPattern(SolidPattern&&) noexcept = default;
        SolidPattern &operator=(const SolidPattern&) noexcept = default;
        SolidPattern &operator=(SolidPattern&&) noexcept = default;

        inline const Colour colour_at(const Tuple&) const noexcept override {
            return colour;
        }

        const Colour getColour() const noexcept;
        void setColour(const Colour&) noexcept;
    private:
        bool doCompare(const Pattern &other) const noexcept override;
    };
}


