#include "octonav/OctoGraph.hpp"
#include <optional>

using namespace std;
using namespace octomap;

// template class OctoGraph<OctoNode>;

inline bool shiftKey(OcTreeKey &key, unsigned char dir);
inline void changeDepth(OcTreeKey &key, int diff);
tuple<OcTreeNode *, OcTreeKey, unsigned int> getNeighborSameOrHigher(const OcTree &octree, const OctoNode &node, unsigned char dir, bool keepDepth = false);
pair<OcTreeNode *, unsigned int> searchWithDepth(const OcTree &octree, const OcTreeKey &key, unsigned int depth);

// Computes h(current)
template <>
float heuristic(shared_ptr<OctoNode> current, shared_ptr<OctoNode> goal)
{
    float sum = 0;
    for (int i = 0; i < 3; i++)
    {
        sum += pow(current->key[i] - goal->key[i], 2);
    }
    return sum;
}

vector<shared_ptr<OctoNode>> OctoGraphGrid::neighbors(const OctoNode &node)
{
    vector<shared_ptr<OctoNode>> neighbors;
    for (int dir = 0; dir < 6; dir++)
    {
        auto [octreeNode, key, depth] = getNeighborSameOrHigher(octree, node, dir, true);
        if (octreeNode == NULL || !octree.isNodeOccupied(octreeNode))
        {
            neighbors.push_back(make_shared<OctoNode>(key, depth));
        }
    }
    return neighbors;
};

unsigned char childLUT[6][4] = {
    {0b000, 0b010, 0b100, 0b110}, // x-
    {0b001, 0b011, 0b101, 0b111}, // x+
    {0b000, 0b001, 0b100, 0b101}, // y-
    {0b010, 0b011, 0b110, 0b111}, // y+
    {0b000, 0b001, 0b010, 0b011}, // z-
    {0b100, 0b101, 0b110, 0b111}, // z+
};

vector<shared_ptr<OctoNode>> OctoGraphSparse::neighbors(const OctoNode &node)
{
    vector<shared_ptr<OctoNode>> neighbors;
    for (int dir = 0; dir < 6; dir++)
    {
        auto [currNode, currKey, currDepth] = getNeighborSameOrHigher(octree, node, dir, false);
        if (currNode == NULL)
        {
            tie(currNode, currKey, currDepth) = getNeighborSameOrHigher(octree, node, dir, true);
            neighbors.push_back(make_shared<OctoNode>(currKey, node.depth));
            continue;
        }
        vector<tuple<OcTreeNode *, OcTreeKey, unsigned int>> queue{{currNode, currKey, currDepth}};
        while (!queue.empty())
        {
            tie(currNode, currKey, currDepth) = queue.back();
            queue.pop_back();
            if (currNode == NULL)
            {
                neighbors.push_back(make_shared<OctoNode>(currKey, currDepth));
            }
            else if (!octree.nodeHasChildren(currNode))
            {
                if (!octree.isNodeOccupied(currNode))
                {
                    // node is free, add it to neighbors
                    neighbors.push_back(make_shared<OctoNode>(currKey, currDepth));
                }
            }
            else
            {
                // explore the children
                for (int i = 0; i < 4; i++)
                {
                    unsigned char childIdx = childLUT[dir][i];
                    OcTreeKey childKey;
                    computeChildKey(childIdx, (1 << (15 - currDepth)), currKey, childKey);
                    OcTreeNode *childNode = octree.nodeChildExists(currNode, childIdx) ? octree.getNodeChild(currNode, childIdx) : NULL;
                    queue.push_back({childNode, childKey, currDepth + 1});
                }
            }
        }
    }
    return neighbors;
};

OcTreeKey &makeKeyUnique(octomap::OcTreeKey &key, unsigned int depth)
{
    if (depth != 0)
    {
        // mask the unnecesary detail at the depth
        uint16_t mask = ~(65535u >> depth);
        key[0] &= mask;
        key[1] &= mask;
        key[2] &= mask;
    }
    return key;
}

/**
 * @brief Exactly like OcTree.search exept also outputs the depth of the node.
 * Finds the node and depth corresponding to a key and specified maximum depth for the search.
 * 
 * @param octree 
 * @param key The key at the lowest depth.
 * @param depth 
 * @return pair<OcTreeNode*, unsigned int> 
 */
pair<OcTreeNode *, unsigned int> searchWithDepth(const OcTree &octree, const OctoNode &node)
{
    const unsigned int tree_depth = octree.getTreeDepth();
    unsigned int depth = node.depth;
    OcTreeNode *root = octree.getRoot();
    assert(depth <= tree_depth);
    if (root == NULL)
        return {NULL, 0};

    if (depth == 0)
        depth = tree_depth;

    // generate appropriate key_at_depth for queried depth
    OcTreeKey key_at_depth = node.key;
    if (depth != tree_depth)
        key_at_depth = octree.adjustKeyAtDepth(node.key, depth);

    OcTreeNode *curNode(root);
    int diff = tree_depth - depth;
    // follow nodes down to requested level (for diff = 0 it's the last level)
    for (int i = (tree_depth - 1); i >= diff; --i)
    {
        unsigned int pos = computeChildIdx(key_at_depth, i);
        if (octree.nodeChildExists(curNode, pos))
        {
            // cast needed: (nodes need to ensure it's the right pointer)
            curNode = octree.getNodeChild(curNode, pos);
        }
        else
        {
            // we expected a child but did not get it
            // is the current node a leaf already?
            if (!octree.nodeHasChildren(curNode))
            {
                // The -1 is imporant. tree_depth - i is the depth of the **child**.
                // Subtract one to the the depth of curNode not the child.
                return {curNode, tree_depth - i - 1};
            }
            else
            {
                // it is not, the child must be NULL
                return {NULL, tree_depth - i};
            }
        }
    } // end for
    return {curNode, depth};
}

/**
 * @brief 
 * 
 * @param key 
 * @param diff Positive implies decreasing depth: making the key coarser.
 * @return OcTreeKey& 
 */
inline void changeDepth(OcTreeKey &key, int diff)
{
    if (diff >= 0)
    {
        key[0] = key[0] >> diff;
        key[1] = key[1] >> diff;
        key[2] = key[2] >> diff;
    }
    else
    {
        key[0] = key[0] << -diff;
        key[1] = key[1] << -diff;
        key[2] = key[2] << -diff;
    }
}

/**
 * @brief Increment/decrement the key based on dir.
 * Dir represents the eight faces of a cube.
 * 
 * @param key The key to be shifted. 
 * @param dir A 0-5 index of the faces of a cube. The least significant bit is the sign + or -.
 * The next two significant bits indicate the cooridnate, x=0 y=1 z=3. 
 * @return bool Return true on success. Return false if key would wraparound.
 */
inline bool shiftKey(OcTreeKey &key, unsigned char dir)
{
    assert(dir < 6);
    int i = dir >> 1;
    if (dir & 1)
    {
        if (key[i] == numeric_limits<key_type>::max())
        {
            return false;
        }
        else
        {
            key[i]++;
        }
    }
    else
    {
        if (key[i] == 0)
        {
            return false;
        }
        else
        {
            key[i]--;
        }
    }
    return true;
}

/**
 * @brief Get the neighbor weith the same depth or higher.
 * 
 * @param octree
 * @param node 
 * @param dir 
 * @param keepDepth Should we keep the depth of the input or coarsen if the depth of the found node is higher.
 * @return tuple<OcTreeNode*, OcTreeKey, unsigned int> Tuple of node, key and depth.
 */
tuple<OcTreeNode *, OcTreeKey, unsigned int> getNeighborSameOrHigher(const OcTree &octree, const OctoNode &node, unsigned char dir, bool keepDepth)
{
    int diff = 16 - node.depth;
    OcTreeKey neighbor_key = node.key;
    // we coarsen the key and add +- 1 to each coordinate based on dir, then make the key fine again.
    changeDepth(neighbor_key, diff);
    if (!shiftKey(neighbor_key, dir))
    {
        // The shifted key would wraparound.
        return {};
    }
    changeDepth(neighbor_key, -diff);

    OcTreeNode *octreeNode;
    unsigned int depth;
    tie(octreeNode, depth) = searchWithDepth(octree, OctoNode(neighbor_key, node.depth));
    if (octreeNode == NULL)
    {
        return {NULL, neighbor_key, node.depth};
    }
    else
    {
        if (keepDepth)
        {
            // we retain the depth, the octree might have a sparser structure but we are ignoring it
            return {octreeNode, neighbor_key, node.depth};
        }
        else
        {
            // we are coarsening the key to the desired depth
            return {octreeNode, makeKeyUnique(neighbor_key, depth), depth};
        }
    }
}

std::optional<OctoNode> coordToEndnode(octomap::OcTree &tree, octomap::point3d coordinate, unsigned int depth)
{
    OctoNode node(tree.coordToKey(coordinate), 0);
    auto [octreeNode, searchDepth] = searchWithDepth(tree, node);

    if(octreeNode != NULL && (tree.isNodeOccupied(octreeNode) || tree.nodeHasChildren(octreeNode))){
        return {};
    }
    node.depth = searchDepth;
    if (depth > 0)
    {
        node.depth = depth;
    }
    node.key = makeKeyUnique(node.key, node.depth);
    return node;
}