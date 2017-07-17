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
        static const auto table = pmeta::make_table("exampleAttribute", &Test::_exampleAttribute);
        return table;
    }

    static const auto &get_methods()
    {
        static const auto table = pmeta::make_table("exampleMethod", &Test::exampleMethod);
        return table;
    }

    static const auto &get_parents()
    {
        static const auto table = pmeta::make_table("ExampleParent", pmeta::type<ExampleParent>());
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
