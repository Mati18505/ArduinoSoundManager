#include "pch.h"
#include "SerialPort.h"
#include "MyException.h"
#include <Windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <thread>
#include <chrono>
#include "Arduino.h"

//TODO: Asynchroniczne pobieranie danych od arduino: bez czekania ms ( jakoœ z buforami)
//Czekanie na odpowiedŸ: 1ms * bajt + 20ms

//Zamiast string - bufor
//Klasa wy¿szego poziomu do obs³ugi np. string
//WaitForEvent(), PollEvent()

enum class VolumeUnit {
	Decibel,
	Scalar
};

void SetSystemVolume(double newVolume, VolumeUnit vUnit) {
	HRESULT hr;

	// -------------------------
	CoInitialize(NULL);
	IMMDeviceEnumerator* deviceEnumerator = NULL;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	IMMDevice* defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume* endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID*)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;

	if (vUnit == VolumeUnit::Decibel)
		hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);

	else if (vUnit == VolumeUnit::Scalar)
			hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);

	endpointVolume->Release();

	CoUninitialize();
}

static float InverseLerp(float xx, float yy, float value)
{
	return (value - xx) / (yy - xx);
}

void ClientCode()
{
	std::string port;
	Arduino arduino{};
	std::cout << "Connected!" << std::endl;
	std::vector<int> v {-1, 2, 3};
	int max = *std::ranges::max_element(v);

	while (true)
	{
		int read = std::stoi(arduino.ReadString());
		float volume = InverseLerp(0.f, 100.f, read);
		std::cout << std::setfill('0') << volume << std::endl;
		SetSystemVolume(volume, VolumeUnit::Scalar);
	}
}

int main() {
	try
	{
		ClientCode();
	}
	catch (const MyException& e)
	{
		std::cout << "Unhandled Exception: Type: " << e.GetType() << ", " << std::endl << e.GetLocationString() << std::endl << "Message: " << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Unhandled STL Exception: " << ", Description: " << e.what() << std::endl;
	}
	
	return 0;
}