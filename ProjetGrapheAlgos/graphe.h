#ifndef GRAPHE_H
#define GRAPHE_H

#include <vector>

class Graphe
{
private:
    std::vector<int> fs;
    std::vector<int> aps;
    bool estOriente;



public:
    Graphe(bool oriente=true);

    void ajouterSommet();
    void ajouterArc(int u, int v);
    void afficherFSAPS() const;

    std::vector<std::vector<int>> toMatriceAdjacence() const;
    void afficherMatriceAdjacence() const;
};

#endif // GRAPHE_H
