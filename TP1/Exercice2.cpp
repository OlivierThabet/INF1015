/*
Programme qui détermine la limite de la 
suite Un+1 = sqrt(2 + Un) avec une 
précision déterminée par l'utilisateur
*/

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

double computeNextU(double Un)
/*Calcul de Un+1*/
{
	return sqrt(2 + Un);
};

int main()
{
	cout << "Precision (nombre de chiffres après la virgule) : ";
	int Precision = 5;
	cin >> Precision;
	double Un = 0.0;
	int PowOfTen = pow(10, Precision);
	while ( (computeNextU(Un) * PowOfTen) / PowOfTen != (Un * PowOfTen) / PowOfTen )
	{
		Un = computeNextU(Un);
	}
	cout << "La limite est : " << fixed << setprecision(Precision) << Un << endl;

	return 0;
}