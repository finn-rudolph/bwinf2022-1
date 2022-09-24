#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string.h>

struct task
{
    uint64_t start, len, completion;
    size_t i;
};

inline bool task_earlier_start(task const &t1, task const &t2)
{
    return t1.start < t2.start;
}

inline bool task_greater_len(task const &t1, task const &t2)
{
    return t1.len > t2.len;
}

struct result
{
    long double mean_time;
    uint64_t max_time;
};

void print_result(result const &res)
{
    std::cout << "Mittlere Wartezeit: " << res.mean_time << '\n'
              << "Maximale Wartezeit: " << res.max_time << '\n';
}

inline uint64_t minutes(uint64_t hours)
{
    return hours * 60;
}

inline uint64_t day_start(uint64_t m)
{
    return (m / minutes(24)) * minutes(24);
}

result calc_result(std::vector<task> const &tasks)
{
    result res = {0.0, 0};
    for (task const &t : tasks)
    {
        res.mean_time += (t.completion - t.start);
        res.max_time = std::max(res.max_time, t.completion - t.start);
    }
    res.mean_time /= (long double)tasks.size();
    return res;
}

// Aktualisiert die Abschlusszeit des Auftrags und gibt die neue aktuelle Zeit
// zurück.
inline uint64_t do_task(task &t, uint64_t curr_time)
{
    // Fülle den aktuellen Arbeitstag auf, falls der Auftrag so lang ist.
    t.completion = std::min(curr_time + t.len, (day_start(curr_time) + minutes(17)));
    t.len -= (t.completion - curr_time);

    if (t.len)
    {
        // Füge die Zeit der Nächte hinzu, die der Auftrag andauert.
        t.completion += (t.len / minutes(8) + 1) * minutes(16);
        t.completion += t.len;
    }
    return t.completion;
}

inline uint64_t init_curr_time(task const &t)
{
    uint64_t curr_time = t.start;

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
    std::vector<uint64_t> times(tasks.size());
    for (task const &t : tasks)
        times[t.i] = t.completion;
    for (uint64_t c : times)
        std::cout << c << ' ';
    std::cout << '\n';
}

result in_order(std::vector<task> &tasks, bool print_times)
{
    std::sort(tasks.begin(), tasks.end(), task_earlier_start);
    uint64_t curr_time = init_curr_time(tasks[0]);

    for (task &t : tasks)
    {
        // Wenn nötig, warte bis zum Beginn des nächsten Auftrags.
        curr_time = std::max(curr_time, t.start);
        curr_time = do_task(t, curr_time);
    }

    if (print_times)
        print_completion_times(tasks);
    return calc_result(tasks);
}

result shortest_first(std::vector<task> &tasks, bool print_times)
{
    std::sort(tasks.begin(), tasks.end(), task_earlier_start);

    // Die Permutation der Aufträge.
    std::vector<size_t> p(tasks.size());
    for (size_t i = 0; i < tasks.size(); i++)
        p[tasks[i].i] = i;

    uint64_t curr_time = init_curr_time(tasks[0]);
    std::priority_queue<task, std::vector<task>, decltype(&task_greater_len)>
        q(&task_greater_len);

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

    if (print_times)
        print_completion_times(tasks);
    return calc_result(tasks);
}

inline void print_help()
{
    std::cout << "Ein oder zwei Argumente erforderlich.\n"
              << "Versionen:\n"
              << "    Abarbeiten in der Eingangsordnung : v1\n"
              << "    Kürzester Auftrag zuerst : v2\n"
              << "Option:\n"
              << "    Anzeigen der Abschlusszeiten aller Aufträge: -t\n";
}

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 3)
    {
        print_help();
        return 0;
    }

    std::vector<task> tasks;
    size_t z = 0;
    while (std::cin.peek() != EOF)
    {
        uint64_t t, l;
        std::cin >> t >> l;
        tasks.push_back({t, l, 0, z++});
    }

    unsigned v = 0;
    bool print_times = 0;
    for (int i = 1; i < argc; i++)
        if (!strcmp(argv[i], "v1"))
            v = 1;
        else if (!strcmp(argv[i], "v2"))
            v = 2;
        else if (!strcmp(argv[i], "-t"))
            print_times = 1;

    switch (v)
    {
    case 1:
        print_result(in_order(tasks, print_times));
        break;
    case 2:
        print_result(shortest_first(tasks, print_times));
        break;
    default:
        print_help();
    }
}
