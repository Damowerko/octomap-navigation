#include <vector>
#include "octonav/visualize.hpp"

using namespace std;
using namespace octomap;

int main()
{
    OcTree tree(0.1); // create empty tree with resolution 0.1

    // insert some measurements of occupied cells

    for (int x = -20; x < 20; x++)
    {
        for (int y = -20; y < 20; y++)
        {
            for (int z = -20; z < 20; z++)
            {
                point3d endpoint((float)x * 0.05f, (float)y * 0.05f, (float)z * 0.05f);
                tree.updateNode(endpoint, true); // integrate 'occupied' measurement
            }
        }
    }

    // insert some measurements of free cells

    for (int x = -30; x < 30; x++)
    {
        for (int y = -30; y < 30; y++)
        {
            for (int z = -30; z < 30; z++)
            {
                point3d endpoint((float)x * 0.02f - 1.0f, (float)y * 0.02f - 1.0f, (float)z * 0.02f - 1.0f);
                tree.updateNode(endpoint, false); // integrate 'free' measurement
            }
        }
    }

    visualize(tree);
}