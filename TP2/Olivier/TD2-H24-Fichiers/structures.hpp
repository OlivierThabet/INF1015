#pragma once
#include <istream>
#include <string>

using namespace std;

struct Film;
struct Acteur;

class ListeFilms
{
public:
	static ListeFilms creerListe(string nomFichier);

	ListeFilms(){};

	void ajouterFilm(Film *film);
	void supprimerFilm(const Film *film);
	Acteur *trouverActeur(const string &nomActeur) const;
	Acteur *lireActeur(istream &fichier);
	Film *lireFilm(istream &fichier);
	void detruireFilm(Film *film);
	void detruireListeFilms();
	void afficherActeur(const Acteur &acteur) const;
	void afficherFilm(const Film &film) const;
	void afficherListeFilms() const;
	void afficherFilmographieActeur(const string &nomActeur) const;

	void incrementerNElements()
	{
		nElements_++;
	}
	void setNElements(int valeur)
	{
		nElements_ = valeur;
	}
	void decrementerNElements()
	{
		nElements_--;
	}
	void setCapacite(int valeur)
	{
		capacite_ = valeur;
	}
	Film **getElements()
	{
		return elements_;
	}

private:
	int nElements_ = 0;
	int capacite_ = 0;
	Film **elements_ = nullptr;
};

struct ListeActeurs
{
	int capacite, nElements;
	Acteur **elements; // Pointeur vers un tableau de Acteur*, chaque Acteur* pointant vers un Acteur.
};

struct Film
{
	std::string titre, realisateur; // Titre et nom du réalisateur (on suppose qu'il n'y a qu'un réalisateur).
	int anneeSortie, recette;		// Année de sortie et recette globale du film en millions de dollars
	ListeActeurs acteurs;
};

struct Acteur
{
	std::string nom;
	int anneeNaissance;
	char sexe;
	ListeFilms joueDans;
};
