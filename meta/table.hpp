#pragma once

#include <tuple>
#include "fwd.hpp"
#include "type.hpp"
#include "for_each.hpp"

namespace pmeta
{
    // See bottom of file for example use
    template<typename Tuple, typename FlatTuple>
    struct table
    {
        table(Tuple &&tuple, FlatTuple &&flatTuple)
                : _tuple(FWD(tuple)), _flatTuple(FWD(flatTuple))
        {}

        template<typename Key, typename Func>
        void get_value(Key &&key, Func &&func);

        template<typename Value, typename Func>
        void get_key(Value &&value, Func &&func);

        const Tuple &getKeyValues() const { return _tuple; }
        const FlatTuple &getFlatKeyValues() const { return _flatTuple; }

    private:
        Tuple _tuple;
        FlatTuple _flatTuple;
    };

    template<typename ...KeyValues>
    auto make_table(KeyValues &&...keyValues);

    /*
     * Implementation details
     */

    namespace detail
    {
        enum Indexes
        {
            KeyPos = 0,
            ValuePos = 1
        };
    }

    /*
     * get_value implementation
     */

    namespace detail
    {
        template<std::size_t KPos, std::size_t VPos, typename Key, typename Table, typename Func, std::size_t ...Is>
        void get_value(Key &&key, Table &&table, Func &&func, std::index_sequence<Is...>);

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Table, typename Func>
        void get_value(Key &&key, Table &&table, Func &&func, std::index_sequence<>)
        {
            throw std::out_of_range("Not found");
        }

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Table, typename Func, std::size_t I, std::size_t ...Is>
        void get_value(Key &&key, Table &&table, Func &&func, std::index_sequence<I, Is...>)
        {
            const auto &pair = std::get<I>(table);

            using KeyType = pmeta_typeof(std::get<KPos>(pair));
            if constexpr (std::is_same<KeyType, pmeta_typeof(key)>::value)
            {
                            if (std::get<KPos>(pair) == key)
                            {
                                func(std::get<VPos>(pair));
                                return;
                            }
            }

            get_value<KPos, VPos>(key, table, func, std::index_sequence<Is...>());
        }

        template<std::size_t KPos, std::size_t VPos, typename Key, typename Func, typename ...Pairs>
        void get_value(Key &&key, const std::tuple<Pairs...> &table, Func &&func)
        {
            get_value<KPos, VPos>(FWD(key), table, FWD(func), std::index_sequence_for<Pairs...>());
        }
    }

    template<typename Tuple, typename FlatTuple>
    template<typename Key, typename Func>
    void table<Tuple, FlatTuple>::get_value(Key &&key, Func &&func)
    {
        detail::get_value<detail::KeyPos, detail::ValuePos>(FWD(key), _tuple, FWD(func));
    }

    template<typename Tuple, typename FlatTuple>
    template<typename Value, typename Func>
    void table<Tuple, FlatTuple>::get_key(Value &&value, Func &&func)
    {
        detail::get_value<detail::ValuePos, detail::KeyPos>(FWD(value), _tuple, FWD(func));
    }

    /*
     * make_table implementation
     */

    namespace detail
    {
        inline auto make_table() { return std::make_tuple(); }

        template<typename Key, typename Value, typename ...Args>
        auto make_table(Key &&key, Value &&type, Args &&...args)
        {
            return std::tuple_cat(
                    std::make_tuple(
                            std::make_pair(FWD(key), FWD(type))
                    ),
                    detail::make_table(FWD(args)...)
            );
        }
    }

    template<typename ...Args>
    auto make_table(Args &&...args)
    {
        return table<decltype(detail::make_table(args...)), decltype(std::make_tuple(FWD(args)...))>(
            detail::make_table(args...),
            std::make_tuple(FWD(args)...)
        );
    }

    template<typename ...Args>
    auto make_type_map(Args &&...args) { return make_table(FWD(args)...); }

    /*
     * Example
     */

    namespace test
    {
        inline void table()
        {
            /*
             * Type map
             */

            auto types = pmeta::make_table(
                    "string", pmeta::type<std::string>(),
                    "int", pmeta::type<int>(),
                    "float", pmeta::type<float>()
            );

            // Get a key from type
            types.get_key(pmeta::type<std::string>(),
                          [](auto &&value)
                          {
                              std::cout << value << std::endl;
                          });

            // Get a type from key
            types.get_value("int",
                    [](auto &&t)
                    {
                        using Type = pmeta_wrapped(t);
                        std::cout << typeid(Type).name() << std::endl;
                    }
            );

            /*
             * Any table
             */

            auto table = pmeta::make_table(
                    "42", 42,
                    12, "Hi there"
            );

            table.get_value("42", [](auto &&value) { std::cout << value << std::endl; });
            table.get_key(42, [](auto &&value) { std::cout << value << std::endl; });
            table.get_value(12, [](auto &&value) { std::cout << value << std::endl; });
            table.get_key("Hi there", [](auto &&value) { std::cout << value << std::endl; });
        }
    }
}
