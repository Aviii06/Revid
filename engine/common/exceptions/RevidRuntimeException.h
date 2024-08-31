#pragma once

#include <exception>
#include <types/Containers.h>
#include <logging/Logging.h>

namespace Revid
{
    class RevidRuntimeException : public std::exception {
    private:
        String message;

    public:
        RevidRuntimeException(const char* msg)
            : message(msg)
        {
           Logger::GetInstance().Log(LogLevel::ERROR, msg);
        }

        const char* what() const throw()
        {
            return message.c_str();
        }
    };
}

