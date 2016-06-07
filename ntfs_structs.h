#pragma once
#include <Windows.h>

typedef struct {
	BYTE chBootInd;
	BYTE chHead;
	BYTE chSector;
	BYTE chCylinder;
	BYTE chType;
	BYTE chLastHead;
	BYTE chLastSector;
	BYTE chLastCylinder;
	DWORD dwRelativeSector;
	DWORD dwNumberSectors;
} PARTITION;

typedef struct {
	char fileSignature[4];
	WORD wFixupOffset;
	WORD wFixupSize;
	LONGLONG n64LogSeqNumber;
	WORD wSequence;
	WORD wHardLinks;
	WORD wAttribOffset;
	WORD wFlags;
	DWORD dwRecLength;
	DWORD dwAllLength;
	LONGLONG n64BaseMftRec;
	WORD wNextAttrID;
	WORD wFixupPattern;
	DWORD dwMFTRecNumber;
} NTFS_MFT_FILE_ENTRY_HEADER;

typedef struct {
	char chJumpInstruction[3];
	char chOemID[4];
	char chDummy[4];
	struct NTFS_BPB {
		WORD wBytesPerSec;
		BYTE uchSecPerClust;
		WORD wReservedSec;
		BYTE uchReserved[3];
		WORD wUnused1;
		BYTE uchMediaDescriptor;
		WORD wUnused2;
		WORD wSecPerTrack;
		WORD wNumberOfHeads;
		DWORD dwHiddenSec;
		DWORD dwUnused3;
		DWORD dwUnused4;
		LONGLONG n64TotalSec;
		LONGLONG n64MFTLogicalClustNum; // ( cluster for MFT
		LONGLONG n64MFTMirrLogicalClustNum;
		int nClustPerMFTRecord;
		int nClustPerIndexRecord;
		LONGLONG n64VolumeSerialNum;
		DWORD dwChecksum;
	} bpb;
	char chBootstrapCode[426];
	WORD wSecMark;
} NTFS_PART_BOOT_SECTOR;

typedef struct
{
	BYTE buff[446];
	PARTITION parts[4];
	BYTE signature1, signature2;
} MBR_SECTOR;

typedef struct
{
	LONGLONG beg_rec_num;
	LONGLONG offset;
} MFT_FRAG;