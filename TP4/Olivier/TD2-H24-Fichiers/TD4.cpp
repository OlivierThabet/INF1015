﻿/**
* fichier d'entête pour le TD4
* \file   TD4.cpp
* \author Thabet et Leblanc
* \date   17 mars 2024
* Créé le 10 mars 2024
*/

#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp" // Structures de données pour la collection de films en mémoire.

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
#include "cppitertools/range.hpp"
#include "gsl/span"
#include "debogage_memoire.hpp"
#include <iomanip>
// Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).
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
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement."); // NOTE: Il n'est pas possible de faire des tests pour couvrir cette ligne en plus du reste du programme en une seule exécution, car cette ligne termine abruptement l'exécution du programme.  C'est possible de la couvrir en exécutant une seconde fois le programme avec un fichier films.bin qui contient par exemple une lettre au début.
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

// Fonctions pour ajouter un Film à une ListeFilms.
//[
void ListeFilms::changeDimension(int nouvelleCapacite)
{
	Film **nouvelleListe = new Film *[nouvelleCapacite];

	if (elements != nullptr)
	{ // Noter que ce test n'est pas nécessaire puique nElements sera zéro si elements est nul, donc la boucle ne tentera pas de faire de copie, et on a le droit de faire delete sur un pointeur nul (ça ne fait rien).
		nElements = min(nouvelleCapacite, nElements);
		for (int i : range(nElements))
			nouvelleListe[i] = elements[i];
		delete[] elements;
	}

	elements = nouvelleListe;
	capacite = nouvelleCapacite;
}

void ListeFilms::ajouterFilm(Film *film)
{
	if (nElements == capacite)
		changeDimension(max(1, capacite * 2));
	elements[nElements++] = film;
}

//]

// Fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
//[
// On a juste fait une version const qui retourne un span non const.  C'est valide puisque c'est la struct qui est const et non ce qu'elle pointe.  Ça ne va peut-être pas bien dans l'idée qu'on ne devrait pas pouvoir modifier une liste const, mais il y aurais alors plusieurs fonctions à écrire en version const et non-const pour que ça fonctionne bien, et ce n'est pas le but du TD (il n'a pas encore vraiment de manière propre en C++ de définir les deux d'un coup).
span<Film *> ListeFilms::enSpan() const { return span(elements, nElements); }

void ListeFilms::enleverFilm(const Film *film)
{
	for (Film *&element : enSpan())
	{ // Doit être une référence au pointeur pour pouvoir le modifier.
		if (element == film)
		{
			if (nElements > 1)
				element = elements[nElements - 1];
			nElements--;
			return;
		}
	}
}
//]

// Fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.
//[

// NOTE: Doit retourner un Acteur modifiable, sinon on ne peut pas l'utiliser pour modifier l'acteur tel que demandé dans le main, et on ne veut pas faire écrire deux versions.
shared_ptr<Acteur> ListeFilms::trouverActeur(const string &nomActeur) const
{
	for (const Film *film : enSpan())
	{
		for (const shared_ptr<Acteur> &acteur : film->acteurs_.enSpan())
		{
			if (acteur->nom == nomActeur)
				return acteur;
		}
	}
	return nullptr;
}
//]

// Les fonctions pour lire le fichier et créer/allouer une ListeFilms.

shared_ptr<Acteur> lireActeur(istream &fichier, const ListeFilms &listeFilms)
{
	Acteur acteur = {};
	acteur.nom = lireString(fichier);
	acteur.anneeNaissance = int(lireUintTailleVariable(fichier));
	acteur.sexe = char(lireUintTailleVariable(fichier));

	shared_ptr<Acteur> acteurExistant = listeFilms.trouverActeur(acteur.nom);
	if (acteurExistant != nullptr)
		return acteurExistant;
	else
	{
		cout << "Création Acteur " << acteur.nom << endl;
		return make_shared<Acteur>(move(acteur)); // Le move n'est pas nécessaire mais permet de transférer le texte du nom sans le copier.
	}
}

Film *lireFilm(istream &fichier, ListeFilms &listeFilms)
{
	Film *film = new Film;
	film->titre_ = lireString(fichier);
	film->realisateur_ = lireString(fichier);
	film->anneeSortie_ = int(lireUintTailleVariable(fichier));
	film->recette_ = int(lireUintTailleVariable(fichier));
	auto nActeurs = int(lireUintTailleVariable(fichier));
	film->acteurs_ = ListeActeurs(nActeurs); // On n'a pas fait de méthode pour changer la taille d'allocation, seulement un constructeur qui prend la capacité.  Pour que cette affectation fonctionne, il faut s'assurer qu'on a un operator= de move pour ListeActeurs.
	cout << "Création Film " << film->titre_ << endl;

	for ([[maybe_unused]] auto i : range(nActeurs))
	{ // On peut aussi mettre nElements avant et faire un span, comme on le faisait au TD précédent.
		film->acteurs_.ajouter(lireActeur(fichier, listeFilms));
	}

	return film;
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);

	int nElements = int(lireUintTailleVariable(fichier));

	ListeFilms listeFilms;
	for ([[maybe_unused]] int i : range(nElements))
	{ // NOTE: On ne peut pas faire un span simple avec ListeFilms::enSpan car la liste est vide et on ajoute des éléments à mesure.
		listeFilms.ajouterFilm(lireFilm(fichier, listeFilms));
	}

	return listeFilms;
}

// Fonction pour détruire une ListeFilms et tous les films qu'elle contient.
//[
// NOTE: La bonne manière serait que la liste sache si elle possède, plutôt qu'on le dise au moment de la destruction, et que ceci soit le destructeur.  Mais ça aurait complexifié le TD2 de demander une solution de ce genre, d'où le fait qu'on a dit de le mettre dans une méthode.
void ListeFilms::detruire(bool possedeLesFilms)
{
	if (possedeLesFilms)
		for (Film *film : enSpan())
			delete film;
	delete[] elements;
}
//]

// Pour que l'affichage de Film fonctionne avec <<, il faut aussi modifier l'affichage de l'acteur pour avoir un ostream; l'énoncé ne demande pas que ce soit un opérateur, mais tant qu'à y être...
ostream &operator<<(ostream &os, const Acteur &acteur)
{
	return os << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

// Fonctions pour afficher un film avec tous ces acteurs ou un livre
//[
void Livre::afficherDetailsLivre(std::ostream& os) const {
	os << "  Nombre de pages: " << nPages_ << endl;
	os << "  Nombre de copies vendues: " << nCopiesVendues_ << endl;
	os << "Auteur:" << auteur_ << endl;
}
void Film::afficherDetailsFilm(std::ostream& os) const {
	os << "  Réalisateur: " << realisateur_ << endl;
	os << "  Recette: " << recette_ << "M$" << endl;
	os << "Acteurs:" << endl;
	for (const auto& acteur : acteurs_.enSpan()) {
		os << *acteur; // Assurez-vous que l'opérateur << pour Acteur est bien défini
	}
}
ostream& operator<<(ostream& os, const Affichable& obj) {
	obj.afficherItem(os);
	return os;
}

//]

// Pour afficher un FilmLivre
//[
std::ostream& operator<<(std::ostream& os, const FilmLivre& filmLivre) {
	filmLivre.afficherItem(os);
	return os;
}
//]

// Pas demandé dans l'énoncé de tout mettre les affichages avec surcharge, mais pourquoi pas.
ostream &operator<<(ostream &os, const ListeFilms &listeFilms)
{
	static const string ligneDeSeparation = //[
		"\033[32m────────────────────────────────────────\033[0m\n";
	os << ligneDeSeparation;
	for (const Film *film : listeFilms.enSpan())
	{
		os << *film << ligneDeSeparation;
	}
	return os;
}

int main()
{
#ifdef VERIFICATION_ALLOCATION_INCLUS
	bibliotheque_cours::VerifierFuitesAllocations verifierFuitesAllocations;
#endif
	bibliotheque_cours::activerCouleursAnsi(); // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.

	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	ListeFilms listeFilms = creerListe("films.bin");
	vector<shared_ptr<Item> > bibliotheque;
	for (Film *film : listeFilms.enSpan())
	{
		shared_ptr<Item> filmPtr(film);
		bibliotheque.push_back(filmPtr);
		cout << "Ajout de " << filmPtr->getTitre() << " dans biblio" << endl;
		// enlever le cout a la fin et le getter
	}
	
	shared_ptr<Livre> livreTrouve = nullptr;

	ifstream fichier("livres.txt");
	while (fichier.good())
	{
		string titre, auteur;
		int anneeSortie, nPages, nCopiesVendues;
		fichier >> quoted(titre) >> anneeSortie >> quoted(auteur) >>  nCopiesVendues >>nPages;
		shared_ptr<Livre> livre =  make_shared<Livre>(Livre(nPages, nCopiesVendues, auteur, titre, anneeSortie));
		bibliotheque.push_back(livre);
		cout << "Ajout de " << livre->getTitre() << " dans biblio" << endl;
		cout << "dont le nombre de pages est " << livre->getNPages() << endl;
		if (livre->getTitre() == "The Hobbit")
		{
			livreTrouve = livre;
		}
		//enlever les cout a la fin et les getters
	}
	bibliotheque.pop_back();
	string titreRecherche = "Le Hobbit : La Bataille des Cinq Armées";
	Film* filmTrouve = listeFilms.trouver([&titreRecherche](const Film& film)
	{
		return film.getTitre() == titreRecherche;
	});

	cout << ligneDeSeparation << endl;
	cout << "Les infos du FilmLivre Le Hobbit sont : " << endl;
	if(filmTrouve != nullptr && livreTrouve != nullptr)
	{
		shared_ptr<FilmLivre> filmLivre =  make_shared<FilmLivre>(FilmLivre(*filmTrouve, *livreTrouve));
		cout << *filmLivre << endl;
	}
	// J'ai un pb ici, hs
	
	listeFilms.detruire(true);
	
}


