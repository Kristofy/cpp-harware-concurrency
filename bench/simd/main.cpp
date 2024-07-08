#include <bits/stdc++.h>
#include <benchmark/benchmark.h>

#include <immintrin.h>
#include <mm_malloc.h>

namespace gen {
template <typename T = int>
std::vector<T> random_range(int n, T min, T max, int seed = 0) {
  std::vector<T> range(n);
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> dis(min, max);
  for (int i = 0; i < n; ++i) {
    range[i] = static_cast<T>(dis(gen));
  }
  return range;
}
}


static void add(benchmark::State &state) {
  auto targets = gen::random_range(1'000'000, 0, 1'000'000'000);
  int* data = targets.data();

  for (auto _ : state) {
      for(int z = 0; z < 1'024; z++) {
        for(int i = 0; i < 1'024; i++) {
          data[z]=data[i]+data[i];
      }
    }
  }

  volatile int check = 0;
  for(int i = 0; i < 1'024; i++) check ^= data[i];

}

static void add_simd_gen(benchmark::State &state) {
  auto targets = gen::random_range(1'000'000, 0, 1'000'000'000);
  int* data = targets.data();
  for (auto _ : state) {
    for (int z = 0; z < 1'024; z++) {
      for (int i = 0; i < 1'024 / 8; i += 8) {
        for (int j = 0; j < 8; j++) {
          data[z] = data[i + j] + data[i + j];
        }
      }
    }
  }

  volatile int check = 0;
  for(int i = 0; i < 1'024; i++) check ^= data[i];

}

static void add_simd_explicit(benchmark::State &state) {
  auto targets = gen::random_range(1'000'000, 0, 1'000'000'000);
  int* data = targets.data();

  for (auto _ : state) {
    for (int z = 0; z < 1'024; z++) {
      for (int i = 0; i < 1'024 / 8; i += 8) {
        __m256i loaded = _mm256_loadu_si256((__m256i_u *)(data + i));
        __m256i ans = _mm256_add_epi32(loaded, loaded);
        _mm256_storeu_si256((__m256i_u *)(data + z), ans);
      };
    }
  }

  volatile int check = 0;
  for(int i = 0; i < 1'024; i++) check ^= data[i];
}

// #define SETUP FIX_ITERATIONS ->Unit(benchmark::kMillisecond) ->Threads(8)
#define SETUP ->Unit(benchmark::kMillisecond)

BENCHMARK(add) SETUP;
BENCHMARK(add_simd_gen) SETUP;
BENCHMARK(add_simd_explicit) SETUP;

BENCHMARK_MAIN();
