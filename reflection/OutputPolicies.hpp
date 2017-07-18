#pragma once

#include <cctype>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <map>

#include "meta/type.hpp"
#include "to_string.hpp"
#include "json.hpp"
#include "meta/for_each.hpp"

namespace putils
{
    namespace OutputPolicies
    {
        struct Json
        {
            static void startSerialize(std::ostream &s) { s << '{'; }
            static void endSerialize(std::ostream &s) { s << '}'; }
            static void serializeNewField(std::ostream &s) { s << ", "; }

            template<typename T, typename Tuple>
            static void unserialize(std::istream &s, T &obj, const Tuple &tuple)
            {
                std::size_t openBraces = 0;
                std::string str;

                while (s)
                {
                    const char c = s.get();
                    if (c == '{' || c == '[')
                        ++openBraces;
                    else if (c == '}' || c == ']')
                        --openBraces;

                    str.append(1, c);

                    if (!openBraces)
                        break;
                }

                const auto jsonObject = putils::json::lex(str);

                pmeta::tuple_for_each(tuple, [&s, &obj, &jsonObject](const auto &attr)
                {
                    std::stringstream stream(jsonObject[attr.first]);
                    unserialize(stream, obj.*(attr.second));
                });
            };

            /*
             * Bools
             */

            static void serialize(std::ostream &s, std::string_view name, bool value)
            { s << name << ": " << std::boolalpha << value << std::noboolalpha; }

            static void unserialize(std::istream &s, bool &attr)
            {
/*
                while (std::isspace(s.peek()))
                    s.get();
                while (s.get() != ':');
                while (std::isspace(s.peek()))
                    s.get();
*/

                std::string value;
                while (s.peek() != ',' && s.peek() != '}')
                {
                    const char c = s.get();
                    if (c == '\\')
                        value.append(1, s.get());
                    else
                        value.append(1, c);
                }

                std::stringstream stream;
                stream << std::boolalpha << putils::chop(value);
                stream >> attr;
            }

            /*
             * Maps
             */

            template<typename Map>
            static void printMap(std::ostream &s, std::string_view name, Map &&map)
            {
                s << name << ": {";
                bool first = true;
                for (const auto &pair : map)
                {
                    if (!first)
                        s << ", ";

                    serialize(s, putils::to_string(pair.first), pair.second);
                    first = false;
                }
                s << "}";
            }

            template<typename Key, typename Value, typename Map>
            static void unserializeMap(std::istream &s, Map &map)
            {
                map.clear();
/*
                while (std::isspace(s.peek()))
                    s.get();
                while (s && s.get() != ':');
                while (std::isspace(s.peek()))
                    s.get();
 */

                while (s && s.get() != '{');

                if (!s)
                    throw std::runtime_error("Bad format: expected '{'");

                while (s.peek() != '}')
                {
                    while (std::isspace(s.peek()))
                        s.get();

                    std::string key;
                    while (s.peek() != ':' && s.peek() != '}')
                    {
                        const char c = s.get();
                        if (c == '\\')
                            key.append(1, s.get());
                        else
                            key.append(1, c);
                    }

                    Key k;
                    {
                        std::stringstream tmp(key);
                        unserialize(tmp, k);
                    }

                    if (s.get() != ':')
                        throw std::runtime_error("Bad format: expected ':'");

                    while (std::isspace(s.peek()))
                        s.get();

                    std::string value;
                    while (s.peek() != ',' && s.peek() != '}')
                    {
                        const char c = s.get();
                        if (c == '\\')
                            value.append(1, s.get());
                        else
                            value.append(1, c);
                    }

                    if (s.peek() == ',')
                        s.get();

                    {
                        std::stringstream tmp(value);
                        unserialize(tmp, map[k]);
                    }
                }
            }

            template<typename Key, typename Value>
            static void serialize(std::ostream &s, std::string_view name,
                                  const std::unordered_map<Key, Value> &map)
            { printMap(s, name, map); }
            template<typename Key, typename Value>
            static void unserialize(std::istream &s, std::unordered_map<Key, Value> &map)
            { unserializeMap<Key, Value>(s, map); }


            template<typename Key, typename Value>
            static void serialize(std::ostream &s, std::string_view name,
                                  const std::map<Key, Value> &map)
            { printMap(s, name, map); }
            template<typename Key, typename Value>
            static void unserialize(std::istream &s, std::map<Key, Value> &map)
            { unserializeMap<Key, Value>(s, map); }

            /*
             * Ptrs
             */

            template<typename Ptr>
            static void printPtr(std::ostream &s, std::string_view name, Ptr &&ptr)
            { serialize(s, name, *ptr); }

            template<typename Ptr>
            static void unserializePtr(std::istream &s, Ptr &&ptr)
            { unserialize(s, *ptr); }

            template<typename T>
            static void serialize(std::ostream &s, std::string_view name, const std::unique_ptr<T> &ptr) { printPtr(s, name, ptr); }
            template<typename T>
            static void unserialize(std::istream &s, std::unique_ptr<T> &ptr)
            {
                ptr.reset(new T);
                unserializePtr(s, ptr);
            }

            template<typename T>
            static void serialize(std::ostream &s, std::string_view name, const std::shared_ptr<T> &ptr) { printPtr(s, name, ptr); }
            template<typename T>
            static void unserialize(std::istream &s, std::shared_ptr<T> &ptr)
            {
                ptr.reset(new T);
                unserializePtr(s, ptr);
            }

            /*
             * Containers
             */

            template<typename Container>
            static void printContainer(std::ostream &s, std::string_view name, const Container &container)
            {
                s << name << ": [";
                bool first = true;
                for (const auto &val : container)
                {
                    if (!first)
                        s << ",";
                    s << val;
                    first = false;
                }

                s << "]";
            }

            template<template<typename, typename> typename Container, typename T, typename _>
            static void unserializeContainer(std::istream &s, Container<T, _> &attr)
            {
                attr.clear();

/*
                while (std::isspace(s.peek()))
                    s.get();
                while (s.get() != ':');
                while (std::isspace(s.peek()))
                    s.get();
*/

                while (s.get() != '[');
                while (s.peek() != ']')
                {
                    T obj;
                    unserializeContainerElement(s, obj);
                    attr.push_back(obj);
                    while (s.peek() != ']' && s.get() != ',');
                }

                s.get();
            }

            template<typename T>
            static void unserializeContainerElement(std::istream &s, T &attr)
            {
                std::string value;
                while (s.peek() != ',' && s.peek() != ']')
                {
                    const char c = s.get();
                    if (c == '\\')
                        value.append(1, s.get());
                    else
                        value.append(1, c);
                }
                std::stringstream(putils::chop(value)) >> attr;
            }

            template<typename T>
            static void serialize(std::ostream &s, std::string_view name, const std::list<T> &container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(std::istream &s, std::list<T> &attr) { unserializeContainer(s, attr); }

            template<typename T>
            static void serialize(std::ostream &s, std::string_view name, const std::vector<T> &container) { printContainer(s, name, container); }
            template<typename T>
            static void unserialize(std::istream &s, std::vector<T> &attr) { unserializeContainer(s, attr); }

            /*
             * "Normal" objects
             */

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void serializeImpl(std::ostream &s, std::string name, const T attr)
            {
                s << name << ": " << (int)attr;
            }

            template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
            static void serializeImpl(std::ostream &s, std::string_view name, const T attr)
            {
                printPtr(s, name, attr);
            }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value && !std::is_pointer<T>::value>>
            static void serializeImpl(std::ostream &s, std::string_view name, const T &attr)
            {
                s << name << ": " << attr;
            }

            template<typename T>
            static void serialize(std::ostream &s, std::string_view name, const T &attr)
            {
                serializeImpl(s, name, attr);
/*
                static_if(bool_v<std::is_enum<T>::value>)
                        .then([&s, &name, attr](auto &&)
                        {
                            s << name << ": " << (int)attr;
                        })
                        .else_if(bool_v<std::is_pointer<T>::value>)
                        .then([&s, &name, attr](auto &&)
                        {
                            printPtr(s, name, attr);
                        })
                        .else_([&s, &name, &attr](auto &&)
                        {
                            s << name << ": " << attr;
                        })
                                (attr);
                */
            }

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void unserializeImpl(std::istream &s, T &attr, const char *)
            {
                unserialize(s, (int&)attr);
            }


            template<typename T, typename = std::enable_if_t<std::is_pointer<T>::value>>
            static void unserializeImpl(std::istream &s, T &attr, std::string_view )
            {
                attr = new typename std::remove_pointer<T>::type;
                unserializePtr(s, attr);
            }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value && !std::is_pointer<T>::value>>
            static void unserializeImpl(std::istream &s, T &attr, std::string)
            {
/*
                while (std::isspace(s.peek()))
                    s.get();
                while (s && s.get() != ':');
                while (std::isspace(s.peek()))
                    s.get();
*/

                std::string value;
                std::size_t openBraces = 0;
                while (s && s.peek() != -1) // && s.peek() != ',' && s.peek() != '}')
                {
                    if (s.peek() == '{')
                        ++openBraces;
                    else if (s.peek() == '}')
                    {
                        if (!openBraces)
                            break;
                        else
                            --openBraces;
                    }
                    else if (s.peek() == ',' && !openBraces)
                        break;

                    const char c = s.get();
                    if (c == '\\')
                        value.append(1, s.get());
                    else
                        value.append(1, c);
                }

                std::stringstream(putils::chop(value)) >> attr;
            }

            template<typename T>
            static void unserialize(std::istream &s, T &attr)
            {
                unserializeImpl(s, attr, "");

/*
                static_if(bool_v<std::is_enum<T>::value>)

                        .then([&s, &name, &attr](auto &&)
                        {
                            unserialize(s, name, (int&)attr);
                        })

                        .else_if(bool_v<std::is_pointer<T>::value>)

                        .then([&s, &name, &attr](auto &&)
                        {
                            attr = new typename std::remove_pointer<T>::type;
                            unserializePtr(s, name, attr);
                        })

                        .else_([&s, &name, &attr](auto &&)
                        {
                            while (std::isspace(s.peek()))
                                s.get();
                            while (s.get() != ':');
                            while (std::isspace(s.peek()))
                                s.get();

                            std::string value;
                            while (s.peek() != ',' && s.peek() != '}')
                            {
                                const char c = s.get();
                                if (c == '\\')
                                    value.append(1, s.get());
                                else
                                    value.append(1, c);
                            }

                            std::stringstream(putils::chop(value)) >> attr;
                        })
                                (attr);
*/

            }
        };


        struct Default
        {
            static void startSerialize(std::ostream &s) {}
            static void endSerialize(std::ostream &s) {}
            static void serializeNewField(std::ostream &s) {}

            template<typename T, typename Tuple>
            static void unserialize(std::istream &s, T &obj, const Tuple &tuple)
            {
                pmeta::tuple_for_each(tuple, [&s, &obj](const auto &attr)
                {
                    try
                    {
                        unserialize(s, obj.*(attr.second));
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
                });
            };

            template<typename T>
            static void serialize(std::ostream &, std::string_view , const std::unique_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::unique_ptr<T> &) {}

            template<typename T>
            static void serialize(std::ostream &, std::string_view , const std::shared_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::shared_ptr<T> &) {}

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void serialize(std::ostream &s, std::string_view name, const T &attr) { s << attr << " "; }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void unserialize(std::istream &s, T &attr) { s >> attr; }

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void serialize(std::ostream &s, std::string_view name, T attr) { s << (int)attr << " "; };

            template<typename T, typename = std::enable_if_t<std::is_enum<T>::value>>
            static void unserialize(std::istream &s, const T &attr) { s >> (int&)attr; }
        };

        struct Binary
        {
            static void startSerialize(std::ostream &s) {}
            static void endSerialize(std::ostream &s) {}
            static void serializeNewField(std::ostream &s) {}

            template<typename T, typename Tuple>
            static void unserialize(std::istream &s, T &obj, const Tuple &tuple)
            {
                pmeta::tuple_for_each(tuple, [&s, &obj](const auto &attr)
                {
                    try
                    {
                        unserialize(s, obj.*(attr.second));
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << e.what() << std::endl;
                    }
                });
            };

            template<typename T>
            static void serialize(std::ostream &, std::string_view , const std::unique_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::unique_ptr<T> &) {}

            template<typename T>
            static void serialize(std::ostream &, std::string_view , const std::shared_ptr<T> &) {}

            template<typename T>
            static void unserialize(std::istream &, std::shared_ptr<T> &) {}

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void serialize(std::ostream &s, std::string_view name, const T &attr) { s.write(&attr, sizeof(attr)); }

            template<typename T, typename = std::enable_if_t<!std::is_enum<T>::value>>
            static void unserialize(std::istream &s, T &attr) { s.read(&attr, sizeof(attr)); }
        };

        struct None
        {
            static void startSerialize(std::ostream &s) {}
            static void endSerialize(std::ostream &s) {}
            static void serializeNewField(std::ostream &s) {}
            template<typename T, typename Tuple>
            static void unserialize(std::istream &s, T &obj, const Tuple &tuple) {}
            template<typename Auto>
            static void serialize(std::ostream &, std::string_view , const Auto &) {}
            template<typename Auto>
            static void unserialize(std::istream &, Auto &) {}
        };
    }
}