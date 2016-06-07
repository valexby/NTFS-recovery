#include "index_record.h"

index_record::index_record()
{
}

index_record::index_record(BYTE* raw)
{
	memcpy(&dwMFTRecNum, raw, 4);
	memcpy(&wSize, raw + 8, 8);
	memcpy(&n64ParentMFT, raw + 16, 64);
	memcpy(&cNameLength, raw + 80, 2);
	name = new BYTE[cNameLength*2+1];
	memcpy(name, raw + 82, cNameLength*2);
	name[cNameLength*2] = 0;
}

index_record::index_record(const index_record& source)
{
	*this = source;
}

index_record& index_record::operator=(const index_record& source)
{
	if (this == &source) return *this;
	dwMFTRecNum = source.dwMFTRecNum;
	memcpy(&wSize, &source.wSize, 8);
	memcpy(&n64ParentMFT, &source.n64ParentMFT, 64);
	memcpy(&cNameLength, &source.cNameLength, 2);
	name = new BYTE[cNameLength * 2 + 1];
	memcpy(name, source.name, cNameLength * 2);
	name[cNameLength * 2] = 0;
	return *this;
}



index_record::~index_record()
{
	delete[] name;
}
