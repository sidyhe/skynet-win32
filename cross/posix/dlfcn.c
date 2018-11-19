#include "dlfcn.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void *dlopen(const char *path, int flag) {
	return LoadLibraryA(path);
}

const char *dlerror() {
	static char str[1024];

	DWORD len;
	DWORD err = GetLastError();

	str[0] = 0;
	len = FormatMessage(
		FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL, 
		err, 
		0, 
		(PTSTR)str, 
		1024, 
		NULL
	);

	if (len == 0)
	{
		str[0] = 0;
	}
	return str;
}

void *dlsym(void *dl, const char *sym) {

	return GetProcAddress(dl, sym);
}

