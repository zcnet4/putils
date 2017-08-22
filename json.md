# [JSON](json.hpp)

JSON lexer and pretty-printer.

### prettyprint
```
std::string prettyprint(auto &&str);
```
Prints JSON-formatted string `str` with pretty alignment.

### Object
```
struct Object
{
    enum class Type
    {
        Value,
        Object,
        Array
    };

    std::string value;
    std::vector<Object> items;
    std::map<std::string, Object> fields;
    Type type;
    // if items.size() > 0, then I'm an array
    // if fields.size() == 0, then I'm a simple value
};
```
Representation for a JSON object, which may be a simple value, an actual object, or an array of objects.

### lex
```
Object lex(auto &&str);
```
Lexes JSON-formatted string `str` and returns a JSON object.
