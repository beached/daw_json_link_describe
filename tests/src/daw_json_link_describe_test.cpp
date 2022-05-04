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
BOOST_DESCRIBE_STRUCT( X, ( ), ( m1, m2 ) );

struct Y {
	X m0;
	std::string m1;
};
BOOST_DESCRIBE_STRUCT( Y, ( ), ( m0, m1 ) );

int main( ) {
	constexpr daw::string_view json_doc0 = R"json(
{
	"m1": 55,
	"m2": 123
}
)json";
	constexpr auto val0 = daw::json::from_json<X>( json_doc0 );
	assert( val0.m1 == 55 );
	assert( val0.m2 == 123 );
	std::cout << "json: " << daw::json::to_json( val0 ) << '\n';

	constexpr daw::string_view json_doc1 = R"json(
{
	"m0": { "m1": 55, "m2": 123 },
	"m1": "Hello World!"
}
)json";

	auto const val1 = daw::json::from_json<Y>( json_doc1 );
	assert( val1.m0.m1 == 55 );
	assert( val1.m0.m2 == 123 );
	assert( val1.m1 == "Hello World!" );
	std::cout << "json: " << daw::json::to_json( val1 ) << '\n';
}
