#pragma once
#include <Windows.h>
#include "index_record.h"

class index_descriptor
{
public:
	BYTE signature[4];
	WORD wUpdtSeqOffset;
	WORD wUpdtSeqSize;
	LONGLONG n64LogFileSeq;
	LONGLONG n64IndexVCN;
	DWORD dwEntiresOffset;
	DWORD dwEntriesSize;
	DWORD dwEntriesAlloc;
	BYTE cChildFlag;
	BYTE padding[3];
	WORD wUptdSeqNum;
	WORD* uptd_seq = nullptr;
	index_record* entries = nullptr;
	int entries_cnt = 0;
	index_descriptor();
	index_descriptor(HANDLE disk, LONGLONG sector, LONGLONG size);
	index_descriptor(const index_descriptor &source);
	~index_descriptor();
private :
	void init(BYTE* raw);
	static void clean_sect_bord(BYTE* raw);
	static int get_entries_cnt(BYTE* start);
};

