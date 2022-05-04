# Copyright (c) Darrell Wright
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/beached/daw_json_link_describe
#

add_library(daw_json_link_describe_test_lib INTERFACE)
target_link_libraries(daw_json_link_describe_test_lib INTERFACE daw::daw-json-link-describe )
target_compile_options(daw_json_link_describe_test_lib INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/permissive->)

add_executable(daw_json_link_describe_bin src/daw_json_link_describe_test.cpp)
target_link_libraries(daw_json_link_describe_bin PRIVATE daw_json_link_describe_test_lib)
add_test(NAME daw_json_link_describe_test COMMAND daw_json_link_describe_bin)

