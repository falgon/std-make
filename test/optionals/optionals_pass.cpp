// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/optionals.hpp>

#include <experimental/optionals.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

struct A
{
  A() :
      v(4)
  {
  }
  int v;
};
struct B
{
};

void f1(stdex::optionals<A, B> const& opt_a_b)
{
  BOOST_TEST(! (opt_a_b.template has_value<0>()));
  stdex::optional_ref<const A> opt_a = stdex::get < 0 > (opt_a_b);
  BOOST_TEST(!opt_a);
}

void f2(stdex::optionals<A, B> const& opt_a_b)
{
  stdex::optional_ref<const A> opt_a = stdex::get < 0 > (opt_a_b);
  BOOST_TEST(opt_a->v == 4);
}

int main()
{

  {
    stdex::optionals<A, B> opt_a_b;
    BOOST_TEST(! (opt_a_b.template has_value<0>()));
  }
  {
    stdex::optionals<A, B> opt_a_b;
    stdex::optional_ref<A> opt_a = stdex::get < 0 > (opt_a_b);
    BOOST_TEST(!opt_a);
    stdex::optional_ref<B> opt_b = stdex::get < 1 > (opt_a_b);
    BOOST_TEST(!opt_b);
  }
  {
    stdex::optionals<A, B> opt_a_b;
    stdex::optional_ref<A> opt_a = stdex::get < 0 > (opt_a_b);
    opt_a = A {};
    BOOST_TEST(opt_a);
    BOOST_TEST(opt_a->v == 4);

    opt_a = stdex::nullopt;
    BOOST_TEST(!opt_a);
    BOOST_TEST(! (opt_a_b.template has_value<0>()));
  }

  {
    stdex::optionals<A, B> opts1;
    stdex::optionals<A, B> opts2;
    swap(opts1, opts2);
  }
  return ::boost::report_errors();
}

