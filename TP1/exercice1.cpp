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
	string sentence;
	getline(cin, sentence);
	string smallestWord = sentence;
	string biggestWord = "";
	double meanLenWords = 0;
	int nOfWords = 0;
	string currentWord = "";
	for (char charac : sentence)
	{
		if (charac == ' ')
		{
			nOfWords++;
			meanLenWords += currentWord.size();
			if (currentWord.size() < smallestWord.size())
			{
				smallestWord = currentWord;
			}
			else if (currentWord.size() > biggestWord.size())
			{
				biggestWord = currentWord;
			}
			currentWord = "";
		}
		else
		{
			currentWord += charac;
		}
	}
	nOfWords++;
	meanLenWords += currentWord.size();
	cout << "Le mot le plus court est : " << smallestWord << endl;
	cout << "Le mot le plus long est : " << biggestWord << endl;
	meanLenWords /= nOfWords;
	cout << "La longueur moyenne est : " << meanLenWords << " lettres" << endl;

	return 0;
}
