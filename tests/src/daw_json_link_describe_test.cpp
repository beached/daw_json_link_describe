// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/daw_json_link_describe
//

#include <daw/json/daw_json_link_describe.h>

#include <boost/describe.hpp>
#include <cassert>
#include <iostream>

struct X {
	int m1;
	int m2;
};
BOOST_DESCRIBE_STRUCT( X, ( ), ( m1, m2 ) )
template<>
inline constexpr bool daw::json::use_boost_describe_v<X> = true;

int main( ) {
	daw::string_view json_doc = R"json(
{
	"m1": 55,
	"m2": 123
}
)json";
	auto val = daw::json::from_json<X>( json_doc );
	assert( val.m1 == 55 );
	assert( val.m2 == 123 );
	std::cout << "json: " << daw::json::to_json( val ) << '\n';
}
