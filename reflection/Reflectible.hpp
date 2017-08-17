#pragma once

#include <atomic>
#include <string>
#include <string_view>

#include "meta/table.hpp"
#include "meta/nameof.hpp"

#define pmeta_reflectible_attribute(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).find("::") + 2), memberPtr
#define pmeta_reflectible_attribute_private(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).find("::") + 3), memberPtr
#define pmeta_reflectible_parent(T) pmeta_nameof(type), pmeta::type<T>()

namespace putils
{
    template<typename CRTP>
    struct Reflectible
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

            CRTP::get_class_name();
            CRTP::get_attributes();
            CRTP::get_methods();
            CRTP::get_parents();
        }
    };

/*
    template<typename T>
    using is_reflectible = std::is_base_of<Reflectible<T>, T>;
*/

    template<typename T>
    class is_reflectible
    {
        template<typename TT>
        static auto test(int)
                -> decltype(T::get_class_name(), T::get_attributes(), T::get_methods(), T::get_parents(), std::true_type());

        template<typename TT>
        static auto test(...) -> std::false_type;

    public:
        static constexpr bool value = decltype(test<T>(0))::value;
    };

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
