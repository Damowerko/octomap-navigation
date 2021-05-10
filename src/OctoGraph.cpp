#include "OctoGraph.hpp"

using namespace std;
using namespace octomap;

vector<OctoNode> OctoGraphGrid::neighbors(OctoNode node)
{
    vector<OctoNode> neighbors;
    OcTreeKey key;
    // Check all eight neighbors and add them to vector if occupied.
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            for (int k = -1; k <= 1; k++)
            {
                if (i == 0 && j == 0 && k == 0)
                {
                    continue;
                }
                OcTreeKey key = node.key;
                key[0] = node.key[0] + i;
                key[1] = node.key[1] + j;
                key[2] = node.key[2] + k;
                OcTreeNode *octreeNode = octree.search(key, node.depth);
                if (octreeNode != NULL && !octree.isNodeOccupied(octreeNode))
                {
                    neighbors.push_back({key, node.depth});
                }
            }
        }
    }
};