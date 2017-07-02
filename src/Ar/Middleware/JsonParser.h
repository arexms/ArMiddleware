#pragma once

#include <json.hpp>

namespace Ar { namespace Middleware
{
    typedef nlohmann::json JsonObject;

    class JsonParser
    {
    public:
        JsonObject parse(const std::string &str);
    };
} }
