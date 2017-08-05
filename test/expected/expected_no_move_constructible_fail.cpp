//! \file test_expected.cpp

// Copyright Pierre Talbot 2013.
// Copyright Vicente J. Botet Escriba 2013,2014.

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
//(See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stde = std::experimental;
template <class T>
using expected_sc = stde::expected<T, std::error_code>;

struct NoDefaultConstructible
{
  NoDefaultConstructible() = delete;
  NoDefaultConstructible(int i)  {};
};

struct NoCopyConstructible
{
  NoCopyConstructible()  {};
  NoCopyConstructible(NoCopyConstructible const&) = delete;
  NoCopyConstructible(NoCopyConstructible &&) = default;
};
struct NoMoveConstructible
{
  NoMoveConstructible()  {};
  NoMoveConstructible(NoMoveConstructible const&) = default;
  NoMoveConstructible(NoMoveConstructible &&) = delete;
  NoMoveConstructible& operator=(NoMoveConstructible const&) = default;
  NoMoveConstructible& operator=(NoMoveConstructible &&) = delete;
};


template<typename T, bool> struct base {
  base() = default;
  base(T v) : v(v) {};
  base(base const&) = default;
  base(base&&) = default;
  T v;
};

template<typename T> struct base<T, false> {
   base() = default;
   base(T v) : v(v) {};
   base(base const&) = default;
   base(base&&) = delete;
   T v;
};

template<typename T>
class optional : public base<T, std::is_move_constructible<T>::value> {
  using base_type = base<T, std::is_move_constructible<T>::value>;
public:
  optional() = default;
  optional(T v) : base_type(v) {};
  optional(const optional&) = default;
  optional(optional &&) = default;
};

int main()
{
  {
    expected_sc<NoDefaultConstructible> x; // FAILS as expected
  }
#if 0
  {
    NoCopyConstructible ncc;
    expected_sc<NoCopyConstructible> x{ncc}; // FAILS as expected
  }
  {
    NoCopyConstructible ncc;
    expected_sc<NoCopyConstructible> x{std::move(ncc)};
    expected_sc<NoCopyConstructible> y{x}; // FAILS as expected

  }
  {
    NoMoveConstructible nmc;
    NoMoveConstructible nmc2 = std::move(nmc); // FAILS as expected

    // fixme
    expected_sc<NoMoveConstructible> x;
    expected_sc<NoMoveConstructible> y =  std::move(x); // DOESN'T FAIL
  }
#endif
  {
    optional<NoMoveConstructible> x;
    optional<NoMoveConstructible> y {std::move(x)}; //FAILS as expected
  }
  static_assert(! std::is_default_constructible<NoDefaultConstructible>::value, "");
  static_assert(! std::is_default_constructible<expected_sc<NoDefaultConstructible>>::value, "");

  static_assert(! std::is_copy_constructible<NoCopyConstructible>::value, "");
  static_assert(! std::is_constructible<expected_sc<NoCopyConstructible>, NoCopyConstructible const& >::value, "");
  static_assert(! std::is_constructible<expected_sc<NoCopyConstructible>, expected_sc<NoCopyConstructible> const& >::value, "");
  static_assert(! std::is_copy_constructible<expected_sc<NoCopyConstructible>>::value, "");

  {
    // fixme
    expected_sc<NoMoveConstructible> x;
    expected_sc<NoMoveConstructible> y =  std::move(x); // DOESN'T FAIL as copy is used instead
  }
  {
    // fixme
    expected_sc<NoMoveConstructible> x;
    expected_sc<NoMoveConstructible> y {expected_sc<NoMoveConstructible>{}}; // DOESN'T FAIL as copy is used instead
  }
  static_assert(! std::is_move_constructible<NoMoveConstructible>::value, "");
  static_assert( std::is_constructible<expected_sc<NoMoveConstructible>, NoMoveConstructible && >::value, "");
  static_assert( std::is_move_constructible<expected_sc<NoMoveConstructible>>::value, "");


  return ::boost::report_errors();
}
