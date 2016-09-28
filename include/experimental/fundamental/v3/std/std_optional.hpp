// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_OPTIONAL_HPP
#define JASEL_FUNDAMENTAL_V3_OPTIONAL_HPP

#include <optional.hpp>

#include <experimental/make.hpp>
#include <experimental/nullable.hpp>
#include <experimental/meta.hpp>

#if defined JASEL_FUNDAMENTAL_EXTENDED
#include <experimental/fundamental/v2/possible_valued/mcd/pointer_like_mcd.hpp>
#include <experimental/fundamental/v2/functor/mcd/possible_valued_mcd.hpp>
#include <experimental/fundamental/v2/monad/mcd/possible_valued_mcd.hpp>
#endif
namespace std
{
namespace experimental
{

  // Callable specialization
  template <>
  struct optional<_t>  : meta::quote<optional> {};

#if defined JASEL_FUNDAMENTAL_EXTENDED
  template <class T>
  struct possible_value::tag<optional<T>> : meta::id<pointer_like> {};

  template <class T>
  struct functor::tag<optional<T>> : meta::id<possible_value> {};

  template <class T>
  struct monad::tag<optional<T>> : meta::id<possible_value> {};
#endif
  // type_constructor customization
  template <class T>
  struct type_constructor<optional<T>> : meta::id<optional<_t>> {};

#if defined JASEL_FUNDAMENTAL_EXTENDED
  template <class T>
  struct value_type<optional<T>> : meta::id<T> { };

#endif

  template <class T>
  struct nullable_traits<optional<T>> : nullable_tag {
    template <class U>
    struct none_type : meta::id<nullopt_t> { };

    static constexpr
    nullopt_t none() { return nullopt; }

    template <class U>
    static constexpr
    bool has_value(optional<U> const& x) noexcept { return bool(x); }

    template <class U>
    static constexpr
    auto deref(optional<U> const& x)
      JASEL_DECLTYPE_RETURN (
          *x
      )
    template <class U>
    static constexpr
    auto deref(optional<U>& x)
      JASEL_DECLTYPE_RETURN (
          *x
      )
  };

//}
}
}

#endif // header
