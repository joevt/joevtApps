//
//  Copyright (c) 2020  joevt   All rights reserved.
//
//  UgaOnGop 
//
//  License: BSD 2 clause License
//
//  Portions Copyright (c) 2016-2017, Microsoft Corporation
//           Copyright (c) 2018, Intel Corporation. All rights reserved.
//           See relevant code in EDK11 for exact details
//

#include <Uefi.h>

#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/PrintLib.h>
#include <Library/HandleParsingLib.h>
#include <Library/DevicePathLib.h>
//#include <Library/FileHandleLib.h>

#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextInEx.h>
#include <Protocol/Shell.h>
#include <Protocol/ComponentName.h>
#include <Protocol/DriverBinding.h>
#include <Protocol/DriverDiagnostics2.h>
#include <Protocol/DriverConfiguration2.h>
#include <Protocol/PciIo.h>
#include <Protocol/Decompress.h>

#include <IndustryStandard/Pci22.h>
#include <IndustryStandard/Pci23.h>
#include <IndustryStandard/PeImage.h>

#include "UgaOnGop.h"


/**
  Provide UGA protocol instances on top of existing GOP instances.

  @retval EFI_SUCCESS on success.
**/
EFI_STATUS
OcProvideUgaPassThrough (
  VOID
  );


#define UTILITY_VERSION L"20201114"

#define UgaOnGop_VERSION 0x1

EFI_DRIVER_BINDING_PROTOCOL gUgaOnGopBinding = {
    UgaOnGopBindingSupported,
    UgaOnGopBindingStart,
    UgaOnGopBindingStop,
    UgaOnGop_VERSION,
    NULL,
    NULL
};

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_COMPONENT_NAME2_PROTOCOL gUgaOnGopComponentName2 = {
    (EFI_COMPONENT_NAME2_GET_DRIVER_NAME) UgaOnGopComponentNameGetDriverName,
    (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) UgaOnGopComponentNameGetControllerName,
    "en"
};

GLOBAL_REMOVE_IF_UNREFERENCED 
EFI_UNICODE_STRING_TABLE mUgaOnGopNameTable[] = {
    { "en", (CHAR16 *) L"UgaOnGop" },
    { NULL , NULL }
};

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_DRIVER_DIAGNOSTICS2_PROTOCOL gUgaOnGopDiagnostics2 = {
    UgaOnGopRunDiagnostics,
    "en"
}; 

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_DRIVER_CONFIGURATION2_PROTOCOL gUgaOnGopConfiguration2 = {
    UgaOnGopConfigurationSetOptions,
    UgaOnGopConfigurationOptionsValid,
    UgaOnGopConfigurationForceDefaults,
    "en"
};


EFI_STATUS
UgaOnGopConfigurationSetOptions( EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                         EFI_HANDLE                               ControllerHandle,
                                         EFI_HANDLE                               ChildHandle,
                                         CHAR8                                    *Language,
                                         EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired )
{
	//DEBUG((DEBUG_INFO, "UgaOnGopConfigurationSetOptions Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
UgaOnGopConfigurationOptionsValid( EFI_DRIVER_CONFIGURATION2_PROTOCOL *This,
                                           EFI_HANDLE                         ControllerHandle,
                                           EFI_HANDLE                         ChildHandle )
{
	//DEBUG((DEBUG_INFO, "UgaOnGopConfigurationOptionsValid Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
UgaOnGopConfigurationForceDefaults( EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                            EFI_HANDLE                               ControllerHandle,
                                            EFI_HANDLE                               ChildHandle,
                                            UINT32                                   DefaultType,
                                            EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired )
{
	//DEBUG((DEBUG_INFO, "UgaOnGopConfigurationForceDefaults Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
UgaOnGopRunDiagnostics( EFI_DRIVER_DIAGNOSTICS2_PROTOCOL *This,
                                EFI_HANDLE                       ControllerHandle,
                                EFI_HANDLE                       ChildHandle, 
                                EFI_DRIVER_DIAGNOSTIC_TYPE       DiagnosticType, 
                                CHAR8                            *Language,
                                EFI_GUID                         **ErrorType, 
                                UINTN                            *BufferSize,
                                CHAR16                           **Buffer )
{
	//DEBUG((DEBUG_INFO, "UgaOnGopRunDiagnostics Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


//
//  Retrieve user readable name of the driver
//
EFI_STATUS
UgaOnGopComponentNameGetDriverName( EFI_COMPONENT_NAME2_PROTOCOL *This,
                                            CHAR8                        *Language,
                                            CHAR16                       **DriverName )
{
    return LookupUnicodeString2( Language,
                                 This->SupportedLanguages,
                                 mUgaOnGopNameTable,
                                 DriverName,
                                 (BOOLEAN)(This == &gUgaOnGopComponentName2) );
}


//
//  Retrieve user readable name of controller being managed by a driver
//
EFI_STATUS
UgaOnGopComponentNameGetControllerName( EFI_COMPONENT_NAME2_PROTOCOL *This,
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
UgaOnGopBindingStart( EFI_DRIVER_BINDING_PROTOCOL *This,
                              EFI_HANDLE                  Controller,
                              EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath )
{
    return EFI_UNSUPPORTED;
}


//
//  Stop this driver on ControllerHandle.
//
EFI_STATUS
UgaOnGopBindingStop( EFI_DRIVER_BINDING_PROTOCOL *This,
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
UgaOnGopBindingSupported( EFI_DRIVER_BINDING_PROTOCOL *This,
                                  EFI_HANDLE                  Controller,
                                  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath )
{
    return EFI_UNSUPPORTED;
}



EFI_STATUS
EFIAPI
UgaOnGopEntryPoint( EFI_HANDLE ImageHandle,
                            EFI_SYSTEM_TABLE *SystemTable )
{
    return OcProvideUgaPassThrough();
}


EFI_STATUS
EFIAPI
UgaOnGopUnload( EFI_HANDLE ImageHandle )
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
        DEBUG((DEBUG_ERROR, "LocateHandleBuffer [%d]\n", Status));
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
                                                       &gEfiDriverBindingProtocolGuid, &gUgaOnGopBinding,
                                                       &gEfiComponentName2ProtocolGuid, &gUgaOnGopComponentName2,
                                                       &gEfiDriverConfiguration2ProtocolGuid, gUgaOnGopConfiguration2, 
                                                       &gEfiDriverDiagnostics2ProtocolGuid, &gUgaOnGopDiagnostics2, 
                                                       NULL );
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "UninstallMultipleProtocolInterfaces returned %d\n", Status));
        return Status;
    }

    return EFI_SUCCESS;
}
