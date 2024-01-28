/*
Programme qui fait deviner un nombre entier
à l'utilisateur. Il lui redemande si l'entrée 
n'est pas valide. Il renvoie si l'entrée est 
trop haute ou trop basse. Quand l'utilisateur 
trouve le résultat, on lui renvoie le nombre 
d'essais dont il a eu besoin.
*/

#include <array>
#include <iostream>
#include <string>
using namespace std;

int gessValue(int min, int max) {
	string userValue;
	while (true) {
		cout << "Entrez un nombre entier : ";
		getline(cin, userValue);
        try {
            int myInt = stoi(userValue);
            if (myInt < min or myInt > max) {
                cerr << "Erreur : Entier en dehors de l'intervalle [" << min << "; " << max << "]" << endl;
                myInt = gessValue(min, max);
            }
            return myInt;
        }
        catch (const std::invalid_argument& e) {
            cerr << "Erreur : Ce n'est pas un entier valide." << endl;
        }
        catch (const std::out_of_range& e) {
            cerr << "Erreur : Dépassement de capacite." << endl;
        }
	}
}

int main() {
    int value = 45;
    int guessedValue = 0;
    int nbOfTries = 0;
    int min = 0;
    int max = 1000;
    while (value != guessedValue) {
        guessedValue = gessValue(min, max);
        nbOfTries++;
        if (guessedValue > value) {
            cout << "Trop haut" << endl;
        }
        else if (guessedValue < value) {
            cout << "Trop bas" << endl;
        }
    }
    cout << "Bravo ! Vous avez trouve " << value << " en " << nbOfTries << " essais.";
    return 0;
}