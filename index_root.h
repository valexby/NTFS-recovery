#pragma once
#include "resident_attr.h"
class index_root :
	public resident_attr
{
public:
	DWORD dwIndexAttrType;
	DWORD dwCollationRule;
	DWORD dwAllocIndSize;
	BYTE cClustPerInd;
	BYTE padding[3];
	DWORD dwFirstEntryOff;
	DWORD dwTotalEnrtySize;
	BYTE cIndFlags;
	BYTE padding2;
	index_root();
	~index_root();
};

