#include <algorithm>
#include <benchmark/benchmark.h>
#include <bits/stdc++.h>
#include <random>

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
} // namespace gen

template <typename T>
static void enumerate_1d_forward(benchmark::State &state) {
  const auto n = state.range(0);
  auto data = gen::random_range<T>(n, 0, 1'000'000'000);
  T leak = 0;
  benchmark::DoNotOptimize(&leak);
  for (auto _ : state) {
    for (int i = 0; i < n; ++i) {
      leak^=data[i];
    }
  }
}

template <typename T>
static void enumerate_1d_backward(benchmark::State &state) {
  const auto n = state.range(0);
  auto data = gen::random_range<T>(n, 0, 1'000'000'000);
  T leak = 0;
  benchmark::DoNotOptimize(&leak);
  for (auto _ : state) {
    for (int i = n - 1; i >= 0; --i) {
      leak^=data[i];
    }
  }
}

template <typename T>
static void enumerate_1d_line(benchmark::State &state) {
  const int n = state.range(0);
  auto data = gen::random_range<T>(n, 0, 1'000'000'000);
  T leak = 0;
  benchmark::DoNotOptimize(&leak);
  for (auto _ : state) {
    for (int i = n; i >= 0; i -= (int)(64 / sizeof(T))) { 
      for (int j = 0; j < (int)(64 / sizeof(T)); ++j) {
        leak^=data[i + j];
      }
    }
    for(int i = 0; i < n % (int)(64 / sizeof(T)); ++i) {
      leak^=data[i];
    }
  }
}

template <typename T>
static void enumerate_1d_random(benchmark::State &state) {
  const int n = state.range(0);
  auto data = gen::random_range<T>(n, 0, 1'000'000'000);
  std::vector<int> indices(n);
  std::iota(std::begin(indices), std::end(indices), 0);
  std::shuffle(std::begin(indices),std::end(indices), std::mt19937(rand()));
  T leak = 0;
  benchmark::DoNotOptimize(&leak);
  for (auto _ : state) {
    for (int i : indices) {
      leak^=data[i];
    }
  }
}


template <typename T>
static void enumerate_2d_forward_rowise_bench(benchmark::State &state) {
  const int n = state.range(0);
  auto data = gen::random_range<T>(n * n, 0, 1'000'000'000);
  T leak = 0;
  benchmark::DoNotOptimize(&leak);
  for (auto _ : state) {
    for(int i = 0; i < n * n; ++i) {
        leak^=data[i];
    }
  }
}


template <typename T>
static void enumerate_2d_forward_colwise_bench(benchmark::State &state) {
  const int n = state.range(0);
  auto data = gen::random_range<T>(n * n, 0, 1'000'000'000);
  T leak = 0;
  benchmark::DoNotOptimize(&leak);
  for (auto _ : state) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
         leak^=data[j * n + i];
      }
    }
  }
}


template <typename T>
static void enumerate_2d_forward_random_bench(benchmark::State &state) {
  const int n = state.range(0);
  auto data = gen::random_range<T>(n * n, 0, 1'000'000'000);
  std::vector<int> indices(n*n);
  std::iota(std::begin(indices), std::end(indices), 0);
  std::shuffle(std::begin(indices), std::end(indices), std::mt19937(rand()));
  T leak = 0;
  benchmark::DoNotOptimize(&leak);
  for (auto _ : state) {
    for(int k : indices) {
      leak^=data[k];
    }
  }
}

static auto enumerate_1d_forward_int_bench = enumerate_1d_forward<int>;
static auto enumerate_1d_forward_long_long_bench = enumerate_1d_forward<long long>;

static auto enumerate_1d_line_int_bench = enumerate_1d_line<int>;
static auto enumerate_1d_random_int_bench = enumerate_1d_random<int>;

static auto enumerate_1d_backward_int_bench = enumerate_1d_backward<int>;

static auto enumerate_2d_forward_rowise_int_bench = enumerate_2d_forward_rowise_bench<int>;
static auto enumerate_2d_forward_colwise_int_bench = enumerate_2d_forward_colwise_bench<int>;
static auto enumerate_2d_forward_random_int_bench = enumerate_2d_forward_random_bench<int>;

#define FAST                                                                   \
  Args({10 * 1})                                                               \
      ->Args({10 * 2})                                                         \
      ->Args({10 * 3})                                                         \
      ->Args({10 * 4})                                                         \
      ->Args({10 * 5})                                                         \
      ->Args({10 * 6})                                                         \
      ->Args({10 * 7})                                                         \
      ->Args({10 * 8})                                                         \
      ->Args({10 * 9})                                                         \
      ->Args({10 * 10})

#define MEDIUM                                                                 \
  Args({100 * 1})                                                             \
      ->Args({100 * 2})                                                       \
      ->Args({100 * 3})                                                       \
      ->Args({100 * 4})                                                       \
      ->Args({100 * 5})                                                       \
      ->Args({100 * 6})                                                       \
      ->Args({100 * 7})                                                       \
      ->Args({100 * 8})                                                       \
      ->Args({100 * 9})                                                       \
      ->Args({100 * 10})

#define SLOW                                                                   \
  Args({100'000 * 1})                                                          \
      ->Args({100'000 * 2})                                                    \
      ->Args({100'000 * 3})                                                    \
      ->Args({100'000 * 4})                                                    \
      ->Args({100'000 * 5})                                                    \
      ->Args({100'000 * 6})                                                    \
      ->Args({100'000 * 7})                                                    \
      ->Args({100'000 * 8})                                                    \
      ->Args({100'000 * 9})                                                    \
      ->Args({100'000 * 10})                                                    \
      ->Args({100'000 * 11})                                                    \
      ->Args({100'000 * 12})                                                    \
      ->Args({100'000 * 13})                                                    \
      ->Args({100'000 * 14})                                                    \
      ->Args({100'000 * 15})

#define INTTERVALS ->Unit(benchmark::kMillisecond)->FAST

#define FIX_ITERATIONS ->Iterations(200)
#define SETUP ->Unit(benchmark::kMicrosecond)

// BENCHMARK(enumerate_1d_forward_int_bench) SETUP->MEDIUM;
// BENCHMARK(enumerate_1d_forward_long_long_bench) SETUP->MEDIUM;

// BENCHMARK(enumerate_1d_forward_int_bench) SETUP->SLOW;
// BENCHMARK(enumerate_1d_forward_long_long_bench) SETUP->SLOW;

// BENCHMARK(enumerate_1d_forward_int_bench) SETUP->SLOW;
// BENCHMARK(enumerate_1d_backward_int_bench) SETUP->SLOW;
// BENCHMARK(enumerate_1d_line_int_bench) SETUP->SLOW;
// BENCHMARK(enumerate_1d_random_int_bench) SETUP->SLOW;

// BENCHMARK(enumerate_2d_forward_rowise_int_bench) SETUP->MEDIUM;
// BENCHMARK(enumerate_2d_forward_colwise_int_bench) SETUP->MEDIUM;
// BENCHMARK(enumerate_2d_forward_random_int_bench) SETUP->MEDIUM;

BENCHMARK(enumerate_2d_forward_rowise_int_bench) SETUP->FAST;
BENCHMARK(enumerate_2d_forward_colwise_int_bench) SETUP->FAST;
BENCHMARK(enumerate_2d_forward_random_int_bench) SETUP->FAST;

#ifndef TEST
BENCHMARK_MAIN();
#endif
