#include "Pos.h"
#include "test_helper_macros.h"

using namespace game;
int main()
{
    auto a = FixedPointNum<std::int8_t, 2>(2);
    auto b = FixedPointNum<std::int8_t, 2>(4);
    auto c = FixedPointNum<std::uint8_t, 2>(32);
    auto d = FixedPointNum<std::int8_t, 2>(-31);

    ASSERT(a + a == b);
    ASSERT(a + 2 == b);
    ASSERT(a + -2 == 0)

    ASSERT_THROWS_DOMAIN_ERROR(c + c)
    ASSERT_THROWS_DOMAIN_ERROR(d + -2)


    return 0;
}