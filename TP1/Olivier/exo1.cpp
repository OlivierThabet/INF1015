#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main()
{
    string phrase, mot, plusPetitMot = "", plusGrandMot = "";
    int longueurTotalePhrase, nombreDeMotsDansPhrase = 0;
    cout << "Saisissez une phrase : ";
    getline(cin, phrase);
    istringstream mots(phrase);

    while (mots >> mot)
    {
        if (plusPetitMot.empty() || mot.length() < plusPetitMot.length())
        {
            plusPetitMot = mot;
        }
        if (plusGrandMot.empty() || mot.length() > plusGrandMot.length())
        {
            plusGrandMot = mot;
        }
        longueurTotalePhrase += mot.length();
        nombreDeMotsDansPhrase += 1;
    }
    double moyenneLongueurMots = static_cast<double>(longueurTotalePhrase) / nombreDeMotsDansPhrase;

    cout << "Le mot le plus court est : " << plusPetitMot << "\n";
    cout << "Le mot le plus long est : " << plusGrandMot << "\n";
    cout << "La longueur moyenne est : " << moyenneLongueurMots << " lettres\n";

    return 0;
}