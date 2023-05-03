#include "GameState.h"

#include <exception>
#include <utility>

namespace game
{
GameState::GameState(QExplicitlySharedDataPointer<Map> map, uint64_t state_number, QVector<Ghost> ghosts, Player player,
                     Pos exit, QVector<Pos> keys)
    : map(std::move(map)), state_number(state_number), ghosts(std::move(ghosts)), player(player), exit(exit),
      keys(std::move(keys))
{
}
GameState GameState::update(Pos direction)
{
    return {this->map, this->state_number + 1, this->ghosts, this->player, this->exit, this->keys};
}
}    // namespace game