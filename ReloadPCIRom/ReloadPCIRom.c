//
//  Copyright (c) 2020  joevt   All rights reserved.
//
//  ReloadPCIRom 
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

#include "ReloadPCIRom.h"

/**
  Connects all available drives and controllers.

  @retval EFI_SUCCESS     The operation was successful.
  @retval EFI_ABORTED     The abort mechanism was received.
**/
EFI_STATUS
LoadPciRomConnectAllDriversToAllControllers (
  VOID
  );

/**
  Command entry point.

  @param[in] RomBar       The Rom Base address.
  @param[in] RomSize      The Rom size.
  @param[in] FileName     The file name.

  @retval EFI_SUCCESS             The command completed successfully.
  @retval EFI_INVALID_PARAMETER   Command usage error.
  @retval EFI_UNSUPPORTED         Protocols unsupported.
  @retval EFI_OUT_OF_RESOURCES    Out of memory.
  @retval Other value             Unknown error.
**/
EFI_STATUS
LoadEfiDriversFromRomImage (
  VOID                      *RomBar,
  UINTN                     RomSize,
  CONST CHAR16              *FileName
  );

#define UTILITY_VERSION L"20201109"

#define ReloadPCIRom_VERSION 0x1

EFI_DRIVER_BINDING_PROTOCOL gReloadPCIRomBinding = {
    ReloadPCIRomBindingSupported,
    ReloadPCIRomBindingStart,
    ReloadPCIRomBindingStop,
    ReloadPCIRom_VERSION,
    NULL,
    NULL
};

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_COMPONENT_NAME2_PROTOCOL gReloadPCIRomComponentName2 = {
    (EFI_COMPONENT_NAME2_GET_DRIVER_NAME) ReloadPCIRomComponentNameGetDriverName,
    (EFI_COMPONENT_NAME2_GET_CONTROLLER_NAME) ReloadPCIRomComponentNameGetControllerName,
    "en"
};

GLOBAL_REMOVE_IF_UNREFERENCED 
EFI_UNICODE_STRING_TABLE mReloadPCIRomNameTable[] = {
    { "en", (CHAR16 *) L"ReloadPCIRom" },
    { NULL , NULL }
};

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_DRIVER_DIAGNOSTICS2_PROTOCOL gReloadPCIRomDiagnostics2 = {
    ReloadPCIRomRunDiagnostics,
    "en"
}; 

GLOBAL_REMOVE_IF_UNREFERENCED
EFI_DRIVER_CONFIGURATION2_PROTOCOL gReloadPCIRomConfiguration2 = {
    ReloadPCIRomConfigurationSetOptions,
    ReloadPCIRomConfigurationOptionsValid,
    ReloadPCIRomConfigurationForceDefaults,
    "en"
};


EFI_STATUS
ReloadPCIRomConfigurationSetOptions( EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                         EFI_HANDLE                               ControllerHandle,
                                         EFI_HANDLE                               ChildHandle,
                                         CHAR8                                    *Language,
                                         EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired )
{
	//DEBUG((DEBUG_INFO, "ReloadPCIRomConfigurationSetOptions Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
ReloadPCIRomConfigurationOptionsValid( EFI_DRIVER_CONFIGURATION2_PROTOCOL *This,
                                           EFI_HANDLE                         ControllerHandle,
                                           EFI_HANDLE                         ChildHandle )
{
	//DEBUG((DEBUG_INFO, "ReloadPCIRomConfigurationOptionsValid Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
ReloadPCIRomConfigurationForceDefaults( EFI_DRIVER_CONFIGURATION2_PROTOCOL       *This,
                                            EFI_HANDLE                               ControllerHandle,
                                            EFI_HANDLE                               ChildHandle,
                                            UINT32                                   DefaultType,
                                            EFI_DRIVER_CONFIGURATION_ACTION_REQUIRED *ActionRequired )
{
	//DEBUG((DEBUG_INFO, "ReloadPCIRomConfigurationForceDefaults Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


EFI_STATUS
ReloadPCIRomRunDiagnostics( EFI_DRIVER_DIAGNOSTICS2_PROTOCOL *This,
                                EFI_HANDLE                       ControllerHandle,
                                EFI_HANDLE                       ChildHandle, 
                                EFI_DRIVER_DIAGNOSTIC_TYPE       DiagnosticType, 
                                CHAR8                            *Language,
                                EFI_GUID                         **ErrorType, 
                                UINTN                            *BufferSize,
                                CHAR16                           **Buffer )
{
	//DEBUG((DEBUG_INFO, "ReloadPCIRomRunDiagnostics Controller:%X Child:%X\n", ControllerHandle, ChildHandle));
    return EFI_UNSUPPORTED;
}


//
//  Retrieve user readable name of the driver
//
EFI_STATUS
ReloadPCIRomComponentNameGetDriverName( EFI_COMPONENT_NAME2_PROTOCOL *This,
                                            CHAR8                        *Language,
                                            CHAR16                       **DriverName )
{
    return LookupUnicodeString2( Language,
                                 This->SupportedLanguages,
                                 mReloadPCIRomNameTable,
                                 DriverName,
                                 (BOOLEAN)(This == &gReloadPCIRomComponentName2) );
}


//
//  Retrieve user readable name of controller being managed by a driver
//
EFI_STATUS
ReloadPCIRomComponentNameGetControllerName( EFI_COMPONENT_NAME2_PROTOCOL *This,
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
ReloadPCIRomBindingStart( EFI_DRIVER_BINDING_PROTOCOL *This,
                              EFI_HANDLE                  Controller,
                              EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath )
{
    return EFI_UNSUPPORTED;
}


//
//  Stop this driver on ControllerHandle.
//
EFI_STATUS
ReloadPCIRomBindingStop( EFI_DRIVER_BINDING_PROTOCOL *This,
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
ReloadPCIRomBindingSupported( EFI_DRIVER_BINDING_PROTOCOL *This,
                                  EFI_HANDLE                  Controller,
                                  EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath )
{
    return EFI_UNSUPPORTED;
}



EFI_STATUS
EFIAPI
ReloadPCIRomEntryPoint( EFI_HANDLE ImageHandle,
                            EFI_SYSTEM_TABLE *SystemTable )
{
	EFI_STATUS          Status;
	EFI_HANDLE          *HandleArray = NULL;
	UINTN               HandleArrayCount = 0;
	UINTN               Index;
	EFI_PCI_IO_PROTOCOL *PciIo;
	UINTN               DriverBindingHandleCount;
	EFI_HANDLE          *DriverBindingHandleBuffer;
	BOOLEAN				didload = FALSE;
	CHAR16				RomFileName[280];
	UINTN               HandleIndex;


	Status = gBS->LocateHandleBuffer (
		ByProtocol,
		&gEfiPciIoProtocolGuid,
		NULL,
		&HandleArrayCount,
		&HandleArray
	);
	if (!EFI_ERROR(Status)) {
		for (Index = 0; Index < HandleArrayCount; Index++) {
			Status = gBS->HandleProtocol (
				HandleArray[Index],
				&gEfiPciIoProtocolGuid,
				(void **)&PciIo
			);
			if (!EFI_ERROR(Status)) {
				if (PciIo->RomImage && PciIo->RomSize) {
					DriverBindingHandleBuffer = NULL;
					PARSE_HANDLE_DATABASE_UEFI_DRIVERS (
						HandleArray[Index],
						&DriverBindingHandleCount,
						&DriverBindingHandleBuffer
					);
					
					if (DriverBindingHandleCount == 0) {
						HandleIndex = ConvertHandleToHandleIndex (HandleArray[Index]);
						DEBUG((DEBUG_INFO, "ReloadPCIRom: Handle:%X\n", HandleIndex ));
						didload = TRUE;
						
						UnicodeSPrint (RomFileName, sizeof (RomFileName), L"Handle%X", HandleIndex);
						Status = LoadEfiDriversFromRomImage (
							PciIo->RomImage,
							PciIo->RomSize,
							RomFileName
						);
						if (EFI_ERROR(Status)) {
							DEBUG((DEBUG_INFO, "ReloadPCIRom: error:%r\n", Status ));
						}
					} // if no driver

					SHELL_FREE_NON_NULL(DriverBindingHandleBuffer);
				} // if RomImage
			} // if HandleProtocol
		} // for HandleArrayCount
	} // if LocateHandleBuffer

	if (didload) {
		Status = LoadPciRomConnectAllDriversToAllControllers ();

		DEBUG((DEBUG_INFO, "ReloadPCIRom: LoadPciRomConnectAllDriversToAllControllers %r.\n", Status));
		
		// install driverg model protocol(s).
		Status = EfiLibInstallAllDriverProtocols2( ImageHandle,
												   SystemTable,
												   &gReloadPCIRomBinding,
												   ImageHandle,
												   NULL,
												   &gReloadPCIRomComponentName2,
												   NULL,
												   &gReloadPCIRomConfiguration2,
												   NULL,
												   &gReloadPCIRomDiagnostics2 );

		if (EFI_ERROR(Status)) {
			DEBUG((DEBUG_ERROR, "EfiLibInstallDriverBindingComponentName2 [%d]\n", Status));
			return Status;
		}
	}

    return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
ReloadPCIRomUnload( EFI_HANDLE ImageHandle )
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
                                                       &gEfiDriverBindingProtocolGuid, &gReloadPCIRomBinding,
                                                       &gEfiComponentName2ProtocolGuid, &gReloadPCIRomComponentName2,
                                                       &gEfiDriverConfiguration2ProtocolGuid, gReloadPCIRomConfiguration2, 
                                                       &gEfiDriverDiagnostics2ProtocolGuid, &gReloadPCIRomDiagnostics2, 
                                                       NULL );
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "UninstallMultipleProtocolInterfaces returned %d\n", Status));
        return Status;
    }

    return EFI_SUCCESS;
}



/**
  Command entry point.

  @param[in] RomBar       The Rom Base address.
  @param[in] RomSize      The Rom size.
  @param[in] FileName     The file name.

  @retval EFI_SUCCESS             The command completed successfully.
  @retval EFI_INVALID_PARAMETER   Command usage error.
  @retval EFI_UNSUPPORTED         Protocols unsupported.
  @retval EFI_OUT_OF_RESOURCES    Out of memory.
  @retval Other value             Unknown error.
**/
EFI_STATUS
LoadEfiDriversFromRomImage (
  VOID                      *RomBar,
  UINTN                     RomSize,
  CONST CHAR16              *FileName
  )

{
  EFI_PCI_EXPANSION_ROM_HEADER  *EfiRomHeader;
  PCI_DATA_STRUCTURE            *Pcir;
  UINTN                         ImageIndex;
  UINTN                         RomBarOffset;
  UINT32                        ImageSize;
  UINT16                        ImageOffset;
  EFI_HANDLE                    ImageHandle;
  EFI_STATUS                    Status;
  EFI_STATUS                    ReturnStatus;
  CHAR16                        RomFileName[280];
  EFI_DEVICE_PATH_PROTOCOL      *FilePath;
  BOOLEAN                       SkipImage;
  UINT32                        DestinationSize;
  UINT32                        ScratchSize;
  UINT8                         *Scratch;
  VOID                          *ImageBuffer;
  VOID                          *DecompressedImageBuffer;
  UINT32                        ImageLength;
  EFI_DECOMPRESS_PROTOCOL       *Decompress;
  UINT32                        InitializationSize;

  ImageIndex    = 0;
  ReturnStatus  = EFI_NOT_FOUND;
  RomBarOffset  = (UINTN) RomBar;

  do {

    EfiRomHeader = (EFI_PCI_EXPANSION_ROM_HEADER *) (UINTN) RomBarOffset;

    if (EfiRomHeader->Signature != PCI_EXPANSION_ROM_HEADER_SIGNATURE) {
      DEBUG((DEBUG_ERROR, "%s: File '%s' Image %d is corrupt.\r\n", L"ReloadPCIRom", FileName, ImageIndex));
//      PrintToken (STRING_TOKEN (STR_LOADPCIROM_IMAGE_CORRUPT), HiiHandle, ImageIndex);
      return ReturnStatus;
    }

    //
    // If the pointer to the PCI Data Structure is invalid, no further images can be located.
    // The PCI Data Structure must be DWORD aligned.
    //
    if (EfiRomHeader->PcirOffset == 0 ||
        (EfiRomHeader->PcirOffset & 3) != 0 ||
        RomBarOffset - (UINTN)RomBar + EfiRomHeader->PcirOffset + sizeof (PCI_DATA_STRUCTURE) > RomSize) {
      break;
    }

    Pcir      = (PCI_DATA_STRUCTURE *) (UINTN) (RomBarOffset + EfiRomHeader->PcirOffset);
    //
    // If a valid signature is not present in the PCI Data Structure, no further images can be located.
    //
    if (Pcir->Signature != PCI_DATA_STRUCTURE_SIGNATURE) {
      break;
    }
    ImageSize = Pcir->ImageLength * 512;
    if (RomBarOffset - (UINTN)RomBar + ImageSize > RomSize) {
      break;
    }

    if ((Pcir->CodeType == PCI_CODE_TYPE_EFI_IMAGE) &&
        (EfiRomHeader->EfiSignature == EFI_PCI_EXPANSION_ROM_HEADER_EFISIGNATURE) &&
        ((EfiRomHeader->EfiSubsystem == EFI_IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER) ||
         (EfiRomHeader->EfiSubsystem == EFI_IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER))) {

      ImageOffset             = EfiRomHeader->EfiImageHeaderOffset;
      InitializationSize      = EfiRomHeader->InitializationSize * 512;

      if (InitializationSize <= ImageSize && ImageOffset < InitializationSize) {

        ImageBuffer             = (VOID *) (UINTN) (RomBarOffset + ImageOffset);
        ImageLength             = InitializationSize - ImageOffset;
        DecompressedImageBuffer = NULL;

        //
        // decompress here if needed
        //
        SkipImage = FALSE;
        if (EfiRomHeader->CompressionType > EFI_PCI_EXPANSION_ROM_HEADER_COMPRESSED) {
          SkipImage = TRUE;
        }

        if (EfiRomHeader->CompressionType == EFI_PCI_EXPANSION_ROM_HEADER_COMPRESSED) {
          Status = gBS->LocateProtocol (&gEfiDecompressProtocolGuid, NULL, (VOID**)&Decompress);
          ASSERT_EFI_ERROR(Status);
          if (EFI_ERROR (Status)) {
            SkipImage = TRUE;
          } else {
            SkipImage = TRUE;
            Status = Decompress->GetInfo (
                                  Decompress,
                                  ImageBuffer,
                                  ImageLength,
                                  &DestinationSize,
                                  &ScratchSize
                                 );
            if (!EFI_ERROR (Status)) {
              DecompressedImageBuffer = AllocateZeroPool (DestinationSize);
              if (ImageBuffer != NULL) {
                Scratch = AllocateZeroPool (ScratchSize);
                if (Scratch != NULL) {
                  DEBUG((DEBUG_ERROR, "%s: File '%s' Image %d decompressing.\r\n", L"ReloadPCIRom", FileName, ImageIndex));
                  Status = Decompress->Decompress (
                                        Decompress,
                                        ImageBuffer,
                                        ImageLength,
                                        DecompressedImageBuffer,
                                        DestinationSize,
                                        Scratch,
                                        ScratchSize
                                       );
                  if (!EFI_ERROR (Status)) {
                    ImageBuffer = DecompressedImageBuffer;
                    ImageLength = DestinationSize;
                    SkipImage   = FALSE;
                  }
                  else {
                    DEBUG((DEBUG_ERROR, "%s: File '%s' Image %d decompress error %r.\r\n", L"ReloadPCIRom", FileName, ImageIndex, Status));
                  }

                  FreePool (Scratch);
                }
              }
            }
          }
        }

        if (!SkipImage) {
          //
          // load image and start image
          //
          UnicodeSPrint (RomFileName, sizeof (RomFileName), L"%s[%d]", FileName, ImageIndex);
          FilePath = FileDevicePath (NULL, RomFileName);

          DEBUG((DEBUG_ERROR, "%s: File '%s' Image %d loading.\r\n", L"ReloadPCIRom", FileName, ImageIndex));
          Status = gBS->LoadImage (
                        TRUE,
                        gImageHandle,
                        FilePath,
                        ImageBuffer,
                        ImageLength,
                        &ImageHandle
                       );
          if (EFI_ERROR (Status)) {
            DEBUG((DEBUG_ERROR, "%s: File '%s' Image %d load error %r.\r\n", L"ReloadPCIRom", FileName, ImageIndex, Status));
            //
            // With EFI_SECURITY_VIOLATION retval, the Image was loaded and an ImageHandle was created
            // with a valid EFI_LOADED_IMAGE_PROTOCOL, but the image can not be started right now.
            // If the caller doesn't have the option to defer the execution of an image, we should
            // unload image for the EFI_SECURITY_VIOLATION to avoid resource leak.
            //
            if (Status == EFI_SECURITY_VIOLATION) {
              gBS->UnloadImage (ImageHandle);
            }

//            PrintToken (STRING_TOKEN (STR_LOADPCIROM_LOAD_IMAGE_ERROR), HiiHandle, ImageIndex, Status);
          } else {
            DEBUG((DEBUG_ERROR, "%s: File '%s' Image %d starting.\r\n", L"ReloadPCIRom", FileName, ImageIndex));
            // *****************************
            Status = gBS->StartImage (ImageHandle, NULL, NULL);
            // *****************************
            if (EFI_ERROR (Status)) {
              DEBUG((DEBUG_ERROR, "%s: File '%s' Image %d start error %r.\r\n", L"ReloadPCIRom", FileName, ImageIndex, Status));
//              PrintToken (STRING_TOKEN (STR_LOADPCIROM_START_IMAGE), HiiHandle, ImageIndex, Status);
            } else {
              ReturnStatus = Status;
            }
          }
        }

        if (DecompressedImageBuffer != NULL) {
          FreePool (DecompressedImageBuffer);
        }

      }
    }

    RomBarOffset = RomBarOffset + ImageSize;
    ImageIndex++;
  } while (((Pcir->Indicator & 0x80) == 0x00) && ((RomBarOffset - (UINTN) RomBar) < RomSize));

  return ReturnStatus;
}


/**
  Connects all available drives and controllers.

  @retval EFI_SUCCESS     The operation was successful.
  @retval EFI_ABORTED     The abort mechanism was received.
**/
EFI_STATUS
LoadPciRomConnectAllDriversToAllControllers (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       HandleCount;
  EFI_HANDLE  *HandleBuffer;
  UINTN       Index;

  Status = gBS->LocateHandleBuffer (
                  AllHandles,
                  NULL,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "%s: Locate all handles error %r.\r\n", L"ReloadPCIRom", Status));
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
  }

  if (HandleBuffer != NULL) {
    FreePool (HandleBuffer);
  }
  return Status;
}
