//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_META_V1_QUOTE_HPP
#define JASEL_META_V1_QUOTE_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/defer.hpp>

namespace std
{
namespace experimental
{
namespace meta
{
inline namespace v1
{
  // transforms a template class into a type_constructor that adds the parameter at the end

  template <template <class ...> class TC>
  struct quote
  {
    template <typename... Ts>
    using invoke = eval<defer<TC, Ts... >>;
  };

}
}
}
}

#endif // header
