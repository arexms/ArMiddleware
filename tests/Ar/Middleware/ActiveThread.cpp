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
        class Response : public IMessage::IResponse
        {
        public:
            RESPONSE(MessageId::TEST_MESSAGE_RESPONSE)
        };

        MESSAGE(MessageId::TEST_MESSAGE)
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
    };

    class ActiveObjectForTestsWithWait : public ActiveObject
    {
        bool _sendResponse = true;
    public:
        ActiveObjectForTestsWithWait(const std::string &name, bool sendResponse)
            : ActiveObject(name)
            , _sendResponse(sendResponse)
        {
        }

        virtual void initialize() override
        {
            at()->registerReceiverForMessage(this, &ActiveObjectForTestsWithWait::receivedMessage);
        }

        virtual void receivedMessage(TestMessage *msg)
        {
            auto response = msg->createResponse();
            if(_sendResponse)
            {
                at()->sendTo(response->to, response);
            }
        }

        virtual void sentAndWaitForResponse(ActiveThread *at_)
        {
            TestMessage *tm = safeNew<TestMessage>();
            at()->sendTo(at_, tm);
            at()->waitForResponseFor<TestMessage>(
                [&](TestMessage::Response *)
                {
                    received();
                },
                [&]()
                {
                    noResponse();
                }
            );
        }
        virtual void received() {}
        virtual void noResponse() {}

    private:
    };

    class MockActiveObjectForTests : public ActiveObjectForTests
    {
    public:
        MOCK_METHOD0( forLambda, void () );
        MOCK_METHOD0( received, void () );
        MOCK_METHOD0( noResponse, void () );
        MOCK_METHOD1( getMessage, void (TestMessage *msg) );
    };

    class MockActiveObjectForTestsWithWait : public ActiveObjectForTestsWithWait
    {
    public:
        using ActiveObjectForTestsWithWait::ActiveObjectForTestsWithWait;

        MOCK_METHOD0( received, void () );
        MOCK_METHOD0( noResponse, void () );
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

    TEST_F(ActiveThreadTest, WaitForResponseWithSuccess)
    {
        _at1.start("at1");
        _at2.start("at2");

        MockActiveObjectForTestsWithWait ao1{"ao1", true};
        MockActiveObjectForTestsWithWait ao2{"ao2", true};
        EXPECT_CALL(ao1, received())
                .Times(1);
        EXPECT_CALL(ao1, noResponse())
                .Times(0);

        ao1.attachAndInitialize(&_at1);
        ao2.attachAndInitialize(&_at2);

        _at1.executeWithin(&_at1,
        [&]()
        {
            ao1.sentAndWaitForResponse(&_at2);
        });

        sleep(1);
        _at1.stop();
        _at2.stop();
    }

    TEST_F(ActiveThreadTest, WaitForResponseWithFail)
    {
        _at1.start("at1");
        _at2.start("at2");

        MockActiveObjectForTestsWithWait ao1{"ao1", false};
        MockActiveObjectForTestsWithWait ao2{"ao1", false};
        EXPECT_CALL(ao1, received())
                .Times(0);
        EXPECT_CALL(ao1, noResponse())
                .Times(1);

        ao1.attachAndInitialize(&_at1);
        ao2.attachAndInitialize(&_at2);

        _at1.executeWithin(&_at1,
        [&]()
        {
            ao1.sentAndWaitForResponse(&_at2);
        });

        sleep(1);
        _at1.stop();
        _at2.stop();
    }
} }