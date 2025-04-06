#pragma once
#include "MyException.h"
#include "Buffer.h"

constexpr int ARDUINO_WAIT_TIME = 2000;
constexpr int MAX_DATA_LENGTH = 255;

class SerialPort
{
public:
    class Exception : public MyException
    {
    public:
        Exception(int line, const std::string& file, const std::string& message)
            : MyException(line, file), message(message)
        {}
        std::string GetType() const override { return "Serial Port Exception"; }
        std::string GetMessage() const override { return message; }
    private:
        std::string message;
    };

    explicit SerialPort(const std::string& portName);
    ~SerialPort() noexcept;

    Buffer ReadSerialPort();
    void WriteSerialPort(const Buffer& buffer);
private:
    void Setup();
    void CheckHandle(const std::string& portName);
    Buffer ReadBuffer();

    typedef void* HANDLE;
    HANDLE handle;
};
