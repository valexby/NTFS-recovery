#pragma once
#include "resident_attr.h"

class std_info :
	public resident_attr
{
public:
	std_info();
	std_info(BYTE* header);
	std_info(const std_info &);
	~std_info();
	std_info& operator=(const std_info&);
	LONGLONG n64FileCreateTime;
	LONGLONG n64FileModTime;
	LONGLONG n64MFTModTime;
	LONGLONG n64FileReadTime;
	DWORD dwFilePerm;
	DWORD dwMaxVersion;
	LONGLONG n64Version;
};

