
// Informatik-Labor WS2019/20120
// Türme von Hanoi

#include "pch.h"
#include <iostream>
#include <time.h>																		//nötig zur Berechnung der Lösungszeit mit 'time_t'
#include <cmath>																		//nötig zur Berechnung der kürzesten Lösung mithilfe von 'pow()'
#include <Windows.h>																	//nötig zum Hinzufügen von 'Sleep'-Phasen bei der Begrüßung

#define MAX_SCHEIBEN 25																	//Das Spielfeld kann aus maximal 25 Scheiben bestehen

using namespace std;																	//Standard Namensraum wird für das gesamte Programm genutzt

//Allgemein gültige Variablen
int aiTurm[MAX_SCHEIBEN][3] = { 0 };													//Array der Türme, x-Wert = Reihe, y-Wert= Spalte
int aiAktuelleAnzahl[3] = { 0 };														//Speicherung der Scheibenzahl für 3 Spalten
int iAnzahlScheiben = 3;																//Anzahl Scheiben Variable
time_t tStart = 0;																		//Startzeit der Stoppuhr
time_t tEnde = 0;																		//Endzeit der Stoppuhr
int iSchritte = 0;																		//Wie viele Schritte wurden benötigt
int iAnzahlZügeMinimal = 0;																//Wie viele Schritte minimal?
char cWahl = 'x';																		//Auto oder manuell?


//Funktionsprototypen
bool spielBeendet();																	//Ist die letzte Scheibe auf dem Zielturm?
bool move(int iQuelle, int iZiel);														//Zugüberprüfung und bewegen der Scheiben
void willkommen();																		//Begrüßung (Intro)
void show();																			//Spielfeldanzeige
void init();																			//Spielstandverwaltung
void automatisch(int quelle, int hilfe, int ziel, int n);								//automatische Lösung
void spielanleitung();																	//Anzeige der Spielanleitung
bool auswahl(char cAuswahl);															//Auswahl des Spielmodus (Endlosabfrage bei falscher Eingabe)

int main()
{
	willkommen();																		//Anzeigen des Begrüßungstextes

	char cAnleitung = 'x';
	cout << "Hallo, Sie m\x94""chten wissen, wie 'T\x81rme von Hanoi' gespielt wird? Dann geben Sie bitte 'j' und danach 'Enter' ein." << endl;
	cout << "Falls nicht, geben Sie bitte einen beliebigen Buchstaben und danach 'Enter' ein." << endl;
	cin >> cAnleitung;																	//Abfrage, ob das Spielprinzip schon bekannt ist oder nochmal erklärt werden soll

	if (cAnleitung == 'j')															
	{
		spielanleitung();																//Spielanleitung wird angezeigt
	}
	else																				
	{
		cout << endl;																	//Platzhalter ohne Spielanleitung
	}

	int iQuelle = 0;																	//Initialisieren und Deklarieren der Quellstabvariable (Eingabe)
	int iZiel = 0;																		//Initialisieren und Deklarieren der Zielstabvariable (Eingabe)

	cout << "Bitte geben Sie die Anzahl der Scheiben (max. 25) f\x81r dieses Spiel ein:\n";		//Abfrage der Scheibenanzahl für das aktuelle Spiel
	cin >> iAnzahlScheiben;
	if (iAnzahlScheiben > 25)
	{
		cout << "Die eingegebene Scheibenanzahl ist zu groß und wurde auf 25 reduziert." << endl;	//Wird für die Scheibenanzahl ein Wert größer 25 eingegeben, so wird dieser später auf 25 reduziert (hier erfolgt die Ankündigung)
	}

	iAnzahlZügeMinimal = pow(2, iAnzahlScheiben) - 1;									//Minimale Zuganzahl über iAnzahlZügeMinimal = 2^n - 1

	cout << "\n-> 'a' f\x81r automatische L\x94sung \n-> 'm' f\x81r manuelle L\x94sung" << endl;	//Abfrage: Automatische oder manuelle Lösung?
	cin >> cWahl;
	cout << endl;

	while (!auswahl(cWahl))																//Eingabe wird solange abgefragt, bis a oder m gewählt wurden
	{
		cout << "'" << cWahl << "' ist keine g\x81ltige Eingabem\x94glichkeit, bitte entweder a oder m eingeben: " << endl;		//Fehler, wenn andere Buchstaben als a oder m eingegeben werden
		cin >> cWahl;
	}

	if (cWahl == 'a')																	//Automatischer Modus gewählt
	{
		time(&tStart);																	//Zeit für Lösung des PC wird gestartet
		init();																			//Startposition der Scheiben wird geladen
		show();																			//Spielfeld wird angezeigt

		automatisch(0, 1, 2, iAnzahlScheiben);											//Funktion für die automatische Lösung wird gestartet

		time(&tEnde);																	//Nach beendeter Ausgabe der Schritte wird die Zeit gestoppt

		cout << "Die automatische L\x94sung hat den Turm erfolgreich verschoben!\n" << endl;
		cout << "Zeit:\t" << tEnde - tStart << " Sekunden" << endl;						//Zeitausgabe
		cout << "Z\x81ge:\t" << iAnzahlZügeMinimal << endl;								//Zuganzahlausgabe
		cout << "\nT\x81rme von Hanoi ist ein Programm von Yassin Amor, Mahmoud Helmy und Marcus Tendler. \nProbieren Sie auch weitere Spiele wie zum Beispiel Rush Hour aus. \nDas Programm kann nun geschlossen und danach erneut gestartet werden, wir w\x81nschen viel Spass beim Spielen!" << endl;
	}

	else if (cWahl == 'm')																//Manueller Modus gewählt
	{
		init();																			//Startposition der Scheiben wird geladen
		show();																			//Spielfeld wird angezeigt
		time(&tStart);																	//Zeit bei Start wird gespeichert
		time(&tEnde);																	//variable Zeit seit Beginn wird am Anfang auf 0 gesetzt

		while (!spielBeendet())															//Solange die Funktion false zurückgibt kann immer weiter eine Scheibe bewegt werden, solange bis alle Scheiben auf Stab 3 sind
		{
			cout << "Sekunden: " << tEnde - tStart << endl;
			cout << "Z\x81ge:     " << iSchritte << endl;

			cout << "\nScheibe von Stab ";												//Eingabe des Quellstabes
			cin >> iQuelle;
			cout << "zu Stab ";
			cin >> iZiel;																//Eingabe des Zielstabes

			if (!move(iQuelle - 1, iZiel - 1))											//Wenn bool false zurückgibt, folgt "Zug nicht erlaubt"
			{
				cout << "Zug nicht erlaubt.\n" << endl;
				time(&tEnde);															//Zwischenzeit wird gespeichert und vor erneutem Versuch angezeigt
			}
			else
			{
				show();																	//Gibt bool true zurück, dann wird das Spiel mit neuen Positionen der Scheiben erneut angezeigt
				time(&tEnde);															//Zwischenzeit wird gespeichert und vor nächstem Zug angezeigt
				iSchritte++;															//Schrittanzahl wird um 1 erhöht
			}

		}

		if (spielBeendet())																//Alle Scheiben sind auf Turm 3, das Spiel ist beendet
		{
			time(&tEnde);																//Zeit bei Ende der Lösung wird gespeichert
			cout << "Herzlichen Gl\x81""ckwunsch, Sie haben den Turm erfolgreich verschoben!\n" << endl;
			cout << "Zeit:               " << tEnde - tStart << " Sekunden" << endl;										//Zeitausgabe
			cout << "Z\x81ge:               " << iSchritte << endl;															//Zuganzahlausgabe
			cout << "Zeit pro Zug :      " << static_cast<float>(tEnde - tStart) / iSchritte << " Sekunden" << endl;		//Durchschnittszeitausgabe

			if (iAnzahlZügeMinimal == iSchritte)										//kürzeste Lösung genutzt
			{
				cout << "Schnelleste L\x94sung: Ja" << endl;
			}
			else																		//nicht kürzeste Lösung genutzt
			{
				cout << "Schnelleste L\x94sung: Nein" << endl;
				cout << "                    Versuchen Sie doch bei einem weiteren Spiel, die minimale Zuganzahl zu erreichen!" << endl;

			}
			cout << "\nT\x81rme von Hanoi ist ein Programm von Yassin Amor, Mahmoud Helmy und Marcus Tendler. \nProbieren Sie auch weitere Spiele wie zum Beispiel 'Rush Hour' aus." << endl;
			cout << "Das Programm kann nun geschlossen und danach erneut gestartet werden, wir w\x81nschen viel Spa\xe1 beim Spielen!\n\n" << endl;
		}

	}
}

void show()
{
	if (cWahl != 'a')																	//Der Bildschirm wird nicht bereinigt, wenn die automatische Lösung gewählt wurde
	{
		system("cls");
	}

	for (int iZeile = iAnzahlScheiben - 1; iZeile >= 0; iZeile--)						//Darstellung des Spielstandes für die maximal nötigen Zeilen (also iAnzahlScheiben -1) 
	{																					//(Zeilen für das Spiel entspricht der Scheibenanzahl (Zählung beginnt bei 0, daher -1))
		for (int iTurm = 0; iTurm < 3; iTurm++)											//For Schleife zum Abgehen des Turms Zeile für Zeile
		{
			if (iZeile < aiAktuelleAnzahl[iTurm])										//Ist die Zahl der Zeile kleiner als die Anzahl der Scheiben auf dem Turm, so muss eine Zahl dargestellt werden
			{
				cout << "\t" << aiTurm[iZeile][iTurm];									//Die in der Zeile befindliche Zahl wird über aiTurm angefragt und dargestellt (Tab \t zwischen Türmen)
			}
			else
			{
				cout << "\t|";															//Ist die Zahl der Zeile größer gleich der Anzahl der Scheiben auf dem Turm, so wird ein senkrechter Strich ergänzt
			}
		}
		cout << endl;
	}
	cout << "----------------------------------\n";										//Abschlussstrich und Turmnummerierung
	cout << "\t\b(1)\t\b(2)\t\b(3)\n\n\n";
}

void init()
{
	if (iAnzahlScheiben > MAX_SCHEIBEN)													//Bei Auswahl von mehr als 25 Scheiben wird iAnzahlScheiben automatisch auf 25 gesetzt
	{
		iAnzahlScheiben = MAX_SCHEIBEN;
	}
	aiAktuelleAnzahl[0] = iAnzahlScheiben;												//Anzahl der Scheiben auf Turm 1(0) ist gleich der maximalen Scheibenanzahl
	aiAktuelleAnzahl[1] = aiAktuelleAnzahl[2] = 0;										//Turm 2(1) und 3(2) sind leer

	for (int i = 0; i < iAnzahlScheiben; i++)											//Schleife zum Festlegen der Scheibenposition für den Anfang
	{
		aiTurm[i][0] = iAnzahlScheiben - i;												//Für Turm 1(0) werden die Scheiben der Größe nach von klein (oben) nach groß (unten) angeordnet, dabei ist die kleinste Nummer der Scheibe der Größte Wert für iAnzahlScheiben
																						//Bsp.: i=0 aiTurm[0][0] = 3-0; (Für unterste Zeile) oder: i=2 aiTurm[2][0] = 3-2; (Für oberste Zeile)
	}																					//Der Wert i der Turmzeile aiTurm[i][0] ist also immer um 1 geringer als die tatsächliche Zeile (i=0 für unterste Zeile, i=1 für 2. Zeile usw.
}

bool move(int iQuelle, int iZiel)
{
	if (iQuelle == iZiel || iQuelle < 0 || iQuelle > 2 || iZiel < 0 || iZiel > 2)		//Eingabe von anderen Türmen als 1, 2 oder 3 ist nicht zulässig, auch von einen Turm auf den gleichen Turm legen ist verboten
	{
		return false;																	//move gibt false zurück
	}
	if (aiAktuelleAnzahl[iQuelle] == 0)													//Gibt es überhaupt eine Scheibe, die bewegt werden kann?
	{
		return false;																	//move gibt false zurück
	}

	int iTempQ = aiAktuelleAnzahl[iQuelle] - 1;											// Zeile der obersten Scheibe vom Quellturm wird übernommen (Zeile = Anzahl -1)
	int iTempZ = aiAktuelleAnzahl[iZiel] - 1;											// Zeile der obersten Scheibe vom Zielturm wird übernommen (Zeile = Anzahl -1)

	if (aiAktuelleAnzahl[iZiel] > 0 && aiTurm[iTempQ][iQuelle] > aiTurm[iTempZ][iZiel])		//Größere nicht auf kleinere Scheibe (Wert von aiTurm 1 nicht größer als aiTurm 2)
	{
		return false;																	//move gibt false zurück
	}

	else																				//wenn vorher keine Bedingung zugetroffen hat, also der Zug erlaubt ist, wird dieser ausgeführt
	{
		aiTurm[iTempZ + 1][iZiel] = aiTurm[iTempQ][iQuelle];							//Position über der höchsten vorhandenen Scheibe auf iZiel nimmt den Wert von iQuelle an
		aiTurm[iTempQ][iQuelle] = 0;													//Scheibe ist weg, der Wert der Position wird 0
		aiAktuelleAnzahl[iZiel]++;														//Anzahl der Scheiben auf Turm "Ziel" um 1 erhöhen
		aiAktuelleAnzahl[iQuelle]--;													//Anzahl der Scheiben auf Turm "Quelle um eins verringern

		return true;																	//move gibt true zurück
	}
}

bool spielBeendet()																		//Funktion für Stab 3 voll
{
	return aiAktuelleAnzahl[2] == iAnzahlScheiben;										//Spiel wird beendet, wenn die Aktuelle Anzahl an Scheiben auf Turm 3(2) gleich maximalen Scheibenanzahl ist (wenn alle Scheiben auf Turm 3(2) liegen)
}

void automatisch(int quelle, int hilfe, int ziel, int n)
{
	if (n > 0)																			//Scheibe 1 ans ziel, wenn die anderen Scheiben auf Zielturm sind, Begrenzung der rekursiven Funktion
	{
		automatisch(quelle, ziel, hilfe, n - 1);										//Die Funktion wird neu aufgerufen, dieses Mal mit vertauschter Reihenfolge von Hilfe und Ziel
																						//n-1, n wird stetig verringert
		move(quelle, ziel);																//die move Funktion wird aufgerufen, statt der manuellen Eingabe von Quelle und Ziel werden die Werte von der automatische Lösung genommen
		show();

		automatisch(hilfe, quelle, ziel, n - 1);										// nachdem die unterste Scheibe ihr Ziel ereicht hat sollen die restlichen Scheiben von mittlerem zu rechten Turm bewegt werden
	}
}

void willkommen()																		//Funktion zur Anzeige des Begrüßungstextes
{
	cout << "Willkommen zu:" << endl; Sleep(1500);
	cout << "*************************************************************************************************************************************************************************" << endl;
	cout << "   ______________                                                                           __     __                                          " << endl;
	cout << "  |_____    _____|                                                                         |  |   |  |                                         " << endl;
	cout << "        |  |          _   _                                                                |  |   |  |                            __           " << endl;
	cout << "        |  |         |_| |_|                                                               |  |___|  |                           (__)          " << endl;
	cout << "        |  |          _   _   _____   ___________     ____    ___  ___   ___    ______     |   ___   |   ____   ______     ___    __           " << endl;
	cout << "        |  |         | | | | |  ___| |   _    _  \\   / __ \\   \\  \\/  /  / _ \\  |   _  \\    |  |   |  |  / _  | |   _  \\   / _ \\  |  |  " << endl;
	cout << "        |  |         | |_| | |  |    |  | |  | |  | |   __/    \\    /  | |_| | |  | |  |   |  |   |  | | |_| | |  | |  | | |_| | |  |         " << endl;
	cout << "        |__|          \\___/  |__|    |__| |__| |__|  \\____|     \\__/    \\___/  |__| |__|   |__|   |__|  \\____| |__| |__|  \\___/  |__|    ";
	cout << "  von Mahmoud\n" <<endl;
	cout << 
	cout << "*************************************************************************************************************************************************************************"; Sleep(600);

	system("cls");

	

bool auswahl(char cAuswahl)			//Hilfsfunktion für die Auswahl des Spielmodus
{
	if (cAuswahl == 'a')			//Bedingungen
	{
		return true;
	}

	if (cAuswahl == 'm')
	{
		return true;
	}
	else
	{
		return false;
	}
}

void spielanleitung()																	//Spielanleitung zu 'Türme von Hanoi'
{
	cout << endl;
	cout << "____________________________________________________________________________________\n" << endl;
	cout << "Spielanleitung und -ablauf: \n" << endl;
	cout << "'T\x81rme von Hanoi' ist ein mathematisches Knobel- und Geduldsspiel.\n" << endl;
	cout << "Das Spiel besteht aus drei gleich gro\xE1""en T\x81rmen (1, 2 und 3), auf die" << endl;
	cout << "mehrere verschieden gro\xE1""e Scheiben (gekennzeichnet durch verschieden gro\xE1""e Zahlen)" << endl;
	cout << "gelegt werden. Zu Beginn liegen alle Scheiben der Gr\x94\xE1""e nach geordnet auf Turm 1" << endl;
	cout << "mit der gr\x94\xE1ten Scheibe unten und der kleinsten oben. Ziel des Spiels ist es," << endl;
	cout << "den kompletten Scheiben-Stapel von Turm 1 nach 3 zu versetzen. " << endl;
	cout << "\nBei jedem Zug darf dabei nur die oberste Scheibe eines beliebigen Turmes versetzt werden." << endl;
	cout << "Voraussetzung: auf dem Zielstab darf sich keine Scheibe mit geringerem Wert befinden. " << endl;
	cout << "Diese Bedingung f\x81hrt dazu, dass zu jedem Zeitpunkt des Spieles die Scheiben " << endl;
	cout << "auf jedem Turm der Gr\x94\xE1""e nach geordnet sind.\n\n";
	cout << "Nun k\x94nnen wir mit dem Spielen beginnen!" << endl;
	cout << "____________________________________________________________________________________\n\n\n";
}