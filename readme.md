Example of how to use Boost.Describe descriptions as a way to map aggregates

See tests/src folder for a working example

Requires C++20

To describe a struct, only public members are supported, see the Boost.Describe documentation.  It will be similar to ```
struct X {
  int m1;
  int m2;
};
BOOST_DESCRIBE_STRUCT( X, ( ), ( m1, m2 ) )
template<>
inline constexpr bool daw::json::use_boost_describe_v<X> = true;
```
The specialization of `daw::json::use_boost_describe` is necessary. After this from_json and to_json will work with the mapped type