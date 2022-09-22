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

    // Vergleicht nur die y-Koordinate.
    bool operator<(point const &p) const
    {
        return y < p.y;
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

std::vector<int> gen_times(size_t n, int width, int height)
{
    std::vector<int> times(n);
    for (int &t : times)
        t = rand_range(0, (width + height) / 8);
    return times;
}

void update_queue(
    std::priority_queue<event> &q,
    std::set<point> &last,
    std::vector<std::vector<bool>> &finished,
    int width, int height,
    int a, int b, int y, int u, int v, uint8_t c,
    double slope_down, double slope_up)
{
    for (int j = std::max(0, a); j <= b && j < height; j++)
    {
        int newx = u - (double)abs(j - y) *
                           (j < y ? slope_down : slope_up);
        auto it = last.find({0, y});
        int xbegin = it != last.end() ? it->x : 0;

        bool can_continue = 1;
        for (int k = std::max(0, xbegin); k <= newx && k < width; k++)
        {
            if (!finished[k][j])
            {
                double t = ((double)k +
                            (double)abs(j - y) *
                                (j < y ? slope_down : slope_up)) /
                           (double)v;
                q.push({{k, j}, t, c});
            }
            else
            {
                can_continue = 0;
                break;
            }
        }

        last.erase(it);
        if (can_continue)
            last.insert({newx, j});
    }
}

int main(int argc, char **argv)
{
    char fname[40] = "kristallmuster.png";
    unsigned op = 0;

    for (size_t i = 1; i < argc; i++)
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

    if (op & DIMENSIONS)
    {
        std::cout << "Dimensionen [Höhe Breite]:\n";
        std::cin >> height >> width;
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
        times = gen_times(n, width, height);

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

    std::vector<std::vector<uint8_t>> res(width, std::vector<uint8_t>(height));
    std::vector<std::vector<bool>> finished(width, std::vector<bool>(height, 0));
    // Enthält für jedes y den Punkt, der bei der Ausbreitung nach links bzw.
    // rechts zuletzt hinzugefügt wurde. Falls die Ausbreitung für ein y nicht
    // mehr möglich ist, ist für dieses auch kein Punkt vorhanden.
    std::vector<std::set<point>> left(n), right(n);
    // Die absoluten Steigungen der Seiten des Vierecks relativ zur y-Achse.
    std::vector<slope> slopes(n);

    for (size_t i = 0; i < n; i++)
    {
        slopes[i].nw = (double)velocities[i].w / (double)velocities[i].n;
        slopes[i].sw = (double)velocities[i].w / (double)velocities[i].s;
        slopes[i].no = (double)velocities[i].o / (double)velocities[i].n;
        slopes[i].nw = (double)velocities[i].o / (double)velocities[i].s;

        left[i].insert(points[i]);
        right[i].insert(points[i]);
    }

    unsigned finished_pixels = 0;
    for (int t = 1; finished_pixels < width * height; t++)
    {
        std::priority_queue<event> q;

        for (size_t i = 0; i < n; i++)
        {
            auto [x, y] = points[i];
            int n = velocities[i].n * t,
                s = velocities[i].s * t,
                o = velocities[i].o * t,
                w = velocities[i].w * t;

            update_queue(q, left[i], finished, width, height,
                         s, n, y, o, o / t, colors[i],
                         (double)o / double(s), (double)o / (double)n);
            update_queue(q, right[i], finished, width, height,
                         s, n, y, w, w / t, colors[i],
                         (double)w / (double)s, (double)w / (double)n);
        }
    }
}