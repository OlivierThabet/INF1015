#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp" // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"		   // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion //}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{
template <typename T>
T lireType(istream &fichier)
{
	T valeur{};
	fichier.read(reinterpret_cast<char *>(&valeur), sizeof(valeur));
	return valeur;
}
#define erreurFataleAssert(message) assert(false && (message)), terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;
size_t lireUintTailleVariable(istream &fichier)
{
	uint8_t entete = lireType<uint8_t>(fichier);
	switch (entete)
	{
	case enteteTailleVariableDeBase + 0:
		return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase + 1:
		return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase + 2:
		return lireType<uint32_t>(fichier);
	default:
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
	}
}

string lireString(istream &fichier)
{
	string texte;
	texte.resize(lireUintTailleVariable(fichier));
	fichier.read((char *)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion //}

// TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.
void ListeFilms::ajouterFilm(Film *film)
{
	int capacite = getCapacite();
	int nElements = getNElements();
	if (capacite < nElements + 1)
	{
		setCapacite(max(capacite * 2, 1));
		Film **nouveauxElements;
		nouveauxElements = new Film *[getCapacite()];
		for (int i = 0; i < nElements; i++)
		{
			nouveauxElements[i] = elements[i];
		}
		nouveauxElements[nElements] = film;
		delete[] elements;
		elements = nouveauxElements;
		/*for (int i = 0; i<nElements; i++)
		{
			nouveauxElements[i] = nullptr;
		}*/
		nouveauxElements = nullptr;
		delete[] nouveauxElements;
		incrementerNElements();
	}
	else
	{
		elements[nElements] = film;
		incrementerNElements();
	}
}

// TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
void ListeFilms::supprimerFilm(const Film *film)
{
	for (int i = 0; i < getNElements(); i++)
	{
		if (elements[i] == film)
		{
			elements[i] = nullptr;
			decrementerNElements();
			break;
		}
	}
}

// TODO: Une fonctions pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
Acteur *ListeFilms::trouverActeur(string nomActeur) const
{
	if (getNElements() != 0)
	{
		for (const auto &film : span(elements, getNElements()))
		{
			if (film->acteurs.nElements != 0)
			{
				for (const auto &acteur : span(film->acteurs.elements, film->acteurs.nElements))
				{
					if (acteur != nullptr && acteur->nom == nomActeur)
					{
						return acteur;
					}
				}
			}
		}
	}
	return nullptr;
}

// TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
Acteur *ListeFilms::lireActeur(istream &fichier)
{
	Acteur acteur = {};
	acteur.nom = lireString(fichier);
	acteur.anneeNaissance = int(lireUintTailleVariable(fichier));
	acteur.sexe = char(lireUintTailleVariable(fichier));

	// TODO: Retourner un pointeur soit vers un acteur existant ou un nouvel acteur ayant les bonnes informations, selon si l'acteur existait déjà.  Pour fins de débogage, affichez les noms des acteurs crées; vous ne devriez pas voir le même nom d'acteur affiché deux fois pour la création.
	Acteur *ptrActeur = trouverActeur(acteur.nom);
	if (ptrActeur == nullptr)
	{
		ptrActeur = new Acteur(acteur);
	}
	return ptrActeur;
}

Film *ListeFilms::lireFilm(istream &fichier)
{
	Film film = {};

	// Lire les informations du film depuis le fichier
	film.titre = lireString(fichier);
	film.realisateur = lireString(fichier);
	film.anneeSortie = int(lireUintTailleVariable(fichier));
	film.recette = int(lireUintTailleVariable(fichier));
	film.acteurs.nElements = int(lireUintTailleVariable(fichier));
	film.acteurs.elements = new Acteur *[2 * film.acteurs.nElements];
	Film *ptrFilm = new Film(film); // Allouer dynamiquement un nouveau film
	for (int i = 0; i < ptrFilm->acteurs.nElements; i++)
	{
		// TODO: Placer l'acteur au bon endroit dans les acteurs du film.
		Acteur *ptrActeur = new Acteur;
		ptrActeur = lireActeur(fichier);

		ptrActeur->joueDans.ajouterFilm(ptrFilm);
		ptrFilm->acteurs.elements[i] = ptrActeur;

		/*for (i = 0; i < ptrActeur->joueDans.nElements; i++)
		{
			ptrActeur->joueDans.elements = nullptr;
		}
		delete[] ptrActeur->joueDans.elements;*/
		ptrActeur = nullptr;
		delete ptrActeur;
	}
	return ptrFilm; // TODO: Retourner le pointeur vers le nouveau film.
}

ListeFilms ListeFilms::creerListe(const string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);

	int nElements = int(lireUintTailleVariable(fichier));

	// TODO: Créer une liste de films vide.
	ListeFilms listeFilm;
	listeFilm.setNElements(0);
	listeFilm.setCapacite(max(2 * listeFilm.getNElements(), 1));
	listeFilm.elements = new Film *[listeFilm.getCapacite()];
	for (int i = 0; i < listeFilm.getNElements(); i++)
	{
		// TODO: Ajouter le film à la liste.
		Film *film = new Film;
		film = listeFilm.lireFilm(fichier);
		listeFilm.ajouterFilm(film);
		// film->acteurs.elements = new Acteur*[film->acteurs.nElements];
		film = nullptr;
		delete film;
	}
	return listeFilm; // TODO: Retourner la liste de films.
}

// TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.
void ListeFilms::detruireFilm(Film *film)
{
	int indice_film = -1;
	for (int i = 0; i < getNElements(); i++)
	{
		if (elements[i] == film)
		{
			indice_film = i;
			break;
		}
	}
	int nActeurDetruit = 0;
	if (indice_film != -1)
	{
		for (auto &acteurListe : span(elements[indice_film]->acteurs.elements, elements[indice_film]->acteurs.nElements))
		{
			if (acteurListe->joueDans.getNElements() < 2)
			{
				delete acteurListe;
				elements[indice_film]->acteurs.nElements--;
			}
			else
			{
				acteurListe->joueDans.supprimerFilm(film);
			}
		}
		delete elements[indice_film];
		decrementerNElements();
		elements[indice_film] = elements[getNElements() - 1];
		elements[getNElements() - 1] = nullptr;
	}
}

// TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.
void ListeFilms::detruireListeFilms()
{
	int nElements = getNElements();
	for (auto &film : span(elements, getCapacite()))
	{
		if (film != nullptr)
		{
			detruireFilm(film);
		}
	}
	delete[] elements;
}

void ListeFilms::afficherActeur(const Acteur &acteur) const
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

// TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
void ListeFilms::afficherFilm(const Film &film) const
{
	cout << "  " << film.titre << ", " << film.realisateur << ", " << film.anneeSortie << ", " << film.recette << endl;
	for (auto &acteur : span(film.acteurs.elements, film.acteurs.nElements))
	{
		afficherActeur(*acteur);
	}
}

void ListeFilms::afficherListeFilms() const
{
	// TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = {"      ──────────────────────────"};
	// TODO: Changer le for pour utiliser un span.
	for (const auto &film : span(elements, getNElements()))
	{
		// TODO: Afficher le film.
		if (film != nullptr)
		{
			afficherFilm(*film);
			cout << ligneDeSeparation << endl;
		}
	}
}

void ListeFilms::afficherFilmographieActeur(const string &nomActeur) const
{
	// TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
	const Acteur *acteur = trouverActeur(nomActeur);
	if (acteur == nullptr)
		cout << "Aucun acteur de ce nom" << endl;
	else
		acteur->joueDans.afficherListeFilms();
	acteur = nullptr;
	delete acteur;
}

int main()
{
	bibliotheque_cours::activerCouleursAnsi(); // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	// int* fuite = new int; //TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Fuite detectee" de "4 octets" affiché à la fin de l'exécution, qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	// TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	// TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).
	ListeFilms listeFilms = ListeFilms::creerListe("films.bin");

	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	// TODO: Afficher le premier film de la liste.  Devrait être Alien.
	cout << listeFilms.elements[0]->titre << endl;

	cout << ligneDeSeparation << "Les films sont:" << endl;
	// TODO: Afficher la liste des films.  Il devrait y en avoir 7.
	listeFilms.afficherListeFilms();

	cout << ligneDeSeparation << "L'année de naissance de Benedict Cumberbatch:" << endl;
	// TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	listeFilms.trouverActeur("Benedict Cumberbatch")->anneeNaissance = 1976;
	listeFilms.afficherActeur(*listeFilms.trouverActeur("Benedict Cumberbatch"));

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	// TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	listeFilms.afficherFilmographieActeur("Benedict Cumberbatch");

	// TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.
	listeFilms.detruireFilm(listeFilms.elements[0]);

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	// TODO: Afficher la liste des films.
	listeFilms.afficherListeFilms();

	// TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	// TODO: Détruire tout avant de terminer le programme.  La bibliothèque de verification_allocation devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
	listeFilms.detruireListeFilms();
}
