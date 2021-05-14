#pragma once
#include <vector>
#include <tuple>
#include <array>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <memory>

class Node
{
public:
    void set_f_score()
    {
        f_score = g_score + h_score;
    }
    std::shared_ptr<Node> parent;
    float f_score = std::numeric_limits<float>::infinity();
    float g_score = std::numeric_limits<float>::infinity();
    float h_score = std::numeric_limits<float>::infinity();

    virtual bool operator>(const Node &other) const
    {
        return f_score > other.f_score;
    }
};

template <class T>
float heuristic(std::shared_ptr<T> current, std::shared_ptr<T> goal);

template <class T>
class Graph
{
public:
    virtual std::vector<std::shared_ptr<T>> neighbors(const T &node) = 0;
};

// Builds path from start to current node.
template <class T>
std::vector<std::shared_ptr<T>> reconstruct_path(std::shared_ptr<T> current)
{
    std::vector<std::shared_ptr<T>> path;

    // recurse through path of nodes and add to path
    while (current != nullptr)
    {
        path.push_back(current);
        current = std::static_pointer_cast<T>(current->parent);
    }
    // reverse path so it's in the correct order
    reverse(path.begin(), path.end());

    return path;
};

// A* implementation. Finds optimal path from start to goal node.
template <class T>
std::vector<std::shared_ptr<T>> find_path(Graph<T> &graph, T start_, T goal_)
{
    static_assert(std::is_base_of<Node, T>::value, "T must inherit from Node.");
    auto start = std::make_shared<T>(start_);
    auto goal = std::make_shared<T>(goal_);

    // Set g, h, f values of start node.
    start->g_score = 0.0;
    start->h_score = heuristic(start, goal);
    start->set_f_score();

    // Priority Queue of nodes to explore. Top element has lowest f score.
    typedef std::shared_ptr<Node> ptr;
    auto greater = [](const ptr n1, const ptr n2) { return *n1 > *n2; };
    std::priority_queue<std::shared_ptr<T>, std::vector<std::shared_ptr<T>>, decltype(greater)> open_list(greater);
    // For time efficiency, we will use a hash set to do the .contains() lookup.
    // Nodes will be added and removed just as they are in the priority_queue.
    std::unordered_set<std::shared_ptr<T>> open_set;
    std::unordered_map<T, std::shared_ptr<T>> nodes;

    // Add start node to Open List.
    open_list.push(start);
    open_set.insert(start);
    nodes[*start] = start;

    while (!open_list.empty())
    {
        // Remove top Node from Open List.
        std::shared_ptr<T> current = open_list.top();
        open_list.pop();
        open_set.erase(current);

        // If you've arrived at the goal node, return path leading to it.
        if (*current == *goal)
        {
            return reconstruct_path(current);
        }
        // Iterate through neighbors of current node.
        std::vector<std::shared_ptr<T>> curr_neighbors = graph.neighbors(*current);
        for (std::shared_ptr<T> neighbor : curr_neighbors)
        {
            if (nodes.count(*neighbor))
            {
                neighbor = nodes[*neighbor];
            }
            else
            {
                nodes[*neighbor] = neighbor;
            }
            // Compute a potential new g score for neighbor if you go through the current node.
            // Assume that weight between nodes is same as h heuristic.
            float tentative_g_score = current->g_score + heuristic(current, neighbor);
            if (tentative_g_score < neighbor->g_score)
            {
                // Store this path to neighbor as it is better than previously stored path.
                neighbor->parent = current;
                neighbor->g_score = tentative_g_score;
                neighbor->h_score = heuristic(neighbor, goal);
                neighbor->set_f_score();

                // Add neighbor to Open List if it's not already there.
                if (open_set.count(neighbor) == 0)
                {
                    open_list.push(neighbor);
                    open_set.insert(neighbor);
                }
            }
        }
    }

    // Search has failed- goal node was not found. An empty std::vector is returned.
    return std::vector<std::shared_ptr<T>>();
};