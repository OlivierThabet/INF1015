/*
Programme qui fait deviner un nombre entier
a l'utilisateur. Il lui redemande si l'entree
n'est pas valide. Il renvoie si l'entree est
trop haute ou trop basse. Quand l'utilisateur
trouve le resultat, on lui renvoie le nombre
d'essais dont il a eu besoin.
*/

#include <iostream>
using namespace std;

const int N_MAXIMUM = 1000;
int generateurDeNombre()
{
    srand(time(0));
    int nombreAleatoire = rand() % (N_MAXIMUM+1);
    return nombreAleatoire;
}

int main()
{

    int essai;
    int nombreDEssais = 1;
    int nombreAleatoire = generateurDeNombre();
    while (essai != nombreAleatoire)
    {
        cout << "Entrez un nombre entier : ";
        cin >> essai;
        if (essai < 0 | essai > N_MAXIMUM)
        {
            continue;
        }
        else if (essai > nombreAleatoire)
        {
            cout << "Trop haut.\n";
            nombreDEssais += 1;
            continue;
        }
        else if (essai < nombreAleatoire)
        {
            cout << "Trop bas.\n";
            nombreDEssais += 1;
            continue;
        }
        else
        {
            cout << "Bravo! Vous avez reussi en " << nombreDEssais << " tentatives!";
        }
    }
    return 0;
}