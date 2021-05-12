#pragma once
#include <vector>
#include <octomap/octomap.h>
#include <octomap/OcTree.h>

/**
 * @brief Plot the octree and path.
 * 
 * @param octree 
 * @param path 
 */
void visualize(octomap::OcTree &octree, const std::vector<octomap::point3d> &path1, const std::vector<octomap::point3d> &path2);