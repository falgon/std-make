// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/functor.hpp>
// <experimental/expected.hpp>


#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
#if __cplusplus >= 201402L
  namespace stde = std::experimental;

  static_assert(stde::is_type_constructible_v<stde::expected<int>>, "ERROR");
  static_assert(std::is_base_of<stde::functor::tag, stde::functor::traits<stde::expected<stde::_t>>> ::value, "ERROR");
  static_assert(stde::is_functor<stde::expected<stde::_t>>::value, "ERROR");

  {
    stde::expected<int> x = stde::make_unexpected(1);
    BOOST_TEST(! x);

    stde::expected<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(! y);
  }
  {
    int v=1;
    stde::expected<int> x = stde::make_expected(v);
    BOOST_TEST(*x == 1);
    //stde::expected<int> y = x.map(twice);
    stde::expected<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
    x = 2;
    BOOST_TEST(*x == 2);
  }
  // fixme should transform be const? How the move can be done then? Or should expected don't do a move?
#if 0
  {
    int v=1;
    const stde::expected<int> x = stde::make_expected(v);
    BOOST_TEST(*x == 1);

    stde::expected<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
  }
#endif
#endif

  return ::boost::report_errors();
}
