/*
Programme qui determine la limite de la
suite Un+1 = sqrt(2 + Un) avec une
precision determinee par l'utilisateur
*/

#include <iostream>
#include <cmath>
using namespace std;

const double VALEUR_INITIALE = 1.0;
const int NOMBRE_DE_CHIFFRES_APRES_LA_VIRGULE = 5;
double valeurFinale;
double suiteReelle()
{

    double valeurDeLaSuite = VALEUR_INITIALE;
    while (true)
    {
        valeurDeLaSuite = sqrt(2 + valeurDeLaSuite);
        return valeurDeLaSuite;
    }
}

double nombreArrondi(double nombreAArrondir, int nombreDeChiffreApresLaVirgule)
{
    double facteur = pow(10.0, nombreDeChiffreApresLaVirgule);
    return round(nombreAArrondir * facteur) / facteur;
}

int main()
{
    valeurFinale = suiteReelle();
    cout << nombreArrondi(valeurFinale, NOMBRE_DE_CHIFFRES_APRES_LA_VIRGULE);
    return 0;
}
