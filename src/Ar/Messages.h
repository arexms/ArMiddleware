#pragma once

#include <Ar/Middleware/IMessage.h>
#include <functional>

namespace Ar {

    using namespace Middleware;

    struct LambdaMessage : public IMessage
    {
        static MessageId MSG_ID() { return MessageId::LAMBDA_MESSAGE; }
        virtual MessageId id() { return MessageId::LAMBDA_MESSAGE; }

        std::function<void()> lambda;
    };
}
