#include "raw_disk_read.h"
#include <iostream>
#include "raw_parser.h"
#include <string>

using namespace std;

BYTE* read_sector(HANDLE disk, LONGLONG sect_num, DWORD sect_col)
{
	LONGLONG offset = sect_num * 512;
	LARGE_INTEGER li;
	li.QuadPart = offset;
	DWORD dwRead;
	BYTE* buffer = new BYTE[sect_col * 512];
	if ((li.LowPart = SetFilePointer(disk, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER) &&
		GetLastError() != NO_ERROR) {
		throw  "Set file pointer error " + to_string(GetLastError());
	}
	if (!ReadFile(disk, buffer, sect_col * 512, &dwRead, nullptr))
		throw "ReadFile error " + GetLastError();
	//printf("Byte readed %d\n", dwRead);
	return buffer;
}

HANDLE open_raw_read()
{
	HANDLE disk;
	disk = CreateFile(L"\\\\.\\PhysicalDrive0", GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if (disk == INVALID_HANDLE_VALUE)
		throw  "CreateFile error " + to_string(GetLastError());
	return disk;
}

MBR_SECTOR MBR_read(HANDLE disk)
{
	BYTE* buffer;
	MBR_SECTOR MBR;
	try 
	{
		buffer = read_sector(disk, 0, 1);
	} 
	catch (string) 
	{
		throw;
	}
	MBR = parse_to_mbr(buffer);
	delete[] buffer;
	return MBR;
}

NTFS_PART_BOOT_SECTOR boot_sector_read(HANDLE disk, MBR_SECTOR MBR, int part_num)
{
	BYTE* buffer;
	NTFS_PART_BOOT_SECTOR boot_sector;
	try	
	{
		buffer = read_sector(disk, MBR.parts[part_num].dwRelativeSector, 1);
	} 
	catch (string)
	{
		throw;
	}
	boot_sector = parse_to_boot_sector(buffer);
	delete[] buffer;
	return boot_sector;
}

void dump_buffer(BYTE* buffer, int size, wchar_t* name)
{
	HANDLE hFile;
	DWORD dwWritten;
	if ((hFile = CreateFile(name, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr))
		== INVALID_HANDLE_VALUE)
	{
		throw "Dump file open error " + to_string(GetLastError());
	}
	if (!WriteFile(hFile, buffer, size, &dwWritten, nullptr))
	{
		throw "Dump save error " + to_string(GetLastError());
	}
	cout << dwWritten << " bytes written\n";
	CloseHandle(hFile);
}