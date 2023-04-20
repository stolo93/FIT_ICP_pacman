#include "Pos.h"

#include <stdexcept>

#define OVERFLOW_CHECKS

template<typename T>
    requires std::integral<T>
constexpr bool would_left_shift_loose_bits(T value, unsigned int shift)
{
    return value > std::numeric_limits<T>::max() >> shift;
}

template<typename T>
    requires std::integral<T>
constexpr bool would_right_shift_loose_bits(T value, unsigned int shift)
{
    return value < std::numeric_limits<T>::max() >> shift;
}

template<typename T>
    requires std::integral<T>
constexpr bool would_addition_overflow(T lhs, T rhs)
{
    return (rhs > 0 && lhs > std::numeric_limits<T>::max() - rhs) ||
           (rhs <= 0 && lhs < std::numeric_limits<T>::min() - rhs);
}

template<typename T>
    requires std::integral<T>
constexpr bool would_subtraction_overflow(T lhs, T rhs)
{
    return (rhs <= 0 && lhs > std::numeric_limits<T>::max() + rhs) ||
           (rhs > 0 && lhs < std::numeric_limits<T>::min() + rhs);
}

template<typename T>
    requires std::integral<T>
constexpr bool would_multiplication_overflow(T lhs, T rhs)
{
    if (std::numeric_limits<T>::is_signed) {
        if (lhs == -1 && rhs == std::numeric_limits<T>::min()) { return true; }
        if (rhs == -1 && lhs == std::numeric_limits<T>::min()) { return true; }
    }

    return (lhs != 0 && rhs > std::numeric_limits<T>::max() / lhs) ||
           (lhs != 0 && rhs < std::numeric_limits<T>::min() / lhs);
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr game::FixedPointNum<Store, binary_decimals>::FixedPointNum(Store value)
{
#ifdef OVERFLOW_CHECKS
    if (would_left_shift_loose_bits(store, binary_decimals)) {
        throw std::domain_error("Number to large to fit into fixed point number");
    }
#endif

    store = value << binary_decimals;
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr game::FixedPointNum<Store, binary_decimals> game::FixedPointNum<Store, binary_decimals>::from_bits(Store bits)
{
    FixedPointNum<Store, binary_decimals> new_num = FixedPointNum();
    new_num.store = bits;
    return new_num;
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
template<unsigned int other_decimals>
constexpr game::FixedPointNum<Store, binary_decimals>
game::FixedPointNum<Store, binary_decimals>::operator+(const FixedPointNum<Store, other_decimals> &rhs)
{
    FixedPointNum<Store, binary_decimals> value = shift_towards_self(rhs);

#ifdef OVERFLOW_CHECKS
    if (would_addition_overflow(store, value.store)) { throw std::domain_error("Addition of values would overflow"); }
#endif

    return FixedPointNum<Store, binary_decimals>(store + value.store);
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
template<unsigned int other_decimals>
constexpr game::FixedPointNum<Store, binary_decimals>
game::FixedPointNum<Store, binary_decimals>::operator-(const FixedPointNum<Store, other_decimals> &rhs)
{
    FixedPointNum<Store, binary_decimals> value = shift_towards_self(rhs);

#ifdef OVERFLOW_CHECKS
    if (would_subtraction_overflow(store, value.store)) {
        throw std::domain_error("Subtraction of values would overflow");
    }
#endif

    return FixedPointNum<Store, binary_decimals>(store - value.store);
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
template<unsigned int other_decimals>
constexpr game::FixedPointNum<Store, binary_decimals>
game::FixedPointNum<Store, binary_decimals>::operator*(const FixedPointNum<Store, other_decimals> &rhs)
{
    static_assert(sizeof(Store) * CHAR_BIT > binary_decimals * other_decimals,
                  "Multiplication of values would loose all data");
#ifdef OVERFLOW_CHECKS
    if (would_multiplication_overflow(store, rhs.store)) {
        throw std::domain_error("Multiplication of values would overflow");
    }
#endif
    auto large_value = store * rhs.store;

    // We may lose precision with this cast, but we lose precision so often wo consider it to be ok
    return FixedPointNum<Store, binary_decimals>(large_value >> ((binary_decimals * other_decimals) - binary_decimals));
}

/// Creates a mask with first *n* bits set
/// \tparam integral The integral type the mask is supposed to work on
/// \param n THe numbers of bits to set to 1
/// \return A *integral* with the first *n* bits set
template<typename integral>
    requires std::integral<integral>
consteval integral create_frac_mask(unsigned int n)
{
    if (n == 0) { return 0; }

    return (1 << n) - 1;
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr game::FixedPointNum<Store, binary_decimals> game::FixedPointNum<Store, binary_decimals>::frac() noexcept
{
    auto mask = create_frac_mask<Store>(binary_decimals);

    return from_bits(store & mask);
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr game::FixedPointNum<Store, binary_decimals> game::FixedPointNum<Store, binary_decimals>::integer() noexcept
{
    auto mask = create_frac_mask<Store>(binary_decimals);

    return from_bits(store & ! mask);
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr Store game::FixedPointNum<Store, binary_decimals>::ceil()
{
    if (this->frac() == 0) {
        return this->store >> binary_decimals;
    } else {
        return (this->integer() + 1).store >> binary_decimals;
    }
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr Store game::FixedPointNum<Store, binary_decimals>::floor() noexcept
{
    return store >> binary_decimals;
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr Store game::FixedPointNum<Store, binary_decimals>::round()
{
    auto mask = create_frac_mask<Store>(binary_decimals);

    if (this->frac().store > mask / 2) {
        return this->ceil();
    } else {
        return this->floor();
    }
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr game::FixedPointNum<Store, binary_decimals>
game::FixedPointNum<Store, binary_decimals>::operator/(const Store &rhs)
{
    return FixedPointNum<Store, binary_decimals>(store / rhs);
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
template<unsigned int other_decimals>
constexpr game::FixedPointNum<Store, binary_decimals> &
game::FixedPointNum<Store, binary_decimals>::operator+=(const FixedPointNum<Store, other_decimals> &rhs)
{
    *this = *this + rhs;
    return *this;
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
template<unsigned int other_decimals>
constexpr game::FixedPointNum<Store, binary_decimals> &
game::FixedPointNum<Store, binary_decimals>::operator-=(const FixedPointNum<Store, other_decimals> &rhs)
{
    *this = *this - rhs;
    return *this;
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
template<unsigned int other_decimals>
constexpr game::FixedPointNum<Store, binary_decimals> &
game::FixedPointNum<Store, binary_decimals>::operator*=(const FixedPointNum<Store, other_decimals> &rhs)
{
    *this = *this * rhs;
    return *this;
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
constexpr game::FixedPointNum<Store, binary_decimals> &
game::FixedPointNum<Store, binary_decimals>::operator/=(const Store &rhs)
{
    *this = *this / rhs;
    return *this;
}

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
template<unsigned int other_decimals>
constexpr game::FixedPointNum<Store, binary_decimals>
game::FixedPointNum<Store, binary_decimals>::shift_towards_self(const FixedPointNum<Store, other_decimals> value)
{
    if (binary_decimals > other_decimals) {
#ifdef OVERFLOW_CHECKS
        if (would_left_shift_loose_bits(value.store, binary_decimals - other_decimals)) {
            throw std::domain_error("Converting between fixed point values would overflow");
        }
#endif
        return FixedPointNum(value.store << (binary_decimals - other_decimals));
    } else if (binary_decimals < other_decimals) {
        // This conversion will almost always loose precision.
        // Therefore, we don't guard against such scenarios
        return FixedPointNum(value.store >> (other_decimals - binary_decimals));
    }

    return FixedPointNum(value.store);
}

constexpr game::Pos::Pos(game::FixedPointNum<int64_t, 3> x, game::FixedPointNum<int64_t, 3> y) noexcept : x(x), y(y) {}
constexpr game::Pos game::Pos::operator+(const game::Pos &rhs)
{
    return {x + rhs.x, y + rhs.y};
}
constexpr game::Pos game::Pos::operator-(const game::Pos &rhs)
{
    return {x - rhs.x, y - rhs.y};
}
constexpr game::Pos game::Pos::operator*(const game::FixedPointNum<int64_t, 3> &rhs)
{
    return {x * rhs, y * rhs};
}

constexpr game::Pos &game::Pos::operator+=(const game::Pos &rhs)
{
    *this = *this + rhs;
    return *this;
}

constexpr game::Pos &game::Pos::operator-=(const game::Pos &rhs)
{
    *this = *this - rhs;
    return *this;
}

constexpr game::Pos &game::Pos::operator*=(const Number &rhs)
{
    *this = *this * rhs;
    return *this;
}
