#include "octonav/OctoGraph.hpp"
#include <optional>

using namespace std;
using namespace octomap;

inline bool shiftKey(OcTreeKey &key, unsigned char dir);
inline void changeDepth(OcTreeKey &key, int diff);
tuple<OcTreeNode *, OcTreeKey, unsigned int> getNeighborSameOrHigher(const OcTree &octree, const OctoNode &node, unsigned char dir, bool keepDepth = false);
pair<OcTreeNode *, unsigned int> searchWithDepth(const OcTree &octree, const OcTreeKey &key, unsigned int depth);

vector<OctoNode> OctoGraphGrid::neighbors(const OctoNode &node)
{
    vector<OctoNode> neighbors;
    for (int dir = 0; dir < 6; dir++)
    {
        auto [octreeNode, key, depth] = getNeighborSameOrHigher(octree, node, dir, true);
        if (octreeNode != NULL && !octree.isNodeOccupied(octreeNode))
        {
            neighbors.push_back(OctoNode(key, depth));
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

vector<OctoNode> OctoGraphSparse::neighbors(const OctoNode &node)
{
    vector<OctoNode> neighbors;
    for (int dir = 0; dir < 6; dir++)
    {
        auto [currNode, currKey, currDepth] = getNeighborSameOrHigher(octree, node, dir, false);
        if (currNode != NULL)
        {
            vector<tuple<OcTreeNode *, OcTreeKey, unsigned int>> queue{};
            do
            {
                if (!octree.nodeHasChildren(currNode))
                {
                    if (!octree.isNodeOccupied(currNode))
                    {
                        // node is free, add it to neighbors
                        neighbors.push_back(OctoNode(currKey, currDepth));
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
                        if (octree.nodeChildExists(currNode, childIdx))
                        {
                            OcTreeNode* childNode = octree.getNodeChild(currNode, childIdx);
                            queue.push_back({childNode, childKey, currDepth+1});       
                        } else {
                            // child does not exist and therefore we will assume is free
                            neighbors.push_back(OctoNode(childKey, currDepth+1));
                        }
                    }
                }
                tie(currNode, currKey, currDepth) = queue.back();
                queue.pop_back();
            } while (!queue.empty());
        }
    }
    return neighbors;
};

inline OcTreeKey& makeKeyUnique(OcTreeKey& key, unsigned int depth)
{
    unsigned int level = 16 - depth;
    if (level != 0)
    {
        // mask the unnecesary detail at the depth
        uint16_t mask = 65536 << level;
        key[0] &= ~mask;
        key[1] &= ~mask;
        key[2] &= ~mask;
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
static pair<OcTreeNode *, unsigned int> searchWithDepth(const OcTree &octree, const OctoNode &node)
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
            { // TODO similar check to nodeChildExists?
                return {curNode, i - tree_depth + 1};
            }
            else
            {
                // it is not, search failed
                return {NULL, 0};
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
    key[0] = key[0] >> diff;
    key[1] = key[1] >> diff;
    key[2] = key[2] >> diff;
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
    tie(octreeNode, depth) = searchWithDepth(octree, node);
    if (octreeNode == NULL)
    {
        return {};
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
            makeKeyUnique(neighbor_key, depth);
            return {octreeNode, neighbor_key, depth};
        }
    }
}