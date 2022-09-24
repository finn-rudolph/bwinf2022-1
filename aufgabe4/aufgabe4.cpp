#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>

struct task
{
    unsigned start, len, completion;
};

inline bool task_start_comp(task const &t1, task const &t2)
{
    return t1.start < t2.start;
}

struct result
{
    double mean_time;
    unsigned max_time;
};

void print_result(result const &res)
{
    std::cout << "Mittlere Wartezeit: " << res.mean_time << '\n'
              << "Maximale Wartezeit: " << res.max_time << '\n';
}

inline unsigned minutes(unsigned hours)
{
    return hours * 60;
}

inline unsigned day_start(unsigned m)
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
    res.mean_time /= (double)tasks.size();
    return res;
}

result in_order(std::vector<task> &tasks)
{
    std::sort(tasks.begin(), tasks.end(), task_start_comp);
    unsigned curr_time = tasks[0].start;

    if (curr_time % minutes(24) < minutes(9))
        curr_time += (curr_time % minutes(24)) - minutes(9);
    else if (curr_time % minutes(24) > minutes(17))
        curr_time += minutes(33) - (curr_time % minutes(24));

    for (task &t : tasks)
    {
        unsigned day_time = curr_time % minutes(24);
        t.completion = std::min(curr_time + t.len, (day_start(curr_time) + minutes(17)));
        t.len -= t.completion - t.start;

        if (t.len)
        {
            t.completion += (t.len % minutes(8) + 1) * minutes(16);
            t.completion += t.len;
        }

        curr_time = t.completion;
    }

    return calc_result(tasks);
}

result shortest_first(std::vector<task> &tasks)
{
}

void print_help()
{
    std::cout << "Genau ein Argument aus Folgenden erforderlich.\n"
              << "v1 : Abarbeiten in der Eingangsordnung\n"
              << "v2 : Kürzester Auftrag zuerst\n"
              << "v3 : -\n";
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        print_help();
        return 0;
    }

    std::vector<task> tasks;
    while (std::cin.peek() != EOF)
    {
        unsigned t, l;
        std::cin >> t >> l;
        tasks.push_back({t, l, 0});
    }

    if (!strcmp(argv[1], "v1"))
        print_result(in_order(tasks));
    else if (!strcmp(argv[1], "v2"))
        print_result(shortest_first(tasks));
    else if (!strcmp(argv[1], "v3"))
        ;
    else
        print_help();
}