#include "main.h"

namespace uslo
{
	extern HWND	mainWindow;	// Main window handle.
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		uslo::LoadConfiguration();
	}
	catch (std::exception& exception)
	{
		char errorMessage[128];
		sprintf_s(errorMessage, sizeof(errorMessage), "Loading the configuration file failed with error: %s\nUsing default configuration instead.", exception.what());

		MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
	}

	int exitCode;

	try
	{
		uslo::Initiate();
		exitCode = uslo::EnterMainLoop();
	}
	catch (stmm::WindowsException& exception)
	{
		char errorMessage[128];
		char exceptionMessage[128];
		sprintf_s(errorMessage, sizeof(errorMessage), "Fatal error %i: %s", exception._GetErrorCode(), exception.what(exceptionMessage, sizeof(exceptionMessage)));
		exitCode = exception._GetErrorCode();

		MessageBox(uslo::mainWindow, errorMessage, "Fatal error", MB_OK | MB_ICONERROR);
	}
	catch (std::exception& exception)
	{
		char errorMessage[128];
		sprintf_s(errorMessage, sizeof(errorMessage), "Fatal error: %s", exception.what());
		exitCode = -1;

		MessageBox(uslo::mainWindow, errorMessage, "Fatal error", MB_OK | MB_ICONERROR);
	}

	try
	{
		uslo::SaveConfiguration();
	}
	catch (std::exception& exception)
	{
		char errorMessage[128];
		sprintf_s(errorMessage, sizeof(errorMessage), "Saving the configuration file failed with error: %s", exception.what());

		MessageBox(NULL, errorMessage, "Error", MB_OK | MB_ICONERROR);
	}

	return exitCode;
}