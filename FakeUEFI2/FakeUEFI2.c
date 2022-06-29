//
//  Copyright (c) 2020  joevt   All rights reserved.
//
//  FakeUEFI2 
//
//  License: BSD 2 clause License
//
//  Portions Copyright (c) 2016-2017, Microsoft Corporation
//           Copyright (c) 2018, Intel Corporation. All rights reserved.
//           See relevant code in EDK11 for exact details
//

#include <Uefi.h>
#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/PrintLib.h>

#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextInEx.h>
#include <Protocol/Shell.h>
#include <Protocol/ComponentName.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/DriverDiagnostics2.h>
#include <Protocol/DriverConfiguration2.h>

#include <IndustryStandard/Bmp.h>

#include "FakeUEFI2.h"


#define UTILITY_VERSION L"20201107"

#define FakeUEFI2_VERSION 0x1

EFI_DRIVER_BINDING_PROTOCOL gFakeUEFI2Binding = {
    FakeUEFI2BindingSupported,
    FakeUEFI2BindingStart,
    FakeUEFI2BindingStop,
    FakeUEFI2_VERSION,
    NULL,
    NULL
};

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_COMPONENT_NAME2_PROTOCOL gFakeUEFI2ComponentName2 = {
    (EFI_COMPONENT_NAME2_GET_DRIVER_NAME) FakeUEFI2ComponentNameGetDriverName,
    (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) FakeUEFI2ComponentNameGetControllerName,
    "en"
};

GLOBAL_REMOVE_IF_UNREFERENCED 
EFI_UNICODE_STRING_TABLE mFakeUEFI2NameTable[] = {
    { "en", (CHAR16 *) L"FakeUEFI2" },
    { NULL , NULL }
};

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_DRIVER_DIAGNOSTICS2_PROTOCOL gFakeUEFI2Diagnostics2 = {
    FakeUEFI2RunDiagnostics,
    "en"
}; 

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_DRIVER_CONFIGURATION2_PROTOCOL gFakeUEFI2Configuration2 = {
    FakeUEFI2ConfigurationSetOptions,
    FakeUEFI2ConfigurationOptionsValid,
    FakeUEFI2ConfigurationForceDefaults,
    "en"
};


EFI_STATUS
FakeUEFI2ConfigurationSetOptions( EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                         EFI_HANDLE                               ControllerHandle,
                                         EFI_HANDLE                               ChildHandle,
                                         CHAR8                                    *Language,
                                         EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired )
{
	//DEBUG((DEBUG_INFO, "FakeUEFI2ConfigurationSetOptions Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
FakeUEFI2ConfigurationOptionsValid( EFI_DRIVER_CONFIGURATION2_PROTOCOL *This,
                                           EFI_HANDLE                         ControllerHandle,
                                           EFI_HANDLE                         ChildHandle )
{
	//DEBUG((DEBUG_INFO, "FakeUEFI2ConfigurationOptionsValid Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
FakeUEFI2ConfigurationForceDefaults( EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                            EFI_HANDLE                               ControllerHandle,
                                            EFI_HANDLE                               ChildHandle,
                                            UINT32                                   DefaultType,
                                            EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired )
{
	//DEBUG((DEBUG_INFO, "FakeUEFI2ConfigurationForceDefaults Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
FakeUEFI2RunDiagnostics( EFI_DRIVER_DIAGNOSTICS2_PROTOCOL *This,
                                EFI_HANDLE                       ControllerHandle,
                                EFI_HANDLE                       ChildHandle, 
                                EFI_DRIVER_DIAGNOSTIC_TYPE       DiagnosticType, 
                                CHAR8                            *Language,
                                EFI_GUID                         **ErrorType, 
                                UINTN                            *BufferSize,
                                CHAR16                           **Buffer )
{
	//DEBUG((DEBUG_INFO, "FakeUEFI2RunDiagnostics Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


//
//  Retrieve user readable name of the driver
//
EFI_STATUS
FakeUEFI2ComponentNameGetDriverName( EFI_COMPONENT_NAME2_PROTOCOL *This,
                                            CHAR8                        *Language,
                                            CHAR16                       **DriverName )
{
    return LookupUnicodeString2( Language,
                                 This->SupportedLanguages,
                                 mFakeUEFI2NameTable,
                                 DriverName,
                                 (BOOLEAN)(This == &gFakeUEFI2ComponentName2) );
}


//
//  Retrieve user readable name of controller being managed by a driver
//
EFI_STATUS
FakeUEFI2ComponentNameGetControllerName( EFI_COMPONENT_NAME2_PROTOCOL *This,
                                                EFI_HANDLE                   ControllerHandle,
                                                EFI_HANDLE                   ChildHandle,
                                                CHAR8                        *Language,
                                                CHAR16                       **ControllerName )
{
    return EFI_UNSUPPORTED;
}


//
//  Start this driver on Controller
//
EFI_STATUS
FakeUEFI2BindingStart( EFI_DRIVER_BINDING_PROTOCOL *This,
                              EFI_HANDLE                  Controller,
                              EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath )
{
    return EFI_UNSUPPORTED;
}


//
//  Stop this driver on ControllerHandle.
//
EFI_STATUS
FakeUEFI2BindingStop( EFI_DRIVER_BINDING_PROTOCOL *This,
                             EFI_HANDLE                  Controller,
                             UINTN                       NumberOfChildren,
                             EFI_HANDLE                  *ChildHandleBuffer )
{
    return EFI_UNSUPPORTED;
}


//
//  See if this driver supports ControllerHandle.
//
EFI_STATUS
FakeUEFI2BindingSupported( EFI_DRIVER_BINDING_PROTOCOL *This,
                                  EFI_HANDLE                  Controller,
                                  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath )
{
    return EFI_UNSUPPORTED;
}


#define EFI_SIGNATURE_32(a, b, c, d) SIGNATURE_32(a, b, c, d)

#define EFI_FIELD_OFFSET(TYPE,Field) ((UINTN)(&(((TYPE *) 0)->Field)))

#define POOL_HEAD_SIGNATURE   EFI_SIGNATURE_32('p','h','d','0')

typedef struct {
  UINT32          Signature;
  UINT32          Size;
  EFI_MEMORY_TYPE Type;
  UINTN           Reserved;
  CHAR8           Data[1];
} POOL_HEAD1;

typedef struct {
  UINT32          Signature;
  UINT32          Reserved;
  EFI_MEMORY_TYPE Type;
  UINTN           Size;
  CHAR8           Data[1];
} POOL_HEAD2;

#define SIZE_OF_POOL_HEAD1 EFI_FIELD_OFFSET(POOL_HEAD1,Data)
#define SIZE_OF_POOL_HEAD2 EFI_FIELD_OFFSET(POOL_HEAD2,Data)

#define POOL_TAIL_SIGNATURE   EFI_SIGNATURE_32('p','t','a','l')
typedef struct {
  UINT32      Signature;
  UINT32      Size;
} POOL_TAIL1;

typedef struct {
  UINT32      Signature;
  UINT32      Reserved;
  UINTN       Size;
} POOL_TAIL2;

#define HEAD_TO_TAIL1(a) ((POOL_TAIL1 *) (((CHAR8 *) (a)) + (a)->Size - sizeof(POOL_TAIL1)))
#define HEAD_TO_TAIL2(a) ((POOL_TAIL2 *) (((CHAR8 *) (a)) + (a)->Size - sizeof(POOL_TAIL2)))
#define DATA_TO_HEAD1(a) ((POOL_HEAD1 *) (((CHAR8 *) (a)) - SIZE_OF_POOL_HEAD1))
#define DATA_TO_HEAD2(a) ((POOL_HEAD2 *) (((CHAR8 *) (a)) - SIZE_OF_POOL_HEAD2))



/*
EFI_STATUS
(EFIAPI *EFI_CREATE_EVENT)(
  IN  UINT32                       Type,
  IN  EFI_TPL                      NotifyTpl,
  IN  EFI_EVENT_NOTIFY             NotifyFunction,
  IN  VOID                         *NotifyContext,
  OUT EFI_EVENT                    *Event
  );
*/


#define EFI_EVENT_SIGNAL_READY_TO_BOOT 0x00000203
#define EFI_EVENT_NOTIFY_SIGNAL_ALL    0x00000400

EFI_STATUS
EFIAPI
MyCreateEventEx(
           UINT32 Type,
           EFI_TPL NotifyTpl,
           EFI_EVENT_NOTIFY NotifyFunction,
     const void *NotifyContext,
     const EFI_GUID *EventGroup,
           EFI_EVENT *Event
  )
{
	EFI_STATUS Success = EFI_SUCCESS;
	if (Type == EVT_NOTIFY_SIGNAL && CompareGuid(EventGroup, &gEfiEventExitBootServicesGuid)) {
		DEBUG((DEBUG_INFO, "FakeUEFI2: CreateEventEx gEfiEventExitBootServicesGuid NotifyTpl:%02X\n", NotifyTpl));
		Success = gBS->CreateEvent (
			EVT_SIGNAL_EXIT_BOOT_SERVICES,
			NotifyTpl,
			NotifyFunction,
			(VOID*)NotifyContext,
			Event
		);
	}
	else if (Type == EVT_NOTIFY_SIGNAL && CompareGuid(EventGroup, &gEfiEventVirtualAddressChangeGuid)) {
		DEBUG((DEBUG_INFO, "FakeUEFI2: CreateEventEx gEfiEventVirtualAddressChangeGuid NotifyTpl:%02X\n", NotifyTpl));
		Success = gBS->CreateEvent (
			EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE,
			NotifyTpl,
			NotifyFunction,
			(VOID*)NotifyContext,
			Event
		);
	}
	else if (Type == EVT_NOTIFY_SIGNAL && CompareGuid(EventGroup, &gEfiEventReadyToBootGuid)) {
		DEBUG((DEBUG_INFO, "FakeUEFI2: CreateEventEx gEfiEventReadyToBootGuid NotifyTpl:%02X\n", NotifyTpl));
		Success = gBS->CreateEvent (
			EFI_EVENT_SIGNAL_READY_TO_BOOT | ((NotifyFunction == NULL) ? EFI_EVENT_NOTIFY_SIGNAL_ALL : 0),
			NotifyTpl,
			NotifyFunction,
			(VOID*)NotifyContext,
			Event
		);
	}
	else {
		DEBUG((DEBUG_INFO, "FakeUEFI2: CreateEventEx type:%08X guid:%g NotifyTpl:%02X\n", Type, EventGroup, NotifyTpl));
		Success = gBS->CreateEvent (
			Type,
			NotifyTpl,
			NotifyFunction,
			(VOID*)NotifyContext,
			Event
		);
	}
	DEBUG((DEBUG_INFO, "FakeUEFI2: CreateEventEx result:%r\n", Success));
	return Success;
}


EFI_STATUS
EFIAPI
MySetMemorySpaceCapabilities (
     EFI_PHYSICAL_ADDRESS BaseAddress,
     UINT64 Length,
     UINT64 Capabilities
  ) {
	EFI_STATUS Success = EFI_UNSUPPORTED;
//	DEBUG((DEBUG_INFO, "FakeUEFI2: MySetMemorySpaceCapabilities result:%r\n", Success));
	return Success;
}

EFI_STATUS
EFIAPI
MyQueryCapsuleCapabilities (
      EFI_CAPSULE_HEADER **CapsuleHeaderArray,
      UINTN CapsuleCount,
      UINT64 *MaximumCapsuleSize,
      EFI_RESET_TYPE *ResetType
  ) {
	EFI_STATUS Success = EFI_UNSUPPORTED;
//	DEBUG((DEBUG_INFO, "FakeUEFI2: MyQueryCapsuleCapabilities result:%r\n", Success));
	return Success;
}

EFI_STATUS
EFIAPI
MyQueryVariableInfo (
      UINT32 Attributes,
      UINT64 *MaximumVariableStorageSize,
      UINT64 *RemainingVariableStorageSize,
      UINT64 *MaximumVariableSize
  ) {
	EFI_STATUS Success = EFI_UNSUPPORTED;
//	DEBUG((DEBUG_INFO, "FakeUEFI2: MyQueryVariableInfo result:%r\n", Success));
	return Success;
}


#define MsgLog(...) DEBUG((DEBUG_INFO, __VA_ARGS__))




INTN PoolType(VOID *Pointer) {
	POOL_HEAD1 *head1 = DATA_TO_HEAD1(Pointer);
	POOL_HEAD2 *head2 = DATA_TO_HEAD2(Pointer);
	// head1 = head2

	if (head1->Signature == POOL_HEAD_SIGNATURE) {
		// head is valid
		POOL_TAIL1 *tail1 = HEAD_TO_TAIL1(head1);
		POOL_TAIL2 *tail2 = HEAD_TO_TAIL2(head2);

		if ( (VOID*)tail1 >= (VOID*)&head1->Data ) {
			// tail1 is valid
			if ( (VOID*)tail2 >= (VOID*)&head2->Data ) {
				// tail2 is valid
				if ((VOID*)tail1 == (VOID*)tail2) {
					// the tails are the same location
					if (tail1->Signature == POOL_TAIL_SIGNATURE) {
						if (tail1->Size == head1->Size) return 1;
						if (tail2->Size == head2->Size) return 2;
						return -1;
					}
					return -2;
				}
				if ((VOID*)tail1 < (VOID*)tail2) {
					// tail1 is closer
					if (tail1->Signature == POOL_TAIL_SIGNATURE && tail1->Size == head1->Size) return 1;
					if (tail2->Signature == POOL_TAIL_SIGNATURE && tail2->Size == head2->Size) return 2;
					if (tail1->Signature == POOL_TAIL_SIGNATURE) return -3;
					if (tail2->Signature == POOL_TAIL_SIGNATURE) return -4;
					return -5;
				}
				// tail2 is closer
				if (tail2->Signature == POOL_TAIL_SIGNATURE && tail2->Size == head2->Size) return 2;
				if (tail1->Signature == POOL_TAIL_SIGNATURE && tail1->Size == head1->Size) return 1;
				if (tail2->Signature == POOL_TAIL_SIGNATURE) return -6;
				if (tail1->Signature == POOL_TAIL_SIGNATURE) return -7;
				return -8;
			}
			// only tail1 is valid
			if (tail1->Signature == POOL_TAIL_SIGNATURE && tail1->Size == head1->Size) return 1;
			if (tail1->Signature == POOL_TAIL_SIGNATURE) return -9;
			return -10;
		}
		if ( (VOID*)tail2 >= (VOID*)&head2->Data ) {
			// only tail2 is valid
			if (tail2->Signature == POOL_TAIL_SIGNATURE && tail2->Size == head2->Size) return 2;
			if (tail2->Signature == POOL_TAIL_SIGNATURE) return -11;
			return -12;
		}
		// neither tail is valid
		return -13;
	}
	// head is not valid
	return -14;
}


INTN LogPoolProc (IN VOID *Pointer, IN VOID **AtPointer, IN CHAR8 *StrPointer, IN CHAR8 *f, UINTN l, BOOLEAN always) {
	INTN result;
	static INTN curLogProc = 0;
	static INTN maxLogProc = 0;
	static INTN maxReported = 0;

	do {
		if (!Pointer) {
			result = -100; break;
		}
		curLogProc++;
		if (curLogProc > maxLogProc) maxLogProc = curLogProc;
		if (curLogProc > 1) {
			result = 1; break;
		}

		if (maxLogProc > maxReported) {
			maxReported = maxLogProc;
			MsgLog("LogPoolProc maxReported:%d\n", maxReported);
		}
	
		POOL_HEAD1 *head1 = DATA_TO_HEAD1(Pointer);
		POOL_HEAD2 *head2 = DATA_TO_HEAD2(Pointer);
		POOL_TAIL1 *tail1 = HEAD_TO_TAIL1(head1);
		POOL_TAIL2 *tail2 = HEAD_TO_TAIL2(head2);
		UINTN head1size = head1->Size - sizeof(POOL_TAIL1) - SIZE_OF_POOL_HEAD1;
		UINTN head2size = head2->Size - sizeof(POOL_TAIL2) - SIZE_OF_POOL_HEAD2;
		INTN type = PoolType(Pointer);
		result = type;
		switch (type) {
			case 1:
				if (always) MsgLog("%a:%d &%a:%p->%p size:%d type:%d\n", f, l, StrPointer, AtPointer, Pointer, head1size, head1->Type);
				result = head1size; break;
			case 2:
				if (always) MsgLog("%a:%d &%a:%p->%p size:%d type:%d\n", f, l, StrPointer, AtPointer, Pointer, head2size, head2->Type);
				result = head2size; break;
			case -1: case -2: case -3: case -5: case -7: case -9: case -10:
				MsgLog("%a:%d &%a:%p->%p problem:%d head:%016lX%016lX%016lX tail:%016lX%016lX\n", f, l, StrPointer, AtPointer, Pointer,
					type, ((UINTN*)head1)[0], ((UINTN*)head1)[1], ((UINTN*)head1)[2], ((UINTN*)tail1)[0], ((UINTN*)tail1)[1]
				);
				break;
			case -4: case -6: case -8: case -11: case -12:
				MsgLog("%a:%d &%a:%p->%p problem:%d head:%016lX%016lX%016lX tail:%016lX%016lX\n", f, l, StrPointer, AtPointer, Pointer,
					type, ((UINTN*)head2)[0], ((UINTN*)head2)[1], ((UINTN*)head2)[2], ((UINTN*)tail2)[0], ((UINTN*)tail2)[1]
				);
				break;
			case -13: case -14: default:
				MsgLog("%a:%d &%a:%p->%p problem:%d head:%016lX%016lX%016lX\n", f, l, StrPointer, AtPointer, Pointer,
					type, ((UINTN*)head1)[0], ((UINTN*)head1)[1], ((UINTN*)head1)[2]
				);
				break;
		}
	} while (0);
	
	curLogProc--;
	return result;
}



EFI_STATUS
EFIAPI
FakeUEFI2EntryPoint( EFI_HANDLE ImageHandle,
                            EFI_SYSTEM_TABLE *SystemTable )
{
    EFI_STATUS Status;
/*
    INTN length;
	for (length = 0; length < 32; length++) {
		VOID *p = AllocateZeroPool(length);
    	DEBUG((DEBUG_INFO, "size:%2d:%p  ", length, p));
    	LogPoolProc (p, &p, "p", "FakeUEFI2.c", 507, TRUE);
	}
*/
    UINTN expectedST  = EFI_FIELD_OFFSET(EFI_SYSTEM_TABLE, ConfigurationTable) + sizeof(gST->ConfigurationTable);
    UINTN expectedBS  = EFI_FIELD_OFFSET(EFI_BOOT_SERVICES, CreateEventEx) + sizeof(gBS->CreateEventEx);
/*
    UINTN expectedRT1 = EFI_FIELD_OFFSET(EFI_RUNTIME_SERVICES, QueryCapsuleCapabilities) + sizeof(gRT->QueryCapsuleCapabilities);
*/
    UINTN expectedRT  = EFI_FIELD_OFFSET(EFI_RUNTIME_SERVICES, QueryVariableInfo) + sizeof(gRT->QueryVariableInfo);
    UINTN expectedDS  = EFI_FIELD_OFFSET(EFI_DXE_SERVICES, SetMemorySpaceCapabilities) + sizeof(gDS->SetMemorySpaceCapabilities);

	DEBUG((DEBUG_INFO, "FakeUEFI2: gST:%p ver:%X size:%X sizeof:%X expected:%X end:%X\n",
			gST, gST->Hdr.Revision, gST->Hdr.HeaderSize, sizeof(*gST), expectedST, (VOID*)gST + gST->Hdr.HeaderSize));
	DEBUG((DEBUG_INFO, "FakeUEFI2: gBS:%p ver:%X size:%X sizeof:%X expected:%X end:%X\n",
			gBS, gBS->Hdr.Revision, gBS->Hdr.HeaderSize, sizeof(*gBS), expectedBS, (VOID*)gBS + gBS->Hdr.HeaderSize));
	DEBUG((DEBUG_INFO, "FakeUEFI2: gRT:%p ver:%X size:%X sizeof:%X expected:%X end:%X\n",
			gRT, gRT->Hdr.Revision, gRT->Hdr.HeaderSize, sizeof(*gRT), expectedRT, (VOID*)gRT + gRT->Hdr.HeaderSize));
	DEBUG((DEBUG_INFO, "FakeUEFI2: gDS:%p ver:%X size:%X sizeof:%X expected:%X end:%X\n",
			gDS, gDS->Hdr.Revision, gDS->Hdr.HeaderSize, sizeof(*gDS), expectedDS, (VOID*)gDS + gDS->Hdr.HeaderSize));

	if (gBS->Hdr.HeaderSize < expectedBS) {
		DEBUG((DEBUG_INFO, "FakeUEFI2: Update gBS\n"));
		EFI_BOOT_SERVICES *newBS = (EFI_BOOT_SERVICES *)AllocateCopyPool(sizeof(*gBS), gBS);
		if (newBS) {
			newBS->CreateEventEx = MyCreateEventEx;
			newBS->Hdr.HeaderSize = sizeof(*gBS);
			gST->BootServices = newBS;
		}
		else {
			DEBUG((DEBUG_ERROR, "FakeUEFI2: AllocateCopyPool fail\n"));
		}
	}

/*
	if (gRT->Hdr.HeaderSize < expectedRT) {
		DEBUG((DEBUG_INFO, "FakeUEFI2: Update gRT\n"));
		EFI_RUNTIME_SERVICES *newRT = (EFI_RUNTIME_SERVICES *)AllocateRuntimeCopyPool(sizeof(*gRT), gRT);
		if (newRT) {
			if (gRT->Hdr.HeaderSize <= expectedRT1) {
				newRT->QueryCapsuleCapabilities = MyQueryCapsuleCapabilities;
			}
			newRT->QueryVariableInfo = MyQueryVariableInfo;
			//newRT->Hdr.HeaderSize = sizeof(*gRT);
			gST->RuntimeServices = newRT;
		}
		else {
			DEBUG((DEBUG_ERROR, "FakeUEFI2: AllocateCopyPool fail\n"));
		}
	}

	INTN Index;
	if (gDS->Hdr.HeaderSize < expectedDS) {
		DEBUG((DEBUG_INFO, "FakeUEFI2: Update gDS\n"));
		EFI_DXE_SERVICES *newDS = (EFI_DXE_SERVICES *)AllocateCopyPool(sizeof(*gDS), gDS);
		if (newDS) {
			newDS->SetMemorySpaceCapabilities = MySetMemorySpaceCapabilities;
			newDS->Hdr.HeaderSize = sizeof(*gDS);
			for (Index = 0; Index < gST->NumberOfTableEntries; Index++) {
				if (CompareGuid (&gEfiDxeServicesTableGuid, &(gST->ConfigurationTable[Index].VendorGuid))) {
					gST->ConfigurationTable[Index].VendorTable = newDS;
				}
			}
		}
		else {
			DEBUG((DEBUG_ERROR, "FakeUEFI2: AllocateCopyPool fail\n"));
		}
	}
*/
	if (gST->Hdr.Revision <= 0x1FFFF) {
		DEBUG((DEBUG_INFO, "FakeUEFI2: Update gST\n"));
		gST->Hdr.Revision = 0x0002001E;
	}

	// install driver model protocol(s).
	Status = EfiLibInstallAllDriverProtocols2( ImageHandle,
											   gST,
											   &gFakeUEFI2Binding,
											   ImageHandle,
											   NULL,
											   &gFakeUEFI2ComponentName2,
											   NULL,
											   &gFakeUEFI2Configuration2,
											   NULL,
											   &gFakeUEFI2Diagnostics2 );

	if (EFI_ERROR(Status)) {
		DEBUG((DEBUG_ERROR, "FakeUEFI2: EfiLibInstallDriverBindingComponentName2 error %r\n", Status));
		return Status;
	}

    return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
FakeUEFI2Unload( EFI_HANDLE ImageHandle )
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN      Index;
    UINTN      HandleCount = 0;
    EFI_HANDLE *HandleBuffer = NULL;

    // get list of all the handles in the handle database.
    Status = gBS->LocateHandleBuffer( AllHandles,
                                      NULL,
                                      NULL,
                                      &HandleCount,
                                      &HandleBuffer );
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "FakeUEFI2: LocateHandleBuffer error %r\n", Status));
        return Status;
    }

    for (Index = 0; Index < HandleCount; Index++) {
        Status = gBS->DisconnectController( HandleBuffer[Index],
                                            ImageHandle,
                                            NULL );
    }

    if (HandleBuffer != NULL) {
        FreePool( HandleBuffer );
    }

    // uninstall protocols installed in the driver entry point
    Status = gBS->UninstallMultipleProtocolInterfaces( ImageHandle,
                                                       &gEfiDriverBindingProtocolGuid, &gFakeUEFI2Binding,
                                                       &gEfiComponentName2ProtocolGuid, &gFakeUEFI2ComponentName2,
                                                       &gEfiDriverConfiguration2ProtocolGuid, gFakeUEFI2Configuration2, 
                                                       &gEfiDriverDiagnostics2ProtocolGuid, &gFakeUEFI2Diagnostics2, 
                                                       NULL );
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "FakeUEFI2: UninstallMultipleProtocolInterfaces returned %r\n", Status));
        return Status;
    }

    return EFI_SUCCESS;
}
