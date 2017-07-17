# Serializable

Mix-in class letting any derived class serialize its attributes with great simplicity.

By default, serializes objects as JSON. Can be easily customized with policies.

### Members

##### Constructor

```
Serializable()
```

A default constructor is available if the CRTP class is [Reflecitble](Reflectible.md).

```
Serializable(Fields &&...attrs)
```

Takes as arguments the various attributes which should be serialized, paired with their names (see `Example`).

##### serialize

```
void serialize(std::ostream &s) const;
std::ostream &operator<<(std::ostream &s, const putils::Serializable<Derived, OutputPolicy> &obj)
```

##### unserialize

```
void unserialize(std::istream &s) noexcept override
std::istream &operator>>(std::istream &s, putils::Serializable<Derived, OutputPolicy> &obj)
```

### Example

```
int main()
{
    struct Tmp : public Serializable<Tmp>
    {
        Tmp(int x, int y)
        :
            Serializable(
                std::make_pair("x", &Tmp::_x),
                std::make_pair("y", &Tmp::_y)
            ), _x(x), _y(y)
        {}

        int _x, _y;
    };

    Tmp test(24, 42);
    std::stringstream s;

    test.serialize(std::cout); std::cout << std::endl;
    test.serialize(s);

    test._x = 5; test._y = 5;

    test.serialize(std::cout); std::cout << std::endl;
    test.unserialize(s);
    test.serialize(std::cout); std::cout << std::endl;
}
```

