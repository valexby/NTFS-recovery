#include "raw_parser.h"
#include <string>

MBR_SECTOR parse_to_mbr(BYTE *buffer)
{
	int i;
	MBR_SECTOR MBR;
	memcpy(MBR.buff, buffer, 446);
	for (i = 0; i<4; i++)
		memcpy(&MBR.parts[i], buffer + 446 + i * 16, 16);
	MBR.signature1 = buffer[510];
	MBR.signature2 = buffer[511];
	return MBR;
}

NTFS_MFT_FILE_ENTRY_HEADER parse_to_mft_header(BYTE *buffer)
{
	int i;
	NTFS_MFT_FILE_ENTRY_HEADER mft_header;
	for (i = 0; i < 4; i++)
		mft_header.fileSignature[i] = buffer[i];
	memcpy(&mft_header.wFixupOffset, buffer + 4, 4);
	mft_header.n64LogSeqNumber = *(buffer + 8);
	memcpy(&mft_header.wSequence, buffer + 16, 8);
	memcpy(&mft_header.dwRecLength, buffer + 24, 8);
	mft_header.n64BaseMftRec = *(buffer + 32);
	memcpy(&mft_header.wNextAttrID, buffer + 40, 4);
	mft_header.dwMFTRecNumber = *(buffer + 44);
	return mft_header;
}

NTFS_PART_BOOT_SECTOR parse_to_boot_sector(BYTE *buffer)
{
	int i;
	NTFS_PART_BOOT_SECTOR boot_sector;
	for (i = 0; i < 3; i++)
		boot_sector.chJumpInstruction[i] = buffer[i];
	for (i = 0; i < 4; i++)
		boot_sector.chOemID[i] = buffer[i + 3];
	for (i = 0; i < 4; i++)
		boot_sector.chDummy[i] = buffer[i + 7];
	memcpy(&boot_sector.bpb.wBytesPerSec, buffer + 11, 2);
	boot_sector.bpb.uchSecPerClust = buffer[13];
	memcpy(&boot_sector.bpb.wReservedSec, buffer + 14, 2);
	for (i = 0; i < 3; i++)
		boot_sector.bpb.uchReserved[i] = buffer[i + 16];
	memcpy(&boot_sector.bpb.wReservedSec, buffer + 19, 2);
	memcpy(boot_sector.bpb.uchReserved, buffer + 16, 3);
	memcpy(&boot_sector.bpb.wUnused1, buffer + 19, 2);
	boot_sector.bpb.uchMediaDescriptor = buffer[21];
	memcpy(&boot_sector.bpb.wUnused2, buffer + 22, 58);
	for (i = 0; i < 426; i++)
		boot_sector.chBootstrapCode[i] = buffer[i + 84];
	memcpy(&boot_sector.wSecMark, buffer + 510, 2);
	return boot_sector;
}

void dump_buffer(BYTE* buffer, int size, wchar_t* name)
{
	HANDLE hFile = CreateFile(name, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (!WriteFile(hFile, buffer, size, nullptr, nullptr))
	{
		throw "Dump save error " + std::to_string(GetLastError());
	}
	CloseHandle(hFile);
}