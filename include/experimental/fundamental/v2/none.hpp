// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2015 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_NONE_HPP
#define JASEL_FUNDAMENTAL_V2_NONE_HPP

#include <experimental/fundamental/v2/holder.hpp>
#include <experimental/meta/v1/id.hpp>
#include <experimental/fundamental/v2/type_constructor.hpp>
#include <experimental/fundamental/v2/none_type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
  template <class T>
  struct nullable_traits;

  template <class T>
  struct nullable_traits<T*>
  {
    static constexpr
    nullptr_t none() { return nullptr; }
  };

  struct none_t {};
  constexpr bool operator==(none_t, none_t) { return true; }
  constexpr bool operator!=(none_t, none_t) { return false; }
  constexpr bool operator<(none_t, none_t) { return false; }
  constexpr bool operator<=(none_t, none_t) { return true; }
  constexpr bool operator>(none_t, none_t) { return false; }
  constexpr bool operator>=(none_t, none_t) { return true; }

  constexpr none_t none() { return none_t{}; }

  template <class TC>
  constexpr auto none()
    -> decltype(nullable_traits<TC>::none())
  {
    //return none_custom(meta::id<TC>{});
    return nullable_traits<TC>::none();
  }

  template <template <class ...> class TC>
  constexpr auto none()
  ->  decltype(none<type_constructor_t<meta::quote<TC>>>())
  {
    return none<type_constructor_t<meta::quote<TC>>>();
  }

}
namespace meta
{
inline namespace v1
{
  //nullptr_t none_custom(id<add_pointer<_t>>) { return nullptr; }
}
}

}
}

#endif // header
