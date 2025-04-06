#include "pch.h"
#include "Arduino.h"
#include "Buffer.h"

Arduino::Arduino(std::string port)
	: connection(port)
{
	std::cout << "Waiting for arduino to restart!" << std::endl << std::endl;
	std::cout << ReadString() << std::endl;
	std::cout << ReadString() << std::endl;
}
void Arduino::SendString(const std::string& s)
{
	connection.WriteSerialPort(Buffer(s));
}

std::string Arduino::ReadString()
{
	//ReadUntil(std::byte byte)
	//ReadBytes(size_t bytes)
	Buffer buffer = connection.ReadSerialPort();
	auto strPtr = (const char*)buffer.DataPtr();
	size_t strSize = buffer.GetSize();
	return std::string(strPtr, strSize);
}