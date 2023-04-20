#include "Pos.h"
#include "test_helper_macros.h"

using namespace game;
int main()
{
    auto _a = FixedPointNum<std::int64_t, 32>(8);    // Should succeed
    auto _b = FixedPointNum<std::int64_t, 0>(INT64_MAX);
    auto _c = FixedPointNum<std::uint8_t, 2>(63);
    auto _d = FixedPointNum<std::int8_t, 2>(31);
    auto _e = FixedPointNum<std::int8_t, 2>(-32);


    ASSERT_THROWS_DOMAIN_ERROR((FixedPointNum<std::int8_t, 7>(9)))
    ASSERT_THROWS_DOMAIN_ERROR((FixedPointNum<std::int8_t, 2>(-33)))
    ASSERT_THROWS_DOMAIN_ERROR((FixedPointNum<std::uint8_t, 2>(64)))

    return 0;
}