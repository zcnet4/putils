#pragma once

#include <cmath>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <vector>

#include "sign.hpp"
#include "Point.hpp"
#include "Direction.hpp"
#include "runTests.hpp"

namespace putils
{
    /*
     * Pathfinding algorithm implementing the AStar algorithm described on Wikipedia
     */

    class AStar
    {
        // Get the next move towards goal
    public:
        template<typename Precision>
        static std::vector<putils::Point<Precision>>
        getNextDirection(const Point<Precision>& start, const Point<Precision>& goal, bool diagonals,
                         Precision step, Precision desiredDistance,
                         const std::function<
                                 bool(const Point<Precision>& from, const Point<Precision>& to)>& canMoveTo) noexcept;
    };

    /*
     * Implementation details
     */

    namespace
    {
        template<typename Precision>
        double heuristic_cost_estimate(const putils::Point<Precision>& start,
                                       const putils::Point<Precision>& goal) noexcept
        {
            return std::sqrt(
                    std::pow(goal.x - start.x, 2) +
                    std::pow(goal.y - start.y, 2)
            );
        }

        template<typename Precision>
        std::vector<putils::Point<Precision>>
        reconstruct_path(const std::unordered_map<Point<Precision>, Point<Precision>>& cameFrom,
                         putils::Point<Precision>& current, const putils::Point<Precision>& pos) noexcept
        {
            std::vector<putils::Point<Precision>> res;
            if (!cameFrom.size())
                return res;

            while (cameFrom.at(current) != pos)
            {
                res.emplace(res.begin(), current);
                current = cameFrom.at(current);
            }

            res.emplace(res.begin(), current);
            return res;
        }
    }

    template<typename Precision>
    std::vector<putils::Point<Precision>>
    AStar::getNextDirection(const Point<Precision>& start, const Point<Precision>& goal, bool diagonals,
                            Precision step, Precision desiredDistance,
                            const std::function<
                                    bool(const Point<Precision>& from, const Point<Precision>& to)>& canMoveTo) noexcept
    {
        // The set of nodes already evaluated.
        std::vector<Point<Precision>> closedSet;
        // The set of currently discovered nodes still to be evaluated.
        // Initially, only the start node is known.
        std::vector<Point<Precision>> openSet({start});
        // For each node, which node it can most efficiently be reached from.
        // If a node can be reached from many nodes, cameFrom will eventually contain the
        // most efficient previous step.
        std::unordered_map<Point<Precision>, Point<Precision>> cameFrom;

        // For each node, the cost of getting from the start node to that node.
        std::unordered_map<Point<Precision>, double> gScore;
        // The cost of going from start to start is zero.
        gScore.emplace(start, 0);

        // For each node, the total cost of getting from the start node to the goal
        // by passing by that node. That value is partly known, partly heuristic.
        std::unordered_map<Point<Precision>, double> fScore;
        // For the first node, that value is completely heuristic.
        fScore.emplace(start, heuristic_cost_estimate(start, goal));

        const auto findClosest = [&fScore](const auto& l, const auto& r)
        { return fScore.at(l) < fScore.at(r); };

        while (openSet.size())
        {
            const auto it = std::min_element(openSet.cbegin(), openSet.cend(), findClosest);
            auto current = *it;

            if (goal.distanceTo(current) < desiredDistance)
                return reconstruct_path(cameFrom, current, start);

            openSet.erase(it);
            closedSet.push_back(current);

            Point<Precision> neighbor;
            for (Precision x = current.x - step; x <= current.x + step; x += step)
                for (Precision y = current.y - step; y <= current.y + step; y += step)
                {
                    if (!diagonals &&
                        ((x == current.x && y == current.y) ||
                         (x != current.x && y != current.y)))
                        continue;
                    else if (diagonals && x == current.x && y == current.y)
                        continue;

                    neighbor = {x, y};

                    if (std::find(closedSet.cbegin(), closedSet.cend(), neighbor) != closedSet.cend())
                        continue; // Ignore the neighbor which is already evaluated.

                    if (goal.distanceTo(neighbor) >= desiredDistance && !canMoveTo(current, neighbor))
                        continue;

                    // The distance from start to a neighbor
                    auto tentative_gScore = gScore.at(current) + step;
                    if (std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end())
                        openSet.push_back(neighbor);
                    else if (tentative_gScore >= gScore[neighbor])
                        continue;                                // This is not a better path

                    // This path is the best until now. Record it!
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = tentative_gScore + heuristic_cost_estimate(neighbor, goal);
                }
        }

        return { };
    }

    namespace test
    {
        inline bool astar()
        {
            return putils::runTests(
                    "First step", []
                    {
                        const auto steps = putils::AStar::getNextDirection<int>(
                                putils::Point2i{0, 0}, putils::Point2i{5, 0}, true, 1, 1,
                                [](const putils::Point2i&, const putils::Point2i&)
                                { return true; }
                        );
                        return steps[0] == putils::Point2i{1, 0};
                    },
                    "Straight line", []
                    {
                        const auto steps = putils::AStar::getNextDirection<int>(
                                putils::Point2i{0, 0}, putils::Point2i{3, 0}, true, 1, 1,
                                [](const putils::Point2i&, const putils::Point2i&)
                                { return true; }
                        );
                        return steps.size() == 3 &&
                               steps[0] == putils::Point2i{1, 0} &&
                               steps[1] == putils::Point2i{2, 0} &&
                               steps[2] == putils::Point2i{3, 0};
                    },
                    "Obstacle", []
                    {
                        const auto steps = putils::AStar::getNextDirection<int>(
                                putils::Point2i{0, 0}, putils::Point2i{3, 0}, true, 1, 1,
                                [](const putils::Point2i& from, const putils::Point2i& to)
                                { return to != putils::Point2i{2, 0}; }
                        );
                        return steps.size() == 3 &&
                               steps[0] == putils::Point2i{1, 0} &&
                               (steps[1] == putils::Point2i{2, -1} || steps[1] == putils::Point2i{2, 1}) &&
                               steps[2] == putils::Point2i{3, 0};
                    }
            );
        }
    };
}
