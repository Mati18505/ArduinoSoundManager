#pragma once
#include "SerialPort.h"

class Arduino
{
public:
	explicit Arduino(std::string port = R"(\\.\COM3)");
	void SendString(const std::string& s);
	std::string ReadString();
private:
	SerialPort connection;
};