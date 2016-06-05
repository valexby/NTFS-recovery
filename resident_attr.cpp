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



resident_attr::~resident_attr()
{
}
