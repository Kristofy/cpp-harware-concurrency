#include <algorithm>
#include <bits/stdc++.h>
#include <random>

#define DO_NOT_OPTIMIZE(x) asm volatile("" : : "r,m"(x) : "memory")

int main() {
	std::vector<int> v(100'000);
	iota(v.begin(), v.end(), 0);
	std::shuffle(v.begin(), v.end(), std::mt19937(rand()));

	int total = 0;
	int total2 = INT_MAX;
	DO_NOT_OPTIMIZE(&total);
	DO_NOT_OPTIMIZE(&total2);
	for(int i = 0; i < v.size(); i+=2) {
		if (v[i] < v.size() / 2) {
			total = std::max(total, v[i]);
		} else if(v[i] % 2 == 0) {
			total2 = std::min(total2, v[i]);
		}
	}

	return 0;
}
