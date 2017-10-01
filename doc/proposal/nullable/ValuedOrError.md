<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXXR0</span></td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2017-09-27</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">ISO/IEC JTC1 SC22 WG21 Programming Language C++</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Audience:</td>
        <td width="435">Library Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escrib&aacute; &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@nokia.com</a>&gt;</td>
    </tr>
</table>

# *SuccessOrFailure*, *ValuedOrError* and *ValueOrNone* types
============================================================

**Abstract**

There are types that contain a success value or a failure value.

In the same way we have *Nullable* types that have a single not-a-value we have types that can contain a single instance of value-type and a mean to retrieve it using  the `deref` function. These types have something in common with *Nullable* and is the ability to know if they have a value or not via the `has_value` function. Types that are possibly valued and have a single error are named in this paper *ValedOrError*. They provide the `error` function. `optional`, pointers and smart pointers are *ValeuOrNone* types. `expected` is a *ValuedOrError* type. The ongoing `result<T>` is *SuccessOrFailure*.




# Table of Contents

* [Introduction](#introduction)
* [Motivation and Scope](#motivation-and-scope)
* [Proposal](#proposal)
* [Design Rationale](#design-rationale)
* [Proposed Wording](#proposed-wording)
* [*Implementability](#implementability)
* [Open points](#open-points)
* [Acknowledgements](#acknowledgements)
* [History](#history)
* [References](#references)

# Introduction


This paper proposes the concept of *SuccessOrFailure* that represents a type that can contain a success value or a failure value that can be used as the result of a function to return the value computed by the function or the reason of the failure of this computation. As a sum type it provides the `visit` function.

*SuccessOrFailure* contains the interface needed to customize the types that can work with the proposed `operator try`. This makes the error propagation on functions returning this kind of types much more simpler.

The paper proposes also some error handling utilities that help while the user wants to recover from error as `resolve`,  `value_or`, `value_or_throw`, `error_or` and `check_error`. 

When the type is *TypeConstructible* and *SuccessOrFailure*, the type can be seen as a *Functor*, an *ApplicativeFunctor*, a *Monad* or a *MonadError*.

Some *SuccessOrFailure* types contain success and/or failure types that wrap a value or an error. However, the user wants to see the wrapped value and error types instead of the wrapping success and failure types. We name these types *ValueOrError*. These types unwrap the wrapped value before calling to the user provided functions.

<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="807">
    <tr>
        <td align="left" valign="top"> BEFORE </td>
        <td align="left" valign="top"> AFTER </td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Construction
        </b></td>
        <td  align="left" valign="top"><b>
        </b></td>
    </tr>
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

        </pre>
        </td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
        </pre>
        </td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Conversion

        </b></td>
        <td  align="left" valign="top"><b>
        </b></td>
    </tr>
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

        </pre>
        </td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
        </pre>
        </td>
    </tr>
    <tr>
        <td  align="left" valign="top"><b>
Return
        </b></td>
        <td  align="left" valign="top"><b>
        </b></td>
    </tr>
    <tr>
        <td align="left" valign="top"><pre class='brush: cpp'>

        </pre>
        </td>
        <td align="left" valign="top"><pre class='brush: cpp'> 
        </pre>
        </td>
    </tr>
    
       
</table>

# Motivation and Scope

## Towards an `operator try`

[TRY] proposes a new `operator try` that would make much simpler the use of `optional` and `expected` as result type of a function.

The customization of the `operator try` consists in returning a type that has 3 functions:

* check for the success of the computation of the parameter
* get the value of the success path
* get the value of the failure path

The result type of getting the value of the success and the failure paths must be implicitly convertible to the return type of the function calling the `operator try`

The name of these functions depend on the concept we want to abstract. If we name this concept *SuccessOrFailure* they could be:

* `succeeded `or `failed`
* `success_value`
* `failure_value`

Here it is an example:
 
```
U h(T);

X<T,E> f();

X<U,E> g() {
    auto t =  try f();
    return h(t);  
}  
```

The meaning of a *try-expression* is given by the following transformation

```c++
X<U,E> g() {
    auto __trier = operator try(f());
    if ( __trier.failed() )
        return __trier.failure_value();
    auto t = __trier.success_value();    
    return h(t);  
}  
```

There is more to say about the *try-expression* when used elsewhere than the initialization part of a variable definition, e.g. when nested as argument of a function call. We will see this later.

## *SuccessOrFailure* operations

While the use of *try-expresssions* is good to propagate failure, we need some generic functions if we want to do something else.

We propose a concept *SuccessOrFailure* that allows to customize the 4 functions and provide access via

* `success_or_failure::succeeded`
* `success_or_failure::failed`
* `success_or_failure::success_value`
* `success_or_failure::failure_value`

We can then overload the `operator try` for this concept

```c++
template <SuccessOrFailure T>
constexpr auto operator try(T&&)
{
    struct tryer {
        T&& v;
        
        constexpr bool succeeded() const noexcept 
        { return success_or_failure::succeeded(v) }
        constexpr bool failed() const noexcept 
        { return success_or_failure::failed(v) }
        constexpr auto success_value() const noexcept 
        { return success_or_failure::success_value(move(v)) }
        constexpr auto failure_value() const noexcept 
        { return success_or_failure::failure_value(move(v)) }
    };
    return tryer{ std::move(v) };
}
```

I don't know if the proposed *try-expression* interpretation is best way,  
however given the previous concept *SuccessOrFailure* we could interpret the *try-expression* alternatively as follows 

```c++
X<U,E> g() {
    auto __tmp = f();
    if ( success_or_failure::failed(__tmp) )
        return success_or_failure::failure_value(move(__tmp));
    auto t = success_or_failure::success_value(move(__tmp));    
    return h(t);  
}  
```

This interpretation is more in line with the transformation of the range-based for loop, which could be seen as simpler but makes again the language dependent on the library.

Note that a *SuccessOrFailure* can be seen as a *Tryable*, but not the opposite.
 
## Error handling with *SuccessOrFailure*

While the *try-expression* is used to propagate the error as such, the user needs at a given moment recover or propagate a different error. Next we describe some these utilities.

### Do we need a generic `value_or` function?

We have `optional::value_or()` and `expected::value_or()` functions with a similar definition. This function can be defined in a generic way for *SuccessOrFailure* types as follows

```c++
template <SuccessOrFailure X, class T>
auto value_or(X&& x, T&& v) 
{
    using namespace success_or_failure;
    if ( succeeded(forward<X>(x) )
        return success_value(move(x));    
    return forward<T>(v);
}
```

### Do we need a generic `value_or_throw` function?

We have `optional::value()` and `expected::value()` functions with a similar definition, but returning a specific exception. It has been argued that the user need sometimes to throw a specific exception more appropriated to his context. We can define a function for *SuccessOrFailure* types that allows to specify the exception to throw as follows

```c++
template <class E, SuccessOrFailure X>
auto value_or_throw(X&& x) 
{
    using namespace success_or_failure;
    if ( succeeded(forward<X>(x) )
        return success_value(move(x));    
    throw E{failure_value(move(x))};
}
```
### Do we need a generic `resolve` function?

The previous function `value_or_throw` is a special case of error handling. We can have a more general one `resolve` that takes a function having as parameter the failure type.

```c++
template <SuccessOrFailure X, class F>
auto resolve(X&& x, F&& f) 
{
    using namespace success_or_failure;
    if ( succeeded(forward<X>(x) )
        return success_value(move(x));    
    throw invoke(forward<F>(f), failure_value(move(x)));
}
```

With this definition `value_or` could be defined as

```c++
template <SuccessOrFailure X, class T>
auto value_or(X&& x, T v) 
{
    return resolve(forward<X>(x), [v](auto &&failure) {
        return v;
    });
}
```

and `value_or_throw` could be defined as

```c++
template <class E, SuccessOrFailure X>
auto value_or_throw(X&& x) 
{
    return resolve(forward<X>(x), [](auto &&failure) {
        throw E{failure};
    });
}
```

### Do we need a generic `error_or` function?

It has been argued that the error should be always available and that often there
is a success value associated to the error. We have the `status_value` proposal
and `expected<T,E>` could be seen more like something like the proposed

```
struct status_value {
    E error; 
    optional<T> opt_value;
};
```

The following code shows a use case

```c++
auto e = function();
switch (e.status())
	success: ....; break;
	too_green: ....; break;
	too_pink: ....; break;
```

With the current interface the user could be tempted to do

```c++
auto e = function();
if (e)
	/*success:*/ ....;
else
	switch (e.error())
	case too_green: ....; break;
	case too_pink: ....; break;
```
	
This could be done with the current interface as follows

```c++
auto e = function();
switch (error_or(e, success))
	success: ....; break;
	too_green: ....; break;
	too_pink: ....; break;
```

where

```c++
template <SuccessOrFailure X, class E>
E error_or(X && x, E&& err) {
    using namespace success_or_failure;
    if ( failed(forward<X>(x) )
        return failure_value(move(x));    
    return forward<E>(err);
}
```

### Need for *ValueOrError*

Note that the previous `value_or` function works for `optional` and `expected` as both have a success type that match the value type. However, `error_or` doesn't works for `expected` as `expected<T,E>` is not implicitly convertible from `E` but from `unpexpected<E>` which wraps an `E`.

For *SuccessOrFailure* types for which the success type wraps the value type and/or the error type, we need to unwrap the success/failure type to get a value/error type. We name those *ValueOrError* types.

```
template <ValueOrError X, class T>
auto value_or(X&& x, T&& v) 
{
    using namespace success_or_failure;
    if ( succeeded(forward<X>(x) )
        return wrapped::unwrap(success_value(move(x)));    
    return forward<T>(v);
}
```

For this *ValueOrError* types it will be better to define 2 functions that unwrap directly the success or the failure value


```
namespace value_or_error {
    using namespace success_or_value;
    using namespace wrapped;
    template <class X>
    auto deref(X&& x) 
    {
        return unwrap(success_value(move(x)));    
    }
    template <class X>
    auto error(X&& x) 
    {
        return unwrap(failure_value(move(x)));    
    }
    template <class X>
    auto has_value(X const& x) 
    {
        return succeeded(x);    
    }
    template <class X>
    auto has_error(X const& x) 
    {
        return failed(x);    
    }

}
```


With these definitions

```
template <ValueOrError X, class T>
auto value_or(X&& x, T&& v) 
{
    using namespace value_or_error;
    if ( has_value(forward<X>(x) )
        return deref(move(x)));    
    return forward<T>(v);
}

template <ValueOrError X, class E>
E error_or(X && x, E&& err) {
    using namespace value_or_error;
    if ( has_error(forward<X>(x) )
        return error(move(x));    
    return forward<E>(err);
}
```

If `wrapped::unwrap` is the identity for non-wrapped types, we have that the previous definition works as well for *SuccessOrFailure* types.


### Do we need a generic `check_error` function?

Another use case which could look much uglier is if the user had to test for whether or not there was a specific error code.

```c++
auto e = function();
while ( e.status == timeout ) {
	sleep(delay);
	delay *=2;
	e = function();
}
```

Here we have a value or a hard error. This use case would need to use something like `check_error` 

```c++
e = function();
while ( check_error(e, timeout) )
{
	sleep(delay);
	delay *=2;
	e = function();
}
```

where

```c++
template <SuccessOrFailure X, class E>
bool check_error(X const& e, E&& err) {
    using namespace success_or_failure;
    if ( succeeded(x) )
        return false;    
    return failure_value(x)) == forward<E>(err);
}
```

We can extend the scope of this function by using instead *ValueOrError*

```c++
template <ValueOrError X, class E>
bool check_error(X const& e, E&& err) {
    using namespace value_or_error;
    if ( has_value(x) )
        return false;    
    return error(x)) == forward<E>(err);
}
```

# Proposal

This paper proposes to

* add `succeeded(n)`, `success_value(n)` and `failure_value(n)` for *SuccessOrFailure* types,
* add `has_value(n)`, `deref(n)` and `error(n)` for *ValuedOrError* types,
* *ValuedOrError* type are *SuccessOrFailure* types,
* add *ValueOrNone* types as an extension of *Nullable* types for which there is only a possible value type,
* *ValueOrNone* type are *ValuedOrError* types,
* add the following algorithms *ValuedOrError*
    * `value_or`,
    * `value_or_throw`,
    * `resolve`,
    * `error_or` and,
    * `check_error`.
    
* customize the standard types to these concepts.

# Design Rationale

## Customization

### Traits versus ADL

## Naming


# Impact on the standard

These changes are entirely based on library extensions and do not require any language features beyond what is available in C++17. There are however some classes in the standard that needs to be customized.

This paper depends in some way on the helper classes proposed in [P0343R1], as e.g. the place holder `_t` and the associated specialization for the type constructors `optional<_t>`, `unique_ptr<_t>`, `shared_ptr<_t>`.

# Proposed Wording

The proposed changes are expressed as edits to [N4564] the Working Draft - C++ Extensions for Library Fundamentals V2, but pretend to go to the V3 TS.

**Add the "SuccessOrFailure Objects" section**

## SuccessOrFailure Objects

### Header <experimental/success_or_failure> synopsis [success_or_failure.synop]

```c++
namespace std::experimental {
inline namespace fundamentals_v3 {
namespace success_or_failure {       
    
    // class traits
    template <class T, class Enabler=void>
        struct traits {};

    // class traits_pointer_like
    struct traits_pointer_like;

    // class traits specialization for pointers
    template <class T>
        struct traits<T*>;
          
    template <class T>
        constexpr bool succeeded(T const& v) noexcept;
    template <class T>
        constexpr bool failed(T const& v) noexcept;
    template <class T>
        constexpr bool succeeded(T* v) noexcept;
    template <class T>
        constexpr bool failed(T* v) noexcept;
        
    template <class T>
        constexpr auto succeess_value(T&& x);
    template <class T>
        constexpr T& succeess_value(T* ptr);
        
    template <class T>
        using success_type_t = decltype(succeess_value(declval<T>));        

    template <class T>
        constexpr auto failure_value(T&& x);
    template <class T>
        constexpr T& failure_value(T* ptr);
        
    template <class T>
        using success_type_t = decltype(failure_value(declval<T>));                        
}
 
    template <class T>
        struct is_success_or_failure;
    template <class T>
        struct is_success_or_failure <const T> : is_success_or_failure <T> {};
    template <class T>
        struct is_success_or_failure <volatile T> : is_success_or_failure <T> {};
    template <class T>
        struct is_success_or_failure <const volatile T> : is_success_or_failure <T> {};
    template <class T>
        struct is_success_or_failure <T*> : true_type {};

    template <class T>
        constexpr bool is_success_or_failure_v = is_success_or_failure <T>::value ;


}}
```

###  class `traits` [success_or_failure.traits]

```c++
namespace success_or_failure {
    template <class T, class Enabler=void>
        struct traits {};

    // class traits_pointer_like
    struct traits_pointer_like
    {
        template <class Ptr>
            static constexpr
            bool succeeded(Ptr ptr) { return bool(ptr) }
        template <class T>
            static constexpr bool failed(Ptr ptr) noexcept;        
        template <class T>
            static constexpr auto succeess_value(Ptr ptr);        
        template <class T>
            static constexpr auto failure_value(Ptr ptr);
            
    };

    // class traits specialization for pointers
    template <class T>
        struct traits<T*> 
            : traits_pointer_like<T*>
        {};
} 
```


###  Template function `succeeded` [success_or_failure.succeeded]

```c++
namespace success_or_failure {
    template <class T>
        bool succeeded(T const& v) noexcept;
}
```

###  Template function `failed` [success_or_failure.failed]

```c++
namespace success_or_failure {
    template <class T>
        bool failed(T const& v) noexcept;
}
```


**Adapt the "ValueOrError Objects" section**

## ValueOrError Objects


### Header <experimental/wrapped> synopsis [Wrapped.synop]


### Header <experimental/value_or_error> synopsis [ValueOrError.synop]

```c++
namespace std::experimental {
inline namespace fundamentals_v3 {
namespace value_or_error {
    using namespace success_or_failure;

    // class traits
    template <class T, class Enabler=void>
        struct traits {};

    // class traits_pointer_like
    struct traits_pointer_like;

    // class traits specialization for pointers
    template <class T>
        struct traits<T*>;
        
    template <class T>
        constexpr bool has_value(T const& v) noexcept;
    template <class T>
        constexpr bool has_value(T* v) noexcept;
    template <class T>
        constexpr bool has_error(T const& v) noexcept;
    template <class T>
        constexpr bool has_error(T* v) noexcept;
        
    template <class T>
        constexpr auto deref(T&& x);
    template <class T>
        constexpr T& deref(T* ptr);
        
    template <class T>
        using value_type_t = decltype(nullable::deref(declval<T>));        

    template <class N, class T>
        constexpr auto value_or(N&& ptr, T&& val); 
                
    template <class N, class F, class T>
        constexpr auto apply_or(N&& n, F&& f, T&& v); 
        
    template <class T>
        constexpr auto error(T&& x);
    template <class T>
        constexpr T& error(T* ptr);

    template <class T>
        using error_type_t = decltype(nullable::error(declval<T>));        

    // when type constructible, is a functor
    template <class T, class F>
        constexpr auto transform(T&& n, F&& f); 

    // when type constructible, is an applicative
    template <class F, class T>
        constexpr auto ap(F&& f, T&& n); 

    // when type constructible, is a monad
    template <class T, class F>
        constexpr auto bind(T&& n, F&& f); 

    // when type constructible, is a monad_error
    template <class T, class F>
        constexpr auto catch_error(T&& n, F&& f); 
    template <class T, class ...Xs>
        constexpr auto make_error(Xs&&...xs); 

    // sum_type::visit
    template <class N, class F>
        constexpr auto visit(N&& n, F&& f); 
        
    // helper functions    
    template <class N, class E>
        constexpr auto error_or(N&& ptr, E&& err); 
                
    template <class N, class E>
        constexpr bool check_error(N&& n, E&& err); 
          
    template <class N, class F>
        constexpr auto resolve(N&& n, F&& f); 
          
}
 
    template <class T>
        struct is_value_or_error;
    template <class T>
        struct is_value_or_error <const T> : is_value_or_error <T> {};
    template <class T>
        struct is_value_or_error <volatile T> : is_value_or_error <T> {};
    template <class T>
        struct is_value_or_error <const volatile T> : is_value_or_error <T> {};
    template <class T>
        struct is_value_or_error <T*> : true_type {};

    template <class T>
        constexpr bool is_value_or_error_v = is_value_or_error <T>::value ;
        
namespace value_or_error {
    template <class X, class T>
        constexpr auto value_or(X&& ptr, T&& val); 
                
    template <class E, class X>
        constexpr auto value_or_throw(X&& ptr); 
                
    template <class X, class E>
        constexpr auto error_or(X&& ptr, E&& err); 
                
    template <class X, class E>
        constexpr bool check_error(X&& n, E&& err); 
          
    template <class X, class F, class T>
        constexpr auto apply_or(X&& n, F&& f, T&& v); 

    template <class X, class F>
        constexpr auto resolve(X&& n, F&& f); 
        
}
        
}
}
```

###  class `traits` [value_or_error.traits]

```c++
namespace value_or_error {
    template <class T, class Enabler=void>
        struct traits {};

    // class traits_pointer_like
    struct traits_pointer_like
    {
        template <class Ptr>
            static constexpr
            bool has_value(Ptr ptr) { return bool(ptr) }
    };

    // class traits specialization for pointers
    template <class T>
        struct traits<T*> 
            : traits_pointer_like<T*>
        {};
} 
```

###  Template function `has_value` [value_or_error.has_value]

```c++
namespace value_or_error {
    template <class T>
        bool has_value(T const& v) noexcept;
    template <class T>
        bool has_value(T* v) noexcept;
}
```

**Adapt the "ValueOrNone Objects" section**

## ValueOrNone Objects


### Header <experimental/value_or_none> synopsis [ValueOrNone.synop]

```c++
namespace std::experimental {
inline namespace fundamentals_v3 {
 
    template <class T>
        struct is_value_or_none;

    template <class T>
        constexpr bool is_value_or_none_v = is_value_or_none <T>::value ;

namespace value_or_none {
    

    // class traits specialization for Nullable and PossiblyValued
    template <class T>
        struct traits<T, when<is_value_or_none_v<T>>>;
        
    // class traits_pointer_like
    struct traits_pointer_like
    {
    };

    // class traits specialization for pointers
    template <class T>
        struct traits<T*> 
            : traits_pointer_like<T*>
        {};
        

}
}
}
```


## Optional Objects

**Add Specialization of *ValueOrNone* [optional.object.possibly_not_valued]**.

20.6.x ValueOrNone

`optional<T>` is a model of *ValueOrNone*.

```c++
namespace value_or_none {
    template <class T>
    struct traits<optional<T>>  : tag 
    {
      template <class U>
      static constexpr
      auto deref(optional<U> const& v) noexcept { return *v; }
    };
}
```   

## Smart Pointers

20.6.x *ValueOrNone*

`unique_ptr<T, D>` is a models of *ValueOrNone*.

```c++
namespace value_or_none {
    template <class T, class D>
    struct traits<unique_ptr<T, D> : traits_pointer_like {};
}
```

`shared_ptr<T>` is a models of *ValueOrNone*.

```c++
namespace value_or_none {
    template <class T>
    struct traits<shared_ptr<T>> : traits_pointer_like {};
}
```

## Expected Objects

**Add Specialization of *SuccessOrFailure* [optional.object.success_or_value]**.


# Implementability

This proposal can be implemented as pure library extension, without any language support, in C++17.

# Open points

The authors would like to have an answer to the following points if there is any interest at all in this proposal:

## File(s) name

Should we include this in `<experimental/functional>` or in a specific file?
We believe that a specific file is a better choice as this is needed in `<experimental/optional>`, `<experimental/any>` and `<experimental/variant>`. We propose to locate each concept in one file `<experimental/valued_or_error>`. 

## About `value_or_error::value(n)`

We could define a wide `value_or_error::value(n)` function on *ValueOrError* that obtain the value or throws an exception. If we want to have a default implementation the function will need to throw a generic exception `bad_access`.

However to preserve the current behavior of `std::optional::value()` we will need to be able to consider this function as a customization point also.

# Future work

## *ValueOrError* as sum type

A *ValueOrError* can be considered as a sum type. It is always useful reflect the related types. `value_or_error::error_type_t` and `value_or_error::value_type_t` give respectively the associated non-a-value and the value types.

## *ValueOrError* as a Functor

While we don't have yet an adopted proposal for *Functor*, we can define a default `value_or_error::transform` function for *ValueOrError* type. 

## *ValueOrError* as an Applicative Functor

While we don't have yet an adopted proposal for *ApplicativeFunctor*, we can define a default `value_or_error::ap` function for *ValueOrError*. 

## *ValueOrError* as a Monad

While we don't have yet an adopted proposal for *Monad*, we can define a default `value_or_error::bind` function for *ValueOrError*. 

## *ValueOrError* as a MonadError

While we don't have yet an adopted proposal for *MonadError*, we can define a default `value_or_error::catch_error` and `value_or_error::make_error` functions for *ValueOrError*. 

# Acknowledgements

Thanks to ...

Special thanks and recognition goes to Technical Center of Nokia - Lannion for supporting in part the production of this proposal.

# History

## Revision 0

* Rollback the modifications of revision 3, as they belong to *ValueOrError* kind of types. 
    * We should move `deref()` to `PossiblyValued`, as `std::any` cannot define it, as it has not a unique value. As consequence the functions added in the previous version `value_or` and `apply_or` should be moved as well.
    * The same applies to the added `visit` function. We cannot visit any *Nullable* as we don't know what are its possibly values. This function belongs to *ValueOrNone* and to *ValueOrError*. *ValueOrNone* is a *Nullable* that has only one possible value type. *ValueOrNone* is specialization of *ValueOrError* where the Error is a unit type.
    * Next follows the concept hierarchy:

        * *PossiblyNotValued* (has_value)
            * *PossiblyValued* (deref) => value_or, apply_or
                * *ValueOrError*  (error) (examples: expected, result => TRY, SumType, Functor, Applicative, Monad, MonadError
                    * *ValueOrNone*  
            * *Nullable* (none) (examples std::variant<none_t, Ts...>, std::any)
                * *ValueOrNone* (examples T*, std::optional, std::unique_ptr, ...)  
                 
    * Move *Functor*, *Applicative* and *Monad* specialization to *ValueOrError*.
    * We could add later the *ValueOrNone* that maps the none type as an error to behave like a *ValueOrError* when there is only a possibly value type and the Error is a unit type.
    * However, we should define *PossiblyNotValued*
     
# References

[N4564]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf "N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS"

[P0032R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0032r0.pdf "Homogeneous interface for variant, any and optional"

[P0050R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0050r0.pdf "C++ generic match function"

[P0088R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf "Variant: a type-safe union that is rarely invalid (v5)"  

[P0091R0]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0091r0.html "Template parameter deduction for constructors (Rev. 3)"

[P0338R2]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0338r2.pdf "C++ generic factories"

[P0343R1]: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0343r1.pdf "Meta-programming High-Order functions"

[P0650R0]: http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0650r0.pdf "C++ Monadic interface"  

[LWG 2510]: http://cplusplus.github.io/LWG/lwg-active.html#2510 "Tag types should not be DefaultConstructible"
        
[CWG 1518]: http://open-std.org/JTC1/SC22/WG21/docs/cwg_active.html#1518 "Explicit default constructors and copy-list-initialization" 
    
[CWG 1630]: http://open-std.org/JTC1/SC22/WG21/docs/cwg_defects.html#1630 "Multiple default constructor templates" 

[SUM_TYPE]: https://github.com/viboes/std-make/tree/master/include/experimental/fundamental/v3/sum_type "Generic Sum Types"
    

* [N4564] N4564 - Working Draft, C++ Extensions for Library Fundamentals, Version 2 PDTS

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4564.pdf

* [P0032R0] Homogeneous interface for variant, any and optional

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0032r0.pdf

* [P0050R0] C++ generic match function

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0050r0.pdf

* [P0088R0] Variant: a type-safe union that is rarely invalid (v5)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0088r0.pdf

* [P0091R0] Template parameter deduction for constructors (Rev. 3)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0091r0.html
    
* [P0338R2] C++ generic factories

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0338r2.pdf

* [P0343R1] - Meta-programming High-Order functions

    http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0343r1.pdf

* [P0650R0] C++ Monadic interface

    http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0650r0.pdf

* [LWG 2510] Tag types should not be DefaultConstructible

    http://cplusplus.github.io/LWG/lwg-active.html#2510
    
* [CWG 1518] Explicit default constructors and copy-list-initialization 

    http://open-std.org/JTC1/SC22/WG21/docs/cwg_active.html#1518

* [CWG 1630] Multiple default constructor templates  

    http://open-std.org/JTC1/SC22/WG21/docs/cwg_defects.html#1630

* [SUM_TYPE] Generic Sum Types

    https://github.com/viboes/std-make/tree/master/include/experimental/fundamental/v3/sum_type

