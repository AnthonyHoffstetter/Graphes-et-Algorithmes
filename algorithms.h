#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graphe.h"
#include "graphevalue.h"
#include <vector>
#include <string>

class Algorithms{
public :
    static std::vector<int> bfs(const Graphe& g, int s);
    static std::vector<std::vector<int>> calculerMatriceDistances(const Graphe& g);

};


#endif // ALGORITHMS_H
