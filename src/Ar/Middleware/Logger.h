#ifndef Ar_Middleware_Logger_h__
#define Ar_Middleware_Logger_h__

#include <Ar/Middleware/LogFlag.h>
#include <Ar/Middleware/LogLevel.h>

#include <string>
#include <mutex>

namespace Ar { namespace Middleware
{
    class Logger
    {
        class Prologue
        {
        public:
            Prologue();

            explicit Prologue( const std::string &prologue_ );

            std::string prologue;
        };

        class Log
        {
        public:
            Log( const std::string &prefix, LogLevel level );

            void setPrologue( const Prologue &prologue );

            const Log& operator()( const char *str, ... ) const;

        private:
            LogLevel    _level;
            Prologue    _prologue;
            std::string _prefix;
            static std::mutex _mutex;
        };


    public:
        Logger( const std::string &prefix, const std::string &prologue = "" );
        void setPrologue( const Prologue &prologue );
        void setPrologue( const std::string &prologue );

        Log debug;
        Log info;
        Log warning;
        Log error;
    };

    typedef Logger LoggingSystem;
} }

#endif // Ar_Middleware_Logger_h__
