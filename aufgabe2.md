<h1 style="text-align: center;">Aufgabe 2: Verzinkt</h1>

<p style="text-align: center;">Team-ID: 00968</p>

<p style="text-align: center;">Finn Rudolph</p>

<p style="text-align: center;">21. September 2022</p>

## Lösungsidee

Die zu erzeugenden Bilder ähneln Voronoi-Diagrammen, mit dem Unterschied, dass sich ein Kristall unterschiedlich schnell in die vier Hauptrichtungen ausbreitet. Während das Voronoi-Diagramm einer Menge von Punkten erzeugt werden kann, indem man Kreise von jedem Punkt aus wachsen lässt, bis sie auf andere Kreise treffen, kann ein Kristall erzeugt werden, indem man ein Viereck von jedem Punkt aus wachsen lässt. Seien die Ausbreitungsgeschwindigkeiten (in Pixel pro Zeiteinheit) nach Nord, Süd, Ost und West (im Folgenden $v_n, v_s, v_o, v_w$ gennant) $v_n = 3, v_s = 7, v_o = 2, v_w = 4$. Dann sieht der Kristall nach einer Zeiteinheit wie folgt aus.

![](grafiken/kristall_t1.svg)

_Abbildung 1: Kristall mit Ausbreitungsgeschwindigkeiten $v_n = 3, v_s = 7, v_o = 2, v_w = 4$ nach $t = 1$ Zeiteinheiten._

Da die Ausbreitungsgeschwindigkeiten konstant sind, ist er nach zwei Zeiteinheiten nur eine um Längenfaktor $2$ skalierte Version von sicht selbt. Für das Bild der Ausbreitung mehrerer Kristalle wird angenommen, dass ein Kristall nicht um die Ecke wachsen kann. 

Die Idee des Algorithmus ist es, die Zeit schrittweise zu erhöhen und jeden Kristall um seinen Ursprung zu erweitern. Allerdings muss beim Zusammentreffen zweier Kristalle genau festgestellt werden können, welcher Pixel zu welchem Kristall gehört. Daher werden zu jedem Zeitpunkt $t$ alle Punkte, die im Kristall enthalten sind und bei $t-1$ noch nicht enthalten waren, zu einer Prioritätswarteschlange hinzugefügt. Die Punkte in der Warteschlange sind nach dem Zeitpunkt geordnet, an dem das Kristall, von dem ein Punkt hinzugefügt wurde, diesen gerade berühren würde. Im Folgenden wird ein Element in der Warteschlange auch Ereignis gennannt. Damit können zu jedem Zeitpunkt erst alle neu eingeschlossenen Punkte zur Prioritätswarteschlange hinzugefügt werden, und anschließend diese Ereignisse in der richtigen Zeitfolge verarbeitet werden. Eine andere Möglichkeit wäre natürlich, einfach für jeden Punkt im Bild den Zeitpunkt des Berührens zu berechnen und alle solche Ereignisse nach der Zeitfolge zu verarbeiten. Jedoch beträgt die Zeitkomplexität damit $\Theta(nwh \lg(nwh))$, wenn $n$ die Anzahl der Punkte, $w$ die Breite des Bilds in Pixeln und $h$ die Höhe des Bilds in Pixeln ist. Durch eine Verbesserung kann in vielen Fällen jedoch eine deutlich bessere Laufzeit erzielt werden. (Leider ist die asymptotische Oberschranke jedoch die gleiche.)

Die von einem Kristall eingeschlossenen Punkte können zu jeder Zeiteinheit mithilfe des Abstands von der vertikalen Geraden, die durch den Ursprung eines Kristalls verläuft, ermittelt werden. Der Ursprung des betrachteten Kristalls soll bei $(x_0, y_0)$ liegen. Durch den Zeitpunkt $t$ und die vier Ausbreitungsgeschwindigkeiten $v_n, v_s, v_o, v_w$ ist für jeden Punkt der Geraden $x = x_0$ ein maximaler Abstand in die negative und positive $x$-Richtung definiert, die ein anderer Punkt haben darf, um im Kristall zu liegen. Im Beispiel von oben kann das folgendermaßen visualisiert werden. Für jedes ganzzahlige $y$ werden alle Punkte mit genau dieser $y$-Koordinate, die innerhalb des Kristalls liegen, farblich markiert. Die Endpunkte des entstehenden Segments geben die maximale Distanz an dieser $y$-Koordinate in negative und postitve $x$-Richtung an. Dafür sind einige Beispielwerte in Abbildung 2 angegeben.

![](grafiken/distanzgrenze.svg)

_Abbildung 2: Visualisierung der maximalen $x$-Distanz eines Punkts von der Geraden $y = x_0$, um im Kristall zu liegen (für jedes ganzzahlige $y$)._

Allgemein kann die maximale $x$-Distanz einfach über die $y$-Koordinate des Punkts und die Steigung des relevanten Liniensegments berechnet werden. Für einen Punkt nordöstlich des Ursprungs des Kristalls ist beispielsweise nur das Liniensegment zwischen der nördlichen und östlichen Spitze des Kristalls relevant. Seien $s_{no}, s_{nw}, s_{so}, s_{sw}$ die Beträge der Steigungen des nordöstlichen, nordwestlichen, südöstlichen und südwestlichen Liniensegments. Dann ist die maximale $x$-Distanz nach Osten eines Punkts mit $y = j$ bei Zeitpuntkt $t$ (angenommen der Kristall startet bei $t = 0$) 
$$
d_\max = \begin{cases}
v_o \cdot t - |j - y_0| \cdot s_{so} & \quad \text{if } j < y_0 \\
v_o \cdot t - |j - y_0| \cdot s_{no} & \quad \text{else}
\end{cases}
$$
Nach Westen verhält es sich ähnlich. An Zeitpunkt $t$ muss also für jedes $y: v_s \cdot t \le v_n \cdot t$ mithilfe der genannten Beziehung für den maximalen $x$-Abstand jeder Punkt zur Warteschlange hinzugefügt werden, der bei $t-1$ noch ausßerhalb des maximalen $x$-Abstands war. Um den $x$-Abstand bei $t-1$ sofort zur Verfügung zu haben, werden für jeden Punkt zwei Arrays $\text{ost}$ und $\text{west}$ der Länge $h$ angelegt. Außerdem wird das Wachstum eines Kristalls ab einem bestimmten Zeitpunkt an einer $y$-Koordinate nicht mehr möglich sein - z. B. wenn er an den Bildrand oder einen anderen Kristall stößt. Auch diese Information wird in den Arrays abgespeichert. 

## Laufzeitanalyse

Die Oberschranke der Laufzeit ist, wie bereits genannt $O(nwh \lg(nwh))$. Die Unterschranke beträgt mit dem verbesserten Ansatz jedoch $\Omega(wh)$, da jeder Pixel mindestens einmal, aber nicht zwingend öfter, betrachtet werden muss. 

Insgesamt betrachtet ist der verbesserte Ansatz ähnlich zur $\Theta(nwh \lg(nwh))$ Lösung. Indem nicht alle Ereignisse auf einmal hinzugefügt werden, wird meistens nicht von jedem Ursprungspunkt aus jeder Pixel betrachtet, da bei Kollision mit einem zweiten Kristall frühzeitig gestoppt wird.

## Implementierung

Das Programm wird in C++ geschrieben, zur Erstellung von PNG-Bildern wird die Bibliothek LodePNG (siehe Quellen) verwendet.

Um die Simulation einfach anpassbar zu machen, können Ursprungskoordinaten, Ausbreitungsgeschwindigkeiten, Startzeit und Farbe jedes Kristalls angegeben werden. Nicht angegebene Parameter werden zufällig gewählt. Auch die Abmessungen des Bilds können angegeben werden, standardmäßig werden sie auf $1920 \times 1080$ gesetzt. Falls die Ursprungskoordinaten der Punkte angegeben werden, werden die Abmessungen so gewählt, dass jeder Punkt mindestens $50$ Pixel Abstand zum unteren und rechten Ende hat. Der Ursprung des Koordinatensystems ist in der oberen linken Ecke, die $y$-Achse verläuft ansteigend nach unten. Um auszuwählen, welche der genannten Größen manuell gesetzt werden sollen, können folgende Flags beim Ausführen des Programms in der Kommandozeile mitgegeben werden.

| Flag | Parameter                       |
| ---- | ------------------------------- |
| `-p` | Koordinaten der Ursprungspunkte |
| `-v` | Ausbreitungsgeschwindigkeiten   |
| `-t` | Startzeiten                     |
| `-c` | Farben                          |
| `-d` | Abmessungen des Bilds           |

Für jede der ersten vier Flags, die gesetzt ist, müssen dann jeweils $n$ Werte gegeben werden. Auch kann der Name des Ergebnisbilds als Argument in der Kommandozeile angegeben werden.

Der erste Teil der `main`-Funktion (Z. 167-270) kümmert sich nur um das Einlesen der Parameter bzw. zufällige Generieren. Da das nicht der Kern der Aufgabe ist, wird dieser Teil nicht erklärt. Auch die Funktionen in den Zeilen 53-98 gehören dazu.

Zunächst werden die für die Simulation nötigen Datenstrukturen angelegt (Z. 272-282). `res` enthält das Ergebnis der Simulation und wird danach an LodePNG gegeben. `diagram` dient dazu, schnell überprüfen zu können, ob ein Pixel zu einem Kristall gehört und zu welchem. `east` und `west` enthalten für jeden Ursprung die bereits angesprochenen Arrays, in denen der letzte nach Osten bzw. Westen eingeschlossene $x$-Wert für jeden $y$-Wert für jeden Kristall steht. `slopes` dient lediglich dazu, die Steigungen aller Liniensegmente nicht ständig neu berechnen zu müssen. `east` und `west` werden inital mit $x_0$ bzw. $x_0 -1$ befüllt, sodass `east` die Punkte auf $x = x_0$ mit einschließt (Z. 291-292).

Die folgende `for`-Schleife enthält die eigentliche Simulation (Z. 284-332). Bis alle Pixel in einem Kristall sind, wird an jedem Zeitpunkt für jeden bereits gestarteten Ursprung zunächst die aktuellen Ausbreitung in die vier Hauptrichtungen berechnet (Z. 305-309). Da das Einfügen von Ereignissen nach Ost und West ähnlich ist, wird es in die Funktion `update_queue` verschoben, um Codewiederholung zu vermeiden. Unter ihren (sehr zahlreichen) Parametern sind viele selbsterklärend, nur einige sollen kurz erläutert werden: `last` ist entweder `east[i]` oder `west[i]`, `a` ist die $y$-Koordinate der südlichen und `b` die der nördlichen Spitze des Kristalls. `u` ist die `vo` bzw. `vw` (siehe Z. 308-309) und `v` ist die Ausbreitungsgeschwindigkeit in Ost- bzw. Westrichtung. In `update_queue` wird zunächst überprüft, ob der Kristall nördlich oder südlich gegen einen anderen Kristall gestoßen ist (Z. 115-129). Ist das der Fall, werden in der entsprechenden Richtung alle Einträge auf `-1` gesetzt. Danach werden für jedes $j : a \le j \le b$ alle neu hinzugekommenen Punkte in die Warteschlange eingefügt. Zunächst werden die $x$-Grenzen der zu betrachteten Punkte ermittelt: Das Ende kann durch oben genannte Beziehung errechnet werden, der Anfang ist noch in `last` gespeichert. Die anschließende `for`-Schleife iteriert von der Geraden $x = x_0$ weg über alle einzufügenden $x$-Koordinaten (Z. 143-159). Es werden zwei Fälle unterschieden: Ist der Pixel noch frei, wird das als Ereignis hinzugefügt, andernfalls die Iteration abgebrochen, da der Kristall gegen einen anderen gestoßen ist. Zuletzt wird der zuletzt betrachtete $x$-Wert aktualisiert oder auf `-1` gesetzt, falls ein anderer Kristall entdeckt wurde. Der Ausdruck zur Errechnung des Zeitpunkts des Zusammenstoßes (Z. 147-150) ist leicht nachvollziehbar, in folgender Zeichnung  wird die Idee veranschaulicht.

// Bild Zeitformel

Nachdem alle Kristallursprünge für den aktuellen Zeitpunkt verarbeitet wurde, wird die Prioritätswarteschlange geleert, sodass alle Ereignisse in richtiger Zeitfolge betrachtet werden (Z. 320-331). Wenn ein Pixel noch frei ist, wird die Farbe seines Kristalls in `res` gesetzt und in `diagram` an dieser Stelle der Index des Kristalls eingetragen. `set_color` findet sich in Z. 100-105, es setzt den R, G und B-Wert des Pixels auf den gleichen Wert, sodass ein Grauton entsteht. Der A-Wert (Alpha-Wert oder Transparenz) wird auf das Maximum gesetzt, da das Bild nicht transparent sein soll.

## Beispiele

## Quellcode

```cpp
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
    double slope_down, double slope_up, int start_time, size_t i, bool west)
{
    // Falls sich der Kristall nach Norden / Süden ab einem y-Wert noch nicht
    // auf der Geraden x = x_0 ausgebreitet hat und dort bereits ein anderer
    // Kristall ist, kann er sich nicht weiter nach Norden / Süden ausbreiten.
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

        if (west)
            xend = 2 * x - xend;

        bool can_continue = 1;
        for (int k = xbegin; (west ? k >= xend : k <= xend) && k < width && k >= 0; k += (west ? -1 : 1))
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
                // Der Kristall ist gegen einen anderen gestoßen.
                can_continue = 0;
                break;
            }
        }

        last[j] = can_continue ? (west ? xend - 1 : xend + 1) : -1;
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

    // Das Ergebnisbild RGBA-Werten für jeden Pixel.
    std::vector<uint8_t> res(width * height * 4, 0);
    // Der Index des Kristalls, dem der Pixel zugehörig ist (oder SIZE_MAX).
    std::vector<std::vector<size_t>> diagram(width, std::vector<size_t>(height, -1));
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
                int vn = velocities[i].n * (t - times[i]),
                    vs = velocities[i].s * (t - times[i]),
                    vo = velocities[i].o * (t - times[i]),
                    vw = velocities[i].w * (t - times[i]);

                update_queue(q, east[i], diagram, width, height,
                             y - vs, y + vn, x, y, vo, velocities[i].o, colors[i],
                             slopes[i].so, slopes[i].no, times[i], i, 0);
                update_queue(q, west[i], diagram, width, height,
                             y - vs, y + vn, x, y, vw, velocities[i].w, colors[i],
                             slopes[i].sw, slopes[i].nw, times[i], i, 1);
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
```

## Quellen

- LodePNG. https://github.com/lvandeve/lodepng