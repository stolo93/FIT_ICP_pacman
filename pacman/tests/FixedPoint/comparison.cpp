#include "Pos.h"
#include "test_helper_macros.h"

int main()
{

    auto a = game::FixedPointNum<std::int64_t, 8>(2);
    auto b = game::FixedPointNum<std::int64_t, 8>(2);
    auto c = game::FixedPointNum<std::int64_t, 8>(4);
    auto d = game::FixedPointNum<std::int64_t, 8>(1);

    ASSERT(a == b);
    ASSERT(b != c);

    ASSERT(a < c);
    ASSERT(a > d);

    return 0;
}