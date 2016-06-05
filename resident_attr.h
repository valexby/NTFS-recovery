#pragma once
#include "std_attr_header.h"

class resident_attr :
	public std_attr_header
{
public:
	DWORD dwNonHeaderAttrLen;
	WORD wAttrOffset;
	BYTE cIndexFlag;
	BYTE cPadding;
	resident_attr();
	resident_attr(BYTE* buffer);
	virtual ~resident_attr() = 0;
};

