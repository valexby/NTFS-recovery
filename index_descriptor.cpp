#include "index_descriptor.h"
#include <string>
#include "raw_disk_read.h"


index_descriptor::index_descriptor()
{
}

index_descriptor::index_descriptor(const index_descriptor &source)
{
	memcpy(signature, source.signature, 4);
	memcpy(&wUpdtSeqOffset, &source.wUpdtSeqOffset, 4);
	memcpy(&n64LogFileSeq, &source.n64LogFileSeq, 16);
	memcpy(&dwEntiresOffset, &source.dwEntiresOffset, 12);
	cChildFlag = source.cChildFlag;
	wUptdSeqNum = source.wUptdSeqNum;
	uptd_seq = new WORD[wUpdtSeqSize - 1];
	memcpy(uptd_seq, source.uptd_seq, (wUpdtSeqSize - 1) * sizeof(WORD));
	entries_cnt = source.entries_cnt;
	entries = new index_record[entries_cnt];
	for (int i = 0; i < entries_cnt;i++)
	{
		entries[i] = source.entries[i];
	}
}

index_descriptor::index_descriptor(HANDLE disk, LONGLONG sector, LONGLONG size)
{
	try {
		BYTE* buffer = read_sector(disk, sector, size);
		//dump_buffer(buffer, size * 512, L"2INDEX");
		clean_sect_bord(buffer);
		init(buffer);
		entries_cnt = get_entries_cnt(buffer + dwEntiresOffset + 24);
		entries = new index_record[entries_cnt];
		int offset = dwEntiresOffset + 24;
		for (int i = 0; i < entries_cnt;i++)
		{
			entries[i] = index_record(buffer + offset);
			offset += entries[i].wSize;
		}
		delete[] buffer;
	}
	catch (std::string)
	{
		throw;
	}
}

int index_descriptor::get_entries_cnt(BYTE* start)
{
	int res = 0;
	DWORD buff;
	memcpy(&buff, start, 4);
	while (*(WORD*)(start + 12) != 2 || buff != 0)
	{
		res++;
		start += *(WORD*)(start + 8);
		memcpy(&buff, start, 4);
	}
	return --res;
}


void index_descriptor::clean_sect_bord(BYTE* raw)
{
	int i;
	for (i = 0; i < *(WORD*)(raw+6) - 1; i++)
	{
		memcpy(raw + (i + 1) * 512 - 2, raw + 42 + i*2, 2);
	}
}

void index_descriptor::init(BYTE* raw)
{
	memcpy(signature, raw, 4);
	memcpy(&wUpdtSeqOffset, raw + 4, 4);
	memcpy(&n64LogFileSeq, raw + 8, 16);
	memcpy(&dwEntiresOffset, raw + 24, 12);
	cChildFlag = *(raw + 36);
	wUptdSeqNum = *(raw + 40);
	uptd_seq = new WORD[wUpdtSeqSize-1];
	memcpy(uptd_seq, raw + 42, (wUpdtSeqSize-1)*2);
}


index_descriptor::~index_descriptor()
{
	delete[] uptd_seq;
}
