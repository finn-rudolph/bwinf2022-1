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

Seien $n$ und $m$ wie oben definiert und $\rho$ die durchschnittliche Wortlänge. Es werden $\Theta(n)$ Vergleiche des Lückensatzes mit einem Teilstring von Tokens durchgeführt (für jedes $i : 1 \le i < n - m)$. Jeder dieser Vergleiche benötigt $\Omega(1)$ und $O(m \cdot \rho)$ Zeit. Die Laufzeit von $\Omega(1)$ tritt auf, wenn sich der erste Buchstabe des ersten Tokens des Lückensatzes vom ersten Buchstaben des Tokens bei $i$ unterscheidet. Dagegen benötigt das Feststellen einer vollständigen Übereinstimmung des Lückensatzes $O(m \cdot \rho)$ Zeit. Somit beträgt die Laufzeit des gesamten Programms $\Omega(n)$ und $O(n \cdot m \cdot \rho)$ Zeit.

## Implementierung

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

