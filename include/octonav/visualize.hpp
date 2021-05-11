#pragma once
#include "OctoGraph.hpp"
#include <vector>
#include <octomap/octomap.h>
#include <octomap/OcTree.h>

static std::vector<OctoNode> EMPTY_VEC = std::vector<OctoNode>();
void visualize(const OctoGraph &graph, const std::vector<OctoNode> &path = EMPTY_VEC);
