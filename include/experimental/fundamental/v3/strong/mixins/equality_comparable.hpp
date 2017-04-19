// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_EQUALITY_COMPARABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_EQUALITY_COMPARABLE_HPP

#include <experimental/meta.hpp>

namespace std
{
  namespace experimental
  {
  inline  namespace fundamental_v3
  {
    namespace mixin
    {
      template <class Final>
      struct equality_comparable
      {
        //! Forwards to the underlying value
        friend constexpr bool operator==(Final const& x, Final const& y) noexcept
        { return x.underlying() == y.underlying();}
        //! Forwards to the underlying value
        friend constexpr bool operator!=(Final const& x, Final const& y) noexcept
        { return x.underlying() != y.underlying();}
      };

      template <class Final, class Other>
      struct equality_comparable_with : equality_comparable<Final>
      {
        //! Forwards to the underlying value
        friend constexpr bool operator==(Final const& x, Other const& y) noexcept
        { return x.underlying() == y.underlying();}
        friend constexpr bool operator==(Other const& x, Final const& y) noexcept
        { return x.underlying() == y.underlying();}
        //! Forwards to the underlying value
        friend constexpr bool operator!=(Final const& x, Other const& y) noexcept
        { return x.underlying() != y.underlying();}
        friend constexpr bool operator!=(Other const& x, Final const& y) noexcept
        { return x.underlying() != y.underlying();}
      };

      // todo: this predicate either must be more specific or it should be more generic as it can be used also on other situations.
      template <class T, class U>
      struct is_comparable_with : false_type {};

      //template <class T>
      //struct is_comparable_with<T,T> : true_type {};

      template <class Final, template <class, class> class Pred=is_comparable_with>
      struct equality_comparable_with_if : equality_comparable<Final>
      {
        //! Forwards to the underlying value
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr bool operator==(Final const& x, Other const& y) noexcept
        { return x.underlying() == y.underlying();}

        //! Forwards to the underlying value
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr bool operator!=(Final const& x, Other const& y) noexcept
        { return x.underlying() != y.underlying();}

      };

    }
  }
}
}

#endif // header