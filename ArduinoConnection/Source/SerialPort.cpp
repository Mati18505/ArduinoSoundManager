#include "pch.h"
#include "SerialPort.h"
#include <format>
#include <Windows.h>

#define COM_EXCEPTION(message) SerialPort::Exception(__LINE__, __FILE__, message)

SerialPort::SerialPort(const std::string& portName)
{
    handle = CreateFileA(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    CheckHandle(portName);
    Setup();
}

SerialPort::~SerialPort()
{
    CloseHandle(handle);
}

Buffer SerialPort::ReadSerialPort()
{
    if (SetCommMask(handle, EV_RXCHAR))
    {
        DWORD eventMask = 0;

        if (WaitCommEvent(handle, &eventMask, nullptr))
        {
            return ReadBuffer();
        }
        else
            throw COM_EXCEPTION("Failed to WaitCommEvent.");
    }
    else
        throw COM_EXCEPTION("Failed to SetCommMask.");
}

void SerialPort::WriteSerialPort(const Buffer& buffer)
{
    DWORD bytesSend;

    if (!WriteFile(handle, (LPCVOID)buffer.DataPtr(), (uint32_t)buffer.GetSize(), &bytesSend, nullptr)) 
        throw COM_EXCEPTION("Failed to send data.");
}

void SerialPort::Setup()
{
    DCB dcbSerialParameters = { 0 };

    if (GetCommState(handle, &dcbSerialParameters)) 
    {
        dcbSerialParameters.BaudRate = CBR_9600;
        dcbSerialParameters.ByteSize = 8;
        dcbSerialParameters.StopBits = ONESTOPBIT;
        dcbSerialParameters.Parity = NOPARITY;
        dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

        if (SetCommState(handle, &dcbSerialParameters))
        {
            PurgeComm(handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

            COMMTIMEOUTS timeouts;
            timeouts.ReadIntervalTimeout = 50;
            timeouts.ReadTotalTimeoutConstant = 50;
            timeouts.ReadTotalTimeoutMultiplier = 10;
            timeouts.WriteTotalTimeoutConstant = 50;
            timeouts.WriteTotalTimeoutMultiplier = 10;

            if (!SetCommTimeouts(handle, &timeouts))
                throw COM_EXCEPTION("Could not set Timeouts.");
        }
        else
            throw COM_EXCEPTION("Could not set Serial port parameters.");
    }
    else
        throw COM_EXCEPTION("Failed to get current serial state.");
}

void SerialPort::CheckHandle(const std::string& portName)
{
    if (handle == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            throw COM_EXCEPTION(std::format("Handle was not attached. Reason: {0} not available\n", portName));
        }
        else
        {
            throw COM_EXCEPTION("Handle was not attached. No Futher Details.");
        }
    }
}

Buffer SerialPort::ReadBuffer()
{
    std::vector<std::byte> bytes;

    DWORD bytesRead;
    do
    {
        std::byte buffer{};

        if (!ReadFile(handle, &buffer, sizeof(buffer), &bytesRead, nullptr))
            throw COM_EXCEPTION("Cannot read from serial port.");
        bytes.push_back(buffer);

    } while (bytesRead > 0);

    return Buffer(bytes);
}
