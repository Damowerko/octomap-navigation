#pragma once
#include "a_star.hpp"

typedef std::array<size_t, 3> coordinate;

class GridGraph : Graph<coordinate>
{
public:
    GridGraph(coordinate size, std::vector<bool> data) : size(size), data(data) {}

    std::vector<coordinate> neighbors(coordinate node)
    {
        // TODO: Implement
    }

private:
    coordinate size;
    std::vector<bool> data;
};