#pragma once
#include "a_star.hpp"
#include "GridGraph.hpp"
#include <octomap/octomap.h>
#include <octomap/OcTree.h>

/**
 * @brief The data structure that describe the nodes of a OctoGraph.
 * 
 */
struct OctoNode
{
    octomap::OcTreeKey key;
    int depth;
};

class OctoGraph : public Graph<OctoNode>
{
public:
    OctoGraph(octomap::OcTree &octree) : octree(octree){};

protected:
    octomap::OcTree &octree;
};

/**
 * @brief Treats the octomap as a 3D grid that can be indexed. 
 * 
 */
class OctoGraphGrid : public OctoGraph
{
public:
    std::vector<OctoNode> neighbors(OctoNode node);
};

/**
 * @brief Takes advantage of octomap sparsity.
 * 
 */
class OctoGraphSparse : public OctoGraph
{
public:
    std::vector<OctoNode> neighbors(OctoNode node);
};