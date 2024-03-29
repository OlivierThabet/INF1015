﻿#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{
template <typename T>
T lireType(istream& fichier)
{
	T valeur{};
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}
#define erreurFataleAssert(message) assert(false&&(message)),terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;
size_t lireUintTailleVariable(istream& fichier)
{
	uint8_t entete = lireType<uint8_t>(fichier);
	switch (entete) {
	case enteteTailleVariableDeBase+0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase+1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase+2: return lireType<uint32_t>(fichier);
	default:
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
	}
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUintTailleVariable(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}

//TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.
ListeFilms ajouterFilm(ListeFilms listeFilms, Film* film)
{
	int capacite = listeFilms.capacite;
	int nElements = listeFilms.nElements;
	//cout << "2 " << "capa " << listeFilms.capacite << " elem " << listeFilms.capacite << endl;
	if (capacite < nElements + 1)
	{
		listeFilms.capacite = max(capacite * 2, 1);
		Film** nouveauxElements = new Film* [listeFilms.capacite];
		for (int i=0; i<nElements; i++)
		{
			nouveauxElements[i] = listeFilms.elements[i];
		}
		//cout << "nElements" << nElements << endl;
		nouveauxElements[nElements] = film;
		delete[] listeFilms.elements;
		//listeFilms.elements = new Film* [listeFilms.capacite];
		listeFilms.elements = nouveauxElements;
		listeFilms.nElements++;
	}
	else
	{
		//cout << "e" << endl;
		listeFilms.elements[nElements] = film;
		//cout << "bfdu " << endl;
		listeFilms.nElements++;
	}
	return listeFilms;
}
//TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
ListeFilms supprimerFilm(ListeFilms& listeFilms, const Film* film)
{
	for (int i = 0; i < listeFilms.nElements; i++)
	{
		if (listeFilms.elements[i] == film)
		{
			delete listeFilms.elements[i];
			// Déplace les éléments suivants pour remplir le trou
			for (int j = i; j < listeFilms.nElements - 1; j++)
			{
				listeFilms.elements[j] = listeFilms.elements[j + 1];
			}
			listeFilms.nElements--;
		}
	}
	return listeFilms;
}

//TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
Acteur* trouverActeur(const ListeFilms listeFilms, const string& nomActeur)
{
	for (const auto& film : span(listeFilms.elements, listeFilms.nElements))
	{
		try
		{
			for (const auto& acteur : span(film->acteurs.elements, film->acteurs.nElements))
			{
				if (acteur != nullptr && acteur->nom == nomActeur)
				{
					return acteur;
				}
			}
		}
		catch (const exception& e)
		{
			return nullptr;
		}

	}
	return nullptr;
}


//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
Acteur* lireActeur(istream& fichier, ListeFilms listeFilms)
{
	Acteur acteur = {};
	acteur.nom            = lireString(fichier);
	acteur.anneeNaissance = int(lireUintTailleVariable (fichier));
	acteur.sexe           = char(lireUintTailleVariable(fichier));
	acteur.joueDans.nElements = 0;
	acteur.joueDans.capacite = 1;
	acteur.joueDans.elements = nullptr;
	//TODO: Retourner un pointeur soit vers un acteur existant ou un nouvel acteur ayant les bonnes informations, selon si l'acteur existait déjà.  Pour fins de débogage, affichez les noms des acteurs crées; vous ne devriez pas voir le même nom d'acteur affiché deux fois pour la création.
	Acteur* ptrActeur = trouverActeur(listeFilms, acteur.nom);
	if (ptrActeur == nullptr)
	{
		ptrActeur = &acteur;
	}
	cout << "Nom acteur : " << acteur.nom << endl;
	return ptrActeur;
}


Film* lireFilm(istream& fichier, ListeFilms listeFilms)
{
	Film* ptrFilm = new Film();  // Allouer dynamiquement un nouveau film

	// Lire les informations du film depuis le fichier
	ptrFilm->titre = lireString(fichier);
	ptrFilm->realisateur = lireString(fichier);
	ptrFilm->anneeSortie = int(lireUintTailleVariable(fichier));
	ptrFilm->recette = int(lireUintTailleVariable(fichier));
	ptrFilm->acteurs.nElements = int(lireUintTailleVariable(fichier));
	ptrFilm->acteurs.elements = new Acteur * [2 * ptrFilm->acteurs.nElements];
	for (int i = 0; i < ptrFilm->acteurs.nElements; i++) {
		//TODO: Placer l'acteur au bon endroit dans les acteurs du film.
		Acteur* ptrActeur = lireActeur(fichier, listeFilms);

		// Placer l'acteur au bon endroit dans les acteurs du film
		ptrFilm->acteurs.elements[i] = ptrActeur;
		//cout << "0" << " capacite " << ptrFilm->acteurs.elements[i]->joueDans.capacite << " elem " << ptrFilm->acteurs.elements[i]->joueDans.nElements << endl;
		ptrActeur->joueDans.capacite = 0;
		ptrActeur->joueDans.nElements = 0;
		ptrActeur->joueDans.elements = nullptr;
		//caca(ptrActeur->joueDans);
		// Ajouter le film à la liste des films dans lesquels l'acteur joue
		ListeFilms liste = ptrActeur->joueDans;
		//cout << " 26 " << liste.nElements << " capa " << liste.capacite << endl;
		ptrActeur->joueDans = ajouterFilm(liste, ptrFilm);
		
		// coutaaa
	}
	return ptrFilm; //TODO: Retourner le pointeur vers le nouveau film.
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);
	
	int nElements = int(lireUintTailleVariable(fichier));

	//TODO: Créer une liste de films vide.
	ListeFilms listeFilms;
	listeFilms.nElements = nElements;
	listeFilms.capacite = 2*nElements;
	listeFilms.elements = new Film*[listeFilms.capacite];
	for (int i = 0; i < nElements; i++) {
		//TODO: Ajouter le film à la liste.
		Film* film = lireFilm(fichier, listeFilms);
		//cout << "abbababab" << listeFilms.elements << "  n_elem  " << listeFilms.nElements << " film " << film->realisateur << endl;
		listeFilms = ajouterFilm(listeFilms, film);
	}
	
	return listeFilms; //TODO: Retourner la liste de films.
}

//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.
void detruireFilm(ListeFilms& listeFilms, const Film* film)
{
	int indice_film = -1;
	for (int i = 0; i < listeFilms.nElements; i++)
	{
		if (listeFilms.elements[i] == film)
		{
			indice_film = i;
			break;
		}
	}
	if (indice_film != -1)
	{
		for (int i = 0; i < listeFilms.elements[indice_film]->acteurs.nElements; i++)
		{
			for (int j = 0; j < film->acteurs.nElements; j++)
			{
				if (listeFilms.elements[indice_film]->acteurs.elements[i] == film->acteurs.elements[j])
				{
					delete[] listeFilms.elements[indice_film]->acteurs.elements[i];
					break;
				}
			}
		}
		delete[] listeFilms.elements[indice_film];
	}
}
//TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.
void detruireListeFilms(ListeFilms& listeFilms)
{
	for (int i = 0; i < listeFilms.nElements; i++)
	{
		delete[] listeFilms.elements[i]->acteurs.elements;
	}
	delete[] listeFilms.elements;
}

void afficherActeur(const Acteur& acteur)
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

//TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).
void afficherFilm(const Film& film)
{
	cout << "  " << film.titre << ", " << film.realisateur << ", " << film.anneeSortie << ", " << film.recette << endl;
	for (auto& acteur : span(film.acteurs.elements, film.acteurs.nElements))
	{
		afficherActeur(*acteur);
	}
}

void afficherListeFilms(const ListeFilms& listeFilms)
{
	//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = {"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};
	cout << ligneDeSeparation;
	//TODO: Changer le for pour utiliser un span.
	for (auto& film : span(listeFilms.elements, listeFilms.nElements)) {
		//TODO: Afficher le film.
		afficherFilm(*film);
		cout << ligneDeSeparation;
	}
}

void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur)
{
	//TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
	const Acteur* acteur = trouverActeur(listeFilms, nomActeur);
	if (acteur == nullptr)
		cout << "Aucun acteur de ce nom" << endl;
	else
		afficherListeFilms(acteur->joueDans);
}

int main()
{
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	int* fuite = new int; //TODO: Enlever cette ligne après avoir vérifié qu'il y a bien un "Fuite detectee" de "4 octets" affiché à la fin de l'exécution, qui réfère à cette ligne du programme.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	//TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	//TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire.  Devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur).
	ListeFilms listeFilms = creerListe("films.bin");
	
	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	//TODO: Afficher le premier film de la liste.  Devrait être Alien.
	cout <<  listeFilms.elements[0]->titre << endl;
	
	cout << ligneDeSeparation << "Les films sont:" << endl;
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.
	/*
	for (int i = 0; i > listeFilms.nElements; i++)
	{
		cout << listeFilms.elements[i]->titre << " ";
	}
	cout << endl;
	*/
	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	
	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	//TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	
	//TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.
	
	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	//TODO: Afficher la liste des films.
	
	//TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.
	
	//TODO: Détruire tout avant de terminer le programme.  La bibliothèque de verification_allocation devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
}
