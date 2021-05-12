#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "octonav/OctoGraph.hpp"
#include "octonav/visualize.hpp"

using namespace std;
using namespace octomap;

TEST_CASE("Tunnel neighbors", "[graph]")
{
    OcTree tree(1);
    OctoGraphGrid graph(tree);

    for (int x = 0; x < 3; x++)
    {
        tree.updateNode(point3d(x, 0, 0), true);  //left
        tree.updateNode(point3d(x, 2, 0), true);  // right
        tree.updateNode(point3d(x, 1, 1), true);  // top
        tree.updateNode(point3d(x, 1, -1), true); // botton

        tree.updateNode(point3d(x, 1, 0), false); // center is free
    }

    OctoNode start = coordToNode(tree, point3d(1, 1, 0), 16);
    REQUIRE(start.depth == 16);

    auto n = graph.neighbors(start);
    REQUIRE(n.size() == 2);
    //REQUIRE(count(n.begin(), n.end(), OctoNode(OcTreeKey(start.key[0] + 1, start.key[1], start.key[2]), 16)) == 1);
    //REQUIRE(count(n.begin(), n.end(), OctoNode(OcTreeKey(start.key[0] - 1, start.key[1], start.key[2]), 16)) == 1);
}