#include <bits/stdc++.h>
#include <benchmark/benchmark.h>

namespace gen {

template <typename T = int>
struct range_spec {
  int n;
  T min;
  T max;
  int seed = 0;
};


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
template <typename T = int>
std::vector<T> random_range(range_spec<T> spec) {
  return random_range(spec.n, spec.min, spec.max, spec.seed);
}
}

static void always_hit(benchmark::State &state) {
  auto targets = gen::random_range({
    .n = 1'000'000, 
    .min = 0,
    .max = 1'000'000'000
  });
  auto rng = gen::random_range(1'000'000, 0, 1'000'000'000);

  for (auto _ : state) {
    int i = 0;
    benchmark::DoNotOptimize(&i);
    for (int z = 0; z < 10'000; z++) {
      if (rng[i] < 1'000'000'000 - 1) {
        i += 1;
      } else {
        i += 2;
      }
    }
  }
}

static void random(benchmark::State &state) {
  auto targets = gen::random_range(1'000'000, 0, 1'000'000'000);
  auto rng = gen::random_range(1'000'000, 0, 1'000'000'000);

  for (auto _ : state) {
    int i = 0;
    benchmark::DoNotOptimize(&i);

    for (int z = 0; z < 10'000; z++) {
      if (rng[i] * 2 > 1'000'000'000) {
        i += 1;
      } else {
        i += 2;
      }
    }
  }
}

static void random_branchless(benchmark::State &state) {
  auto targets = gen::random_range(1'000'000, 0, 1'000'000'000);
  auto rng = gen::random_range(1'000'000, 0, 1'000'000'000);

  for (auto _ : state) {
    int i = 0;
    benchmark::DoNotOptimize(&i);

    for (int z = 0; z < 10'000; z++) {
      i += 1 + (rng[i] * 2 > 1'000'000'000);
    }
  }
}
// #define SETUP FIX_ITERATIONS ->Unit(benchmark::kMillisecond) ->Threads(8)
#define SETUP ->Unit(benchmark::kMillisecond)

// BENCHMARK(always_hit) SETUP;
BENCHMARK(random) SETUP;
// BENCHMARK(random_branchless) SETUP;

BENCHMARK_MAIN();
