#pragma once
#include <string>
#include <map>
#include <Ar/Middleware/Singleton.h>
#include <Ar/Middleware/Logger.h>

namespace Ar { namespace Middleware
{
    class ActiveThread;

    class ActiveThreadAddresses : public Singleton<ActiveThreadAddresses>
    {
    public:
        ActiveThreadAddresses();
        void add(const std::string &atName, ActiveThread *address);
        ActiveThread* get(const std::string &atName);
        void remove(const std::string &atName);

    protected:
        Logger& log();

    private:
        std::map<std::string, ActiveThread *> _addresses;
        Logger                                _log;
    };

    // IMPLEMENTATION
    inline Logger& ActiveThreadAddresses::log()
    {
        return _log;
    }
} }
