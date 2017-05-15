#pragma once

namespace Ar { namespace Middleware
{
    template <typename T>
    class Singleton
    {
    public:
        static T& getInstance()
        {
            static T instance;
            return instance;
        }

        Singleton(Singleton const&)       = delete;
        void operator=(Singleton const&)  = delete;

    protected:
        Singleton() {}
    };
} }
