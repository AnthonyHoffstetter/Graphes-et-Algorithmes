#include "algorithms.h"
#include <queue>
#include <limits>
#include <iostream>

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
