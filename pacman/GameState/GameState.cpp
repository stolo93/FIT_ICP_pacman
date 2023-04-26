#include "GameState.h"

#include <exception>
#include <utility>

namespace game
{
GameState GameState::update()
{
    throw std::logic_error("Not implemented");
}
GameState::GameState(QExplicitlySharedDataPointer<Map> map, uint64_t state_number, QVector<Ghost> ghosts, Player player,
                     Pos exit, QVector<Pos> keys)
    : map(std::move(map)), state_number(state_number), ghosts(std::move(ghosts)), player(player), exit(exit),
      keys(std::move(keys))
{
}
}    // namespace game