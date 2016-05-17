//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_APPLICATIVE_APPLICATIVE_HPP
#define JASEL_FUNDAMENTAL_V2_APPLICATIVE_APPLICATIVE_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type M is model of an Applicative if it is a model of Functor and
///
///   std::experimental::TypeConstructor<M>
///   std::experimental::ValueType<M>
///
///   make<TypeConstructor<M>>(ValueType<M>) -> M
///   ap(F const &, M const &) -> Invoke<TypeConstructor<M>, ResultType<ValueType<F>, ValueType<M>>>
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/functor/functor.hpp>
#include <experimental/fundamental/v2/applicative/applicative_tag.hpp>
#include <experimental/fundamental/v2/applicative/ap.hpp>

#endif // header
