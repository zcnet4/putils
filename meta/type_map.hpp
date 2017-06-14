#pragma once

#include <tuple>
#include "fwd.hpp"
#include "type.hpp"

namespace pmeta
{
    // See bottom of file for example use
    template<typename Tuple>
    struct type_map
    {
        template<typename Arg>
        type_map(Arg &&tuple) : _tuple(FWD(tuple)) {}

        template<typename Key, typename Func>
        void get_type(Key &&key, Func &&func);

        template<typename T>
        auto get_key();

    private:
        Tuple _tuple;
    };

    template<typename ...KeyValues>
    auto make_type_map(KeyValues &&...keyValues);

    /*
     * Implementation details
     */

    namespace detail
    {
        enum Indexes
        {
            TypeName = 0,
            Type = 1
        };
    }

    /*
     * get_type implementation
     */

    namespace detail
    {
        template<typename Key, typename TypeMap, typename Func, std::size_t ...Is>
        void get_type(Key &&key, TypeMap &&type_map, Func &&func, std::index_sequence<Is...>);

        template<typename Key, typename TypeMap, typename Func>
        void get_type(Key &&key, TypeMap &&type_map, Func &&func, std::index_sequence<>)
        {
            throw std::out_of_range("No such type");
        }

        template<typename Key, typename TypeMap, typename Func, std::size_t I, std::size_t ...Is>
        void get_type(Key &&key, TypeMap &&type_map, Func &&func, std::index_sequence<I, Is...>)
        {
            const auto &pair = std::get<I>(type_map);

            if (std::get<TypeName>(pair) == key)
            {
                func(std::get<Type>(pair));
                return;
            }

            get_type(key, type_map, func, std::index_sequence<Is...>());
        }

        template<typename Key, typename Func, typename ...Pairs>
        void get_type(Key &&key, const std::tuple<Pairs...> &type_map, Func &&func)
        {
            get_type(key, type_map, func, std::index_sequence_for<Pairs...>());
        }
    }

    template<typename Tuple>
    template<typename Key, typename Func>
    void type_map<Tuple>::get_type(Key &&key, Func &&func)
    {
        detail::get_type(key, _tuple, func);
    }

    /*
     * get_key implementation
     */

    namespace detail
    {
        template<typename TypeMap, std::size_t ...Is>
        auto get_key(TypeMap &&type_map, std::index_sequence<Is...>);

        template<typename T, typename TypeMap, std::size_t I>
        auto get_key(TypeMap &&type_map, std::index_sequence<I>)
        {
            const auto &pair = std::get<I>(type_map);

            using WrapperType = std::tuple_element_t<Type, std::decay_t<decltype(pair)>>;
            using ObjectType = typename WrapperType::wrapped;

            if (std::is_same<ObjectType, T>::value)
                return std::get<TypeName>(pair);

            throw std::out_of_range("No such type");
        }

        template<typename T, typename TypeMap, std::size_t I, std::size_t ...Is>
        auto get_key(TypeMap &&type_map, std::index_sequence<I, Is...>)
        {
            const auto &pair = std::get<I>(type_map);

            using WrapperType = std::tuple_element_t<Type, std::decay_t<decltype(pair)>>;
            using ObjectType = typename WrapperType::wrapped;

            if (std::is_same<ObjectType, T>::value)
                return std::get<TypeName>(pair);

            return get_key<T>(type_map, std::index_sequence<Is...>());
        };

        template<typename T, typename ...Pairs>
        auto get_key(const std::tuple<Pairs...> &type_map)
        {
            return get_key<T>(type_map, std::index_sequence_for<Pairs...>());
        }
    }

    template<typename Tuple>
    template<typename T>
    auto type_map<Tuple>::get_key()
    {
        return detail::get_key<T>(_tuple);
    }

    /*
     * make_type_map implementation
     */

    namespace detail
    {
        inline auto make_type_map() { return std::make_tuple(); }

        template<typename Key, typename Type, typename ...Args>
        auto make_type_map(Key &&key, Type &&type, Args &&...args)
        {
            return std::tuple_cat(
                    std::make_tuple(
                            std::make_tuple(FWD(key), FWD(type))
                    ),
                    detail::make_type_map(FWD(args)...)
            );
        }
    }

    template<typename ...Args>
    auto make_type_map(Args &&...args)
    {
        return type_map<decltype(detail::make_type_map(FWD(args)...))>(
            detail::make_type_map(FWD(args)...)
        );
    }

    /*
     * Example
     */

    namespace test
    {
        inline void type_map()
        {
            auto types = pmeta::make_type_map(
                    "string", pmeta::type<std::string>(),
                    "int", pmeta::type<int>(),
                    "float", pmeta::type<float>()
            );

            // Get a key from type
            std::cout << types.get_key<std::string>() << std::endl;

            // Get a type from key
            types.get_type("int",
                    [](auto &&t)
                    {
                        using Type = pmeta_wrapped(t);
                        std::cout << typeid(Type).name() << std::endl;
                    }
            );
        }
    }
}
