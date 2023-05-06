#include "Pos.h"
#include "test_helper_macros.h"

using namespace game;
int main()
{
    auto a = game::FixedPointNum<std::int8_t, 2>(2);
    auto b = game::FixedPointNum<std::int8_t, 2>(4);
    auto c = game::FixedPointNum<std::uint8_t, 2>(32);
    auto zero = game::FixedPointNum<std::int8_t, 2>();
    auto one = game::FixedPointNum<std::int8_t, 2>(1);

    ASSERT(a * a == 4)
    ASSERT(b * -2 == -8)
    ASSERT(a * zero == zero)
    ASSERT(a * one == a)

    // Differing sizes
    auto five = game::FixedPointNum<std::int64_t, 16>(5);
    auto two = game::FixedPointNum<std::int64_t, 7>(2);

    ASSERT(five * two == 10)
    ASSERT(two * five == 10);

    return 0;
}