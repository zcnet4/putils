#pragma once

#include <iostream>
#include <memory>
#include "meta/for_each.hpp"
#include "chop.hpp"
#include "fwd.hpp"
#include "OutputPolicies.hpp"

namespace putils
{
    // OutputPolicy: type ressembling DefaultOutputPolicy (above), with a
    // template<T> static int serialize(std::ostream &s, const T &attr) function that will be called to
    // serialize each attribute
    template<typename Derived, bool Unserialize = true, typename OutputPolicy = OutputPolicies::Json>
    class Serializable
    {
        // Serialization implementation detail: pointer to generic tuple which will be serialized
        // Tuple should be given all the attributes to serialize
    private:
        // Base interface for Serializer template
        struct SerializerBase
        {
            virtual ~SerializerBase() {}

            virtual void serialize(const Derived *, std::ostream &s) const noexcept = 0;
            virtual void unserialize(Derived *, std::istream &s) = 0;
        };

        template<typename ...Attrs>
        struct Serializer : SerializerBase
        {
            Serializer(Attrs &&...attrs)
            {
                if (_attrs == nullptr)
                    _attrs = std::make_unique<std::tuple<Attrs...>>(FWD(attrs)...);
            }

            // For each member pointer in _attrs, serialize it
            void serialize(const Derived *obj, std::ostream &s) const noexcept override
            {
                OutputPolicy::startSerialize(s);

                bool first = true;
                pmeta::tuple_for_each(*_attrs, [&s, obj, &first](const auto &attr)
                {
                    if (!first)
                        OutputPolicy::serializeNewField(s);
                    OutputPolicy::serialize(s, attr.first, obj->*(attr.second));
                    first = false;
                });

                OutputPolicy::endSerialize(s);
            }

            void unserializeImpl(putils::Serializable<Derived, true, OutputPolicy> *obj, std::istream &s)
            {
                OutputPolicy::unserialize(s, *static_cast<Derived*>(obj), *_attrs);
            }

            void unserializeImpl(putils::Serializable<Derived, false, OutputPolicy> *, std::istream &) noexcept
            {}

            void unserialize(Derived *obj, std::istream &s) override
            {
                unserializeImpl(obj, s);
            }

            // Static tuple containing the member pointers to be serialized for this class (Derived)
        private:
            static std::unique_ptr<std::tuple<Attrs ...>> _attrs;
        };

        // Constructor
    public:
        template<typename ...Fields> // MemberPointers: std::pair<std::string, Derived::*attr>
        Serializable(Fields &&...attrs)
                : _serializer(new Serializer<Fields...>(FWD(attrs)...)) {}

    public:
        std::ostream &serialize(std::ostream &s) const noexcept
        {
            auto __tmp = static_cast<const Derived *>(this);
            _serializer->serialize(static_cast<const Derived *>(this), s);
            return s;
        }

        std::istream &unserialize(std::istream &s)
        {
            _serializer->unserialize(static_cast<Derived *>(this), s);
            return s;
        }

    public:
        friend std::ostream &operator<<(std::ostream &s, const Derived &obj) noexcept
        {
            return obj.serialize(s);
        }

        friend std::istream &operator>>(std::istream &s, Derived &obj)
        {
            return obj.unserialize(s);
        }

        std::shared_ptr<SerializerBase> _serializer;
    };
}

template<typename Derived, bool Unserialize, typename OutputPolicy>
template<typename ...Attrs>
std::unique_ptr<std::tuple<Attrs...>>    putils::Serializable<Derived, Unserialize, OutputPolicy>::Serializer<Attrs...>::_attrs = nullptr;

namespace putils
{
    namespace test
    {
        inline void serializable()
        {
            struct Tmp : public Serializable<Tmp>
            {
                Tmp(int x, int y,
                    const std::vector<int> &vec, const std::list<int> &list,
                    int *ptr, std::unique_ptr<int> &&unique, const std::shared_ptr<int> &shared,
                    const std::unordered_map<std::string, int> &unordered, const std::map<std::string, int> &map
                )
                        :
                        Serializable(
                                std::make_pair("x", &Tmp::_x),
                                std::make_pair("y", &Tmp::_y),
                                std::make_pair("vec", &Tmp::_vec),
                                std::make_pair("list", &Tmp::_list),
                                std::make_pair("ptr", &Tmp::_ptr),
                                std::make_pair("unique", &Tmp::_unique),
                                std::make_pair("shared", &Tmp::_shared),
                                std::make_pair("unordered", &Tmp::_unordered),
                                std::make_pair("map", &Tmp::_map)
                        ), _x(x), _y(y),
                        _vec(vec), _list(list),
                        _ptr(ptr), _unique(std::move(unique)), _shared(shared),
                        _unordered(unordered), _map(map)
                {}

                int _x, _y;

                std::vector<int> _vec;
                std::list<int> _list;

                int *_ptr;
                std::unique_ptr<int> _unique;
                std::shared_ptr<int> _shared;

                std::unordered_map<std::string, int> _unordered;
                std::map<std::string, int> _map;
            };

            Tmp test(24, 42,
                    { 1, 2, 3 }, { 4, 5, 6 },
                    new int(42), std::make_unique<int>(84), std::make_shared<int>(500),
                    { { "one", 1 }, { "two", 2 }, { "three", 3 } },
                    { { "four", 4 }, { "five", 5 }, { "six", 6 } }
            );

            std::stringstream s;

            std::cout << test << std::endl;
            s << test;

            test._x = 5; test._y = 5;
            test._vec = { 4, 5, 6 }; test._list = { 7, 8, 9 };
            test._ptr = new int(1); test._unique = std::make_unique<int>(2); test._shared = std::make_shared<int>(3);
            test._unordered = { { "four", 4 }, { "five", 5 }, { "six", 6} };
            test._map = { { "one", 1 }, { "two", 2 }, { "three", 3 } };

                    std::cout << test << std::endl;
            s >> test;
            std::cout << test << std::endl;
        }
    }
}
