#include <string>
#include <exception>
#include "src_Export.h"

class DivideByZeroException : public std::exception
{
public:
	explicit DivideByZeroException(const char* message)
#ifdef _MSC_VER
		: std::exception(message)
#else
		: std::exception()
#endif
	{
	}
};

class src_EXPORT Calc
{
public:
	int Add(int a, int b);
	int Subtract(int a, int b);
	int Divide(int a, int b);
};
