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
	resident_attr(const resident_attr &);
	resident_attr& operator=(const resident_attr&);
	virtual ~resident_attr();
};

