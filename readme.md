Example of how to use Boost.Describe descriptions as a way to map aggregates

See tests/src folder for a working example

Requires C++17 and uses [DAW JSON Link](https://github.com/beached/daw_json_link) v3

To describe a struct, only public members are supported, see the Boost.Describe documentation. It will be similar to

```cpp
struct X {
  int m1;
  int m2;
};
BOOST_DESCRIBE_STRUCT( X, ( ), ( m1, m2 ) )

int main( ) {
  std::string_view json_doc = R"json(
  {
    "m1": 55,
    "m2": 123
  }
  )json";

  auto val = daw::json::from_json<X>( json_doc );
  assert( val.m1 == 55 );
  assert( val.m2 == 123 );
  puts( daw::json::to_json( val ).c_str( ) );
}
```

The specialization of `daw::json::use_boost_describe` is necessary. After this from_json and to_json will work with the
mapped type
