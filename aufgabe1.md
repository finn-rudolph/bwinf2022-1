<h1 style="text-align: center;">Aufgabe 1: Störung</h1>

<p style="text-align: center;">Teilnahme-ID: </p>

<p style="text-align: center;">Finn Rudolph</p>

<p style="text-align: center;">19. September 2022</p>

## Lösungsidee

Das Problem wird durch Durchsuchen des großen Texts nach Vorkommnissen des Lückensatzes gelöst. Um das zu erleichtern, soll der Text in einem Vorverarbeitungsschritt in Tokens aufgespalten werden, wobei ein Token entweder ein Wort oder ein Satzzeichen ist. Auch der Lückensatz wird derartig zerlegt. Im Folgenden sei $n$ die Anzahl an Tokens im großen Text und $m$ die Anzahl an Tokens oder Lücken im Lückensatz. Mithilfe der Vorverarbeitung muss nur noch überprüft werden, ob für ein $i : 0 \le i < n - m$ gilt, dass alle Tokens im Intervall $[i, i + m)$ im großen Text gleich den Tokens im Lückensatz sind. Falls das Token im Lückensatz eine Lücke ist, wird der Vergleich übersprungen. Eine Optimierung ist hierbei, an Stellen mehrerer aufeinanderfolgender Lücken sofort alle gemeinsam zu überspringen. Ein Beispiel für solch einen Fall ist `stoerung4.txt`:

```
ein _ _ tag
```

Wenn Index $1$ (beginnend bei Index $0$) erreicht wird, kann gleich zu Index $3$ gesprungen werden. Bei größeren Lückensätzen kann das einen erheblichen Geschwindigkeitsvorteil bieten.

Der Hauptvorteil der Aufspaltung des Texts in Tokens ist, dass die Implementierung der eigentlichen Suche wesentliche einfacher wird, da Leerzeichen, Satzzeichen, neue Zeilen etc. nicht beachtet werden müssen.

### Laufzeitanalyse

Seien $n$ und $m$ wie oben definiert und $\rho$ die durchschnittliche Wortlänge. Es werden $\Theta(n)$ Vergleiche des Lückensatzes mit einem Teilstring von Tokens durchgeführt (für jedes $i : 1 \le i < n - m)$. Jeder dieser Vergleiche benötigt $\Omega(1)$ und $O(m \cdot \rho)$ Zeit. Die untere Schranke von $\Omega(1)$ wird erreicht, wenn sich der erste Buchstabe des ersten Tokens des Lückensatzes vom ersten Buchstaben des Tokens bei $i$ unterscheidet. Dagegen benötigt das Feststellen einer vollständigen Übereinstimmung des Lückensatzes $O(m \cdot \rho)$ Zeit. Somit beträgt die Laufzeit des gesamten Programms $\Omega(n)$ und $O(n \cdot m \cdot \rho)$ Zeit.

## Implementierung

Der Algorithmus wird in C++ implementiert. Die Implementierung ist in drei Teile gegliedert: Im ersten Teil (Z. 97-117 in `main`) werden der große Text und der Lückensatz eingelesen und in Tokens zerteilt. Der zweite Teil (Z. 118-150 in `main`) ist die eigentliche Suche nach Übereinstimmungen. Zuletzt werden die Ergebnisse der Suche ausgegeben (Z. 151-164 in `main`). Der große Text, in dem gesucht werden soll, ist standardmäßig `alice.txt`, kann aber als Argument in der Kommandozeile angegeben werden (Z. 97-99).

Das Einlesen des großen Texts übernimmt die Funktion `read_text`. Alle nichtleeren Zeichen werden nach Satzzeichen, Anführungszeichen oder Wort klassifiziert und in der Struktur `token` gespeichert. Ein Token beinhaltet entweder ein ganzes Wort oder nur ein einzelnes Satzzeichen, und der gesamte Text wird so in einen Vektor aus Tokens umgewandelt. Um bei der späteren Ausgabe von gefundenen Übereinstimmungen präzise Angaben machen zu können, wird der Text zeilenweise eingelesen und zu jedem Token abgespeichert, in welcher Zeile es erscheint und, falls es ein Wort ist, an welcher Stelle es in der Zeile steht. Noch eine Anmerkung zur Klassifizierung nach Satzzeichen, Anführungszeichen und Wort: Normale Satzzeichen wir `.` oder `,` werden durch `is_punctuation` durch einen simplen Vergleich erkannt. Das Anführungszeichen » oder « zu erkennen ist allerdings etwas schwieriger, da es nach UTF-8-Codierung zwei Bytes einnimt. Gelöst wird dieses Problem durch die Funktion `is_quotation`, die zwei aufeinanderfolgende Bytes auf eine Übereinstimmung mit dem UTF-8-Code von » oder « überprüft. Die Umwandlung des Texts in `read_text` geschieht folgendermaßen: Der Iterator `it` iteriert über alle Zeichen der eingelesenen Zeile `l`. In jeder Iteration der for-Schleife wird ein (oder kein) Wort verarbeitet und alle darauf folgenden Leer- und Satzzeichen. Das HInzufügen eines Worts geschieht in Zeile 59-70. Beim Hinzufügen von Satzzeichen (Z. 72-82) muss wieder darauf geachtet werden, dass Anführungszeichen zwei Bytes einnehmen.

Das Einlesen des Lückensatzes (Z. 104-116) geschieht durch eine for-Schleife, in der alle eingelesenen Wörter in eine Struktur `pattern_token` umgewandelt werden und dem Vektor `pattern` hinzugefügt werden. Lücken werden dadurch angezeigt, dass `is_gap` auf $1$ gesetzt wird. Falls direkt aufeinanderfolgende Lücken auftreten, wird kein neues `pattern_token` hinzugefügt, sondern die Lückenlänge im bereits vorhandenen erhöht.

Das Finden von Übereinstimmungen geschieht mithilfe einer while-Schleife, gefundene Übereinstimmungen werden als Paar aus Zeile und Wortnummer in `matches` gespeichert. `i` ist der Anfangsindex der aktuellen Übereinstimmung in `text`. `j` ist die Länge der aktuellen Übereinstimmung, gemessen an der Anzahl Tokens in `text` und `k` der Index des nächsten zu überprüfenden Tokens im Lückensatz. `j` kann von `k` abweichen, da meherere Lücken als ein `pattern_token` zusammengefasst werden können. In der while-Schleife werden drei Fälle unterschieden: Ist das aktuelle Token im Lückensatz eine Lücke, werden entsprechend Tokens übersprungen. Der zweite Fall ist, dass die nächsten Tokens in Text und Lückensatz übereinstimmen, sodass `j` und `k` erhöht werden. Im dritten Fall wird die aktuelle Übereinstimmung abgebrochen und `i` erhöht. Zuletzt wird überprüft, ob die aktuelle Übereinstimmung vollständig ist, sodass sie zu `matches` hinzugefügt werden kann.

Im letzten Teil des Programms werden die Ergebnisse der Suche ausgegeben. Die Angabe von Zeile und Stelle des ersten Worts ist deshalb sinnvoll, da Bücher üblicherweise nach Zeilen gegliedert sind und so das Auffinden der Stelle besonders einfach wird.

## Beispiele

Die Richtigkeit der Ausgabe wurde bei allen gezeigten Beispielen mithilfe der Suchfunktion eines Texteditors überprüft. Dazu wurde nach einem im Lückensatz vorkommenden Wort gesucht und alle Vorkommnisse dessen angesehen, um Übereinstimmungen zu finden.

### Alice im Wunderland

#### stoerung0.txt

```
das _ mir _ _ _ vor
```

```
1 Übereinstimmung(en) (Zeile, Wort):
440, 2
```

#### stoerung1.txt

```
ich muß _ clara _
```

```
2 Übereinstimmung(en) (Zeile, Wort):
425, 2
441, 10
```

#### stoerung2.txt

```
fressen _ gern _
```

```
3 Übereinstimmung(en) (Zeile, Wort):
213, 10
214, 3
214, 7
```

#### stoerung3.txt

```
das _ fing _
```

```
2 Übereinstimmung(en) (Zeile, Wort):
2319, 4
3301, 10
```

#### stoerung4.txt

```
ein _ _ tag
```

```
1 Übereinstimmung(en) (Zeile, Wort):
2293, 6
```

#### stoerung5.txt

```
wollen _ so _ sein
```

```
1 Übereinstimmung(en) (Zeile, Wort):
2185, 2
```

## Quellcode

```cpp
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
```

