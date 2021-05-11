#include <vector>
#include <tuple>
#include <array>
#include <queue>

class Node
{
public:
    void set_f_score()
    {
        f_score = f_score + g_score;
    }
    Node* parent;
    float f_score, g_score, h_score = std::numeric_limits<float>::infinity();
};

template <class T>
class Graph
{
public:
    virtual std::vector<T> neighbors(const T &node);
};

/**
 * @brief 
 */
template <class T>
float a_star(Graph<T> graph, T start, T end, float (*heuristic)(T, T)){
    // TODO: implement
};