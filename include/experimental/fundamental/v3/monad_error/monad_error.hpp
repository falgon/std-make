//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_MONAD_ERROR_MONAD_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_MONAD_ERROR_MONAD_ERROR_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////

#include <utility>
#include <type_traits>
#include <experimental/meta.hpp>
#include <experimental/type_constructible.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/monad.hpp>

//! Usage
//! Intendeed usage
//! <code>
//! E e;
//! auto x = monad_error::make_error<expected>(e);
//! //decltype(x) is unexpected<E>
//!
//! <end_code>
//! <code>
//!
//! A1 a1;
//! A2 a2;
//! auto x = monad_error::make_error<expected, E>(a1, a2);
//! //decltype(x) is unexpected<E>
//!
// The problem is that type_constructor<expected<T,E>> is expected<_t,E>
// And we are unable to deduce from expected that it has 2 parameters
// Currently we have that
// type_constructor<expected> is expected<_t>
// We are lucky as we have that Error has a default parameter
//!
//! Other usage
//! <code>
//! E e;
//! auto x = monad_error::make_error<expected<_t>>(e);
//! //decltype(x) is unexpected<E>
//!
//! <end_code>
//! <code>
//!
//! A1 a1;
//! A2 a2;
//! auto x = monad_error::make_error<expected<_t, E>>(a1, a2);
//! //decltype(x) is unexpected<E>
//!

//! <end_code>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class T>
    struct is_monad_error;

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
    constexpr bool is_monad_error_v = is_monad_error<T>::value;
#endif

namespace monad_error
{
  using namespace monad; // NOLINT google-build-using-namespace

  struct tag{};

  template <class M, class Enabler=void>
    struct traits
#if  ! defined JASEL_DOXYGEN_INVOKED
        : traits<M, meta::when<true>> {}
#endif
        ;

  // Default failing specialization
  template <typename MM, bool condition>
  struct traits<MM, meta::when<condition>>
  {
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

    // make_error<M>: E -> error_type<M,E> where E = error_type<M>
    template <class M, class ...Xs>
     constexpr
     auto make_error(Xs&& ...xs) = delete;

    // catch_error: [T] x (E->T) -> [T]
    template <class M, class F>
      static auto catch_error(M&& x, F&& y) = delete;
#endif
  };

  template <class M, class F>
  auto
  catch_error(M&& x, F&& f)
       JASEL_DECLTYPE_RETURN_NOEXCEPT(
          traits<type_constructor_t<decay_t<M>>>::catch_error(forward<M>(x), forward<F>(f))
       )

  // make_error overload: requires a type constructor, , deduce the error type associated to the type constructor
  //template <class TC, int = 0, int..., class ...Xs>
  template <class TC, class ...Xs>
  constexpr auto
  make_error(Xs&& ...xs)
    JASEL_DECLTYPE_RETURN_NOEXCEPT(
      traits<type_constructor_t<TC>>::template make_error<TC>(std::forward<Xs>(xs)...)
    )

  // make_error overload: requires a template class, deduce the error type associated to the template
  //template <template <class ...> class TC, int = 0, int..., class ...Xs>
  template <template <class ...> class TC, class ...Xs>
  constexpr auto
  make_error(Xs&& ...xs)
    JASEL_DECLTYPE_RETURN_NOEXCEPT(
      traits<type_constructor_t<meta::quote<TC>>>::template make_error<type_constructor_t<meta::quote<TC>>>(std::forward<Xs>(xs)...)
    )
}

  template <class T>
    struct is_monad_error
#if ! defined JASEL_DOXYGEN_INVOKED
        : is_base_of<monad_error::tag, monad_error::traits<T>> {}
#endif
        ;
  template <class T>
    struct is_monad_error<const T> : is_monad_error<T> {};
  template <class T>
    struct is_monad_error<volatile T> : is_monad_error<T> {};
  template <class T>
    struct is_monad_error<const volatile T> : is_monad_error<T> {};


}
}
}
#endif // header
