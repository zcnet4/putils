# Meta

Phiste's metaprogramming utilities.

### Classes

* [GenLinearHierarchy](GenLinearHierarchy.md): given a class template `T` and a typelist `List`, generate a new type inheriting from `T` specialized on each type in `List`.
* [type](type.md): object representation of a type
* [type_map](type_map.md): map keys (`std::string`s, `int`s...) to types

### Function-only headers

* apply.hpp: C++14 implementation of `std::apply`
* for_each.hpp: call a function for each element in a tuple, or instantiate a template type and call a static function for each type in a list
* has_type.hpp: type_trait indicating whether a tuple contains a certain type
