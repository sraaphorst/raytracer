/**
 * Matrix.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <array>

#include "transformers.h"
#include "vector.h"

namespace raytracer {
    using namespace transformers;

    template<typename T, size_t rows, size_t cols,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Matrix {
    public:
        using type = T;
        using row_type = std::array<T, cols>;
        using col_type = std::array<T, rows>;
        using matrix_type = std::array<row_type, rows>;

    protected:
        const matrix_type contents;

        /// Used in matrix multiplication.
        static constexpr T dot_product(const row_type &r1, const row_type &r2) noexcept {
            return transformers::Reducer<T, T, cols>::result(
                    [](T t1, T t2) { return t1 * t2; },
                    [](const T &t1, const T &t2) { return t1 + t2; }, 0,
                    r1, r2);
        }

    public:
        constexpr Matrix() noexcept {};
        constexpr Matrix(const matrix_type &contents) noexcept : contents{contents} {}
        constexpr Matrix(matrix_type&& contents) noexcept : contents{contents} {}
        constexpr Matrix(const Matrix&) noexcept = default;
        constexpr Matrix(Matrix&&) noexcept = default;

        constexpr Matrix(std::initializer_list<row_type> lst):
            contents{
                make_array<row_type,rows>([&lst](int i) {
                    return make_array<T,cols>([&lst,i](int j) {
                        return lst.begin()[i][j];
                    });
                })
            } {}

        ~Matrix() = default;

        constexpr Matrix &operator=(const Matrix&) noexcept = default;
        constexpr Matrix &operator=(Matrix&&) noexcept = default;

        constexpr const row_type &operator[](size_t idx) const {
            return contents[idx];
        }

        /**
         * Despite my best efforts, I cannot make this genuinely constexpr due to the lambdas.
         * You cannot, for example, set:
         *     constexpr Matrix<T, cols, rows> m = transpose()
         * without getting an error about transpose() not being constexpr.
         * I even tried using templates to unroll without using a function, but have to then pass contents in, and
         * "this" is not constexpr, so it didn't work either.
         */
        constexpr const Matrix<T, cols, rows> transpose() const {
            using column_type = std::array<T, rows>;

            return Matrix<T, cols, rows>{indextransform<column_type, cols>([this](int c) {
                return indextransform<T, rows>([this, c](int r){ return this->contents[r][c]; });
            })};
        }

        constexpr Matrix operator+(const Matrix &other) const {
            return Matrix{contents + other.contents};
        };

        constexpr Matrix operator-(const Matrix &other) const {
            return Matrix{contents - other.contents};
        }

        /// I don't think this will be constexpr because transpose is never really constexpr.
        constexpr Matrix<T, rows, rows> operator*(const Matrix<T, cols, rows> &other) const {
            // This cannot be declared constexpr.
            const Matrix otherT = other.transpose();

            return Matrix<T, rows, rows>{indextransform<std::array<T, rows>, rows>([this, otherT](int i) {
                return indextransform<T, rows>([this, otherT, i](int j){ return dot_product(contents[i], otherT.contents[j]); });
            })};
        }

        /// This one, however, is constexpr, as checked by assigning the value to be returned to a constexpr variable.
        constexpr Vector<T, rows> operator*(const Vector<T, cols> &v) const {
            return Vector<T, rows>{unitransform<T, row_type, rows>([&v] (const row_type &r) { return dot_product(r, v.contents); }, contents)};
        }

        constexpr Matrix operator*(T factor) const {
            return Matrix{factor * contents};
        }

        constexpr Matrix operator/(T denom) const {
            return Matrix{contents / denom};
        };

        constexpr bool operator==(const Matrix &other) const {
            return equals(contents, other.contents);
        }

        constexpr bool operator!=(const Matrix &other) const {
            return !(*this == other);
        }

        static constexpr size_t row_count() {
            return rows;
        }

        static constexpr size_t column_count() {
            return cols;
        }

        static constexpr size_t size() {
            static_assert(rows == cols, "Matrix::size() only for use with square matrices");
            return rows;
        }

        constexpr T determinant() {
            static_assert(rows == cols, "Matrix::determinant() only for use with square matrices");
            return array_determinant<T,rows>(contents);
        }

        /// Omit row i and column j to get a submatrix of one dimension less in rows and cols.
        Matrix<T, rows-1, cols-1> submatrix(size_t i, size_t j) {
            return array_submatrix<T,rows,cols>(contents, i, j);
        }

        /// Calculate the minor(i,j) of a matrix, i.e. the determinant of the submatrix(i,j).
        T minor(size_t i, size_t j) {
            static_assert(rows == cols, "Matrix::minor() only for use with square matrices");
            return submatrix(i, j).determinant();
        }

        /// Calculate the cofactor(i,j) of a matrix, which is just (i+j)^(-1) * minor(i,j).
        T cofactor(size_t i, size_t j) {
            static_assert(rows == cols, "Matrix::cofactor() only for use with square matrices");
            return ((i + j) % 2 == 0 ? 1 : -1) * submatrix(i, j).determinant();
        }

        /// Multiply by factor on the left.
        friend constexpr Matrix operator*(T factor, const Matrix &m) {
            return m * factor;
        }

        /// Multiply by vector on the left.
        template<typename S, size_t m, size_t n>
        friend constexpr Vector<S,n> operator*(const Vector<S,m>&, const Matrix<S,m,n,S>&);
    };

    /// We use the property that (v * A)^T = A^t * v^t.
    template<typename S, size_t m, size_t n>
    constexpr Vector<S,n> operator*(const Vector<S,m> &v, const Matrix<S,m,n,S> &matrix) {
        return matrix.transpose() * v;
    }

    struct matrix_constants {
        /**
         * Unlike make_array, make_uniform_matrix and make_diagonal matrix allow us to be constexpr as they don't
         * use any std::function.
         */
        template<typename T, size_t R, size_t C>
        static constexpr Matrix<T, R, C> ones = make_uniform_matrix<T, R, C>(1);

        /// Identity matrix, only defined as a square matrix.
        template<typename T, size_t N>
        static constexpr Matrix<T, N, N> I = make_diagonal_matrix<T, N, N>(0, 1);
    };
}
