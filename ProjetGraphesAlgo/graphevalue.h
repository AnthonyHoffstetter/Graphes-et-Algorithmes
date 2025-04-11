#ifndef GRAPHEVALUE_H
#define GRAPHEVALUE_H

#include "graphe.h"

struct Successeur;

struct Sommet {
    int id;
    Sommet* sommetSuivant;
    Successeur* fileSucesseur;
};

struct Successeur{
    int poids;
    Sommet* sommet;
    Successeur* successeurSuivant;
};

class GrapheValue : public Graphe
{
private:
    Sommet* listePrincipale;

public:
    Sommet* trouverSommet(int id) const;
    GrapheValue(bool oriente=true);

    // Surcharge des méthodes de Graphe
    virtual void ajouterSommet() override;
    virtual void saisirDepuisFichier(const std::string &nomFichier) override;
    virtual void saisirDepuisClavier() override;
    virtual void sauvegarderDansFichier(const std::string& nomFichier) const override;


    // Méthodes spécifiques à GrapheValue
    void ajouterArc(int u, int v, int poids); // version avec poids
    void afficherListeDynamique() const;

};

#endif // GRAPHEVALUE_H
