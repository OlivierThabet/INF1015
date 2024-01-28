#include <iostream>
using namespace std;

struct Test
{
    int num1;
    int num2;
    int resultat() const
    {
        return num1 * num2;
    }
};

int multiRusse(int a, int b)
{
    int produit = 0;

    while (a >= 1)
    {
        if (a % 2 != 0)
        {
            produit += b;
        }
        a /= 2;
        b *= 2;
        
    }
    return produit;
}
int main()
{
    struct Test tests[]{{1, 47}, {36, 48}, {47, 50}};
    int ntestsReussis = 0;
    for (int i = 0; i < 3; i++)
    {
        if (tests[i].resultat() == multiRusse(tests[i].num1, tests[i].num2))
        {
            ntestsReussis += 1;
        }
    }
    cout << ntestsReussis << "/3 tests passent.";
}
