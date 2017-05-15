#pragma once

namespace Ar {
    namespace Middleware
    {
        enum class MessageId
        {
            BASE                        = 0,
            OTHER                       = 1,
            OTHER_RESPONSE              = 2,
            RESET                       = 3,
            RESET_RESPONSE              = 4,
            ADD_UDP_TRX_ROUTE           = 5,
            ADD_UDP_TRX_ROUTE_RESPONSE  = 6,
            UDP_PACKET                  = 7,
            UDP_PACKET_RESPONSE         = 8,
            LAMBDA_MESSAGE              = 9,
            END
        };
    }
}

