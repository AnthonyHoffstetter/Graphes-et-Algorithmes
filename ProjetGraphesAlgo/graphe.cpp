#include "graphe.h"
#include <iostream>
#include <sstream>

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

void Graphe::saisirDepuisClavier() {
    std::string ligne;
    std::cout << "Saisissez les arcs (format : u v), ou 'fin' pour terminer :\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, ligne);

        if (ligne == "fin") break;

        std::istringstream iss(ligne);
        int u, v;
        if (iss >> u >> v) {
            // Ajouter des sommets si besoin
            int maxUV = std::max(u, v);
            while (aps[0] < maxUV) {
                ajouterSommet();
            }

            ajouterArc(u, v);
        } else {
            std::cout << "Entrée invalide. Format attendu : u v ou 'fin'.\n";
        }
    }

    std::cout << "Graphe saisi avec succes.\n";
}


void Graphe::saisirDepuisFichier(const std::string &nomFichier){
    std::ifstream fichier(nomFichier);
    if (!fichier) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier.\n";
        return;
    }
    std::string ligne;

    aps.clear();
    fs.clear();
    aps.push_back(0);
    fs.push_back(0);

    while (std::getline(fichier, ligne)) {
        if (ligne.empty() || ligne[0] == '#') continue; // ignorer les commentaires

        std::istringstream iss(ligne);
        int u,v;
        if (iss >> u >> v){
            if(aps[0]<std::max(u,v)){
            while(aps[0]!=std::max(u,v))
                ajouterSommet();
            }
        ajouterArc(u,v);
        }
    }

    fichier.close();


}

void Graphe::sauvegarderDansFichier(const std::string& nomFichier) const{
    std::ofstream fichier(nomFichier);

    fichier << "# Graphe sauvegardé\n";
    fichier << "# Liste des arcs du graphe\n\n";

    for (int u = 1; u <= aps[0]; ++u) {
        for (int i = aps[u]; fs[i] != 0; ++i) {
            int v = fs[i];
            fichier << u << " " << v << "\n";
        }
    }

    fichier.close();
    std::cout << "Graphe sauvegarde dans : " << nomFichier << std::endl;

}

bool Graphe::ArcDejaPresent(int u, int v) const {
    if (u <= 0 || u >= aps.size() || v <= 0 || v >= aps.size()) {
        std::cerr << "Erreur : sommet " << u << " ou " << v << " inexistant.\n";
        return false;
    }

    int i = aps[u];
    while(fs[i]!=v && fs[i]!=0)
        i++;

    return fs[i]==v;

}


void Graphe::ajouterArc(int u, int v) {
    if (u <= 0 || u >= aps.size()) {
        std::cerr << "Erreur : sommet " << u << " inexistant.\n";
        return;
    }

    if(ArcDejaPresent(u,v))
        return;

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
        if(ArcDejaPresent(v,u))
            return;
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
    int n = aps[0];
    std::vector<std::vector<int>> mat(n, std::vector<int>(n, 0));


    for (int u = 1; u <= n; ++u) {
        int i = aps[u];
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


