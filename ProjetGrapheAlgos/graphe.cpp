#include "graphe.h"
#include <iostream>

Graphe::Graphe(bool oriente) : estOriente(oriente)
{
    aps.push_back(0);
    fs.push_back(0);
}

void Graphe::ajouterSommet(){
    fs.push_back(0);
    aps.push_back(fs.size()-1);
    fs[0]++;
    aps[0]++;
}

//NE GERE PAS LE CAS OU IL Y A UN DOUBLON (graphe non orienté)
void Graphe::ajouterArc(int u, int v) {
    if (u <= 0 || u >= aps.size()) {
        std::cerr << "Erreur : sommet " << u << " inexistant.\n";
        return;
    }

    int start = aps[u];
    int i = start;
    while (fs[i] != 0 && fs[i]<v) {
        ++i;
    }
    fs.insert(fs.begin() + i, v);
    fs[0]++;
    for (int j = u + 1; j < aps.size(); ++j) {
        aps[j]++;
    }

    if (!estOriente) {
        start = aps[v];
        i = start;

        while (fs[i] != 0 && fs[i] < u) ++i;
        fs.insert(fs.begin() + i, u);
        fs[0]++;
        for (int j = v + 1; j < aps.size(); ++j) aps[j]++;
    }

}

void Graphe::afficherFSAPS() const {
    std::cout << "FS: ";
    for (int x : fs) std::cout << x << " ";
    std::cout << "\nAPS: ";
    for (int x : aps) std::cout << x << " ";
    std::cout << "\n";
}

//A CORRIGER
std::vector<std::vector<int>> Graphe::toMatriceAdjacence() const{
    int n = aps[0];
    std::vector<std::vector<int>> mat(n, std::vector<int>(n, 0));

    for (int u = 1; u <= n; ++u) {
        int i = aps[u - 1];
        while (fs[i] != 0) {
            int v = fs[i];
            mat[u - 1][v - 1] = 1;
            ++i;
        }
    }

    return mat;
}

//A CORRIGER
void Graphe::afficherMatriceAdjacence() const {
    auto mat = toMatriceAdjacence();
    for (const auto& ligne : mat) {
        for (int val : ligne)
            std::cout << val << " ";
        std::cout << "\n";
    }
}
