#include "lodepng.h"

#include <memory.h>
#include <iostream>
#include <queue>
#include <climits>
#include <vector>
#include <set>

enum options
{
    POINTS = 1,     // -p
    VELOCITY = 2,   // -v
    TIME = 4,       // -t
    COLOR = 8,      // -c
    DIMENSIONS = 16 // -d
};

struct point
{
    int x, y;

    size_t get_index(int width)
    {
        return (y * width + x) * 4;
    }
};

struct vel
{
    int n, s, o, w;
};

struct slope
{
    double nw, sw, no, so;
};

struct event
{
    point p;
    double t;
    size_t i;
    uint8_t c;

    bool operator<(event const &e) const
    {
        return t > e.t;
    }
};

// Returns a random integer in [min, max)
inline int rand_range(int min, int max)
{
    return (std::rand() % (max - min)) + min;
}

std::vector<uint8_t> gen_colors(size_t n)
{
    std::vector<uint8_t> colors(n);
    for (uint8_t &c : colors)
        c = rand_range(0, 255);
    return colors;
}

std::vector<point> gen_points(size_t n, int width, int height)
{
    std::vector<point> points(n);
    for (point &p : points)
    {
        p.x = rand_range(0, width);
        p.y = rand_range(0, height);
    }
    return points;
}

std::vector<vel> gen_velocities(size_t n)
{
    int const minv = 1, maxv = 10;
    std::vector<vel> velocities(n);
    for (vel &v : velocities)
    {
        v.n = rand_range(minv, maxv);
        v.s = rand_range(minv, maxv);
        v.o = rand_range(minv, maxv);
        v.w = rand_range(minv, maxv);
    }
    return velocities;
}

std::vector<int> gen_times(size_t n, int width)
{
    std::vector<int> times(n);
    for (int &t : times)
        t = rand_range(0, width / 64);
    return times;
}

inline void set_color(std::vector<uint8_t> &res, int width, point p, uint8_t c)
{
    for (size_t i = 0; i < 3; i++)
        res[p.get_index(width) + i] = c;
    res[p.get_index(width) + 3] = 255;
}

void update_queue(
    std::priority_queue<event> &q,
    std::vector<int> &last,
    std::vector<std::vector<size_t>> &diagram,
    int width, int height,
    int a, int b, int x, int y, int u, int v, uint8_t c,
    double slope_down, double slope_up, int start_time, size_t i, bool left)
{
    for (int j = y; j <= b && j < height; j++)
    {
        if (diagram[x][j] != SIZE_MAX && diagram[x][j] != i && last[j] != -1)
            while (j < height && last[j] != -1)
                last[j++] = -1;
    }

    for (int j = y - 1; j >= a && j >= 0; j--)
    {
        if (diagram[x][j] != SIZE_MAX && diagram[x][j] != i && last[j] != -1)
            while (j >= 0 && last[j] != -1)
                last[j--] = -1;
    }

    for (int j = std::max(0, a); j <= b && j < height; j++)
    {
        int xend = x + u - (double)abs(j - y) * (j < y ? slope_down : slope_up);
        int xbegin = last[j];

        if (xbegin < 0 || xbegin >= width)
            continue;

        if (left)
            xend = 2 * x - xend;

        bool can_continue = 1;
        for (int k = xbegin; (left ? k >= xend : k <= xend) && k < width && k >= 0; k += (left ? -1 : 1))
        {
            if (diagram[k][j] == SIZE_MAX)
            {
                double t = ((double)abs(k - x) +
                            (double)abs(j - y) *
                                (j < y ? slope_down : slope_up)) /
                           (double)v;
                q.push({{k, j}, t + (double)start_time, i, c});
            }
            else
            {
                can_continue = 0;
                break;
            }
        }

        last[j] = can_continue ? (left ? xend - 1 : xend + 1) : -1;
    }
}

int main(int argc, char **argv)
{
    char fname[40] = "kristallmuster.png";
    unsigned op = 0;

    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-p"))
            op |= POINTS;
        else if (!strcmp(argv[i], "-v"))
            op |= VELOCITY;
        else if (!strcmp(argv[i], "-t"))
            op |= TIME;
        else if (!strcmp(argv[i], "-c"))
            op |= COLOR;
        else if (!strcmp(argv[i], "-d"))
            op |= DIMENSIONS;
        else
            strcpy(fname, argv[i]);

    std::vector<point> points;
    std::vector<vel> velocities;
    std::vector<int> times;
    std::vector<uint8_t> colors;
    int width = 0, height = 0;

    size_t n;
    std::cout << "Anzahl an Punkten: ";
    std::cin >> n;

    srand(time(0));

    if (op & DIMENSIONS)
    {
        std::cout << "Dimensionen [Breite Höhe]:\n";
        std::cin >> width >> height;
        if (height > width)
            std::swap(height, width);
    }

    if (op & POINTS)
    {
        std::cout << "Koordinaten der Punkte [x y]:\n";
        int max_x = 0, max_y = 0;
        for (size_t i = 0; i < n; i++)
        {
            int x, y;
            std::cin >> x >> y;
            points.push_back({x, y});
            max_x = std::max(max_x, x);
            max_y = std::max(max_y, y);
        }

        if (!(op & DIMENSIONS))
        {
            width = max_x + 50;
            height = max_y + 50;
        }
    }
    else
    {
        if (!(op & DIMENSIONS))
        {
            width = 1920;
            height = 1080;
        }
        points = gen_points(n, width, height);
    }

    if (op & VELOCITY)
    {
        std::cout << "Geschwindigkeiten in Nord- / Süd- / Ost- / West-Richtung [N S O W]:\n";
        for (size_t i = 0; i < n; i++)
        {
            int n, s, o, w;
            std::cin >> n >> s >> o >> w;
            velocities.push_back({n, s, o, w});
        }
    }
    else
        velocities = gen_velocities(n);

    if (op & TIME)
    {
        std::cout << "Startzeitpunkte [t]:\n";
        for (size_t i = 0; i < n; i++)
        {
            int t;
            std::cin >> t;
            times.push_back(t);
        }
    }
    else
        times = gen_times(n, width);

    if (op & COLOR)
    {
        std::cout << "Farben als Ganzzahlen zwischen 0 und 255 [c]:\n";
        for (size_t i = 0; i < n; i++)
        {
            uint8_t c;
            std::cin >> c;
            colors.push_back(c);
        }
    }
    else
        colors = gen_colors(n);

    std::vector<uint8_t> res(width * height * 4, 0);
    std::vector<std::vector<size_t>> diagram(width, std::vector<size_t>(height, -1));
    // Enthält für jedes y den Punkt, der bei der Ausbreitung nach links bzw.
    // rechts zuletzt hinzugefügt wurde. Falls die Ausbreitung für ein y nicht
    // mehr möglich ist, ist für dieses auch kein Punkt vorhanden.
    std::vector<std::vector<int>> left(n, std::vector<int>(height)),
        right(n, std::vector<int>(height));
    // Die absoluten Steigungen der Seiten des Vierecks relativ zur y-Achse.
    std::vector<slope> slopes(n);

    for (size_t i = 0; i < n; i++)
    {
        slopes[i].nw = (double)velocities[i].w / (double)velocities[i].n;
        slopes[i].sw = (double)velocities[i].w / (double)velocities[i].s;
        slopes[i].no = (double)velocities[i].o / (double)velocities[i].n;
        slopes[i].so = (double)velocities[i].o / (double)velocities[i].s;

        std::fill(left[i].begin(), left[i].end(), points[i].x);
        std::fill(right[i].begin(), right[i].end(), points[i].x + 1);
    }

    int finished_pixels = 0;

    for (int t = 0; finished_pixels < width * height; t++)
    {
        std::priority_queue<event> q;

        for (size_t i = 0; i < n; i++)
        {
            if (times[i] < t)
            {
                auto [x, y] = points[i];
                int vn = velocities[i].n * (t - times[i]),
                    vs = velocities[i].s * (t - times[i]),
                    vo = velocities[i].o * (t - times[i]),
                    vw = velocities[i].w * (t - times[i]);

                update_queue(q, left[i], diagram, width, height,
                             y - vs, y + vn, x, y, vo, velocities[i].o, colors[i],
                             slopes[i].so, slopes[i].no, times[i], i, 1);
                update_queue(q, right[i], diagram, width, height,
                             y - vs, y + vn, x, y, vw, velocities[i].w, colors[i],
                             slopes[i].sw, slopes[i].nw, times[i], i, 0);
            }
        }

        while (!q.empty())
        {
            auto [p, curr_time, i, c] = q.top();
            q.pop();

            if (diagram[p.x][p.y] == SIZE_MAX)
            {
                set_color(res, width, p, c);
                diagram[p.x][p.y] = i;
                finished_pixels++;
            }
        }
    }

    lodepng::encode(fname, res, width, height);
}
