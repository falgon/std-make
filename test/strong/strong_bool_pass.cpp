// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_bool.hpp>

#include <experimental/strong_bool.hpp>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

using EngineStarted = stdex::strong_bool<class EngineStartedTag>;
using CrewReady     = stdex::strong_bool<class CrewReadyTag>;

void set_status(EngineStarted started, CrewReady ready){
}

struct X {
    operator bool() { return true; }
};

struct Y {
    explicit operator bool() { return true; }
};


int main()
{

  {
      //EngineStarted es1 = true;     // error - explicit required

      //EngineStarted es2 {nullptr};  // error void* deleted

      //EngineStarted es3 {1};        // error int deleted
      //short s;
      //EngineStarted es4 {s};        // error int deleted

      //EngineStarted es5 {1.0};      // error double deleted

      EngineStarted es {true};        // ok
      CrewReady     cr {false};       // ok

      //EngineStarted es6 =true;       // error
      //EngineStarted es6 =cr;       // error
      //EngineStarted es7 {cr};       // fixme: how can we forbid this providing explicit conversion to bool? clang compile this while gcc don't.

      //bool b = cr;                  // error - implicit conversion required

      bool ans = es && !cr;           // ok - explicit conversion required by the use of &&
      BOOST_TEST(ans);
      if (cr)                         // ok - explicit conversion required by the use cr in a if
          std::cout << "True\n";

      //set_status(true, true);       // error - implicit conversion required
      set_status(EngineStarted{true}, CrewReady{true});        // function call
  }
  {
      //EngineStarted es {Y{}};        // error, explicit conversion
      EngineStarted es {X{}};        // ok, implicit conversion from UDT
  }
  {
      EngineStarted es1 {true};
      EngineStarted es2 {false};
      CrewReady     cr {false};
      BOOST_TEST(es2<es1);
      BOOST_TEST(es2!=es1);
      //BOOST_TEST(cr<es1); // error
  }

  return ::boost::report_errors();
}

