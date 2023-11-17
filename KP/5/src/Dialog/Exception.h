#pragma once

#include <iostream>
#include <exception>
#include <string>

namespace Dialog
{
    class Exception : public std::exception
    {
    private:
        std::string message;
    public:
        const char* what() const noexcept override;
        static void LogException(const Exception& e);
        Exception(const std::string msg);
        Exception();
        ~Exception();
    };
};