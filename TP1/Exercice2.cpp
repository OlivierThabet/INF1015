/*
Programme qui determine la limite de la
suite Un+1 = sqrt(2 + Un) avec une
precision determinee par l'utilisateur
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

double computeNextU(double un)
/*Calcul de un+1*/
{
	return sqrt(2 + un);
};

int main()
{
	cout << "Precision (nombre de chiffres apres la virgule) : ";
	int precision = 5;
	cin >> precision;
	double un = 0.0;
	int powOfTen = pow(10, precision);
	while ((computeNextU(un) * powOfTen) / powOfTen != (un * powOfTen) / powOfTen)
	{
		un = computeNextU(un);
	}
	cout << "La limite est : " << fixed << setprecision(Precision) << un << endl;

	return 0;
}