#include "resident_attr.h"



resident_attr::resident_attr()
{
}

resident_attr::resident_attr(BYTE* buffer) : std_attr_header(buffer)
{
	dwNonHeaderAttrLen = *(buffer + 16);
	wAttrOffset = *(buffer + 20);
	memcpy(&cIndexFlag, buffer + 22, 2);
}

resident_attr::resident_attr(const resident_attr& source)
{
	*this = source;
}

resident_attr& resident_attr::operator=(const resident_attr& source)
{
	std_attr_header::operator=(source);
	if (this == &source) return *this;
	dwNonHeaderAttrLen = source.dwNonHeaderAttrLen;
	wAttrOffset = source.wAttrOffset;
	cIndexFlag = source.cIndexFlag;
	cPadding = source.cPadding;
	return *this;
}


resident_attr::~resident_attr()
{
}
