#include "graphevalue.h"
#include <iostream>
#include <sstream>

GrapheValue::GrapheValue(bool oriente) : Graphe(oriente),listePrincipale{nullptr}
{}

void GrapheValue::ajouterSommet() {
    int nouvelId = 1;

    Sommet* courant = listePrincipale;
    while (courant && courant->sommetSuivant) {
        courant = courant->sommetSuivant;
    }

    if (courant) {
        nouvelId = courant->id + 1;
    }

    Sommet* nouveau = new Sommet{nouvelId, nullptr, nullptr};

    if (!listePrincipale) {
        listePrincipale = nouveau;
    } else {
        courant->sommetSuivant = nouveau;
    }

    Graphe::ajouterSommet();

    std::cout << "Sommet ajoute avec id : " << nouvelId << std::endl;
}


Sommet* GrapheValue::trouverSommet(int id) const {
    Sommet* courant = listePrincipale;
    while (courant) {
        if (courant->id == id) return courant;
        courant = courant->sommetSuivant;
    }
    return nullptr;
}

void GrapheValue::ajouterArc(int u, int v, int poids) {
    // Vérifie si l'arc existe déjà
    if (ArcDejaPresent(u, v)) {
        std::cout << "Arc " << u << " → " << v << " déjà présent." << std::endl;
        return;
    }

    // Trouver ou ajouter le sommet source
    Sommet* src = trouverSommet(u);
    if (!src) {
        ajouterSommet();
        src = trouverSommet(u);
    }

    // Trouver ou ajouter le sommet destination
    Sommet* dest = trouverSommet(v);
    if (!dest) {
        ajouterSommet();
        dest = trouverSommet(v);
    }

    // Créer un nouveau successeur
    Successeur* nouveau = new Successeur{poids, dest, nullptr};

    if (!src->fileSucesseur) {
        src->fileSucesseur = nouveau;
    } else {
        Successeur* courant = src->fileSucesseur;
        while (courant->successeurSuivant) {
            courant = courant->successeurSuivant;
        }
        courant->successeurSuivant = nouveau;
    }

    Graphe::ajouterArc(u, v);

    std::cout << "Arc ajoute : " << u << " -> " << v << " (poids: " << poids << ")" << std::endl;
}


void GrapheValue::afficherListeDynamique() const{
    Sommet* courant = listePrincipale;

    while (courant) {
        std::cout << "Sommet " << courant->id << " : ";

        Successeur* s = courant->fileSucesseur;
        while (s) {
            std::cout << " -> " << s->sommet->id << " (poids: " << s->poids << ") ";
            s = s->successeurSuivant;
        }

        std::cout << std::endl;
        courant = courant->sommetSuivant;
    }
}

void GrapheValue::saisirDepuisFichier(const std::string &nomFichier){
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
        int u,v,p;
        if (iss >> u >> v >> p){
            if(aps[0]<std::max(u,v)){
                while(aps[0]!=std::max(u,v))
                    ajouterSommet();
            }
            ajouterArc(u,v,p);
        }
    }

    fichier.close();


}


void GrapheValue::sauvegarderDansFichier(const std::string& nomFichier) const{
}


void GrapheValue::saisirDepuisClavier(){
    std::string ligne;
    std::cout << "Saisissez les arcs avec le poids (format : u v poids), ou 'fin' pour terminer :\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, ligne);

        if (ligne == "fin") break;

        std::istringstream iss(ligne);
        int u, v,p;
        if (iss >> u >> v>>p) {
            // Ajouter des sommets si besoin
            int maxUV = std::max(u, v);
            while (aps[0] < maxUV) {
                ajouterSommet();
            }

            ajouterArc(u, v,p);
            Graphe::ajouterArc(u,v);
        } else {
            std::cout << "Entree invalide. Format attendu : u v ou 'fin'.\n";
        }
    }

    std::cout << "Graphe saisi avec succes.\n";
}


