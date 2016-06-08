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
		if (*(DWORD*)(buffer+24) > 512)
		{
			BYTE* temp = buffer;
			buffer = read_sector(disk, sector, *(DWORD*)(temp + 24) / 512 + (*(DWORD*)(temp + 24) / 512 != 0 ? 1 : 0));
			delete[] temp;
		}
		dump_buffer(buffer, *(DWORD*)(buffer + 24), L"DUMP1");
		init(buffer);
		clean_sect_bord(buffer);
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
	for (int i = 0; i < attr_col;i++)
	{
		delete attributes[i];
	}
	delete[] attributes;
	delete[] updateSeq;
}

void file_descriptor::clean_sect_bord(BYTE* raw) const
{
	int i;
	for (i = 0; i < wUpdtSeqSize-1; i++)
	{
		memcpy(raw + (i + 1) * 512 - 2, &updateSeq[i], 2);
	}
}

file_descriptor::file_descriptor(const file_descriptor& source)
{
	*this = source;
}


file_descriptor& file_descriptor::operator=(const file_descriptor& source)
{
	if (this == &source) return *this;
	memcpy(signature, source.signature, 4);
	memcpy(&wUpdtSeqOffset, &source.wUpdtSeqOffset, 4);
	n64LogFileSeq = source.n64LogFileSeq;
	memcpy(&wUseCnt, &source.wUseCnt, 8);
	memcpy(&dwRealFileSize, &source.dwRealFileSize, 8);
	n64RefToBaseFile = source.n64RefToBaseFile;
	memcpy(&wNextAttrId, &source.wNextAttrId, 4);
	updateSeq = new WORD[wUpdtSeqSize - 1];
	memcpy(updateSeq, source.updateSeq, wUpdtSeqSize - 1);
	attr_col = source.attr_col;
	attributes = new std_attr_header*[attr_col];
	for (int i = 0; i < attr_col; i++)
	{
		if (source.attributes[i]->cResident == 1) attributes[i] = new non_resident_attr(*(non_resident_attr*)source.attributes[i]);
		else
		{
			switch(source.attributes[i]->dwAttrType)
			{
			case 0x10:
				attributes[i] = new std_info(*(std_info*)source.attributes[i]);
				break;
			default:
				attributes[i] = new std_attr_header(*source.attributes[i]);
				break;
			}
		}
	}
	return *this;
}


void file_descriptor::init(BYTE* buffer)
{
	int i;
	for (i = 0; i < 4; i++)
		signature[i] = buffer[i];
	memcpy(&wUpdtSeqOffset, buffer + 4, 4);
	memcpy(&n64LogFileSeq, buffer + 8, 8);
	memcpy(&wUseCnt, buffer + 16, 8);
	memcpy(&dwRealFileSize, buffer + 24, 8);
	memcpy(&n64RefToBaseFile, buffer + 32, 8);
	memcpy(&wNextAttrId, buffer + 40, 4);
	updateSeq = new WORD[wUpdtSeqSize-1];
	memcpy(updateSeq, buffer + 44, wUpdtSeqSize * 2 - 2);
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

bool file_descriptor::isDirectory() const
{
	for (int i = 0; i < attr_col;i++)
	{
		if (attributes[i]->dwAttrType == 0xA0) return true;
	}
	return false;
}

int file_descriptor::get_attr_pos(int signature) const
{
	for (int i = 0; i < attr_col; i++)
	{
		if (attributes[i]->dwAttrType == signature) return i;
	}
	return -1;
}

wchar_t* file_descriptor::get_file_name() const
{
	wchar_t* out;
	int pos = get_attr_pos(0x30);
	out = new wchar_t[attributes[pos]->attrContent[88]];
	for (int i = 0; i < attributes[pos]->attrContent[88] * 2; i+=2)
		memcpy(&out[i / 2], &attributes[pos]->attrContent[90 + i], 2);
	return out;
}
