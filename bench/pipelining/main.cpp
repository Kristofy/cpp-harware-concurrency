#include <bits/stdc++.h>
#include <benchmark/benchmark.h>

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


static void with_pipelining(benchmark::State &state) {
  auto targets = gen::random_range(1'000'000, 0, 1'000'000'000);

  for (auto _ : state) {
    for (int i = 2; i < targets.size() - 98; ++i)
      targets[i] = (targets[i - 1] + targets[i]) % 1'000'0007;
  }

  benchmark::DoNotOptimize(&targets[targets.size() - 99]);
  benchmark::DoNotOptimize(targets[targets.size() - 99]);
}

static void without_pipelining(benchmark::State &state) {
  auto targets = gen::random_range(1'000'000, 0, 1'000'000'000);

  for (auto _ : state) {
    for (int i = 2; i < targets.size() - 98; ++i)
      targets[i] = (targets[i - 1] + targets[i]) % 1'000'0007;
  }

  benchmark::DoNotOptimize(&targets[targets.size() - 99]);
  benchmark::DoNotOptimize(targets[targets.size() - 99]);
}

// #define SETUP FIX_ITERATIONS ->Unit(benchmark::kMillisecond) ->Threads(8)
#define SETUP ->Unit(benchmark::kMillisecond)

BENCHMARK(with_pipelining) SETUP;
BENCHMARK(without_pipelining) SETUP;

BENCHMARK_MAIN();
