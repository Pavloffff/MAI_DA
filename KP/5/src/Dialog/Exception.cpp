#include "Exception.h"

const char *Dialog::Exception::what() const noexcept
{
    return message.c_str();
}

void Dialog::Exception::LogException(const Exception &e)
{
    std::cerr << e.what() << std::endl;
}

Dialog::Exception::Exception(const std::string msg) : message(msg)
{
}

Dialog::Exception::Exception()
{
}

Dialog::Exception::~Exception()
{
}
