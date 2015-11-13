#include <windows.h>

/************************************************************************/
/* convert Utf8 char to Unicode/LocalCode TCHAR                         */
/************************************************************************/
TCHAR* ConvertUtf8ToTChar(const char* src)
{
	unsigned int size_local;
	char* dest2;
	unsigned int size = strlen(src)+1;
	WCHAR* dest = malloc(sizeof(WCHAR)*size);
	memset( dest, 0, sizeof(WCHAR)*size );
	MultiByteToWideChar(CP_UTF8, 0, src, -1, dest, (int)size);
#ifdef UNICODE
	return dest;
#else
	//convert unicode to local code.
	size_local = wcslen(dest)*2 + 1;
	dest2 = malloc(sizeof(char)*size_local);
	WideCharToMultiByte(CP_ACP, 0, dest, -1, dest2, size, NULL, NULL);
	free(dest);
	return dest2;
#endif
}

/************************************************************************/
/* convert Unicode/LocalCode TCHAR to Utf8 char                         */
/************************************************************************/
char* ConvertTCharToUtf8(const TCHAR* src)
{
#ifdef UNICODE
	WCHAR* tmp = (WCHAR*)src;
	int size = wcslen(src)*3 + 1;
	char* dest = new char[size];
	memset( dest, 0, size );
	WideCharToMultiByte(CP_UTF8, 0, tmp, -1, dest, size, NULL, NULL);
	return dest;
#else
	char* dest2;
	char* tmp = (char*)src;
	unsigned int size = strlen(tmp)+1;
	WCHAR* dest = malloc(sizeof(WCHAR)*size);
	memset( dest, 0, sizeof(WCHAR)*size );
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int)size); // convert local code to unicode.

	size = wcslen(dest)*3 + 1;
	dest2 = malloc(sizeof(char)*size);
	memset( dest2, 0, size );
	WideCharToMultiByte(CP_UTF8, 0, dest, -1, dest2, size, NULL, NULL); // convert unicode to utf8.
	free(dest);
	return dest2;
#endif
}