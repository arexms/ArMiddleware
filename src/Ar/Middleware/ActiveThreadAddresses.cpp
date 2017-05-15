#include <Ar/Middleware/ActiveThreadAddresses.h>

namespace Ar { namespace Middleware
{
    ActiveThreadAddresses::ActiveThreadAddresses()
    : _log("AT_ADDR")
    {
        log().setPrologue("ATA: ");
    }

    void ActiveThreadAddresses::add(const std::string &atName, ActiveThread *address)
    {
        log().info("Registered: %s", atName.c_str());
        _addresses[atName] = address;
    }

    ActiveThread *ActiveThreadAddresses::get(const std::string &atName)
    {
        auto address = _addresses.find(atName);
        if(address!=_addresses.end())
        {
            return address->second;
        }

        log().warning("Address not found: %s", atName.c_str());
        return nullptr;
    }

    void ActiveThreadAddresses::remove(const std::string &atName)
    {
        if(!atName.empty())
        {
            _addresses.erase(atName);
        }
        log().info("Removed: %s", atName.c_str());
    }
} }
