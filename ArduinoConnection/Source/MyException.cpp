#include "pch.h"
#include "MyException.h"
#include <sstream>

MyException::MyException(int line, const std::string& file)
	: line(line), file(file)
{}

std::string MyException::GetType() const { return "Framework Exception"; }

std::string MyException::GetMessage() const { return "Default framework exception"; }

std::string MyException::GetLocationString() const
{
	std::stringstream ss;
	ss << "File: " << file << ", Line: " << line;
	return ss.str();
}

const char* MyException::what() const
{
	whatString = GetMessage();
	return whatString.c_str();
}