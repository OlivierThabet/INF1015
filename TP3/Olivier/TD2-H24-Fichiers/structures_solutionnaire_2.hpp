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

private:
	void changeDimension(int nouvelleCapacite);

	int capacite = 0, nElements = 0;
	Film** elements = nullptr; // Pointeur vers un tableau de Film*, chaque Film* pointant vers un Film.
	bool possedeLesFilms_ = false; // Les films seront détruits avec la liste si elle les possède.
};

template<typename T>
class Liste {
	public:
	Liste(int capaciteInitiale) : capacite(capaciteInitiale), nElements(0), 
	elements(make_unique<shared_ptr<T>[]>(capaciteInitiale)){}
	Liste(const Liste& autre): capacite(autre.capacite), nElements(autre.nElements), 
	elements(make_unique<shared_ptr<T>[]>(autre.capacite)){
		for (int i = 0; i < nElements; ++i) {
			elements[i] = autre.elements[i];
		}
		}
	int getCapacite() const {return capacite;}
	int getNElements() const {return nElements;}
	unique_ptr<shared_ptr<T>[]> getElements() const {return elements;}
	void setNElements(int nElement) {this->nElements = nElement;}
	private:
	int capacite, nElements;
	unique_ptr<shared_ptr<T>[]> elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
	
};
/*template<typename T>
ostream& operator<<(ostream& os, const Liste<T>& liste) {
    for (int i = 0; i < liste.nElements; ++i) {
        os << *liste.elements[i] <<"\n"; 
    }
    return os;
}
*/

struct Film
{
	string titre="NA", realisateur="NA"; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie=0, recette=0; // Année de sortie et recette globale du film en millions de dollars
	Liste<Acteur> acteurs;
    };


struct Acteur
{
	string nom; int anneeNaissance; char sexe;
	//ListeFilms joueDans;
	/*ostream& operator<<(ostream& os, const Acteur& acteur) {
    os << "Acteur: " << acteur.nom << ", " << acteur.anneeNaissance << ", " << acteur.sexe;
    return os};*/

	
};
