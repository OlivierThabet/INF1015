﻿	
/**
* fichier d'entête pour le TD4
* \file   structures.hpp
* \author Thabet et Leblanc
* \date   17 mars 2024
* Créé le 10 mars 2024
*/
// Solutionnaire du TD3 INF1015 hiver 2024
// Par Francois-R.Boyer@PolyMtl.ca
#pragma once
// Structures mémoires pour une collection de films.
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <cassert>
#include "gsl/span"
using gsl::span;
using namespace std;

struct Film; struct Acteur; // Permet d'utiliser les types alors qu'ils seront défini après.

class ListeFilms {
public:
	ListeFilms() = default;
	void ajouterFilm(Film* film);
	void enleverFilm(const Film* film);
	shared_ptr<Acteur> trouverActeur(const string& nomActeur) const;
	span<Film*> enSpan() const;
	int size() const { return nElements; }
	void detruire(bool possedeLesFilms = false);
	Film*& operator[] (int index) { assert(0 <= index && index < nElements);  return elements[index]; }
	Film* trouver(const function<bool(const Film&)>& critere) {
		for (auto& film : enSpan())
			if (critere(*film))
				return film;
		return nullptr;
	}

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
};

template <typename T>
class Liste {
public:
	Liste() = default;
	explicit Liste(int capaciteInitiale) :  // explicit n'est pas matière à ce TD, mais c'est un cas où c'est bon de l'utiliser, pour ne pas qu'il construise implicitement une Liste à partir d'un entier, par exemple "maListe = 4;".
		capacite_(capaciteInitiale),
		elements_(make_unique<shared_ptr<T>[]>(capacite_))
	{
	}
	Liste(const Liste<T>& autre) :
		capacite_(autre.nElements_),
		nElements_(autre.nElements_),
		elements_(make_unique<shared_ptr<T>[]>(nElements_))
	{
		for (int i = 0; i < nElements_; ++i)
			elements_[i] = autre.elements_[i];
	}
	//NOTE: On n'a pas d'operator= de copie, ce n'était pas nécessaire pour répondre à l'énoncé. On aurait facilement pu faire comme dans les notes de cours et définir l'operator= puis l'utiliser dans le constructeur de copie.
	//NOTE: Nos constructeur/operator= de move laissent la liste autre dans un état pas parfaitement valide; il est assez valide pour que la destruction et operator= de move fonctionnent, mais il ne faut pas tenter d'ajouter, de copier ou d'accéder aux éléments de cette liste qui "n'existe plus". Normalement le move sur les classes de la bibliothèque standard C++ laissent les objets dans un "valid but unspecified state" (https://en.cppreference.com/w/cpp/utility/move). Pour que l'état soit vraiment valide, on devrait remettre à zéro la capacité et nombre d'éléments de l'autre liste.
	Liste(Liste<T>&&) = default;  // Pas nécessaire, mais puisque c'est si simple avec unique_ptr...
	Liste<T>& operator= (Liste<T>&&) noexcept = default;  // Utilisé pour l'initialisation dans lireFilm.

	void ajouter(shared_ptr<T> element)
	{
		assert(nElements_ < capacite_);  // Comme dans le TD précédent, on ne demande pas la réallocation pour ListeActeurs...
		elements_[nElements_++] = move(element);
	}

	// Noter que ces accesseurs const permettent de modifier les éléments; on pourrait vouloir des versions const qui retournent des const shared_ptr, et des versions non const qui retournent des shared_ptr.  En C++23 on pourrait utiliser "deducing this".
	shared_ptr<T>& operator[] (int index) const { assert(0 <= index && index < nElements_); return elements_[index]; }
	span<shared_ptr<T>> enSpan() const { return span(elements_.get(), nElements_); }

private:
	int capacite_ = 0, nElements_ = 0;
	unique_ptr<shared_ptr<T>[]> elements_;
};

using ListeActeurs = Liste<Acteur>;

class Affichable
{
public:
	virtual void afficherItem(ostream& os) const = 0;
};

class Item{
	protected:
	string titre_= "NA";
	int anneeSortie_=0;
	public:
	Item() = default;	
	Item(string titre, int anneeSortie): titre_(titre), anneeSortie_(anneeSortie) {};	
	friend Film* lireFilm(istream& fichier, ListeFilms& listeFilms);
	friend shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const;
	//friend ostream& operator<< (ostream& os, const Film& film);
	// ensuite
	string getTitre() const { return titre_; }
	int getAnneeSortie() const { return anneeSortie_; }
	void afficherBase(ostream& os) const
	{
		os << "Titre: " << titre_ << endl;
		os << "Année de sortie: " << anneeSortie_ << endl;
	}
};	

class Livre: virtual public Item, public Affichable
{
private:
int nPages_=0, nCopiesVendues_=0;
string auteur_="NA";
public:
Livre() = default;	
Livre(int nPages, int nCopiesVendues, string auteur, string titre, int anneeSortie): 
Item(titre, anneeSortie),nPages_(nPages), nCopiesVendues_(nCopiesVendues), auteur_(auteur) {};
int getNPages() const { return nPages_; }
void afficherDetailsLivre(ostream& os) const;
virtual void afficherItem(ostream& os) const override
{
	Item::afficherBase(os);
	afficherDetailsLivre(os);
}
};

class Film : virtual public Item, public Affichable
{
	private:
	string realisateur_; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int recette_=0; // Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs_;
	public:
	//friend ostream& operator<< (ostream& os, const Film& film);	
	friend Film* lireFilm(istream& fichier, ListeFilms& listeFilms);
	friend shared_ptr<Acteur> ListeFilms::trouverActeur(const string& nomActeur) const;
	void afficherDetailsFilm(ostream& os) const;
	virtual void afficherItem(ostream& os) const override
	{
		Item::afficherBase(os);
		afficherDetailsFilm(os);
	}
};

struct Acteur
{
	string nom; int anneeNaissance=0; char sexe='\0';
};

class FilmLivre : public Film, public Livre
{
public:
	FilmLivre(const Film& film, const Livre& livre) :
		Item(film.getTitre(), film.getAnneeSortie()),
		Film(film),
		Livre(livre)
	{}
	virtual void afficherItem(ostream& os) const override
	{
		Item::afficherBase(os);
		Film::afficherDetailsFilm(os);
		Livre::afficherDetailsLivre(os);
	}
};