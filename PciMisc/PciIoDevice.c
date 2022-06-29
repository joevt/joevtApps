//
//  Copyright (c) 2021  joevt   All rights reserved.
//
//  FixPCIeLinkRate 
//
//  License: BSD 2 clause License
//
//  Portions Copyright (c) 2016-2017, Microsoft Corporation
//           Copyright (c) 2018, Intel Corporation. All rights reserved.
//           See relevant code in EDK11 for exact details
//


//========================================================================================
// Includes

#include "PciIoDevice.h"

#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/SortLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>


//========================================================================================
// from PciCommand.c

/**
  Check the capability supporting by given device.

  @param PciIoDevice   Pointer to instance of PCI_IO_DEVICE.

  @retval TRUE         Capability supported.
  @retval FALSE        Capability not supported.

**/
BOOLEAN
PciCapabilitySupport (
  IN PCI_IO_DEVICE  *PciIoDevice
  )
{
  if ((PciIoDevice->Pci.Hdr.Status & EFI_PCI_STATUS_CAPABILITY) != 0) {
    return TRUE;
  }

  return FALSE;
}

/**
  Locate capability register block per capability ID.

  @param PciIoDevice       A pointer to the PCI_IO_DEVICE.
  @param CapId             The capability ID.
  @param Offset            A pointer to the offset returned.
  @param NextRegBlock      A pointer to the next block returned.

  @retval EFI_SUCCESS      Successfully located capability register block.
  @retval EFI_UNSUPPORTED  Pci device does not support capability.
  @retval EFI_NOT_FOUND    Pci device support but can not find register block.

**/
EFI_STATUS
LocateCapabilityRegBlock (
	IN PCI_IO_DEVICE *PciIoDevice,
	IN UINT8 CapId,
	IN OUT UINT8 *Offset,
	OUT UINT8 *NextRegBlock OPTIONAL
)
{
	UINT8 CapabilityPtr;
	UINT16 CapabilityEntry;
	UINT8 CapabilityID;
	EFI_STATUS Status;

	//
	// To check the capability of this device supports
	//
	if (!PciCapabilitySupport (PciIoDevice)) {
		return EFI_UNSUPPORTED;
	}

	if (*Offset != 0) {
		CapabilityPtr = *Offset;
	}
	else {
		CapabilityPtr = PciRead8 (Status, PciIoDevice, IS_CARDBUS_BRIDGE (&PciIoDevice->Pci) ? EFI_PCI_CARDBUS_BRIDGE_CAPABILITY_PTR : PCI_CAPBILITY_POINTER_OFFSET);
	}

	while ((CapabilityPtr >= 0x40) && ((CapabilityPtr & 0x03) == 0x00)) {
		CapabilityEntry = PciRead16 (Status, PciIoDevice, CapabilityPtr);
		CapabilityID = (UINT8) CapabilityEntry;
		if (CapabilityID == CapId) {
			*Offset = CapabilityPtr;
			if (NextRegBlock != NULL) {
				*NextRegBlock = (UINT8) (CapabilityEntry >> 8);
			}

			return EFI_SUCCESS;
		}

		//
		// Certain PCI device may incorrectly have capability pointing to itself,
		// break to avoid dead loop.
		//
		if (CapabilityPtr == (UINT8) (CapabilityEntry >> 8)) {
			break;
		}

		CapabilityPtr = (UINT8) (CapabilityEntry >> 8);
	}
	
	Status = EFI_NOT_FOUND;
	return Status;
}

/**
  Locate PciExpress capability register block per capability ID.

  @param PciIoDevice       A pointer to the PCI_IO_DEVICE.
  @param CapId             The capability ID.
  @param Offset            A pointer to the offset returned.
  @param NextRegBlock      A pointer to the next block returned.

  @retval EFI_SUCCESS      Successfully located capability register block.
  @retval EFI_UNSUPPORTED  Pci device does not support capability.
  @retval EFI_NOT_FOUND    Pci device support but can not find register block.

**/
EFI_STATUS
LocatePciExpressCapabilityRegBlock (
	IN     PCI_IO_DEVICE *PciIoDevice,
	IN     UINT16 CapId,
	IN OUT UINT32 *Offset,
	   OUT UINT32 *NextRegBlock OPTIONAL
	)
{
	EFI_STATUS Status;
	UINT32 CapabilityPtr;
	UINT32 CapabilityEntry;
	UINT16 CapabilityID;

	//
	// To check the capability of this device supports
	//
	if (!PciIoDevice->IsPciExp) {
		return EFI_UNSUPPORTED;
	}

	if (*Offset != 0) {
		CapabilityPtr = *Offset;
	} else {
		CapabilityPtr = EFI_PCIE_CAPABILITY_BASE_OFFSET;
	}

	while (CapabilityPtr != 0) {
		//
		// Mask it to DWORD alignment per PCI spec
		//
		CapabilityPtr &= 0xFFC;
		CapabilityEntry = PciRead32 (Status, PciIoDevice, CapabilityPtr);

		if (CapabilityEntry == MAX_UINT32) {
			DEBUG ((
				DEBUG_WARN,
				"%a: [%02x|%02x|%02x] failed to access config space at offset 0x%x\n",
				__FUNCTION__,
				PciIoDevice->BusNumber,
				PciIoDevice->DeviceNumber,
				PciIoDevice->FunctionNumber,
				CapabilityPtr
			));
			break;
		}

		CapabilityID = (UINT16) CapabilityEntry;

		if (CapabilityID == CapId) {
			*Offset = CapabilityPtr;
			if (NextRegBlock != NULL) {
				*NextRegBlock = (CapabilityEntry >> 20) & 0xFFF;
			}

			return EFI_SUCCESS;
		}

		CapabilityPtr = (CapabilityEntry >> 20) & 0xFFF;
	}

	Status = EFI_NOT_FOUND;
	return Status;
}


//========================================================================================
// Pci bus re-enumeration

STATIC
VOID
SaveBus (
	PCI_IO_DEVICE *b
) {
	b->SavePciIo     = b->PciIo    ;
	b->SaveBusNumber = b->BusNumber;
	if (b->PciType1) {
		b->SavePrimaryBus      = b->PciType1->Bridge.PrimaryBus    ;
		b->SaveSecondaryBus    = b->PciType1->Bridge.SecondaryBus  ;
		b->SaveSubordinateBus  = b->PciType1->Bridge.SubordinateBus;
	}
}


#if DO_SEARCHFORHIDDENDEVICES

STATIC
VOID
RenumberBuses (
	PCI_IO_DEVICE *b
) {
	if (b->IsSegment) {
	}
	else {
		RenumberBuses (b->Parent);

		DEBUG_CODE (
			Print (L"Renumber from  ");
			OutputPciInfo (b, L"", 13, FALSE);
			Print (L"\n");
		);
		if (b->Parent->IsSegment) {
			b->NewBusNumber = b->BusNumber;
			b->NewPrimaryBus = b->PciType1->Bridge.PrimaryBus;
			b->NewSecondaryBus = b->Parent->BigUnusedStart;
			b->NewSubordinateBus = b->Parent->BigUnusedEnd;
		}
		else {
			b->NewPrimaryBus = b->Parent->NewSecondaryBus;
			b->NewSecondaryBus = b->NewPrimaryBus < PCI_MAX_BUS ? b->NewPrimaryBus + 1 : PCI_MAX_BUS;
			b->NewSubordinateBus = b->Parent->NewSubordinateBus;
		}
		PCI_IO_DEVICE *c;
		for (c = b->FirstChild; c; c = c->NextSibling) {
			c->NewBusNumber = b->NewSecondaryBus;
			DEBUG_CODE (
				Print (L"Child bus change: %02x -> %02x\n", c->BusNumber, c->NewBusNumber);
			);
		}
		DEBUG_CODE (
			Print (L"Renumber to    ");
			OutputPciInfo (b, L"", 13, TRUE);
			Print (L"\n");
		);
	}
}


STATIC
VOID
UnrenumberBuses (
	PCI_IO_DEVICE *b
) {
	if (b->IsSegment) {
	}
	else {
		UnrenumberBuses (b->Parent);

		DEBUG_CODE (
			Print (L"Restore from  ");
			OutputPciInfo (b, L"", 13, FALSE);
			Print (L"\n");
		);

		b->NewBusNumber      = b->SaveBusNumber     ;
		b->NewPrimaryBus     = b->SavePrimaryBus    ;
		b->NewSecondaryBus   = b->SaveSecondaryBus  ;
		b->NewSubordinateBus = b->SaveSubordinateBus;

		PCI_IO_DEVICE *c;
		for (c = b->FirstChild; c; c = c->NextSibling) {
			c->NewBusNumber = c->SaveBusNumber;
			DEBUG_CODE (
				Print (L"Child bus change: %02x -> %02x\n", c->BusNumber, c->NewBusNumber);
			);
		}

		DEBUG_CODE (
			Print (L"Restore to    ");
			OutputPciInfo (b, L"", 13, TRUE);
			Print (L"\n");
		);
	}
}


STATIC
VOID
WriteBuses (
	PCI_IO_DEVICE *b,
	BOOLEAN Restore
) {
	if (b->IsSegment) {
	}
	else {
		if (!Restore) {
			WriteBuses (b->Parent, Restore);
			b->PciIo = NULL;
		}

		DEBUG_CODE (
			Print (L"Write  ");
			OutputPciInfo (b, L"", 13, FALSE);
			Print (L"\n");
		);

		#define BusChange(Name, NAME) \
		do { \
			if (b->New ## Name ## Bus != b->PciType1->Bridge.Name ## Bus ) { \
				PciWrite8 (Status, b, PCI_BRIDGE_ ## NAME ## _BUS_REGISTER_OFFSET, b->New ## Name ## Bus); \
				b->PciType1->Bridge.Name ## Bus = b->New ## Name ## Bus; \
			} \
		} \
		while (0)
		
		EFI_STATUS Status;
		BusChange (Primary    , PRIMARY    );
		BusChange (Secondary  , SECONDARY  );
		BusChange (Subordinate, SUBORDINATE);
		
		PCI_IO_DEVICE *c;
		for (c = b->FirstChild; c; c = c->NextSibling) {
			DEBUG_CODE (
				Print (L"Child bus change: %02x -> %02x\n", c->BusNumber, c->NewBusNumber);
			);
			c->BusNumber = c->NewBusNumber;
		}

		if (Restore) {
			WriteBuses (b->Parent, Restore);
			b->PciIo = b->SavePciIo;
		}
	}
}


STATIC
EFI_STATUS
MoveBuses (
	PCI_IO_DEVICE *b
) {
	EFI_STATUS Status = EFI_SUCCESS;
	RenumberBuses (b);
	if (b->NewSecondaryBus == PCI_MAX_BUS || b->NewSubordinateBus < b->NewSecondaryBus) {
		Status = EFI_OUT_OF_RESOURCES;
	}
	else {
		WriteBuses (b, FALSE);
	}
	return Status;
}


STATIC
EFI_STATUS
UnmoveBuses (
	PCI_IO_DEVICE *b
) {
	UnrenumberBuses (b);
	WriteBuses (b, TRUE);
	return EFI_SUCCESS;
}


STATIC
EFI_STATUS
VerifyBuses (
	PCI_IO_DEVICE *b,
	BOOLEAN *MovedBuses
)
{
	EFI_STATUS Status = EFI_SUCCESS;
	*MovedBuses = FALSE;

	BOOLEAN NeedToMoveBuses = FALSE;
	for (PCI_IO_DEVICE *c = b; !c->IsSegment; c = c->Parent) {
		if (
			   c->PciType1->Bridge.SecondaryBus == 0 || c->PciType1->Bridge.SubordinateBus == 0
			|| c->BusNumber < c->Parent->PciType1->Bridge.SecondaryBus || c->BusNumber > c->Parent->PciType1->Bridge.SubordinateBus
		)
		NeedToMoveBuses = TRUE;
		break;
	}

	if (NeedToMoveBuses) {
		DEBUG ((DEBUG_INFO, "[ Move bus\n"));
		Status = MoveBuses (b);
		if (EFI_ERROR(Status)) {
			DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: MoveBuses Result %r.\n", Status));
		}
		if (!EFI_ERROR(Status)) {
			*MovedBuses = TRUE;
		}
		DEBUG ((DEBUG_INFO, "] Move bus %r\n", Status));
	}
	return Status;
}

#endif


//========================================================================================
// Pci device stringification

CHAR8 *
GetLocationString (
	PCI_IO_DEVICE *PciIoDevice,
	BOOLEAN UseNew
)
{
	CHAR8 *location = AllocatePool (8); // "ff:1f:7"
	AsciiSPrint (
		location, 8, "%02x:%02x.%x",
		UseNew ? PciIoDevice->NewBusNumber : PciIoDevice->BusNumber,
		PciIoDevice->DeviceNumber,
		PciIoDevice->FunctionNumber
	);
	return location;
}


CHAR8 *
GetBusesString (
	PCI_IO_DEVICE *PciIoDevice,
	BOOLEAN UseNew
)
{
	CHAR8 *buses = AllocatePool (9); // "-[00-ff]"
	if (!PciIoDevice || !PciIoDevice->PciType1 || (!IS_PCI_BRIDGE(PciIoDevice->PciType1) && !IS_CARDBUS_BRIDGE(PciIoDevice->PciType1))) {
		AsciiSPrint (buses, 9, "");
	}
	else {
		UINT8 SecondaryBus = UseNew ? PciIoDevice->NewSecondaryBus : PciIoDevice->PciType1->Bridge.SecondaryBus;
		UINT8 SubordinateBus = UseNew ? PciIoDevice->NewSubordinateBus : PciIoDevice->PciType1->Bridge.SubordinateBus;
		if (SecondaryBus == SubordinateBus) {
			AsciiSPrint (buses, 9, "-[%02x]", SecondaryBus);
		}
		else {
			AsciiSPrint (buses, 9, "-[%02x-%02x]", SecondaryBus, SubordinateBus);
		}
	}
	return buses;
}


CHAR8 *
GetLinksString (
	PCI_CAPABILITY_PCIEXP *PciExpressCap
)
{
	CHAR8 *links = AllocatePool (14); // "g4x16 > g1x16"
	if (
		  PciExpressCap->LinkCapability.Bits.MaxLinkSpeed == PciExpressCap->LinkStatus.Bits.CurrentLinkSpeed
		&& PciExpressCap->LinkCapability.Bits.MaxLinkWidth == PciExpressCap->LinkStatus.Bits.NegotiatedLinkWidth
	) {
		AsciiSPrint (
			links, 14, "g%dx%d",
			PciExpressCap->LinkCapability.Bits.MaxLinkSpeed,
			PciExpressCap->LinkCapability.Bits.MaxLinkWidth
		);
	}
	else {
		AsciiSPrint (
			links, 14, "g%dx%d > g%dx%d",
			PciExpressCap->LinkCapability.Bits.MaxLinkSpeed,
			PciExpressCap->LinkCapability.Bits.MaxLinkWidth,
			PciExpressCap->LinkStatus.Bits.CurrentLinkSpeed,
			PciExpressCap->LinkStatus.Bits.NegotiatedLinkWidth
		);
	}
	return links;
}


CHAR8 *
GetInfoString (
	PCI_DEVICE_INDEPENDENT_REGION *Hdr
)
{
	CHAR8 *info = AllocatePool (21); // "[1234:1234] [123456]"
	AsciiSPrint (
		info, 21, "[%04x:%04x] [%02x%02x%02x]",
		Hdr->VendorId,
		Hdr->DeviceId,
		Hdr->ClassCode[2],
		Hdr->ClassCode[1],
		Hdr->ClassCode[0]
	);
	return info;
}

#if 0
// How to make UEFI Shell output unicode characters? These can be piped to a text file at least.
#define kPciTreeTop       L"┬"  //   0x252C
#define kPciTreeSibling   L"├"  //   0x251C
#define kPciTreeUncle     L"│"  //   0x2502
#define kPciTreeNoSibling L"└"  //   0x2514
#define kPciTreeNoUncle   L" "  //     0x20
#define kPciTreeBridge     L"┬" //   0x252C
#define kPciTreeEndPoint   L"─" //   0x2500
#define kIndentTop           0
#define kIndentAncestor      1
#define kIndentSelf          0
#endif

#if 1
#define kPciTreeTop       L"+"
#define kPciTreeSibling   L"+"
#define kPciTreeUncle     L"|"
#define kPciTreeNoSibling L"`"
#define kPciTreeNoUncle   L" "
#define kPciTreeBridge     L"+"
#define kPciTreeEndPoint   L"-"
#define kIndentTop           0
#define kIndentAncestor      1
#define kIndentSelf          0
#endif

#if 0
//                       xxxxxxxxx              kIndentTop
#define kPciTreeTop    L"---------+======"
//                                xx            kIndentAncestor
#define kPciTreeSibling         L"+-"
#define kPciTreeUncle           L"| "
#define kPciTreeNoSibling       L"`-"
#define kPciTreeNoUncle         L"  "
//                                  xxx        kIndentSelf
#define kPciTreeBridge            L"---+-------"
#define kPciTreeEndPoint          L"-----------"

#define kIndentTop           9
#define kIndentAncestor      2
#define kIndentSelf          3
#endif

#define STR_LEN(x) (ARRAY_SIZE(x) - 1)


VOID
OutputPciInfo (
	PCI_IO_DEVICE *PciIoDevice,
	CHAR16 *IndentString,
	UINTN IndentMaxLength,
	BOOLEAN UseNew
)
{
	CHAR8 *location = GetLocationString (PciIoDevice, UseNew);
	CHAR8 *buses = GetBusesString (PciIoDevice, UseNew);
	CHAR8 *info = GetInfoString (&PciIoDevice->Pci.Hdr);
	CHAR8 *links = NULL;
	if (PciIoDevice->IsPciExp) {
		links = GetLinksString (&PciIoDevice->PciExpressCap);
	}
	CHAR16 *classes = NULL;
	CHAR16 *devices = NULL;
#if DO_PCIIDS
	#define MaxStrSize 400
	classes = AllocatePool(MaxStrSize);
	devices = AllocatePool(MaxStrSize);
	classes[0] = 0;
	devices[0] = 0;

	CONST CHAR16 *Vendor = NULL;
	CONST CHAR16 *Device = NULL;
	CONST CHAR16 *SubSystem = NULL;
	CONST CHAR16 *Class = NULL;
	CONST CHAR16 *SubClass = NULL;
	CONST CHAR16 *ProgIf = NULL;

	GetPciIdsNames (
		&PciIds[0],
		PciIoDevice->Pci.Hdr.VendorId,
		PciIoDevice->Pci.Hdr.DeviceId,
		PciIoDevice->PciType1 ? 0 : PciIoDevice->Pci.Device.SubsystemVendorID << 16 | PciIoDevice->Pci.Device.SubsystemID,
		&Vendor, &Device, &SubSystem
	);

	UnicodeSPrint (
		devices, MaxStrSize, L"%s%s%s%s%s%s",
		Vendor ? Vendor : L"",
		Vendor && Device ? L" " : L"",
		Device ? Device : L" Device",
		SubSystem ? L" (" : L"",
		SubSystem ? SubSystem : L"",
		SubSystem ? L")" : L""
	);

	GetPciIdsNames (
		&PciIds[1],
		PciIoDevice->Pci.Hdr.ClassCode[2],
		PciIoDevice->Pci.Hdr.ClassCode[1],
		PciIoDevice->Pci.Hdr.ClassCode[0],
		&Class, &SubClass, &ProgIf
	);

	UnicodeSPrint (
		classes, MaxStrSize, L"%s%s%s%s%s",
		SubClass ? L"" : Class,
		SubClass ? SubClass : L"",
		(Class || SubClass) && ProgIf ? L" (" : L"",
		ProgIf ? ProgIf : L"",
		ProgIf ? L")" : L""
	);
#endif // DO_PCIIDS

	Print (
		L"%*s%s%a%a%*a # %-13a %a"
#if DO_PCIIDS
		L" %-35s : %s"
#endif // DO_PCIIDS
		,
		kIndentTop, L"",
		IndentString,
		location,
		buses,
		IndentMaxLength - StrLen (IndentString) - AsciiStrLen (buses), "",
		links ? links : "",
		info
#if DO_PCIIDS
		, classes ? classes : L""
		, devices ? devices : L""
#endif // DO_PCIIDS
	);

	if (location) FreePool (location);
	if (buses) FreePool (buses);
	if (links) FreePool (links);
	if (info) FreePool (info);
	if (classes) FreePool (classes);
	if (devices) FreePool (devices);
} // OutputPciInfo


//========================================================================================
// Get/sort/treeify Pci devices

EFI_STATUS
FillInPciIoDevice(
	IN PCI_IO_DEVICE *PciIoDevice
)
{
	EFI_STATUS Status;

	DEBUG ((DEBUG_INFO, "[ FillInPciIoDevice\n"));
	PciAccess (Status, Read, PciIoDevice, 32, 0, sizeof (PciIoDevice->Pci) / sizeof (UINT32), &PciIoDevice->Pci);
	if (EFI_ERROR(Status)) {
		goto Done;
	}

	if (IS_PCI_BRIDGE(&PciIoDevice->Pci) || IS_CARDBUS_BRIDGE(&PciIoDevice->Pci)) {
		PciIoDevice->PciType1 = (PCI_TYPE01*)&PciIoDevice->Pci;
	}
	SaveBus (PciIoDevice);

	PciIoDevice->PciExpressCapabilityOffset = 0;
	Status = LocateCapabilityRegBlock (
		PciIoDevice,
		EFI_PCI_CAPABILITY_ID_PCIEXP,
		&PciIoDevice->PciExpressCapabilityOffset,
		NULL
	);
	if (!EFI_ERROR(Status)) {
		DEBUG ((DEBUG_INFO, "Read PciExpress Capability offset:%x size:%d shorts:%d\n", PciIoDevice->PciExpressCapabilityOffset, sizeof (PciIoDevice->PciExpressCap), sizeof (PciIoDevice->PciExpressCap) / sizeof (UINT16), &PciIoDevice->PciExpressCap));
		PciIoDevice->IsPciExp = TRUE;
		PciAccess (Status, Read, PciIoDevice, 16, PciIoDevice->PciExpressCapabilityOffset,
			sizeof (PciIoDevice->PciExpressCap) / sizeof (UINT16), &PciIoDevice->PciExpressCap
		);
		if (EFI_ERROR(Status)) {
			DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: Read PCIe Cap Status %r.\n", Status));
			PciIoDevice->IsPciExp = FALSE;
			Status = EFI_SUCCESS;
		}
	}
	else if (Status != EFI_NOT_FOUND && Status != EFI_UNSUPPORTED) {
		DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: LocateCapabilityRegBlock Status %r.\n", Status));
	}
	else {
		Status = EFI_SUCCESS;
	}

Done:
	DEBUG ((DEBUG_INFO, "] FillInPciIoDevice\n"));
	return Status;
}


STATIC
VOID
InitRootBridge (
	IN PCI_IO_DEVICE *PciIoDevice,
	EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo,
	UINTN PciDeviceID
)
{
	SetMem(PciIoDevice, sizeof(*PciIoDevice), 0);
	PciIoDevice->PciRootBridgeIo = PciRootBridgeIo;
	if (PciRootBridgeIo) {
		PciIoDevice->SegmentNumber = PciRootBridgeIo->SegmentNumber;
		PciIoDevice->IsSegment = TRUE;
	}
	PciIoDevice->PciType1 = (PCI_TYPE01*)&PciIoDevice->Pci;
	PciIoDevice->PciType1->Hdr.HeaderType = HEADER_TYPE_PCI_TO_PCI_BRIDGE;
	PciIoDevice->PciType1->Bridge.SubordinateBus = PCI_MAX_BUS;
	PciIoDevice->PciDeviceID = PciDeviceID;
}


EFI_STATUS
PciRootIoToPciIoDevice(
	EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo,
	IN OUT PCI_IO_DEVICE **PciIoDeviceOut,
	UINTN PciDeviceID
)
{
	EFI_STATUS Status = EFI_SUCCESS;
	DEBUG ((DEBUG_INFO, "[ PciRootIoToPciIoDevice\n"));

	if (!PciIoDeviceOut) {
		Status = EFI_INVALID_PARAMETER;
		goto Done;
	}
	
	PCI_IO_DEVICE *PciIoDevice = AllocatePool (sizeof (*PciIoDevice));
	if (!PciIoDevice) {
		Status = EFI_OUT_OF_RESOURCES;
		goto Done;
	}
	InitRootBridge (PciIoDevice, PciRootBridgeIo, PciDeviceID);
	
	DEBUG_CODE (
		OutputPciInfo (PciIoDevice, L"", 13, FALSE);
		Print (L"\n");
	);

Done:
	if (EFI_ERROR(Status)) {
		if (PciIoDevice) {
			FreePool (PciIoDevice);
			PciIoDevice = NULL;
		}
	}
	if (PciIoDeviceOut) *PciIoDeviceOut = PciIoDevice;

	DEBUG ((DEBUG_INFO, "] PciRootIoToPciIoDevice %r\n", Status));
	return Status;
}

EFI_STATUS
PciIoToPciIoDevice(
	IN EFI_PCI_IO_PROTOCOL *PciIo,
	IN OUT PCI_IO_DEVICE **PciIoDeviceOut,
	UINTN PciDeviceID
)
{
	EFI_STATUS Status = EFI_SUCCESS;
	DEBUG ((DEBUG_INFO, "[ PciIoToPciIoDevice\n"));

	if (!PciIo || !PciIoDeviceOut) {
		Status = EFI_INVALID_PARAMETER;
		goto Done;
	}
	
	PCI_IO_DEVICE *PciIoDevice = AllocateZeroPool (sizeof (*PciIoDevice));
	if (!PciIoDevice) {
		Status = EFI_OUT_OF_RESOURCES;
		goto Done;
	}
	
	PciIoDevice->PciDeviceID = PciDeviceID;
	PciIoDevice->PciIo = PciIo;
	
	UINTN SegmentNumber;
	UINTN BusNumber;
	UINTN DeviceNumber;
	UINTN FunctionNumber;
	Status = PciIoDevice->PciIo->GetLocation (
		PciIoDevice->PciIo,
		&SegmentNumber,
		&BusNumber,
		&DeviceNumber,
		&FunctionNumber
	);
	if (EFI_ERROR(Status)) {
		goto Done;
	}
	PciIoDevice->SegmentNumber = SegmentNumber;
	PciIoDevice->BusNumber = BusNumber;
	PciIoDevice->DeviceNumber = DeviceNumber;
	PciIoDevice->FunctionNumber = FunctionNumber;

	Status = FillInPciIoDevice (PciIoDevice);

	DEBUG_CODE (
		OutputPciInfo (PciIoDevice, L"", 13, FALSE);
		Print (L"\n");
	);

Done:
	if (EFI_ERROR(Status)) {
		if (PciIoDevice) {
			FreePool (PciIoDevice);
			PciIoDevice = NULL;
		}
	}
	if (PciIoDeviceOut) *PciIoDeviceOut = PciIoDevice;

	DEBUG ((DEBUG_INFO, "] PciIoToPciIoDevice %r\n", Status));
	return Status;
}


STATIC
INTN
ComparePciDevice(CONST VOID *A, CONST VOID *B)
{
	CONST PCI_IO_DEVICE *a = *(CONST PCI_IO_DEVICE **)A;
	CONST PCI_IO_DEVICE *b = *(CONST PCI_IO_DEVICE **)B;

	INTN result =
		  (a->SegmentNumber < b->SegmentNumber) ? -1
		: (a->SegmentNumber > b->SegmentNumber) ? 1
		: (a->BusNumber < b->BusNumber) ? -1
		: (a->BusNumber > b->BusNumber) ? 1
		: (a->DeviceNumber < b->DeviceNumber) ? -1
		: (a->DeviceNumber > b->DeviceNumber) ? 1
		: (a->FunctionNumber < b->FunctionNumber) ? -1
		: (a->FunctionNumber > b->FunctionNumber) ? 1
		: (a->PciDeviceID < b->PciDeviceID) ? -1
		: (a->PciDeviceID > b->PciDeviceID) ? 1
		: 0;

	return result;
}


#if DO_SEARCHFORHIDDENDEVICES
STATIC
UINTN
SearchForHiddenDevices (
	PCI_IO_DEVICE *HostBridge
)
{
	DEBUG ((DEBUG_INFO, "[ Search for hidden devices\n"));

	EFI_STATUS Status;

	PCI_IO_DEVICE *b;
	PCI_IO_DEVICE *PciIoDevice;
	UINTN PciDeviceCount = 0;
	
	/* Search for hidden devices */
	for (b = HostBridge->Chain; b; b = b->Chain) {
		if (!b->FirstChild) {
			DEBUG_CODE (
				OutputPciInfo (b, L"", 13, FALSE);
				Print (L"\n");
			);
			
			BOOLEAN MovedBuses;
			Status = VerifyBuses (b, &MovedBuses);
			if (EFI_ERROR(Status)) {
				continue;
			}
			
			UINTN DeviceNumber;
			for (DeviceNumber = 0; DeviceNumber <= PCI_MAX_DEVICE; DeviceNumber++) {
				UINTN FunctionNumber;
				for (FunctionNumber = 0; FunctionNumber <= PCI_MAX_FUNC; FunctionNumber++) {
					PciIoDevice = AllocateZeroPool (sizeof (*PciIoDevice));
					if (!PciIoDevice) {
						DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: AllocateZeroPool (hidden device) Result NULL.\n"));
						continue;
					}
					
					PciIoDevice->PciRootBridgeIo = b->PciRootBridgeIo;
					PciIoDevice->SegmentNumber = b->SegmentNumber;
					PciIoDevice->BusNumber = b->PciType1->Bridge.SecondaryBus;
					PciIoDevice->DeviceNumber = DeviceNumber;
					PciIoDevice->FunctionNumber = FunctionNumber;
					PciIoDevice->PciDeviceID = HostBridge->NextPciDeviceID;
			
					PciIoDevice->Pci.Hdr.Command = PciRead16 (Status, PciIoDevice, PCI_COMMAND_OFFSET);
					if (PciIoDevice->Pci.Hdr.Command == MAX_UINT16) {
						FreePool (PciIoDevice);
						continue;
					}
			
					DEBUG((DEBUG_INFO, "    Found hidden device %02x:%02x.%x\n", PciIoDevice->BusNumber, PciIoDevice->DeviceNumber, PciIoDevice->FunctionNumber));

					Status = FillInPciIoDevice (PciIoDevice);
					if (EFI_ERROR(Status)) {
						DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: FillInPciIoDevice Status %r.\n", Status));
						FreePool (PciIoDevice);
						continue;
					}

					if (PciIoDevice->PciType1) {
						*HostBridge->LastBridge = PciIoDevice;
						HostBridge->LastBridge = &PciIoDevice->Chain;
					}
					PciIoDevice->Parent = b;
					if (!b->FirstChild) {
						b->FirstChild = PciIoDevice;
					}
					else {
						b->LastChild->NextSibling = PciIoDevice;
					}
					b->LastChild = PciIoDevice;
					
					HostBridge->NextPciDeviceID++;
					PciDeviceCount++;

					if ((FunctionNumber == 0) && !IS_PCI_MULTI_FUNC(&(b->Pci))) {
						break;
					}
				} // for FunctionNumber
			} // for DeviceNumber
			
			if (MovedBuses) {
				UnmoveBuses (b);
			}
		} // if no children
	} // for
	*HostBridge->LastBridge = NULL;

	DEBUG ((DEBUG_INFO, "] Search for hidden devices found:%d\n", PciDeviceCount));
	return PciDeviceCount;
} // SearchForHiddenDevices
#endif


EFI_STATUS
GetPciDevices (
	PCI_IO_DEVICE *HostBridge
)
{
	DEBUG ((DEBUG_INFO, "[ GetPciDevices\n"));

	EFI_STATUS Status;

	UINTN PciRootIoCount;
	EFI_HANDLE *PciRootIoArray = NULL;
	UINTN PciIoCount;
	EFI_HANDLE *PciIoArray = NULL;
	UINTN PciDeviceCount = 0;
	PCI_IO_DEVICE **PciDeviceArray = NULL;

	if (!HostBridge) {
		Status = EFI_INVALID_PARAMETER;
		goto Done;
	}
	InitRootBridge (HostBridge, NULL, 0);
	HostBridge->LastBridge = &HostBridge->Chain;
	HostBridge->NextPciDeviceID = 1;

	Status = gBS->LocateHandleBuffer (
		ByProtocol,
		&gEfiPciRootBridgeIoProtocolGuid,
		NULL,
		&PciRootIoCount,
		&PciRootIoArray
	);
	if (EFI_ERROR(Status)) {
		goto Done;
	}
	
	Status = gBS->LocateHandleBuffer (
		ByProtocol,
		&gEfiPciIoProtocolGuid,
		NULL,
		&PciIoCount,
		&PciIoArray
	);
	if (EFI_ERROR(Status)) {
		goto Done;
	}

	PCI_IO_DEVICE *PciIoDevice;

	DEBUG ((DEBUG_INFO, "[ Create PciDeviceArray (%d)\n", PciRootIoCount + PciIoCount));
	PciDeviceArray = (PCI_IO_DEVICE **)AllocatePool (sizeof(PCI_IO_DEVICE *) * (PciRootIoCount + PciIoCount));
	if (!PciDeviceArray) {
		Status = EFI_OUT_OF_RESOURCES;
		goto Done;
	}
	

	DEBUG ((DEBUG_INFO, "[ Get Roots (%d)\n", PciRootIoCount));
	UINTN PciRootIoIndex;
	for (PciRootIoIndex = 0; PciRootIoIndex < PciRootIoCount; PciRootIoIndex++) {
		DEBUG ((DEBUG_INFO, "[ Root:%d id:%d #:%d\n", PciRootIoIndex, HostBridge->NextPciDeviceID, PciDeviceCount));
		EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *PciRootBridgeIo;
		Status = gBS->HandleProtocol(
			PciRootIoArray[PciRootIoIndex],
			&gEfiPciRootBridgeIoProtocolGuid,
			(void **)&PciRootBridgeIo
		);
		if (EFI_ERROR(Status)) {
			DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: HandleProtocol PciRootBridgeIo Status %r.\n", Status));
		}
		else {
			Status = PciRootIoToPciIoDevice(PciRootBridgeIo, &PciIoDevice, HostBridge->NextPciDeviceID);
			if (EFI_ERROR(Status)) {
				DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: PciRootIoToPciIoDevice Status %r.\n", Status));
			}
			else {		
				PciDeviceArray[PciDeviceCount++] = PciIoDevice;
				HostBridge->NextPciDeviceID++;
			}
		}
		DEBUG ((DEBUG_INFO, "] Root\n"));
	} // for root
	DEBUG ((DEBUG_INFO, "] Get Roots (%d)\n", PciDeviceCount));
	
	if (!PciDeviceCount) {
		goto Done;
	}


	DEBUG ((DEBUG_INFO, "[ Get Devices (%d)\n", PciIoCount));
	UINTN Index;
	for (Index = 0; Index < PciIoCount; Index++) {
		DEBUG ((DEBUG_INFO, "[ Device:%d id:%d #:%d\n", Index, HostBridge->NextPciDeviceID, PciDeviceCount));
		EFI_PCI_IO_PROTOCOL *PciIo;
		Status = gBS->HandleProtocol(
			PciIoArray[Index],
			&gEfiPciIoProtocolGuid,
			(void **)&PciIo
		);
		if (EFI_ERROR(Status)) {
			DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: HandleProtocol PciIo Status %r.\n", Status));
		}
		else {
			Status = PciIoToPciIoDevice(PciIo, &PciIoDevice, HostBridge->NextPciDeviceID);
			if (EFI_ERROR(Status)) {
				DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: PciIoToPciIoDevice Status %r.\n", Status));
			}
			else {
				PciDeviceArray[PciDeviceCount++] = PciIoDevice;
				HostBridge->NextPciDeviceID++;
			}
		}
		DEBUG ((DEBUG_INFO, "] Device\n"));
	} // for device
	DEBUG ((DEBUG_INFO, "] Get Devices (%d)\n", PciIoCount));

	DEBUG ((DEBUG_INFO, "] Create PciDeviceArray (%d)\n", PciDeviceCount));

	DEBUG ((DEBUG_INFO, "[ PerformQuickSort (%d)\n", PciDeviceCount));
	PerformQuickSort (PciDeviceArray, PciDeviceCount, sizeof(PCI_IO_DEVICE *), ComparePciDevice);
	DEBUG ((DEBUG_INFO, "] PerformQuickSort\n"));

	DEBUG ((DEBUG_INFO, "[ Build list of bridges\n"));
	/* Build list of bridges */
	for (Index = 0; Index < PciDeviceCount; Index++) {
		PciIoDevice = PciDeviceArray[Index];
		if (PciIoDevice->PciType1) {
			*HostBridge->LastBridge = PciIoDevice;
			HostBridge->LastBridge = &PciIoDevice->Chain;
		}
	}
	*HostBridge->LastBridge = NULL;
	DEBUG ((DEBUG_INFO, "] Build list of bridges\n"));


	PCI_IO_DEVICE *b;
	PCI_IO_DEVICE *c, *BestParent;


	DEBUG ((DEBUG_INFO, "[ Create a bridge tree\n"));
	/* Create a bridge tree by finding the best parent for every segment and device (but not HostBridge
	   which will be the parent of all the segments and anything that doesn't have a segment) */
	for (Index = 0; Index < PciDeviceCount; Index++) {
		b = PciDeviceArray[Index];
		BestParent = NULL;
		for (c = HostBridge; c; c = c->Chain) {
			if (
				c != b
				&& (c == HostBridge || b->SegmentNumber == c->SegmentNumber)
				&& b->BusNumber >= c->PciType1->Bridge.SecondaryBus && b->BusNumber <= c->PciType1->Bridge.SubordinateBus
				&& (
					!BestParent
/*
					|| BestParent->PciType1->Bridge.SubordinateBus - BestParent->PciType1->Bridge.SecondaryBus
					> c->PciType1->Bridge.SubordinateBus - c->PciType1->Bridge.SecondaryBus
*/
					|| (b->BusNumber - c->PciType1->Bridge.SecondaryBus
					< b->BusNumber - BestParent->PciType1->Bridge.SecondaryBus)
					|| (
						(b->BusNumber - c->PciType1->Bridge.SecondaryBus
						== b->BusNumber - BestParent->PciType1->Bridge.SecondaryBus)
						&& c->PciDeviceID > BestParent->PciDeviceID // HostBridge and first segment are identical (SecondaryBus are zero) so make sure first segment is best for its children 
					)
				)
			) {
				BestParent = c;
			}
		}
		if (BestParent) {
			b->Parent = BestParent;
			if (!BestParent->FirstChild) {
				BestParent->FirstChild = b;
			}
			if (BestParent->LastChild) {
				BestParent->LastChild->NextSibling = b;
			}
			BestParent->LastChild = b;
		}
	}
	DEBUG ((DEBUG_INFO, "] Create a bridge tree\n"));
	

	DEBUG ((DEBUG_INFO, "[ Get root for each device\n"));
	for (Index = 0; Index < PciDeviceCount; Index++) {
		b = PciDeviceArray[Index];
		for (c = b->Parent; c; c = c->Parent) {
			if (!b->PciRootBridgeIo && c->PciRootBridgeIo) {
				b->PciRootBridgeIo = c->PciRootBridgeIo;
			}
			if (c->IsSegment) {
				b->SegmentDevice = c;
				SetBus (c->UsedBuses, b->BusNumber);
				DEBUG ((DEBUG_INFO, "SetBus %02X = %08X %08X %08X %08X %08X %08X %08X %08X\n", b->BusNumber, c->UsedBuses[0], c->UsedBuses[1], c->UsedBuses[2], c->UsedBuses[3], c->UsedBuses[4], c->UsedBuses[5], c->UsedBuses[6], c->UsedBuses[7]));
				if (b->PciType1) {
					for (UINT8 Bus = b->PciType1->Bridge.SecondaryBus; Bus <= b->PciType1->Bridge.SubordinateBus; Bus++) {
						SetBus (c->UsedBuses, Bus);
						DEBUG ((DEBUG_INFO, "SetBus %02X = %08X %08X %08X %08X %08X %08X %08X %08X\n", Bus, c->UsedBuses[0], c->UsedBuses[1], c->UsedBuses[2], c->UsedBuses[3], c->UsedBuses[4], c->UsedBuses[5], c->UsedBuses[6], c->UsedBuses[7]));
					}
				}
			}
		}
	}
	DEBUG ((DEBUG_INFO, "] Get root for each device\n"));
	
	
	DEBUG ((DEBUG_INFO, "[ Find max unused range\n"));
	for (b = HostBridge->FirstChild; b; b = b->NextSibling) {
  		UINTN MaxUnusedLength = 0;
  		UINTN MaxUnusedStart = 0;

  		UINTN UnusedLength = 0;
		UINTN UnusedStart = 0;
		UINTN Bus = 0;
		while (Bus < 256) {
			while (Bus < 256 && IsBus(b->UsedBuses, Bus)) Bus++;
			UnusedStart = Bus;
			while (Bus < 256 && !IsBus(b->UsedBuses, Bus)) Bus++;
			UnusedLength = Bus - UnusedStart;
			
			if (UnusedLength > MaxUnusedLength) {
				MaxUnusedLength = UnusedLength;
				MaxUnusedStart = UnusedStart;
			}
		}
		
		b->BigUnusedStart = MaxUnusedStart < PCI_MAX_BUS ? MaxUnusedStart : PCI_MAX_BUS;
		b->BigUnusedEnd = (MaxUnusedStart + MaxUnusedLength - 1) < PCI_MAX_BUS ? (MaxUnusedStart + MaxUnusedLength - 1) : PCI_MAX_BUS;
	}
	DEBUG ((DEBUG_INFO, "] Find max unused range\n"));


	DEBUG_CODE (
		INTN DeviceIndex;
		for (DeviceIndex = -1; DeviceIndex < (INTN)PciDeviceCount; DeviceIndex++) {
			b = DeviceIndex < 0 ? HostBridge : PciDeviceArray[DeviceIndex];
			Print (L"%02d: seg:%02d par:%02d chld:%02d-%02d sib:%02d chain:%02d io:%d rio:%d unused:%02x-%02x  ",
				b->PciDeviceID,
				b->SegmentDevice ? b->SegmentDevice->PciDeviceID : 0,
				b->Parent ? b->Parent->PciDeviceID : 0,
				b->FirstChild ? b->FirstChild->PciDeviceID : 0,
				b->LastChild ? b->LastChild->PciDeviceID : 0,
				b->NextSibling ? b->NextSibling->PciDeviceID : 0,
				b->Chain ? b->Chain->PciDeviceID : 0,
				!!b->PciIo,
				!!b->PciRootBridgeIo,
				b->BigUnusedStart,
				b->BigUnusedEnd
			);
			OutputPciInfo (b, L"", 13, FALSE);
			Print (L"\n");
		}
	);

	
	UINTN FoundDevices = 0;
	do {
#if DO_THUNDERBOLT
		InitAllThunderbolt (HostBridge);
#endif
#if DO_SEARCHFORHIDDENDEVICES
		FoundDevices = SearchForHiddenDevices (HostBridge);
#endif
	} while (FoundDevices);
	

Done:
	if (PciDeviceArray) FreePool (PciDeviceArray);
	if (PciIoArray) FreePool (PciIoArray);
	if (PciRootIoArray) FreePool (PciRootIoArray);
	
	DEBUG ((DEBUG_INFO, "] GetPciDevices %r\n", Status));
	return Status;
} // GetPciDevices


//========================================================================================

/*
	Depth first iteration of pci tree starting with first segment.
*/
VOID
PciTreeIterate (
	PCI_IO_DEVICE *HostBridge,
	PciTreeOneItem callback,
	VOID *context,
	BOOLEAN DoPrint
)
{
	if (!HostBridge) {
		return;
	}

	PCI_IO_DEVICE *PciIoDevice;
	UINTN IndentLength;
	UINTN IndentMaxLength = 0;

	if (DoPrint) {
		PCI_IO_DEVICE *b; // bridge
		PCI_IO_DEVICE *c; // ancestors
		for (b = HostBridge->Chain; b; b = b->Chain) {
			UINTN Depth = 0;
			for (c = b->Parent; c; c = c->Parent) {
				Depth++;
			}
			IndentLength = (kIndentAncestor + kIndentSelf) * (Depth - 1) + STR_LEN(kPciTreeBridge) - kIndentSelf
				+ ((b->PciType1->Bridge.SecondaryBus == b->PciType1->Bridge.SubordinateBus) ? 5 : 8);
			if (IndentLength > IndentMaxLength) IndentMaxLength = IndentLength;

			if (b->FirstChild) {
				IndentLength = (kIndentAncestor + kIndentSelf) * Depth + STR_LEN(kPciTreeEndPoint) - kIndentSelf;
				if (IndentLength > IndentMaxLength) IndentMaxLength = IndentLength;
			}
		}
	}
	
	#define IndentStringMaxLen 200
	CHAR16 IndentString[IndentStringMaxLen];

	PciIoDevice = HostBridge->FirstChild;
	while (PciIoDevice) {

		IndentString[0] = L'\0';
		IndentLength = 0;

		if (DoPrint) {
			Print (L"%s[%04x]\n", kPciTreeTop, PciIoDevice->SegmentNumber);
		}
		
		callback (PciIoDevice, context);

		PciIoDevice = PciIoDevice->FirstChild;
		while (PciIoDevice) {

			if (DoPrint) {
				UnicodeSPrint (
					&IndentString[IndentLength], (IndentStringMaxLen - IndentLength) * sizeof(CHAR16), L"%s%s",
					PciIoDevice->NextSibling ? kPciTreeSibling : kPciTreeNoSibling,
					PciIoDevice->PciType1 ? kPciTreeBridge : kPciTreeEndPoint
				);
				OutputPciInfo (PciIoDevice, IndentString, IndentMaxLength, FALSE);
				Print (L"\n");
			}
			
			callback (PciIoDevice, context);

			if (PciIoDevice->FirstChild) {
				if (DoPrint) {
					UnicodeSPrint (
						&IndentString[IndentLength], (IndentStringMaxLen - IndentLength) * sizeof(CHAR16), L"%s%*a",
						PciIoDevice->NextSibling ? kPciTreeUncle : kPciTreeNoUncle,
						(kIndentAncestor + kIndentSelf), ""
					);
					IndentLength += (kIndentAncestor + kIndentSelf);
				}
				PciIoDevice = PciIoDevice->FirstChild;
			}
			else if (PciIoDevice->NextSibling) {
				PciIoDevice = PciIoDevice->NextSibling;
			} else {
				while ((PciIoDevice = PciIoDevice->Parent)) {
					if (PciIoDevice->IsSegment) {
						break;
					}
					if (DoPrint) {
						IndentLength -= (kIndentAncestor + kIndentSelf);
					}
					if (PciIoDevice->NextSibling) {
						PciIoDevice = PciIoDevice->NextSibling;
						break;
					}
				}
			}
			if (PciIoDevice->Parent == HostBridge) {
				break;
			}
		} // while while devices
		PciIoDevice = PciIoDevice->NextSibling;
	} // while segments
} // PciTreeIterate


//========================================================================================
