/*
Programme qui effectue la multiplication
russe pour deux nombres et qui verifie cette
derniere au moyen de trois tests.
*/

#include <array>
#include <iostream>
#include <string>
using namespace std;

struct Test
{
	int a;
	int b;
	int c;
};

int russianMultiplication(int a, int b)
{
	int result = 0;

	while (a > 0)
	{
		if (a % 2 != 0)
		{
			result += b;
		};
		a /= 2;
		b *= 2;
	}
	return result;
}

int main()
{
	Test myArray[3];
	myArray[0].a = 37;
	myArray[0].b = 129;
	myArray[0].c = russianMultiplication(37, 129);

	myArray[1].a = 69;
	myArray[1].b = 420;
	myArray[1].c = russianMultiplication(69, 420);

	myArray[2].a = 42;
	myArray[2].b = 1048;
	myArray[2].c = russianMultiplication(42, 1048);
	int nTest = 3;
	int nTestSucceed = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (myArray[i].a * myArray[i].b == myArray[i].c)
		{
			nTestSucceed++;
		}
	}
	cout << nTestSucceed << "/" << nTest << " reussis.";
}