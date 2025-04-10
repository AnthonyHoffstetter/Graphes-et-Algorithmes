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

void Graphe::ajouterArc(int u, int v) {
    if (u <= 0 || u >= aps.size()) {
        std::cerr << "Erreur : sommet " << u << " inexistant.\n";
        return;
    }

    int start = aps[u];               // début de la liste de u
    int i = start;

    // Chercher le 0 de fin
    while (fs[i] != 0 && fs[i]<v) {
        ++i;
    }

    // Insérer v juste avant le 0
    fs.insert(fs.begin() + i, v);

    // Incrémenter fs[0] (nb total d'arcs)
    fs[0]++;

    // Mettre à jour les aps suivants (décalage)
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

std::vector<std::vector<int>> Graphe::toMatriceAdjacence() const{
    int n = aps[0]; // nombre de sommets
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

void Graphe::afficherMatriceAdjacence() const {
    auto mat = toMatriceAdjacence();
    for (const auto& ligne : mat) {
        for (int val : ligne)
            std::cout << val << " ";
        std::cout << "\n";
    }
}
