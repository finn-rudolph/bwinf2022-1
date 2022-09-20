#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

typedef std::vector<std::vector<bool>> bmatrix;
typedef std::vector<std::vector<size_t>> matrix;

void get_reachable(std::vector<std::vector<size_t>> const &g, size_t u,
                   bmatrix &reachable, matrix &pre)
{
    reachable[u][0] = 1;
    std::queue<std::pair<size_t, size_t>> q;
    q.push({u, 0});

    while (!q.empty())
    {
        size_t x = q.front().first;
        size_t dis = q.front().second;
        q.pop();

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

    size_t const dis_lim = ((n * n) + 1) / 2;
    bmatrix r1(n, std::vector<bool>(dis_lim, 0));
    bmatrix r2(n, std::vector<bool>(dis_lim, 0));
    matrix pre1(n, std::vector<size_t>(dis_lim, SIZE_MAX));
    matrix pre2(n, std::vector<size_t>(dis_lim, SIZE_MAX));

    get_reachable(g, 0, r1, pre1);
    get_reachable(g, 1, r2, pre2);

    for (size_t i = 0; i < dis_lim; i++)
        for (size_t u = 0; u < n; u++)
            if (r1[u][i] && r2[u][i])
            {
                std::cout << "benötigte Sprünge: " << i << "\n\n";

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