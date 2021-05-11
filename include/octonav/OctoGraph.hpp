#pragma once
#include "a_star.hpp"
#include <octomap/octomap.h>
#include <octomap/OcTree.h>

octomap::OcTreeKey &makeKeyUnique(octomap::OcTreeKey &key, unsigned int depth);

/**
 * @brief The data structure that describe the nodes of a OctoGraph.
 * 
 */
class OctoNode : Node
{
public:
    /**
     * @brief Construct a new Octo Node object
     * 
     * @param key A unique key indentifying the node. Make sure you call makeKeyUnique.
     * @param depth The depth of the node. The tree's root is at depth zero.
     */
    OctoNode(octomap::OcTreeKey key, int depth) : key(key), Node(), depth(depth)
    {
        assert((key == makeKeyUnique(key, depth)));
    }

    bool operator==(const OctoNode& other) const {
        return key == other.key && depth == other.depth;
    }

    friend std::ostream& operator<<(std::ostream &out, OctoNode node) {
        out << "{Key: [" << node.key[0] << "," << node.key[1] << "," << node.key[2] << "], Depth: " << node.depth << "}";
        return out;
    }

    octomap::OcTreeKey key;
    int depth;
};

std::pair<octomap::OcTreeNode *, unsigned int> searchWithDepth(const octomap::OcTree &octree, const OctoNode &node);

class OctoGraph : public Graph<OctoNode>
{
public:
    OctoGraph(octomap::OcTree &octree) : octree(octree){};
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
    std::vector<OctoNode> neighbors(const OctoNode &node);

private:
    OctoGraphGrid graphGrid;
};

