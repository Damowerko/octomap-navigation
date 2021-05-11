#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_set>
#include <vector>
#include <queue>

using namespace std;

class Node {
public:
    Node() : x(0.0), y(0.0), z(0.0), parent(nullptr), 
        f_score(numeric_limits<float>::infinity()), 
        g_score(numeric_limits<float>::infinity()),
        h_score(numeric_limits<float>::infinity()) {}
    Node(float x_, float y_, float z_) : x(x_), y(y_), z(z_), parent(nullptr), 
        f_score(numeric_limits<float>::infinity()), 
        g_score(numeric_limits<float>::infinity()),
        h_score(numeric_limits<float>::infinity()) {}
    
    void set_f_score() {
        this->f_score = this->g_score + this->h_score;
    }
    
    const float x;
    const float y;
    const float z;
    float f_score;
    float g_score;
    float h_score;
    Node* parent;
};

bool compare(Node* n1, Node* n2) {
    // custom comparison for node's f scores
    return (n1->f_score > n2->f_score);
};

vector<Node*> find_neighbors(Node* node) {
    // TODO
    return vector<Node*>();
}

class A_star_base {
public:
    A_star_base() {}
    
    // A* implementation. Finds optimal path from start to goal node.
    vector<Node*> find_path(Node* start, Node* goal);
    
    // Computes h(current)
    virtual float compute_h_score(Node* current, Node* goal)=0;
    
    // Builds path from start to current node.
    vector<Node*> reconstruct_path(Node* current);
};

vector<Node*> A_star_base::reconstruct_path(Node* current) {
    vector<Node*> path;
    
    // recurse through path of nodes and add to path
    while (current != nullptr) {
        path.push_back(current);
        current = current->parent;
    }
    // reverse path so it's in the correct order
    reverse(path.begin(), path.end());
    
    return path;
};

vector<Node*> A_star_base::find_path(Node* start, Node* goal) {
    // Set g, h, f values of start node.
    start->g_score = 0.0;
    start->h_score = compute_h_score(start, goal);
    start->set_f_score();
    
    // Priority Queue of nodes to explore. Top element has lowest f score.
    priority_queue<Node*, vector<Node*>, decltype(&compare)> open_list(&compare);
    // For time efficiency, we will use a hash set to do the .contains() lookup.
    // Nodes will be added and removed just as they are in the priority_queue.
    unordered_set<Node*> open_set;
    
    // Add start node to Open List.
    open_list.push(start);
    open_set.insert(start);
    
    while (!open_list.empty()) {
        // Remove top Node from Open List.
        Node* current = open_list.top();
        open_list.pop();
        open_set.erase(open_set.find(current));
        
        // If you've arrived at the goal node, return path leading to it.
        if (current == goal) {
            return reconstruct_path(current);
        }
        
        // Iterate through neighbors of current node.
        vector<Node*> curr_neighbors = find_neighbors(current);
        for (Node* neighbor : curr_neighbors) {
            // Compute a potential new g score for neighbor if you go through the current node.
            // Assume that weight between nodes is same as h heuristic.
            float tentative_g_score = current->g_score + compute_h_score(current, neighbor);
            if (tentative_g_score < neighbor->g_score) {
                // Store this path to neighbor as it is better than previously stored path.
                neighbor->parent = current;
                neighbor->g_score = tentative_g_score;
                neighbor->h_score = this->compute_h_score(neighbor, goal);
                neighbor->set_f_score();
                
                // Add neighbor to Open List if it's not already there.
                if (open_set.count(neighbor) == 0) {
                    open_list.push(neighbor);
                    open_set.insert(neighbor);
                }
            }
        }
    }
    
    // Search has failed- goal node was not found. An empty vector is returned.
    return vector<Node*>();
};

int main()
{

    return 0;
}
