#include <iostream>
#include <Windows.h>

typedef struct {
	BYTE chBootInd;
	BYTE chHead;
	BYTE chSector;
	BYTE chCylinder;
	BYTE chType;
	BYTE chLastHead;
	BYTE chLastSector;
	BYTE chLastCylinder;
	DWORD dwRelativeSector;
	DWORD dwNumberSectors;
} _PARTITION;

typedef struct {
	BYTE buff[446];
	_PARTITION parts[4];
} MBRhold;

int main(int argc, char* argv[])
{
	int i, j;
	DWORD dwRead;
	BYTE buffer[512];
	HANDLE hDisk;
	MBRhold MBR;
	hDisk = CreateFile(L"\\\\.\\PhysicalDrive0", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (hDisk == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile Error %d\n", GetLastError());
		CloseHandle(hDisk);
		return 0;
	}

	//SetFilePointer(hDisk, 0 * 512, 0, FILE_BEGIN);

	if (!ReadFile(hDisk, &buffer, 512, &dwRead, NULL))
	{
		printf("ReadFile error %d\n", GetLastError());
		CloseHandle(hDisk);
		return 0;
	}
	printf("Byte readed %d\n", dwRead);
	for (i = 0; i < 446; i++)
		MBR.buff[i] = buffer[i];
	for (i=0;i<4;i++)
	{
		MBR.parts[i].chBootInd = buffer[i*16+446];
		MBR.parts[i].chHead = buffer[i*16+447];
		MBR.parts[i].chSector = buffer[i*16+448];
		MBR.parts[i].chCylinder = buffer[i*16+449];
		MBR.parts[i].chType = buffer[i*16+450];
		MBR.parts[i].chLastHead = buffer[i*16+451];
		MBR.parts[i].chLastSector = buffer[i*16+452];
		MBR.parts[i].chLastCylinder = buffer[i*16+453];
		MBR.parts[i].dwRelativeSector = 0;
		for (j=0;j<4;j++)
		{
			MBR.parts[i].dwRelativeSector = MBR.parts[i].dwRelativeSector << 8;
			MBR.parts[i].dwRelativeSector = MBR.parts[i].dwRelativeSector | buffer[i*16+454+j];
		}
		MBR.parts[i].dwNumberSectors = 0;
		for (j=0;j<4;j++)
		{
			MBR.parts[i].dwNumberSectors = MBR.parts[i].dwNumberSectors << 8;
			MBR.parts[i].dwNumberSectors = MBR.parts[i].dwNumberSectors | buffer[i*16+448+j];
		}
	}
	for (i = 0; i < 4; i++)
	{
		switch (MBR.parts[i].chType)
		{
		case 0x00:
			printf("Empty\n");
			break;
		case 0x07:
			printf("NTFS\n");
			break;
		case 0x83:
			printf("Linux Native\n");
			break;
		case 0x82:
			printf("Linux Swap or Solaris X86\n");
			break;
		default:
			printf("Uncommon file system\n");
		}
	}
	CloseHandle(hDisk);
	return 0;
}