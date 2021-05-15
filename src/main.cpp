#include <vector>
#include <chrono>
#include "octonav/visualize.hpp"
#include "octonav/a_star.hpp"
#include "octonav/OctoGraph.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <string>

using namespace std;
using namespace octomap;

int main(int argc, char *argv[])
{
    // parameters
    bool doVisualize = false;
    string root_dir = "/home/damow/octomap-navigation/";
    string map = "fr78";
    int n_samples = 100;

    unique_ptr<OcTree> tree_ptr;
    uniform_real_distribution<> x_dis, y_dis, z_dis;
    default_random_engine rng;
    if (map == "fr78")
    {
        tree_ptr = make_unique<OcTree>(0.05);
        x_dis = uniform_real_distribution<>(-8, -2);
        y_dis = uniform_real_distribution<>(-5, 1);
        z_dis = uniform_real_distribution<>(0.1, 2);
    }
    OcTree &tree = *tree_ptr;
    tree.readBinary(root_dir + "maps/" + map + ".bt");
    OctoGraphGrid grid(tree);
    OctoGraphSparse sparse(tree);

    ofstream csv;
    csv.open(root_dir + "out/" + map + ".csv");
    csv << "Distance,Grid Time,Sparse Time,Grid Length,Sparse Length" << endl;

    for (int i = 0; i < n_samples; i++)
    {
        point3d start_point(x_dis(rng), y_dis(rng), z_dis(rng));
        point3d goal_point(x_dis(rng), y_dis(rng), z_dis(rng));

        optional<OctoNode> start_grid = coordToEndnode(tree, start_point, 16);
        optional<OctoNode> goal_grid = coordToEndnode(tree, goal_point, 16);
        optional<OctoNode> start_sparse = coordToEndnode(tree, start_point);
        optional<OctoNode> goal_sparse = coordToEndnode(tree, goal_point);

        if(!start_grid || !goal_grid || !start_sparse || !goal_sparse){
            // if not good staring points skipping
            n_samples++;
            continue;
        }

        auto t0 = chrono::high_resolution_clock::now();
        vector<shared_ptr<OctoNode>> grid_nodes = find_path<OctoNode>(grid, start_grid.value(), goal_grid.value());
        auto t1 = chrono::high_resolution_clock::now();
        vector<shared_ptr<OctoNode>> sparse_nodes = find_path<OctoNode>(sparse, start_sparse.value(), goal_sparse.value());
        auto t2 = chrono::high_resolution_clock::now();

        float distance = (start_point - goal_point).norm();
        chrono::duration<double, milli> grid_time = t1 - t0;
        chrono::duration<double, milli> sparse_time = t2 - t1;

        csv << distance << "," << grid_time.count() << "," << sparse_time.count() << "," << grid_nodes.size() << "," << sparse_nodes.size() << endl;

        if (doVisualize)
        {
            cout << "Distance: " << distance << endl;
            cout << "Grid Time: " << grid_time.count() << endl;
            cout << "Sparse Time: " << sparse_time.count() << endl;
            cout << "Grid Len: " << grid_nodes.size() << endl;
            cout << "Sparse Len: " << sparse_nodes.size() << endl;

            vector<point3d> grid_path, sparse_path;
            for (auto node : grid_nodes)
            {
                grid_path.push_back(tree.keyToCoord(node->key, node->depth));
            }
            for (auto node : sparse_nodes)
            {
                sparse_path.push_back(tree.keyToCoord(node->key, node->depth));
            }
            visualize(tree, grid_path, sparse_path);
            cout << "Press any key to continue..." << endl;
            cin.get();
        }
    }
    csv.close();
}