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

    static bool calculRangs(const Graphe& g, std::vector<int>& rang);

    static void tarjan(const Graphe& g, std::vector<int>& cfc, int& nbCfc);
    static std::vector<std::vector<int>> construireGrapheReduit(const Graphe& g, const std::vector<int>& cfc, int nbCfc);

    static void articulationsEtIsthmes(const Graphe& g, std::vector<int>& points, std::vector<std::pair<int, int>>& isthmes);

    static std::vector<int> dijkstra(const GrapheValue& g, int source);

    static std::vector<std::vector<int>> dantzig(const GrapheValue& g);

    static std::vector<std::tuple<int, int, int>> kruskal(const GrapheValue& g);








};


#endif // ALGORITHMS_H
