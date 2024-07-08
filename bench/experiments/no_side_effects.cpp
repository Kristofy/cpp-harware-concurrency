int main() {
	int v[1'000'000];
	for(int k = 0; k < 20; k++) {
		for(int i = 0; i < 1'000'000; i++) {
			for(int j = 0; j < i; j++) {
				v[i] = (v[i] + j) + 1'000'0007;
			}
		}
	}

	return 0;
}
