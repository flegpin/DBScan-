#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cmath>
#include <ctime>
using namespace std;

//implementation de l'algorithme DBscan qui prend entree trois parametre, le fichier, epsilon, et minpts


//structure representant un point
struct coordonnees
{
	int x;	//l'abscisse
	int y;	//l'ordonnee
	bool visite;	//pour determiner si un point a ete visité ou non
	int nombre;	//contient le nombre de voisin de chaque points
	int categorie;		//la categorie a laquelle le point appartient
	vector<int> voisin; //indice des points qui sont dans l'entourage de ce point
};

class DBScan
{
private:
	vector<coordonnees> points;	//va contenir l'ensemble des points
	string donnees_entrainement;
	int minPts;
	int epsilon;
	int categorie;	//le nombre different de categories qui existent 
	
public:
	DBScan();		//constructeur
	void lecture();	//fonction de lecture du fichier d'entree
	
	void categorisation();	//application de l'algorithme pour categoriser les points
};

DBScan::DBScan()
{
	//invite de commande pour entrere= les données 
	cout << "Bienvenue dans le programme de prediction id3" << endl;
	cout << "Veuillez entrer le nom du fichier contenant les données d'entrainement" << endl;
	cin >> donnees_entrainement;
	cout << "Merci, quel est la valeure de minPts ?" << endl;
	cin >> minPts;
	cout << "Maintenant la valeure d'epsilon" << endl;
	cin >> epsilon;
	system("cls");

	categorie = 1;//categorie de depart

	lecture();
	categorisation();
}
void DBScan::lecture()
{
	//lecture du fichier contenant les données
	ifstream Fichier(donnees_entrainement.c_str());
	if (Fichier)
	{
		while (Fichier.good())
		{
			string ligne, mot;
			getline(Fichier, ligne);//recupere la premiere ligne
			stringstream ss(ligne);

			if (ligne.empty())	//si elle n'est pas vide
				continue;

			coordonnees *temp1 = new coordonnees; //structure qui va contenir les coordonnes du point et sera insere dans le vecteur
			bool abscisse = true;//nous permettra de savoir si c'est l'abscise ou l'ordonnee que l'on insere
			while (!ss.eof())
			{
				int temp2 = 0;	//variable temporaire qui stocke le point lors de la conversion de string en int
				ss >> mot;//on recupere le premier mot

				if (isalnum(mot[1]))//au cas au on a deux caractere
				{
					temp2 = mot[0] - 48;	//on insere le premier chiffre
					temp2 *= 10;			//on multiplie par 10, pour le decalage du deuxieme chiffre
					temp2 +=mot[1] - 48;	//on ajoute le deuxieme chiffre
				}
				else
					temp2 = mot[0] - 48;
				
				if (abscisse)	//on insere l'abscisse
				{
					temp1->x = temp2;
					abscisse=false;	//on met a false
				}
				else
				{
					temp1->y = temp2;
					abscisse = true;//on met a true pour la prochaine iteration
				}
			}
			//on insere ces coordonnes dans le vecteur de points
			temp1->visite = false;	//le point na pas été visité au debut
			temp1->categorie = 0;			//il n'appartient a aucune categorie
			temp1->nombre = 0;	//il na aucun point a son voisinage avant le calcul
			temp1->voisin;
			points.push_back(*temp1);
		}
	}
	else
		cout << "Erreur d'ouverture" << endl;
}

void DBScan::categorisation()
{
	vector<coordonnees>::iterator it;
	//debut de la categorisation, pour chaque point
	int i = 0;
	for (it = points.begin(); it < points.end(); it++)
	{
		vector<coordonnees>::iterator it2;
		//on parcour la distance par rapport a tous les autres points
		int j = 0;
		for (it2 = points.begin(); it2 < points.end(); it2++)
		{
			//calcul de la distance entre les deux points
			float dist = sqrt(pow(it->x - it2->x, 2) + pow(it->y - it2->y, 2));
			cout << "La distance entre le point " << i << " et le point " << j << " est : " << dist << endl;
			if ((dist < epsilon) && (i!=j))	// si la distance entre la deux points est inferieur a epsilon et ce n'est pas le meme point
			{
				it->voisin.push_back(j); // on insere l'indice du point
				it->nombre++;			 // on incremente le nombre de voisin
				cout << "----On l'ajoute" << endl;
			}
			j++;
		}
		i++;
	}
	cout << endl<<endl<<"Tous les points contiennent maintenant le nombre de voisin" << endl;
	i = 0;
	for (it = points.begin(); it < points.end(); it++)
	{
		cout << "Les voisins du point " <<it->x<<"--"<<it->y << " sont: " << endl;
		for (int y = 0; y < it->nombre; y++)
		{
			cout << points[it->voisin[y]].x << "--" << points[it->voisin[y]].y << endl;
		}
		i++;
	}
	system("pause");
	system("cls");

	// on va encore parcourir les points
	for (it = points.begin(); it < points.end(); it++)
	{
		bool categorie_inc = false;	//prendra la valeur true si on entre dans la condition en bas, sinon la categorie sera incrementee inutilement
		if (it->visite == false)	//si le point est visité
		{
			categorie_inc = true;
			it->visite = true;	//on le marque comme visité
			cout << "On marque le point " << it->x << "--" << it->y << " comme visite" << endl;
			if (it->nombre >= minPts)	//si il a au moins minPts dans son entourage
			{
				// on l'ajoute dans une categorie
				it->categorie = categorie;
				cout << "il a au moins minPts voisin, on met le dans la categorie " << it->categorie << endl;

				for (int i = 0; i < it->voisin.size(); i++)	//boucle qui va parcourir les voisin du point
				{
					//on verifie si il est marque
					if (!points[it->voisin[i]].visite)//	it->voisin[i] contient l'indice du voisin, donc on peut le localiser 
					{									//  dans le vecteur points a partir de l'indice
						cout << "	son voisin " << points[it->voisin[i]].x << "--" << points[it->voisin[i]].y << " n'est pas marque, on le marque" << endl;
						points[it->voisin[i]].visite = true; //on le marque
						if (points[it->voisin[i]].nombre >= minPts)	// si il a au moins minPts voisins
						{
							cout << "	il a au moins minPts voisin, on le met dans la categorie " << it->categorie << endl;
							//on ajoute ces points voisins comme voisin du pere
							points[it->voisin[i]].categorie = categorie;	//on l'ajoute a cette categorie

							//affichage des voisin du point
							cout << "\tSes voisins sont: " << endl;
							for (int y = 0; y < points[it->voisin[i]].nombre; y++)
							{
								cout <<"\t\t"<< points[points[it->voisin[i]].voisin[y]].x << "--" << points[points[it->voisin[i]].voisin[y]].y << endl;
							}

							for (int y = 0; y < points[i].nombre; y++)
							{
								bool present = false;
								int z = 0;	//pour regler un probleme d'affichage multiple
								//on verifie si le point n'est pas deja present comme indice dans le point avant de l'ajouter
								for (vector<int>::iterator it3 = it->voisin.begin(); it3 < it->voisin.end(); it3++) //it3 parcour l'indice des voisins du voisin concerné, c'est le second voisin	
								{
									if (*it3 == points[it->voisin[i]].voisin[y])	//si les indice sont les meme, se point est deja present dans le pere comme etant un voisin
									{
										cout << "\t\tLe voisin " << points[points[it->voisin[i]].voisin[y]].x << "--" << points[points[it->voisin[i]].voisin[y]].y << " du point est deja present dans le pere" << endl;
										present = true;
									}
									else if (points[points[it->voisin[i]].voisin[y]].x == it->x && points[points[it->voisin[i]].voisin[y]].y == it->y)
									{	//dans le cas ou le second voisin est le pere lui meme 
										if (z == 0)
										{
											z++;
											cout << "\t\tLe voisin " << points[points[it->voisin[i]].voisin[y]].x << "--" << points[points[it->voisin[i]].voisin[y]].y << " du point correspond au pere" << endl;
										}
										present = true;
									}
								}
								if (present == false)	//on ajoute le voisin seulement si il n'est pas deja present
								{
									it->nombre++;	// augmentation du nombre de voisin
									it->voisin.push_back(points[i].voisin[y]);
									cout << "\t\tCe voisin " << points[points[i].voisin[y]].x << "--" << points[points[i].voisin[y]].y << " n'est Pas present dans le pere, on l'ajoute" << endl;

								}							
							}
						}
						else
							cout << "\til na pas assez de points voisin " << endl;

					}
					else
						cout << "son voisin " << points[it->voisin[i]].x << "--" << points[it->voisin[i]].y << " est deja marque ou c'est le meme point, on passe a la suite" << endl;

				}
			}
			else
			{
					cout << "il na pas assez de points voisin, c'est du bruit " << endl;
				
			}
			system("pause");
			
		}
		else
			cout << "Le point " << it->x << "--" << it->y << " est deja visite" <<endl<<endl;
		if (categorie_inc==true)
			categorie++;

	}
	
	system("pause");
	system("cls");

	cout << "La categorisation est terminee a ce stade, affichage des differentes categories" << endl;

	for (int i = 1; i < categorie-1; i++)
	{
		cout << endl<<"Categorie " << i << endl;
		for (vector<coordonnees>::iterator it = points.begin(); it < points.end();it++)
		{
			if (it->categorie==i)
				cout << it->x << "--"<<it->y<<endl;
		}
	}
	//affichage du bruit
	cout <<endl<< "Bruit"<< endl;
	for (vector<coordonnees>::iterator it = points.begin(); it < points.end(); it++)
	{
		if (it->categorie == 0)
			cout << it->x << "--" << it->y << endl;
	}
}


int main()
{
	srand(time(NULL));
	clock_t time;//fonction gerant le temps,retourne le nombre de coup d'horloge
	time = clock(); //debut mesure du temps

	DBScan algo;
	cout << "|Duree d'execution " << (double)time / CLOCKS_PER_SEC << " |";
	system("pause");
	return 0;
}