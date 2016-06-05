#pragma once
#include "ntfs_structs.h"

MBR_SECTOR parse_to_mbr(BYTE *buffer);
void dump_buffer(BYTE* buffer, int size, wchar_t* name);
NTFS_PART_BOOT_SECTOR parse_to_boot_sector(BYTE *buffer);
NTFS_MFT_FILE_ENTRY_HEADER parse_to_mft_header(BYTE *buffer);