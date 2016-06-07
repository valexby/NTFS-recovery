#include "std_info.h"



std_info::std_info()
{
}

std_info::std_info(const std_info &source)
{
	*this = source;
}

std_info& std_info::operator=(const std_info& source)
{
	if (this == &source) return *this;
	resident_attr::operator=(source);
	memcpy(&n64FileCreateTime, &source.n64FileCreateTime, 32);
	memcpy(&dwFilePerm, &source.dwFilePerm, 8);
	n64Version = source.n64Version;
	return *this;
}

std_info::std_info(BYTE* buffer) : resident_attr(buffer)
{
	if (cResident == 0)
	{
		buffer += wAttrOffset;
		memcpy(&n64FileCreateTime, buffer, 32);
		memcpy(&dwFilePerm, buffer + 32, 8);
		memcpy(&n64Version ,buffer + 40,8);
	}
}



std_info::~std_info()
{
}