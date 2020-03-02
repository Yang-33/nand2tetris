#include <vector>
#include "svector.h"

std::vector<int> Vector::Add(std::vector<int> a, std::vector<int> b) {
	std::vector<int> res = a;
	for (auto c : b) {
		res.push_back(c);
	}
	// int p = "11";
	return res;
}
