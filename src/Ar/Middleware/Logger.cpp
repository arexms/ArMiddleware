#include <Ar/Middleware/Logger.h>

#include <cstdarg>

#include <chrono>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstring>

namespace Ar { namespace Middleware
{
    /*static const std::string red("\033[0;31m");
    static const std::string green("\033[1;32m");
    static const std::string yellow("\033[1;33m");
    static const std::string cyan("\033[0;36m");
    static const std::string magenta("\033[0;35m");*/
    static const std::string reset("\033[0m");

    static const char *LogLevelStr[] =
    {
        "ERR",
        "WRN",
        "INF",
        "DBG",
        "???"
    };

    static const char *LogColor[] =
    {
        "\033[0;31m", // red
        "\033[1;33m", // yellow
        "\033[1;32m", // green
        "\033[0;35m", // magenta
    };
    
    std::mutex Logger::Log::_mutex;

    Logger::Prologue::Prologue()
    {}

    Logger::Prologue::Prologue( const std::string &prologue_ )
        : prologue( prologue_ )
    {}

    Logger::Log::Log(const std::string &prefix, LogLevel level)
        : _level(level)
        , _prefix(prefix)
    {
        if(prefix.size()>8)
        {
            _prefix = _prefix.substr(0, 8);
        }
        else if(prefix.size()<8)
        {
            _prefix = std::string(8 - prefix.size(), ' ') + prefix;
        }
    }

    void Logger::Log::setPrologue( const Prologue &prologue )
    {
        _prologue = prologue;
    }

    const Logger::Log& Logger::Log::operator()( const char *str, ... ) const
    {
        std::unique_lock<std::mutex> lock( _mutex );
        //if( Logger::_logSet[ _flag ] < _level ) return *this;
        //std::string now = boost::posix_time::to_simple_string( boost::posix_time::microsec_clock::local_time() );

            auto now_ = std::chrono::high_resolution_clock::now();
            auto in_time_t = std::chrono::high_resolution_clock::to_time_t(now_);

                auto mc_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(now_.time_since_epoch());
                auto mc = mc_since_epoch.count() % 1000000;

            std::stringstream ss;
#define STRING2(x) #x
#define STRING(x) STRING2(x)

#pragma message(STRING(__cplusplus))

#if __cplusplus >= 201103L      // does not wotk with older iomanip header - no std::put_time      
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << ":" << mc;
#else
            ss << "0000-00-00" << ":" << mc;
#endif // __cplusplus >= 201103L
            std::string now = ss.str();

        std::string	temp = std::string( "[" ) + now + "][" + LogLevelStr[ _level ] + "][" + _prefix + "]" + " " + _prologue.prologue + str + "\n";

        char buff[ 2048 ] = {0};
        va_list args;
        va_start( args, str );
        vsprintf( buff, temp.c_str(), args );
        va_end( args );

        {
            printf( "%s%s%s", LogColor[ _level ], buff, reset.c_str() );
        }
        
        std::fstream file;
        file.open( "logs.txt", std::fstream::app );
        file.write( buff, std::strlen( buff ) );
        file.close();
        
        return *this;
    }

    Logger::Logger(const std::string &prefix, const std::string &prologue)
        : debug( prefix, DEBUG )
        , info( prefix, INFO )
        , warning( prefix, WARNING )
        , error( prefix, ERROR )
    {
        setPrologue( prologue );
    }

    void Logger::setPrologue( const Prologue &prologue )
    {
        debug.setPrologue( prologue );
        info.setPrologue( prologue );
        warning.setPrologue( prologue );
        error.setPrologue( prologue );
    }

    void Logger::setPrologue( const std::string &prologue )
    {
        Prologue pr( prologue );
        setPrologue( pr );
    }
} }
