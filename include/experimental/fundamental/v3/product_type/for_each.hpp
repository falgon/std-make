//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_FOR_EACH_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_FOR_EACH_HPP

#include <experimental/fundamental/v3/product_type.hpp>
#include <utility>
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace product_type
{
  namespace detail {

    template <class F, class ProductType, std::size_t... I>
    constexpr void for_each_impl( F&& f, ProductType&& pt, index_sequence<I...> )
    {
      swallow(invoke(std::forward<F>(f), product_type::get<I>(forward<ProductType>(pt)))...);
    }

  } // namespace detail

  /**
   * Invoke the Callable object f with a product_type of the arguments.
   *
   * @par f Callable object to be invoked on each product type element
   * @par pt product type whose elements to be used as arguments to f
   *
   * @pre
   * - DProductType is a model of ProductType.
   * - DF is a model of Callable<product_type::element_t<I, DProductType>>...  and
   *
   * where DF and DProductType are respectively the decay of F and ProductType removing cv and reference.
   */

  template <class F, class ProductType
  // todo add constraint on F
  , class = enable_if_t< is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>> >
  >
  constexpr void for_each(F&& f, ProductType&& pt)
  {
      detail::for_each_impl(forward<F>(f), forward<ProductType>(pt),
          make_index_sequence<product_type::size_v<remove_cv_t<remove_reference_t<ProductType>>>>{});
  }

}
}}
}

#endif // header
