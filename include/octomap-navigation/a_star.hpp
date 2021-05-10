#pragma once
#include <vector>
#include <tuple>
#include <array>
#include <queue>

template <class T>
class Graph
{
public:
    virtual std::vector<T> neighbors(T node);
};

/**
 * @brief 
 */
template <class T>
float a_star(Graph<T> graph, T start, T end, float (*heuristic)(T, T)){
    // TODO: implement
};