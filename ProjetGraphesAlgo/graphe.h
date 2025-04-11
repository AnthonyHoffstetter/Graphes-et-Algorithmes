#ifndef GRAPHE_H
#define GRAPHE_H

#include <vector>
#include <string>
#include <fstream>

class Graphe
{
    friend class MainWindow;
    friend class AjouterArcsDialog;
    friend class Algorithms;

protected:
    std::vector<int> fs;
    std::vector<int> aps;
    bool estOriente;

public:
    Graphe(bool oriente=true);
    virtual ~Graphe() = default;

    virtual void saisirDepuisFichier(const std::string &nomFichier);
    virtual void saisirDepuisClavier();
    virtual void sauvegarderDansFichier(const std::string& nomFichier) const ;

    virtual void ajouterSommet();
    bool ArcDejaPresent(int u, int v) const;
    void ajouterArc(int u, int v);
    void afficherFSAPS() const;

    std::vector<std::vector<int>> toMatriceAdjacence() const;
    void afficherMatriceAdjacence() const;
};

#endif // GRAPHE_H
