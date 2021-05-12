#include "octonav/a_star.hpp"

bool compare(std::shared_ptr<Node> n1, std::shared_ptr<Node> n2)
{
    // custom comparison for node's f scores
    return n1->f_score > n2->f_score;
};