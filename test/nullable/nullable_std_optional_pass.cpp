// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/nullable.hpp>
// <experimental/optional.hpp>

#if defined JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
#warning JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
int main()
{
}
#else

#define JASEL_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
#include <experimental/optional.hpp>

#include <boost/detail/lightweight_test.hpp>

std::experimental::nullopt_t null() { return std::experimental::nullopt;}

struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int twice(int i) {
  return 2*i;
}

std::experimental::optional<double> inverse(double x) {
  if (x==0) return std::experimental::nullopt;
  return 1/x;
}

int main()
{
  namespace stde = std::experimental;
  static_assert(not stde::is_nullable<stde::none_t>::value, "ERROR");
  static_assert(stde::is_nullable<stde::optional<int>>::value, "ERROR");
  static_assert(stde::meta::is_callable<stde::optional<stde::_t>(int)>::value, "ERROR");
  static_assert(std::is_same<stde::value_type_t<stde::optional<int&>>, int&>::value, "ERROR");

  {
    auto n = stde::none_t{};
    stde::optional<int> x(n);
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
    BOOST_TEST(x == stde::none());
  }
  {
    stde::optional<stde::optional<int>> x { stde::none() };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
  {
    stde::optional<stde::optional<int>> x { stde::optional<int>(stde::none()) };
    BOOST_TEST(x);
    BOOST_TEST(has_value(x));
  }

# if !defined __clang__ && defined __GNUC__ // NOTE: GNUC is also defined for Clang
#   if (__GNUC__ < 6)
  {
    stde::optional<stde::optional<int>> x { { stde::none() } };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
#endif
#endif

#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { stde::none() } };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
#endif


#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { null() } };
    BOOST_TEST( ! x);
    BOOST_TEST( ! has_value(x));
  }
  {
    stde::optional<stde::optional<int>> x { { stde::nullopt } };
    BOOST_TEST( ! x);
    BOOST_TEST( ! has_value(x));
  }
#endif
  {
    stde::optional<stde::optional<int>> x { stde::none<stde::optional>() };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }

#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { stde::none<stde::optional>() } };
    BOOST_TEST( ! x);
    BOOST_TEST( ! has_value(x));
  }
#endif
  {
    stde::optional<int> x = stde::none<stde::optional>();
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
    stde::optional<int> y = stde::fmap(twice, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=1;
    stde::optional<int> x = stde::make_optional(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(value(x) == 1);
    stde::optional<int> y = stde::fmap(twice, x);
    BOOST_TEST(stde::value(y) == 2);
    BOOST_TEST(x != stde::none());

  }
  {
    int v=0;
    stde::optional<int> x = stde::make_optional(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(value(x) == 0);
  }
#if 0
  {
    short v=0;
    stde::optional<int> x = stde::make_optional<int>(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(value(x) == 0);
  }
#endif
  static_assert(std::is_constructible<A,int, int>::value, "a");
  static_assert(std::is_constructible<A>::value, "b");

  static_assert(std::is_constructible<stde::optional<A>, stde::in_place_t,int, int>::value, "a");
  static_assert(std::is_constructible<stde::optional<A>, stde::in_place_t>::value, "b");

#if 0
  {
    int v=1;
    stde::optional<A> x = stde::make_optional<A>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
    BOOST_TEST(value(x).v == 2);
  }
  {
    stde::optional<int> x = stde::make_optional<int>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
    BOOST_TEST(value(x) == 0);
  }
#endif
  {
    stde::optional<int> o((stde::in_place));
    BOOST_TEST(o);
    BOOST_TEST(has_value(o));
  }
#if 0
  {
    stde::optional<int> o((stde::in_place));
    stde::optional<int> x = stde::make_optional<int>();
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
#endif
  {
    int v=0;
    stde::optional<int&> x = stde::make_optional(std::ref(v));
    BOOST_TEST(&v == &x.value());
    BOOST_TEST(&v == std::addressof(x.value()));
    BOOST_TEST(&v == std::addressof(value(x)));
  }

  return ::boost::report_errors();
}
#endif
