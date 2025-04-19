#include "algorithms.h"
#include <queue>
#include <limits>
#include <iostream>
#include <stack>
#include <functional>
#include <QVector>

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
    cfc.assign(n + 1, 0); // cfc[i] : composante de sommet i
    std::vector<int> num(n + 1, 0), low(n + 1, 0);
    std::vector<bool> enPile(n + 1, false);
    std::stack<int> pile;

    int compteur = 0;
    nbCfc = 0;

    std::function<void(int)> dfs = [&](int u) {
        num[u] = low[u] = ++compteur;
        pile.push(u);
        enPile[u] = true;

        for (int i = g.aps[u]; g.fs[i] != 0; ++i) {
            int v = g.fs[i];
            if (num[v] == 0) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);
            } else if (enPile[v]) {
                low[u] = std::min(low[u], num[v]);
            }
        }

        if (low[u] == num[u]) {
            nbCfc++;
            int v;
            do {
                v = pile.top(); pile.pop();
                enPile[v] = false;
                cfc[v] = nbCfc;
            } while (v != u);
        }
    };

    for (int u = 1; u <= n; ++u) {
        if (num[u] == 0)
            dfs(u);
    }
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

void Algorithms::articulationsEtIsthmes(const Graphe& g, std::vector<int>& points, std::vector<std::pair<int, int>>& isthmes) {
    if (g.estOriente) return;

    int n = g.aps[0];
    std::vector<int> num(n + 1, 0), low(n + 1, 0);
    int compteur = 0;

    std::vector<bool> articulation(n + 1, false);

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

    for (int i = 1; i <= n; ++i) {
        if (articulation[i]) points.push_back(i);
    }
}

std::vector<int> Algorithms::dijkstra(const GrapheValue& g, int source, std::vector<int>& pred) {
    const int INF = INT_MAX;
    int n = g.aps[0];

    std::vector<int> dist(n + 1, INF);
    std::vector<bool> visite(n + 1, false);
    pred.resize(n + 1, -1);

    dist[source] = 0;

    for (int i = 1; i <= n; ++i) {
        // Trouver le sommet non visité avec la plus petite distance
        int u = -1;
        int minDist = INF;
        for (int j = 1; j <= n; ++j) {
            if (!visite[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;
        visite[u] = true;

        // Parcourir ses successeurs
        const Sommet* sommetU = g.trouverSommet(u);
        Successeur* s = sommetU ? sommetU->fileSucesseur : nullptr;
        while (s) {
            int v = s->sommet->id;
            int poids = s->poids;

            if (dist[u] != INF && dist[u] + poids < dist[v]) {
                dist[v] = dist[u] + poids;
                pred[v] = u;
            }

            s = s->successeurSuivant;
        }
    }

    return dist;
}

std::vector<std::vector<int>> Algorithms::dantzig(const GrapheValue& g) {
    const int INF = INT_MAX;
    int n = g.aps[0];
    std::vector<std::vector<int>> dist(n + 1, std::vector<int>(n + 1, INF));

    // Distance à soi-même = 0
    for (int i = 1; i <= n; ++i)
        dist[i][i] = 0;

    // Remplir les poids depuis la liste dynamique
    for (int u = 1; u <= n; ++u) {
        const Sommet* sommetU = g.trouverSommet(u);
        Successeur* s = sommetU->fileSucesseur;
        while (s) {
            int v = s->sommet->id;
            dist[u][v] = s->poids;

            // Ajouter l’arc inverse si non orienté
            if (!g.estOriente)
                dist[v][u] = s->poids;

            s = s->successeurSuivant;
        }
    }

    for (int k = 1; k <= n; ++k) {
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    // Détection de cycle négatif
    for (int i = 1; i <= n; ++i) {
        if (dist[i][i] < 0)
            throw std::runtime_error("Cycle de poids négatif détecté !");
    }

    return dist;
}

std::vector<std::tuple<int, int, int>> Algorithms::kruskal(const GrapheValue& g) {
    if (g.estOriente) {
        throw std::runtime_error("Kruskal ne s'applique que sur des graphes non orientés.");
    }

    int n = g.aps[0];
    std::vector<std::tuple<int, int, int>> arcs;

    // 1. Récupérer tous les arcs pondérés
    for (int u = 1; u <= n; ++u) {
        const Sommet* sommetU = g.trouverSommet(u);
        Successeur* s = sommetU->fileSucesseur;
        while (s) {
            int v = s->sommet->id;
            int poids = s->poids;

            if (u < v) // éviter les doublons
                arcs.emplace_back(u, v, poids);

            s = s->successeurSuivant;
        }
    }

    // 2. Trier les arcs par poids
    std::sort(arcs.begin(), arcs.end(), [](auto& a, auto& b) {
        return std::get<2>(a) < std::get<2>(b); // tri par poids
    });

    // 3. Initialiser Union-Find
    std::vector<int> parent(n + 1);
    for (int i = 1; i <= n; ++i)
        parent[i] = i;

    auto find = [&](int x) {
        while (x != parent[x])
            x = parent[x];
        return x;
    };

    auto unite = [&](int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        parent[rx] = ry;
        return true;
    };

    // 4. Ajouter les arcs sans créer de cycles
    std::vector<std::tuple<int, int, int>> arbre;

    for (auto& [u, v, poids] : arcs) {
        if (unite(u, v)) {
            arbre.emplace_back(u, v, poids);
            if (arbre.size() == n - 1) break;
        }
    }

    return arbre;
}

