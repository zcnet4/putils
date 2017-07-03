#pragma once

#include <iostream>
#include <string>

#include <cstddef>
#include <unordered_map>
#include "BaseModule.hpp"
#include "meta/for_each.hpp"
#include "Mediator.hpp"

namespace kdw { class Command; }

namespace putils
{
    template<typename CRTP, typename ...DataPackets>
    class Module : public virtual BaseModule
    {
    public:
        Module(Mediator *m = nullptr) : BaseModule(m)
        {
            static_assert(std::is_base_of<Module, CRTP>::value, "Module's first template parameter should be its inheriting class");

            pmeta::tuple_for_each(std::tuple<pmeta::type<DataPackets>...>(),
                    [this](auto &&type)
                    {
                        this->setHandler<pmeta_wrapped(type)>(
                                [this](const auto &p) { static_cast<CRTP*>(this)->handle(p); }
                        );
                    });
        }

    private:
        template<typename Data>
        void setHandler(const std::function<void(const Data &)> &handler)
        {
            _handlers[pmeta::type<Data>::index] = [this, handler](const ADataPacket &packet)
            {
                handler(static_cast<const putils::DataPacket<Data> &>(packet).data);
            };
        }

    protected:
        template<typename Response, typename Query>
        Response query(Query &&q) // q: DataPacket containing a "putils::BaseModule *sender" field
        {
            return Handler<Response>(getMediator(), FWD(q)).res;
        }

    private:
        template<typename Response>
        struct Handler : Module<Handler<Response>, Response>
        {
            template<typename Q>
            Handler(Mediator *m, Q &&query) : BaseModule(m)
            {
                query.sender = this;
                this->send(FWD(query));
            }

            void handle(const Response &response) { res = response; }

            Response res;
        };

    private:
		const std::vector<pmeta::type_index> &getSubscriptions() const noexcept final
    	{
			static const std::vector<pmeta::type_index> _subs = { pmeta::type<DataPackets>::index... };
			return _subs;
    	}

    	void receive(const putils::ADataPacket &packet) noexcept final
        {
            const auto it = _handlers.find(packet.type);
            if (it != _handlers.end())
                it->second(packet);
        }

    private:
        std::unordered_map<pmeta::type_index, std::function<void(const putils::ADataPacket &)>> _handlers;
    };

    namespace test
    {
        inline void mediator()
        {
            struct ExampleQuery
            {
                std::string data;
                BaseModule *sender;
            };

            struct LogModule : putils::Module<LogModule, std::string, ExampleQuery> // will receive std::strings and ExampleQueries
            {
                void handle(const std::string &packet) const { std::cout << "Received " << packet << std::endl; }

                void handle(const ExampleQuery &packet) const { sendTo(42, packet.sender); }
            };

            struct SenderModule : putils::Module<SenderModule> // will not receive anything
            {
                void doWork() const { send(std::string("hi there!")); }

                void doQuery() { std::cout << "Query result is " << query<int>(ExampleQuery{ "data", nullptr }) << std::endl; }
            };

            Mediator mediator;
            SenderModule sender;
            LogModule log;

            mediator.addModule(&sender);
            mediator.addModule(&log);

            sender.doWork();
            sender.doQuery();
        }
    }
}
