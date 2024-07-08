---
theme: seriph
title: Segment-Tree and Sparse-table
class: text-center
highlighter: shiki
drawings:
  persist: false
transition: slide-left
mdc: true
---

# Levels Of Hardware Concurrency

---
---

### Quiz: Milyen gyors a következő program?

O(n^2) algoritmus, 1 millió elemre. Háromszögszámok Kiszámíttása
```cpp
int main() {
	int v[1'000'000];
	for(int i = 0; i < 1'000'000; i++) {
		for(int j = 0; j < i; j++) {
			v[i] = (v[i] + j) % 1'000'0007;
		}
	}

	return 0;
}
```

* A) 5-20 s
* B) 1-5 s
* C) 200-1000 ms
* D) 50 - 200 ms
* E) 5 - 50 ms
* F) < 5 ms

<v-click>

- Nincs megfigyelhető melékhatás, és a fordíttó "as if" elven fordítt

</v-click>


---
---

### Quiz: Milyen gyorsak a következő utasíttások? Állítsd sorrendbe

Pl.: Szorzás < kivonás, bitszerinti és < osztás < összeadás

* Összeadás
* Kivonás
* Bitszerinti és
* Szorzás
* Osztás

---
---

Az eredmények 128 számíttásra értelmezettek

| Benchmark  | Time    | CPU     | Iterations |
| ---------- | ------- | ------- | ---------- |
| add_bench  | 65.1 ns | 65.1 ns | 10086556   |
| sub_bench  | 41.5 ns | 41.5 ns | 16866030   |
| band_bench | 40.7 ns | 40.7 ns | 17262059   |
| bor_bench  | 40.5 ns | 40.5 ns | 17214392   |
| mul_bench  | 36.8 ns | 36.7 ns | 20222192   |
| div_bench  | 184 ns  | 184 ns  | 3951897    |

- Mit jelenthet ha a `Time` és a `CPU` értékek eltérnek?

---
---

### Quiz: Mi történt?

| Benchmark          | Time    | CPU     | Iterations |
| ------------------ | ------- | ------- | ---------- |
| mod_bench          | 177 ns  | 177 ns  | 3964187    |
| mod_constant_bench | 98.9 ns | 98.9 ns | 7064763    |

Változó értékkel / konstant értékkel vett modulo

```cpp
int a, b;
cin >> a >> b;
cout << a % b;
```

```cpp
int a;
cin >> a;
cout << a % 1000000007;
```

<v-click>

- A konstans modulót a fordíttó lecseréli szorzás és bitshift műveletekre
- Hasonlóképpen mint a `a = a / 2` Ugyan az mint `a = a >> 1`

</v-click>

---
---

### Quiz: Ha 1 művelet X idő akkor 2, vagy 4 szer ugyan az a művelet mennyi idő?

* A) Ugyan annyi mint 1 darab
* B) Annyiszorosa mint ahány darab
* C) Nem tudom

<v-click>

| Benchmark    | Time    | CPU     | Iterations | Per operation |
| ------------ | ------- | ------- | ---------- | ------------- |
| add_bench    | 64.4 ns | 64.4 ns | 10916068   | 64.4 ns       |
| addx2_bench  | 67.2 ns | 67.1 ns | 10692708   | 33.6 ns       |
| addx4_bench  | 99.7 ns | 99.6 ns | 7522478    | 24.9 ns       |
| addx8_bench  | 183 ns  | 183 ns  | 3837707    | 22.9 ns       |
| addx16_bench | 369 ns  | 368 ns  | 1915196    | 23.1 ns       |

</v-click>

---
---

## [Superscalar execution](https://en.wikipedia.org/wiki/Superscalar_processor)


- Több végrehajtó egység van minden processzor magban amelyek tudnak számíttásokat végezni
- Latency vs Throughput

---
---

## [Instruction pipelining](https://en.wikipedia.org/wiki/Instruction_pipelining)

Ciklusokon keresztül is működik feltéve, hogy nincs függőség

```cpp
std::vector<int> fib(100);
fib[1] = 1;
for(int i = 2; i < v.size(); i++) {
	fib[i] = fib[i-1] + fib[i-2];
}
```

- Gyenge függőség

---
---

Erős függőség

```cpp
int i = 0;
for (int z = 0; z < 10'000; z++) {
  if (random_szamok[i] * 2 < 1'000'000'000) {
	i += 1;
  } else {
	i += 2;
  }
}
```

- Nem tudni hogy a +1-edik vagy a +2edik elemet kell-e használni

---
---

### Quiz: Adottak a következő mérések 4 int pár és 4 float pár összeadására, mennyi idő a kettő együtt?

<v-switch>
  <template #0>

| Benchmark          | Time    | CPU     | Iterations | Per operation |
| ------------------ | ------- | ------- | ---------- | ------------- |
| addx4_bench        | 92.9 ns | 92.9 ns | 7534969    | 23.2 ns       |
| addfx4_bench       | 171 ns  | 171 ns  | 4083067    | 42.8 ns       |

</template>

<template #1> 


| Benchmark          | Time    | CPU     | Iterations | Per operation |
| ------------------ | ------- | ------- | ---------- | ------------- |
| addx4_bench        | 92.9 ns | 92.9 ns | 7534969    | 23.2 ns       |
| addfx4_bench       | 171 ns  | 171 ns  | 4083067    | 42.8 ns       |
| bin_mixed_v0_bench | 216 ns  | 216 ns  | 3468772    | 27 ns         |

</template>

</v-switch>


---
---

## [Out of order execution](https://en.wikipedia.org/wiki/Out-of-order_execution)

- A fordíttó "as if" elvéhez hasonló
- A processzor újra rendezheti a műveleteket, hogy a végrehajtás gyorsabb legyen

- [Nézzük meg](https://www.anandtech.com/show/6355/intels-haswell-architecture/8)
- Több ALU, FPU és SIMD egység is lehet

---
---

## SIMD

### Quiz: Melyik a leggyorsabb összeadás művelet?

<v-clicks>

<div>

* A)

```cpp
for(int i = 0; i < 1'024; i++)
	targets[i]+targets[i];
```

</div>

<div>

* B)

```cpp
for(int i = 0; i < 1'024 / 8; i++)
  for(int j = 0; j < 8; j++)
	  targets[i*8 + j] + targets[i*8 + j];
```
</div>

<div>

* C)

```cpp
for(int i = 0; i < 1'024 / 8; i+=8)
  for(int j = 0; j < 8; j++)
  	 targets[i + j] + targets[i + j];
```


</div>

<div>

* D)

```cpp
for (int i = 0; i < 1'024 / 8; i += 8) {
	__m256i loaded = _mm256_loadu_si256((__m256i_u *)(data + i));
	__m256i ans = _mm256_add_epi32(loaded, loaded);
}
```
</div>
</v-clicks>

---
---

## Branch prediction
### Quiz: Melyik a leggyorsabb művelet?

* A)
```cpp
int i = 0;
for (int z = 0; z < 10'000; z++) {
  if (rng[i] < 1'000'000'000 - 1) { i += 1; } else { i += 2; }
}
```

* B)
```cpp
int i = 0;
for (int z = 0; z < 10'000; z++) {
  if (rng[i] * 2* < 1'000'000'000) { i += 1; } else { i += 2; }
}
```

* C)
```cpp
for (int z = 0; z < 10'000; z++) {
  i += 1 + (rng[i] * 2 > 1'000'000'000);
}
```

---
---

### Eszközök

- C++20 -tól a nyelvnek része a `[[likely]]` és `[[unlikely]]` attribútum

```cpp
if (a < 10) [[likely]] {
  // ...
}
```

- Régebbi verziókon a `__builtin_expect` 

```cpp
if (__builtin_expect(a < 10, 1)) {
  // ...
}
```

- `perf stat` a mérésre


---
---

## Caches

- Cache hierarchia
- Cache Sebessége
- Cache mérete
- Cache egysége

---
---

### Quiz: mekkora a különbség az előre és hátra olvasás között?

```cpp
for (int i = 0; i < n; ++i) {
  data[i];
}
```

vs 

```cpp
for (int i = n - 1; i >= 0; --i) {
  data[i];
}
```

<v-click>

* Semmi, a fordíttó és a CPU okos

</v-click>


---
---

### Eszközök

- Perf stat cache-miss-eket is mér
- `__builtin_prefetch` -el megkérhetjük a processzrot hogy olvasson be adatokat a cache-be

---
---

### Quiz: Mi történt itt?

Int ek és long long ok beolvasása

<img src="/assets/int_vs_long.png" />

---
---

<img src="/assets/enumerate_1d_wo_random.png" />



---
---

<img src="/assets/enumerate_1d_with_random.png" />

---
---


<img src="/assets/enumerate_2d_small.png" />

---
---

<img src="/assets/enumerate_2d.png" />

---
---


# Binary Search

- [Cpp talk](https://www.youtube.com/watch?v=1RIPMQQRBWk)
- [hpc algorithmica](https://en.algorithmica.org/hpc/data-structures/binary-search/)


---
---

## Linear Search

```cpp
int linear(const std::vector<int> &data, int target) {
  for (int i = 0; i < static_cast<int>(data.size()); ++i) {
    if (data[i] == target) {
      return i;
    }
  }
  return data.size();
}
```

---
---

## Standard

```cpp
template <typename T = int>
int standard(const std::vector<T> &data, T target) {
  const auto idx =
      std::lower_bound(data.begin(), data.end(), target) - data.begin();
  return data[idx] == target ? idx : data.size();
}
```

---
---

<img src="/assets/binsearch_standard_vs_linear.png" />

---
---


## Usaco

```cpp
int usaco(const std::vector<T> &data, int target) {
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
```


---
---


<img src="/assets/with_usaco_small.png" />

---
---

## Iterative v0

```cpp
int iterative_v0(const std::vector<int> &data, int target) {
  int pred = 0;
  for (int k = 30; k >= 0; --k) {
    auto idx = pred + (1 << k);
    if (idx < (int)data.size() && data[idx] <= target) {
      pred = idx;
    }
  }
  return data[pred] == target ? pred : data.size();
}
```


---
---

<img src="/assets/with_iterative_v0_small.png" />

---
---


## Iterative v1

```cpp
int iterative_v1(const std::vector<int> &data, int target) {
  const int n = data.size();
  int pred = 0;
  for (int k = 1 << 30; k > 0; k /= 2) {
    pred += (pred + k < n && data[pred + k] <= target) * k;
  }
  return data[pred] == target ? pred : data.size();
}
```


---
---

<img src="/assets/with_iterative_v1_small.png" />

---
---

## Iterative V2

```cpp
int iterative_v2(const std::vector<int> &data, int target) {
  const int *ptr = data.data();
  int len = data.size();
  while (len > 1) {
    int half = len / 2;
    ptr += (ptr[half - 1] < target) * half;
    len -= half;
  }
  return *ptr == target ? ptr - data.data() : data.size();
}
```


---
---


<img src="/assets/binsearch_all_small.png" />

---
---


<img src="/assets/binsearch_all_medium.png" />

---
---


<img src="/assets/binsearch_all_large.png" />

---
---

- Eytzinger layout
- B+ Tree
- SIMD
- [Cpp talk](https://www.youtube.com/watch?v=1RIPMQQRBWk)
- [hpc algorithmica](https://en.algorithmica.org/hpc/data-structures/binary-search/)


---
---


<img src="/assets/binsearch_fast.png" />

---
---

## Hyperthreading

- Számunkra csak akkor hasznos ha io-bound vagy memory-bound a program

---
---

## MultiThreading

### Quiz: Ha egy thread X idő alatt számol el 1'000 ig, akkor 2 thread Mennyi idő alatt?

```cpp
int arr[2];

start_counting(arr[0], 1'000);
start_counting(arr[1], 1'000);
```

<v-click>

- [MESI](https://en.wikipedia.org/wiki/MESI_protocol)

</v-click>

---
---












---
---

### C++ érdekesség

```cpp
struct spec {
    int n;
    int min;
    int max;
};

std::vector<int> random_range(spec s) {
    // ...
}

auto rng = random_range({
	.n = 1'000'000,
	.min = 0,
	.max = 1'000'000'000}
);
```


