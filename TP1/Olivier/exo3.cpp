/*
Programme qui demande un nombre entier
a l'utilisateur et qui l'insere au bon endroit
dans un std::array deja trie.
*/

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

const int TAILLE_DU_TABLEAU = 5;

void tableauTrie(int nombreAAjouter)
{
    int nombresATrier[5] = {nombreAAjouter, 1, 10, 100, 1000};
    for (int i = 0; i < TAILLE_DU_TABLEAU - 1; ++i)
    {
        for (int j = 0; j < TAILLE_DU_TABLEAU - i - 1; ++j)
        {
            if (nombresATrier[j] > nombresATrier[j + 1])
            {
                int temp = nombresATrier[j];
                nombresATrier[j] = nombresATrier[j + 1];
                nombresATrier[j + 1] = temp;
            }
        }
    }
    cout << "{";
    for (int i = 0; i < TAILLE_DU_TABLEAU; ++i)
    {
        cout << nombresATrier[i];
        if (i < TAILLE_DU_TABLEAU - 1)
        {
            cout << ", ";
        }
    }
    cout << "}" << endl;
}


int main()
{
    try
    {
        int nombreAAjouter;
        cout << "Entrez un nombre entier: ";
        cin >> nombreAAjouter;

        if (cin.fail())
        {
            throw invalid_argument("Il faut que l'entree soit un nombre entier");
        }
        else
        {
            tableauTrie(nombreAAjouter);
        }
    }
        catch (const invalid_argument &e)
        {
            cout << e.what();
        }
        return 0;
    }
