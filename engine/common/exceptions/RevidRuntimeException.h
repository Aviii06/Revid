#pragma once

#include <types/Containers.h>
#include <logging/Logging.h>

namespace Revid
{
    class RevidRuntimeException
    {
    public:
        RevidRuntimeException(String msg)
        {
           Logger::Log(LogLevel::ERROR, msg);
        }

        String& what() noexcept
        {
            return message;
        }

    private:
        String message;

    };
}

