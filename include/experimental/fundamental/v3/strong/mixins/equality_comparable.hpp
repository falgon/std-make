// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_EQUALITY_COMPARABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_EQUALITY_COMPARABLE_HPP

#include <experimental/fundamental/v3/strong/mixins/is_compatible_with.hpp>
#include <experimental/meta.hpp>
#include <type_traits>

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
        { return x._backdoor()._underlying() == y._backdoor()._underlying();}
        //! Forwards to the underlying value
        friend constexpr bool operator!=(Final const& x, Final const& y) noexcept
        { return x._backdoor()._underlying() != y._backdoor()._underlying();}
      };

      template <class Final, class Other>
      struct equality_comparable_with : equality_comparable<Final>
      {
        static_assert(is_same<Final, Other>::value==false, "Final and Other must be different");
        //! Forwards to the underlying value
        friend constexpr bool operator==(Final const& x, Other const& y) noexcept
        { return x._backdoor()._underlying() == y._backdoor()._underlying();}
        friend constexpr bool operator==(Other const& x, Final const& y) noexcept
        { return x._backdoor()._underlying() == y._backdoor()._underlying();}
        //! Forwards to the underlying value
        friend constexpr bool operator!=(Final const& x, Other const& y) noexcept
        { return x._backdoor()._underlying() != y._backdoor()._underlying();}
        friend constexpr bool operator!=(Other const& x, Final const& y) noexcept
        { return x._backdoor()._underlying() != y._backdoor()._underlying();}
      };

      template <class Final, template <class, class> class Pred=is_compatible_with>
      struct equality_comparable_with_if
      {
        //! Forwards to the underlying value
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr bool operator==(Final const& x, Other const& y) noexcept
        { return x._backdoor()._underlying() == y._backdoor()._underlying();}

        //! Forwards to the underlying value
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr bool operator!=(Final const& x, Other const& y) noexcept
        { return x._backdoor()._underlying() != y._backdoor()._underlying();}
      };
    }
  }
}
}

#endif // header
