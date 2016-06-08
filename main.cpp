#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>

#include "ntfs_structs.h"
#include "raw_disk_read.h"
#include "file_descriptor.h"
#include "non_resident_attr.h"
#include "index_descriptor.h"
using namespace std;

HANDLE disk;
MBR_SECTOR MBR;
NTFS_PART_BOOT_SECTOR boot_sector;
int part_num;

int ask_part_numb(MBR_SECTOR MBR);
void err_exit(string err_msg);
void print_ind_rec(index_record record);
void print_dir(index_descriptor cur_dir);
file_descriptor move_to_dir(LONGLONG mft_entry_numb, vector<MFT_FRAG> frags);
index_descriptor move_to_index(file_descriptor dir_rec);
vector<MFT_FRAG> get_mtf_chain();
void move_data(file_descriptor file_rec);

int main()
{
	BYTE *buffer;
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
	vector<MFT_FRAG> frags = get_mtf_chain();
	file_descriptor cur_dir = move_to_dir(5, frags);
	index_descriptor cur_index = move_to_index(cur_dir);
	print_dir(cur_index);
	string instruct;
	cin >> instruct;
	while (instruct!="exit")
	{
		int cur_way = atoi(instruct.c_str());
		if (cur_way != 1)
		{
			cur_dir = move_to_dir(cur_index.entries[cur_way-2].dwMFTRecNum, frags);
			if (cur_dir.isDirectory())
			{
				cur_index = move_to_index(cur_dir);
				print_dir(cur_index);
			}
			else
			{
				cout << cur_dir.get_file_name();
				move_data(cur_dir);
			}
		}
		cin >> instruct;
	}
	
	CloseHandle(disk);
	return 0;
}

file_descriptor move_to_dir(LONGLONG mft_entry_numb, vector<MFT_FRAG> frags)
{
	int i = 1;
	while (i < frags.size() && mft_entry_numb > frags[i].beg_rec_num) i++;
	i--;
	LONGLONG offset = frags[i].offset + (mft_entry_numb - frags[i].beg_rec_num) * 2;
	BYTE* test_buffer = read_sector(disk, offset, 2);
	delete[] test_buffer;
	return file_descriptor(disk, offset);
}

vector<MFT_FRAG> get_mtf_chain()
{
	LONGLONG mft_relative_offset, mft_sector;
	mft_relative_offset = boot_sector.bpb.uchSecPerClust * boot_sector.bpb.n64MFTLogicalClustNum;
	mft_sector = mft_relative_offset + MBR.parts[part_num].dwRelativeSector;
	int i, data_attr_pos;
	file_descriptor mft_file = file_descriptor(disk, mft_sector);
	for (i = 0; i < mft_file.attr_col; i++)
	{
		if (mft_file.attributes[i]->dwAttrType == 0x80) break;
	}
	data_attr_pos = i;
	if (i > mft_file.attr_col) throw "Data attribute in mft not found";
	vector<MFT_FRAG> result = vector<MFT_FRAG>(((non_resident_attr*)mft_file.attributes[data_attr_pos])->runs_col);
	result[0].beg_rec_num = 0;
	result[0].offset = mft_sector;
	for (i = 1; i < ((non_resident_attr*)mft_file.attributes[data_attr_pos])->runs_col;i++)
	{
		result[i].beg_rec_num = result[i-1].beg_rec_num + 
			((non_resident_attr*)mft_file.attributes[data_attr_pos])->dataRuns[i - 1].n64AttrSizeClust * 8 / 2;
		result[i].offset = result[i - 1].offset +
			((non_resident_attr*)mft_file.attributes[data_attr_pos])->dataRuns[i].n64AttrOffsetClust * 8;
	}
	return result;
}

index_descriptor move_to_index(file_descriptor dir_rec)
{
	int pos;
	pos = dir_rec.get_attr_pos(0xA0);
	if (pos == -1) throw "No index attribute recods is found";
	return index_descriptor(disk, MBR.parts[part_num].dwRelativeSector +
		8 * ((non_resident_attr*)dir_rec.attributes[pos])->dataRuns[0].n64AttrOffsetClust,
		8 * ((non_resident_attr*)dir_rec.attributes[pos])->dataRuns[0].n64AttrSizeClust);
}

void move_data(file_descriptor file_rec)
{
	int pos;
	pos = file_rec.get_attr_pos(0x80);
	wchar_t name[] = L"FRAGMENT1";
	if (pos == -1) {
		throw "Data attribute in rec not found";
	}
	if (file_rec.attributes[pos]->cResident == 0)
	{
		dump_buffer(file_rec.attributes[pos]->attrContent, file_rec.attributes[pos]->attr_cont_len, L"Test");
	}
	else
	{
		non_resident_attr* fragments = (non_resident_attr*)file_rec.attributes[pos];
		for (int i = 0; i < fragments->runs_col;i++)
		{
			BYTE* buffer;
			buffer = read_sector(disk, MBR.parts[part_num].dwRelativeSector + fragments->dataRuns[i].n64AttrOffsetClust * 8,
				fragments->dataRuns[i].n64AttrSizeClust * 8);
			dump_buffer(buffer, fragments->dataRuns[i].n64AttrSizeClust * 8 * 512, name);
			name[8]++;
		}
	}
}

void print_dir(index_descriptor cur_dir)
{
	int i;
	cout << " 1 ..\n";
	for (i = 0; i < cur_dir.entries_cnt;i++)
	{
		if (i + 2 < 10) cout << ' ';
		cout << i+2 << ' ';
		print_ind_rec(cur_dir.entries[i]);
		cout << endl;
	}
}

void print_ind_rec(index_record record)
{
	int i;
	for (i = 0; i < record.cNameLength*2; i+=2)
		cout << static_cast<char>(record.name[i]);
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
