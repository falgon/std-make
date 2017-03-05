// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ENUMS_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ENUMS_HPP

#if __cplusplus >= 201402L

#include <experimental/fundamental/v3/strong/wrapper.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/optional.hpp>
#include <exception>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // By default it should checks if x is in the range of the underlying type
  // This function must be specialized for C++98 enums which have a more specific range
  template <class E, class Int>
  constexpr bool is_valid_enum(Int x) {return true;}

  // This function must be specialized for each enum
  template <class E>
  constexpr bool is_enumerator(E x) {return false;}

  template <class E, class Int>
  constexpr bool is_valid_enumerator(Int x) {return is_valid_enum<E>(x) && is_enumerator(E(x));}


  template <class E, class Int>
  constexpr E enum_cast(Int x) {return static_cast<E>(x);}

  struct bad_enum_cast : std::logic_error {
      bad_enum_cast() : std::logic_error("Bad enum cast") {}
  };

  template <class E, class Int>
  constexpr E to_valid_enum(Int x) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else throw bad_enum_cast();}

  template <class E, class Int>
  constexpr E to_valid_enum_or(Int x, E other) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else return other;}

  template <class E, class Int>
  constexpr E to_enumerator(Int x) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else throw bad_enum_cast();}

  template <class E, class Int>
  constexpr E to_enumerator_or(Int x, E other) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else return other;}

  template <class E, class Int>
  constexpr optional<E> try_to_enumerator(Int x) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else return nullopt;}

  template <class E, class Int>
  constexpr optional<E> try_to_valid_enum(Int x) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else return nullopt;}



  template <class E, class UT=typename underlying_type<E>::type>
  struct enum_wrapper : protected_wrapper<UT>
  {
      using base_type = protected_wrapper<UT>;
      using base_type::base_type;

      typedef E enum_type;

      constexpr enum_wrapper() noexcept = default;
      constexpr enum_wrapper(enum_type v) noexcept : base_type(UT(v)) {}

      enum_type enum_value() const noexcept { return enum_type(this->value); }
      enum_type get() const noexcept { return enum_type(this->value); }
      explicit operator enum_type() const noexcept { return enum_type(this->value); }

  };

  //! strong_enum ensures that static_cast is not allowed between two strong_enums
  //! Note that static_cast of two C++11 enum class is allowed as they are integral types.
  //! strong_enumt behaves almost as an C++11 enum class: ut.
  //! However there are some syntactical issues.
  //! In C++98, if you want scoped enums you should wrap the enum in a struct
  //! struct SE {
  //!     enum type { E0, E1, E2 };
  //! };
  //! typedef strong_enum<SE> SSE

  template <class E, class UT=typename underlying_type<E>::type>
  struct strong_enum final : enum_wrapper<E, UT>
  {
      using base_type = enum_wrapper<E, UT>;
      using base_type::enum_wrapper;

      friend constexpr bool operator==(strong_enum x, strong_enum y) noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_enum x, strong_enum y) noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_enum x, strong_enum y) noexcept { return x.value < y.value; }
      friend constexpr bool operator<=(strong_enum x, strong_enum y) noexcept { return x.value <= y.value; }
      friend constexpr bool operator>(strong_enum x, strong_enum y) noexcept { return x.value > y.value; }
      friend constexpr bool operator>=(strong_enum x, strong_enum y) noexcept { return x.value >= y.value; }

  };

  template <class E, class UT>
  struct underlying_type<strong_enum<E,UT>> { typedef UT type; };

  // safe_enum is a strong_enum that checks the validity of the values of the enum using is_valid_enum
  template <class E, class UT=typename underlying_type<E>::type>
  struct safe_enum final: enum_wrapper<E,UT>
  {
      using base_type = enum_wrapper<E,UT>;
      using base_type::base_type;
      using underlying_t = typename base_type::underlying_t;

      explicit safe_enum(underlying_t v): base_type(UT(to_valid_enum<E>(v))) {}
      //constexpr safe_enum() noexcept : safe_enum(0) {};

      friend constexpr bool operator==(safe_enum x, safe_enum y) noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(safe_enum x, safe_enum y) noexcept { return x.value != y.value; }
      friend constexpr bool operator<(safe_enum x, safe_enum y) noexcept { return x.value < y.value; }
      friend constexpr bool operator<=(safe_enum x, safe_enum y) noexcept { return x.value <= y.value; }
      friend constexpr bool operator>(safe_enum x, safe_enum y) noexcept { return x.value > y.value; }
      friend constexpr bool operator>=(safe_enum x, safe_enum y) noexcept { return x.value >= y.value; }

  };

  template <class E, class UT>
  struct underlying_type<safe_enum<E,UT>> { typedef UT type; };

  // ordinal_enum is a strong_enum that checks the validity of the values of the enum using is_enumerator
  // is_enumerator is specialized for ordinal enums
  template <class E, class UT=typename underlying_type<E>::type>
  struct ordinal_enum final: enum_wrapper<E,UT>
  {
      using base_type = enum_wrapper<E,UT>;
      using base_type::base_type;
      using underlying_t = typename  base_type::underlying_t;

      // ordinal_enum is not safe if the default value is not one of the enumerators
      constexpr ordinal_enum() noexcept = default;
      explicit ordinal_enum(underlying_t v): base_type(UT(to_enumerator<E>(v))) {}

      friend constexpr bool operator==(ordinal_enum x, ordinal_enum y) noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(ordinal_enum x, ordinal_enum y) noexcept { return x.value != y.value; }
      friend constexpr bool operator<(ordinal_enum x, ordinal_enum y) noexcept { return x.value < y.value; }
      friend constexpr bool operator<=(ordinal_enum x, ordinal_enum y) noexcept { return x.value <= y.value; }
      friend constexpr bool operator>(ordinal_enum x, ordinal_enum y) noexcept { return x.value > y.value; }
      friend constexpr bool operator>=(ordinal_enum x, ordinal_enum y) noexcept { return x.value >= y.value; }

  };
  template <class E, class UT>
  struct underlying_type<ordinal_enum<E,UT>> { typedef UT type; };


}
}
}

#endif
#endif // header
