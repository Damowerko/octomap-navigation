#pragma once
#include "OctoGraph.hpp"
#include <vector>
#include <octomap/octomap.h>
#include <octomap/OcTree.h>

/**
 * @brief The data structure that describe the nodes of a OctoGraph.
 * 
 */

static std::vector<OctoNode> EMPTY_VEC = std::vector<OctoNode>();
// std::vector<int> EMPTY_VEC = std::vector<int>();
class Visualizer
{
public:
    /**
     * @brief Construct a new Visualizer object
     * 
     * @param graph OctoGraph
     * @param path List of OctoNodes
     */
    void visualize(const OctoGraph &graph, const std::vector<OctoNode> &path = EMPTY_VEC);
};
