//#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* functie ce citeste dintr-un fisier o matrice si un scor salvate 
anterior; in cazul in care fisierul nu exista, functia returneaza
1, pentru a sari peste optiunea "Resume" din  meniu */
int citire (int m[][4], int *sc)
{
	int i, j;
	FILE *f;
	f = fopen ("save", "r");
	if (f == NULL)
		return 1;
	fscanf (f, "%d", sc);
	for (i = 0; i < 4 ; i++)
		for (j = 0; j < 4; j++)
			fscanf (f, "%d", &m[i][j]);
	return 0;
}
/* functie pentru salvarea unei table de joc si a scorului la momentul
respectiv pentru a putea continua un joc, chiar si dupa ce programul 
a fost inchis */
void scriere (int m[][4], int *sc)
{
	int i, j = 0;
	FILE *f;
	f = fopen ("save", "w");
	fprintf (f, "%d\n", *sc);
	for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
				fprintf (f, "%d ", m[i][j]);
		fprintf (f, "\n");
		}
	fclose (f);
}
/* functie pentru afisarea tabelei de scoruri, citita dintr-un fisier;
pentru intoarcerea la meniu se va apasa orice tasta */
void highscores (int sc)
{
	erase ();
	int i, v[6];
	FILE *f;
	f = fopen ("highscore", "r");
	/* vectorul se initializeaza cu 0, astfel incat in tabel se va 
	afisa 0 daca in dreptul intrarilor care nu au o valoare 
	corespunzatoare in fisier */
	for (i = 0; i < 6; i++)
		v[i] = 0;
	if (f != NULL)
		for (i = 1; i < 6; i++)
			fscanf (f, "%d", &v[i]);
	attron (A_BOLD);
	mvprintw (5, (COLS - 16) / 2, "Highscores");
	attroff (A_BOLD);
	WINDOW *w;
	w = newwin (7, 13, (LINES - 8) / 2, (COLS - 17) / 2);
	box (w, 0, 0);
	refresh ();
	/* daca scorul la sfarsitul unei runde intra in primele 5, acesta 
	e afisat si scos in evidenta */
	for (i = 1; i < 6; i++)
	{
		if (sc == v[i])
		{
			wattron (w, A_STANDOUT);
			sc = -1;
		}
		else
			wattroff (w, A_STANDOUT);
		mvwprintw (w, i, 1, "%d. %d", i, v[i]);
	}
	/* daca scorul la finalul unei runde este prea mic pentru a intra 
	in primele 5 acesta este afisat separat */
	if (sc <= v[5] && sc != -1)
	{
		wattron (w, A_STANDOUT);
		mvprintw (17, (COLS - 20) / 2, "Your Score: %d", sc);
		wattroff (w, A_STANDOUT);
	}
	wrefresh (w);
	mvprintw (19, (COLS - 26) / 2, "Press any key to exit");
	timeout (99999999);
	getch ();
}
/* functie pentru sortarea unui vector, folosita pentru aranjarea 
tabelei highscores */
void sort(int v[], int n)
{
	int c, d, t;
	for (c = 0; c < n; c++)
	{
		for (d = 0; d < n - 1; d++)
		{
			if (v[d] < v[d+1])
			{
				t = v[d+1];
				v[d+1] = v[d];
				v[d] = t;
			}
		}
	}
}
/* functie pentru tiparirea in fisier a unui nou scor, daca acest este
mai mare decat a cincea intrare deja existenta */
void phscore (int sc)
{
	int i, v[8];
	FILE *f, *g;
	f = fopen ("highscore", "r");
	for (i = 0; i < 6; i++)
		v[i]=0;
	if (f != NULL)
		for (i = 1; i < 6; i++)
			fscanf (f, "%d", &v[i]);
	v[6] = sc;
	sort (v, 7);
	g = fopen ("highscore", "w");
	for (i = 0; i < 6; i++)
		fprintf (g, "%d ", v[i]);
	fclose (g);
}
/* functie ce copiaza prima matrice primita ca argument in cea de-a doua
matrice, primia tot ca argument */
void cp (int m[][4], int a[][4])
{
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			a[i][j] = m[i][j];
}
/* functie ce compara doua matrici primite ca argument; functia returneaza
0 daca cele doua sunt egale si 1 daca cel putin 1 termen difera */
int cmp (int m[][4], int a[][4])
{
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (a[i][j] != m[i][j])
				return 1;
	return 0;
}
/* functie ce alege aleator dintre 2 sau 4 si insereaza acest numar 
intr-o pozitie aleasa si ea aleator */
void ran (int m[][4])
{
	int i, j, n;
	i = rand () % 4;
	j = rand () % 4;
	n = rand () % 100 + 5;
	if (m[i][j] != 0)
		ran (m);
	else
		if (n % 4 != 0)
			m[i][j] = 2;
		else
			m[i][j] = 4;
}
/* functie ce creeaza ferestrele in care vor fi afisate elementele 
matricei, la un set de coordonate specifice */
WINDOW *create (int a, int b, int c, int d)
{
	WINDOW *w;
	w = newwin (a, b, c, d);
	wrefresh (w);
	return w;
}
/* functie pentru selectarea culorii unei ferestre, in functie de numarul
ce trebuie afisat in aceasta */
int cod (int a)
{
	int i, x = 1;
	for (i = 2; i <= a; i = i * 2)
	{
		if (x == 16)
			x = x / 16 ;
		if (a == i)
			return x;
		x++;
	}
	return x;
}
/* functie ce initializeaza perechile de culori pentru casutele tablei 
de joc */
void incol ()
{
	init_pair (1, 7, 3);
	init_pair (2, 7, 2);
	init_pair (3, 7, 4);
	init_pair (4, 7, 5);
	init_pair (5, 7, 6);
	init_pair (6, 7, 9);
	init_pair (7, 7, 12);
	init_pair (8, 7, 1);
	init_pair (9, 7, 8);
	init_pair (10, 7, 17);
	init_pair (11, 7, 16);
	init_pair (12, 7, 10);
	init_pair (13, 7, 13);
	init_pair (14, 7, 14);
	init_pair (15, 7, 11);
	init_pair (17, 7, 7);
	init_pair (18, 0, 0);
}
/* functie ce numara numarul de cifre al unui numar primit ca argument, 
pentru a ajuta la pozitionarea centrala in interiorul unei casute */
int number (int a)
{
	int x;
	while (a > 0)
	{
		x++;
		a = a / 10;
	}
	return x;
}
/* functie ce afiseaza continutul ferestrei de joc: data si ora, scorul
si tabela de joc */
void afisare (int m[][4], int sc, int x)
{
	/* curatarea ecranului, pentru a sterge orice informatii ramase
	din afisarile anterioare si ascunderea cursorului */
	erase ();
	curs_set (0);
	int i, j, a, c;
	/* se creeaza fereastra ce va contine casutele tablei de joc */
	WINDOW *w, *W;
	refresh ();
	W = newwin(17, 31, 6, 20);
	box( W, 0, 0);
	wbkgd (W, COLOR_PAIR (17));
	wrefresh (W);
	// se afiseaza data si ora
	mvprintw (0, 25, "Press F6 for help");
	time_t rawtime;
	struct tm *ti;
	time (&rawtime);
	ti = localtime (&rawtime);
	mvprintw (2, 46, "%s", asctime (ti));
	mvprintw (4, 30, "score: %d", sc);
	if (x < 10)
		mvprintw (2, 1, "Autosave enabled");
	else
		mvprintw (2, 1, "Autosave disabled");
	for (i = 0; i < 4; i++)
	{
		a = 4;
		for (j = 0; j < 4; j++)
		{
			w = create (3, 6, (i + 1.8) * 4, 18 + j + a);
			if (m[i][j] != 0)
			{
				/* se afiseaza fiecare casuta, cu o culoare
				 diferita in functie de numarul pe care il
				 contine */
				c = number (m[i][j]);
				mvwprintw (w, 1, (6 - c)/ 2, "%d", m[i][j]);
				wbkgd (w, COLOR_PAIR (cod (m[i][j])));
			}
			else
				wbkgd (w, COLOR_PAIR (18));
			wrefresh (w);
			a = a + 6;
		}
	}
}
/* functie pentru afisarea tabelei de instructiuni intr-o fereastra noua */
void help ()
{
	erase ();
	curs_set (0);
	WINDOW *w;
	refresh ();
	w = newwin(18, 32, (LINES - 18) / 2, (COLS - 32) / 2);
	box( w, 0, 0);
	wrefresh (w);
    	mvwprintw (w, 1, 15,"'w'");
	mvwprintw (w, 2, 13,"move up");
    	mvwprintw (w, 4, 5,"'a'");
    	mvwprintw (w, 5, 2,"move left");
    	mvwprintw (w, 4, 24,"'d'");
    	mvwprintw (w, 5, 20,"move right");
    	mvwprintw (w, 7, 15,"'s'");
    	mvwprintw (w, 8, 12,"move down");
    	mvwprintw (w, 10, 1, "'u' - undo");
	mvwprintw (w, 11, 1, "'ESC' - exit without saving");
	mvwprintw (w, 12, 1, "'q' - exit, saving progress");
	mvwprintw (w, 13, 1, "'b' - enable/disable autosave");
	mvwprintw (w, 15, 1, "Automove timer: 25 seconds");
	mvprintw (22, 30, "Press any key to exit");
	wrefresh (w);
	timeout (999999);
	getch ();
}
/* functie ce muta toate elementele matricii in sus, atata timp cat exista 
o casuta ce contine valoarea 0 pe aceasta directie; daca se intalnesc 2 
numere identice, acestea se aduna; functia va returna 2048 la intalnirea
acestui numar printre elementele matricii */
int up (int m[][4], int *sc)
{
	int i, j, x, rez = 0;
	for (j = 0; j < 4; j++)
	{
		x = 5;
		for (i = 0; i < 3; i++)
		{
			if (m[i][j] == m[i+1][j] && x != i && m[i][j] != 0)
				if (x != i + 1)
				{
					m[i][j] = m[i][j] * 2;
					m[i+1][j] = 0;
					x = i;
					*sc = *sc + m[i][j];
					if (m[i][j] == 2048)
						rez = m[i][j];
				}
				if (m[i][j] == 0 && m[i+1][j] != 0)
				{
					m[i][j] = m[i+1][j];
					m[i+1][j] = 0;
					i = -1;
				}
		}
	}
	return rez;
}
/* functie ce muta toate elementele matricii la stanga, atata timp cat 
exista o casuta ce contine valoarea 0 pe aceasta directie; daca se 
intalnesc 2 numere identice, acestea se aduna; functia va returna 2048 
la intalnirea acestui numar printre elementele matricii */
int left (int m[][4], int *sc)
{
	int i, j, x, rez = 0;
	for (i = 0; i < 4; i++)
	{
		x = 5;
		for (j = 0; j < 3; j++)
		{
			if (m[i][j] == m[i][j+1] && x != j && m[i][j] != 0)
				if (x != j + 1)
				{
					m[i][j] = m[i][j] * 2;
					m[i][j+1] = 0;
					x = j;
					*sc = *sc + m[i][j];
					if (m[i][j] == 2048)
						rez = m[i][j];
				}
				if (m[i][j] == 0 && m[i][j+1] != 0)
				{
					m[i][j] = m[i][j+1];
					m[i][j+1]=0;
					j = -1;
				}
		}
	}
	return rez;
}
/* functie ce muta toate elementele matricii in jos, atata timp cat exista 
o casuta ce contine valoarea 0 pe aceasta directie; daca se intalnesc 2 
numere identice, acestea se aduna; functia va returna 2048 la intalnirea
acestui numar printre elementele matricii */
int down (int m[][4], int *sc)
{
	int i, j, x, rez = 0;
	for (j = 0; j < 4; j++)
	{
		x = 5;
		for (i = 3; i > 0; i--)
		{
			if (m[i][j] == m[i-1][j] && x != i && m[i][j] != 0)
				if (x != i - 1)
				{
					m[i][j] = m[i][j] * 2;
					m[i-1][j] = 0;
					x = i;
					*sc = *sc + m[i][j];
					if (m[i][j] == 2048)
						rez = m[i][j];
				}
				if (m[i][j] == 0 && m[i-1][j] != 0)
				{
					m[i][j] = m[i-1][j];
					m[i-1][j] = 0;
					i = 4;
				}
		}
	}
	return rez;
}
/* functie ce muta toate elementele matricii la dreapta, atata timp cat 
exista o casuta ce contine valoarea 0 pe aceasta directie; daca se 
intalnesc 2 numere identice, acestea se aduna; functia va returna 2048 
la intalnirea acestui numar printre elementele matricii */
int right (int m[][4], int *sc)
{
	int i, j, x, rez = 0;
	for (i = 0; i < 4; i++)
	{
		x = 5;
		for (j = 3; j > 0; j--)
		{
			if (m[i][j] == m[i][j-1] && x != j-1 && m[i][j] != 0)
				if (x != j)
				{
					m[i][j] = m[i][j] * 2;
					m[i][j-1] = 0;
					x = j;
					*sc = *sc + m[i][j];
					if (m[i][j] == 2048)
						rez = m[i][j];
				}
				if (m[i][j] == 0 && m[i][j-1] != 0)
				{
					m[i][j] = m[i][j-1];
					m[i][j-1] = 0;
					j = 4;
				}
		}
	}
	return rez;
}
/* functie ce initializeaza matricea cu valoarea 0; se aduaga dupa aceea 
unul din numerele 2 sau 4 intr-o pozitie aleatoare; se adauga apoi inca 
un numar aleator, folosind functia definita mai sus */
void init (int m[][4])
{
	int i, j, n;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			m[i][j] = 0;
	i = rand () % 4;
	j = rand () % 4;
	n = rand ();
	if (n % (i + j + 1) == 0)
		m[i][j] = 2;
	else
		m[i][j] = 4;
	ran (m);
}
/* functie ce calculeaza numarul de zerouri dintr-o matrice data */
int count (int a[][4])
{
	int i, j, c = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (a[i][j] == 0)
				c++;
	return c;
}
/* functie ce returneaza 0 daca nu se mai poate efectua miscari; in caz
contrar se returneaza un numar ce corespunde operatiei ce elibereaza cat
mai multe casute */
int verif (int p[][4])
{
	int s = 0, a[4][4], max = 0, sem = 0;
	cp (p, a);
	down (p, &s);
	if (cmp (a, p) != 0)
		if (count (p) > max)
		{
			max = count (p);
			sem = 1;
		}
	cp (a, p);
	left (p, &s);
	if (cmp (a, p) != 0)
		if (count (p) > max)
		{
			max = count (p);
			sem = 2;
		}
	cp (a, p);
	right (p, &s);
	if (cmp (a, p) != 0)
		if (count (p) > max)
		{
			max = count (p);
			sem = 3;
		}
	cp (a, p);
	up (p, &s);
	if (cmp (a, p) != 0)
		if (count (p) > max)
		{
			max = count (p);
			sem = 4;
		}
	cp (a, p);
	return sem;
}
/* functie ce efectueaza o miscare automat, in functie de numarul returnat 
de functia "verif ()" */
void automove (int m[][4], int *sc)
{
	int c;
	c = verif (m);
	switch (c)
	{
		case 1:
			down (m, sc);
			break;
		case 2:	
			left (m, sc);
			break;
		case 3:	
			right (m, sc);
			break;
		case 4:
			up (m, sc);
			break;
	}
}
// functia principala a jocului; ea controleaza majoritatea actiunilor 
void game (int m[][4], int *s)
{
	int n = *s, e = 0, sc = *s, b = 0, sem = 1, r, a, c=0, sav = 0;
	int p[4][4], q[4][4], tmr;
	// se initializeaza perechile de culori
	incol ();
	keypad (stdscr, TRUE);
	while (1)
	{
		// implementarea functiei de autosave, din 10 in 10 miscari
		if (sav == 10)
		{
			scriere (m, &sc);
			sav = 0;
		}
		/* se verifica daca apare numarul 2048; daca da utilizatorul 
		este intrebat daca doreste sa continue; daca da nu se va 
		mai intra in if */
		if (!c && r == 2048)
		{
			mvprintw (7, 4, "You Win!");
			mvprintw (8, 1, "Continue? (y/n)");
			timeout (99999999);
			e = 0;
			while (!e)
			{
				a = getch ();
				switch (a)
				{
					/* in cazul in care utilizatorul nu 
					doreste sa continue jocul, apare 
					tabela highscores, ce afiseaza 
					cele mai mari 5 scoruri si pe cel 
					curent, daca acesta este mai mic 
					decat scorurile existente */
					case 'n':
						highscores (sc);
						return ;
					case 'y':
						c = 1;
						afisare (m, sc, sav);
						e = 1;
						break;
				}
			}
		}
		cp (m, p);
		sc = n;
		e = 0;
		do
		{
			/* se realizeaza afisarea ecranului in fiecare 
			secunda pentru a arata ceasul in timp real; se
			verifica daca au trecut 25 de secunde fara input */
			tmr = 0;
			do
			{
				timeout (1000);
				tmr++;
				afisare (m, sc, sav);
				a = getch ();
				if (a != -1)
					tmr = 26;
			}while (tmr < 25);
			e = 1;
			/* daca au trecut 25 de secunde fara sa primeasca 
			un input, programul realizeaza o miscare automat */
			if (tmr == 25)
			{
				automove (m, &sc);
				sem = 1;
			}
			else
			{
				switch (a)
				{
					/* functia undo nu poate fi apelata 
					de doua ori la rand */
					case 'u':
						if (sem == 1)
						{
							cp (q, m);
							sc = b;
							sem = 0;
						}
						break;
					case 'w':
						r = up (m, &sc);
						sem = 1;
						break;
					case 'a':
						r = left (m, &sc);
						sem = 1;
						break;
					case 's':
						r = down (m, &sc);
						sem = 1;
						break;
					case 'd':
						r = right (m, &sc);
						sem = 1;
						break;
					case 'q':
						scriere (m, &sc);
						return ;
					/* la apasarea tastei ESCAPE jocul se
					va intoarce in meniu fara a salva 
					matricea */
					case 27:
						return;
					/* functia autosave poate fi 
					activata sau dezactivata prin 
					apasarea tastei "b" */
					case 'b':
						if (sav > 10)
							sav = -1;
						else
							sav = 15;
						afisare (m, sc, sav);
						e = 0;
						break;
					case 270:
						help ();
						afisare (m, sc, sav);
						e = 0;
						break;
					default :
						e = 0;
						break;
				}
			}
		// programul va astepta un input valid
		}while (e == 0);
		/* se verifica daca mai poate fi efectuata o miscare; daca 
		nu atunci se afiseaza un mesaj corespunzator si utilizatorul
		este trimis inapoi la meniu, in afara cazului in care se 
		apeleaza functia undo */
		if (verif (p) == 0)
		{
			if (a == 'u')
			{
				cp (q, m);
				sc = b;
				sem = 0;
			}
			else
			{
				/* jucatorul este informat ca a pierdut 
				jocul; programul asteapta un input sau 25
				de secunde si apoi afiseaza tabela 
				highscores */
				attron (A_BOLD);
				mvprintw (13, 3, "Game Over");
				mvprintw (14, 1, "Press any key...");
				attroff (A_BOLD);
				phscore (sc);
				timeout (999999);
				a = getch ();
				highscores(sc);
				return ;
			}
		}
		b = n;
		n = sc;
		/* se verifica daca miscarea efectuata este valida; daca da 
		se adauga un nou element aleator si se afiseaza matricea; 
		daca nu se afiseaza un mesaj corespunzator si se asteapta 
		un nou input */
		if (cmp (m, p) == 0)
		{
			afisare (m, sc, sav);
			mvprintw (13, 1	, "Incorrect move");
			timeout (600);
			a = getch();
		}
		else
		{
			if (sem == 1)
				ran (m);
			afisare (m, sc, sav);
			sav++;
		}
		cp (p, q);
	}
}
// functie ce controleaza meniul programului
int menu ()
{
	WINDOW *w;
	char list[4][12] = {"New Game", "Resume", "Highscores", "Quit"};
	char name[12];
	int ch, i = 0, e = 0, sc, m[4][4], g = 0, sem = 0;
	start_color ();
	while (!g)
	{
		erase ();
	w = newwin(6, 16, 8, (COLS - 22) / 2);
		box (w, 0, 0);
		attron (A_BOLD);
		init_pair (1, COLOR_YELLOW, COLOR_BLACK);
		attron (COLOR_PAIR (1));
		mvprintw (3, (COLS - 8) / 2, "2048");
		attroff (A_BOLD);
		attroff (COLOR_PAIR (1));
		refresh ();
		for(i = 0; i < 4; i++)
		{
			if(i == 0)
				wattron (w, A_STANDOUT);
			else
				wattroff (w, A_STANDOUT);
				sprintf (name, "%-12s", list[i]);
				mvwprintw (w, i + 1, 3, "%s", name);
		}
		wrefresh (w);
		keypad (w, TRUE);
		curs_set (0);
		i = 0;
		/* navigarea prin meniu; daca fisierul "save" nu exista 
		optiunea resume nu va fi disponibila */
		while(!e)
		{
			if (citire (m, &sc) == 1)
				sem = 0;
			else
				sem = 1;
			ch = wgetch (w);
			sprintf (name, "%-12s", list[i]);
			mvwprintw (w, i + 1, 3, "%s", name);
			switch(ch)
			{
				case KEY_UP:
					i--;
					i = ( i < 0 ) ? 3 : i;
					if (sem == 0)
						if (i == 1)
							i--;
					break;
				case KEY_DOWN:
					i++;
					i = ( i > 3 ) ? 0 : i;
					if (sem == 0)
						if (i == 1)
							i++;
					break;
				case KEY_ENTER:
					e = 1;
					break;
			}
			if (ch == 10)
				e = 1;
			wattron (w, A_STANDOUT);
			sprintf (name, "%-12s", list[i]);
			mvwprintw (w, i + 1, 3, "%s", name);
			wattroff (w, A_STANDOUT);
		}
		// selectarea optiunii alese de utilizator
		switch (i)
		{
			case 0:
				init (m);
				sc = 0;
				game (m, &sc);
				break;
			case 1:
				citire (m, &sc);
				game (m, &sc);
				break;
			case 2:
				highscores(-1);
				break;
			case 3:
				g = 1;
				break;
		}
		e = 0;
		erase ();
		refresh ();
		wrefresh (w);
	}
	return 0;
}
int main ()
{
	initscr ();
	raw ();
	noecho ();
	srand ( (unsigned) time (NULL));
	menu ();
	refresh ();
	endwin ();
	return 0;
}
