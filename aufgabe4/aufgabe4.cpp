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

inline bool task_start_comp(task const &t1, task const &t2)
{
    return t1.start < t2.start;
}

inline bool task_len_comp(task const &t1, task const &t2)
{
    return t1.len < t2.len;
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

result in_order(std::vector<task> &tasks)
{
    std::sort(tasks.begin(), tasks.end(), task_start_comp);
    uint64_t curr_time = init_curr_time(tasks[0]);

    for (task &t : tasks)
    {
        // Wenn nötig, warte bis zum Beginn des nächsten Auftrags.
        curr_time = std::max(curr_time, t.start);
        curr_time = do_task(t, curr_time);
    }

    return calc_result(tasks);
}

result shortest_first(std::vector<task> &tasks)
{
    std::sort(tasks.begin(), tasks.end(), task_start_comp);
    uint64_t curr_time = init_curr_time(tasks[0]);
    std::priority_queue<task, std::vector<task>, decltype(&task_len_comp)>
        q(&task_len_comp);

    auto it = tasks.begin();
    while (it != tasks.end() || !q.empty())
    {
        if (q.empty())
            curr_time = std::max(curr_time, it->start);
        while (it->start <= curr_time && it != tasks.end())
        {
            q.push(*it);
            it++;
        }

        curr_time = do_task(tasks[q.top().i], curr_time);
        q.pop();
    }

    return calc_result(tasks);
}

inline void print_help()
{
    std::cout << "Genau ein Argument erforderlich.\n"
              << "Versionen:\n"
              << "    Abarbeiten in der Eingangsordnung : v1\n"
              << "    Kürzester Auftrag zuerst : v2\n";
}

int main(int argc, char **argv)
{
    if (argc != 2)
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

    if (!strcmp(argv[1], "v1"))
        print_result(in_order(tasks));
    else if (!strcmp(argv[1], "v2"))
        print_result(shortest_first(tasks));
    else
        print_help();
}
