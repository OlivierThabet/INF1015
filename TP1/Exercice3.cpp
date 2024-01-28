/*
Programme qui demande un nombre entier
a l'utilisateur et qui l'insere au bon endroit
dans un std::array deja trie.
*/

#include <array>
#include <iostream>
#include <string>
using namespace std;

const int SIZE = 6;
int getValue()
{
	while (true)
	{
		try
		{
			cout << "Entrez un entier : ";
			string userValue;
			getline(cin, userValue);
			return stoi(userValue);
		}
		catch (const std::invalid_argument &e)
		{
			cerr << "Erreur : Veuillez entrer un entier." << endl;
		}
		catch (const std::out_of_range &e)
		{
			cerr << "Erreur : Veuillez entrer un nombre plus petit." << endl;
		}
	}
}

std::array<int, SIZE> insertValue(std::array<int, SIZE> myArray, int myValue, int nElement)
{
	for (int i = nElement - 1; i >= 0; --i)
	{
		if (myArray[i] > myValue)
		{
			myArray[i + 1] = myArray[i];
			myArray[i] = 0;
		}
		else if (i != 0)
		{
			myArray[i + 1] = myValue;
			return myArray;
		}
		else
		{
			myArray[i] = myValue;
			return myArray;
		}
	}
	return myArray;
}

int main()
{
	array<int, SIZE> myArray = {1, 3, 4, 7, 9};
	int nElement = 5;
	int myValue = getValue();
	cout << "Le tableau, avant insertion est : {";
	for (int i = 0; i < myArray.size() - 1; ++i)
	{
		if (i != myArray.size() - 2)
		{
			cout << myArray[i] << ", ";
		}
		else
		{
			cout << myArray[i];
		}
	}
	cout << "}" << endl;
	myArray = insertValue(myArray, myValue, nElement);
	cout << "Le tableau, apres insertion est : {";
	for (int i = 0; i < myArray.size(); ++i)
	{
		if (i != myArray.size() - 1)
		{
			cout << myArray[i] << ", ";
		}
		else
		{
			cout << myArray[i];
		}
	}
	cout << "}";
	return 0;
}