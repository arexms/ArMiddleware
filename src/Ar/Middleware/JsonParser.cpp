#include <Ar/Middleware/JsonParser.h>

namespace Ar { namespace Middleware
{
    using json = nlohmann::json;

    JsonObject JsonParser::parse(const std::string &str)
    {
        return json::parse(str);
    }
} }