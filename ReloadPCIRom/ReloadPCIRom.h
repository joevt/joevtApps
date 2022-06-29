//
//  Copyright (c) 2020  joevt   All rights reserved.
//
//  ReloadPCIRom 
//
//  License: BSD 2 clause License
//
//  Portions Copyright (c) 2018, Intel Corporation. All rights reserved.
//           See relevant code in EDK11 for exact details
//


#ifndef _ReloadPCIRom_H_
#define _ReloadPCIRom_H_

#define ReloadPCIRom_DEV_SIGNATURE SIGNATURE_32 ('f', 'e', 'd', 'r')

typedef struct {
    UINT32                   Signature;
    EFI_HANDLE               Handle;
    VOID                     *ReloadPCIRomVariable;
    UINT8                    DeviceType;
    BOOLEAN                  FixedDevice;
    UINT16                   Reserved;
    EFI_UNICODE_STRING_TABLE *ControllerNameTable;
} ReloadPCIRom_DEV;

#define ReloadPCIRom_DEV_FROM_THIS(a)  CR (a, ReloadPCIRom_DEV, ReloadPCIRomVariable, ReloadPCIRom_DEV_SIGNATURE)

// Global Variables
extern EFI_DRIVER_BINDING_PROTOCOL        gReloadPCIRomBinding;
extern EFI_COMPONENT_NAME2_PROTOCOL       gReloadPCIRomComponentName2;
extern EFI_DRIVER_DIAGNOSTICS2_PROTOCOL   gReloadPCIRomDiagnostics2;
extern EFI_DRIVER_CONFIGURATION2_PROTOCOL gReloadPCIRomConfiguration2;


EFI_STATUS
EFIAPI
ReloadPCIRomBindingSupported( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                                  IN EFI_HANDLE                  Controller,
                                  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath );

EFI_STATUS
EFIAPI
ReloadPCIRomBindingStart( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                              IN EFI_HANDLE                  Controller,
                              IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath );

EFI_STATUS
EFIAPI
ReloadPCIRomBindingStop( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                             IN EFI_HANDLE                  Controller,
                             IN UINTN                       NumberOfChildren,
                             IN EFI_HANDLE                  *ChildHandleBuffer );

EFI_STATUS
EFIAPI
ReloadPCIRomComponentNameGetDriverName( IN  EFI_COMPONENT_NAME2_PROTOCOL *This,
                                            IN  CHAR8                        *Language,
                                            OUT CHAR16                       **DriverName );

EFI_STATUS
EFIAPI
ReloadPCIRomComponentNameGetControllerName( IN  EFI_COMPONENT_NAME2_PROTOCOL *This,
                                                IN  EFI_HANDLE                   ControllerHandle,
                                                IN  EFI_HANDLE                   ChildHandleL,
                                                IN  CHAR8                        *Language,
                                                OUT CHAR16                       **ControllerName );
 
EFI_STATUS
EFIAPI
ReloadPCIRomRunDiagnostics( IN  EFI_DRIVER_DIAGNOSTICS2_PROTOCOL *This,
                                IN  EFI_HANDLE                       ControllerHandle,
                                IN  EFI_HANDLE                       ChildHandle,
                                IN  EFI_DRIVER_DIAGNOSTIC_TYPE       DiagnosticType,
                                IN  CHAR8                            *Language,
                                OUT EFI_GUID                         **ErrorType,
                                OUT UINTN                            *BufferSize,
                                OUT CHAR16                           **Buffer );

EFI_STATUS
EFIAPI
ReloadPCIRomConfigurationSetOptions( IN  EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                         IN  EFI_HANDLE                               ControllerHandle,
                                         IN  EFI_HANDLE                               ChildHandle,
                                         IN  CHAR8                                    *Language,
                                         OUT EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired );

EFI_STATUS
EFIAPI
ReloadPCIRomConfigurationOptionsValid( IN EFI_DRIVER_CONFIGURATION2_PROTOCOL *This,
                                           IN EFI_HANDLE                         ControllerHandle,
                                           IN EFI_HANDLE                         ChildHandle );

EFI_STATUS
EFIAPI
ReloadPCIRomConfigurationForceDefaults( IN  EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                            IN  EFI_HANDLE                               ControllerHandle,
                                            IN  EFI_HANDLE                               ChildHandle,
                                            IN  UINT32                                   DefaultType,
                                            OUT EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired );

#endif // _ReloadPCIRom_H_
