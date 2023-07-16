#include <ncurses.h>
#include "labIO.h"
#include<iostream>
#include<fstream>
#include<unistd.h>
#include <chrono>

using namespace std;
using namespace chrono;

void init(Case & c)
{
	c.N=c.S=c.E=c.W=false;
	c.etat=false;
}

void init(laby & L, int p, int q)
{
	L.p=p; L.q=q;
	L.tab=new Case *[p];
	for(int i=0;i<p;i++) L.tab[i]=new Case[q];
	for(int i=0;i<p;i++)
	{
		for(int j=0;j<q;j++) init(L.tab[i][j]);
	}
}

laby *lire_Laby(const char  *filename)
{
	ifstream my_cin;

	my_cin.open(filename);

	laby *L;
	L=new laby;
	my_cin>>(*L).p>>(*L).q;
	init((*L),(*L).p,(*L).q);

	for(int i=0;i<(*L).p;i++)
	{
		for(int j=0;j<(*L).q;j++)
		{
			my_cin>>(*L).tab[i][j].W>>(*L).tab[i][j].N>>(*L).tab[i][j].S>>(*L).tab[i][j].E;
		}
	}
	return L;
}
void afficheChemin(laby &L);
void pauseMenu(laby &L);
/*
int lireScore()
{
	ifstream my_cin('score.txt');
	string score;
	score = getline(my_cin, score);
	int s = int(score);
}
*/
void enregistrerScore(int score)
{
	ofstream my_cout;
	my_cout.open("score.txt");
	my_cout<<"votre score est de : "<<score<<" secondes";
}

void deplacement(laby &L)
{
	int x{0}, y{0};
	Mark_Case(y, x);
	int commande = 0;
	auto start = high_resolution_clock::now();
	char s[1];
	while(true)
	{
		
		auto end = high_resolution_clock::now();
		mvprintw(10, 45, "%ld", duration_cast<seconds>(end - start).count());
		commande = LireCommande(false);
		//gauche
		if (L.tab[y][x].W && commande == 9 && x > 0 && x <= L.p - 1)
		{
			UMark_Case(y, x);
			x--;
			Mark_Case(y, x);
		}
		//droite
		if (L.tab[y][x].E && commande == 3 && x >= 0 && x < L.p - 1)
		{
			UMark_Case(y, x);
			x++;
			Mark_Case(y, x);
		}
		//bas
		if (L.tab[y][x].S &&commande == 6 && y >= 0 && y < L.q - 1)
		{
			UMark_Case(y, x);
			y++;
			Mark_Case(y, x);
		}
		//haut
		if (L.tab[y][x].N && commande == 12 && y > 0 && y <= L.q - 1)
		{
			UMark_Case(y, x);
			y--;
			Mark_Case(y, x);
		}

		if (commande == -1)
		{
			pauseMenu(L);
		}

		if (x == L.q-1 && y == L.p-1)
		{
			mvprintw(L.p*2+7, 0, "+-------+\n|gagné !|\n+-------+");
			auto end = high_resolution_clock::now();
			enregistrerScore(duration_cast<seconds>(end - start).count());
			mvprintw(L.p*2+12, 0, "score enregistré dans score.txt !");
			pauseMenu(L);

			//usleep(100000000);
			//refresh();

			break;
		}
	}
}

void deplacement2P(laby &L)
{
	int x{0}, y{0};
	int i{0}, j{L.q-1};
	Mark_Case(y, x);
	Mark_CaseJ2(i, j);
	int commande = 0;
	int commandeJ2 = 0;
	while(true)
	{
		commande = LireCommande(true), commandeJ2 = LireCommandeJ2();
		
		//gauche
		if (commande == 9 && x > 0 && x <= L.p - 1 && L.tab[y][x].W)
		{
			UMark_Case(y, x);
			x--;
			Mark_Case(y, x);
		}
		//droite
		if (commande == 3 && x >= 0 && x < L.p - 1 && L.tab[y][x].E)
		{
			UMark_Case(y, x);
			x++;
			Mark_Case(y, x);
		}
		//bas
		if (commande == 6 && y >= 0 && y < L.q - 1 && L.tab[y][x].S)
		{
			UMark_Case(y, x);
			y++;
			Mark_Case(y, x);
		}
		//haut
		if (commande == 12 && y > 0 && y <= L.q - 1 && L.tab[y][x].N)
		{
			UMark_Case(y, x);
			y--;
			Mark_Case(y, x);
		}

		//gauche j2
		if (commandeJ2 == 9 && j > 0 && j <= L.p - 1 && L.tab[i][j].W)
		{
			UMark_Case(i, j);
			j--;
			Mark_CaseJ2(i, j);
		}
		//droite j2
		if (commandeJ2 == 3 && j >= 0 && j < L.p - 1 && L.tab[i][j].E)
		{
			UMark_Case(i, j);
			j++;
			Mark_CaseJ2(i, j);
		}
		//bas j2
		if (commandeJ2 == 6 && i >= 0 && i < L.q - 1 && L.tab[i][j].S)
		{
			UMark_Case(i, j);
			i++;
			Mark_CaseJ2(i, j);
		}
		//haut j2
		if (commandeJ2 == 12 && i > 0 && i <= L.q - 1 && L.tab[i][j].N)
		{
			UMark_Case(i, j);
			i--;
			Mark_CaseJ2(i, j);
		}

		if (commandeJ2 == -1)
		{
			pauseMenu(L);
		}

		if (x == L.p-1 && y == L.q-1)
		{
			mvprintw(L.p*2+7, 0, "+----------+\n|J1 gagné !|\n+----------+");
		}

		if (i == L.p-1 && j == 0)
		{
			mvprintw(L.p*2+7, 0, "+----------+\n|J2 gagné !|\n+----------+");
		}
	}
}

//fonctions de la pile

void init(pile &p, int t)
{
	p.taille = t;
	p.nbe = 0;
	p.T = new couple[t];
}

bool pleine(pile &p) {
    return p.nbe == p.taille;
}

bool vide(pile &p) {
    return p.nbe == 0;
}

couple depiler(pile & p)
{
	if(p.nbe>0)
	{
		p.nbe--;
		return(p.T[p.nbe]);
	}
	else
	{
		cout<<"la pile est vide"<<endl;
		exit(1);
	}
}

bool empiler(pile &p, couple element) {

    if (pleine(p)) {
        return false;
    }

    p.T[p.nbe] = element;
    p.nbe++;
    return true;
}

void ouvrirDoite(laby &L, pile &P, couple &c)
{
	couple *c1 = new couple;
	c1->i = c.i;
	c1->j = c.j+1;
	L.tab[c1->i][c1->j].etat = true;
	L.tab[c.i][c.j].E = true;
	L.tab[c1->i][c1->j].W = true;
	empiler(P, c);
	empiler(P, *c1);

	delete c1;
}

void ouvrirGauche(laby &L, pile &P, couple &c)
{
	couple *c1 = new couple;
	c1->i = c.i;
	c1->j = c.j-1;
	L.tab[c1->i][c1->j].etat = true;
	L.tab[c.i][c.j].W = true;
	L.tab[c1->i][c1->j].E = true;
	empiler(P, c);
	empiler(P, *c1);

	delete c1;
}

void ouvrirHaut(laby &L, pile &P, couple &c)
{
	couple *c1 = new couple;
	c1->i = c.i-1;
	c1->j = c.j;
	L.tab[c1->i][c1->j].etat = true;
	L.tab[c.i][c.j].N = true;
	L.tab[c1->i][c1->j].S = true;
	empiler(P, c);
	empiler(P, *c1);

	delete c1;
}

void ouvrirBas(laby &L, pile &P, couple &c)
{
	couple *c1 = new couple;
	c1->i = c.i+1;
	c1->j = c.j;
	L.tab[c1->i][c1->j].etat = true;
	L.tab[c.i][c.j].S = true;
	L.tab[c1->i][c1->j].N = true;
	empiler(P, c);
	empiler(P, *c1);

	delete c1;
}

laby *my_creation(int p, int q)
{
	//création du laby fermé
	laby *L;
	L = new laby;
	L -> p = p;
	L -> q = q;
	L -> tab = new Case*[p];

	for (int i = 0; i < p; ++i)
	{
		L -> tab[i] = new Case[q];
		for (int j = 0; j < q; ++j)
		{
			L -> tab[i][j].N = false;
			L -> tab[i][j].W = false;
			L -> tab[i][j].S = false;
			L -> tab[i][j].E = false;
		}
	}

	pile *P = new pile;
		
	init((*P), (*L).p*(*L).q);
	couple c;
	c.i = 0;
	c.j = 0;
	L -> tab[c.i][c.j].etat = true;
	empiler(*P, c);

	int selecteur;

	while(!vide(*P))
	{
		selecteur = rand()%4;
		//cout<<selecteur<<endl;
		c = depiler(*P);
		//cout<<c.i<<endl;

		//haut
		if (selecteur == 0)
		{
			if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
			{
				ouvrirHaut(*L, *P, c);
			}
			else
			{
				selecteur = rand()%3; //j'en suis ici
				if (selecteur == 0)
				{
					if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
					{
						ouvrirBas(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur == 0)
						{
							if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
							else if(c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}

						}
						if (selecteur == 1)
						{
							if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
							else if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
						}
					}
				}

				if (selecteur == 1)
				{
					if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
					{
						ouvrirGauche(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur == 0)
						{
							if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
							else if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
						}
						if (selecteur == 1)
						{
							if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
							else if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
						}
					}
				}
				if (selecteur == 2)
				{
					if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
					{
						ouvrirDoite(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur = 0)
						{
							if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
							else if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
						}
						if (selecteur = 1)
						{
							if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
							else if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
						}
						

					}
				}
			}

		}
		//bas
		if (selecteur == 1)
		{
			if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
			{
				ouvrirBas(*L, *P, c);
			}
			else
			{
				selecteur = rand()%3; //j'en suis ici
				if (selecteur == 0)
				{
					if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
					{
						ouvrirHaut(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur == 0)
						{
							if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
							else if(c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}

						}
						if (selecteur == 1)
						{
							if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
							else if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
						}
					}
				}

				if (selecteur == 1)
				{
					if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
					{
						ouvrirGauche(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur == 0)
						{
							if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
							else if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
						}
						if (selecteur == 1)
						{
							if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
							else if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
						}
					}
				}
				if (selecteur == 2)
				{
					if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
					{
						ouvrirDoite(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur = 0)
						{
							if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
							else if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
						}
						if (selecteur = 1)
						{
							if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
							else if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
						}
						

					}
				}
			}
		}
		//gauche
		if (selecteur == 2)
		{
			if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
			{
				ouvrirGauche(*L, *P, c);
			}
			else
			{
				selecteur = rand()%3; //j'en suis ici
				if (selecteur == 0)
				{
					if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
					{
						ouvrirBas(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur == 0)
						{
							if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
							else if(c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}

						}
						if (selecteur == 1)
						{
							if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
							else if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
						}
					}
				}

				if (selecteur == 1)
				{
					if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
					{
						ouvrirHaut(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur == 0)
						{
							if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
							else if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
						}
						if (selecteur == 1)
						{
							if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
							else if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
							{
								ouvrirDoite(*L, *P, c);
							}
						}
					}
				}
				if (selecteur == 2)
				{
					if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
					{
						ouvrirDoite(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur = 0)
						{
							if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
							else if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
						}
						if (selecteur = 1)
						{
							if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
							else if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
						}
						

					}
				}
			}
		}
		//droite
		if (selecteur == 3)
		{
			//cout<<c.j<<endl;
			if (c.j+1 >= 0 && c.j+1 < L -> q && !L -> tab[c.i][c.j+1].etat)
			{
				ouvrirDoite(*L, *P, c);
			}
			else
			{
				selecteur = rand()%3; //j'en suis ici
				if (selecteur == 0)
				{
					if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
					{
						ouvrirBas(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur == 0)
						{
							if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
							else if(c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}

						}
						if (selecteur == 1)
						{
							if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
							else if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
						}
					}
				}

				if (selecteur == 1)
				{
					if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
					{
						ouvrirGauche(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur == 0)
						{
							if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
							else if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
						}
						if (selecteur == 1)
						{
							if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
							else if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
							{
								ouvrirHaut(*L, *P, c);
							}
						}
					}
				}
				if (selecteur == 2)
				{
					if (c.i-1 >= 0 && c.i-1 <= L -> p && !L -> tab[c.i-1][c.j].etat)
					{
						ouvrirHaut(*L, *P, c);
					}
					else
					{
						selecteur = rand()%2;
						if (selecteur = 0)
						{
							if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
							else if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
						}
						if (selecteur = 1)
						{
							if (c.j-1 >= 0 && c.j-1 <= L -> q && !L -> tab[c.i][c.j-1].etat)
							{
								ouvrirGauche(*L, *P, c);
							}
							else if (c.i+1 >= 0 && c.i+1 < L -> p && !L -> tab[c.i+1][c.j].etat)
							{
								ouvrirBas(*L, *P, c);
							}
						}
						

					}
				}
			}
		}
		
	}
	delete P;

	return L;
}



void enregistrer(laby &L)
{
	ofstream my_cout;
	my_cout.open("test.txt");
	my_cout<<L.p<<" "<<L.q;

	for(int i = 0; i<L.p; i++)
	{
		for(int j = 0; j<L.q; j++)
		{
			my_cout<<" ";
			my_cout<<L.tab[i][j].W<<" ";
			my_cout<<L.tab[i][j].N<<" ";
			my_cout<<L.tab[i][j].S<<" ";
			my_cout<<L.tab[i][j].E;

		}
	}
}

pile *explorer(laby &L)
{
	pile *P = new pile;

	init((*P), (L).p*(L).q);
	couple c;
	c.i = 0;
	c.j = 0;
	L.tab[c.i][c.j].marq = true;
	empiler(*P, c);
	Mark_Case(c.i, c.j);


	while(c.i != L.p-1 || c.j != L.q-1)
	{
		usleep(100000);
		refresh();
		Mark_Case_Pile(*P);
		//cout<<selecteur<<endl;
		//cout<<c.i<<endl;

		//bas
		if (L.tab[c.i][c.j].S && !L.tab[c.i+1][c.j].marq)
		{
			empiler(*P, c);
			c.i ++;
			L.tab[c.i][c.j].marq = true;
			Mark_Case_Pile(*P);
			empiler(*P, c);

		}

		if(c.i == L.p-1 && c.j == L.q-1)
		{
			Mark_Case(c.i, c.j);
			break;
		}

		//haut
		if (L.tab[c.i][c.j].N && !L.tab[c.i-1][c.j].marq)
		{
			empiler(*P, c);
			c.i --;
			L.tab[c.i][c.j].marq = true;
			Mark_Case_Pile(*P);
			empiler(*P, c);

		}

		if(c.i == L.p-1 && c.j == L.q-1)
		{
			Mark_Case(c.i, c.j);
			break;
		}

		//droite
		if (L.tab[c.i][c.j].E && !L.tab[c.i][c.j+1].marq)
		{
			empiler(*P, c);
			c.j ++;
			L.tab[c.i][c.j].marq = true;
			Mark_Case_Pile(*P);
			empiler(*P, c);

		}

		if(c.i == L.p-1 && c.j == L.q-1)
		{
			Mark_Case(c.i, c.j);
			break;
		}

		//gauche
		if (L.tab[c.i][c.j].W && !L.tab[c.i][c.j-1].marq)
		{
			empiler(*P, c);
			c.j --;
			L.tab[c.i][c.j].marq = true;
			Mark_Case_Pile(*P);
			empiler(*P, c);

		}

		if(c.i == L.p-1 && c.j == L.q-1)
		{
			Mark_Case(c.i, c.j);
			break;
		}

		else
		{
			UMark_Case(c.i, c.j);
			c = depiler(*P);
		}
	}

	return P;
}

pile *calculeChemin(laby &L)
{
	pile *P = new pile;

	init((*P), (L).p*(L).q);
	couple c;
	c.i = 0;
	c.j = 0;
	L.tab[c.i][c.j].marq = true;
	empiler(*P, c);


	while(c.i != L.p-1 || c.j != L.q-1)
	{
		//cout<<selecteur<<endl;
		//cout<<c.i<<endl;

		//bas
		if (L.tab[c.i][c.j].S && !L.tab[c.i+1][c.j].marq)
		{
			empiler(*P, c);
			c.i ++;
			L.tab[c.i][c.j].marq = true;
			empiler(*P, c);

		}

		//haut
		if (L.tab[c.i][c.j].N && !L.tab[c.i-1][c.j].marq)
		{
			empiler(*P, c);
			c.i --;
			L.tab[c.i][c.j].marq = true;
			empiler(*P, c);

		}

		//droite
		if (L.tab[c.i][c.j].E && !L.tab[c.i][c.j+1].marq)
		{
			empiler(*P, c);
			c.j ++;
			L.tab[c.i][c.j].marq = true;
			empiler(*P, c);

		}

		//gauche
		if (L.tab[c.i][c.j].W && !L.tab[c.i][c.j-1].marq)
		{
			empiler(*P, c);
			c.j --;
			L.tab[c.i][c.j].marq = true;
			empiler(*P, c);

		}

		else
		{
			c = depiler(*P);
		}
	}
	empiler(*P, c);

	return P;
}

void afficheChemin(laby &L)
{
	clear();
	Show_Lab(L);
	pile *solution = new pile;
    solution = calculeChemin(L);
	Mark_Case_Pile(*solution);
	mvprintw(L.p*2+1, 0, "+---------------+\n|               |\n|voici la       |\n|solution :     |\n|               |\n+---------------+");
}

void game()
{
	srand(time(NULL));
    laby *L1 = my_creation(10, 10);
    InitCurses();
	Show_Lab(*L1);
	int ch = wgetch(stdscr) ;
	mvprintw(L1->p*2+1, 0, "+---------------+\n|déplacez vous  |\n|avec les       |\n|fleches ou     |\n|z, q, s et d   |\n|[F1] : menu    |\n+---------------+");
    deplacement(*L1);
    EndCurses();
}

void game2Players()
{
	srand(time(NULL));
    laby *L1 = my_creation(10, 10);
    InitCurses();
	Show_Lab(*L1);
	int ch = wgetch(stdscr) ;
	mvprintw(L1->p*2+1, 0, "+---------------+\n|J1 : z,q,s et d|\n|J2 : Flèches   |\n+---------------+");
    deplacement2P(*L1);
    EndCurses();
}

void pauseMenu(laby &L)
{
	mvprintw(L.p*2+7, 0, "+-----------------------------------+\n|<- : montrer solution              |\n|-> : montrer solution en temps réel|\n|haut : écrire le laby             |\n+-----------------------------------+");
	int commande;
	while(true)
	{
		commande = LireCommande(false);
		if (commande == 9)
		{
			afficheChemin(L);
		}
		if (commande == 3)
		{
			explorer(L);
		}

		if (commande == 12)
		{
			enregistrer(L);
			mvprintw(L.p*2+7, 0,"eregistré !");
		}
	}
}

void mainMenu()
{
	int choix = 0;
	cout<<"[1] jouer seul\n";
	cout<<"[2] jouer avec un ami\n";
	cout<<"[3] Voir le dernier laby écrit\n";
	cout<<"\nfaite votre choix : \n";
	while(choix < 1 || choix > 3)
	{
		cin>>choix;
	}

	if (choix == 1)
	{
		game();
	}

	if (choix == 2)
	{
		game2Players();
	}

	if (choix == 3)
	{
		laby *L1 = lire_Laby("test.txt");
		Show_Lab(*L1);
	}
}



int main()
{
    //laby *L1=lire_Laby("test.txt");
    
    mainMenu();

}