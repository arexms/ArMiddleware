#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Ar/Middleware/JsonParser.h>

using ::testing::_;

namespace Ar { namespace Middleware
{
    using json = nlohmann::json;

    TEST(JsonParserTest, JP)
    {
        JsonParser jp;
        auto ret = jp.parse("{ \"happy\": true, \"pi\": 3.141 }");
        EXPECT_EQ(3.141, ret["pi"].get<double>());
        EXPECT_TRUE(ret["happy"].get<bool>());
    }
} }
