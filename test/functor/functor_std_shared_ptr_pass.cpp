// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/functor.hpp>
// <experimental/memory.hpp>

#include <experimental/memory.hpp>
#include <experimental/functor.hpp>
#include <experimental/nullable.hpp>

#include <boost/detail/lightweight_test.hpp>

int twice(int i) {
  return 2*i;
}

int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_nullable<std::shared_ptr<int>>::value, "ERROR");
  static_assert(stde::is_type_constructible<std::shared_ptr<int>>::value, "ERROR");
  static_assert(stde::is_functor<std::shared_ptr<stde::_t>>::value, "ERROR");

  {
    std::shared_ptr<int> x = stde::none<std::shared_ptr>();
    BOOST_TEST(! x);
    BOOST_TEST(! stde::has_value(x));
    BOOST_TEST(x == stde::none());
    BOOST_TEST(stde::none() == x);
    BOOST_TEST(x == stde::none<std::shared_ptr<stde::_t>>());

    std::shared_ptr<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=1;
    std::shared_ptr<int> x = std::make_shared<int>(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(x != stde::none());
    std::shared_ptr<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
    x = std::make_shared<int>(2);
    BOOST_TEST(*x == 2);
  }
  {
    int v=1;
    const std::shared_ptr<int> x = std::make_shared<int>(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(x != stde::none());
    BOOST_TEST(*x == 1);

    std::shared_ptr<int> y = stde::functor::transform(x, twice);
    BOOST_TEST(*y == 2);
  }

  return ::boost::report_errors();
}
