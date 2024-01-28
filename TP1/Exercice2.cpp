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
	const int PRECISION = 5;
	double un = 0.0;
	int powOfTen = pow(10, PRECISION);
	while ((computeNextU(un) * powOfTen) / powOfTen != (un * powOfTen) / powOfTen)
	{
		un = computeNextU(un);
	}
	cout << "La limite est : " << fixed << setprecision(PRECISION) << un << endl;

	return 0;
}