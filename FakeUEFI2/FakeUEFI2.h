//
//  Copyright (c) 2020  joevt   All rights reserved.
//
//  FakeUEFI2 
//
//  License: BSD 2 clause License
//
//  Portions Copyright (c) 2018, Intel Corporation. All rights reserved.
//           See relevant code in EDK11 for exact details
//


#ifndef _FakeUEFI2_H_
#define _FakeUEFI2_H_

#define FakeUEFI2_DEV_SIGNATURE SIGNATURE_32 ('f', 'e', 'd', 'r')

typedef struct {
    UINT32                   Signature;
    EFI_HANDLE               Handle;
    VOID                     *FakeUEFI2Variable;
    UINT8                    DeviceType;
    BOOLEAN                  FixedDevice;
    UINT16                   Reserved;
    EFI_UNICODE_STRING_TABLE *ControllerNameTable;
} FakeUEFI2_DEV;

#define FakeUEFI2_DEV_FROM_THIS(a)  CR (a, FakeUEFI2_DEV, FakeUEFI2Variable, FakeUEFI2_DEV_SIGNATURE)

// Global Variables
extern EFI_DRIVER_BINDING_PROTOCOL        gFakeUEFI2Binding;
extern EFI_COMPONENT_NAME2_PROTOCOL       gFakeUEFI2ComponentName2;
extern EFI_DRIVER_DIAGNOSTICS2_PROTOCOL   gFakeUEFI2Diagnostics2;
extern EFI_DRIVER_CONFIGURATION2_PROTOCOL gFakeUEFI2Configuration2;


EFI_STATUS
EFIAPI
FakeUEFI2BindingSupported( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                                  IN EFI_HANDLE                  Controller,
                                  IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath );

EFI_STATUS
EFIAPI
FakeUEFI2BindingStart( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                              IN EFI_HANDLE                  Controller,
                              IN EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath );

EFI_STATUS
EFIAPI
FakeUEFI2BindingStop( IN EFI_DRIVER_BINDING_PROTOCOL *This,
                             IN EFI_HANDLE                  Controller,
                             IN UINTN                       NumberOfChildren,
                             IN EFI_HANDLE                  *ChildHandleBuffer );

EFI_STATUS
EFIAPI
FakeUEFI2ComponentNameGetDriverName( IN  EFI_COMPONENT_NAME2_PROTOCOL *This,
                                            IN  CHAR8                        *Language,
                                            OUT CHAR16                       **DriverName );

EFI_STATUS
EFIAPI
FakeUEFI2ComponentNameGetControllerName( IN  EFI_COMPONENT_NAME2_PROTOCOL *This,
                                                IN  EFI_HANDLE                   ControllerHandle,
                                                IN  EFI_HANDLE                   ChildHandleL,
                                                IN  CHAR8                        *Language,
                                                OUT CHAR16                       **ControllerName );
 
EFI_STATUS
EFIAPI
FakeUEFI2RunDiagnostics( IN  EFI_DRIVER_DIAGNOSTICS2_PROTOCOL *This,
                                IN  EFI_HANDLE                       ControllerHandle,
                                IN  EFI_HANDLE                       ChildHandle,
                                IN  EFI_DRIVER_DIAGNOSTIC_TYPE       DiagnosticType,
                                IN  CHAR8                            *Language,
                                OUT EFI_GUID                         **ErrorType,
                                OUT UINTN                            *BufferSize,
                                OUT CHAR16                           **Buffer );

EFI_STATUS
EFIAPI
FakeUEFI2ConfigurationSetOptions( IN  EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                         IN  EFI_HANDLE                               ControllerHandle,
                                         IN  EFI_HANDLE                               ChildHandle,
                                         IN  CHAR8                                    *Language,
                                         OUT EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired );

EFI_STATUS
EFIAPI
FakeUEFI2ConfigurationOptionsValid( IN EFI_DRIVER_CONFIGURATION2_PROTOCOL *This,
                                           IN EFI_HANDLE                         ControllerHandle,
                                           IN EFI_HANDLE                         ChildHandle );

EFI_STATUS
EFIAPI
FakeUEFI2ConfigurationForceDefaults( IN  EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                            IN  EFI_HANDLE                               ControllerHandle,
                                            IN  EFI_HANDLE                               ChildHandle,
                                            IN  UINT32                                   DefaultType,
                                            OUT EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired );

#endif // _FakeUEFI2_H_
