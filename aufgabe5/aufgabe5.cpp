#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

void get_reachable(
    std::vector<std::vector<size_t>> const &g,
    size_t u,
    std::vector<std::vector<bool>> &reachable,
    std::vector<std::vector<size_t>> &pre)
{
    reachable[u][0] = 1;
    std::queue<std::pair<size_t, size_t>> q;
    q.push({u, 0});

    while (!q.empty())
    {
        auto [x, dis] = q.front();
        q.pop();

        // Markiere noch nicht erreichte Nachbarn als erreichbar.
        for (size_t y : g[x])
            if (dis + 1 < reachable[0].size() && !reachable[y][dis + 1])
            {
                reachable[y][dis + 1] = 1;
                pre[y][dis + 1] = x;
                q.push({y, dis + 1});
            }
    }
}

int main()
{
    size_t n, m;
    std::cin >> n >> m;

    std::vector<std::vector<size_t>> g(n);
    for (size_t i = 0; i < m; i++)
    {
        size_t a, b;
        std::cin >> a >> b;
        g[a - 1].push_back(b - 1);
    }

    // Enthält bei [u][i], ob Vertex u von Vertex 0 bzw. 1 mit i Sprüngen
    // erreichbar ist.
    std::vector<std::vector<bool>> r1(n, std::vector<bool>(n * n, 0)),
        r2(n, std::vector<bool>(n * n, 0));
    // Enthält den Vorgänger von u nach i Sprüngen bei [u][i].
    std::vector<std::vector<size_t>> pre1(n, std::vector<size_t>(n * n, -1)),
        pre2(n, std::vector<size_t>(n * n, -1));

    get_reachable(g, 0, r1, pre1);
    get_reachable(g, 1, r2, pre2);

    for (size_t i = 0; i < n * n; i++)
        for (size_t u = 0; u < n; u++)
            if (r1[u][i] && r2[u][i])
            {
                std::cout << "benötigte Sprünge: " << i << "\n\n";

                // Rekonstruktion des Pfads von Vertex 0 bzw. 1.
                std::vector<size_t> path1, path2;
                size_t x = u, y = u;
                while (x != SIZE_MAX)
                {
                    path1.push_back(x);
                    path2.push_back(y);
                    x = pre1[x][i];
                    y = pre2[y][i];
                    i--;
                }

                std::cout << "Sashas Pfad:\n";
                for (auto it = path1.rbegin(); it != path1.rend(); it++)
                    std::cout << (*it + 1) << ' ';
                std::cout << "\n\nMikas Pfad:\n";
                for (auto it = path2.rbegin(); it != path2.rend(); it++)
                    std::cout << (*it + 1) << ' ';
                std::cout << '\n';

                return 0;
            }

    std::cout << "Absolvieren des Parcours nicht möglich.\n";
}