#include <Ar/Middleware/ActiveObject.h>
#include <Ar/Middleware/ActiveThread.h>

namespace Ar { namespace Middleware
{
    ActiveObject::ActiveObject(const std::string &aoName)
        : _at(nullptr)
        , _log(aoName)
    {

    }

    ActiveObject::~ActiveObject()
    {

    }

    void ActiveObject::attachAndInitialize(ActiveThread *at_)
    {
        attachTo(at_);
        at()->initializeActiveObject(this);
    }
} }
