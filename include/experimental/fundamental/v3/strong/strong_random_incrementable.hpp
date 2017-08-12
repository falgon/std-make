// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_RANDOM_INCREMENTABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_RANDOM_INCREMENTABLE_HPP
/**
 * @file
 * strong_random_incrementable<Tag, Rep> is a generalization of std::chrono::duration<Rep, Period>
 *
 * Where Tag is a tag class that has a conversion function between Domains
 *
 * The idea is to be able to define std::chrono::duration<Rep, Period> as an alias of strong_random_incrementable<domain::duration<Period>, Rep>.
 *
 * In addition to the underlying() function it provides the count() member function.
 */

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/incrementable.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/type_traits.hpp>

#include <iosfwd>
#include <limits>
#include <functional>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  /**
  `strong_random_incrementable` is a strongly type tagged by the tag @c Tag that wraps an integral type @c UT and support pointer arithmetic.
  No conversion to the underlying type is provided.
  */

  /// strong_random_incrementable is a strong number with restricted arithmetic close to the pointer arithmetic.
  template <class Tag, class UT, class DiffType>
  struct strong_random_incrementable final
    : strong_type<strong_random_incrementable<Tag, UT, DiffType>, UT>
    , mixin::comparable<strong_random_incrementable<Tag, UT, DiffType>>
    , mixin::random_incrementable<strong_random_incrementable<Tag, UT, DiffType>, DiffType>
    , mixin::streamable<strong_random_incrementable<Tag, UT, DiffType>>
  {
      // associated types
      using domain = Tag;
      using rep = UT;
      using difference_type = DiffType;

      using base_type = strong_type<strong_random_incrementable<Tag, UT, DiffType>, UT>;

      // constructors
      using base_type::base_type;
  };


}
}


  /// Hash specialization forwarding to the hash of underlying type
  template <class Tag, class UT, class DT>
  struct hash<experimental::strong_random_incrementable<Tag,UT, DT>>
    : experimental::wrapped_hash<experimental::strong_random_incrementable<Tag, UT, DT>> {};

}
#endif // header
