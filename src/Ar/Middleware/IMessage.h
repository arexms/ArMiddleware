#pragma once


#include <Ar/Middleware/MessageIds.h>
#include <Ar/Middleware/Utils.h>
#include <string>

namespace Ar {
    namespace Middleware
    {
        class ActiveThread;

        struct IMessage
        {
            struct IResponse;

            static const MessageId MSG_ID = MessageId::BASE;

            IMessage(ActiveThread *from_ = nullptr, ActiveThread *to_ = nullptr)
                : from(from_)
                , to(to_)
            {}

            virtual ~IMessage()
            {}

            virtual MessageId id() = 0;

            template <typename T>
            T* castTo()
            {
                return static_cast<T*>(this);
            }

            virtual IResponse* createResponse() { return nullptr; }

            template <typename T>
            static typename T::Response* createResponse(T *message)
            {
                return safeNew<typename T::Response>(message);
            }

            ActiveThread *from;
            ActiveThread *to;
        };

        struct IMessage::IResponse : IMessage
        {
            IResponse(IMessage *message)
                : IMessage{message->to, message->from}
                , result(0)
            {}

            int result;

        private:
            virtual IResponse* createResponse()
            {
                //assert(false, "execution of createResponse() for Response message is invalid");
                return nullptr;
            }
        };
#define MESSAGE(ID)                             \
    static MessageId MSG_ID() { return ID; }    \
    virtual MessageId id() { return ID; }       \
    virtual Response* createResponse()          \
    {                                           \
        return IMessage::createResponse(this);  \
    }

#define RESPONSE(ID)                            \
    Response(IMessage *message)                 \
        : IMessage::IResponse(message)          \
    {}                                          \
    static MessageId MSG_ID() { return ID; }    \
    virtual MessageId id() { return ID; }
    }
}
