/**
* @file Pos.h
* @date 2023-04-26
* Project - ICP - project PacMan
 */

#ifndef PACMAN_POS_H
#define PACMAN_POS_H

#include <climits>
#include <compare>
#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>

namespace game::internal
{

/** Creates a mask with first *n* bits set.
* @tparam integral The integral type the mask is supposed to work on.
* @param n THe numbers of bits to set to 1.
* @return A *integral* with the first *n* bits set.
*/
 template<typename integral>
constexpr integral create_frac_mask(unsigned int n)
{
    if (n == 0) { return 0; }

    return (1 << n) - 1;
}

#ifdef OVERFLOW_CHECKS
template<typename T>
constexpr bool would_addition_overflow(T lhs, T rhs)
{
    return (rhs > 0 && lhs > std::numeric_limits<T>::max() - rhs) ||
           (rhs <= 0 && lhs < std::numeric_limits<T>::min() - rhs);
}

template<typename T>
constexpr bool would_subtraction_overflow(T lhs, T rhs)
{
    return (rhs <= 0 && lhs > std::numeric_limits<T>::max() + rhs) ||
           (rhs > 0 && lhs < std::numeric_limits<T>::min() + rhs);
}

template<typename T>
constexpr bool would_multiplication_overflow(T lhs, T rhs)
{
    if (std::numeric_limits<T>::is_signed) {
        if (lhs == -1 && rhs == std::numeric_limits<T>::min()) { return true; }
        if (rhs == -1 && lhs == std::numeric_limits<T>::min()) { return true; }
    }

    return (lhs != 0 && rhs > std::numeric_limits<T>::max() / lhs) ||
           (lhs != 0 && rhs < std::numeric_limits<T>::min() / lhs);
}
#endif

}    // namespace game::internal

namespace game
{

/**
 * A fixed point integer.
 * @tparam Store The integer type to store the fixed point number in (defines the signedness and byte size of the integer).
 * @tparam binary_decimals The amount of bits to use for the decimal part of the fixed point integer
 */
template<typename Store, unsigned int binary_decimals>
class FixedPointNum final
{
    static_assert(binary_decimals <= sizeof(Store) * CHAR_BIT, "Not enough space in Store to fit decimal digits");

    Store store;

public:
    /**
     * The amount of bits left for the integer part of the integer
     */
    static constexpr unsigned int IntegerBits =
            (sizeof(Store) * CHAR_BIT - binary_decimals) - std::numeric_limits<Store>::is_signed;

    /**
     * Initializes a fixed point number with a zero.
     */
    constexpr FixedPointNum()
    {
        this->store = 0;
    };

    /**
     * Constructs a fixed point with the given integer value.
     * @param value
     */
    // Implicitly convert integers into FixedPointNumbers for ease of use
    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr FixedPointNum(Store value)
    {
#ifdef OVERFLOW_CHECKS
        if (binary_decimals != 0) {
            if (value > ((1ll << IntegerBits) - 1)) {
                throw std::domain_error("Number to large to fit into fixed point number");
            }

            if (std::numeric_limits<Store>::is_signed && value < -(1ll << IntegerBits)) {
                throw std::domain_error("Number to small to fit into fixed point number");
            }
        }
#endif

        store = value << binary_decimals;
    };

    /**
     * Converts a binary decimal to a different amount of binary decimals.
     * @tparam other_decimals The amount of decimals the source fixed point number is made of.
     * @param other The fixed point number we're changing.
     */
    // Constructor for changing fixed point number precision
    template<unsigned int other_decimals>
    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr FixedPointNum(FixedPointNum<Store, other_decimals> other)
    {
        *this = shift_towards_self(other);
    }

    /**
     * Creates a fixed point decimal from a float.
     * @param num The double to parse
     */
    static constexpr FixedPointNum<Store, binary_decimals> from_float(double num)
    {

        // Multiply by scaling factor to move the decimals that will get saved in the FixedPointNum into the whole part
        num *= (double) (1 << binary_decimals);


        // Restore the saved decimals
        return FixedPointNum(num) / (1 << binary_decimals);
    }

    /**
     * Create a new fixed point number with the given underlying bits.
     * @param bits
     */
    static constexpr FixedPointNum<Store, binary_decimals> from_bits(Store bits)
    {
        FixedPointNum<Store, binary_decimals> new_num = FixedPointNum();
        new_num.store = bits;
        return new_num;
    };

    /**
     * Gets the bits underlying data the fixed point number is made of.
     */
    [[nodiscard]] constexpr Store to_bits() const
    {
        return this->store;
    }

    /**
     * Converts the fixed point number to a floating point type.
     * @tparam Float The floating point type to convert to (Will compile if given integers but will loose precision)
     */
    template<class Float>
    constexpr Float to_floating_point() const
    {
        auto scaled_value = static_cast<Float>(this->to_bits());
        auto scale = static_cast<Float>(1 << binary_decimals);
        return scaled_value / scale;
    }

    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> operator+(const FixedPointNum<Store, other_decimals> &rhs) const
    {
        FixedPointNum<Store, binary_decimals> value = shift_towards_self(rhs);

#ifdef OVERFLOW_CHECKS
        if (game::internal::would_addition_overflow(store, value.store)) {
            throw std::domain_error("Addition of values would overflow");
        }
#endif

        return from_bits(store + value.store);
    };

    constexpr FixedPointNum<Store, binary_decimals> operator+(const Store rhs)
    {
        return *this + FixedPointNum(rhs);
    }


    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> operator-(const FixedPointNum<Store, other_decimals> &rhs) const
    {
        FixedPointNum<Store, binary_decimals> value = shift_towards_self(rhs);

#ifdef OVERFLOW_CHECKS
        if (game::internal::would_subtraction_overflow(store, value.store)) {
            throw std::domain_error("Subtraction of values would overflow");
        }
#endif

        return from_bits(store - value.store);
    }

    constexpr FixedPointNum<Store, binary_decimals> operator-(const Store rhs) const
    {
        return *this - FixedPointNum(rhs);
    }

    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> operator*(const FixedPointNum<Store, other_decimals> &rhs) const
    {

        auto large_value = store * rhs.to_bits();

        // We may lose precision with this cast, but we lose precision so often wo consider it to be ok
        return from_bits(large_value >> other_decimals);
    }

    constexpr FixedPointNum<Store, binary_decimals> operator*(const Store rhs) const
    {
        return *this * FixedPointNum(rhs);
    }

    /**
     * Returns the fractional part of the fixed point number
     * @returns The fraction.
     */
    [[nodiscard]] constexpr FixedPointNum<Store, binary_decimals> frac() const noexcept
    {
        auto mask = game::internal::create_frac_mask<Store>(binary_decimals);

        return from_bits(store & mask);
    }

    /**
     * Returns the integer part of the fixed point number
     * @returns An integer.
     */
    [[nodiscard]] constexpr FixedPointNum<Store, binary_decimals> integer() const noexcept
    {
        auto mask = game::internal::create_frac_mask<Store>(binary_decimals);

        return from_bits(store & ! mask);
    }


    [[nodiscard]] constexpr Store ceil() const
    {
        if (this->frac() == 0) {
            return this->store >> binary_decimals;
        } else {
            return (this->store >> binary_decimals) + 1;
        }
    }


    [[nodiscard]] constexpr Store floor() const noexcept
    {
        return store >> binary_decimals;
    }


    [[nodiscard]] constexpr Store round() const
    {
        auto mask = game::internal::create_frac_mask<Store>(binary_decimals);

        if (this->frac().store > mask / 2) {
            return this->ceil();
        } else {
            return this->floor();
        }
    }

    [[nodiscard]] constexpr FixedPointNum<Store, binary_decimals> abs() const
    {
        return FixedPointNum::from_bits(std::abs(this->store));
    }


    constexpr FixedPointNum<Store, binary_decimals> operator/(const Store &rhs) const
    {
        return FixedPointNum<Store, binary_decimals>::from_bits(store / rhs);
    }


    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator+=(const FixedPointNum<Store, other_decimals> &rhs)
    {
        *this = *this + rhs;
        return *this;
    }


    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator-=(const FixedPointNum<Store, other_decimals> &rhs)
    {
        *this = *this - rhs;
        return *this;
    }


    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator*=(const FixedPointNum<Store, other_decimals> &rhs)
    {
        *this = *this * rhs;
        return *this;
    }


    constexpr FixedPointNum<Store, binary_decimals> &operator/=(const Store &rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    constexpr bool operator<(const FixedPointNum<Store, binary_decimals> &other) const
    {
        return this->store < other.store;
    }

    constexpr bool operator>(const FixedPointNum<Store, binary_decimals> &other) const
    {
        return this->store > other.store;
    }

    constexpr bool operator<=(const FixedPointNum<Store, binary_decimals> &other) const
    {
        return this->store <= other.store;
    }

    constexpr bool operator>=(const FixedPointNum<Store, binary_decimals> &other) const
    {
        return this->store >= other.store;
    }

    constexpr bool operator==(const FixedPointNum<Store, binary_decimals> &other) const
    {
        return this->store == other.store;
    }

    constexpr bool operator!=(const FixedPointNum<Store, binary_decimals> &other) const
    {
        return this->store != other.store;
    }

private:
    template<unsigned int other_decimals>
    [[nodiscard]] constexpr FixedPointNum<Store, binary_decimals>
    shift_towards_self(const FixedPointNum<Store, other_decimals> value) const
    {
        if (binary_decimals == other_decimals) { return value; }

        if (binary_decimals > other_decimals) {
#ifdef OVERFLOW_CHECKS
            if (std::abs(value.floor()) > ((1ull << binary_decimals) - 1)) {
                throw std::domain_error("Number to large to fit into fixed point number");
            }

            if (std::numeric_limits<Store>::is_signed && value.floor() < -(1ll << binary_decimals)) {
                throw std::domain_error("Number to small to fit into fixed point number");
            }
#endif
            return FixedPointNum::from_bits(value.to_bits() << (binary_decimals - other_decimals));
        }

        if (binary_decimals < other_decimals) {
            if (binary_decimals == other_decimals) { throw std::logic_error("They're equal"); }
            // This conversion will almost always loose precision.
            // Therefore, we don't guard against such scenarios
            return FixedPointNum::from_bits(value.to_bits() >> (other_decimals - binary_decimals));
        }

        return value;
    }
};

/**
 * A 2d vector
 */
class Pos
{

public:
    typedef FixedPointNum<std::int64_t, 3> Number;

    FixedPointNum<std::int64_t, 3> x;
    FixedPointNum<std::int64_t, 3> y;

    /**
     * Zero initializes the position
     */
    constexpr Pos() noexcept : x(0), y(0) {};
    constexpr Pos(game::FixedPointNum<int64_t, 3> x, game::FixedPointNum<int64_t, 3> y) noexcept : x(x), y(y) {}

    [[nodiscard]] constexpr Pos floor() const
    {
        return {this->x.floor(), this->y.floor()};
    }

    [[nodiscard]] constexpr Pos ceil() const
    {
        return {this->x.ceil(), this->y.ceil()};
    }

    [[nodiscard]] constexpr Pos round() const
    {
        return {this->x.round(), this->y.round()};
    }

    constexpr game::Pos operator+(const game::Pos &rhs) const
    {
        return {x + rhs.x, y + rhs.y};
    }
    constexpr game::Pos operator-(const game::Pos &rhs) const
    {
        return {x - rhs.x, y - rhs.y};
    }
    constexpr game::Pos operator*(const game::FixedPointNum<int64_t, 3> &rhs) const
    {
        return {x * rhs, y * rhs};
    }

    constexpr game::Pos &operator+=(const game::Pos &rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    constexpr game::Pos &operator-=(const game::Pos &rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    constexpr game::Pos &operator*=(const Number &rhs)
    {
        *this = *this * rhs;
        return *this;
    }
};

}    // namespace game

#endif    // PACMAN_POS_H
