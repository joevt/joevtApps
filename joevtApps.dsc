
[Defines]
  PLATFORM_NAME                  = joevtApps
  PLATFORM_GUID                  = 72498222-0521-413d-9e5a-73a0995b45fd
  PLATFORM_VERSION               = 0.01
  DSC_SPECIFICATION              = 0x00010006
  OUTPUT_DIRECTORY               = Build/joevtApps
  SUPPORTED_ARCHITECTURES        = IA32|X64|EBC
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT

#
#  Debug output control    (DebugLib.h)
#
  !if $(TARGET) == "RELEASE"
  DEFINE DEBUG_ENABLE_OUTPUT      = TRUE        # FALSE      Set to TRUE to enable debug output
  DEFINE DEBUG_PRINT_ERROR_LEVEL  = 0x80000000  # 0x80000040 40:DEBUG_INFO | 80000000:DEBUG_ERROR
  DEFINE DEBUG_PROPERTY_MASK      = 0x02        # 0          1:DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED | 2:DEBUG_PROPERTY_DEBUG_PRINT_ENABLED | 4:DEBUG_PROPERTY_DEBUG_CODE_ENABLED
  !else
  DEFINE DEBUG_ENABLE_OUTPUT      = TRUE        # FALSE      Set to TRUE to enable debug output
  DEFINE DEBUG_PRINT_ERROR_LEVEL  = 0xFFFFFFFF  # 0x80000040 40:DEBUG_INFO | 80000000:DEBUG_ERROR
  DEFINE DEBUG_PROPERTY_MASK      = 0x06        # 0          1:DEBUG_PROPERTY_DEBUG_ASSERT_ENABLED | 2:DEBUG_PROPERTY_DEBUG_PRINT_ENABLED | 4:DEBUG_PROPERTY_DEBUG_CODE_ENABLED
  !endif
  EFI1_COMPAT                     = TRUE        # FALSE      Set to TRUE for EFI 1.1 compatibility


[PcdsFeatureFlag]

[PcdsFixedAtBuild]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|$(DEBUG_PROPERTY_MASK)
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|$(DEBUG_PRINT_ERROR_LEVEL)

  # Thunderbolt Configuration
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcDcSwitch|0x0
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcpiGpeSignature|0
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAcpiGpeSignaturePorting|0
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtAspm|0x0
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtCioPlugEventGpioPad|0x02010011
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtControllerEn|0x1
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtControllerType|0x1
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtForcepowerGpioPad|13
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtGpioAccessType|0x2
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtGpioLevel|0x1
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtHotNotify|0x1
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtHotSMI|0x1
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieExtraBusRsvd|56
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieMemAddrRngMax|26
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieMemRsvd|100
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPciePMemAddrRngMax|28
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPciePMemRsvd|100
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtPcieRpNumber|0x1
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtSecurityMode|0x1
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtGpio5Filter               
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtSetClkReq|0x1
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtWakeupSupport|0x0
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdDTbtWin10Support|0x0
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdExpander|0x0
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdPchPcieRootPortHpe|0x00000001
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdRtd3Tbt|0x1
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtClkReq|0x1
 #gCometlakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtClkReqDelay|0x0
  gCometlakeOpenBoardPkgTokenSpaceGuid.PcdRtd3TbtOffDelay|5000

[PcdsFixedAtBuild.IPF]

[LibraryClasses]
  # Entry Point Libraries
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf

  # Common Libraries
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf


  #
  # To save size, use NULL library for DebugLib and ReportStatusCodeLib.
  #

  !if $(DEBUG_ENABLE_OUTPUT)
    !if $(EFI1_COMPAT)
	    DebugLib|joevtApps/EdkDebugLibConOut/UefiDebugLibConOut.inf
	!else
	    DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
	!endif
    DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  !else   ## DEBUG_ENABLE_OUTPUT
    DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  !endif  ## DEBUG_ENABLE_OUTPUT
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf

  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
  HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf

  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  #PeiLib|Include/Library/PeiLib.h

  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf



  # Libraries from edk2-platforms
  
  # Libraries from and for OpenCorePkg
  OcBlitLib|OpenCorePkg/Library/OcBlitLib/OcBlitLib.inf
  OcConsoleLib|OpenCorePkg/Library/OcConsoleLib/OcConsoleLib.inf
  OcMiscLib|OpenCorePkg/Library/OcMiscLib/OcMiscLib.inf
  OcFileLib|OpenCorePkg/Library/OcFileLib/OcFileLib.inf
  OcGuardLib|OpenCorePkg/Library/OcGuardLib/OcGuardLib.inf
  OcStringLib|OpenCorePkg/Library/OcStringLib/OcStringLib.inf
  OcDevicePathLib|OpenCorePkg/Library/OcDevicePathLib/OcDevicePathLib.inf
  FrameBufferBltLib|MdeModulePkg/Library/FrameBufferBltLib/FrameBufferBltLib.inf
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf

[Components]
  joevtApps/XhciDxe/XhciDxe.inf {
    <PcdsFixedAtBuild>
      #gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x0
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0xFF
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0xFFFFFFFF
  }
  joevtApps/FvSimpleFileSystemDxe/FvSimpleFileSystemDxe.inf
  joevtApps/Fv2OnFvThunk/Fv2OnFvThunk.inf
  joevtApps/gfxutil_efi/gfxutil.inf

  joevtApps/FakeUEFI2/FakeUEFI2.inf
  joevtApps/ReloadPCIRom/ReloadPCIRom.inf
  joevtApps/UgaOnGop/UgaOnGop.inf
# joevtApps/GopOnUga/GopOnUga.inf
  joevtApps/FixPCIeLinkRate/FixPCIeLinkRate.inf

  MdeModulePkg/Bus/Pci/PciSioSerialDxe/PciSioSerialDxe.inf
# MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
# MdeModulePkg/Universal/SerialDxe/SerialDxe.inf {
#   <LibraryClasses>
#     SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
# }

[BuildOptions]
  XCODE:*_*_*_CC_FLAGS = -save-temps -D OC_TARGET_$(TARGET)=1 -Wno-error=parentheses-equality -Wno-error=tautological-bitwise-compare
  GCC:*_*_*_CC_FLAGS   = -save-temps -D OC_TARGET_$(TARGET)=1
  MSFT:*_*_*_CC_FLAGS  = -save-temps -D OC_TARGET_$(TARGET)=1
