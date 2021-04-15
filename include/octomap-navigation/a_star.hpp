#include <vector>
#include <tuple>
#include <array>
#include <queue>
#include "octomap-navigation/utils.hpp"

using namespace std;



template<class T>
class Graph {
public:
    virtual vector<T> neighbors(T node);
    virtual float cost(T node1, T node2);
};

typedef array<size_t, 3> coordinate;

class Grid3 : Graph<coordinate> {
public:
    Grid3(coordinate size, vector<bool> data) : size(size), data(data) {}

    vector<coordinate> neighbors(coordinate node){
        // TODO: Implement        
    }
private:
    coordinate size;
    vector<bool> data;
};

/**
 * @brief 
 */
template<class T>
float a_star (Graph<T> graph, T start, T end, float (*function)(T,T) heuristic) {
    // TODO: implement
};