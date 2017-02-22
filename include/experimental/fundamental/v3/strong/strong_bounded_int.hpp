// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP

#include <experimental/fundamental/v3/strong/wrapper.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>

#include <stdexcept>


namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  struct bad_bounded_int_cast : std::range_error {
    bad_bounded_int_cast() : std::range_error("Bad bounded int cast") {}
  };
  /**
  `strong_bounded_int` is a strongly type that wraps an integral type  @c UT and behaves like an  @c UT
  The main goal is to be able to define strong bounded integers that don't mix between them and that have a reduced set of values.
  The single conversion is to the underlying integer type.

  Note that this is not a safe int, it just forbids the conversions between
  different strong bounded integers types.

  Example
  <code>
  using X = strong_bounded_int<XTag, int, 0, 9>;
  using Y = strong_bounded_int<YTag, int, 1, 5>;
  void f(X, Y);

  f(X(1), Y(1));

  </code>
  */
  template <class Tag, class UT, UT Low, UT High, class Default = uninitialized_t>
  struct strong_bounded_int final : protected_wrapper<UT, Default>
  {
      using base_type = protected_wrapper<UT, Default>;
      using base_type::base_type;
      using base_type::underlying;

      using tag_type = Tag;
      using underlying_t = UT;

      enum bounds : UT { low=Low, high=High };

      static constexpr bool valid(UT x) noexcept
      {
        return (Low <= x && x <= High) ;
      }
      static constexpr UT cast(UT x)
      {
        if ( valid(x) ) return x;
        throw bad_bounded_int_cast();
      }
      // copy constructor/assignment default
      constexpr strong_bounded_int() noexcept : base_type() {}
      constexpr strong_bounded_int(UT v) : base_type(cast(v)) {}

      // additive operators
      friend constexpr strong_bounded_int operator+(strong_bounded_int x)  noexcept
      { return x; }
      friend constexpr strong_bounded_int operator+(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value + y.value); }
      constexpr strong_bounded_int& operator+=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value + y.value);
        return *this;
      }
      constexpr strong_bounded_int operator++()  noexcept
      { return strong_bounded_int(++this->value); }
      constexpr strong_bounded_int operator++(int)  noexcept
      { return strong_bounded_int(this->value++); }

      friend constexpr strong_bounded_int operator-(strong_bounded_int x)  noexcept
      { return strong_bounded_int(-x.value); }
      friend constexpr strong_bounded_int operator-(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value - y.value); }
      constexpr strong_bounded_int& operator-=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value - y.value);
        return *this;
      }
      constexpr strong_bounded_int operator--()  noexcept
      { return strong_bounded_int(--this->value); }
      constexpr strong_bounded_int operator--(int)  noexcept
      { return strong_bounded_int(this->value--); }

      //  Multiplicative operators
      friend constexpr strong_bounded_int operator*(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value * y.value); }
      constexpr strong_bounded_int& operator*=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value * y.value);
        return *this;
      }

      friend constexpr strong_bounded_int operator/(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value / y.value); }
      constexpr strong_bounded_int& operator/=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value / y.value);
        return *this;
      }

      friend constexpr strong_bounded_int operator%(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value % y.value); }
      constexpr strong_bounded_int& operator%=(strong_bounded_int y)  noexcept
      {
        this->value = cast(this->value % y.value);
        return *this;
      }

      // Bitwise logic operators
      friend constexpr strong_bounded_int operator~(strong_bounded_int x)  noexcept
      { return strong_bounded_int(~x.value); }

      friend constexpr strong_bounded_int operator&(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value & y.value); }
      constexpr strong_bounded_int& operator&=(strong_bounded_int y)  noexcept
      { this->value = cast(this->value & y.value); return *this; }
      friend constexpr strong_bounded_int operator|(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value | y.value); }
      constexpr strong_bounded_int& operator|=(strong_bounded_int y)  noexcept
      { this->value = cast(this->value | y.value); return *this; }
      friend constexpr strong_bounded_int operator^(strong_bounded_int x, strong_bounded_int y)  noexcept
      { return strong_bounded_int(x.value ^ y.value); }
      constexpr strong_bounded_int& operator^=(strong_bounded_int y)  noexcept
      { this->value = cast(this->value ^ y.value); return *this; }

      // Bitwise logic operators
      friend constexpr strong_bounded_int operator<<(strong_bounded_int x, int y)  noexcept
      { return strong_bounded_int(x.value << y); }
      constexpr strong_bounded_int& operator<<=(int y)  noexcept
      { this->value = cast(this->value << y); return *this; }
      friend constexpr strong_bounded_int operator>>(strong_bounded_int x, int y)  noexcept
      { return strong_bounded_int(x.value >> y); }
      constexpr strong_bounded_int& operator>>=(int y)  noexcept
      { this->value = cast(this->value >> y); return *this; }

      // relational operators
      friend constexpr bool operator==(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_bounded_int x, strong_bounded_int y)  noexcept { return x.value >= y.value; }
  };

}
}
}

#endif // header
