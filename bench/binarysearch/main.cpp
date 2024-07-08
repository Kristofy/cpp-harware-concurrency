#include <benchmark/benchmark.h>
#include <bits/stdc++.h>
#include <immintrin.h>

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
static int usaco(const std::vector<T> &data, int target) {
  int lo = 0, hi = data.size();
  while (lo < hi) {
    int mid = lo + (hi - lo) / 2;
    if (data[mid] >= target) {
      hi = mid;
    } else {
      lo = mid + 1;
    }
  }
  return data[lo] == target ? lo : data.size();
}

template <typename T = int>
static int iterative_v0(const std::vector<T> &data, T target) {
  int pred = 0;
  // 31 is too much because 2^32 - 1 would be the sum
  for (int k = 30; k >= 0; --k) {
    auto idx = pred + (1 << k);
    if (idx < (int)data.size() && data[idx] <= target) {
      pred = idx;
    }
  }
  return data[pred] == target ? pred : data.size();
}

template <typename T = int>
static int iterative_v1(const std::vector<T> &data, T target) {
  const int n = data.size();
  int pred = 0;
  for (int k = 1 << 30; k > 0; k /= 2) {
    pred += (pred + k < n && data[pred + k] <= target) * k;
  }
  return data[pred] == target ? pred : data.size();
}

template <typename T = int>
static int iterative_v2(const std::vector<T> &data, T target) {
  const T *ptr = data.data();
  int len = data.size();
  while (len > 1) {
    int half = len / 2;
    ptr += (ptr[half - 1] < target) * half;
    len -= half;
  }
  return *ptr == target ? ptr - data.data() : data.size();
}

template <typename T = int>
static int standard(const std::vector<T> &data, T target) {
  const auto idx =
      std::lower_bound(data.begin(), data.end(), target) - data.begin();
  return data[idx] == target ? idx : data.size();
}

template <typename T = int>
static int linear(const std::vector<T> &data, T target) {
  for (int i = 0; i < static_cast<int>(data.size()); ++i) {
    if (data[i] == target) {
      return i;
    }
  }
  return data.size();
}

template <typename T = int> std::vector<T> create_sorted_data(int n) {
  auto data = gen::random_range(n, 0, 1'000'000'000);
  std::sort(data.begin(), data.end());

  return data;
}

constexpr int OPS_PER_QUERY = 10000;

template <typename T, int (*F)(const std::vector<T> &, T),
          std::vector<T> (*Create)(int) = create_sorted_data>
static void sorted_bench(benchmark::State &state) {
  const int n = state.range(0);
  auto data = Create(n);
  const auto targets = gen::random_range(OPS_PER_QUERY, 0, 1'000'000'000);
  for (auto _ : state) {
    for (int t = 0; t < OPS_PER_QUERY; ++t)
      benchmark::DoNotOptimize(F(data, targets[t]));
  }
}

template <typename T, int (*F)(const std::vector<T> &, T),
          int (*H)(const std::vector<T> &, T),
          std::vector<T> (*Create)(int) = create_sorted_data>
static bool is_same(int n = 100'000) {
  auto data = Create(n);
  const auto targets = gen::random_range(OPS_PER_QUERY, 0, 1'000'000'000);
  for (int t = 0; t < OPS_PER_QUERY; ++t) {
    auto x = F(data, targets[t]);
    auto y = H(data, targets[t]);

    if (x == (int)data.size() || y == (int)data.size()) {
      if (x != y) {
        return false;
      }
      continue;
    }

    if (data[x] != data[y]) {
      return false;
    }
  }
  return true;
}

static auto linear_bench = sorted_bench<int, linear>;
static auto usaco_bench = sorted_bench<int, usaco>;
static auto iterative_v0_bench = sorted_bench<int, iterative_v0>;
static auto iterative_v1_bench = sorted_bench<int, iterative_v1>;
static auto iterative_v2_bench = sorted_bench<int, iterative_v2>;
static auto standard_bench = sorted_bench<int, standard>;

#define ASSERT_SAME_N(T, F, H, n)                                              \
  do {                                                                         \
    std::cerr << (!is_same<T, F, H>(n)                                         \
                      ? "Failed Test, " #F " and " #H " are not the same\n"    \
                      : "Test Passed, " #F " and " #H " are the same\n");      \
  } while (0)

#define ASSERT_SAME(T, F, H)                                                   \
  do {                                                                         \
    std::cerr << (!is_same<T, F, H>()                                          \
                      ? "Failed Test, " #F " and " #H " are not the same\n"    \
                      : "Test Passed, " #F " and " #H " are the same\n");      \
  } while (0)

#ifdef TEST
int main() {
#else
static int _main() {
#endif
  ASSERT_SAME_N(int, standard, linear, 100);
  ASSERT_SAME(int, standard, usaco);
  ASSERT_SAME(int, standard, iterative_v0);
  ASSERT_SAME(int, standard, iterative_v1);
  ASSERT_SAME(int, standard, iterative_v2);

  return 0;
};

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
  Args({1000 * 1})                                                             \
      ->Args({1000 * 2})                                                       \
      ->Args({1000 * 3})                                                       \
      ->Args({1000 * 4})                                                       \
      ->Args({1000 * 5})                                                       \
      ->Args({1000 * 6})                                                       \
      ->Args({1000 * 7})                                                       \
      ->Args({1000 * 8})                                                       \
      ->Args({1000 * 10})

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
      ->Args({100'000 * 10})                                                   \
      ->Args({100'000 * 11})                                                   \
      ->Args({100'000 * 12})                                                   \
      ->Args({100'000 * 13})                                                   \
      ->Args({100'000 * 15})                                                   \
      ->Args({100'000 * 16})                                                   \
      ->Args({100'000 * 17})                                                   \
      ->Args({100'000 * 18})                                                   \
      ->Args({100'000 * 19})                                                   \
      ->Args({100'000 * 21})                                                   \
      ->Args({100'000 * 22})                                                   \
      ->Args({100'000 * 22})

#define INTTERVALS ->Unit(benchmark::kMillisecond)->FAST

#define FIX_ITERATIONS ->Iterations(200)
// #define SETUP FIX_ITERATIONS ->Unit(benchmark::kMillisecond) ->Threads(8)
// #define SETUP FIX_ITERATIONS ->Unit(benchmark::kMillisecond)
#define SETUP ->Unit(benchmark::kMillisecond)

BENCHMARK(usaco_bench) SETUP -> FAST;
BENCHMARK(standard_bench) SETUP -> FAST;
BENCHMARK(linear_bench) SETUP -> FAST;
// BENCHMARK(iterative_v0_bench) SETUP -> FAST;
// BENCHMARK(iterative_v1_bench) SETUP -> FAST;
// BENCHMARK(iterative_v2_bench) SETUP -> FAST;

// BENCHMARK(usaco_bench) SETUP -> MEDIUM;
// BENCHMARK(standard_bench) SETUP -> MEDIUM;
// BENCHMARK(iterative_v0_bench) SETUP -> MEDIUM;
// BENCHMARK(iterative_v1_bench) SETUP -> MEDIUM;
// BENCHMARK(iterative_v2_bench) SETUP -> MEDIUM;

// BENCHMARK(usaco_bench) SETUP->SLOW;
// BENCHMARK(standard_bench) SETUP->SLOW;
// BENCHMARK(iterative_v0_bench) SETUP->SLOW;
// BENCHMARK(iterative_v1_bench) SETUP->SLOW;
// BENCHMARK(iterative_v2_bench) SETUP->SLOW;

#ifndef TEST
BENCHMARK_MAIN();
#endif
