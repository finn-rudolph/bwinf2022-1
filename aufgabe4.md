<h1 style="text-align: center;">Aufgabe 4: Fahrradwerkstatt</h1>

<p style="text-align: center;">Team-ID: 00968</p>

<p style="text-align: center;">Finn Rudolph</p>

<p style="text-align: center;">24. September 2022</p>

**Inhaltsverzeichnis**

[TOC]

## Lösungsidee

### Aufgabenteil 1

Im Folgenden wird immer von Minuten als Zeiteinheit ausgegangen. Um diese Teilaufgabe zu lösen, werden die Aufträge zunächst nach Eingangszeit sortiert und dann abgearbeitet. Währenddessen soll ständig der aktuelle Zeitpunkt in einer Variablen aktualisiert werden, und für jeden Auftrag sein Zeitpunkt des Abschließens abgespeichert werden. Damit kann später die mittlere und maximale Wartezeit berechnet werden. 

Beim Abarbeiten eines Auftrags muss darauf geachtet werden, dass er möglicherweise nicht in einen Tag passt und seine Wartezeit so die Nacht mit einschließt. Deshalb wird zunächst geprüft, ob der Auftrag noch im aktuellen Tag abschließbar ist. Wenn das möglich ist, kann einfach die aktuelle Zeit um die Dauer des Auftrags erhöht werden. Andernfalls wird die Dauer des Auftrags um die übrige Zeit bis 17 Uhr des aktuellen Tages reduziert. Dadurch kann der Auftrag so behandelt werden, als würde er direkt am Anfang eines Arbeitstags um 9 Uhr starten. Der Zeitpunkt seines Abschlusses $t_{\text{Abschluss}}$ kann nun wie folgt berechnet werden ($\text{Dauer}$ ist nur die übrige Dauer nach der eben beschriebenen Subtraktion.):
$$
t_{\text{Abschluss}} = \bigg \lfloor \frac {\text{Dauer}} {8 \cdot 60} + 1 \bigg \rfloor \cdot 16 \cdot 60 + \text{Dauer}
$$
Der Teil in der Abrundungsfunktion ist die Anzahl an Nächten, über die der Auftrag andauert ($+1$ wegen der zuvor übersprungenen Nacht). Zu dieser Zahl multipliziert mit der Anzahl Minuten einer Nacht, muss nur noch die für den Auftrag benötigte Arbeitszeit addiert werden. Auch in diesem Fall wird schließlich die aktuelle Zeit auf $t_{\text{Abschluss}}$ gesetzt.

Um nun den Mittelwert aller Wartezeiten zu berechenen, wird $t_\text{Abschluss} - t_\text{Eingang}$, wobei $t_\text{Eingang}$ die Eingangszeit des Auftrags ist, für jeden Auftrag aufsummiert und durch $n$, die Anzahl der Aufträge geteilt. Das Maximum wird durch Iteration über alle Aufträge mit einem laufend aktualisierten Maximum von $t_\text{Abschluss} - t_\text{Eingang}$ berechnet.

### Aufgabenteil 2

### Aufgabenteil 3

## Laufzeitanalyse

## Implementierung

## Beispiele

![](grafiken/fahrrad_beispiel_6.svg)

## Quellcode