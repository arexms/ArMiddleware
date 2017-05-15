#pragma once

#include <string>
#include <list>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <Ar/Middleware/IMessage.h>
#include <Ar/Middleware/SharedPtr.h>
#include <Ar/Middleware/Logger.h>

namespace Ar {
    namespace Middleware
    {
        class ActiveThread;

        class ActiveThreadCore
        {
            typedef std::list<IMessage*> Messages;
        public:
            ActiveThreadCore(ActiveThread *at);
            virtual ~ActiveThreadCore();
            void start();
            void stop();
            bool sendToMe(IMessage *message);
            virtual IMessage* waitFor(MessageId id);

        protected:
            virtual void run();
            virtual bool push(IMessage *message);
            virtual IMessage* pop();
            virtual bool dispatch(IMessage *message);
            virtual Messages::iterator find(MessageId id);
            virtual void releaseAllMessages();
            ActiveThread* at();
            Logger& log();

        private:
            ActiveThread                        *_at;
            Messages                            _messages;
            SharedPtr<std::thread>              _thread;
            bool                                _stop;
            mutable std::mutex                  _mutex;
            mutable std::condition_variable     _condMsg;
        };

        // IMPLEMENTATION
        inline ActiveThread* ActiveThreadCore::at()
        {
            return _at;
        }
    }
}
