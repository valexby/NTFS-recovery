#pragma once
#include <Windows.h>

class std_attr_header
{
public:
	DWORD dwAttrType;
	DWORD dwFullAttrLen;
	BYTE cResident;
	BYTE cNameLen;
	WORD wNameOffset;
	WORD wFlags;
	WORD wAtrrId;
	BYTE* attrContent = nullptr;
	int attr_cont_len;
	char* name = nullptr;
	std_attr_header(BYTE* buffer);
	std_attr_header();
	std_attr_header(const std_attr_header &);
	virtual ~std_attr_header();
	std_attr_header& operator=(const std_attr_header&);
private:
	void init(BYTE* buffer);
};

