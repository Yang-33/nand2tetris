#include <iostream>

#include "nest/svector.h"

int main() {
	std::cout << "tapichan" << std::endl;

	Vector v;
	std::vector<int> a = { 11,22 };
	std::vector<int> b = { 33,44 };
	std::vector<int> c = v.Add(a, b);
	for (auto e : c) {
		std::cout << e << std::endl;
	}
	std::cout << "success!" << std::endl;

	return 0;
}