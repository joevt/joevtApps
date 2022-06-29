//
//  Copyright (c) 2021  joevt   All rights reserved.
//
//  pciidsFile.h
//
//  License: BSD 2 clause License
//

#ifndef _PCIIDSFILE_H
#define _PCIIDSFILE_H

typedef struct _PciIdItem PciIdItem;

struct _PciIdItem {
	UINTN Value;
	CHAR16* Name;
	UINTN NumChildren;
	PciIdItem *Children;
};

extern PciIdItem *PciIds;

VOID
FreePciIds (
	VOID
);

EFI_STATUS
ReadPciIds (
	IN EFI_HANDLE SelfImageHandle
);


CONST PciIdItem *
PciIdsSearch (
	IN CONST PciIdItem *PciIds,
	IN UINTN value
);


VOID
GetPciIdsNames (
	IN CONST PciIdItem *PciIds,
	IN UINTN value1,
	IN UINTN value2,
	IN UINTN value3,
	IN OUT CONST CHAR16 **name1,
	IN OUT CONST CHAR16 **name2,
	IN OUT CONST CHAR16 **name3
);


#endif
