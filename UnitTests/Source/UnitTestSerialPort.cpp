#include "pch.h"
#include "CppUnitTest.h"
#include "SerialPort.h"
#include <functional>
#include <thread>
#include <chrono>
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestSerialPort
{

	TEST_CLASS(UnitTestSerialPort)
	{
	public:
		TEST_METHOD(BasicTest)
		{
			std::string testCase = "random";

			{
				SerialPort sp{ R"(\\.\COM5)" };
				sp.WriteSerialPort(testCase);
			}
			SerialPort sp{ R"(\\.\COM5)" };
			Assert::AreEqual(sp.ReadSerialPort(), testCase);
		}
	};
}