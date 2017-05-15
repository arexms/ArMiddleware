#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Ar/Middleware/ActiveObject.h>
#include <Ar/Middleware/ActiveThread.h>

using ::testing::_;

namespace Ar { namespace Middleware
{
    class MockActiveObject : public ActiveObject
    {
    public:
        MockActiveObject()
        : ActiveObject("aoTest")
        {
        }

        public:
        using ActiveObject::initialize;

        MOCK_METHOD0( initialize, void () );
    };

    class ActiveObjectTest : public testing::Test
    {
    public:
    private:
    };

    class MockActiveThread : public ActiveThread
    {
    public:
        MOCK_METHOD1( initializeActiveObject, void (ActiveObject *ao) );
    };

    TEST_F(ActiveObjectTest, CheckCorrectnessOfAttach)
    {
        ActiveObject ao{"aoTest"};
        MockActiveThread mockActiveThread;

        EXPECT_CALL(mockActiveThread, initializeActiveObject(_))
                .Times(1);

        ao.attachAndInitialize(&mockActiveThread);

        EXPECT_EQ(ao.at(), &mockActiveThread);
    }

    TEST_F(ActiveObjectTest, CheckCorrectnessOfInitialisation)
    {
        MockActiveObject mockActiveObject;
        ActiveThread activeThread;
        activeThread.start("atTest");

        EXPECT_CALL(mockActiveObject, initialize())
                .Times(1);

        mockActiveObject.attachAndInitialize(&activeThread);
        sleep(1);
        activeThread.stop();
    }
} }