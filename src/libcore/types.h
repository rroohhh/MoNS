/*
  Copyright (C) 2017 by Robin Heinemann. All Rights Reserved.
  types.h -- basic types
*/

#ifndef __TYPES_H_
#define __TYPES_H_

#include <cstdint>

#include "types/algorithm.h"
#include "types/chrono.h"
#include "types/declval.h"
#include "types/except.h"
#include "types/forward.h"
#include "types/fstream.h"
#include "types/function.h"
#include "types/ios.h"
#include "types/iterator.h"
#include "types/limits.h"
#include "types/map.h"
#include "types/meta.h"
#include "types/move.h"
#include "types/ostream.h"
#include "types/pair.h"
#include "types/pointer.h"
#include "types/ptr_fun.h"
#include "types/random.h"
#include "types/sstream.h"
#include "types/string.h"
#include "types/thread.h"
#include "types/tuple.h"
#include "types/vector.h"

namespace mons {
    using i8    = int8_t;
    using i16   = int16_t;
    using i32   = int32_t;
    using i64   = int64_t;
    using u8    = uint8_t;
    using u16   = uint16_t;
    using u32   = uint32_t;
    using u64   = uint64_t;
    using size  = int_fast64_t;
    using usize = uint_fast64_t;

    using f32 = float;
    using f64 = double;
    using f   = double;

    template <typename valueType>
    struct vector3 {
        valueType pos[3];
        constexpr vector3() : pos() {}
        constexpr vector3(valueType a) : pos{a, a, a} {}
        constexpr vector3(valueType a, valueType b, valueType c)
            : pos{a, b, c} {}

        vector3(const vector3 & other) {
            this->pos[0] = other.pos[0];
            this->pos[1] = other.pos[1];
            this->pos[2] = other.pos[2];
        }

        template <typename VT>
        friend ostream & operator<<(ostream & output, const vector3<VT> & vec) {
            return output << "[" << vec.pos[0] << ", " << vec.pos[1] << ", "
                          << vec.pos[2] << "]";
        }

        vector3<valueType> operator+(const vector3<valueType> & rhs) const {
            vector3<valueType> vec(*this);
            vec += rhs;
            return vec;
        }

        vector3<valueType> operator-(const vector3<valueType> & rhs) const {
            vector3<valueType> vec(*this);
            vec -= rhs;
            return vec;
        }

        vector3<valueType> operator-() const {
            vector3<valueType> vec(*this);
            vec -= vector3<valueType>();
            return vec;
        }

        template <typename scalaType>
        vector3<valueType> operator*(const scalaType & rhs) const {
            vector3<valueType> vec(*this);
            vec *= rhs;
            return vec;
        }

        valueType operator*(const vector3<valueType> & rhs) const {
            return pos[0] * rhs.pos[0] + pos[1] * rhs.pos[1] + pos[2] * rhs.pos[2];
        }

        template <typename scalaType>
        vector3<valueType> operator/(const scalaType & rhs) const {
            vector3<valueType> vec(*this);
            vec /= rhs;
            return vec;
        }

        vector3<valueType> & operator+=(const vector3<valueType> & rhs) {
			pos[0] += rhs.pos[0];
			pos[1] += rhs.pos[1];
			pos[2] += rhs.pos[2];

            return *this;
        }

        vector3<valueType> & operator-=(const vector3<valueType> & rhs) {
			pos[0] -= rhs.pos[0];
			pos[1] -= rhs.pos[1];
			pos[2] -= rhs.pos[2];

            return *this;
        }

        template <typename scalaType>
        vector3<valueType> & operator*=(const scalaType & rhs) {
			pos[0] *= rhs;
			pos[1] *= rhs;
			pos[2] *= rhs;

            return *this;
        }

        template <typename scalaType>
        vector3<valueType> & operator/=(const scalaType & rhs) {
			pos[0] /= rhs;
			pos[1] /= rhs;
			pos[2] /= rhs;

            return *this;
        }

        bool operator==(const vector3<valueType> & rhs) const {
            return (pos[0] == rhs.pos[0]) && (this->pos[1] == rhs.pos[1]) &&
                   (pos[2] == rhs.pos[2]);
        }

        auto norm() const -> decltype(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]) {
            return sqrt(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
        }
    };

    typedef vector3<double> v3;

    /*
         * namespace std {
     *     template <typename valueType>
     *     struct hash<vector3<valueType>>
     *         : public unary_function<const vector3<valueType> &,
     *                                      size_t> {
     *         inline size_t
     *         operator()(const vector3<valueType> & vec) const {
     *             size_t seed = 0;
     *             hash_combine(seed, vec.x);
     *             hash_combine(seed, vec.y);
     *             hash_combine(seed, vec.z);
         *
     *             return seed;
     *         }
     *     };
     * }
         */
}

#endif
