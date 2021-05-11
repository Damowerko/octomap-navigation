#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "octonav/OctoGraph.hpp"

using namespace std;
using namespace octomap;

TEST_CASE("OctoGraphGrid neighbors", "[graph]") {
    OcTree tree(1);
    for(int x = 0; x < 3; x++){
        tree.updateNode(point3d(x, 0, 0), true);
        tree.updateNode(point3d(x, 1, 0), false);
        tree.updateNode(point3d(x, 2, 0), true);
    }
    tree.writeBinary("/home/damow/octomap-navigation/test_tree.bt");
}