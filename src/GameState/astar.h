//
// Created by samuel on 7.5.2023.
//

#ifndef PACMAN_ASTAR_H
#define PACMAN_ASTAR_H

#include "Map.h"
#include "Pos.h"

#include <QExplicitlySharedDataPointer>
#include <vector>

namespace game
{

struct PathNode {
    Pos position;
    double g, h, f;

    PathNode();

    PathNode(Pos position, double g, double h);

    PathNode(const PathNode &other) noexcept;

    PathNode(const PathNode &&other) noexcept;

    bool operator==(const PathNode &other) const;

    PathNode &operator=(const PathNode &other);

    [[nodiscard]] std::vector<PathNode> generate_neighbours(const QExplicitlySharedDataPointer<Map> &map,
                                                            const Pos &goal) const;

    static bool is_node_valid(QExplicitlySharedDataPointer<Map> map, const PathNode &node);
};

class A_Star
{
public:
    static std::vector<Pos> a_star(const QExplicitlySharedDataPointer<Map> &map, const Pos &start, const Pos &goal);

    static std::vector<Pos> dumb_move(const QExplicitlySharedDataPointer<Map> &map, const Pos &start, const Pos &goal);

    static inline double euclidean_distance(const Pos &a, const Pos &b);

protected:
    static std::vector<Pos> reconstruct_path(const PathNode &start, const PathNode &goal,
                                             const std::unordered_map<PathNode, PathNode> &came_from);
};

}    // namespace game

#endif    // PACMAN_ASTAR_H
