//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201402L

#include <experimental/fundamental/v3/functional/overload.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <boost/detail/lightweight_test.hpp>

struct X {
  X& f(char) {
    std::cout << "X::f" << std::endl;
    return *this;
  }
  int g(char) {
    return 33;
  }
  bool operator==(X const&) { return true; }
};
struct Y {
};

struct final_function_object_const final {
  void operator ()(int ) const { }
};
struct final_function_object_const2 final {
  void operator ()(Y ) const { }
};
struct final_function_object_non_const final {
  void operator ()(int ) { }
};

struct YYYY {};
struct Foo final
{
  void operator()(int i) const { std::cout << "Foo int: " << i << "\n"; }
};
struct Bar final
{
  void operator()(double) const { std::cout << "Bar int: " << "\n"; }
};


struct function_with_state {
  void operator ()(int ) { invoked = true; }
  bool invoked = false;
};
struct function_without_state {
  constexpr void operator ()(int ) const noexcept { }
};
struct function_without_state_x {
  constexpr void operator ()(float ) const noexcept { }
};
struct convertible_to_function_object {
  operator function_without_state() const {
    return function_without_state{};
  }
};

struct function_without_state_throw {
  JASEL_NORETURN void operator ()(int ) const { throw 1;}
};
struct function_without_state2 {
  constexpr int operator ()(int arg) const noexcept { return arg;}
};
struct function_without_state3 {
  int operator ()(int arg) const noexcept { return arg;}
};

struct function_with_rvalue_ref_q {
  int operator ()(int ) && { return 1; }
  int operator ()(int ) const && { return 2; }
  int operator ()(int ) & { return 3;}
  int operator ()(int ) const & { return 4;}
};
struct final_function_with_rvalue_ref_q final {
  int operator ()(int ) && { return 1; }
  int operator ()(int ) const && { return 2; }
  int operator ()(int ) & { return 3;}
  int operator ()(int ) const & { return 4;}
};
struct function_with_lvalue_ref_q {
  void operator ()(int ) & { }
};
struct function_with_no_ref_q {
  void operator ()(int ) { }
};
struct function_with_ref_q {
  int operator ()(int ) & { return 1; }
  int operator ()(int ) && { return 2; }
};

struct function_with_cv_q {
  int operator ()(int ) { return 0; }
  int operator ()(int ) const { return 1; }
  int operator ()(int ) volatile { return 2; }
  int operator ()(int ) const volatile { return 3; }
};


int nonMember( float ) {
  std::cout << "int(float)" << std::endl;
  return 42;
}
int nonMember_throw( float ) {
  throw 1;;
}
struct XXXX {};
void freefnx(XXXX ) {  std::cout <<"X\n"; }


namespace cppljevans {
  //Purpose:
  //  See if can multi-inherit classes
  //  with member function with same
  //  name but different signature.
  //
  //Result:
  //  Works.
  //
  //===========
  template<int I>
  struct udt_i
  /**@brief
   *  User Defined Type.
   */
  {
        friend
      std::ostream&
    operator<<
      ( std::ostream& os
      , udt_i const&
      )
      { return os<<"udt_i<"<<I<<">";
      }
  };
  template<int I>
  struct functor_i
  /**@brief
   *  User Defined Functor.
   */
  {
    using arg_t=udt_i<I>;
    void operator()(arg_t const&a)
    { std::cout<<"functor_i<"<<I<<">::operator()("<<a<<" const&).\n";
    }
  };
  template<typename Functor>
  struct forwarder
  /**@brief
   *  this->operator() forwards to Functor::operator()
   */
  {
    Functor functor_v;
    forwarder(Functor const& functor_a)
      : functor_v(functor_a)
    {}
    using arg_t=typename Functor::arg_t;
    void operator()(arg_t const&arg_v)
    { functor_v(arg_v);
    }
  };
  #ifdef __clang__
  template<typename... Functors>
  struct overloader
  /**@brief
   *  "Mixin" all the Functors::operator()'s
   *  into this class.
   */
  : public forwarder<Functors>...
  {
    overloader(Functors&&... functors_a)
    : forwarder<Functors>(functors_a)...
    {}
  };
  #else
  template<typename... Functors>
  struct overloader
  /**@brief
   *  "Mixin" all the Functors::operator()'s
   *  into this class.
   */
    ;
  template<>
  struct overloader<>
  {
    void operator()()
    {
    }
  };
  template<typename Functor0, typename... Functors>
  struct overloader<Functor0, Functors...>
  : public forwarder<Functor0>, overloader<Functors...>
  {
    overloader(Functor0&& functor0, Functors&&... functors_a)
    : forwarder<Functor0>(std::forward<Functor0>(functor0))
    , overloader<Functors...>(std::forward<Functors>(functors_a)...)
    {}
    using forwarder<Functor0>::operator();
    using overloader<Functors...>::operator();
  };
#endif
}


int main()
{
  {
    using namespace cppljevans;

    overloader
    < functor_i<0>
    , functor_i<1>
    , functor_i<2>
    >
  ovldr
    ( functor_i<0>{}
    , functor_i<1>{}
    , functor_i<2>{}
    )
    ;
  ovldr(udt_i<0>{});
  ovldr(udt_i<1>{});
  ovldr(udt_i<2>{});
  }
  using namespace std::experimental;
//  {
//    auto f = overload(
//        [](int i, double d) {},
//        [](std::string str, int i) {
//        },
//        [](double d, int i) {
//          BOOST_TEST(false);
//        }
//    );
//    f(1,1); // ambiguous call compile fails
//  }
  {
    final_function_object_const foo;

    auto f = overload(foo);
    f(1);

  }
  {
    final_function_object_non_const foo;

    auto f = overload(foo,  // foo should be copied
        [](std::string ) {
      BOOST_TEST(false);
      return 1;
    }
    );
    f(1);

  }
  {
    final_function_object_const foo;
    final_function_object_const2 bar;

    auto f = overload(foo, bar);
    f(1);

  }
  {
    final_function_object_const foo;

    auto f = overload<int>(foo);
    f(1);

  }
  {
    final_function_object_non_const foo;

    auto f = overload<int>(foo,  // foo should be copied
        [](std::string ) {
      BOOST_TEST(false);
      return 1;
    }
    );
    f(1);

  }
  {
    final_function_object_non_const foo;

    overload<int>(foo,
        [](std::string ) {
      BOOST_TEST(false);
      return 1;
    }
    )(1);
  }
#if 0
  //../include/yafpl/v1/functional/overload.hpp:29:18: error: no member named 'operator()' in 'convertible_to_function_object'
  //        using F::operator();
                ~~~^

  {
    convertible_to_function_object foo;

    auto f = overload<int>(foo);
    f(1);

  }
#endif
  {
    function_without_state foo;

    auto f = overload<int>(foo,  // foo should be copied
        [](std::string ) {
      BOOST_TEST(false);
      return 1;
    }
    //,    nonMember
    );
    f(1);

  }
  {
    function_without_state foo;

    auto f = overload(foo,  // foo should be copied
        [](std::string ) {
      BOOST_TEST(false);
    },
    nonMember
    );
    f(1);

  }
  {
    function_without_state foo;

    auto f = overload(foo,  // foo should be copied
        [](std::string ) {
      BOOST_TEST(false);
    },
    freefnx
    );
    f(XXXX());

  }
  {
    auto ol = overload(
        [](int i) { printf("int: %d\n", i); },
        [](const char* s) { printf("cc*: %s\n", s); },
        [](double d) { printf("double: %f\n", d); },
        &freefnx
      );
    XXXX x;
    ol(x);
  }
  {
    function_without_state foo;

    auto f = overload(std::move(foo),  // foo should be copied
        [](std::string ) {
      BOOST_TEST(false);
    },
    [](...)
    {
      std::cout << "int(...)" << std::endl;
    }
    );
    f(1);

  }
  {
    function_with_state foo;

    BOOST_TEST(! foo.invoked);

    overload(std::ref(foo),
        [](std::string ) {
      BOOST_TEST(false);
    })(1);

    BOOST_TEST(foo.invoked);
  }
  {
    overload(function_with_state{},
        [](std::string ) {
      BOOST_TEST(false);
    })(1);
  }
  {
    function_with_state foo;

    BOOST_TEST(! foo.invoked);

    overload(foo,
        [](std::string ) {
      BOOST_TEST(false);
    })(1);

    BOOST_TEST(! foo.invoked);
  }
  {
    function_with_state foo;

    BOOST_TEST(! foo.invoked);

    overload(std::ref(foo), [](std::string ) { })("aaa");

    BOOST_TEST(! foo.invoked);
  }
  {
    auto f = overload(
        [](int )
        {
          std::cout << "int(int)" << std::endl;
          return 1;
        },
        [](auto const&)
        {
          std::cout << "int(auto const&)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(std::vector<int>{}) == -1);
  }
  {
    auto f = overload<int>(
        [](int )
        {
          std::cout << "int(int)" << std::endl;
          return 1;
        },
        [](auto const&)
        {
          std::cout << "int(auto const&)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(std::vector<int>{}) == -1);
  }
  {
    auto f = overload(
        [](std::vector<int>&& )
        {
          std::cout << "int(std::vector<int>&&)" << std::endl;
          return 1;
        },
        [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(std::vector<int>{}) == 1);
  }
  {
    auto f = overload<int>(
        [](std::vector<int>&& )
        {
          std::cout << "int(std::vector<int>&&)" << std::endl;
          return 1;
        },
        [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(std::vector<int>{}) == 1);
  }
  {
    auto f = overload(
        [](int i)
        {
          std::cout << "int(int)" << std::endl;
          return i;
        },
        [](std::string && i)
        {
          std::cout << "string(string&&)" << std::endl;
          return i;
        },
        [](auto)
        {
          std::cout << "..." << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(1) == 1);
    BOOST_TEST(f(std::string("1")) == "1");
    BOOST_TEST_EQ(f(X{}), -1);
  }
  {
    auto f = overload<int>(
        [](int i)
        {
          std::cout << "int(int)" << std::endl;
          return i;
        },
        [](std::string && i)
        {
          std::cout << "string(string&&)" << std::endl;
          return i;
        },
        [](auto)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(1) == 1);
    BOOST_TEST(f(std::string("1")) == "1");
    BOOST_TEST_EQ(f(X{}), -1);
  }
  {
    auto f = overload(
        nonMember
#if defined __GNUC__ and ! defined __clang__
        , &X::f
#endif
        , [](auto)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
    BOOST_TEST(f(1.0f) == 42);
#if defined __GNUC__ and ! defined __clang__
    X x;
    BOOST_TEST(f(x, 'c') == x);
#endif
    BOOST_TEST_EQ(f(Y{}), -1);
  }
  {
    auto f = overload<X&>(
         &X::f
    );
    X x;
    BOOST_TEST(f(x, 'c') == x);
  }
  {
    auto f = overload<int>(
         &X::g
    );
    X x;
    BOOST_TEST(f(x, 'c') == 33);
  }
  {
    auto f = overload(
         &X::g
    );
    X x;
    BOOST_TEST(f(x, 'c') == 33);
  }
  {
    auto f1 = overload(
        nonMember
    );
    BOOST_TEST(f1(1.0) == 42);
  }
  {
    auto f1 = overload<int>(
        &nonMember
    );
    BOOST_TEST(f1(1.0) == 42);
  }
  {
    auto f1 = overload(
        //overload/include_pass.cpp:429:12: error: attempt to use a deleted function
        &nonMember
        , &X::g
    );
    X x;
    BOOST_TEST(f1(x, 'c') == 33);
#if defined __GNUC__ and ! defined __clang__
    BOOST_TEST(f1(1.0) == 42);
#endif
  }
  {
    auto f = overload<int>(
        nonMember
        //overload/include_pass.cpp:429:12: error: attempt to use a deleted function
        , &X::g
        , [](...)
        {
          std::cout << "int(...)" << std::endl;
          return -1;
        }
    );
#if defined __GNUC__ and ! defined __clang__
    BOOST_TEST(f(1.0) == 42);
#endif
    X x;
    BOOST_TEST(f(x, 'c') == 33);
#if defined __GNUC__ and ! defined __clang__
    BOOST_TEST(f(Y{})== -1);
#endif
  }
  {
    constexpr auto f = overload(function_without_state{}
#if defined __GNUC__ and ! defined __clang__
        , [](std::string str) {
      return 1;
    }
#endif
    ,    nonMember
    );
    static_assert(noexcept(f(1)), "");
    f(1);

  }
  {
    constexpr auto f = overload(function_without_state2{}
#if defined __GNUC__ and ! defined __clang__
        , [](std::string str) {
      return 1;
    }
#endif
    );
    constexpr auto x = f(1);
    static_assert(1 == x, "");

  }
  {
    static_assert(noexcept(function_without_state3{}(1)), "");

    constexpr auto f = overload(function_without_state3{});
    static_assert(noexcept(f(1)), "");
#if 0
    constexpr auto x = f(1); // COMPILE ERROR as expected
    //overload/include_pass.cpp:409:25: erreur: call to non-constexpr function ‘int function_without_state3::operator()(int) const’
    //     constexpr auto x = f(1);
    static_assert(1 == x, "");
#endif
    auto y = f(1);
    BOOST_TEST(1 == y);

  }
  {

    static_assert(! noexcept(nonMember_throw(1.0)), "");
    constexpr auto f = overload(&nonMember_throw);
    static_assert(! noexcept(f(1.0)), "");

  }
  {
    //overload/include_pass.cpp:468:9: note: non-literal type '(lambda at overload/include_pass.cpp:468:9)' cannot be used in a constant expression
    constexpr auto f = overload<int>(function_without_state2{}
#if defined __GNUC__ and ! defined __clang__
        , [](std::string str) {
      return 1;
    }
#endif
    );
    f(1);

  }
  { // overload noexcept
#if defined __GNUC__ and ! defined __clang__
    static_assert(noexcept(overload(function_without_state{},function_without_state_x{})), "");
#endif
    auto f = overload(function_without_state{},function_without_state_x{});
    f(1);
  }
  {
#if defined __GNUC__ and ! defined __clang__
    static_assert(noexcept(overload(nonMember)), "");
#endif

    auto f = overload(nonMember,
        [](std::string ) noexcept {
      return 1;
    }
    );
    auto y = f(1.0);
    BOOST_TEST(42 == y);

  }

  {
    auto f = overload(function_with_no_ref_q{});
    f(1);
  }
  {
    auto f = overload(function_with_lvalue_ref_q{});
    f(1);
  }
  {
    BOOST_TEST(1 ==function_with_rvalue_ref_q{}(1));

    BOOST_TEST(1 ==detail::wrap_call<function_with_rvalue_ref_q>(function_with_rvalue_ref_q{})(1));

    auto f = overload(function_with_rvalue_ref_q{});
    BOOST_TEST_EQ(3, f(1));

  }
  {
    BOOST_TEST(1 ==overload(function_with_rvalue_ref_q{})(1));
  }
  {
    auto f = overload(function_with_ref_q{});
    BOOST_TEST(1 ==f(1));
  }
  {
    detail::wrap_call<final_function_with_rvalue_ref_q>(final_function_with_rvalue_ref_q{})(1);
    BOOST_TEST(1 ==overload(final_function_with_rvalue_ref_q{})(1));
  }
  {
    auto f = overload(final_function_with_rvalue_ref_q{});
    BOOST_TEST(3 ==f(1));
  }
  {
    BOOST_TEST(2 == overload(function_with_ref_q{})(1));
  }

  {
    auto f = overload(function_with_cv_q{});
    BOOST_TEST(0 ==f(1));
  }
  {
    const auto f = overload(function_with_cv_q{});
    BOOST_TEST(1 ==f(1));
  }
  {
    volatile auto f = overload(function_with_cv_q{});
    BOOST_TEST(2 ==f(1));
  }
  {
    const volatile auto f = overload(function_with_cv_q{});
    BOOST_TEST(3 ==f(1));
  }
  {
    constexpr auto f = overload(function_with_cv_q{});
    BOOST_TEST(1 ==f(1));
  }
  {
    BOOST_TEST(0 ==overload(function_with_cv_q{})(1));
  }
#if 0
  // This couldn't work :(
  {
    const auto ol = std::experimental::overload(
          Foo()
          , Bar()
        );

    ol(1234);
  }
#endif
  return boost::report_errors();
}
#else
int main() { return 0; }
#endif


