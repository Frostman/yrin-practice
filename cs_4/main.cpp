#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;

int main(int argc, char * argv[]) {
	if(argc != 2) {
		printf("Please specify path to .exe file\n");
		_getch();
		return 1;
	}

	std::string path = argv[1];	
	
	HANDLE hRead = CreateFile(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);

	if(hRead == INVALID_HANDLE_VALUE) {
		printf("Can't open file: %s", path);
		_getch();
		return -1;
	}
	
	DWORD dwFileSize = GetFileSize(hRead, NULL);
	PBYTE FileBuffer = (PBYTE) malloc(dwFileSize * sizeof(BYTE));
	DWORD dwAmountRead;
	
	BOOL res = ReadFile(hRead, (PVOID)FileBuffer, dwFileSize, &dwAmountRead, NULL);
	CloseHandle(hRead);
	if(!res) {
		printf("Failed to read file\n");
		_getch();
		return -1;
	}
	
	for(int i = 0x1902; i <= 0x1903; ++i){
		FileBuffer[i] = ((BYTE) 0x90);
	}
	HANDLE hWrite = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WriteFile(hWrite, FileBuffer, dwFileSize, &dwAmountRead, NULL);
	CloseHandle(hWrite);
	printf("File '%s' successfully cracked.\n", path.c_str());
	_getch();

	return 0;
}