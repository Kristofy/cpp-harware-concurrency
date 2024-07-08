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


template <typename T = int> 
std::vector<T> create_sorted_data(int n) {
  auto data = gen::random_range(n, 0, 1'000'000'000);
  std::sort(data.begin(), data.end());

  return data;
}


template <typename T>
constexpr T add(T& a, T& b) {
  return a + b;
}

template <typename T>
constexpr T sub(T& a, T& b) {
  return a - b;
}

template <typename T>
constexpr T mul(T& a, T& b) {
  return a * b;
}

template <typename T>
constexpr T div(T& a, T& b) {
  return a / b;
}

template <typename T>
constexpr T band(T& a, T& b) {
  return a & b;
}

template <typename T>
constexpr T bor(T& a, T& b) {
  return a | b;
}

template <typename T>
constexpr T mod(T& a, T& b) {
  return a % b;
}


template <typename T>
using BinaryOp = T(*)(T&, T&);

constexpr int OPS_PER_QUERY = 128;
template <typename T, BinaryOp<T>... Functions>
static void bin_op_bench(benchmark::State &state) {
  auto lhs = gen::random_range<T>(OPS_PER_QUERY, 1, 10'000);
  auto rhs = gen::random_range<T>(OPS_PER_QUERY, 1, 10'000);
  for (auto _ : state) {
    for (int t = 0; t < OPS_PER_QUERY; ++t)
      (void(benchmark::DoNotOptimize(Functions(lhs[t], rhs[t]))), ...);
  }
}


static void bin_mixed_v0_bench(benchmark::State &state) {
  std::vector<float> floats = gen::random_range<float>(OPS_PER_QUERY, 1, 10'000);
  std::vector<float> floats2 = gen::random_range<float>(OPS_PER_QUERY, 1, 10'000);
  std::vector<int> ints = gen::random_range<int>(OPS_PER_QUERY, 1, 10'000);
  std::vector<int> ints2 = gen::random_range<int>(OPS_PER_QUERY, 1, 10'000);
  for(auto _ : state) {
    for (int t = 0; t < OPS_PER_QUERY; ++t) {
        benchmark::DoNotOptimize(floats[t] + floats2[t]);
        benchmark::DoNotOptimize(ints[t] + ints[t]);
        benchmark::DoNotOptimize(floats[t] + floats2[t]);
        benchmark::DoNotOptimize(ints[t] + ints2[t]);
        benchmark::DoNotOptimize(floats[t] + floats2[t]);
        benchmark::DoNotOptimize(ints[t] + ints2[t]);
        benchmark::DoNotOptimize(floats[t] + floats2[t]);
        benchmark::DoNotOptimize(ints[t] + ints2[t]);
    }
  }
}

static void mod_constant_bench(benchmark::State &state) {
  auto lhs = gen::random_range<int>(OPS_PER_QUERY, 1, 10'000);
  for (auto _ : state) {
    for (int t = 0; t < OPS_PER_QUERY; ++t)
        benchmark::DoNotOptimize(lhs[t] % 100);
  }
}




static auto add_bench = bin_op_bench<int, add>;
static auto sub_bench = bin_op_bench<int, sub>;
static auto mul_bench = bin_op_bench<int, mul>;
static auto div_bench = bin_op_bench<int, div>;
static auto bor_bench = bin_op_bench<int, bor>;
static auto band_bench = bin_op_bench<int, band>;
static auto mod_bench = bin_op_bench<int, mod>;

static auto addx2_bench = bin_op_bench<int, add, add>;
static auto addx4_bench = bin_op_bench<int, add, add, add, add>;
static auto addx8_bench = bin_op_bench<int, add, add, add, add, add, add, add, add>;
static auto addx16_bench = bin_op_bench<int, add, add, add, add, add, add, add, add, add, add, add, add, add, add, add, add>;


static auto addfx4_bench = bin_op_bench<float, add, add, add, add>;
static auto bandx4_bench = bin_op_bench<int, band, band, band, band>;


#define FIX_ITERATIONS ->Iterations(200)
// #define SETUP FIX_ITERATIONS ->Unit(benchmark::kMicrosecond)
#define SETUP ->Unit(benchmark::kNanosecond)

BENCHMARK(add_bench) SETUP;
BENCHMARK(sub_bench) SETUP;
BENCHMARK(band_bench) SETUP;
BENCHMARK(bor_bench) SETUP;
BENCHMARK(mul_bench) SETUP;
BENCHMARK(div_bench) SETUP;
BENCHMARK(mod_bench) SETUP;
BENCHMARK(mod_constant_bench) SETUP;


BENCHMARK(add_bench) SETUP;
BENCHMARK(addx2_bench) SETUP;
BENCHMARK(addx4_bench) SETUP;
BENCHMARK(addx8_bench) SETUP;
BENCHMARK(addx16_bench) SETUP;

BENCHMARK(addx4_bench) SETUP;
BENCHMARK(addfx4_bench) SETUP;
BENCHMARK(bin_mixed_v0_bench) SETUP;

#ifndef TEST
BENCHMARK_MAIN();
#endif
