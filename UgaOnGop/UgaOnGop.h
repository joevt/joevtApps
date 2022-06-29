//
//  Copyright (c) 2020  joevt   All rights reserved.
//
//  UgaOnGop 
//
//  License: BSD 2 clause License
//
//  Portions Copyright (c) 2018, Intel Corporation. All rights reserved.
//           See relevant code in EDK11 for exact details
//


#ifndef _UgaOnGop_H_
#define _UgaOnGop_H_

#define UgaOnGop_DEV_SIGNATURE SIGNATURE_32 ('f', 'e', 'd', 'r')

typedef struct {
    UINT32                   Signature;
    EFI_HANDLE               Handle;
    VOID                     *UgaOnGopVariable;
    UINT8                    DeviceType;
    BOOLEAN                  FixedDevice;
    UINT16                   Reserved;
    EFI_UNICODE_STRING_TABLE *ControllerNameTable;
} UgaOnGop_DEV;

#define UgaOnGop_DEV_FROM_THIS(a)  CR (a, UgaOnGop_DEV, UgaOnGopVariable, UgaOnGop_DEV_SIGNATURE)

// Global Variables
extern EFI_DRIVER_BINDING_PROTOCOL        gUgaOnGopBinding;
extern EFI_COMPONENT_NAME2_PROTOCOL       gUgaOnGopComponentName2;
extern EFI_DRIVER_DIAGNOSTICS2_PROTOCOL   gUgaOnGopDiagnostics2;
extern EFI_DRIVER_CONFIGURATION2_PROTOCOL gUgaOnGopConfiguration2;


EFI_STATUS
EFIAPI
UgaOnGopBindingSupported( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                                  IN EFI_HANDLE                  Controller,
                                  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath );

EFI_STATUS
EFIAPI
UgaOnGopBindingStart( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                              IN EFI_HANDLE                  Controller,
                              IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath );

EFI_STATUS
EFIAPI
UgaOnGopBindingStop( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                             IN EFI_HANDLE                  Controller,
                             IN UINTN                       NumberOfChildren,
                             IN EFI_HANDLE                  *ChildHandleBuffer );

EFI_STATUS
EFIAPI
UgaOnGopComponentNameGetDriverName( IN  EFI_COMPONENT_NAME2_PROTOCOL *This,
                                            IN  CHAR8                        *Language,
                                            OUT CHAR16                       **DriverName );

EFI_STATUS
EFIAPI
UgaOnGopComponentNameGetControllerName( IN  EFI_COMPONENT_NAME2_PROTOCOL *This,
                                                IN  EFI_HANDLE                   ControllerHandle,
                                                IN  EFI_HANDLE                   ChildHandleL,
                                                IN  CHAR8                        *Language,
                                                OUT CHAR16                       **ControllerName );
 
EFI_STATUS
EFIAPI
UgaOnGopRunDiagnostics( IN  EFI_DRIVER_DIAGNOSTICS2_PROTOCOL *This,
                                IN  EFI_HANDLE                       ControllerHandle,
                                IN  EFI_HANDLE                       ChildHandle,
                                IN  EFI_DRIVER_DIAGNOSTIC_TYPE       DiagnosticType,
                                IN  CHAR8                            *Language,
                                OUT EFI_GUID                         **ErrorType,
                                OUT UINTN                            *BufferSize,
                                OUT CHAR16                           **Buffer );

EFI_STATUS
EFIAPI
UgaOnGopConfigurationSetOptions( IN  EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                         IN  EFI_HANDLE                               ControllerHandle,
                                         IN  EFI_HANDLE                               ChildHandle,
                                         IN  CHAR8                                    *Language,
                                         OUT EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired );

EFI_STATUS
EFIAPI
UgaOnGopConfigurationOptionsValid( IN EFI_DRIVER_CONFIGURATION2_PROTOCOL *This,
                                           IN EFI_HANDLE                         ControllerHandle,
                                           IN EFI_HANDLE                         ChildHandle );

EFI_STATUS
EFIAPI
UgaOnGopConfigurationForceDefaults( IN  EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                            IN  EFI_HANDLE                               ControllerHandle,
                                            IN  EFI_HANDLE                               ChildHandle,
                                            IN  UINT32                                   DefaultType,
                                            OUT EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired );

#endif // _UgaOnGop_H_
