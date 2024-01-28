/*
Programme qui détermine le mot le plus
petit, le mot le plus grand et la taille
moyenne des mots pour une phrase donnée
*/

#include <iostream>
#include <string>
using namespace std;

int main()
{
	cout << "Saisissez une phrase : ";
	string Sentence;
	getline(cin, Sentence);
	string SmallestWord = Sentence;
	string BiggestWord = "";
	double MeanLenWords = 0;
	double NbOfWords = 0;
	string CurrentWord = "";
	for (char charac : Sentence)
	{
		if (charac == ' ')
		{
			NbOfWords++;
			MeanLenWords += CurrentWord.size();
			if (CurrentWord.size() < SmallestWord.size())
			{
				SmallestWord = CurrentWord;
			}
			else if (CurrentWord.size() > BiggestWord.size())
			{
				BiggestWord = CurrentWord;
			}
			CurrentWord = "";
		}
		else
		{
			CurrentWord += charac;
		}
	}
	NbOfWords++;
	MeanLenWords += CurrentWord.size();
	cout << "Le mot le plus court est : " << SmallestWord << endl;
	cout << "Le mot le plus long est : " << BiggestWord << endl;
	MeanLenWords /= NbOfWords;
	cout << "La longueur moyenne est : " << MeanLenWords << " lettres" << endl;

	return 0;
}