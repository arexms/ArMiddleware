#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Ar/Middleware/ActiveObject.h>
#include <Ar/Middleware/ActiveThread.h>

using ::testing::_;

namespace Ar { namespace Middleware
{
    class TestMessage : public IMessage
    {
    public:
        static MessageId MSG_ID() { return MessageId::TEST_MESSAGE; }
        virtual MessageId id() { return  MessageId::TEST_MESSAGE; } 
    };

    class ActiveObjectForTests : public ActiveObject
    {
    public:
        ActiveObjectForTests()
        : ActiveObject("aoForTest")
        {

        }

        virtual void initialize() override
        {
            at()->registerReceiverForMessage(this, &ActiveObjectForTests::getMessage);
        }

        virtual void getMessage(TestMessage *) {}
        virtual void forLambda() {}
    private:
    };

    class MockActiveObjectForTests : public ActiveObjectForTests
    {
    public:
        MOCK_METHOD0( forLambda, void () );
        MOCK_METHOD1( getMessage, void (TestMessage *msg) );
    };

    class ActiveThreadTest : public testing::Test
    {
    protected:
        ActiveThread _at1;
        ActiveThread _at2;
    };

    TEST_F(ActiveThreadTest, SendToByPointer)
    {
        auto at1 = ActiveThread::create();
        at1->start("at1");
        _at2.start("at2");
        MockActiveObjectForTests ao;

        EXPECT_CALL(ao, getMessage(_))
                .Times(1);

        ao.attachAndInitialize(at1.get());

        TestMessage *tm = safeNew<TestMessage>();
        _at2.sendTo(at1.get(), tm);

        sleep(1);
        at1->stop();
        _at2.stop();
    }

    TEST_F(ActiveThreadTest, SendToByName)
    {
        _at1.start("at1");
        _at2.start("at2");
        MockActiveObjectForTests ao;

        EXPECT_CALL(ao, getMessage(_))
                .Times(1);

        ao.attachAndInitialize(&_at1);

        TestMessage *tm = safeNew<TestMessage>();
        _at2.sendTo("at1", tm);

        sleep(1);
        _at1.stop();
        _at2.stop();
    }

    TEST_F(ActiveThreadTest, ExecuteWithinByPointer)
    {
        _at1.start("at1");
        _at2.start("at2");
        MockActiveObjectForTests ao;

        EXPECT_CALL(ao, forLambda())
                .Times(1);

        ao.attachAndInitialize(&_at1);

        _at2.executeWithin(&_at1, [&ao](){ao.forLambda();});

        sleep(1);
        _at1.stop();
        _at2.stop();
    }

    TEST_F(ActiveThreadTest, ExecuteWithinByName)
    {
        _at1.start("at1");
        _at2.start("at2");
        MockActiveObjectForTests ao;

        EXPECT_CALL(ao, forLambda())
                .Times(1);

        ao.attachAndInitialize(&_at1);

        _at2.executeWithin("at1", [&ao](){ao.forLambda();});

        sleep(1);
        _at1.stop();
        _at2.stop();
    }

    TEST_F(ActiveThreadTest, SendToNotExecutedIfAoDoesNotBelongToGivenAt)
    {
        _at1.start("at1");
        _at2.start("at2");
        MockActiveObjectForTests ao;

        EXPECT_CALL(ao, getMessage(_))
                .Times(0);

        ao.attachAndInitialize(&_at1);

        TestMessage *tm = safeNew<TestMessage>();
        _at2.sendTo("at2", tm);

        sleep(1);
        _at1.stop();
        _at2.stop();
    }

    TEST_F(ActiveThreadTest, SendToNotExecutedIfGivenAtNotExist)
    {
        _at1.start("at1");
        _at2.start("at2");
        MockActiveObjectForTests ao;

        EXPECT_CALL(ao, getMessage(_))
                .Times(0);

        ao.attachAndInitialize(&_at1);

        TestMessage *tm = safeNew<TestMessage>();
        _at2.sendTo("none_at", tm);

        sleep(1);
        _at1.stop();
        _at2.stop();
    }
} }