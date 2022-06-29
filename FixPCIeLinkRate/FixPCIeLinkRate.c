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
#include "FixPCIeLinkRate.h"

#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>


//========================================================================================
// Utilities

STATIC
VOID
SetMaxLinkSpeed (
	PCI_IO_DEVICE *PciIoDevice,
	PCI_IO_DEVICE *ChildPciIoDevice
)
{
	EFI_STATUS Status;
	UINTN TargetLinkSpeedValue = ChildPciIoDevice->PciExpressCap.LinkCapability.Bits.MaxLinkSpeed;
	if (TargetLinkSpeedValue > PciIoDevice->PciExpressCap.LinkCapability.Bits.MaxLinkSpeed) {
		TargetLinkSpeedValue = PciIoDevice->PciExpressCap.LinkCapability.Bits.MaxLinkSpeed;
	}

	#define Action(Status, d, w, r) \
		PciAccess (Status, w, d, 16, d->PciExpressCapabilityOffset + OFFSET_OF(PCI_CAPABILITY_PCIEXP, r), \
			1, &d->PciExpressCap.r \
		); \
		if (EFI_ERROR(Status)) { \
			DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: " #w " " #r " Status %r.\n", Status)); \
			break; \
		}
	#define Read(Status, d, r) Action (Status, d, Read, r)
	#define Write(Status, d, r) Action (Status, d, Write, r)
	#define Modify(Status, d, r, f, v) \
		Read (Status, d, r); \
		d->PciExpressCap.r.Bits.f = v; \
		Write (Status, d, r);
	
	do {
		Modify(Status, PciIoDevice, LinkControl2, TargetLinkSpeed, TargetLinkSpeedValue);
		do {
			Read(Status, PciIoDevice, LinkStatus);
		} while (PciIoDevice->PciExpressCap.LinkStatus.Bits.LinkTraining);
		do {
			Modify(Status, PciIoDevice, LinkControl, RetrainLink, 1);
			do {
				Read(Status, PciIoDevice, LinkStatus);
			} while (PciIoDevice->PciExpressCap.LinkStatus.Bits.LinkTraining);
			Read(Status, ChildPciIoDevice, LinkStatus);
		} while (0);
	} while (0);
	
	#undef Action
	#undef Read
	#undef Write
	#undef Modify
} // SetMaxLinkSpeed


VOID
FixPCIeLinkRate (
	PCI_IO_DEVICE *PciIoDevice,
	VOID *context
) {
	PCI_IO_DEVICE **ParentDevice = context;

	if (PciIoDevice->IsSegment) {
		*ParentDevice = NULL;
	}
	else {
		if (
			*ParentDevice && PciIoDevice->IsPciExp
			&& PciIoDevice->PciExpressCap.LinkStatus.Bits.CurrentLinkSpeed < PciIoDevice->PciExpressCap.LinkCapability.Bits.MaxLinkSpeed
		) {
			SetMaxLinkSpeed (*ParentDevice, PciIoDevice);

			CHAR8 *links;
			UINTN Index;
			for (Index = 0; Index < 2; Index++) {
				links = GetLinksString (&(Index ? PciIoDevice : *ParentDevice)->PciExpressCap);
				Print (
					L"# %-13a %a",
/*
					L"%*s%s%a%a%*a # %-13a %a",
					kIndentTop + StrLen (IndentString), L"",
					L"", // IndentString
					"       ", // location
					"", // buses
					IndentMaxLength - StrLen (IndentString), "",
*/
					links ? links : "",
					Index ? "(updated child)" : "(updated parent)"
				);
				Print (L"\n");
				FreePool (links);
			}
		}

		*ParentDevice = (
			   PciIoDevice->FirstChild
			&& PciIoDevice->IsPciExp
			&& PciIoDevice->PciExpressCap.LinkStatus.Bits.CurrentLinkSpeed < PciIoDevice->PciExpressCap.LinkCapability.Bits.MaxLinkSpeed
			&& (
				   PciIoDevice->PciExpressCap.Capability.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_ROOT_PORT
				|| PciIoDevice->PciExpressCap.Capability.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_DOWNSTREAM_PORT				
			)
		) ? PciIoDevice : NULL;
	}
} // FixPCIeLinkRate


//========================================================================================
// Main 

EFI_STATUS
EFIAPI
FixPCIeLinkRateEntryPoint (
	EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE *SystemTable
)
{
	DEBUG ((DEBUG_INFO, "[ FixPCIeLinkRateEntryPoint\n"));

	#if DO_PCIIDS
	//UseInternalPciIds();
	ReadPciIds(ImageHandle);
	#endif

	EFI_STATUS Status;
	PCI_IO_DEVICE HostBridge;
	Status = GetPciDevices (&HostBridge);

	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: LocateHandleBuffer Status %r.\n", Status));
	}
	else {
		PCI_IO_DEVICE *ParentDevice = NULL;
		PciTreeIterate (&HostBridge, FixPCIeLinkRate, &ParentDevice, TRUE);
	} // if LocateHandleBuffer

	DEBUG ((DEBUG_INFO, "] FixPCIeLinkRateEntryPoint\n"));
    return EFI_SUCCESS;
}
