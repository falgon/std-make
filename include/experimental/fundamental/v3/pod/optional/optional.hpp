// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

// todo
// * add view::optional that contains a pointer to a class C and is parameterized by the present data member pointer and
//    the value data member pointer
//  template <class C, class T, T C::*Value, class Bool, Bool C::*Present>
//  class optional_view;
//
// * add a optional that has the type of the mask as parameter
// * what is common to all these classes
//  If we make abstraction of the storage, all these classes define almost the same interface, except maybe the construction
//

#ifndef JASEL_FUNDAMENTAL_V3_POD_OPTIONAL_HPP
#define JASEL_FUNDAMENTAL_V3_POD_OPTIONAL_HPP

#if defined __clang__
#if (__clang_major__ >= 4) && (__cplusplus > 201402L)
#define JASEL_HAVE_OPTIONAL
#endif
#else
#endif

# include <experimental/fundamental/v2/config.hpp>

#if defined JASEL_HAVE_OPTIONAL
#include <optional>
#else
# include <experimental/fundamental/v3/in_place.hpp>
# include <experimental/fundamental/v3/optional/nullopt.hpp>
# include <experimental/fundamental/v3/optional/bad_optional_access.hpp>
#endif
#include <functional>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace pod
{
  using std::nullopt_t;
  using std::nullopt;
  using std::in_place_t;
  using std::in_place;
  using std::bad_optional_access;

  template <class T, class Bool=bool>
  class optional
  {
  public:
    static_assert(is_pod<T>::value, "T must be a POD");
    static_assert(is_pod<Bool>::value, "Bool must be a POD");

    using bool_type = Bool;
    using value_type = T;

    constexpr optional() = default; // optional is a POD
    //! You should use
    //! pod::optional<T> opt;
    //! opt.construct();
    //! opt = nullopt;
    //! opt.reset();
    // @par Effects Constructs the object that does not contain a value.
    void construct()  noexcept
    {
      m_present = false;
    }

    optional(optional const&) = default; // optional is a POD
    optional(optional &&) = default; // optional is a POD
    optional& operator=(optional const&) = default; // optional is a POD
    optional& operator=(optional &&) = default; // optional is a POD
    ~optional() = default;

    //! You could use
    //! opt1.copy_assign(opt2);
    //! which will copy the value type only if m_present.
    //! Note that copy and assignement will copy all the bytes
    void copy_assign(optional const& other) noexcept
    {
      m_present =  other.m_present;
      if (m_present)
      {
        m_value = other.m_value;
      }
    }
    void move_assign(optional && other) noexcept
    {
      if (m_present)
      {
        m_value = move(other.m_value);
      }
      m_present =  other.m_present;
    }

    constexpr optional(nullopt_t) noexcept
    : m_present{false}
    {
    }

    constexpr optional( T&& value ) noexcept
        : m_present ( true ), m_value ( move(value) )
    {
    }
    constexpr optional( T const& value ) noexcept
        : m_present ( true ), m_value ( value )
    {
    }

#if 0
    template < class U = value_type >
    /* EXPLICIT */ constexpr optional( U&& value ) noexcept
    {
      m_value = move(value);
      m_present =  true;
    }

    template < class U, class B >
    /* EXPLICIT */ optional( const optional<U, B>& other ) noexcept;
    template < class U, class B >
    /* EXPLICIT */ optional( optional<U, B>&& other ) noexcept;
#endif
    // fixme: should inplace use {args...} instead of T(args..)
    template< class... Args >
    JASEL_CXX14_CONSTEXPR explicit optional( std::in_place_t, Args&&... args ) noexcept
    {
      m_value = T(forward<Args>(args)...);
      m_present =  true;
    }
    template< class U, class... Args >
    JASEL_CXX14_CONSTEXPR explicit optional( std::in_place_t,
                                 std::initializer_list<U> ilist,
                                 Args&&... args ) noexcept
    {
      m_value = T(ilist, forward<Args>(args)...);
      m_present =  true;
    }

    // Assignment
    optional& operator=(nullopt_t) noexcept
    {
      m_present =  false;
      return *this;
    }

    optional& operator=(T&& value) noexcept
    {
      m_value = move(value);
      m_present =  true;
      return *this;
    }

    optional& operator=(T const& value) noexcept
    {
      m_value = value;
      m_present =  true;
      return *this;
    }

    template <class... Args>
    void emplace(Args&&... args) noexcept
    {
      m_value = T(std::forward<Args>(args)...);
      m_present = true;
    }

    template <class U, class... Args>
    void emplace(initializer_list<U> il, Args&&... args) noexcept
    {
      m_value = T(il, std::forward<Args>(args)...);
      m_present = true;
    }

    void swap(optional<T,Bool>& rhs) noexcept
    {
        swap(m_value, rhs.m_value);
        swap(m_present, rhs.m_present);
    }


    JASEL_CXX14_CONSTEXPR const T* operator->() const { return &m_value; }
    JASEL_CXX14_CONSTEXPR T* operator->() { return &m_value; }
    JASEL_CXX14_CONSTEXPR const T& operator*() const& { return m_value; }
    JASEL_CXX14_CONSTEXPR T& operator*() & { return m_value; }
    JASEL_CXX14_CONSTEXPR const T&& operator*() const&& { return move(m_value); }
    JASEL_CXX14_CONSTEXPR T&& operator*() && { return move(m_value); }

    constexpr bool has_value() const noexcept { return bool(m_present); }
    constexpr explicit operator bool() const noexcept { return has_value(); }

    JASEL_CXX14_CONSTEXPR value_type& value() &
    {
      if (has_value())
        return m_value;
      throw bad_optional_access();
    }
    JASEL_CXX14_CONSTEXPR value_type const& value() const&
    {
      if (has_value())
        return m_value;
      throw bad_optional_access();
    }

    JASEL_CXX14_CONSTEXPR value_type&& value() &&
    {
      if (has_value())
        return m_value;
      throw bad_optional_access();
    }
    JASEL_CXX14_CONSTEXPR value_type const&& value() const&&
    {
      if (has_value())
        return m_value;
      throw bad_optional_access();
    }

    template< class U >
    JASEL_CXX14_CONSTEXPR value_type value_or(U && default_value) const&
    {
      return has_value() ? **this : static_cast<T>(forward<U>(default_value));
    }
    template< class U >
    JASEL_CXX14_CONSTEXPR value_type value_or(U && default_value) &&
    {
      return has_value() ? move(**this) : static_cast<T>(forward<U>(default_value));
    }

    void reset() noexcept
    {
      m_present = false;
    }

  private:
    bool_type m_present;
    value_type m_value;
  };

  // 20.5.8, Relational operators
  template <class T, class B>
  constexpr bool operator==(const optional<T, B>& x, const optional<T, B>& y)
  {
    return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
  }

  template <class T, class B>
  constexpr bool operator!=(const optional<T, B>& x, const optional<T, B>& y)
  {
    return !(x == y);
  }

  template <class T, class B>
  constexpr bool operator<(const optional<T, B>& x, const optional<T, B>& y)
  {
    return (!y) ? false : (!x) ? true : *x < *y;
  }

  template <class T, class B>
  constexpr bool operator>(const optional<T, B>& x, const optional<T, B>& y)
  {
    return (y < x);
  }

  template <class T, class B>
  constexpr bool operator<=(const optional<T, B>& x, const optional<T, B>& y)
  {
    return !(y < x);
  }

  template <class T, class B>
  constexpr bool operator>=(const optional<T, B>& x, const optional<T, B>& y)
  {
    return !(x < y);
  }

#if 0
  // 20.5.9, Comparison with nullopt
  constexpr bool operator==(nullopt_t, nullopt_t) noexcept
  {
    return true;
  }
  constexpr bool operator!=(nullopt_t, nullopt_t) noexcept
  {
    return false;
  }
#endif

  template <class T, class B>
  constexpr bool operator==(const optional<T, B>& x, nullopt_t) noexcept
  {
    return (!x);
  }

  template <class T, class B>
  constexpr bool operator==(nullopt_t, const optional<T, B>& x) noexcept
  {
    return (!x);
  }

  template <class T, class B>
  constexpr bool operator!=(const optional<T, B>& x, nullopt_t) noexcept
  {
    return bool(x);
  }

  template <class T, class B>
  constexpr bool operator!=(nullopt_t, const optional<T, B>& x) noexcept
  {
    return bool(x);
  }

  template <class T, class B>
  constexpr bool operator<(const optional<T, B>&, nullopt_t) noexcept
  {
    return false;
  }

  template <class T, class B>
  constexpr bool operator<(nullopt_t, const optional<T, B>& x) noexcept
  {
    return bool(x);
  }

  template <class T, class B>
  constexpr bool operator<=(const optional<T, B>& x, nullopt_t) noexcept
  {
    return (!x);
  }

  template <class T, class B>
  constexpr bool operator<=(nullopt_t, const optional<T, B>&) noexcept
  {
    return true;
  }

  template <class T, class B>
  constexpr bool operator>(const optional<T, B>& x, nullopt_t) noexcept
  {
    return bool(x);
  }

  template <class T, class B>
  constexpr bool operator>(nullopt_t, const optional<T, B>&) noexcept
  {
    return false;
  }

  template <class T, class B>
  constexpr bool operator>=(const optional<T, B>&, nullopt_t) noexcept
  {
    return true;
  }

  template <class T, class B>
  constexpr bool operator>=(nullopt_t, const optional<T, B>& x) noexcept
  {
    return (!x);
  }

#if 0
  // 20.5.10, Comparison with T
  template <class T, class B>
  constexpr bool operator==(const optional<T>& x, const T& v)
  {
    return bool(x) ? *x == v : false;
  }

  template <class T, class B>
  constexpr bool operator==(const T& v, const optional<T>& x)
  {
    return bool(x) ? v == *x : false;
  }

  template <class T, class B>
  constexpr bool operator!=(const optional<T>& x, const T& v)
  {
    return bool(x) ? *x != v : true;
  }

  template <class T, class B>
  constexpr bool operator!=(const T& v, const optional<T, B>& x)
  {
    return bool(x) ? v != *x : true;
  }

  template <class T, class B>
  constexpr bool operator<(const optional<T, B>& x, const T& v)
  {
    return bool(x) ? *x < v : true;
  }

  template <class T, class B>
  constexpr bool operator>(const T& v, const optional<T, B>& x)
  {
    return bool(x) ? v > *x : true;
  }

  template <class T, class B>
  constexpr bool operator>(const optional<T, B>& x, const T& v)
  {
    return bool(x) ? *x > v : false;
  }

  template <class T, class B>
  constexpr bool operator<(const T& v, const optional<T, B>& x)
  {
    return bool(x) ? v < *x : false;
  }

  template <class T, class B>
  constexpr bool operator>=(const optional<T, B>& x, const T& v)
  {
    return bool(x) ? *x >= v : false;
  }

  template <class T, class B>
  constexpr bool operator<=(const T& v, const optional<T, B>& x)
  {
    return bool(x) ? v <= *x : false;
  }

  template <class T, class B>
  constexpr bool operator<=(const optional<T, B>& x, const T& v)
  {
    return bool(x) ? *x <= v : true;
  }

  template <class T, class B>
  constexpr bool operator>=(const T& v, const optional<T, B>& x)
  {
    return bool(x) ? v >= *x : true;
  }
#endif
  // 20.5.12, Specialized algorithms
  template <class T, class B>
  void swap(optional<T, B>& x, optional<T, B>& y) noexcept(noexcept(x.swap(y)))
  {
    x.swap(y);
  }

  template <class T>
  constexpr optional<typename decay<T>::type, bool> make_optional(T&& v)
  {
    return optional<typename decay<T>::type, bool>(forward<T>(v));
  }

#if 0
  template <class T, class B, class ... Args>
  constexpr optional<T, B> make_optional(Args&&... args)
  {
    return optional<T, B>(forward<Args>(args)...);
  }

  template <class T, class B, class U, class ... Args>
  constexpr optional<T, B> make_optional( initializer_list<U> il, Args&&... args)
  {
    return optional<T, B>(il, forward<Args>(args)...);
  }
#endif
}
}
}
}
namespace std
{
  template <typename T, class B>
  struct hash<std::experimental::pod::optional<T, B>>
  {
    typedef typename hash<T>::result_type result_type;
    typedef std::experimental::pod::optional<T, B> argument_type;

    constexpr result_type operator()(argument_type const& arg) const {
      return arg ? std::hash<T>{}(*arg) : result_type{};
    }
  };
}

#endif // header
