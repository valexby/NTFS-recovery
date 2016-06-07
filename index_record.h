#pragma once
#include <windows.h>

class index_record
{
public:
	//LONGLONG n64MFTRecNum;
	DWORD dwMFTRecNum;
	WORD wSize;
	WORD wNameOffset;
	WORD wFlag;
	BYTE padding[2];
	LONGLONG n64ParentMFT;
	LONGLONG n64CreateTime;
	LONGLONG n64ModTime;
	LONGLONG n64MFTModTime;
	LONGLONG n64LastAccessTime;
	LONGLONG n64AllocMod;
	LONGLONG n64RealSize;
	LONGLONG n64FileFlags;
	BYTE cNameLength;
	BYTE cNamespace;
	BYTE* name = nullptr;
	index_record();
	index_record(BYTE* raw);
	index_record(const index_record&);
	~index_record();
	index_record& operator=(const index_record&);
};

