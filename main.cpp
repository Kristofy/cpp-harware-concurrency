#include <bits/stdc++.h>

using namespace std;

template<typename T, typename Seq>
struct expander;

template<typename T, size_t... Is>
struct expander<T, index_sequence<Is...>> {
    template<typename E, size_t>
    using elem = E;
    using type = tuple<elem<T, Is>...>;
};

template <typename T, int N>
struct repeat_tuple {
    using type = typename expander<T, make_index_sequence<N>>::type;
};

template<typename T, int N>
using repeat_tuple_t = typename repeat_tuple<T, N>::type;

template<int I, typename T>
auto take(vector<T>& v) {
    using EquivalentTuple = repeat_tuple_t<T, I>;
    return *(EquivalentTuple*)(v.data());
}


int main() {
  vector<int> v = {1, 2, 3, 4, 5};
  // note the reversed parameter order

  auto[three, two, one] = take<3>(v); // Az implementáció linkelve
  cout << one << " " << two << " " << three << endl; // 1, 2, 3

}

