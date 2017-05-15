#pragma once

#include <Ar/Middleware/Logger.h>

namespace Ar {
    namespace Middleware
    {
        class ActiveThread;

        class ActiveObject
        {
            friend class ActiveThread;

        public:
            ActiveObject(const std::string &aoName);
            virtual ~ActiveObject();
            inline void attachTo(ActiveThread *at_);
            void attachAndInitialize(ActiveThread *at_);
            inline ActiveThread* at();
            
        protected:
            virtual void initialize() {}
            inline Ar::Middleware::Logger& log();
            inline const Ar::Middleware::Logger& log() const;

        private:
            ActiveThread *_at;
            mutable Ar::Middleware::Logger _log;
        };

        // IMPLEMENTATION
        inline void ActiveObject::attachTo(ActiveThread *at_)
        {
            _at = at_;
        }

        inline ActiveThread* ActiveObject::at()
        {
            return _at;
        }

        inline Ar::Middleware::Logger& ActiveObject::log()
        {
            return _log;
        }

        inline const Ar::Middleware::Logger& ActiveObject::log() const
        {
            return _log;
        }
    }
}
