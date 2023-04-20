#include "Pos.h"
#include "test_helper_macros.h"

using namespace game;
int main()
{
    auto a = game::FixedPointNum<std::int8_t, 2>(2);
    auto b = game::FixedPointNum<std::int8_t, 2>(4);
    auto c = game::FixedPointNum<std::uint8_t, 2>(32);

    ASSERT(a * a == 4)
    ASSERT(b * -2 == -8)

    ASSERT_THROWS_DOMAIN_ERROR(c * 10)
    return 0;
}