#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string.h>

struct task
{
    int64_t start, len, completion;
    size_t i;
};

inline int64_t priority(task const &t, int64_t a, int64_t b)
{
    return a * t.start + b * t.len;
}

struct task_priority_comp
{
    int64_t a, b;
    task_priority_comp(int64_t a_, int64_t b_)
    {
        a = a_;
        b = b_;
    }

    bool operator()(task const &x, task const &y) const
    {
        return priority(x, a, b) < priority(y, a, b);
    }
};

void print_result(std::vector<task> const &tasks)
{
    long double mean_time = 0.0, squares_mean = 0.0;
    int64_t max_time = 0;
    for (task const &t : tasks)
    {
        mean_time += (t.completion - t.start);
        max_time = std::max(max_time, t.completion - t.start);
        squares_mean += (t.completion - t.start) * (t.completion - t.start);
    }
    mean_time /= (long double)tasks.size();
    squares_mean /= (long double)tasks.size();

    std::cout << "Mittlere Wartezeit: " << mean_time << '\n'
              << "Maximale Wartezeit: " << max_time << '\n'
              << "Mittelwert der Quadrate: " << squares_mean << '\n';
}

inline int64_t minutes(int64_t hours)
{
    return hours * 60;
}

inline int64_t day_start(int64_t m)
{
    return (m / minutes(24)) * minutes(24);
}

// Aktualisiert die Abschlusszeit des Auftrags und gibt die neue aktuelle Zeit
// zurück.
inline int64_t do_task(task &t, int64_t curr_time)
{
    // Fülle den aktuellen Arbeitstag auf, falls der Auftrag so lang ist.
    t.completion = std::min(curr_time + t.len, (day_start(curr_time) + minutes(17)));
    t.len -= (t.completion - curr_time);

    if (t.len)
    {
        // Füge auch die Zeit der Nächte hinzu, die der Auftrag andauert.
        t.completion += (t.len / minutes(8) + 1) * minutes(16) + t.len;
    }
    return t.completion;
}

inline int64_t init_curr_time(task const &t)
{
    int64_t curr_time = t.start;

    // Behandelt den Fall, dass ein Auftrag außerhalb der Arbeitszeit eingeht.
    if (curr_time % minutes(24) < minutes(9))
        curr_time = day_start(curr_time) + minutes(9);
    else if (curr_time % minutes(24) > minutes(17))
        curr_time = day_start(curr_time) + minutes(24) + minutes(9);

    return curr_time;
}

void print_completion_times(std::vector<task> &tasks)
{
    std::cout << "Abschlusszeiten:\n";
    std::vector<int64_t> times(tasks.size());
    for (task const &t : tasks)
        times[t.i] = t.completion;
    for (int64_t c : times)
        std::cout << c << ' ';
    std::cout << '\n';
}

inline void print_help()
{
    std::cout << "Gebrauch:\n"
              << "  ./aufgabe4 [a] [b]\n"
              << "  optional: -t zum Ausgeben der Abschlusszeiten\n";
}

int main(int argc, char **argv)
{
    if (argc != 3 && argc != 4)
    {
        print_help();
        return 0;
    }

    std::vector<task> tasks;
    size_t z = 0;
    while (std::cin.peek() != EOF)
    {
        int64_t t, l;
        std::cin >> t >> l;
        tasks.push_back({t, l, 0, z++});
    }

    int64_t a = 0, b = 0;
    bool print_times = 0;
    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "-t"))
            print_times = 1;
        else
        {
            a = std::stoul(argv[i]);
            b = std::stoul(argv[++i]);
        }

    std::sort(tasks.begin(), tasks.end(), task_priority_comp(1, 0));

    // Die Permutation der Aufträge.
    std::vector<size_t> p(tasks.size());
    for (size_t i = 0; i < tasks.size(); i++)
        p[tasks[i].i] = i;

    int64_t curr_time = init_curr_time(tasks[0]);
    std::priority_queue<task, std::vector<task>, task_priority_comp>
        q(task_priority_comp(-a, -b));

    auto it = tasks.begin();
    while (it != tasks.end() || !q.empty())
    {
        if (q.empty())
            curr_time = std::max(curr_time, it->start);
        while (it != tasks.end() && it->start <= curr_time)
        {
            q.push(*it);
            it++;
        }

        curr_time = do_task(tasks[p[q.top().i]], curr_time);
        q.pop();
    }

    print_result(tasks);
    if (print_times)
        print_completion_times(tasks);
}