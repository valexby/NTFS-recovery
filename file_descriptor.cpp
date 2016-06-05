#include "file_descriptor.h"
#include "raw_disk_read.h"
#include <string>
#include "std_attr_header.h"
#include "std_info.h"
#include "non_resident_attr.h"

file_descriptor::file_descriptor(HANDLE disk, LONGLONG sector)
{
	try 
	{
		BYTE* buffer = read_sector(disk, sector, 1);
		init(buffer);
		if (dwRealFileSize > 512)
		{
			delete[] buffer;
			buffer = read_sector(disk, sector, dwRealFileSize / 512 + 1);
		}
		int i, cur_offset = wFirstAttrOffset;
		attr_col = get_attr_col(buffer + wFirstAttrOffset);
		attributes = new std_attr_header*[attr_col];
		for (i = 0; i < attr_col;i++)
		{
			attributes[i] = build_attr(buffer + cur_offset);
			cur_offset += attributes[i]->dwFullAttrLen;
		}
		delete[] buffer;
	}
	catch(std::string)
	{
		throw;
	}
}

file_descriptor::~file_descriptor()
{
	std_info* temp;
	for (int i = 0; i < attr_col;i++)
	{
		delete attributes[i];
	}
	delete[] attributes;
	delete[] updateSeq;
}

void file_descriptor::init(BYTE* buffer)
{
	int i;
	for (i = 0; i < 4; i++)
		signature[i] = buffer[i];
	memcpy(&wOffsetOfUpdtSeq, buffer + 4, 4);
	n64LogFileSeq = *(buffer + 8);
	memcpy(&wUseCnt, buffer + 16, 8);
	memcpy(&dwRealFileSize, buffer + 24, 8);
	n64RefToBaseFile = *(buffer + 32);
	memcpy(&wNextAttrId, buffer + 40, 4);
	updateSeq = new WORD[wSizeOfUpdtSeq];
	memcpy(updateSeq, buffer + 44, wSizeOfUpdtSeq * 2);
}

int file_descriptor::get_attr_col(BYTE* start)
{
	int out = 0, offset;
	while (*reinterpret_cast<DWORD*>(start)!=0xFFFFFFFF)
	{
		memcpy(&offset, start + 4, 4);
		start += offset;
		out++;
	}
	return out;
}

bool file_descriptor::is_resident(BYTE* raw)
{
	return *(raw + 8) == 0;
}

std_attr_header* file_descriptor::build_attr(BYTE* raw)
{
	DWORD type = *raw;
	std_attr_header* res_ptr;
	if (!is_resident(raw)) return new non_resident_attr(raw);
	switch(type)
	{
	case 0xFFFFFFFF:
		throw "Unexpected end of attribute list";
	case 0x10:
		res_ptr = new std_info(raw);
		break;
	default:
		res_ptr = new std_attr_header(raw);
		break;
	}
	return res_ptr;
}
