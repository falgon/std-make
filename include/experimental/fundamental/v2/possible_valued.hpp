//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_HPP
#define JASEL_FUNDAMENTAL_V2_POSSIBLEVALUED_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type PV is model of a PossibleValued if
///
///   std::experimental::TypeConstructor<PV>
///   std::experimental::ValueType<PV> -> PV
///   std::experimental::NoneType<PV> -> PV
///
///   make<TypeConstructor<PV>>(const ValueType<PV>& v) -> PV
///   none<TypeConstructor<PV>>() -> NoneType<PV>
///   has_value(const PV &) -> bool
///   value(PV& ) -> ValueType<PV>&
///   value(const PV &) -> const ValueType<PV> &
///   none(const PV &) -> NoneType<PV>
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/make.hpp>
#include <experimental/nullable.hpp>
#include <experimental/fundamental/v3/value_type.hpp>
#include <experimental/fundamental/v2/possible_valued/has_value.hpp>
#include <experimental/fundamental/v2/possible_valued/value.hpp>
#include <experimental/fundamental/v2/possible_valued/not_a_value.hpp>
#include <experimental/fundamental/v2/possible_valued/possible_value_tag.hpp>

#endif // header
