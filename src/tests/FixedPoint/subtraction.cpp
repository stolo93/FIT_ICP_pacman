#include "Pos.h"
#include "test_helper_macros.h"

using namespace game;
int main()
{
    auto a = game::FixedPointNum<std::int8_t, 2>(2);
    auto b = game::FixedPointNum<std::int8_t, 2>(4);
    auto c = game::FixedPointNum<std::uint8_t, 2>(32);
    auto d = game::FixedPointNum<std::int8_t, 2>(-5);

    ASSERT(a - a == 0);
    ASSERT(b - 2 == a);
    ASSERT(a - d == 7)
    ASSERT(a - 3 == -1)

    ASSERT_THROWS_DOMAIN_ERROR(d - 31)
    ASSERT_THROWS_DOMAIN_ERROR(c - -1)

    return 0;
}