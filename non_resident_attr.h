#pragma once
#include "std_attr_header.h"
class non_resident_attr :
	public std_attr_header
{
public:
	LONGLONG n64InitVCN;
	LONGLONG n64FinalVCN;
	WORD wDataRunOffset;
	WORD wCompressUnitLen;
	DWORD dwPadding;
	LONGLONG n64AttrAllocSize;
	LONGLONG n64AttrRealSize;
	LONGLONG n64InitDataLen;
	attr_data_run* dataRuns = nullptr;
	int runs_col = 0;
	non_resident_attr();
	non_resident_attr(BYTE* raw);
	~non_resident_attr();
private:
	static int get_runs_col(BYTE* raw);
};

