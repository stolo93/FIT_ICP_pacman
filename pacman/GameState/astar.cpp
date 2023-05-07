//
// Created by samuel on 7.5.2023.
//

#include "astar.h"
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <unordered_set>

namespace std {
    template<>
    struct hash<game::PathNode> {
        size_t operator()(const game::PathNode &node) const {
            return (hash<float>()(node.position.x.to_floating_point<float>()) ^ hash<float>()(node.position.y.to_floating_point<float>()) << 3);
        }
    };
}

namespace game {
PathNode::PathNode(Pos position, double g, double h) : position(position), g(g), h(h), f(g+h) {}

PathNode::PathNode(const PathNode &other) noexcept : position(Pos(other.position.x, other.position.y)), g(other.g), h(other.h), f(other.f) {}

bool PathNode::operator==(const game::PathNode &other) const{
    return position.x == other.position.x && position.y == other.position.y;
}

PathNode& PathNode::operator=(const game::PathNode &other) {
    if (this == &other) {
        return *this;
    }
    position = Pos(other.position.x, other.position.y);
    g = other.g;
    h = other.h;
    f = other.f;
    return *this;
}

std::vector<PathNode> PathNode::generate_neighbours(const QExplicitlySharedDataPointer<Map> &map, const Pos& goal) const {
    // Define possible movements
    const static std::vector<Pos> DIRECTIONS{
        {0, -1}, // up
        {0, 1}, // down
        {-1, 0}, // left
        {1, 0} // right
    };

    std::vector<PathNode> neighbours {};
    const Pos original_position {this->position};

    for (const auto& direction : DIRECTIONS){
        Pos new_position {original_position.x + direction.x, original_position.y + direction.y};
        double new_h = A_Star::euclidean_distance(new_position, goal);
        double new_g = this->g + 1; // Cost of travel between tiles

        PathNode new_node {new_position, new_g, new_h};
        if (is_node_valid(map, new_node)){
            neighbours.emplace_back(new_node);
        }
    }

    return neighbours;
}

bool PathNode::is_node_valid(QExplicitlySharedDataPointer<game::Map> map, const game::PathNode &node) {
    if (node.position.x < 0 || node.position.y < 0 || node.position.x >= map->get_width() || node.position.y >= map->get_height()) {
        return false;
    }
    auto x = node.position.x.round();
    auto y = node.position.y.round();
    auto map_elem = map->get_map()[y][x];

    return  map_elem != MapElement::Wall;

}

std::vector<Pos> A_Star::a_star(const QExplicitlySharedDataPointer<Map>& map, const game::Pos &start, const game::Pos &goal) {
    auto node_compare =
            [](const PathNode &a, const PathNode &b) -> bool {
                if (a.f == b.f) { return a.h > b.h; }
                else { return a.f > b.f; }
            };

    std::priority_queue<PathNode, std::vector<PathNode>, decltype(node_compare)> open_set{node_compare};
    std::unordered_map<PathNode, PathNode> came_from;
    std::unordered_map<PathNode, double> g_costs {};
    std::unordered_set<PathNode> closed_set {};
    std::unordered_set<PathNode> open_nodes_set {};

    PathNode start_node {start, 0, euclidean_distance(start, goal)};
    open_set.emplace(start_node);
    g_costs[start_node] = 0.0;
    open_nodes_set.insert(start_node);

    while(! open_set.empty()){
        auto current = open_set.top();
        open_set.pop();
        open_nodes_set.erase(current);
        closed_set.insert(current);

        if (current.position.x == goal.x && current.position.y == goal.y){
            return reconstruct_path(start_node, current, came_from);
        }

        for (const auto& neighbour : current.generate_neighbours(map, goal)){
            if(closed_set.find(neighbour) != closed_set.end()){
                continue; // Neighbour already in the closed set
            }

            // If the node is not in the open set or if the cost is better than before
            if (open_nodes_set.find(neighbour) == open_nodes_set.end() || neighbour.g < g_costs[neighbour]){
                // TODO fix nex line
//                came_from[neighbour] = current;
                g_costs[neighbour] = neighbour.g;

                if (open_nodes_set.find(neighbour) == open_nodes_set.end()){
                    open_set.push(neighbour);
                    open_nodes_set.insert(neighbour);
                }

            }
        }
    }
    // In case there is no path
    return {};
}

double A_Star::euclidean_distance(const Pos &a, const Pos &b) {
    return sqrt(pow((b.x - a.x).to_floating_point<float>(), 2) + pow((b.y - a.y).to_floating_point<float>(), 2));
}

std::vector<Pos> A_Star::reconstruct_path(const PathNode& start, const PathNode& goal, const std::unordered_map<PathNode, PathNode>& came_from) {
    std::vector<Pos> path;
    PathNode current = goal;
    while (current.position.x != start.position.x && current.position.y != start.position.y) {
        path.push_back(current.position);
        current = came_from.at(current);
    }
    path.push_back(start.position);
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Pos> A_Star::dumb_move(const QExplicitlySharedDataPointer<Map>& map, const game::Pos &start, const game::Pos &goal){
    auto node_compare =
            [](const PathNode &a, const PathNode &b) -> bool {
                if (a.f == b.f) { return a.h > b.h; }
                else { return a.f > b.f; }
            };

    PathNode start_node {start, 0, euclidean_distance(start, goal)};
    auto neighbours = start_node.generate_neighbours(map, goal);

    if (neighbours.empty()){
        return {start,start};
    }

    PathNode min_dist = neighbours[0];
    for (int i = 1; i < neighbours.size(); ++i){
        if (! node_compare(neighbours[i], min_dist)){
            min_dist = neighbours[i];
        }
    }

    return {start, min_dist.position};
}
} // game
