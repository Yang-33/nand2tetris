#include "calc.h"

int Calc::Add(int a, int b)
{
	return a + b;
}

int Calc::Subtract(int a, int b)
{
	return a - b;
}

int Calc::Divide(int a, int b)
{
	if (b == 0)
	{
		throw DivideByZeroException("Divide By Zero");	
	}
	return a / b;
}
