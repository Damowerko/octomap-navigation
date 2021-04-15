#include <vector>
#include <tuple>
#include <array>
#include <queue>
#include <octomap/octomap.h>
#include <octomap/OcTree.h>

template<class T>
class Graph {
public:
    virtual std::vector<T> neighbors(T node);
    virtual float cost(T node1, T node2);
};

typedef std::array<size_t, 3> coordinate;

class GridGraph : Graph<coordinate> {
public:
    GridGraph(coordinate size, std::vector<bool> data) : size(size), data(data) {}

    std::vector<coordinate> neighbors(coordinate node){
        // TODO: Implement        
    }
private:
    coordinate size;
    std::vector<bool> data;
};

/**
 * @brief 
 */
template<class T>
float a_star (Graph<T> graph, T start, T end, float (*function)(T,T) heuristic) {
    // TODO: implement
};