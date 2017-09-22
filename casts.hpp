#pragma once

#include <memory>
#include "runTests.hpp"

namespace putils
{
    template<typename Derived, typename Base>
    std::unique_ptr<Derived> static_unique_ptr_cast(std::unique_ptr<Base> &&p);

    template<typename Derived, typename Base>
    std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> &&p);

    /*
     * Implementation details
     */

    template<typename Derived, typename Base>
    std::unique_ptr<Derived> static_unique_ptr_cast(std::unique_ptr<Base> &&p)
    {
        auto d = static_cast<Derived *>(p.release());
        return std::unique_ptr<Derived>(d);
    }

    template<typename Derived, typename Base>
    std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> &&p)
    {
        auto result = dynamic_cast<Derived *>(p.get());
        if (result != nullptr)
        {
            p.release();
            return std::unique_ptr<Derived>(result);
        }
        return std::unique_ptr<Derived>(nullptr);
    }

    namespace test
    {
        inline bool casts()
        {
            struct A { virtual bool f() { return false; } };
            struct B : public A { bool f() final { return true; } };

            return putils::runTests(
                    "static_unique_ptr_cast", []
                    {
                        auto ptr = std::unique_ptr<A>(new B);
                        auto child = putils::static_unique_ptr_cast<B>(std::move(ptr));
                        return child->f();
                    },
                    "dynamic_unique_ptr_cast succeeds", []
                    {
                        auto ptr = std::unique_ptr<A>(new B);
                        auto child = putils::dynamic_unique_ptr_cast<B>(std::move(ptr));
                        return child != nullptr;
                    },
                    "dynamic_unique_ptr_cast fails", []
                    {
                        auto ptr = std::make_unique<A>();
                        auto child = putils::dynamic_unique_ptr_cast<B>(std::move(ptr));
                        return child == nullptr;
                    }
            );
        }
    }
}
