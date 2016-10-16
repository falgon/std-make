// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include <experimental/product_type.hpp>
#include <experimental/array.hpp>
#include <sstream>
#include <memory>

#include <boost/detail/lightweight_test.hpp>

int f(int,int,int) { return 0; }

struct X {
  int i;
  int j;
  int k;
  X(int i, int j, int k) : i(i), j(j), k(k){}
};
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
#if 0
  namespace details {
    template<class> struct is_ref_wrapper : std::false_type {};
    template<class T> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

    template<class T>
    using not_ref_wrapper = std::negation<is_ref_wrapper<std::decay_t<T>>>;

    template <class D, class...> struct return_type_helper { using type = D; };
    template <class... Types>
    struct return_type_helper<void, Types...> : std::common_type<Types...> {
        static_assert(std::conjunction_v<not_ref_wrapper<Types>...>,
                      "Types cannot contain reference_wrappers when D is void");
    };

    template <class D, class... Types>
    using return_type = std::array<typename return_type_helper<D, Types...>::type,
                                   sizeof...(Types)>;
  }

  template < class D = void, class... Types>
  constexpr details::return_type<D, Types...> make_array(Types&&... t) {
    return {std::forward<Types>(t)... };
  }
#else
std::array<int,2> make_array(int i, int j) {
  return {{i,j}};
}
#endif
}
}
}

int main()
{
  namespace stde = std::experimental;
  {
      using T = std::array<int,3>;
      static_assert(stde::detail::has_tuple_like_size_access<T>::value, "Hrr");
      static_assert(stde::detail::has_tuple_like_element_access<0, T>::value, "Hrr");
      static_assert(stde::detail::has_tuple_like_get_access<0, T>::value, "Hrr");
      static_assert(stde::has_tuple_like_access<T>::value, "Hrr");
      static_assert(stde::is_product_type_v<T>, "Hrr");
      static_assert(! stde::is_product_type_v<T&>, "Hrr");

      T p  = { {0,1,2} };
      static_assert(3 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      static_assert(std::is_same<int&, decltype(stde::product_type::get<0>(p))>::value, "Hrr");
      BOOST_TEST(3 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      BOOST_TEST(2 == stde::product_type::get<2>(p));
      //auto x = stde::product_type::get<3>(p); // COMPILE FAIL AS REQUIRED
  }
  {
      using T = std::array<int,3>;
      const T p  = { {0,1,2} };
      static_assert(3 == stde::product_type::size<T>::value, "Hrr");
      static_assert(std::is_same<const int, std::tuple_element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<0,decltype(p)>>::value, "Hrr");
      static_assert(std::is_same<const int, stde::product_type::element_t<1,decltype(p)>>::value, "Hrr");
      //static_assert(std::is_same<const int, stde::product_type::element_t<3,decltype(p)>>::value, "Hrr"); // COMPILE FAIL AS REQUIRED
      BOOST_TEST(3 == stde::product_type::size<T>::value);
      BOOST_TEST(0 == stde::product_type::get<0>(p));
      BOOST_TEST(1 == stde::product_type::get<1>(p));
      BOOST_TEST(2 == stde::product_type::get<2>(p));
      //auto x = stde::product_type::get<3>(p); // COMPILE FAIL AS REQUIRED
  }
  {
    static_assert(std::is_same<int&&, decltype(stde::product_type::get<0>(stde::make_array(0,1)))>::value, "Hrr");
    BOOST_TEST(0 == stde::product_type::get<0>(stde::make_array(0,1)));
  }
  {
    using T = std::array<int,3>;
    const T arr  = { {0,1,2} };
    BOOST_TEST(0 == stde::product_type::apply(f, arr));
  }
  {
      using T = std::array<int,3>;
      const T arr  = { {0,1,2} };
      BOOST_TEST(std::make_tuple(0,1,2) == stde::product_type::to_tuple(arr));
  }
  {
      using T = std::array<std::unique_ptr<int>,1>;
      T arr  = { {std::unique_ptr<int>(new int(1))} };
      BOOST_TEST(arr[0]);
      auto res = stde::product_type::to_tuple(std::move(arr));
      BOOST_TEST(! arr[0]);
      BOOST_TEST(stde::product_type::get<0>(res));
  }
  {
    using T = std::array<int,2>;
    const T p  = { {0,1} };
    using U = std::array<int,2>;
    const U q  = { {2,3} };
    BOOST_TEST(std::make_tuple(0,1,2,3) == stde::product_type::cat(p, q));
  }
#if __cplusplus >= 201402L and defined JASEL_SUPPORT_SWAPPABLE
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::array<int,2>;
    U q  = { {2,3} };
    stde::swappable::swap(p,q);
    BOOST_TEST(0 == stde::product_type::get<0>(q));
    BOOST_TEST(1 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
#endif
  {
    using T = std::array<int,3>;
    const T arr  = { {0,1,2} };
    auto x = stde::product_type::make_from_product_type<X>(arr);
    BOOST_TEST(0 == x.i);
    BOOST_TEST(1 == x.j);
    BOOST_TEST(2 == x.k);
  }
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::array<short,2>;
    U q  = { {2,3} };
    stde::product_type::copy(q,p);
    BOOST_TEST(2 == stde::product_type::get<0>(q));
    BOOST_TEST(3 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
#if 0
  // Fails as std::array doen't defines the assignment from another convertible array
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::array<short,2>;
    U q  = { {2,3} };
    p = q;
    BOOST_TEST(2 == stde::product_type::get<0>(q));
    BOOST_TEST(3 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
#endif
  {
    using T = std::array<std::unique_ptr<int>,1>;
    T p  = { {std::unique_ptr<int>(new int(1))} };
    T q;
    stde::product_type::copy(std::move(p), q);
    BOOST_TEST(! p[0]);
    BOOST_TEST(stde::product_type::get<0>(q));
  }
  // COMPILE FAILS AS EXPECTED
#if 0
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::array<int,1>;
    U q  = { {2} };
    stde::product_type::copy(q,p);
  }
#endif
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const U q = {2,3};
#else
    const U q = std::make_tuple(2,3);
#endif
    stde::product_type::copy(q,p);
    BOOST_TEST(2 == stde::product_type::get<0>(q));
    BOOST_TEST(3 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    using U = std::tuple<int,int>;
#if defined JASEL_SUPPORT_TUPLE
    const U q  = {2,3};
#else
    const U q = std::make_tuple(2,3);
#endif
    stde::product_type::copy(q,p);
    BOOST_TEST(2 == stde::product_type::get<0>(q));
    BOOST_TEST(3 == stde::product_type::get<1>(q));
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
  {
    using T = std::array<int,2>;
    T p  = { {0,1} };
    stde::product_type::copy(std::make_tuple(2,3), p);
    BOOST_TEST(2 == stde::product_type::get<0>(p));
    BOOST_TEST(3 == stde::product_type::get<1>(p));
  }
  {
    using T = std::array<int,2>;
    T p  = { {1,2} };
    int r=0 ;
    stde::product_type::for_each(p, [&r](auto x) {r+=x;});
    BOOST_TEST(3 == r);
  }
  {
    using T = std::array<int,2>;
    T p  = { {1,2} };
    stde::product_type::for_each(p,[](auto& v) {std::cout << v << "\n";});
  }
#if 1
  {
    auto to_string = [](auto x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    };

    using T = std::array<int,2>;
    T p  = { {0,1} };
    auto res = stde::product_type::transform(p, to_string);
    static_assert(
        std::is_same<std::array<std::string, 2>, decltype(res)>::value,
        "");

    BOOST_TEST(stde::product_type::transform(p, to_string)
      ==
        std::make_array(std::string("0"), "1")
    );
  }
#endif
  {
    auto to_string = [](auto x) {
        std::ostringstream ss;
        ss << x;
        return ss.str();
    };
    auto f = [=](std::string s, auto element) {
        return "f(" + s + ", " + to_string(element) + ")";
    };
    // with an initial state
    using T = std::array<int,4>;
    T p  = { {2,3,4,5} };

    BOOST_TEST(
        stde::product_type::fold_left(p, "1", f)
            ==
        "f(f(f(f(1, 2), 3), 4), 5)"
    );
    // without initial state
    using U = std::array<std::string, 5>;
    U q  = { {"1", "2", "3", "4", "5"} };
    BOOST_TEST(
        stde::product_type::fold_left(q, f)
            ==
        "f(f(f(f(1, 2), 3), 4), 5)"
    );
  }
  return ::boost::report_errors();
}
#else
int main() { return 0; }
#endif
