#pragma once

#include <string>
#include <Ar/Middleware/ActiveThreadCore.h>
#include <Ar/Middleware/ActiveObject.h>
#include <Ar/Middleware/SharedPtr.h>
#include <Ar/Middleware/Logger.h>
#include <functional>
#include <map>

namespace Ar {
    namespace Middleware
    {
        class ActiveThread
        {
            class MessageExecutorBase
            {
            public:
                virtual ~MessageExecutorBase() {}
                virtual void execute(IMessage *) = 0;
            };

            template <typename M>
            class MessageExecutor : public MessageExecutorBase
            {
            public:
                MessageExecutor(std::function<void(M*)> function)
                    : _function(function)
                {}

                virtual void execute(IMessage *message)
                {
                    auto m = message->castTo<M>();
                    _function(m);
                }

            private:
                std::function<void(M*)> _function;
            };

        public:
            friend class ActiveThreadCore;
            friend class ActiveObject;

            static SharedPtr<ActiveThread> create();

            ActiveThread();
            ~ActiveThread();

            void start(const std::string &name);
            void stop();
            bool sendTo(const std::string &atName, IMessage *message);
            bool sendTo(ActiveThread *at, IMessage *message);
            bool executeWithin(const std::string &atName, std::function<void()> lambda);
            bool executeWithin(ActiveThread *at, std::function<void()> lambda);

            template <typename R>
            void waitForResponseFor(std::function<void(typename R::Response*)> success);
            template <typename R>
            void waitForResponseFor(std::function<void(typename R::Response*)> success, std::function<void()> noResponse);

            template <typename M>
            void registerReceiverForMessage(std::function<void(M*)> f);

            template <typename T, typename M>
            void registerReceiverForMessage(T *ao, void (T::*m)(M*));

            const std::string& name() const;
            
        protected:
            void initializeActiveObject(ActiveObject *ao);
            void sendToMe(IMessage *message);
            SharedPtr<ActiveThreadCore>& core();
            bool dispatch(IMessage *message);
            Logger& log();

        private:
            std::string                                         _name;
            SharedPtr<ActiveThreadCore>                         _core;
            Logger                                              _log;
            std::map<MessageId, SharedPtr<MessageExecutorBase>> _receivers;
        };

        // IMPLEMENTATION
        template <typename R>
        inline void ActiveThread::waitForResponseFor(std::function<void(typename R::Response*)> success)
        {
            waitForResponseFor<R>(success, [](){});
        }

        template <typename R>
        inline void ActiveThread::waitForResponseFor(std::function<void(typename R::Response*)> success, std::function<void()> noResponse)
        {
            auto message = core()->waitFor(R::Response::MSG_ID());
            if(message)
            {
                success(message->template castTo<typename R::Response>());
                safeDel(message);
            }
            else
            {
                noResponse();
            }
        }

        template <typename M>
        inline void ActiveThread::registerReceiverForMessage(std::function<void(M*)> f)
        {
            _receivers[M::MSG_ID()] = safeNew<MessageExecutor<M>>(f);
        }

        template <typename T, typename M>
        inline void ActiveThread::registerReceiverForMessage(T *ao, void (T::*m)(M*))
        {
            static_assert(std::is_base_of<ActiveObject, T>::value, "Could not register: T is not an ActiveObject!");
            registerReceiverForMessage<M>
                    (std::bind(m, ao, std::placeholders::_1));
        }

        inline const std::string & ActiveThread::name() const
        {
            return _name;
        }

        inline SharedPtr<ActiveThreadCore>& ActiveThread::core()
        {
            return _core;
        }

        inline Logger& ActiveThread::log()
        {
            return _log;
        }
    }
}
