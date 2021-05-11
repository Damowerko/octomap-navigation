#pragma once
#include "OctoGraph.hpp"
#include <octomap/octomap.h>
#include <octomap/OcTree.h>

/**
 * @brief The data structure that describe the nodes of a OctoGraph.
 * 
 */
class Visualizer
{
public:
    /**
     * @brief Construct a new Visualizer object
     * 
     * @param key A unique key indentifying the node. Make sure you call makeKeyUnique.
     * @param depth The depth of the node. The tree's root is at depth zero.
     */
    Visualizer(OctoGraph &graph) : graph(graph){};

private:
    OctoGraph &graph;
};
