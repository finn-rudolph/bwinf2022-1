#include <vector>
#include <string>
#include <iostream>
#include <fstream>

inline bool is_punctuation(char c)
{
    return c == '.' || c == ',' || c == ';' || c == '(' || c == ')' ||
           c == '[' || c == ']' || c == '{' || c == '}' || c == '"' ||
           c == '=' || c == ':' || c == '-' || c == '_' || c == '!' ||
           c == '?' || c == '$' || c == '*' || c == '\'';
}

// Ob der gegebene Iterator zu einem » oder « Zeichen (UTF-8) zeigt.
inline bool is_quotation(std::string const &s, std::string::iterator const &it)
{
    return s.end() - it >= 2 && *it == (char)0xc2 &&
           (*(it + 1) == (char)0xab || *(it + 1) == (char)0xbb);
}

inline bool is_word(std::string const &s, std::string::iterator const &it)
{
    return !is_punctuation(*it) && !is_quotation(s, it) &&
           *it != ' ' && *it != '\n';
}

// Beinhaltet ein Wort / Satzzeichen und dessen Position.
struct token
{
    std::string s;
    size_t line, word;
};

// Beinhaltet entweder ein Wort oder eine Anzahl aufeinanderfolgender Lücken.
struct pattern_token
{
    std::string s;
    bool is_gap;
    size_t gap_len;
};

std::vector<token> read_text(std::string const &fname)
{
    std::ifstream fin(fname);
    fin.tie(0);

    std::vector<token> text;
    std::string l;
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
                // Wandle alle Buchstaben in Kleinbuchstaben um.
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

int main(int argc, char **argv)
{
    std::string fname = "alice.txt";
    if (argc >= 2)
        fname = argv[1];

    std::vector<token> text = read_text(fname);
    size_t n = text.size();

    std::vector<pattern_token> pattern;
    size_t m = 0;

    while (std::cin.peek() != EOF)
    {
        std::string s;
        std::cin >> s;
        if (pattern.empty() || !pattern.back().is_gap || s[0] != '_')
            pattern.push_back({s, s[0] == '_', s[0] == '_'});
        else
            pattern.back().gap_len++;
        m++;
    }

    std::vector<std::pair<size_t, size_t>> matches;

    // i, j: Indinzes im Text (Tokens im Intervall [i, i + j) stimmen überein)
    // k: Index im Lückensatz
    size_t i = 0, j = 0, k = 0;

    while (i + j < n)
    {
        if (pattern[k].is_gap)
        {
            j += pattern[j].gap_len;
            k++;
        }
        else if (text[i + j].s == pattern[k].s)
        {
            j++;
            k++;
        }
        else
        {
            j = 0;
            k = 0;
            i++;
        }

        if (j == m)
        {
            matches.push_back(std::make_pair(text[i].line, text[i].word));
            j = 0;
            k = 0;
            i++;
        }
    }

    if (matches.empty())
        std::cout << "Lückensatz konnte nicht gefunden werden.\n";
    else
    {
        std::cout << matches.size();
        if (matches.size() == 1)
            std::cout << " Übereinstimmung (Zeile, Wort):\n";
        else
            std::cout << " Übereinstimmungen (Zeile, Wort):\n";
        for (auto const &[l, w] : matches)
            std::cout << l << ", " << w << '\n';
    }
}