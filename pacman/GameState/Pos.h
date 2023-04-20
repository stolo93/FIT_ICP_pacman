#ifndef PACMAN_POS_H
#define PACMAN_POS_H

#include <climits>
#include <compare>
#include <cstdint>
#include <numeric>

namespace game
{

template<typename Store, unsigned int binary_decimals>
    requires std::integral<Store>
class FixedPointNum final
{
    static_assert(binary_decimals <= sizeof(Store) * CHAR_BIT, "Not enough space in Store to fit decimal digits");

    Store store;

public:
    // Initializes a fixed point number with a zero
    constexpr FixedPointNum() = default;
    // Implicitly convert integers into FixedPointNumbers for ease of use
    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr FixedPointNum(Store value);

    static constexpr FixedPointNum<Store, binary_decimals> from_bits(Store bits);

    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> operator+(const FixedPointNum<Store, other_decimals> &rhs);
    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> operator-(const FixedPointNum<Store, other_decimals> &rhs);
    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> operator*(const FixedPointNum<Store, other_decimals> &rhs);
    constexpr FixedPointNum<Store, binary_decimals> operator/(const Store &rhs);

    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator+=(const FixedPointNum<Store, other_decimals> &rhs);
    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator-=(const FixedPointNum<Store, other_decimals> &rhs);
    template<unsigned int other_decimals>
    constexpr FixedPointNum<Store, binary_decimals> &operator*=(const FixedPointNum<Store, other_decimals> &rhs);
    constexpr FixedPointNum<Store, binary_decimals> &operator/=(const Store &other);
    constexpr std::strong_ordering operator<=>(const FixedPointNum<Store, binary_decimals> &other) const = default;

    constexpr FixedPointNum<Store, binary_decimals> integer() noexcept;
    constexpr FixedPointNum<Store, binary_decimals> frac() noexcept;

    constexpr Store ceil();
    constexpr Store floor() noexcept;
    constexpr Store round();


private:
    template<unsigned int other_decimals>
    static constexpr FixedPointNum<Store, binary_decimals>
    shift_towards_self(FixedPointNum<Store, other_decimals> value);
};

class Pos
{

public:
    typedef FixedPointNum<std::int64_t, 3> Number;

    FixedPointNum<std::int64_t, 3> x;
    FixedPointNum<std::int64_t, 3> y;

    constexpr Pos(game::FixedPointNum<std::int64_t, 3> x, game::FixedPointNum<std::int64_t, 3> y) noexcept;

    constexpr Pos operator+(const Pos &rhs);
    constexpr Pos operator-(const Pos &rhs);
    constexpr Pos operator*(const Number &rhs);

    constexpr Pos &operator+=(const Pos &rhs);
    constexpr Pos &operator-=(const Pos &rhs);
    constexpr Pos &operator*=(const Number &rhs);
};

}    // namespace game

#endif    // PACMAN_POS_H
