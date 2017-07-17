# lua

sol, a single header lua encapsulation, along with a few helpers.

* registerType: a template function that, given a `Reflectible` type `T`, registers it by its name, providing all its member functions and attributes, to a sol::state