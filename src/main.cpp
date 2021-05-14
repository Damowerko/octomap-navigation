#include <vector>
#include <chrono>
#include "octonav/visualize.hpp"
#include "octonav/a_star.hpp"
#include "octonav/OctoGraph.hpp"

using namespace std;
using namespace octomap;

int main()
{
    OcTree fr78(0.05);
    fr78.readBinary("/home/damow/octomap-navigation/maps/fr_078_tidyup.bt");

    point3d start_point(-4, 0, 0.4);
    point3d end_point(-5.5, -2, 0.4);

    OctoGraphGrid grid(fr78);
    OctoGraphSparse sparse(fr78);

    cout << "Finding path" << endl;

    auto t0 = chrono::high_resolution_clock::now();
    vector<shared_ptr<OctoNode>> grid_nodes = find_path<OctoNode>(grid, coordToNode(fr78, start_point, 16), coordToNode(fr78, end_point, 16));
    auto t1 = chrono::high_resolution_clock::now();
    vector<shared_ptr<OctoNode>> sparse_nodes = find_path<OctoNode>(sparse, coordToNode(fr78, start_point), coordToNode(fr78, end_point));
    auto t2 = chrono::high_resolution_clock::now();

    vector<point3d> grid_path, sparse_path;
    for (auto node : grid_nodes)
    {
        grid_path.push_back(fr78.keyToCoord(node->key, node->depth));
    }
    for (auto node : sparse_nodes)
    {
        sparse_path.push_back(fr78.keyToCoord(node->key, node->depth));
    }

    float distance = (start_point - end_point).norm();
    chrono::duration<double, milli> grid_time = t1 - t0;
    chrono::duration<double, milli> sparse_time = t2 - t1;

    cout << "Distance: " << distance << endl;
    cout << "Grid Time: " << grid_time.count() << endl;
    cout << "Grid Len: " << grid_path.size() << endl;
    cout << "Sparse Time: " << sparse_time.count() << endl;
    cout << "Sparse Len: " << sparse_path.size() << endl;

    visualize(fr78, grid_path, sparse_path);
    cout << "Press any key to continue..." << endl;
    cin.get();
}