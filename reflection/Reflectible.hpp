#pragma once

#include <atomic>
#include <string>
#include <string_view>

#include "meta/table.hpp"

namespace putils
{
    template<typename CRTP>
    class Reflectible
    {
    public:
        Reflectible()
        {
            // A reflectible type T should inherit from Reflectible<T> and define
            // `get_attributes`, `get_methods` and `get_parents` static member functions
            // that return a pmeta::table mapping strings to:
            //          `get_attributes`: member pointers to the attributes
            //          `get_methods`: member pointers to the methods
            //          `get_parents`: pmeta::type objects for each parent class

            CRTP::get_attributes();
            CRTP::get_methods();
            CRTP::get_parents();
        }
    };

    template<typename T>
    using is_reflectible = std::is_base_of<Reflectible<T>, T>;

    namespace test
    {
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

        inline void reflectible()
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
    }
}