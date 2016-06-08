#pragma once
#include <Windows.h>
#include "std_attr_header.h"
#include <string>

using namespace std;
class non_resident_attr;

class file_descriptor {
public :
	char signature[4];
	WORD wUpdtSeqOffset;
	WORD wUpdtSeqSize;
	LONGLONG n64LogFileSeq;
	WORD wUseCnt;
	WORD wHardLinksCnt;
	WORD wFirstAttrOffset;
	WORD wFlags;
	DWORD dwRealFileSize;
	DWORD dwAllocSpace;
	LONGLONG n64RefToBaseFile;
	WORD wNextAttrId;
	WORD wUpdateSeqNumb;
	WORD* updateSeq = nullptr;
	std_attr_header** attributes = nullptr;
	int attr_col = 0;
	file_descriptor(HANDLE disk, LONGLONG sector);
	~file_descriptor();
	file_descriptor(const file_descriptor &);
	file_descriptor& operator=(const file_descriptor&);
	bool isDirectory() const;
	int get_attr_pos(int signature) const;
	wchar_t* get_file_name() const;
private :
	void init(BYTE* buffer);
	static std_attr_header* build_attr(BYTE* raw);
	static bool is_resident(BYTE* raw);
	static int get_attr_col(BYTE* start);
	void clean_sect_bord(BYTE* raw) const;
};
