<h1 style="text-align: center;">Aufgabe 4: Fahrradwerkstatt</h1>

<p style="text-align: center;">Team-ID: 00968</p>

<p style="text-align: center;">Finn Rudolph</p>

<p style="text-align: center;">24. September 2022</p>

**Inhaltsverzeichnis**

[TOC]

## Lösungsidee

### Abarbeiten der Aufträge in Reihenfolge

Im Folgenden wird immer von Minuten als Zeiteinheit ausgegangen. Um diese Teilaufgabe zu lösen, werden die Aufträge zunächst nach Eingangszeit sortiert. Dass die Aufträge bereits nach Eingangszeit sortiert sind, ist von der Aufgabenstellung nicht garantiert. Anschließend können sie der Reihe nach abgearbeitet werden. Währenddessen soll ständig der aktuelle Zeitpunkt in einer Variablen aktualisiert werden, und für jeden Auftrag sein Zeitpunkt des Abschließens abgespeichert werden. Damit kann später die mittlere und maximale Wartezeit berechnet werden. Zu Beginn wird der aktuelle Zeitpunkt auf den Eingangszeitpunkt des ersten Auftrags gesetzt. Falls dieser außerhalb von Marcs Arbeitszeiten liegt, wird der aktuelle Zeitpunkt auf den nächsten Beginn eines Arbeitstags gesetzt.

Beim Abarbeiten eines Auftrags muss darauf geachtet werden, dass er möglicherweise nicht in einen Tag passt und seine Wartezeit so die Nacht mit einschließt. Deshalb wird zunächst geprüft, ob der Auftrag noch im aktuellen Tag abschließbar ist. Wenn das möglich ist, kann einfach die aktuelle Zeit um die Dauer des Auftrags erhöht werden. Andernfalls wird die Dauer des Auftrags um die übrige Zeit bis 17 Uhr des aktuellen Tages reduziert. Dadurch kann der Auftrag so behandelt werden, als würde er direkt am Anfang eines Arbeitstags um 9 Uhr starten. Der Zeitpunkt seines Abschlusses $t_{\text{Abschluss}}$ kann nun wie folgt berechnet werden ($\text{Dauer}$ ist nur die übrige Dauer nach der eben beschriebenen Subtraktion):
$$
t_{\text{Abschluss}} = \bigg \lfloor \frac {\text{Dauer}} {8 \cdot 60} + 1 \bigg \rfloor \cdot 16 \cdot 60 + \text{Dauer}
$$
Der Teil in der Abrundungsfunktion ist die Anzahl an Nächten, über die der Auftrag andauert ($+1$ wegen der zuvor übersprungenen Nacht). Zu dieser Zahl multipliziert mit der Anzahl Minuten einer Nacht, muss nur noch die für den Auftrag benötigte Arbeitszeit addiert werden. Auch in diesem Fall wird schließlich die aktuelle Zeit auf $t_{\text{Abschluss}}$ gesetzt. Eine Anmerkung: Falls die aktuelle Zeit vor dem Beginn des abzuarbeitenden Auftrags liegt, muss sie auf dessen Eingangszeit gesetzt werden. Das entspricht dem Fall, das Marc eine Zeit lang keine Aufträge hatte, da der Abschlusszeitpunkt des letzten Auftrags vor dem Eingangszeitpunkt des aktuellen Auftrags liegt.

Um nun den Mittelwert aller Wartezeiten zu berechenen, wird $t_\text{Abschluss} - t_\text{Eingang}$, wobei $t_\text{Eingang}$ die Eingangszeit des Auftrags ist, für jeden Auftrag aufsummiert und durch $n$, die Anzahl der Aufträge geteilt. Das Maximum wird durch Iteration über alle Aufträge mit einem laufend aktualisierten Maximum von $t_\text{Abschluss} - t_\text{Eingang}$ berechnet. Eine Anmerkung: Da Aufträge den gleichen Eingangszeitpunkt haben können, ist die Reihenfolge der Abarbeitung nicht eindeutig definiert, sodass sich trotz gleichem Verfahren unterschiedliche mittlere und maximale Wartezeiten ergeben können. Beispielsweise können sich diese unterscheiden, wenn in einer Implementierung der Sortieralgorithmus stabil sortiert, und in einer anderen Implementierung nicht.

### Kürzester Auftrag zuerst

Auch hier werden die Aufträge zunächst aufsteigend nach Eingangszeit sortiert. Jedoch soll hier immer der aktuell kürzeste bereits eingegangene Auftrag abgearbeitet werden. Um das zu erreichen, werden alle Aufträge, deren Eingangszeit kleiner oder gleich dem aktuellen Zeitpunkt ist, in eine Prioritätswarteschlange eingefügt, in der sie nach Dauer geordnet werden. Anschließend wird der Auftrag mit der kürzesten Dauer der Warteschlange entnommen und nach dem gleichen Verfahren wie in Teilaufgabe 1 abgearbeitet. Auch der aktuelle Zeitpunkt wird wieder auf den Abschlusszeitpunkt des Auftrags gesetzt. Dieses Verfahren (Einfügen in die Warteschlange und Abarbeiten des kürzesten Auftrags) wird wiederholt, bis alle Aufträge abgearbeitet sind. Ähnlich wie in Teilaufgabe 1 muss auch hier die aktuelle Zeit auf die Eingangzeit des nächsten abzuarbeitenden Auftrags gesetzt werden, falls die aktuelle Zeit vor dieser liegt und die Warteschlange leer ist.

Dieses Verfahren ist ebenfalls problematisch, da die Eingangszeit eines Auftrags völlig außer Acht gelassen wird. So kann es passieren, dass ein lang dauernder Auftrag sehr lange aufgeschoben wird, wenn ständig kurze Aufträge eingehen. Es ist zwar nachvollziehbar, einige kurze Aufträge schnell abzuarbeiten, aber ab einem gewissen Zeitpunkt sollte die frühere Eingangszeit eines längeren Auftrags überwiegen.

### Priorisierung nach einer Kombination von Eingangszeit und Dauer

Zunächst soll eine neue Kennzahl vorgestellt werden, dann ein Verfahren zur Abarbeitung der Aufträge. Das Problem an der Kennzahl der durchschnittlichen Wartezeit ist, dass mehrere kleine Wartezeiten gleichwertig zu ihrer Summe sind. Beispielweise haben folgende Wartezeiten die gleiche mittlere Wartezeit ($n = 5$).
$$
0, 2, 5, 0, 2 \implies \text{Mittlere Wartezeit: } \frac {0 + 2+ 5 + 0 + 2} 5 = \frac 9 5 \\
0, 0, 0, 0, 9 \implies \text{Mittlere Wartezeit: } \frac {0 + 0 + 0 + 0 +9} 5 = \frac 9 5 \\
$$
Jedoch ist erstere Folge an Wartezeiten wesentlich erstrebenswerter, da die Wartezeit gleichmäßiger auf Kunden verteilt ist und nicht ein Kunde eine sehr hohe Wartzeit erfährt. Das würde in diesem Beispiel durch die maximale Wartezeit erfasst werden, jedoch bezieht sich diese nur auf einen einzigen Auftrag und ist deshalb auch nicht repräsentativ. Siehe folgendes Beispiel mit $n = 5$.
$$
6, 3, 3, 3, 3 \implies \text{Mittlere Wartezeit: } \frac {18} 5, \text{Maximale Wartezeit: } 6 \\
6, 6, 6, 0, 0 \implies \text{Mittlere Wartezeit: } \frac {18} 5, \text{Maximale Wartezeit: } 6 \\
$$
Jedoch ist auch hier die erste Folge aufgrund der faireren Verteilung an Wartezeit besser geeignet. Um die Gesamtwartezeit und Gleichmäßigkeit der Verteilung in einer Größe zu erfassen, soll die neue Größe der Durchschnitt der Quadrate der Wartezeiten sein. Die Werte für das erste Beispiele sind $33 / 5$ für die erste Folge und $81/5$ für die zweite Folge. Wie man sieht, hat die sehr große Wartezeit von $9$ bei der zweiten Folge den Ausschlag gegeben, dass die Summe der Quadrate höher ist. Für das zweite Beispiel sind die Werte $72/5$ für die erste Folge und $108/5$ für die zweite Folge. Auch hier wurde die ungleichmäßigere Verteilung erfasst. Man kann an der Summe der Quadrate bemängeln, dass für eine gleichmäßigere Verteilung auch eine höhere durchschnittliche Wartezeit in Kauf genommen wird. Jedoch ist das aus Sicht eines Kunden, der eine sehr lange Wartezeit abbekommt, durchaus das richtige Vorgehen.

Das Hauptproblem der beiden obigen Verfahren zur Abarbeitung von Aufträgen ist, dass sie nur eine der zwei gegebenen Größen eines Auftrags in Betracht ziehen. Das erste Verfahren achtet allein auf die Eingangszeit, das zweite Verfahren allein auf die Dauer. Daher ist eine naheliegende Idee, die Eingangszeit und Dauer zu einer Größe, die die Priorität eines Auftrags angibt, zu kombinieren. Sei $f(e, d)$ eine Funktion, die die Priorität eines Auftrags angibt, wobei $d$ die Dauer und $e$ die Eingangszeit des Auftrags ist. Kleinere Werte von $f$ bedeuten größere Priorität. Es ergeben sich sehr viele Möglichkeiten für $f$, einige Beispiele sind ($a$ und $b$ sind Konstanten)
$$
f_1(e, d) = a\cdot e + b \cdot d \\
f_2(e, d) = a \cdot e + b \cdot d^2 \\
f_3(e, d) = a \cdot e + b^d
$$
Durch Veränderung von $f$ kann der Eingangszeit oder der Dauer eines Auftrags ein höherer Stellenwert zugewiesen werden. Beispielsweise ist bei $f_2$ die Dauer eines Auftrags entscheidender als bei $f_1$, und bei $f_3$ ist diese nochmal entscheidender als bei $f_2$. Die Zuweisungen der Priorität in den ersten beiden Versionen sind nur Spezialfälle mit $f(e, d) = e$ für das Abarbeiten der Aufträge in Reihenfolge und $f(e, d) = d$ für die Wahl des aktuell kürzesten Auftrags. Nun gilt es herauszufinden, welches $f$ einen geringstmöglichen Durchschnitt der Quadrate liefert. Dafür müssen schon einige Beispiele vorweggenommen werden. Nach einigen Versuchen stellten sich Werte von $f$ mit einem quadratischen oder sogar exponentiellen Zusammenhang als ungeeignet heraus, da sie $e$ oder $d$ zu stark gewichten. Daher werden im Folgenden nur Funktionen $f(e, d) = a \cdot e + b \cdot d$ betrachtet. Die Mittelwerte der Quadrate der Wartezeiten sind für einige Werte von $a$ und $b$ in Tabelle 1 aufgelistet.

| $a, b$ | fahrradwerkstatt0.txt | fahrradwerkstatt1.txt | fahrradwerkstatt2.txt | fahrradwerkstatt3.txt | fahrradwerkstatt4.txt |
| ------ | --------------------- | --------------------- | --------------------- | --------------------- | --------------------- |
| 1, 0   | 1.48519e+09           | 5.94398e+09           | 3.37013e+09           | 1.21029e+09           | 7.62149e+09           |
| 0, 1   | 1.02768e+09           | 2.42656e+09           | 1.5388e+09            | 3.22712e+09           | 8.09416e+09           |
| 1, 1   | 1.42372e+09           | 5.87056e+09           | 3.09179e+09           | 1.18107e+09           | 7.38219e+09           |
| 1, 10  | 8.98935e+08           | 5.28238e+09           | 1.63148e+09           | 9.84819e+08           | 5.86953e+09           |
| 10, 1  | 1.47722e+09           | 5.91297e+09           | 3.31495e+09           | 1.1969e+09            | 7.58012e+09           |
| 2, 3   | 1.36578e+09           | 5.82826e+09           | 3.0155e+09            | 1.17472e+09           | 7.3076e+09            |
| 1, 18  | 8.53904e+08           | 4.80054e+09           | 1.16784e+09           | 8.65062e+08           | 5.34088e+09           |
| 1, 42  | 9.24319e+08           | 3.57496e+09           | 1.10003e+09           | 7.88014e+08           | 4.99621e+09           |

_Tabelle 1: Mittelwert der Quadrate der Wartezeiten für verschiedene Werte von $a$ und $b$._

Während des Testen erwiesen sich größere Werte von $b$ im Vergleich zu $a$ als vorteilhaft, daher sind mehr Beispiele in diese Richtung vorhanden. Um aus der Tabelle nun den besten Wert für $a, b$ herauszufinden, soll $a = 1, b = 0$ (entspricht Abarbeiten der Aufträge in Reihenfolge) als Referenz dienen. Die Ergebnisse mit anderen Werte von $a, b$ sollen prozentual zu den Referenzwerten ausgedrückt werden, sodass der Mittelwert dieser Prozentwerte über alle Beispieleingaben als Indikator für die Qualität von $a, b$ herangezogen werden kann. Ein geringerer Mittelwert bedeutet höhere Qualität. Die Ergebnisse dieser Berechnung sind in Tabelle 2 dargestellt. Es ist nötig, die Werte prozentual zu einem Referenzwert auszudrücken, da der Mittelwert der Quadrate für verschiedene Beispieleingaben in verschiedenen Größenordnungen liegen kann.

| $a, b$ | 1, 0  | 0, 1     | 1, 1     | 1, 10     | 10, 1     | 2, 3    | 1, 18     | 1, 42     |
| ------ | ----- | -------- | -------- | --------- | --------- | ------- | --------- | --------- |
|        | 0.0 % | -2.515 % | -3.837 % | -28.762 % | -18.670 % | -38.878 | -37.107 % | -42.863 % |

_Tabelle 2: Mittelwert der prozentualen Unterschiede des Mittelwerts der Quadrate für verschiedene Werte von $a, b$ zu $a = 1, b = 0$._

Man sieht, dass $a = 1, b = 0$ offensichtlich die schlechteste Strategie für diese Beispieleingaben ist. Ein größerer Wert für $b$ im Vergleich zu $a$ ist tendenziell besser, bei $a = 1, b = 42$ wird der geringste Mittelwert der Quadrate erzielt. 

## Implementierung

Das Programm wird in C++ geschrieben und für Linux x86-64 kompiliert. 

In der `main`-Funktion werden die Aufträge eingelesen und das gewünschte Verfahren ausgeführt. Für jede der drei Versionen gibt es eine eigene Hauptfunktion: `in_order` für Teilaufgabe 1, `shortest_first` für Teilaufgabe 2

### Abarbeiten der Aufträge in Reihenfolge 

### Kürzester Auftrag zuerst



## Beispiele

![](grafiken/fahrrad_beispiel_6.svg)

## Quellcode