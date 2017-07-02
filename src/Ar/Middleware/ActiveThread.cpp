#include <Ar/Middleware/ActiveThread.h>
#include <Ar/Middleware/Utils.h>
#include <Ar/Middleware/ActiveThreadAddresses.h>
#include <Ar/Messages.h>
#include <iostream>

namespace Ar { namespace Middleware
{
    SharedPtr<ActiveThread> ActiveThread::create()
    {
        return SharedPtr<ActiveThread>(Ar::Middleware::safeNew<Ar::Middleware::ActiveThread>());
    }

    ActiveThread::ActiveThread()
        : _log("AT")
    {
    }

    ActiveThread::~ActiveThread()
    {
        stop();
        //_log.warning("END");
    }

    void ActiveThread::start(const std::string &name)
    {
        _name = name;
        log().setPrologue(this->name()+": ");

        _core = SharedPtr<ActiveThreadCore>(safeNewWith1Arg<ActiveThreadCore, ActiveThread*>(this));
        _core->start();
        ActiveThreadAddresses::getInstance().add(this->name(), this);
        log().debug("start() started");
    }

    void ActiveThread::stop()
    {
        if(!_core.isEmpty() && !_core->isStopped())
        {
            log().debug("stop() stopping");
            ActiveThreadAddresses::getInstance().remove(this->name());
            _core->stop();
        }
    }

    void ActiveThread::sendToMe(IMessage *message)
    {
        _core->sendToMe(message);
    }

    bool ActiveThread::sendTo(const std::string &atName, IMessage *message)
    {
        auto to = ActiveThreadAddresses::getInstance().get(atName);
        if(to!=nullptr)
        {
            sendTo(to, message);
            return true;
        }

        log().warning("sendTo() Receiver '%s not found!", atName.c_str());
        return false;
    }

    bool ActiveThread::sendTo(ActiveThread *at, IMessage *message)
    {
        message->from = this;
        message->to = at;
        message->to->sendToMe(message);
        return true;
    }

    bool ActiveThread::executeWithin(const std::string &atName, std::function<void()> lambda)
    {
        auto msg = safeNew<LambdaMessage>();

        msg->lambda = lambda;
        return sendTo(atName, msg);
    }

    bool ActiveThread::executeWithin(ActiveThread *at, std::function<void()> lambda)
    {
        auto msg = safeNew<LambdaMessage>();

        msg->lambda = lambda;
        return sendTo(at, msg);
    }

    void ActiveThread::initializeActiveObject(ActiveObject *ao)
    {
        auto msg = safeNew<LambdaMessage>();
        msg->lambda = [ao]
        {
            ao->initialize();
        };
        sendTo(ao->at(), msg);
    }

    bool ActiveThread::dispatch(IMessage *message)
    {
        log().debug("dispatch() received %i", message->id());

        if(message->id() == Ar::Middleware::MessageId::LAMBDA_MESSAGE)
        {
            auto m = message->castTo<LambdaMessage>();
            m->lambda();
        }
        else
        {
            auto receiver = _receivers.find(message->id());

            if (receiver != _receivers.end())
            {
                receiver->second->execute(message);
                return true;
            }

            log().info("dispatch() No registered receiver for %i!", message->id());
        }
        return false;
    }
} }
