#include "algorithms.h"
#include <queue>
#include <limits>
#include <iostream>
#include <stack>
#include <functional>

std::vector<int> Algorithms::bfs(const Graphe& g, int s) {
    int n = g.aps[0];
    std::vector<int> dist(n + 1, std::numeric_limits<int>::max());
    std::vector<bool> vis(n + 1, false);
    std::queue<int> q;

    dist[s] = 0;
    vis[s] = true;
    q.push(s);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        int i = g.aps[u];
        while (g.fs[i] != 0) {
            int v = g.fs[i];
            if (!vis[v]) {
                vis[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
            i++;
        }
    }

    return dist;
}

std::vector<std::vector<int>> Algorithms::calculerMatriceDistances(const Graphe& g) {
    int n = g.aps[0];
    std::vector<std::vector<int>> matrice(n + 1, std::vector<int>(n + 1, -1)); // -1 = non atteignable

    for (int s = 1; s <= n; ++s) {
        std::vector<int> dist = bfs(g, s);
        for (int t = 1; t <= n; ++t) {
            if (dist[t] != std::numeric_limits<int>::max()) {
                matrice[s][t] = dist[t];
            }
        }
    }

    return matrice;
}

bool Algorithms::calculRangs(const Graphe& g, std::vector<int>& rang) {
    int n = g.aps[0];
    rang.assign(n + 1, -1);
    std::vector<int> d(n + 1, 0);
    std::queue<int> q;
    int compte = 0;

    // Étape 1 : calcul des prédécesseurs
    for (int u = 1; u <= n; ++u) {
        for (int i = g.aps[u]; g.fs[i] != 0; ++i) {
            int v = g.fs[i];
            d[v]++;
        }
    }

    // Étape 2 : initialisation file
    for (int i = 1; i <= n; ++i) {
        if (d[i] == 0) {
            rang[i] = 0;
            q.push(i);
        }
    }

    // Étape 3 : propagation
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        compte++;

        for (int i = g.aps[u]; g.fs[i] != 0; ++i) {
            int v = g.fs[i];
            d[v]--;
            if (d[v] == 0) {
                rang[v] = rang[u] + 1;
                q.push(v);
            }
        }
    }

    // Si tous les sommets n’ont pas été traités → circuit
    return (compte == n);
}

// ===== Tarjan =====
void Algorithms::tarjan(const Graphe& g, std::vector<int>& cfc, int& nbCfc) {
    int n = g.aps[0];
    std::vector<int> num(n + 1, 0), low(n + 1, 0);
    std::vector<bool> enPile(n + 1, false);
    std::stack<int> pile;
    cfc.assign(n + 1, 0);

    int compteur = 0;
    nbCfc = 0;

    std::function<void(int)> strongConnect = [&](int u) {
        compteur++;
        num[u] = low[u] = compteur;
        pile.push(u);
        enPile[u] = true;

        for (int i = g.aps[u]; g.fs[i] != 0; ++i) {
            int v = g.fs[i];
            if (num[v] == 0) {
                strongConnect(v);
                low[u] = std::min(low[u], low[v]);
            } else if (enPile[v]) {
                low[u] = std::min(low[u], num[v]);
            }
        }

        if (low[u] == num[u]) {
            nbCfc++;
            int v;
            do {
                v = pile.top();
                pile.pop();
                enPile[v] = false;
                cfc[v] = nbCfc;
            } while (v != u);
        }
    };

    for (int u = 1; u <= n; ++u) {
        if (num[u] == 0) {
            strongConnect(u);
        }
    }

    std::cout << "Nombre de composantes fortement connexes : " << nbCfc << std::endl;
}

std::vector<std::vector<int>> Algorithms::construireGrapheReduit(const Graphe& g, const std::vector<int>& cfc, int nbCfc) {
    std::vector<std::vector<int>> GR(nbCfc + 1);
    std::vector<std::vector<bool>> deja(nbCfc + 1, std::vector<bool>(nbCfc + 1, false));
    int n = g.aps[0];

    for (int u = 1; u <= n; ++u) {
        for (int i = g.aps[u]; g.fs[i] != 0; ++i) {
            int v = g.fs[i];
            int cu = cfc[u];
            int cv = cfc[v];
            if (cu != cv && !deja[cu][cv]) {
                GR[cu].push_back(cv);
                deja[cu][cv] = true;
            }
        }
    }

    return GR;
}

void Algorithms::afficherGrapheReduit(const std::vector<std::vector<int>>& GR) {
    int nbCfc = GR.size() - 1;
    std::cout << "Graphe reduit (CFC -> CFC) :\n";

    for (int u = 1; u <= nbCfc; ++u) {
        std::cout << "CFC " << u << " -> ";
        if (GR[u].empty()) {
            std::cout << "(aucun successeur)";
        } else {
            for (int v : GR[u]) {
                std::cout << v << " ";
            }
        }
        std::cout << "\n";
    }
}

void Algorithms::afficherBasesReduit(const std::vector<std::vector<int>>& GR) {
    int nbCfc = GR.size() - 1;
    std::vector<int> ddi(nbCfc + 1, 0);

    for (int u = 1; u <= nbCfc; ++u) {
        for (int v : GR[u]) {
            ddi[v]++;
        }
    }

    for (int i = 1; i <= nbCfc; ++i) {
        if (ddi[i] == 0) {
            std::cout << i << " ";
        }
    }
}

void Algorithms::analyserCFC(const Graphe& g) {
    std::vector<int> cfc;
    int nbCfc = 0;

    std::cout << "Determination des composantes fortement connexes...\n";
    tarjan(g, cfc, nbCfc);

    std::cout << "\nSommets par composante :\n";
    for (int i = 1; i <= nbCfc; ++i) {
        std::cout << "CFC " << i << " : ";
        for (int j = 1; j < cfc.size(); ++j) {
            if (cfc[j] == i) std::cout << j << " ";
        }
        std::cout << "\n";
    }

    auto GR = construireGrapheReduit(g, cfc, nbCfc);

    std::cout << "\nGraphe reduit :\n";
    afficherGrapheReduit(GR);

    std::cout << "\nBases (CFC sans predecesseur) : ";
    afficherBasesReduit(GR);
    std::cout << std::endl;
}

void Algorithms::articulationEtIsthmes(const Graphe& g) {
    if (g.estOriente) {
        std::cout << "L'algorithme des points d'articulation et isthmes s'applique uniquement aux graphes NON orientés.\n";
        return;
    }

    int n = g.aps[0];
    std::vector<int> num(n + 1, 0), low(n + 1, 0);
    std::vector<bool> visite(n + 1, false);
    int compteur = 0;

    std::vector<bool> articulation(n + 1, false);
    std::vector<std::pair<int, int>> isthmes;

    std::function<void(int, int)> dfs = [&](int u, int parent) {
        compteur++;
        num[u] = low[u] = compteur;
        int enfants = 0;

        for (int i = g.aps[u]; g.fs[i] != 0; ++i) {
            int v = g.fs[i];
            if (v == parent) continue;

            if (num[v] == 0) {
                enfants++;
                dfs(v, u);
                low[u] = std::min(low[u], low[v]);

                if (low[v] >= num[u] && parent != -1)
                    articulation[u] = true;

                if (low[v] > num[u])
                    isthmes.emplace_back(u, v);

            } else {
                low[u] = std::min(low[u], num[v]);
            }
        }

        if (parent == -1 && enfants > 1)
            articulation[u] = true;
    };

    for (int u = 1; u <= n; ++u) {
        if (num[u] == 0)
            dfs(u, -1);
    }

    std::cout << "\nPoints d'articulation : ";
    for (int i = 1; i <= n; ++i) {
        if (articulation[i]) std::cout << i << " ";
    }

    std::cout << "\nIsthmes : ";
    for (const auto& pair : isthmes) {
    std::cout << "(" << pair.first << ", " << pair.second << ") ";
    }

    std::cout << std::endl;
}



