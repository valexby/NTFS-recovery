#include <iostream>
#include <string>
#include <Windows.h>
#include "ntfs_structs.h"
#include "raw_disk_read.h"
#include "raw_parser.h"
#include "file_descriptor.h"

using namespace std;

HANDLE disk;
int ask_part_numb(MBR_SECTOR MBR);
void err_exit(string err_msg);

int main()
{
	int i, part_num;
	BYTE *buffer;
	MBR_SECTOR MBR;
	NTFS_PART_BOOT_SECTOR boot_sector;
	try {
		disk = open_raw_read();
		buffer = read_sector(disk, 0, 1);
		MBR = MBR_read(disk);
		delete[] buffer;
	}
	catch(string err_msg) {
		err_exit(err_msg);
	}
	if (MBR.signature1 != 0x55 || MBR.signature2 != 0xAA)
		err_exit("Wrong MBR signature\n");

	while ((part_num = ask_part_numb(MBR)) == -1){}
	try {
		boot_sector = boot_sector_read(disk, MBR, part_num);
	}
	catch (string err_msg) {
		err_exit(err_msg);
	}

	LONGLONG mft_relative_offset = boot_sector.bpb.uchSecPerClust * boot_sector.bpb.n64MFTLogicalClustNum;
	LONGLONG mft_sector = mft_relative_offset + MBR.parts[part_num].dwRelativeSector;
	file_descriptor *fd = new file_descriptor(disk, mft_sector);
	delete fd;

	wchar_t dump_name[6] = L"dumpA";
	for (i = 0; i < 1; i++)
	{
		DWORD dwWriten;
		buffer = read_sector(disk, mft_sector + i *10, 2);
		dump_buffer(buffer, 6 * 2048, dump_name);
		if (i < 24) dump_name[4]++; 
		else dump_name[0]++;
		delete[] buffer;
	}
	buffer = read_sector(disk, MBR.parts[part_num].dwRelativeSector+24, 24);
	dump_buffer(buffer, 6*2048, dump_name);
	delete[] buffer;

	CloseHandle(disk);
	return 0;
}

int ask_part_numb(MBR_SECTOR MBR) {
	int part_num, i;
	for (i = 0; i < 4; i++)
	{
		cout << i + 1;
		switch (MBR.parts[i].chType)
		{
		case 0x00:
			cout << " Empty";
			break;
		case 0x07:
			cout << " NTFS";
			break;
		case 0x83:
			cout << " Linux Native";
			break;
		case 0x82:
			cout << " Linux Swap or Solaris X86";
			break;
		default:
			cout << " Uncommon file system";
		}
		cout << "(" << ((MBR.parts[i].dwNumberSectors / 1024) / 1024) / 2 << "Gb)" << endl;
	}
	cout << " Chouse partition\n";
	part_num = getchar() - '0' - 1;
	if (MBR.parts[part_num].chType != 0x07)
	{
		cout << " Is not a NTFS partition\n";
		return -1;
	}
	return part_num;
}

void err_exit(string err_msg)
{
	CloseHandle(disk);
	cerr << err_msg << endl;
	exit(0);
}
