//
//  Copyright (c) 2021  joevt   All rights reserved.
//
//  pciidsFile.c
//
//  License: BSD 2 clause License
//

#include "pciidsFile.h"

#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>

#include <Guid/FileInfo.h>


/*
	Create a string from all the file path nodes at the end of a device path.
*/
CHAR16 *
GetPathParts (
	IN EFI_DEVICE_PATH_PROTOCOL *FilePath
)
{
	EFI_DEVICE_PATH_PROTOCOL *n1;
	EFI_DEVICE_PATH_PROTOCOL *n2 = NULL;
	CHAR16* p = NULL;
	UINTN PathSize = 0;
	for (n1 = FilePath; !IsDevicePathEnd (n1); n1 = NextDevicePathNode (n1)) {
		if (n1->Type == MEDIA_DEVICE_PATH && n1->SubType == MEDIA_FILEPATH_DP) {
			PathSize += StrSize (((FILEPATH_DEVICE_PATH *)n1)->PathName);
			if (!n2) n2 = n1;
		}
		else {
			n2 = NULL;
			PathSize = 0;
		}
	}
	if (n2) {
		p = AllocatePool (PathSize);
		CHAR16* c = p;
		for (n1 = n2; !IsDevicePathEnd (n1); n1 = NextDevicePathNode (n1)) {
			c += UnicodeSPrint (c, PathSize, L"%s%s", c == p ? L"" : L"\\", ((FILEPATH_DEVICE_PATH *)n1)->PathName);
		}
	}
	return p;
}


CHAR16 *
ReplaceBaseName (
	IN CHAR16* FilePath,
	IN CHAR16* FileName
)
{
	CHAR16 *NewFilePath = NULL;
	UINTN PathLen = StrLen (FilePath);
	CHAR16 *e = FilePath + PathLen - 1;
	for (CHAR16 *c = e; c >= FilePath - 1; c--) {
		if (c < FilePath || *c == L'\\') {
			NewFilePath = CatSPrint (NULL, L"%.*s%s", c - FilePath + 1, FilePath, FileName);
			break;
		}
	}
	return NewFilePath;
}


EFI_FILE_INFO *
LibFileInfo (
	IN EFI_FILE_HANDLE FileHandle
)
{
	EFI_STATUS    Status;
	EFI_FILE_INFO *FileInfo = NULL;
	UINTN         Size = 0;

	Status = FileHandle->GetInfo (FileHandle, &gEfiFileInfoGuid, &Size, FileInfo);
	if (Status == EFI_BUFFER_TOO_SMALL) {
		UINTN NewSize = Size + sizeof (CHAR16); // see explanation at OpenCorePkg/Library/OcFileLib/GetFileInfo.c
		UINTN FinalSize = NewSize;
		FileInfo = AllocatePool (NewSize);
		Status = FileHandle->GetInfo (FileHandle, &gEfiFileInfoGuid, &FinalSize, FileInfo);
	}

	return EFI_ERROR (Status) ? NULL : FileInfo;
}


EFI_STATUS
ReadFile (
	IN EFI_HANDLE SelfImageHandle,
	IN CHAR16 *FileName,
	IN OUT VOID **Buffer,
	IN OUT UINT64 *BufferSize
)
{
	DEBUG ((DEBUG_INFO, "[ %a FileName:'%s'\n", __FUNCTION__, FileName));

	EFI_STATUS Status = EFI_SUCCESS;
	
	if (!Buffer) {
		Status = EFI_INVALID_PARAMETER;
		DEBUG ((DEBUG_ERROR, "%a %r\n", __FUNCTION__, Status));
	}

	EFI_LOADED_IMAGE *SelfLoadedImage = NULL;
	if (!EFI_ERROR (Status)) {
		Status = gBS->HandleProtocol(
			SelfImageHandle,
			&gEfiLoadedImageProtocolGuid,
			(VOID **) &SelfLoadedImage
		);
		DEBUG ((DEBUG_INFO, "%a LoadedImageProtocol %r\n", __FUNCTION__, Status));
	}

	CHAR16 *SelfFilePath = NULL;
	if (!EFI_ERROR (Status)) {
		SelfFilePath = GetPathParts (SelfLoadedImage->FilePath);
		if (!SelfFilePath) Status = EFI_INVALID_PARAMETER;
		DEBUG ((DEBUG_INFO, "%a SelfFilePath %r '%s'\n", __FUNCTION__, Status, SelfFilePath));
	}

	CHAR16 *FilePath = NULL;	
	if (!EFI_ERROR (Status)) {
		FilePath = ReplaceBaseName (SelfFilePath, FileName);
		if (!FilePath) Status = EFI_INVALID_PARAMETER;
		DEBUG ((DEBUG_INFO, "%a ReadFilePath %r '%s'\n", __FUNCTION__, Status, FilePath));
	}

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Volume = NULL;
	if (!EFI_ERROR (Status)) {
		Status = gBS->HandleProtocol (
			SelfLoadedImage->DeviceHandle,
			&gEfiSimpleFileSystemProtocolGuid,
			(VOID **) &Volume
		);
		DEBUG ((DEBUG_INFO, "%a SimpleFileSystemProtocol %r\n", __FUNCTION__, Status));
	}

	EFI_FILE_HANDLE SelfRootDir = NULL;
	if (!EFI_ERROR (Status)) {
		Status = Volume->OpenVolume (Volume, &SelfRootDir);
		DEBUG ((DEBUG_INFO, "%a OpenVolume %r\n", __FUNCTION__, Status));
	}

	EFI_FILE_PROTOCOL *FileHandle = NULL;
	if (!EFI_ERROR (Status)) {
		Status = SelfRootDir->Open(
			SelfRootDir,
			&FileHandle,
			FilePath,
			EFI_FILE_MODE_READ,
			0
		);
		DEBUG ((DEBUG_INFO, "%a Dir->Open %r\n", __FUNCTION__, Status));
	}
	
	EFI_FILE_INFO *FileInfo = NULL;
	if (!EFI_ERROR (Status)) {
		FileInfo = LibFileInfo (FileHandle);
		if (!FileInfo) Status = EFI_LOAD_ERROR;
		DEBUG ((DEBUG_INFO, "%a FileInfo %r\n", __FUNCTION__, Status));
	}
	
	UINT64 FileSize = 0;
	*Buffer = NULL;
	if (!EFI_ERROR (Status)) {
		FileSize = FileInfo->FileSize;
		*Buffer = AllocatePool (FileSize);
		if (!*Buffer) Status = EFI_OUT_OF_RESOURCES;
		DEBUG ((DEBUG_INFO, "%a AllocatePool %r Size:%d\n", __FUNCTION__, Status, FileSize));
	}
	if (FileInfo) FreePool (FileInfo);
	
	if (!EFI_ERROR (Status)) {
		Status = FileHandle->Read (FileHandle, &FileSize, *Buffer);
		DEBUG ((DEBUG_INFO, "%a Read %r Size:%d\n", __FUNCTION__, Status, FileSize));
	}

	if (FileHandle) FileHandle->Close (FileHandle);
	if (SelfRootDir) SelfRootDir->Close (SelfRootDir);
	if (FilePath) FreePool (FilePath);
	if (SelfFilePath) FreePool (SelfFilePath);
	
	if (BufferSize) *BufferSize = FileSize;
	
	DEBUG ((DEBUG_INFO, "] %a %r\n", __FUNCTION__, Status));
	return Status;
}

PciIdItem PciIdsFile[] = {
	{0, L"Vendors", 0, NULL},
	{1, L"Classes", 0, NULL},
};


PciIdItem *PciIds = &PciIdsFile[0];


static CHAR16* pciidsBuffer = NULL;
static PciIdItem *pciidsFile = NULL;

VOID
FreePciIds (
	VOID
)
{
	if (pciidsBuffer) {
		FreePool (pciidsBuffer);
		pciidsBuffer = NULL;
	}
	if (pciidsFile) {
		FreePool (pciidsFile);
		pciidsFile = NULL;
	}
}


EFI_STATUS
ReadPciIds (
	IN EFI_HANDLE SelfImageHandle
)
{
	DEBUG ((DEBUG_INFO, "[ %a\n", __FUNCTION__));

	UINT64 BufferSize;
	EFI_STATUS Status;
	
	Status = ReadFile (SelfImageHandle, L"pci.ids.ucs-2le", (VOID**)&pciidsBuffer, &BufferSize);
	if (EFI_ERROR (Status)) {
		goto Done;
	}

	UINTN vendorCount = 0;
	UINTN deviceCount = 0;
	UINTN subsystemCount = 0;
	UINTN classCount = 0;
	UINTN subclassCount = 0;
	UINTN progifCount = 0;

	CHAR16 *BufferEnd = (VOID*)pciidsBuffer + BufferSize;
	CHAR16 *c;
	CHAR16 *l;
	
	#define istab(x) l[x] == L'\t'
	#define isspc(x) l[x] == L' '
	#define iscom(x) l[x] == L'#'
	#define ishex(x) l[x] > L'#'
	#define get4(x) ((l[x] - ((l[x] < L'a') ? L'0' : (L'a' - 10))))
	#define get8(x) ((get4(x) << 4) | get4(x+1))
	#define get16(x) ((get8(x) << 8) | get8(x+2))
	#define get32(x) ((get16(x) << 16) | get16(x+5))
	#define type8 UINT8
	#define type16 UINT16
	#define type32 UINT32
	#define doincs(x) \
		switch (x) { \
			case 7: if (class->Name) class++; \
			case 6: if (subclass->Name) subclass++; \
			case 5: if (progif->Name) progif++; \
			case 4: if (vendor->Name) vendor++; \
			case 3: if (device->Name) device++; \
			case 2: if (subsystem->Name) subsystem++; \
		} while (0)

	#define initone(t, b, v, r) { *(type ## b *)(&l[2]) = get ## b(v); l[0] = t; r ## Count++ ; }
	#define doone(b, n, r, p) { doincs(*l); r->Value = *(type ## b *)(&l[2]); r->Name = &l[n]; if (!(p)->Children) (p)->Children = r; (p)->NumChildren++; break; }

	CHAR16 *start = pciidsBuffer;
	if (start[0] == 0xfeff) start++; // skip the BOM

	l = start;
	for (c = start; c < BufferEnd; c++) {
		if (*c == L'\n') {
			*c = L'\0';
			if (l[0]) {
				// non-zero length lines
				/**/ if (iscom(0)) { l[0] = 1; }
				else {
					// lines with values
					/**/ if (isspc(1)) initone(7,  8, 2, class    )
					else if (ishex(0)) initone(4, 16, 0, vendor   )
					else if (istab(1)) // two tabs
					/**/ if (isspc(4)) initone(5,  8, 2, progif   )
						else           initone(2, 32, 2, subsystem)
					else // one tab
					/**/ if (isspc(3)) initone(6,  8, 1, subclass )
						else           initone(3, 16, 1, device   )
				}
				l[1] = c - l + 1;
			}
			l = c + 1;
		}
	}

	// allow an extra empty record for each of the 6 types
	pciidsFile = AllocateZeroPool ((vendorCount + deviceCount + subsystemCount + classCount + subclassCount + progifCount + 6) * sizeof(PciIdItem));
	if (!pciidsFile) {
		FreePciIds ();
		Status = EFI_OUT_OF_RESOURCES;
		goto Done;
	}

	PciIdItem *vendor = pciidsFile;
	PciIdItem *device = vendor + vendorCount + 1; // allow an extra empty record for each type
	PciIdItem *subsystem = device + deviceCount + 1;
	PciIdItem *class = subsystem + subsystemCount + 1;
	PciIdItem *subclass = class + classCount + 1;
	PciIdItem *progif = subclass + subclassCount + 1;
	
	DEBUG ((DEBUG_INFO,
		"vendors:%d devices:%d subsystems:%d classes:%d subclasses:%d progifs:%d\n",
		vendorCount, deviceCount, subsystemCount, classCount, subclassCount, progifCount
	));

	for (l = start; l < BufferEnd;) {
		if (*l == 0) l++;
		else {
			switch (*l) {
				case 7: doone( 8,  6, class    , &PciIds[1])
				case 4: doone(16,  6, vendor   , &PciIds[0])
				case 5: doone( 8,  6, progif   , subclass  )
				case 2: doone(32, 13, subsystem, device    )
				case 6: doone( 8,  5, subclass , class     )
				case 3: doone(16,  7, device   , vendor    )
			}
			l += l[1];
		}
	}

Done:
	DEBUG_CODE (
		if (EFI_ERROR(Status)) DEBUG ((DEBUG_ERROR, "%a %r\n", __FUNCTION__, Status));
	);
	DEBUG ((DEBUG_INFO, "] %a\n", __FUNCTION__));
	return Status;
}


CONST PciIdItem *
PciIdsSearch (
	IN CONST PciIdItem *PciIds,
	IN UINTN value
)
{
	if (PciIds && PciIds->Children) {
		UINTN MinIndex = 0;
		UINTN MaxIndex = PciIds->NumChildren;
		while (MaxIndex > MinIndex) {
			UINTN TestIndex = (MaxIndex + MinIndex) >> 1;
			UINTN FoundValue = PciIds->Children[TestIndex].Value;
			if (FoundValue == value) {
				return &PciIds->Children[TestIndex];
			}
			if (FoundValue < value) {
				MinIndex = TestIndex + 1;
				continue;
			}
			MaxIndex = TestIndex;
		} // while
	} // if
	return NULL;
}


VOID
GetPciIdsNames (
	IN CONST PciIdItem *PciIds,
	IN UINTN value1,
	IN UINTN value2,
	IN UINTN value3,
	IN OUT CONST CHAR16 **name1,
	IN OUT CONST CHAR16 **name2,
	IN OUT CONST CHAR16 **name3
)
{
	if (name1) *name1 = NULL;
	if (name2) *name2 = NULL;
	if (name3) *name3 = NULL;
	PciIds = PciIdsSearch (PciIds, value1); if (!PciIds) return; if (name1) *name1 = PciIds->Name;
	PciIds = PciIdsSearch (PciIds, value2); if (!PciIds) return; if (name2) *name2 = PciIds->Name;
	PciIds = PciIdsSearch (PciIds, value3); if (!PciIds) return; if (name3) *name3 = PciIds->Name;
}
