// Copyright (c) 2016 Antony Polukhin
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PFR_DETAIL_SEQUENCE_TUPLE_HPP
#define BOOST_PFR_DETAIL_SEQUENCE_TUPLE_HPP

#if __cplusplus < 201402L
#   error C++14 is required for this header.
#endif

#include <utility>      // metaprogramming stuff
#include <cstddef>      // std::size_t

///////////////////// Tuple that holds it's values in the supplied order
namespace boost { namespace pfr { namespace detail { namespace sequence_tuple {

template <std::size_t N, class T>
struct base_from_member {
    T value;
};

template <class I, class ...Tail>
struct tuple_base;



template <std::size_t... I, class ...Tail>
struct tuple_base< std::index_sequence<I...>, Tail... >
    : base_from_member<I , Tail>...
{
    static constexpr std::size_t size_v = sizeof...(I);

    constexpr tuple_base() noexcept = default;
    constexpr tuple_base(tuple_base&&) noexcept = default;
    constexpr tuple_base(const tuple_base&) noexcept = default;

    constexpr tuple_base(Tail... v) noexcept
        : base_from_member<I, Tail>{ v }...
    {}
};

template <>
struct tuple_base<std::index_sequence<> > {
    static constexpr std::size_t size_v = 0;
};

template <std::size_t N, class T>
constexpr T& get_impl(base_from_member<N, T>& t) noexcept {
    return t.value;
}

template <std::size_t N, class T>
constexpr const T& get_impl(const base_from_member<N, T>& t) noexcept {
    return t.value;
}

template <std::size_t N, class T>
constexpr volatile T& get_impl(volatile base_from_member<N, T>& t) noexcept {
    return t.value;
}

template <std::size_t N, class T>
constexpr const volatile T& get_impl(const volatile base_from_member<N, T>& t) noexcept {
    return t.value;
}

template <std::size_t N, class T>
constexpr T&& get_impl(base_from_member<N, T>&& t) noexcept {
    return std::forward<T>(t.value);
}


template <class ...Values>
struct tuple: tuple_base<
    std::make_index_sequence<sizeof...(Values)>,
    Values...>
{
    using tuple_base<
        std::make_index_sequence<sizeof...(Values)>,
        Values...
    >::tuple_base;
};


template <std::size_t N, class ...T>
constexpr decltype(auto) get(tuple<T...>& t) noexcept {
    static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(t);
}

template <std::size_t N, class ...T>
constexpr decltype(auto) get(const tuple<T...>& t) noexcept {
    static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(t);
}

template <std::size_t N, class ...T>
constexpr decltype(auto) get(const volatile tuple<T...>& t) noexcept {
    static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(t);
}

template <std::size_t N, class ...T>
constexpr decltype(auto) get(volatile tuple<T...>& t) noexcept {
    static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(t);
}

template <std::size_t N, class ...T>
constexpr decltype(auto) get(tuple<T...>&& t) noexcept {
    static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
    return get_impl<N>(std::move(t));
}

template <std::size_t I, class T>
using tuple_element = std::remove_reference< decltype(
        ::boost::pfr::detail::sequence_tuple::get<I>( std::declval<T>() )
    ) >;


}}}} // namespace boost::pfr::detail::sequence_tuple

#endif // BOOST_PFR_CORE_HPP
