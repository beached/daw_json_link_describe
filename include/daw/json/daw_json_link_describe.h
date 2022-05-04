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
	/// bool value of true.  This defaults to on, all described structs, but can be opted-out of if
	/// one wants to do a custom mapping of a Boost.Described struct
	template<typename, typename = void>
	inline constexpr bool use_boost_describe_v = true;

	namespace describe_impl {
		template<typename, typename>
		struct describe_member_impl;

		template<typename T, std::size_t... Is>
		struct describe_member_impl<T, std::index_sequence<Is...>> {
			static constexpr char const name[sizeof...( Is )]{ T::name[Is]... };
			using type = json_link<name, traits::member_type_of_t<DAW_TYPEOF( T::pointer )>>;
		};

		template<typename T>
		using describe_member =
		  describe_member_impl<T,
		                       std::make_index_sequence<std::char_traits<char>::length( T::name ) + 1>>;

		template<typename T>
		using detect_boost_public_member_descriptor_fn =
		  decltype( boost_public_member_descriptor_fn( std::declval<T **>( ) ) );

		template<typename T>
		inline constexpr bool has_boost_describe_description_v =
		  daw::is_detected_v<detect_boost_public_member_descriptor_fn, T>;
	} // namespace describe_impl

	template<typename T>
	struct json_data_contract<T,
	                          std::enable_if_t<describe_impl::has_boost_describe_description_v<T> and
	                                           use_boost_describe_v<T>>> {
	private:
		using pub_desc_t = boost::describe::describe_members<T, boost::describe::mod_public>;
		using pri_desc_t = boost::describe::describe_members<T, boost::describe::mod_private>;
		using pro_desc_t = boost::describe::describe_members<T, boost::describe::mod_protected>;
		static_assert(
		  boost::mp11::mp_empty<pri_desc_t>::value,
		  "Classes with private member variables are not supported. Must use a manual mapping." );
		static_assert(
		  boost::mp11::mp_empty<pro_desc_t>::value,
		  "Classes with protected member variables are not supported. Must use a manual mapping." );

		template<typename U>
		using desc_t = typename describe_impl::describe_member<U>::type;

		template<template<typename...> typename List, typename... Ts>
		static auto generate_member_list( List<Ts...> const & ) -> json_member_list<desc_t<Ts>...>;

		template<template<typename...> typename List, typename... Ts>
		static constexpr auto make_member_tuple( T const &value, List<Ts...> const & ) {
			return std::forward_as_tuple( value.*Ts::pointer... );
		}

	public:
		using type = DAW_TYPEOF( generate_member_list( pub_desc_t{ } ) );

		static constexpr auto to_json_data( T const &value ) {
			return make_member_tuple( value, pub_desc_t{ } );
		}
	};
} // namespace daw::json
