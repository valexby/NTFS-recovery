#include "std_attr_header.h"
#include <string>


std_attr_header::std_attr_header(BYTE* buffer)
{
	init(buffer);
}

std_attr_header::std_attr_header()
{
	
}

std_attr_header::std_attr_header(const std_attr_header& source)
{
	dwAttrType = source.dwAttrType;
	dwFullAttrLen = source.dwFullAttrLen;
	cResident = source.cResident;
	cNameLen = source.cNameLen;
	wNameOffset = source.wNameOffset;
	wFlags = source.wFlags;
	wAtrrId = source.wAtrrId;
	name = new char[cNameLen];
	memcpy(name, source.name, cNameLen);
}


std_attr_header::~std_attr_header()
{
	delete[] name;
}

std_attr_header& std_attr_header::operator=(const std_attr_header& source)
{
	if (this != &source)
	{
		dwAttrType = source.dwAttrType;
		dwFullAttrLen = source.dwFullAttrLen;
		cResident = source.cResident;
		cNameLen = source.cNameLen;
		wNameOffset = source.wNameOffset;
		wFlags = source.wFlags;
		wAtrrId = source.wAtrrId;
		name = new char[cNameLen];
		memcpy(name, source.name, cNameLen);
	}
	return *this;
}

void std_attr_header::init(BYTE* buffer)
{
	memcpy(&dwAttrType, buffer, 8);
	memcpy(&cResident, buffer + 8, 2);
	memcpy(&wNameOffset, buffer + 10, 6);
	
	{
		
	}
	if (cNameLen != 0)
	{
		name = new char[cNameLen];
		memcpy(name, buffer + wNameOffset, cNameLen);
	}
}
