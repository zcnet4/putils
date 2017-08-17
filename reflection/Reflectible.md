# Reflectible

Template class that should be inherited by any type wanting to expose its members.

### Reflection methods

These should be defined by all types inheriting from Reflectible. This is enforced by Reflectible, and client code will not compile unless these are defined.

##### get_class_name

```
static const auto get_class_name();
```
Returns the class name.

##### get_attributes

```
static const auto &get_attributes();
```
Returns a table mapping strings to pointers to the attributes.

##### get_methods

```
static const auto &get_methods();
```
Returns a table mapping strings to pointers to the methods.

##### get_parents
```
static const auto &get_parents();
```
Returns a table mapping strings to `pmeta::type` objects for each of the class' parents.

### type trait

```
template<typename T>
using is_reflectible = std::is_base_of<Reflectible<T>, T>;
```

Lets client code check whether a given-type is reflectible.

### Helper macros

##### pmeta_reflectible_attribute(memberPtr)

Takes a member pointer as parameter and generates a pair of parameters under the form `"var", &Class::var` to avoid redundancy when passing parameters to `pmeta::make_table`. For instance:

```
const auto table = pmeta::make_table(
    "x", &Point::x,
    "y", &Point::y
);
```

can be refactored to:

```
const auto table = pmeta::make_table(
    pmeta_reflectible_attribute(&Point::x),
    pmeta_reflectible_attribute(&Point::y)
);
```

##### pmeta_reflectible_attribute_private(memberPtr)

Provides the same functionality as `pmeta_reflectible_attribute`, but skips the first character of the attribute's name (such as an `_` or `m`) that would mark a private member. For instance:

```
const auto table = pmeta::make_table(
    "name", &Human::_name,
    "age", &Human::_age
);
```

can be refactored to:

```
const auto table = pmeta::make_table(
    pmeta_reflectible_attribute_private(&Human::_name),
    pmeta_reflectible_attribute_private(&Human::_age)
);
```

### Example

```
class ExampleParent
{
};

class Test : public Reflectible<Test>, public ExampleParent
{
public:
    Test()
    {
    }

public:
    std::string exampleMethod() const { return "Method"; }

private:
    std::string _exampleAttribute = "Attribute";

public:
    static const auto get_class_name() { return "Test"; }

    static const auto &get_attributes()
    {
        static const auto table = pmeta::make_table(
                pmeta_reflectible_attribute_private(&Test::_exampleAttribute)
        );
        return table;
    }

    static const auto &get_methods()
    {
        static const auto table = pmeta::make_table(
                pmeta_reflectible_attribute(&Test::exampleMethod)
        );
        return table;
    }

    static const auto &get_parents()
    {
        static const auto table = pmeta::make_table(
                pmeta_reflectible_parent(ExampleParent)
        );
        return table;
    }
};

int main()
{
    Test t;

    // Walk attributes
    pmeta::tuple_for_each(Test::get_attributes().getKeyValues(),
                          [&t](auto &&pair)
                          {
                              const auto &name = std::get<0>(pair);
                              const auto ptr = std::get<1>(pair);
                              std::cout << name << ": " << t.*ptr << std::endl;
                          }
    );

    // Walk methods
    pmeta::tuple_for_each(Test::get_methods().getKeyValues(),
                          [&t](auto &&pair)
                          {
                              const auto &name = std::get<0>(pair);
                              const auto ptr = std::get<1>(pair);
                              std::cout << name << ": " << (t.*ptr)() << std::endl;
                          }
    );

    // Walk parents
     pmeta::tuple_for_each(Test::get_parents().getKeyValues(),
                          [&t](auto &&pair)
                          {
                              const auto &name = std::get<0>(pair);
                              using ParentType = pmeta_wrapped(std::get<1>(pair));
                              std::cout << name << ": " << typeid(ParentType).name() << std::endl;
                          }
    );
}
```
