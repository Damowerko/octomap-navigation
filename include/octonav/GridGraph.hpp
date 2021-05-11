#include "a_star.hpp"

class GridNode : Node {
    int x;
    int y;
    int z;
};


class GridGraph : Graph<GridNode> {

};