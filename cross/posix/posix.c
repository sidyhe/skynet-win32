#include <WinSock2.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID _)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		{
			WSADATA wd;

			WSAStartup(MAKEWORD(2, 2), &wd);
		}
		break;
	case DLL_PROCESS_DETACH:
		{
			WSACleanup();
		}
		break;
	default:
		break;
	}

	return TRUE;
}
