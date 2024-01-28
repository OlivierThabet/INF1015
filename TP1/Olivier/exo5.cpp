#include <iostream>
using namespace std;

;
int generateurDeNombre()
{
    srand(time(0));
    int nombreAleatoire = rand() % 1001;
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
        if (essai < 0 | essai > 1000)
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