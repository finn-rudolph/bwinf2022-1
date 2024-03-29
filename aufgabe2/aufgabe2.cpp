#include "lodepng.h"

#include <memory.h>
#include <iostream>
#include <queue>
#include <climits>
#include <vector>
#include <set>
#include <cmath>

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

    // Gibt den Index im Ergebnisbild zurück.
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

// Gibt eine zufällige Ganzzahl in [min, max) zurück.
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
    int const v_min = 1, v_max = 10;
    std::vector<vel> velocities(n);
    for (vel &v : velocities)
    {
        v.n = rand_range(v_min, v_max);
        v.s = rand_range(v_min, v_max);
        v.o = rand_range(v_min, v_max);
        v.w = rand_range(v_min, v_max);
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
    double slope_down, double slope_up, int start_time, size_t i, bool west)
{
    for (int j = std::max(0, a); j <= b && j < height; j++)
    {
        double d = (double)abs(j - y) * (j < y ? slope_down : slope_up);
        if (abs(d - std::round(d)) < 1e-6)
            d = std::round(d);
        else
            d = std::ceil(d);

        int xend = x + u - d;
        int xbegin = last[j];

        if ((!west && xbegin >= width) || (west && xbegin < 0) ||
            xbegin == INT_MIN)
            continue;

        if (west)
            xend = 2 * x - xend;

        bool can_continue = 1;
        for (int k = xbegin;
             (west ? k >= xend : k <= xend) && k < width && k >= 0;
             k += (west ? -1 : 1))
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
                // Der Kristall ist seitlich gegen einen anderen gestoßen.
                can_continue = 0;
                break;
            }
        }

        last[j] = can_continue ? (west ? xend - 1 : xend + 1) : INT_MIN;
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
        std::cout << "Dimensionen des Bilds [Breite Höhe]:\n";
        std::cin >> width >> height;
        if (height > width)
            std::swap(height, width);
    }

    if (op & POINTS)
    {
        std::cout << "Koordinaten der Punkte [x y]:\n";
        int x_max = 0, y_max = 0;
        for (size_t i = 0; i < n; i++)
        {
            int x, y;
            std::cin >> x >> y;
            points.push_back({x, y});
            x_max = std::max(x_max, x);
            y_max = std::max(y_max, y);
        }

        if (!(op & DIMENSIONS))
        {
            width = x_max + 50;
            height = y_max + 50;
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
        std::cout << "Geschwindigkeiten in Nord- / Süd- / Ost- / West-Richtung "
                     "[N S O W]:\n";
        for (size_t i = 0; i < n; i++)
        {
            int v_n, v_s, v_o, v_w;
            std::cin >> v_n >> v_s >> v_o >> v_w;
            velocities.push_back({v_n, v_s, v_o, v_w});
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
            int c;
            std::cin >> c;
            colors.push_back(c);
        }
    }
    else
        colors = gen_colors(n);

    // Das Ergebnisbild mit RGBA-Werten für jeden Pixel.
    std::vector<uint8_t> res(width * height * 4, 0);
    // Der Index des Kristalls, dem der Pixel zugehörig ist (oder SIZE_MAX).
    std::vector<std::vector<size_t>>
        diagram(width, std::vector<size_t>(height, SIZE_MAX));
    // Enthält für jedes y den Punkt, der bei der Ausbreitung nach Ost bzw.
    // West zuletzt hinzugefügt wurde. Falls die Ausbreitung für ein y nicht
    // mehr möglich ist, steht an der Stelle -1.
    std::vector<std::vector<int>> east(n, std::vector<int>(height)),
        west(n, std::vector<int>(height));
    // Die Beträge der Steigungen der Seiten des Vierecks relativ zur y-Achse.
    std::vector<slope> slopes(n);

    for (size_t i = 0; i < n; i++)
    {
        slopes[i].nw = (double)velocities[i].w / (double)velocities[i].n;
        slopes[i].sw = (double)velocities[i].w / (double)velocities[i].s;
        slopes[i].no = (double)velocities[i].o / (double)velocities[i].n;
        slopes[i].so = (double)velocities[i].o / (double)velocities[i].s;

        std::fill(east[i].begin(), east[i].end(), points[i].x);
        std::fill(west[i].begin(), west[i].end(), points[i].x - 1);
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
                int v_n = velocities[i].n * (t - times[i]),
                    v_s = velocities[i].s * (t - times[i]),
                    v_o = velocities[i].o * (t - times[i]),
                    v_w = velocities[i].w * (t - times[i]);

                update_queue(q, east[i], diagram, width, height,
                             y - v_s, y + v_n, x, y, v_o, velocities[i].o,
                             colors[i], slopes[i].so, slopes[i].no, times[i],
                             i, 0);
                update_queue(q, west[i], diagram, width, height,
                             y - v_s, y + v_n, x, y, v_w, velocities[i].w,
                             colors[i], slopes[i].sw, slopes[i].nw, times[i],
                             i, 1);
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