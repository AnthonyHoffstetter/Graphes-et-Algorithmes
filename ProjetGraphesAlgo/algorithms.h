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
    static void afficherGrapheReduit(const std::vector<std::vector<int>>& GR);
    static void afficherBasesReduit(const std::vector<std::vector<int>>& GR);
    static void analyserCFC(const Graphe& g);

    static void articulationEtIsthmes(const Graphe& g);




};


#endif // ALGORITHMS_H
