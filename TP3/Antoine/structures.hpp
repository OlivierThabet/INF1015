#pragma once
// Structures mémoires pour une collection de films.
#include <iostream>
#include <string>
#include <cassert>
#include "gsl/span"
#include <memory>
using gsl::span;
using namespace std;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront définis après.

class ListeFilms {
public:
	ListeFilms() = default;
	ListeFilms(const string& nomFichier);
	ListeFilms(const ListeFilms& l) { assert(l.elements == nullptr); } // Pas demandé dans l'énoncé, mais on veut s'assurer qu'on ne fait jamais de copie de liste, car la copie par défaut ne fait pas ce qu'on veut.  Donc on ne permet pas de copier une liste non vide (la copie de liste vide est utilisée dans la création d'un acteur).
	~ListeFilms();
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	Film* operator[](size_t index)
	{
		if (index < static_cast<size_t>(nElements)) {
			return enSpan()[index];
		}
		else {
			// Gestion de l'erreur ici, par exemple, lancer une exception ou retourner une référence à un objet spécial
			throw std::out_of_range("Index hors limites dans ListeFilms");
		}
	}
	template<typename Predicat>
	Film* trouverFilm(Predicat predicat) const
	{
		for (int i = 0; i < nElements; i++)
		{
			if (predicat(elements[i]))
			{
				return elements[i];
			}
		}
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

template<typename T>
class Liste {
public:
	int capacite, nElements;
	unique_ptr<shared_ptr<T>[]> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
	Liste(int nT = 0) : capacite(nT), nElements(0), elements(make_unique<shared_ptr<T>[]>(nT)) {};
	Liste(const Liste& autre) : capacite(autre.capacite), nElements(autre.nElements), elements(std::make_unique<std::shared_ptr<T>[]>(autre.capacite)) {
		for (int i = 0; i < nElements; ++i) {
			elements[i] = autre.elements[i];
		}
	}
	void ajouterT(shared_ptr<T> t) {
		if (nElements == capacite) {
			int CapaciteT = max(1, 2 * capacite);
			unique_ptr<shared_ptr<T>[]> copieListe(make_unique<shared_ptr<T>[]>(CapaciteT));
			for (int i = 0; i < nElements; ++i) {
				copieListe[i] = elements[i];
			}
			for (int i = nElements; i < CapaciteT; ++i) {
				copieListe[i] = nullptr;
			}
			elements = move(copieListe);
			capacite = CapaciteT;
		}
		elements[nElements] = t;
		++nElements;
	}
};
using ListeActeurs = Liste<Acteur>;
struct Acteur
{
	string nom = "NA";
	int anneeNaissance = 0;
	char sexe = 'U'; // U pour inconnu
	//ListeFilms joueDans;
	friend ostream& operator<<(ostream& os, const Acteur& acteur)
	{
		return os << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe;
	}

};

struct Film
{
	string titre = "NA", realisateur = "NA"; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie = 0, recette = 0; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
	friend ostream& operator<<(ostream& os, const Film& film) {
		os << "Titre: " << film.titre << endl;
		os << "  Réalisateur: " << film.realisateur << "  Année :" << film.anneeSortie << endl;
		os << "  Recette: " << film.recette << "M$" << endl;
		os << "Acteurs:" << endl;
		for (int i = 0; i < film.acteurs.nElements; i++) {
			os << "  " << *(film.acteurs.elements[i]) << endl;
		}
		return os;
	}
};
