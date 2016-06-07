#pragma once
#include <Windows.h>
#include "ntfs_structs.h"

BYTE* read_sector(HANDLE disk, LONGLONG sect_num, DWORD sect_col);
HANDLE open_raw_read();
MBR_SECTOR MBR_read(HANDLE disk);
void dump_buffer(BYTE* buffer, int size, wchar_t* name);
NTFS_PART_BOOT_SECTOR boot_sector_read(HANDLE disk, MBR_SECTOR MBR, int part_num);