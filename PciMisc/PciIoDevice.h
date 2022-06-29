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

#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <IndustryStandard/Pci.h>


//========================================================================================
// Macro for Pci reading/writing


#define PciAccess(Status, action, PciIoDevice, bits, reg, count, buffer) \
	({ \
		Status = EFI_INVALID_PARAMETER; \
		if (PciIoDevice->PciIo) { \
			Status = PciIoDevice->PciIo->Pci.action ( \
				PciIoDevice->PciIo, \
				EfiPciIoWidthUint ## bits, \
				reg, \
				count, \
				buffer \
			); \
		} \
		else if (PciIoDevice->PciRootBridgeIo) { \
			Status = PciIoDevice->PciRootBridgeIo->Pci.action ( \
				PciIoDevice->PciRootBridgeIo, \
				EfiPciWidthUint ## bits, \
				EFI_PCI_ADDRESS (PciIoDevice->BusNumber, PciIoDevice->DeviceNumber, PciIoDevice->FunctionNumber, reg), \
				count, \
				buffer \
			); \
		} \
	})

#define PciRead8(  Status, b, reg     ) ({UINT8  buffer =  -1; PciAccess(Status, Read , b,  8, reg, 1, &buffer); buffer; })
#define PciRead16( Status, b, reg     ) ({UINT16 buffer =  -1; PciAccess(Status, Read , b, 16, reg, 1, &buffer); buffer; })
#define PciRead32( Status, b, reg     ) ({UINT32 buffer =  -1; PciAccess(Status, Read , b, 32, reg, 1, &buffer); buffer; })
#define PciWrite8( Status, b, reg, val) ({UINT8  buffer = val; PciAccess(Status, Write, b,  8, reg, 1, &buffer);         })
#define PciWrite16(Status, b, reg, val) ({UINT16 buffer = val; PciAccess(Status, Write, b, 16, reg, 1, &buffer);         })
#define PciWrite32(Status, b, reg, val) ({UINT32 buffer = val; PciAccess(Status, Write, b, 32, reg, 1, &buffer);         })

#define    IsBus(buses, x) (buses[(x) / 32] &   ((UINT32)1 << ((x) % 32)))
#define   SetBus(buses, x)  buses[(x) / 32] |=  ((UINT32)1 << ((x) % 32))
#define ClearBus(buses, x)  buses[(x) / 32] &= ~((UINT32)1 << ((x) % 32))


//========================================================================================
// from PciBus.h


typedef struct _PCI_IO_DEVICE              PCI_IO_DEVICE;

struct _PCI_IO_DEVICE {
  EFI_PCI_IO_PROTOCOL                       *PciIo; // joevt - must be a pointer because it's a handle

  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL           *PciRootBridgeIo;

  //
  // PCI configuration space header type
  //
  PCI_TYPE00                                Pci;
  PCI_TYPE01                                *PciType1; // added for convenience - points to Pci if it's a bridge

  //
  // Segment, Bus, Device, Function numbers
  //
  UINTN										PciDeviceID; // each device gets a unique ID. 0 = parent of list of segments; 1 = 
  UINTN                                     SegmentNumber; // this would normally come from PciRootBridgeIo->SegmentNumber
  UINT8                                     BusNumber;
  UINT8                                     DeviceNumber;
  UINT8                                     FunctionNumber;

  //
  // Segment/bridge list
  //
  PCI_IO_DEVICE                             *Chain; // chain of bridges // valid for bridges
  PCI_IO_DEVICE                             **LastBridge; // valid for HostBridge
  UINTN                                     NextPciDeviceID; // valid for HostBridge
  PCI_IO_DEVICE								*SegmentDevice; // valid for descendants of Segments
  BOOLEAN									IsSegment; // true for child of HostBridge
  BOOLEAN                                   DidThunderbolt; // valid for bridges

  //
  // Tree structure
  //
  PCI_IO_DEVICE                             *Parent; // The bridge device this pci device is subject to
  PCI_IO_DEVICE                             *FirstChild;
  PCI_IO_DEVICE                             *LastChild;
  PCI_IO_DEVICE                             *NextSibling;

  //
  // Used and unused Bus numbers
  //
  UINT32									UsedBuses[8]; // valid for Segments
  UINT8										BigUnusedStart; // valid for Segments
  UINT8										BigUnusedEnd; // valid for Segments

  //
  // Bus number renumbering
  // 
  EFI_PCI_IO_PROTOCOL                       *SavePciIo;

  UINT8                                     SaveBusNumber;
  UINT8                                     SavePrimaryBus;
  UINT8                                     SaveSecondaryBus;
  UINT8                                     SaveSubordinateBus;

  UINT8                                     NewBusNumber;
  UINT8                                     NewPrimaryBus;
  UINT8                                     NewSecondaryBus;
  UINT8                                     NewSubordinateBus;

  //
  // Pci Express
  //
  BOOLEAN                                   IsPciExp;
  UINT8                                     PciExpressCapabilityOffset;
  PCI_CAPABILITY_PCIEXP                     PciExpressCap; // keep a copy of the express capabilities

  //
  // ToDo
  //
#if DO_THUNDERBOLT
  UINT8                                     TbtType;
  DTBT_COMMON_CONFIG                        DTbtCommonConfig;
#endif
};


//========================================================================================
// from PciCommand.h

/**
  Check the capability supporting by given device.

  @param PciIoDevice   Pointer to instance of PCI_IO_DEVICE.

  @retval TRUE         Capability supported.
  @retval FALSE        Capability not supported.

**/
BOOLEAN
PciCapabilitySupport (
  IN PCI_IO_DEVICE  *PciIoDevice
  );

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
  IN PCI_IO_DEVICE  *PciIoDevice,
  IN UINT8          CapId,
  IN OUT UINT8      *Offset,
  OUT UINT8         *NextRegBlock OPTIONAL
  );

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
  IN     UINT16        CapId,
  IN OUT UINT32        *Offset,
     OUT UINT32        *NextRegBlock OPTIONAL
  );


//========================================================================================
// Pci device stringification

CHAR8 *
GetLinksString (
	PCI_CAPABILITY_PCIEXP *PciExpressCap
);


VOID
OutputPciInfo (
	PCI_IO_DEVICE *PciIoDevice,
	CHAR16 *IndentString,
	UINTN IndentMaxLength,
	BOOLEAN UseNew
);


//========================================================================================
// Get/sort/treeify Pci devices


EFI_STATUS
GetPciDevices (
	PCI_IO_DEVICE *HostBridge
);


typedef VOID (* PciTreeOneItem)(PCI_IO_DEVICE *PciIoDevice, VOID *conntext);

/*
	Depth first iteration of pci tree starting with first segment.
*/
VOID
PciTreeIterate (
	PCI_IO_DEVICE *HostBridge,
	PciTreeOneItem callback,
	VOID *context,
	BOOLEAN DoPrint
);


//========================================================================================
