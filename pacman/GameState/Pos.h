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

/// Creates a mask with first *n* bits set
/// \tparam integral The integral type the mask is supposed to work on
/// \param n THe numbers of bits to set to 1
/// \return A *integral* with the first *n* bits set
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

template<typename Store, unsigned int binary_decimals>
class FixedPointNum final
{
    static_assert(binary_decimals <= sizeof(Store) * CHAR_BIT, "Not enough space in Store to fit decimal digits");

    Store store;

public:
    static constexpr unsigned int IntegerBits =
            (sizeof(Store) * CHAR_BIT - binary_decimals) - std::numeric_limits<Store>::is_signed;

    // Initializes a fixed point number with a zero
    constexpr FixedPointNum() = default;

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

    static constexpr FixedPointNum<Store, binary_decimals> from_bits(Store bits)
    {
        FixedPointNum<Store, binary_decimals> new_num = FixedPointNum();
        new_num.store = bits;
        return new_num;
    };

    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> operator+(const FixedPointNum<Store, other_decimals> &rhs)
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
    constexpr FixedPointNum<Store, binary_decimals> operator-(const FixedPointNum<Store, other_decimals> &rhs)
    {
        FixedPointNum<Store, binary_decimals> value = shift_towards_self(rhs);

#ifdef OVERFLOW_CHECKS
        if (game::internal::would_subtraction_overflow(store, value.store)) {
            throw std::domain_error("Subtraction of values would overflow");
        }
#endif

        return from_bits(store - value.store);
    }

    constexpr FixedPointNum<Store, binary_decimals> operator-(const Store rhs)
    {
        return *this - FixedPointNum(rhs);
    }

    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> operator*(const FixedPointNum<Store, other_decimals> &rhs)
    {
        static_assert(sizeof(Store) * CHAR_BIT > binary_decimals * other_decimals,
                      "Multiplication of values would loose all data");
#ifdef OVERFLOW_CHECKS
        if (game::internal::would_multiplication_overflow(store, rhs.store)) {
            throw std::domain_error("Multiplication of values would overflow");
        }
#endif
        auto large_value = store * rhs.store;

        // We may lose precision with this cast, but we lose precision so often wo consider it to be ok
        return from_bits(large_value >> ((binary_decimals * other_decimals) - binary_decimals));
    }

    constexpr FixedPointNum<Store, binary_decimals> operator*(const Store rhs)
    {
        return *this * FixedPointNum(rhs);
    }

    constexpr FixedPointNum<Store, binary_decimals> frac() noexcept
    {
        auto mask = game::internal::create_frac_mask<Store>(binary_decimals);

        return from_bits(store & mask);
    }


    constexpr FixedPointNum<Store, binary_decimals> integer() noexcept
    {
        auto mask = game::internal::create_frac_mask<Store>(binary_decimals);

        return from_bits(store & ! mask);
    }


    [[nodiscard]] constexpr Store ceil() const
    {
        if (this->frac() == 0) {
            return this->store >> binary_decimals;
        } else {
            return (this->integer() + 1).store >> binary_decimals;
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


    constexpr FixedPointNum<Store, binary_decimals> operator/(const Store &rhs) const
    {
        return FixedPointNum<Store, binary_decimals>(store / rhs);
    }


    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator+=(const FixedPointNum<Store, other_decimals> &rhs) const
    {
        *this = *this + rhs;
        return *this;
    }


    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator-=(const FixedPointNum<Store, other_decimals> &rhs) const
    {
        *this = *this - rhs;
        return *this;
    }


    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator*=(const FixedPointNum<Store, other_decimals> &rhs) const
    {
        *this = *this * rhs;
        return *this;
    }


    constexpr FixedPointNum<Store, binary_decimals> &operator/=(const Store &rhs) const
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
    constexpr FixedPointNum<Store, binary_decimals> shift_towards_self(const FixedPointNum<Store, other_decimals> value)
    {
        if (binary_decimals > other_decimals) {
#ifdef OVERFLOW_CHECKS
            if (value.floor() > ((1ull << binary_decimals) - 1)) {
                throw std::domain_error("Number to large to fit into fixed point number");
            }

            if (std::numeric_limits<Store>::is_signed && value.floor() < -(1ull << binary_decimals)) {
                throw std::domain_error("Number to small to fit into fixed point number");
            }
#endif
            return FixedPointNum(value.store << (binary_decimals - other_decimals));
        } else if (binary_decimals < other_decimals) {
            // This conversion will almost always loose precision.
            // Therefore, we don't guard against such scenarios
            return FixedPointNum(value.store >> (other_decimals - binary_decimals));
        }

        return value;
    }
};

class Pos
{

public:
    typedef FixedPointNum<std::int64_t, 3> Number;

    FixedPointNum<std::int64_t, 3> x;
    FixedPointNum<std::int64_t, 3> y;

    constexpr Pos(game::FixedPointNum<int64_t, 3> x, game::FixedPointNum<int64_t, 3> y) noexcept : x(x), y(y) {}
    constexpr game::Pos operator+(const game::Pos &rhs)
    {
        return {x + rhs.x, y + rhs.y};
    }
    constexpr game::Pos operator-(const game::Pos &rhs)
    {
        return {x - rhs.x, y - rhs.y};
    }
    constexpr game::Pos operator*(const game::FixedPointNum<int64_t, 3> &rhs)
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
