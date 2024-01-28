/*
Prog ramme qui détermine le mot le plus
petit, le mot le plus grand et la taille
moyenne des mots pour une phrase donnée
*/

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main()
{
    string sentence, word, smallestWord = "", biggestWord = "";
    double totalLengthSentence = 0.0, numberOfWordsInSentence = 0.0;
    cout << "Saisissez une phrase : ";
    getline(cin, sentence);
    istringstream words(sentence);

    while (words >> word)
    {
        if (smallestWord.empty() || word.length() < smallestWord.length())
        {
            smallestWord = word;
        }
        if (biggestWord.empty() || word.length() > biggestWord.length())
        {
            biggestWord = word;
        }
        totalLengthSentence += word.size();
        numberOfWordsInSentence += 1;
    }
    double moyenneLongueurMots = totalLengthSentence / numberOfWordsInSentence;

    cout << "Le mot le plus court est : " << smallestWord << "\n";
    cout << "Le mot le plus long est : " << biggestWord << "\n";
    cout << "La longueur moyenne est : " << moyenneLongueurMots << " lettres\n";

    return 0;
}
