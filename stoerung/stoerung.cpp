#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>

inline bool is_punctuation(char c)
{
    return c == '.' || c == ',' || c == ';' || c == '(' || c == ')' ||
           c == '[' || c == ']' || c == '{' || c == '}' || c == '"' ||
           c == '=' || c == ':' || c == '-' || c == '_' || c == '!' ||
           c == '?' || c == '$' || c == '*';
}

// Gibt zurück, ob der gegebene Iterator zu einem » oder « Zeichen (UTF-8) zeigt.
inline bool is_quotation(std::string const &s, std::string::iterator const &it)
{
    return s.end() - it >= 2 && *it == (char)0xc2 &&
           (*(it + 1) == (char)0xab || *(it + 1) == (char)0xbb);
}

inline bool is_word(std::string const &s, std::string::iterator const &it)
{
    return !is_punctuation(*it) && *it != ' ' && *it != '\n' && !is_quotation(s, it);
}

// Beinhaltet ein Wort / Satzzeichen und dessen Position.
struct token
{
    std::string s;
    size_t line, word;
};

std::vector<token> read_text(std::string const &fname)
{
    std::ifstream fin(fname);

    std::vector<token> text;
    std::string l = "\0";
    size_t line = 1;

    while (!fin.eof())
    {
        std::getline(fin, l);
        size_t word = 1;

        for (auto it = l.begin(); it != l.end(); it++)
        {

            std::string s;
            while (it != l.end() && is_word(l, it))
            {
                if (*it >= 'A' && *it <= 'Z')
                    *it = (*it - 'A') + 'a';
                s.push_back(*it);
                it++;
            }

            if (!s.empty())
                text.push_back({s, line, word});

            while (it != l.end() && !is_word(l, it))
            {
                if (is_punctuation(*it))
                    text.push_back({std::string(1, *it), line, word});
                else if (is_quotation(l, it))
                {
                    text.push_back({std::string({*it, *(it + 1)}), line, word});
                    it++;
                }
                it++;
            }

            if (*it != ' ')
                it--;
            word++;
        }

        line++;
    }

    return text;
}

// Der zurückgegebene Vektor beinhaltet bei Index i die Länge des längsten
// Teilpatterns, das gleich einem Präfix des Patterns ist und bei i endet.
std::vector<size_t> similar_subpatterns(std::vector<std::string> const &pattern)
{
    std::vector<size_t> v(pattern.size() + 1, 0);
    size_t i = 1, k = 0;

    while (i + k < pattern.size())
    {
        if (pattern[i + k] == pattern[k])
        {
            v[i + k] = k + 1;
            k++;
        }
        else if (!k)
        {
            i++;
        }
        else
        {
            i = i + k - v[k];
            k = v[k];
        }
    }

    return v;
}

int main(int argc, char **argv)
{
    std::string fname = "alice.txt";
    if (argc >= 2)
        fname = argv[1];

    std::vector<token> text = read_text(fname);

    std::vector<std::string> pattern;
    while (std::cin.peek() != EOF)
    {
        std::string s;
        std::cin >> s;
        pattern.push_back(s);
    }

    std::vector<size_t> v = similar_subpatterns(pattern);
    std::vector<std::pair<size_t, size_t>> occurences;

    for (size_t i = 0; i < text.size(); i++)
    {
        size_t j = 0;
        while (j < pattern.size() && (pattern[j] == "_" || text[i + j].s == pattern[j]))
            j++;

        if (j == pattern.size())
            occurences.push_back(std::make_pair(text[i].line, text[i].word));
        i = i + j - v[j];
    }

    if (occurences.empty())
        std::cout << "Lückensatz konnte nicht gefunden werden.\n";
    else
    {
        std::cout << "Vorkommnisse (Zeile, Wort):\n";
        for (auto const &[l, w] : occurences)
            std::cout << l << ", " << w << '\n';
    }
}