// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/only_when.hpp
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_ONLY_WHEN_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_ONLY_WHEN_HPP

#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

#define JASEL_ENABLE_IF(...) typename ::std::enable_if<(__VA_ARGS__), bool>::type = true

template <typename T, template <typename, typename> class TypePred>
class only_when
{
  T _val;

public:
  template <typename U, JASEL_ENABLE_IF(TypePred<T, U>::value)>
    only_when (U&& v) : _val(::std::forward<U>(v)) {}

  template <typename U, JASEL_ENABLE_IF(!TypePred<T, U>::value)>
    only_when (U&&) = delete;

  T get() const { return _val; }
};

#undef JASEL_ENABLE_IF


}
}
}

#endif // header
