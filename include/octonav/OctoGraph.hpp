#pragma once
#include "a_star.hpp"
#include <octomap/octomap.h>
#include <octomap/OcTree.h>

inline octomap::OcTreeKey& makeKeyUnique(octomap::OcTreeKey &key, unsigned int level);

/**
 * @brief The data structure that describe the nodes of a OctoGraph.
 * 
 */
class OctoNode
{
public:
    /**
     * @brief Construct a new Octo Node object
     * 
     * @param key A unique key indentifying the node. Make sure you call makeKeyUnique.
     * @param depth The depth of the node. The tree's root is at depth zero.
     */
    OctoNode(octomap::OcTreeKey key, int depth) : key(key), depth(depth)
    {
        assert((key == makeKeyUnique(key, depth)));
    }
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
    using OctoGraph::OctoGraph;
    std::vector<OctoNode> neighbors(const OctoNode &node);
};

/**
 * @brief Takes advantage of octomap sparsity.
 * 
 */
class OctoGraphSparse : public OctoGraph
{
public:
    OctoGraphSparse(octomap::OcTree &octree) : OctoGraph(octree), graphGrid(octree){};
    std::vector<OctoNode> neighbors(const OctoNode& node);

private:
    OctoGraphGrid graphGrid;
};