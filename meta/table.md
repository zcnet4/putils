# type_map

Map types to any object.

### Members

##### Constructor
```
type_map(auto &&tuple);
```
See `Example`.

##### getDetectionType
```
void getDetectionType(auto &&key, auto &&func);
```
Retrieve the type associated to `key` and call `func` by passing it the `pmeta::type` associated to it.
See `Example`.

##### get_key
```
template<typename T>
auto get_key();
```
Returns the key associated to `T`.

### Example

```
int main()
{
    auto types = pmeta::make_type_map(
        "string", pmeta::type<std::string>(),
        "int", pmeta::type<int>(),
        "float", pmeta::type<float>()
    );

    // Get a key from type
    std::cout << types.get_key<std::string>() << std::endl;

    // Get a type from key
    types.getDetectionType("int",
        [](auto &&t)
        {
            using Type = pmeta_wrapped(t);
            std::cout << typeid(Type).name() << std::endl;
        }
    );
}
```