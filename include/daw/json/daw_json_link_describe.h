// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_json_link_describe
//

#pragma once

#include <daw/daw_consteval.h>
#include <daw/daw_traits.h>
#include <daw/json/daw_json_link.h>

#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <tuple>

namespace daw::json {
	/// Types that use Boost.Describe need to specialize use_boost_describe_v for their type with a
	/// bool value of true
	template<typename, typename = void>
	inline constexpr bool use_boost_describe_v = false;

	namespace describe_impl {
		template<std::size_t N>
		struct static_string {
			char const value[N];

			template<std::size_t... Is>
			DAW_CONSTEVAL static_string( char const *str, std::index_sequence<Is...> )
			  : value{ str[Is]... } {}
			DAW_CONSTEVAL static_string( char const *str )
			  : static_string( str, std::make_index_sequence<N>{ } ) {}
		};

		template<template<typename...> typename List, typename... Ts>
		inline auto get_member_list( List<Ts...> const &lst ) {
			// This is needed because NTTP/CNTTP's cannot be literals.  Need to copy them to an array
			static constexpr auto names =
			  std::tuple{ static_string<std::char_traits<char>::length( Ts::name ) + 1>( Ts::name )... };
			return [&]<std::size_t... Is>( std::index_sequence<Is...> ) {
				return json_member_list<
				  json_link<std::get<Is>( names ).value,
				            traits::member_type_of_t<DAW_TYPEOF( Ts::pointer )>>...>{ };
			}
			( std::make_index_sequence<sizeof...( Ts )>{ } );
		}

		template<typename T, template<typename...> typename List, typename... Ts>
		constexpr auto make_member_tuple( T const &value, List<Ts...> const & ) {
			return std::forward_as_tuple( value.*Ts::pointer... );
		}
	} // namespace describe_impl

	template<typename T>
	struct json_data_contract<T, std::enable_if_t<use_boost_describe_v<T>>> {
	private:
		using pub_desc_t = boost::describe::describe_members<T, boost::describe::mod_public>;
		using pri_desc_t = boost::describe::describe_members<T, boost::describe::mod_private>;
		using pro_desc_t = boost::describe::describe_members<T, boost::describe::mod_protected>;
		static_assert(
		  boost::mp11::mp_empty<pri_desc_t>::value,
		  "Classes with private member variables are not supported. Do a manual mapping." );
		static_assert(
		  boost::mp11::mp_empty<pro_desc_t>::value,
		  "Classes with protected member variables are not supported. Do a manual mapping." );

	public:
		using type = DAW_TYPEOF( describe_impl::get_member_list( std::declval<pub_desc_t>( ) ) );

		static constexpr auto to_json_data( T const &value ) {
			return describe_impl::make_member_tuple( value, pub_desc_t{ } );
		}
	};

} // namespace daw::json
