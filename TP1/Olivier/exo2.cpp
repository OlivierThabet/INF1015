#include <iostream>
#include <cmath>
using namespace std;

double VALEUR_INITIALE = 1.0;
int NOMBRE_DE_CHIFFRES_APRES_LA_VIRGULE = 5;
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
    cout << "Selectionnez le nombre de chiffres apres la virgule desire: ";
    valeurFinale = suiteReelle();
    cout << nombreArrondi(valeurFinale, NOMBRE_DE_CHIFFRES_APRES_LA_VIRGULE);
    return 0;
}
