/*
Programme qui determine le gagnant
pour une partie de tic-tac-toe
*/

#include <iostream>
#include <fstream>
#include <string>
#include <array>
using namespace std;

string searchHorizontal(array<string, 3> game)
{
	for (int i = 0; i < game.size(); ++i)
	{
		int nO = 0;
		int nX = 0;
		for (int j = 0; j < game.size(); ++j)
		{
			if (game[i][j] == 'x')
			{
				nX++;
			}
			else if (game[i][j] == 'O')
			{
				nO++;
			}
		}
		if (nX == 3)
		{
			return "Le joueur 1 gagne";
		}
		if (nO == 3)
		{
			return "Le joueur 2 gagne";
		}
	}
	return "Egalite";
}

string searchVertical(array<string, 3> game)
{
	for (int j = 0; j < game.size(); ++j)
	{
		int nO = 0;
		int nX = 0;
		for (int i = 0; i < game.size(); ++i)
		{
			if (game[i][j] == 'x')
			{
				nX++;
			}
			else if (game[i][j] == 'o')
			{
				nO++;
			}
		}
		if (nX == 3)
		{
			return "Le joueur 1 gagne";
		}
		if (nO == 3)
		{
			return "Le joueur 2 gagne";
		}
	}
	return "Egalite";
}

string searchDiagonal(array<string, 3> game)
{
	for (int nOfDiag = 0; nOfDiag < 2; ++nOfDiag)
	{
		int nO = 0;
		int nX = 0;
		int j = 2 * nOfDiag;
		for (int i = 0; i < game.size(); ++i)
		{
			if (game[i][j] == 'x')
			{
				nX++;
			}
			else if (game[i][j] == 'o')
			{
				nO++;
			}
			j += 1 - 2 * nOfDiag;
		}
		if (nX == 3)
		{
			return "Le joueur 1 gagne";
		}
		if (nO == 3)
		{
			return "Le joueur 2 gagne";
		}
	}
	return "Egalite";
}

int main()
{
	string nameFile;
	cout << "Entrez le nom du fichier : ";
	getline(cin, nameFile);
	ifstream file("grilles/" + nameFile);
	if (file.is_open())
	{
		string line;
		array<string, 3> game;
		int nOfLine = 0;
		while (getline(file, line))
		{
			cout << line << endl;
			string line_i = "";
			line_i += line[0];
			line_i += line[2];
			line_i += line[4];
			game[nOfLine] = line_i;
			nOfLine++;
		}
		string result;
		result = searchVertical(game);
		if (result == "Egalite")
		{
			result = searchHorizontal(game);
			if (result == "Egalite")
			{
				result = searchDiagonal(game);
				cout << result;
			}
			else
			{
				cout << result;
			}
		}
		else
		{
			cout << result;
		}
	}
	else
	{
		cout << "Le fichier n'a pas pu etre ouvert";
	}

	return 0;
}
