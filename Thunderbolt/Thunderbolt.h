#ifndef _THUNDERBOLT_H_
#define _THUNDERBOLT_H_

#include <Uefi.h>

#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
//#include <Pi/PiPeiCis.h>
//
//#include <Ppi/ReadOnlyVariable2.h>
//
//#include <Library/DebugLib.h>
//#include <Library/UefiLib.h>
//#include <Library/BaseLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/MemoryAllocationLib.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>
//#include <Library/UefiDriverEntryPoint.h>
//#include <Library/PrintLib.h>
//#include <Library/HandleParsingLib.h>
//#include <Library/DevicePathLib.h>
//#include <Library/SortLib.h>
//#include <Library/PciSegmentLib.h>
//#include <Library/PcdLib.h>
//#include <Library/TimerLib.h>
//#include <Library/PeiServicesLib.h>
//#include <Library/IoLib.h>
//
//#include <Protocol/LoadedImage.h>
//#include <Protocol/SimpleFileSystem.h>
//#include <Protocol/GraphicsOutput.h>
//#include <Protocol/SimpleTextInEx.h>
//#include <Protocol/ComponentName.h>
//#include <Protocol/DriverBinding.h>
//#include <Protocol/DriverDiagnostics2.h>
//#include <Protocol/DriverConfiguration2.h>
//#include <Protocol/PciIo.h>
//#include <Protocol/PciRootBridgeIo.h>
//#include <Protocol/Decompress.h>
#include <Protocol/PciHotPlugInit.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmPeriodicTimerDispatch2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmPowerButtonDispatch2.h>
#include <Protocol/SmmUsbDispatch2.h>
#include <Protocol/SmmGpiDispatch2.h>
//
//#include <IndustryStandard/Pci.h>
//#include <IndustryStandard/PeImage.h>


//========================================================================================
// Thunderbolt

/**
  Get PCH PWRM base address.

  @param[out] Address                   Address of PWRM base address.

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_INVALID_PARAMETER         Invalid pointer passed.
**/
EFI_STATUS
EFIAPI
PchPwrmBaseGet (
  OUT UINT32                            *Address
  );


EFI_STATUS
EFIAPI
PchPwrmBaseGet (
  OUT UINT32                            *Address
  )
{
	return 0;
}

//========================================================================================
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Library/PeiCheckIommuSupportLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Library/PeiCheckIommuSupportLib.h

/** @file
  Header file for the PeiCheckIommuSupport library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_CHECK_IOMMU_SUPPORT_LIBRARY_H_
#define _PEI_CHECK_IOMMU_SUPPORT_LIBRARY_H_

/**
  Check Iommu Ability base on SKU type, CSME FW type, Vtd and setup options.
**/
VOID
PeiCheckIommuSupport (
  VOID
  );

#endif // _PEI_CHECK_IOMMU_SUPPORT_LIBRARY_H_


//========================================================================================
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Library/DxeCheckIommuSupportLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Library/DxeCheckIommuSupportLib.h

/** @file
  Header file for the DxeCheckIommuSupport library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_CHECK_IOMMU_SUPPORT_LIBRARY_H_
#define _DXE_CHECK_IOMMU_SUPPORT_LIBRARY_H_

/**
  Detect ME FW and Board Type and return the result via IommuSkuCheck.

  IommuSkuCheck
  BIT0: Indicate system has a Corporate CSME firmware
  BIT1: Indicate BIOS is running on a CML RVP
  BIT2: Indicate BIOS is running on a CFL-H RVP
  BIT3: Indicate BIOS is running on a CFL-S 8+2 RVP

  @retval Return 0 means not support, otherwise value is defined by IommuSkuCheck
**/
UINT8
DetectMeAndBoard (
  VOID
  );

/**
  DxeCheckIommuSupport

  Only WHL/CFL-H/CFL-S 8+2 Crop SKUs support Iommu.
  This function will save sku information to PcdIommuSkuCheck.
  BIOS will use PcdIommuSkuCheck and other factors to set PcdVTdPolicyPropertyMask on the next boot in PEI phase

  This function might perform a system reset.
**/
EFI_STATUS
EFIAPI
DxeCheckIommuSupport (
  VOID
  );
#endif // _DXE_CHECK_IOMMU_SUPPORT_LIBRARY_H_


//========================================================================================
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Protocol/DisableBmeProtocol.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Protocol/DisableBmeProtocol.h

/** @file
  Definitions for DisableBmeProtocol


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DISABLE_TBT_BME_PROTOCOL_H_
#define _DISABLE_TBT_BME_PROTOCOL_H_

typedef struct EFI_DISABLE_BME_PROTOCOL EFI_DISABLE_TBT_BME_PROTOCOL;

/**
  This is for disable TBT BME bit under shell environment

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
typedef
VOID
(EFIAPI *DISABLE_BUS_MASTER_ENABLE) (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


struct EFI_DISABLE_BME_PROTOCOL {
  DISABLE_BUS_MASTER_ENABLE DisableBme;
};

extern EFI_GUID gDxeDisableTbtBmeProtocolGuid;


#endif


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Acpi/TbtNvsAreaDef.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/TbtNvsAreaDef.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/TbtNvsAreaDef.h

/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

  //
  // Define TBT NVS Area operation region.
  //

#ifndef _TBT_NVS_AREA_DEF_H_
#define _TBT_NVS_AREA_DEF_H_

#pragma pack (push,1)
typedef struct {
  UINT8    ThunderboltSmiFunction;                  ///< Offset 0       Thunderbolt(TM) SMI Function Number
  UINT8    ThunderboltHotSmi;                       ///< Offset 1       SMI on Hot Plug for TBT devices
  UINT8    TbtWin10Support;                         ///< Offset 2       TbtWin10Support
  UINT8    TbtGpioFilter;                           ///< Offset 3       Gpio filter to detect USB Hotplug event
  UINT8    ThunderboltHotNotify;                    ///< Offset 4       Notify on Hot Plug for TBT devices
  UINT8    TbtSelector;                             ///< Offset 5       Thunderbolt(TM) Root port selector
  UINT8    WAKFinished;                             ///< Offset 6       WAK Finished
  UINT8    DiscreteTbtSupport;                      ///< Offset 7       Thunderbolt(TM) support
  UINT8    TbtAcpiRemovalSupport;                   ///< Offset 8       TbtAcpiRemovalSupport
  UINT32   TbtFrcPwrEn;                             ///< Offset 9       TbtFrcPwrEn
  UINT32   TbtFrcPwrGpioNo0;                        ///< Offset 13      TbtFrcPwrGpioNo
  UINT8    TbtFrcPwrGpioLevel0;                     ///< Offset 17      TbtFrcPwrGpioLevel
  UINT32   TbtCioPlugEventGpioNo0;                  ///< Offset 18      TbtCioPlugEventGpioNo
  UINT32   TbtPcieRstGpioNo0;                       ///< Offset 22      TbtPcieRstGpioNo
  UINT8    TbtPcieRstGpioLevel0;                    ///< Offset 26      TbtPcieRstGpioLevel
  UINT8    CurrentDiscreteTbtRootPort;              ///< Offset 27      Current Port that has plug event
  UINT8    RootportSelected0;                       ///< Offset 28      Root port Selected by the User
  UINT8    RootportSelected0Type;                   ///< Offset 29      Root port Type
  UINT8    RootportSelected1;                       ///< Offset 30      Root port Selected by the User
  UINT8    RootportSelected1Type;                   ///< Offset 31      Root port Type
  UINT8    RootportEnabled0;                        ///< Offset 32      Root port Enabled by the User
  UINT8    RootportEnabled1;                        ///< Offset 33      Root port Enabled by the User
  UINT32   TbtFrcPwrGpioNo1;                        ///< Offset 34      TbtFrcPwrGpioNo
  UINT8    TbtFrcPwrGpioLevel1;                     ///< Offset 38      TbtFrcPwrGpioLevel
  UINT32   TbtCioPlugEventGpioNo1;                  ///< Offset 39      TbtCioPlugEventGpioNo
  UINT32   TbtPcieRstGpioNo1;                       ///< Offset 43      TbtPcieRstGpioNo
  UINT8    TbtPcieRstGpioLevel1;                    ///< Offset 47      TbtPcieRstGpioLevel
  UINT8    TBtCommonGpioSupport;                    ///< Offset 48      Set if Single GPIO is used for Multi/Different Controller Hot plug support
  UINT8    CurrentDiscreteTbtRootPortType;          ///< Offset 49      Root Port type for which SCI Triggered
  UINT8    TrOsup;                                  ///< Offset 50      Titan Ridge Osup command
  UINT8    TbtAcDcSwitch;                           ///< Offset 51      TBT Dynamic AcDc L1
  UINT8    DTbtControllerEn0;                       ///< Offset 52      DTbtController0 is enabled or not.  @deprecated since revision 2
  UINT8    DTbtControllerEn1;                       ///< Offset 53      DTbtController1 is enabled or not.  @deprecated since revision 2
  UINT8    TbtAspm;                                 ///< Offset 54      ASPM setting for all the PCIe device in TBT daisy chain.
  UINT8    TbtL1SubStates;                          ///< Offset 55      L1 SubState for for all the PCIe device in TBT daisy chain.
  UINT8    TbtSetClkReq;                            ///< Offset 56      CLK REQ for all the PCIe device in TBT daisy chain.
  UINT8    TbtLtr;                                  ///< Offset 57      LTR for for all the PCIe device in TBT daisy chain.
  UINT8    TbtPtm;                                  ///< Offset 58      PTM for for all the PCIe device in TBT daisy chain.
  UINT8    TbtWakeupSupport;                        ///< Offset 59      Send Go2SxNoWake or GoSxWake according to TbtWakeupSupport
  UINT16   Rtd3TbtOffDelay;                         ///< Offset 60      Rtd3TbtOffDelay TBT RTD3 Off Delay
  UINT8    TbtSxWakeSwitchLogicEnable;              ///< Offset 62      TbtSxWakeSwitchLogicEnable Set True if TBT_WAKE_N will be routed to PCH WakeB at Sx entry point. HW logic is required.
  UINT8    Rtd3TbtSupport;                          ///< Offset 63      Enable Rtd3 support for TBT. Corresponding to Rtd3Tbt in Setup.
  UINT8    Rtd3TbtClkReq;                           ///< Offset 64      Enable TBT RTD3 CLKREQ mask.
  UINT16   Rtd3TbtClkReqDelay;                      ///< Offset 65      TBT RTD3 CLKREQ mask delay.
  //
  // Revision Field:
  //
  UINT8    TbtRevision;                             ///< Offset 67      Revison of TbtNvsArea
} TBT_NVS_AREA;

#pragma pack(pop)
#endif


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Protocol/TbtNvsArea.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Protocol/TbtNvsArea.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Protocol/TbtNvsArea.h

/** @file
  This file defines the TBT NVS Area Protocol.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_NVS_AREA_H_
#define _TBT_NVS_AREA_H_

//
// Platform NVS Area definition
//
//#include <TbtNvsAreaDef.h>

//
// Includes
//
#define TBT_NVS_DEVICE_ENABLE 1
#define TBT_NVS_DEVICE_DISABLE 0

//
// Forward reference for pure ANSI compatibility
//
typedef struct _TBT_NVS_AREA_PROTOCOL TBT_NVS_AREA_PROTOCOL;

///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gTbtNvsAreaProtocolGuid;

/**
 Making any TBT_NVS_AREA structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:   - Initial version.\n
  <b>Revision 2</b>:   - Adding TBT NVS AREA Revision, Deprecated DTbtControllerEn0, DTbtControllerEn1.\n
**/
#define TBT_NVS_AREA_REVISION       2

//
// Platform NVS Area Protocol
//
typedef struct _TBT_NVS_AREA_PROTOCOL {
  TBT_NVS_AREA     *Area;
} TBT_NVS_AREA_PROTOCOL;

#endif // _TBT_NVS_AREA_H_


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Library/PeiTbtPolicyLib.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Library/PeiTbtPolicyLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Library/PeiTbtPolicyLib.h

/** @file
  Prototype of the PeiTbtPolicyLib library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_TBT_POLICY_LIB_H_
#define _PEI_TBT_POLICY_LIB_H_

/**
  Install Tbt Policy

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallPeiTbtPolicy (
  VOID
  );

/**
  Update PEI TBT Policy Callback
**/
VOID
EFIAPI
UpdatePeiTbtPolicyCallback (
  VOID
  );

/**
  Print PEI TBT Policy
**/
VOID
EFIAPI
TbtPrintPeiPolicyConfig (
  VOID
  );
#endif // _DXE_TBT_POLICY_LIB_H_


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Library/DxeTbtPolicyLib.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Library/DxeTbtPolicyLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Library/DxeTbtPolicyLib.h

/** @file
  Prototype of the DxeTbtPolicyLib library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_TBT_POLICY_LIB_H_
#define _DXE_TBT_POLICY_LIB_H_


/**
  Install TBT Policy.

  @param[in] ImageHandle                Image handle of this driver.

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallTbtPolicy (
  IN  EFI_HANDLE                    ImageHandle
  );

/**
  Update Tbt Policy Callback.

  @param[in] Event         A pointer to the Event that triggered the callback.
  @param[in] Context       A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateTbtPolicyCallback (
  VOID
  );

/**
  Print DXE TBT Policy
**/
VOID
TbtPrintDxePolicyConfig (
  VOID
  );
#endif // _DXE_TBT_POLICY_LIB_H_


//========================================================================================
//   edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/GpioConfig.h
//   edk2-platforms/Silicon/Intel/PurleyRefreshSiliconPkg/Pch/Include/GpioConfig.h
//   edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/GpioConfig.h
// = edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Include/Library/GpioConfig.h

/** @file
  Header file for GpioConfig structure used by GPIO library.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_CONFIG_H_
#define _GPIO_CONFIG_H_

#pragma pack(push, 1)

///
/// For any GpioPad usage in code use GPIO_PAD type
///
typedef UINT32 GPIO_PAD;

///
/// GpioPad with additional native function information.
/// This type is used to represent signal muxing alternatives. Platform will provide such value to
/// identify muxing selection for given signal on a specific SOC.
/// Please refer to the board layout
///
typedef UINT32 GPIO_NATIVE_PAD;


///
/// For any GpioGroup usage in code use GPIO_GROUP type
///
typedef UINT32 GPIO_GROUP;

/**
  GPIO configuration structure used for pin programming.
  Structure contains fields that can be used to configure pad.
**/
typedef struct {
  /**
  Pad Mode
  Pad can be set as GPIO or one of its native functions.
  When in native mode setting Direction (except Inversion), OutputState,
  InterruptConfig, Host Software Pad Ownership and OutputStateLock are unnecessary.
  Refer to definition of GPIO_PAD_MODE.
  Refer to EDS for each native mode according to the pad.
  **/
  UINT32 PadMode            : 5;
  /**
  Host Software Pad Ownership
  Set pad to ACPI mode or GPIO Driver Mode.
  Refer to definition of GPIO_HOSTSW_OWN.
  **/
  UINT32 HostSoftPadOwn     : 2;
  /**
  GPIO Direction
  Can choose between In, In with inversion, Out, both In and Out, both In with inversion and out or disabling both.
  Refer to definition of GPIO_DIRECTION for supported settings.
  **/
  UINT32 Direction           : 6;
  /**
  Output State
  Set Pad output value.
  Refer to definition of GPIO_OUTPUT_STATE for supported settings.
  This setting takes place when output is enabled.
  **/
  UINT32 OutputState         : 2;
  /**
  GPIO Interrupt Configuration
  Set Pad to cause one of interrupts (IOxAPIC/SCI/SMI/NMI).
  This setting is applicable only if GPIO is in GpioMode with input enabled.
  Refer to definition of GPIO_INT_CONFIG for supported settings.
  **/
  UINT32 InterruptConfig     : 9;
  /**
  GPIO Power Configuration.
  This setting controls Pad Reset Configuration.
  Refer to definition of GPIO_RESET_CONFIG for supported settings.
  **/
  UINT32 PowerConfig        : 8;
  /**
  GPIO Electrical Configuration
  This setting controls pads termination.
  Refer to definition of GPIO_ELECTRICAL_CONFIG for supported settings.
  **/
  UINT32 ElectricalConfig   : 9;
  /**
  GPIO Lock Configuration
  This setting controls pads lock.
  Refer to definition of GPIO_LOCK_CONFIG for supported settings.
  **/
  UINT32 LockConfig         : 4;
  /**
  Additional GPIO configuration
  Refer to definition of GPIO_OTHER_CONFIG for supported settings.
  **/
  UINT32 OtherSettings      : 9;

  UINT32 RsvdBits           : 10;    ///< Reserved bits for future extension
} GPIO_CONFIG;


typedef enum {
  GpioHardwareDefault        = 0x0    ///< Leave setting unmodified
} GPIO_HARDWARE_DEFAULT;

/**
  GPIO Pad Mode
  Refer to GPIO documentation on native functions available for certain pad.
  If GPIO is set to one of NativeX modes then following settings are not applicable
  and can be skipped:
  - Interrupt related settings
  - Host Software Ownership
  - Output/Input enabling/disabling
  - Output lock
**/
typedef enum {
  GpioPadModeHwDefault  = 0x0,
  GpioPadModeGpio       = 0x1,
  GpioPadModeNative1    = 0x3,
  GpioPadModeNative2    = 0x5,
  GpioPadModeNative3    = 0x7,
  GpioPadModeNative4    = 0x9,
  GpioPadModeNative5    = 0xB,
  GpioPadModeNative6    = 0xD,
  GpioPadModeNative7    = 0xF
} GPIO_PAD_MODE;

/**
  Host Software Pad Ownership modes
  This setting affects GPIO interrupt status registers. Depending on chosen ownership
  some GPIO Interrupt status register get updated and other masked.
  Please refer to EDS for HOSTSW_OWN register description.
**/
typedef enum {
  GpioHostOwnDefault = 0x0,   ///< Leave ownership value unmodified
  /**
  Set HOST ownership to ACPI.
  Use this setting if pad is not going to be used by GPIO OS driver.
  If GPIO is configured to generate SCI/SMI/NMI then this setting must be
  used for interrupts to work
  **/
  GpioHostOwnAcpi    = 0x1,
  /**
  Set HOST ownership to GPIO Driver mode.
  Use this setting only if GPIO pad should be controlled by GPIO OS Driver.
  GPIO OS Driver will be able to control the pad if appropriate entry in
  ACPI exists (refer to ACPI specification for GpioIo and GpioInt descriptors)
  **/
  GpioHostOwnGpio    = 0x3
} GPIO_HOSTSW_OWN;

///
/// GPIO Direction
///
typedef enum {
  GpioDirDefault         = 0x0,                ///< Leave pad direction setting unmodified
  GpioDirInOut           = (0x1 | (0x1 << 3)), ///< Set pad for both output and input
  GpioDirInInvOut        = (0x1 | (0x3 << 3)), ///< Set pad for both output and input with inversion
  GpioDirIn              = (0x3 | (0x1 << 3)), ///< Set pad for input only
  GpioDirInInv           = (0x3 | (0x3 << 3)), ///< Set pad for input with inversion
  GpioDirOut             = 0x5,                ///< Set pad for output only
  GpioDirNone            = 0x7                 ///< Disable both output and input
} GPIO_DIRECTION;

/**
  GPIO Output State
  This field is relevant only if output is enabled
**/
typedef enum {
  GpioOutDefault         = 0x0,  ///< Leave output value unmodified
  GpioOutLow             = 0x1,  ///< Set output to low
  GpioOutHigh            = 0x3   ///< Set output to high
} GPIO_OUTPUT_STATE;

/**
  GPIO interrupt configuration
  This setting is applicable only if pad is in GPIO mode and has input enabled.
  GPIO_INT_CONFIG allows to choose which interrupt is generated (IOxAPIC/SCI/SMI/NMI)
  and how it is triggered (edge or level). Refer to PADCFG_DW0 register description in
  EDS for details on this settings.
  Field from GpioIntNmi to GpioIntApic can be OR'ed with GpioIntLevel to GpioIntBothEdge
  to describe an interrupt e.g. GpioIntApic | GpioIntLevel
  If GPIO is set to cause an SCI then also GPI_GPE_EN is enabled for this pad.
  If GPIO is set to cause an NMI then also GPI_NMI_EN is enabled for this pad.
  Not all GPIO are capable of generating an SMI or NMI interrupt.
  When routing GPIO to cause an IOxAPIC interrupt care must be taken, as this
  interrupt cannot be shared and its IRQn number is not configurable.
  Refer to EDS for GPIO pads IRQ numbers (PADCFG_DW1.IntSel)
  If GPIO is under GPIO OS driver control and appropriate ACPI GpioInt descriptor
  exist then use only trigger type setting (from GpioIntLevel to GpioIntBothEdge).
  This type of GPIO Driver interrupt doesn't have any additional routing setting
  required to be set by BIOS. Interrupt is handled by GPIO OS Driver.
**/

typedef enum {
  GpioIntDefault           = 0x0,  ///< Leave value of interrupt routing unmodified
  GpioIntDis               = 0x1,  ///< Disable IOxAPIC/SCI/SMI/NMI interrupt generation
  GpioIntNmi               = 0x3,  ///< Enable NMI interrupt only
  GpioIntSmi               = 0x5,  ///< Enable SMI interrupt only
  GpioIntSci               = 0x9,  ///< Enable SCI interrupt only
  GpioIntApic              = 0x11, ///< Enable IOxAPIC interrupt only
  GpioIntLevel       = (0x1 << 5), ///< Set interrupt as level triggered
  GpioIntEdge        = (0x3 << 5), ///< Set interrupt as edge triggered (type of edge depends on input inversion)
  GpioIntLvlEdgDis   = (0x5 << 5), ///< Disable interrupt trigger
  GpioIntBothEdge    = (0x7 << 5)  ///< Set interrupt as both edge triggered
} GPIO_INT_CONFIG;

#define B_GPIO_INT_CONFIG_INT_SOURCE_MASK  0x1F ///< Mask for GPIO_INT_CONFIG for interrupt source
#define B_GPIO_INT_CONFIG_INT_TYPE_MASK    0xE0 ///< Mask for GPIO_INT_CONFIG for interrupt type

/**
  GPIO Power Configuration
  GPIO_RESET_CONFIG allows to set GPIO Reset type (PADCFG_DW0.PadRstCfg) which will
  be used to reset certain GPIO settings.
  Refer to EDS for settings that are controllable by PadRstCfg.
**/
typedef enum {
  GpioResetDefault   = 0x00,        ///< Leave value of pad reset unmodified
  /**
  Resume Reset (RSMRST)
    GPP: PadRstCfg = 00b = "Powergood"
    GPD: PadRstCfg = 11b = "Resume Reset"
  Pad setting will reset on:
  - DeepSx transition
  - G3
  Pad settings will not reset on:
  - S3/S4/S5 transition
  - Warm/Cold/Global reset
  **/
  GpioResumeReset      = 0x01,
  /**
  Host Deep Reset
    PadRstCfg = 01b = "Deep GPIO Reset"
  Pad settings will reset on:
  - Warm/Cold/Global reset
  - DeepSx transition
  - G3
  Pad settings will not reset on:
  - S3/S4/S5 transition
  **/
  GpioHostDeepReset    = 0x03,
  /**
  Platform Reset (PLTRST)
    PadRstCfg = 10b = "GPIO Reset"
  Pad settings will reset on:
  - S3/S4/S5 transition
  - Warm/Cold/Global reset
  - DeepSx transition
  - G3
  **/
  GpioPlatformReset    = 0x05,
  /**
  Deep Sleep Well Reset (DSW_PWROK)
    GPP: not applicable
    GPD: PadRstCfg = 00b = "Powergood"
  Pad settings will reset on:
  - G3
  Pad settings will not reset on:
  - S3/S4/S5 transition
  - Warm/Cold/Global reset
  - DeepSx transition
  **/
  GpioDswReset         = 0x07
} GPIO_RESET_CONFIG;

/**
  GPIO Electrical Configuration
  Configuration options for GPIO termination setting
**/
typedef enum {
  GpioTermDefault          = 0x0,  ///< Leave termination setting unmodified
  GpioTermNone             = 0x1,  ///< none
  GpioTermWpd5K            = 0x5,  ///< 5kOhm weak pull-down
  GpioTermWpd20K           = 0x9,  ///< 20kOhm weak pull-down
  GpioTermWpu1K            = 0x13, ///< 1kOhm weak pull-up
  GpioTermWpu2K            = 0x17, ///< 2kOhm weak pull-up
  GpioTermWpu5K            = 0x15, ///< 5kOhm weak pull-up
  GpioTermWpu20K           = 0x19, ///< 20kOhm weak pull-up
  GpioTermWpu1K2K          = 0x1B, ///< 1kOhm & 2kOhm weak pull-up
  /**
  Native function controls pads termination
  This setting is applicable only to some native modes.
  Please check EDS to determine which native functionality
  can control pads termination
  **/
  GpioTermNative           = 0x1F
} GPIO_ELECTRICAL_CONFIG;

#define B_GPIO_ELECTRICAL_CONFIG_TERMINATION_MASK    0x1F   ///< Mask for GPIO_ELECTRICAL_CONFIG for termination value

/**
  GPIO LockConfiguration
  Set GPIO configuration lock and output state lock.
  GpioPadConfigUnlock/Lock and GpioOutputStateUnlock can be OR'ed.
  By default GPIO pads will be locked unless GPIO lib is explicitly
  informed that certain pad is to be left unlocked.
  Lock settings reset is in Powergood domain. Care must be taken when using this setting
  as fields it locks may be reset by a different signal and can be controlled
  by what is in GPIO_RESET_CONFIG (PADCFG_DW0.PadRstCfg). GPIO library provides
  functions which allow to unlock a GPIO pad. If possible each GPIO lib function will try to unlock
  an already locked pad upon request for reconfiguration
**/
typedef enum {
  /**
  Perform default action
   - if pad is an GPO, lock configuration but leave output unlocked
   - if pad is an GPI, lock everything
   - if pad is in native, lock everything
**/
  GpioLockDefault         = 0x0,
  GpioPadConfigUnlock     = 0x3,  ///< Leave Pad configuration unlocked
  GpioPadConfigLock       = 0x1,  ///< Lock Pad configuration
  GpioOutputStateUnlock   = 0xC,  ///< Leave Pad output control unlocked
  GpioPadUnlock           = 0xF,  ///< Leave both Pad configuration and output control unlocked
  GpioPadLock             = 0x5,  ///< Lock both Pad configuration and output control
  /**
    Below statuses are used for
    return from GpioGetPadConfig function
  **/
  GpioLockTxLockCfgUnLock = 0x7,  ///< Tx State locked, Pad Configuration unlocked
  GpioLockTxUnLockCfgLock = 0xD   ///< Tx State unlocked, Pad Configuration locked
} GPIO_LOCK_CONFIG;

#define B_GPIO_LOCK_CONFIG_PAD_CONF_LOCK_MASK  0x3  ///< Mask for GPIO_LOCK_CONFIG for Pad Configuration Lock
#define B_GPIO_LOCK_CONFIG_OUTPUT_LOCK_MASK    0xC  ///< Mask for GPIO_LOCK_CONFIG for Pad Output Lock

/**
  Other GPIO Configuration
  GPIO_OTHER_CONFIG is used for less often settings and for future extensions
  Supported settings:
   - RX raw override to '1' - allows to override input value to '1'
      This setting is applicable only if in input mode (both in GPIO and native usage).
      The override takes place at the internal pad state directly from buffer and before the RXINV.
**/
typedef enum {
  GpioRxRaw1Default           = 0x0,  ///< Use default input override value
  GpioRxRaw1Dis               = 0x1,  ///< Don't override input
  GpioRxRaw1En                = 0x3   ///< Override input to '1'
} GPIO_OTHER_CONFIG;

#define B_GPIO_OTHER_CONFIG_RXRAW_MASK           0x3   ///< Mask for GPIO_OTHER_CONFIG for RxRaw1 setting

#pragma pack(pop)

#endif //_GPIO_CONFIG_H_


//========================================================================================
//   edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/Library/GpioLib.h
//   edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/Library/GpioLib.h
//   edk2-platforms/Silicon/Intel/PurleyRefreshSiliconPkg/Pch/Include/Library/GpioLib.h
// = edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Include/Library/GpioLib.h

/** @file
  Header file for GpioLib.
  All function in this library is available for PEI, DXE, and SMM

  @note: When GPIO pads are owned by ME Firmware, BIOS/host should not
         attempt to access these GPIO Pads registers, registers value
         returned in this case will be 0xFF.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GPIO_LIB_H_
#define _GPIO_LIB_H_

//#include <Library/GpioConfig.h>

#define GPIO_NAME_LENGTH_MAX  32

typedef struct {
  GPIO_PAD           GpioPad;
  GPIO_CONFIG        GpioConfig;
} GPIO_INIT_CONFIG;

/**
  This procedure will initialize multiple GPIO pins. Use GPIO_INIT_CONFIG structure.
  Structure contains fields that can be used to configure each pad.
  Pad not configured using GPIO_INIT_CONFIG will be left with hardware default values.
  Separate fields could be set to hardware default if it does not matter, except
  GpioPad and PadMode.
  Function will work in most efficient way if pads which belong to the same group are
  placed in adjacent records of the table.
  Although function can enable pads for Native mode, such programming is done
  by reference code when enabling related silicon feature.

  @param[in] NumberofItem               Number of GPIO pads to be updated
  @param[in] GpioInitTableAddress       GPIO initialization table

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid group or pad number
**/
EFI_STATUS
GpioConfigurePads (
  IN UINT32                    NumberOfItems,
  IN GPIO_INIT_CONFIG          *GpioInitTableAddress
  );

//
// Functions for setting/getting multiple GpioPad settings
//

/**
  This procedure will read multiple GPIO settings

  @param[in]  GpioPad                   GPIO Pad
  @param[out] GpioData                  GPIO data structure

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid GpioPad
**/
EFI_STATUS
GpioGetPadConfig (
  IN  GPIO_PAD               GpioPad,
  OUT GPIO_CONFIG            *GpioData
  );

/**
  This procedure will configure multiple GPIO settings

  @param[in] GpioPad                    GPIO Pad
  @param[in] GpioData                   GPIO data structure

  @retval EFI_SUCCESS                   The function completed successfully
  @retval EFI_INVALID_PARAMETER         Invalid GpioPad
**/
EFI_STATUS
GpioSetPadConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_CONFIG               *GpioData
  );

//
// Functions for setting/getting single GpioPad properties
//

/**
  This procedure will set GPIO output level

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Output value
                                  0: OutputLow, 1: OutputHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetOutputValue (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    Value
  );

/**
  This procedure will get GPIO output level

  @param[in]  GpioPad             GPIO pad
  @param[out] OutputVal           GPIO Output value
                                  0: OutputLow, 1: OutputHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetOutputValue (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *OutputVal
  );

/**
  This procedure will get GPIO input level

  @param[in]  GpioPad             GPIO pad
  @param[out] InputVal            GPIO Input value
                                  0: InputLow, 1: InputHigh

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetInputValue (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *InputVal
  );

/**
  This procedure will get GPIO IOxAPIC interrupt number

  @param[in]  GpioPad             GPIO pad
  @param[out] IrqNum              IRQ number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadIoApicIrqNumber (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *IrqNum
  );

/**
  This procedure will configure GPIO input inversion

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value for GPIO input inversion
                                  0: No input inversion, 1: Invert input

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetInputInversion (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    Value
  );

/**
  This procedure will get GPIO pad input inversion value

  @param[in] GpioPad              GPIO pad
  @param[out] InvertState         GPIO inversion state
                                  0: No input inversion, 1: Inverted input

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetInputInversion (
  IN GPIO_PAD                  GpioPad,
  OUT UINT32                   *InvertState
  );

/**
  This procedure will set GPIO interrupt settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value of Level/Edge
                                  use GPIO_INT_CONFIG as argument

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetPadInterruptConfig (
  IN GPIO_PAD                 GpioPad,
  IN GPIO_INT_CONFIG          Value
  );

/**
  This procedure will set GPIO electrical settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value of termination
                                  use GPIO_ELECTRICAL_CONFIG as argument

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetPadElectricalConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_ELECTRICAL_CONFIG    Value
  );

/**
  This procedure will set GPIO Reset settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value for Pad Reset Configuration
                                  use GPIO_RESET_CONFIG as argument

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetPadResetConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_RESET_CONFIG         Value
  );

/**
  This procedure will get GPIO Reset settings

  @param[in] GpioPad              GPIO pad
  @param[in] Value                Value of Pad Reset Configuration
                                  based on GPIO_RESET_CONFIG

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadResetConfig (
  IN GPIO_PAD                  GpioPad,
  IN GPIO_RESET_CONFIG         *Value
  );

/**
  This procedure will get Gpio Pad Host Software Ownership

  @param[in] GpioPad              GPIO pad
  @param[out] PadHostSwOwn        Value of Host Software Pad Owner
                                  0: ACPI Mode, 1: GPIO Driver mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetHostSwOwnershipForPad (
  IN GPIO_PAD                 GpioPad,
  OUT UINT32                  *PadHostSwOwn
  );

/**
  This procedure will set Gpio Pad Host Software Ownership

  @param[in] GpioPad              GPIO pad
  @param[in]  PadHostSwOwn        Pad Host Software Owner
                                  0: ACPI Mode, 1: GPIO Driver mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioSetHostSwOwnershipForPad (
  IN GPIO_PAD                  GpioPad,
  IN UINT32                    PadHostSwOwn
  );

///
/// Possible values of Pad Ownership
/// If Pad is not under Host ownership then GPIO registers
/// are not accessible by host (e.g. BIOS) and reading them
/// will return 0xFFs.
///
typedef enum {
  GpioPadOwnHost = 0x0,
  GpioPadOwnCsme = 0x1,
  GpioPadOwnIsh  = 0x2,
} GPIO_PAD_OWN;

/**
  This procedure will get Gpio Pad Ownership

  @param[in] GpioPad              GPIO pad
  @param[out] PadOwnVal           Value of Pad Ownership

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadOwnership (
  IN  GPIO_PAD                GpioPad,
  OUT GPIO_PAD_OWN            *PadOwnVal
  );

/**
  This procedure will check state of Pad Config Lock for pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] PadCfgLockRegVal    Value of PadCfgLock register
                                  Bit position - PadNumber
                                  Bit value - 0: NotLocked, 1: Locked

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioGetPadCfgLockForGroupDw (
  IN  GPIO_GROUP                  Group,
  IN  UINT32                      DwNum,
  OUT UINT32                      *PadCfgLockRegVal
  );

/**
  This procedure will check state of Pad Config Lock for selected pad

  @param[in]  GpioPad             GPIO pad
  @param[out] PadCfgLock          PadCfgLock for selected pad
                                  0: NotLocked, 1: Locked

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadCfgLock (
  IN GPIO_PAD                   GpioPad,
  OUT UINT32                    *PadCfgLock
  );

/**
  This procedure will check state of Pad Config Tx Lock for pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLockTx register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[out] PadCfgLockTxRegVal  Value of PadCfgLockTx register
                                  Bit position - PadNumber
                                  Bit value - 0: NotLockedTx, 1: LockedTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioGetPadCfgLockTxForGroupDw (
  IN  GPIO_GROUP                  Group,
  IN  UINT32                      DwNum,
  OUT UINT32                      *PadCfgLockTxRegVal
  );

/**
  This procedure will check state of Pad Config Tx Lock for selected pad

  @param[in]  GpioPad             GPIO pad
  @param[out] PadCfgLock          PadCfgLockTx for selected pad
                                  0: NotLockedTx, 1: LockedTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetPadCfgLockTx (
  IN GPIO_PAD                   GpioPad,
  OUT UINT32                    *PadCfgLockTx
  );

/**
  This procedure will clear PadCfgLock for selected pads within one group.
  Unlocking a pad will cause an SMI (if enabled)

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToUnlock        Bitmask for pads which are going to be unlocked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotUnlock, 1: Unlock

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioUnlockPadCfgForGroupDw (
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    PadsToUnlock
  );

/**
  This procedure will clear PadCfgLock for selected pad.
  Unlocking a pad will cause an SMI (if enabled)

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioUnlockPadCfg (
  IN GPIO_PAD                   GpioPad
  );

/**
  This procedure will set PadCfgLock for selected pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToLock          Bitmask for pads which are going to be locked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotLock, 1: Lock

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioLockPadCfgForGroupDw (
  IN GPIO_GROUP                   Group,
  IN UINT32                       DwNum,
  IN UINT32                       PadsToLock
  );

/**
  This procedure will set PadCfgLock for selected pad

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioLockPadCfg (
  IN GPIO_PAD                   GpioPad
  );

/**
  This procedure will clear PadCfgLockTx for selected pads within one group.
  Unlocking a pad will cause an SMI (if enabled)

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLockTx register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToUnlockTx      Bitmask for pads which are going to be unlocked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotUnLockTx, 1: LockTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioUnlockPadCfgTxForGroupDw (
  IN GPIO_GROUP                Group,
  IN UINT32                    DwNum,
  IN UINT32                    PadsToUnlockTx
  );

/**
  This procedure will clear PadCfgLockTx for selected pad.
  Unlocking a pad will cause an SMI (if enabled)

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioUnlockPadCfgTx (
  IN GPIO_PAD                   GpioPad
  );

/**
  This procedure will set PadCfgLockTx for selected pads within one group

  @param[in]  Group               GPIO group
  @param[in]  DwNum               PadCfgLock register number for current group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  PadsToLockTx        Bitmask for pads which are going to be locked,
                                  Bit position - PadNumber
                                  Bit value - 0: DoNotLockTx, 1: LockTx

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or DwNum parameter number
**/
EFI_STATUS
GpioLockPadCfgTxForGroupDw (
  IN GPIO_GROUP                   Group,
  IN UINT32                       DwNum,
  IN UINT32                       PadsToLockTx
  );

/**
  This procedure will set PadCfgLockTx for selected pad

  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioLockPadCfgTx (
  IN GPIO_PAD                   GpioPad
  );

/**
  This procedure will get Group to GPE mapping.
  It will assume that only first 32 pads can be mapped to GPE.
  To handle cases where groups have more than 32 pads and higher part of group
  can be mapped please refer to GpioGetGroupDwToGpeDwX()

  @param[out] GroupToGpeDw0       GPIO group to be mapped to GPE_DW0
  @param[out] GroupToGpeDw1       GPIO group to be mapped to GPE_DW1
  @param[out] GroupToGpeDw2       GPIO group to be mapped to GPE_DW2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetGroupToGpeDwX (
  IN GPIO_GROUP               *GroupToGpeDw0,
  IN GPIO_GROUP               *GroupToGpeDw1,
  IN GPIO_GROUP               *GroupToGpeDw2
  );

/**
  This procedure will get Group to GPE mapping. If group has more than 32 bits
  it is possible to map only single DW of pins (e.g. 0-31, 32-63) because
  ACPI GPE_DWx register is 32 bits large.

  @param[out]  GroupToGpeDw0       GPIO group mapped to GPE_DW0
  @param[out]  GroupDwForGpeDw0    DW of pins mapped to GPE_DW0
  @param[out]  GroupToGpeDw1       GPIO group mapped to GPE_DW1
  @param[out]  GroupDwForGpeDw1    DW of pins mapped to GPE_DW1
  @param[out]  GroupToGpeDw2       GPIO group mapped to GPE_DW2
  @param[out]  GroupDwForGpeDw2    DW of pins mapped to GPE_DW2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetGroupDwToGpeDwX (
  OUT GPIO_GROUP                *GroupToGpeDw0,
  OUT UINT32                    *GroupDwForGpeDw0,
  OUT GPIO_GROUP                *GroupToGpeDw1,
  OUT UINT32                    *GroupDwForGpeDw1,
  OUT GPIO_GROUP                *GroupToGpeDw2,
  OUT UINT32                    *GroupDwForGpeDw2
  );

/**
  This procedure will get GPE number for provided GpioPad.
  PCH allows to configure mapping between GPIO groups and related GPE (GpioSetGroupToGpeDwX())
  what results in the fact that certain Pad can cause different General Purpose Event. Only three
  GPIO groups can be mapped to cause unique GPE (1-tier), all others groups will be under one common
  event (GPE_111 for 2-tier).

  1-tier:
  Returned GpeNumber is in range <0,95>. GpioGetGpeNumber() can be used
  to determine what _LXX ACPI method would be called on event on selected GPIO pad

  2-tier:
  Returned GpeNumber is 0x6F (111). All GPIO pads which are not mapped to 1-tier GPE
  will be under one master GPE_111 which is linked to _L6F ACPI method. If it is needed to determine
  what Pad from 2-tier has caused the event, _L6F method should check GPI_GPE_STS and GPI_GPE_EN
  registers for all GPIO groups not mapped to 1-tier GPE.

  @param[in]  GpioPad             GPIO pad
  @param[out] GpeNumber           GPE number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetGpeNumber (
  IN GPIO_PAD                   GpioPad,
  OUT UINT32                    *GpeNumber
  );

/**
  This procedure is used to clear SMI STS for a specified Pad

  @param[in]  GpioPad             GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioClearGpiSmiSts (
  IN GPIO_PAD                   GpioPad
  );

/**
  This procedure is used by Smi Dispatcher and will clear
  all GPI SMI Status bits

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
GpioClearAllGpiSmiSts (
  VOID
  );

/**
  This procedure is used to disable all GPI SMI

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
GpioDisableAllGpiSmi (
  VOID
  );

/**
  This procedure is used to register GPI SMI dispatch function.

  @param[in]  GpioPad             GPIO pad
  @param[out] GpiNum              GPI number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetGpiSmiNum (
  IN GPIO_PAD          GpioPad,
  OUT UINTN            *GpiNum
  );

/**
  This procedure is used to check GPIO inputs belongs to 2 tier or 1 tier architecture

  @param[in]  GpioPad             GPIO pad

  @retval     Data                0 means 1-tier, 1 means 2-tier
**/
BOOLEAN
GpioCheckFor2Tier (
  IN GPIO_PAD                  GpioPad
  );

/**
  This procedure is used to clear GPE STS for a specified GpioPad

  @param[in]  GpioPad             GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioClearGpiGpeSts (
  IN GPIO_PAD                  GpioPad
  );

/**
  This procedure is used to read GPE STS for a specified Pad

  @param[in]  GpioPad             GPIO pad
  @param[out] GpeSts              Gpe status for given pad
                                  The GpeSts is true if the status register is set for given Pad number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetGpiGpeSts (
  IN GPIO_PAD                  GpioPad,
  OUT BOOLEAN                  *GpeSts
  );

/**
  This procedure is used to get SMI STS for a specified Pad

  @param[in]  GpioPad             GPIO pad
  @param[out] SmiSts              Smi status for given pad
                                  The SmiSts is true if the status register is set for given Pad number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GpioGetGpiSmiSts (
  IN GPIO_PAD                   GpioPad,
  OUT BOOLEAN                   *SmiSts
  );

/**
  Generates GPIO name from GpioPad

  @param[in]  GpioPad             GpioPad
  @param[out] GpioNameBuffer      Caller allocated buffer for GPIO name of GPIO_NAME_LENGTH_MAX size
  @param[in]  GpioNameBufferSize  Size of the buffer

  @retval CHAR8*  Pointer to the GPIO name
**/
CHAR8*
GpioGetPadName (
  IN  GPIO_PAD  GpioPad,
  OUT CHAR8*    GpioNameBuffer,
  IN  UINT32    GpioNameBufferSize
  );

/**
  Generates GPIO group name from GroupIndex

  @param[in] GroupIndex  Gpio GroupIndex

  @retval CHAR8*  Pointer to the GPIO group name
**/
CONST
CHAR8*
GpioGetGroupName (
  IN UINT32  GroupIndex
  );

#endif // _GPIO_LIB_H_


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Include/Library/GpioExpanderLib.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Include/Library/GpioExpanderLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Include/Library/GpioExpanderLib.h

/** @file
  Support for IO expander TCA6424.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GPIO_EXPANDER_LIB_H_
#define _GPIO_EXPANDER_LIB_H_

//#include <Uefi.h>
//#include <Library/DebugLib.h>
//#include <Library/TimerLib.h>
//#include <Library/IoLib.h>
//#include <Library/UefiLib.h>
//#include <PchAccess.h>
//#include <Library/PchSerialIoLib.h>

/**
  Set the Direction value for the given Expander Gpio pin.

  This function is to Set the direction value for the GPIO
  Pin within the giving Expander.

  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value
  @param[in]  Value       none
**/
VOID
GpioExpSetDirection (
  IN UINT8 Expander,
  IN UINT8 Pin,
  IN UINT8 Direction
  );
/**
  Set the input value for the given Expander Gpio pin.

  This function is to get the input value for the GPIO
  Pin within the giving Expander.

  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value
  @param[in]  Value       none

**/
VOID
GpioExpSetPolarity  (
  IN UINT8 Expander,
  IN UINT8 Pin,
  IN UINT8 Polarity
  );
/**
  Set the Output value for the given Expander Gpio pin.

  This function is to Set the Output value for the GPIO
  Pin within the giving Expander.

  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value
  @param[in]  Value       none

**/
VOID
GpioExpSetOutput    (
  IN UINT8 Expander,
  IN UINT8 Pin,
  IN UINT8 Value
  );
/**
  Returns the data from register value giving in the input.

  This function is to get the data from the Expander
  Registers by following the I2C Protocol communication


  @param[in]  Bar0       Bar address of the SerialIo Controller
  @param[in]  Address    Expander Value with in the Contoller
  @param[in]  Register   Address of Input/Output/Configure/Polarity
                         registers with in the Expander

  @retval     UINT8      Value returned from the register
**/
UINT8
GpioExpGetInput     (
  IN UINT8 Expander,
  IN UINT8 Pin
  );

/**
  Configures all registers of a single IO Expander in one go.

  @param[in]  Expander    Expander number (0/1)
  @param[in]  Direction   Bit-encoded direction values. BIT0 is for pin0, etc. 0=output, 1=input
  @param[in]  Polarity    Bit-encoded input inversion values. BIT0 is for pin0, etc. 0=normal, 1=inversion
  @param[in]  Output      Bit-encoded output state, ignores polarity, only applicable if direction=INPUT. BIT0 is for pin0, etc. 0=low, 1=high

**/
VOID
GpioExpBulkConfig (
  IN UINT8  Expander,
  IN UINT32 Direction,
  IN UINT32 Polarity,
  IN UINT32 Output
  );

/**
  Returns the Controller on which GPIO expander is present.

  This function returns the Controller value

  @param[out] Controller              Pointer to a Controller value on
                                      which I2C expander is configured.

  @retval     EFI_SUCCESS              non.
**/
EFI_STATUS
GpioExpGetController (
  OUT UINT8 *Controller
  );

#endif


//========================================================================================
// =  edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/TbtPolicyCommonDefinition.h
//    edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/TbtPolicyCommonDefinition.h
// =  edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/TbtPolicyCommonDefinition.h

/** @file
TBT Policy Common definition.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_POLICY_COMMON_H_
#define _TBT_POLICY_COMMON_H_

//#include <Library/GpioLib.h>
//#include <IndustryStandard/Pci22.h>

#define MAX_DTBT_CONTROLLER_NUMBER 2

#define TYPE_PCIE           0x01
#define TYPE_PEG            0x02

#pragma pack(push, 1)

//
// dTBT Force Power GPIO Data Structure
//
typedef struct _DTBT_FORCE_POWER_GPIO_CONFIG {
  UINT8          GpioAccessType;     ///< Where the GPIO comes from [a.k.a how to access the GPIO],Where the GPIO comes from. 0: Disabled; 1: PCH, 2: I/O Expander
  UINT8          Expander;           ///< Applicable to GpioAccessType = IoExpander {TCA6424A} type
  GPIO_PAD       GpioPad;                 ///< GPIO Pad Number
  BOOLEAN        GpioLevel;               ///< 0 = Active Low; 1 = Active High
  UINT8          Reserved[1];             ///< Reserved for DWORD alignment
} DTBT_FORCE_POWER_GPIO_CONFIG;

//
// dTBT CIO Plug Event GPIO Data Structure
//
typedef struct _DTBT_CIO_PLUG_EVENT_GPIO_CONFIG {
  GPIO_PAD       GpioPad;                 ///< GPIO Pad Number
  UINT32         AcpiGpeSignature;        ///< AcpiPlatform driver will change the XTBT method to the _Lxx or _Exx that we assign in this item.
  BOOLEAN        AcpiGpeSignaturePorting; ///< 0 = No porting required(for 2-tier GPI GPE event architecture), 1 = Porting required(for 1-tier GPI GPE event architecture)
  UINT8          Reserved[3];             ///< Reserved for DWORD alignment
} DTBT_CIO_PLUG_EVENT_GPIO_CONFIG;

//
// dTBT PCIE Reset GPIO Data Structure
//
typedef struct _DTBT_PCIE_RESET_GPIO_CONFIG {
  GPIO_PAD       GpioPad;                 ///< GPIO Pad Number
  BOOLEAN        GpioLevel;               ///< 0 = Active Low; 1 = Active High
  UINT8          Reserved[3];             ///< Reserved for DWORD alignment
} DTBT_PCIE_RESET_GPIO_CONFIG;

//
// dTBT Controller Data Structure
//
typedef struct _DTBT_CONTROLLER_CONFIG{
  UINT8                           DTbtControllerEn; ///< Enable/Disable DTbtController.
  UINT8                           Type;             ///< 01-Pcie RP, 02- PEG,Reserved. <Specific according to Board Design>
  UINT8                           PcieRpNumber;     ///< RP Number/ PEG Port (0,1,2) that connecet to dTBT controller. <Specific according to Board Design>
  DTBT_FORCE_POWER_GPIO_CONFIG    ForcePwrGpio;     ///< The GPIO pin that can force dTBT Power On. <Specific according to Board Design>
  DTBT_CIO_PLUG_EVENT_GPIO_CONFIG CioPlugEventGpio; ///< The GPIO pin that can generate Hot-Plug event. <Specific according to Board Design>
  DTBT_PCIE_RESET_GPIO_CONFIG     PcieRstGpio;      ///< The GPIO pin that is use to perform Reset when platform enters to Sx, it is required for platforms where PCI_RST pin connected to Tbt is controlled with GPIO <Specific according to Board Design>
  GPIO_PAD                        PdResetGpioPad;   ///< PD HRESET GPIO Pad Number
  GPIO_PAD                        PdSxEntryGpioPad; ///< PD SX Entry GPIO Pad Number
  GPIO_PAD                        PdSxAckGpioPad;   ///< PD SX Ack GPIO Pad Number
  UINT8                           Reserved[1];      ///< Reserved for DWORD alignment
} DTBT_CONTROLLER_CONFIG;

//
// dTBT Controller Data Structure
//
typedef struct _DTBT_COMMON_CONFIG{
  UINT8            TbtBootOn;                    ///< Send BootOn Mailbox command when TbtBootOn is enabled.
  UINT8            TbtUsbOn;                     ///< Send UsbOn Mailbox command when TbtBootOn is enabled.
  UINT8            Gpio3ForcePwr;                ///< Force GPIO to power on or not
  UINT16           Gpio3ForcePwrDly;             ///< The delay time after do ForcePwr
  BOOLEAN          DTbtSharedGpioConfiguration;  ///< Multiple DTBT controllers share the same GPIO pin <Specific according to Board Design>
  BOOLEAN          PcieRstSupport;               ///< 0 = Not Support, 1 = Supported. it is required for platforms where PCI_RST pin connected to Tbt is controlled with GPIO
  UINT8            SecurityMode;                 ///< 0: SL0 No Security, 1: SL1 User Authorization, 2: SL2 Secure Connect, 3: SL3 Display Port and USB
  UINT8            ControlIommu;                 ///< Control Iommu behavior in pre-boot, 0: Disabled Iommu, 1: Enable Iommu, Disable exception list, 2: Enable Iommu, Enable exception list
  UINT8            Reserved[3];                  ///< Reserved for DWORD alignment
} DTBT_COMMON_CONFIG;

#pragma pack(pop)

#endif


//========================================================================================
//   edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/SystemAgent/Include/MemInfoHob.h
//   edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/SystemAgent/Include/MemInfoHob.h
// = edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/SystemAgent/Include/MemInfoHob.h

/** @file
  This file contains definitions required for creation of
  Memory S3 Save data, Memory Info data and Memory Platform
  data hobs.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _MEM_INFO_HOB_H_
#define _MEM_INFO_HOB_H_

#pragma pack (push, 1)

extern EFI_GUID gSiMemoryS3DataGuid;
extern EFI_GUID gSiMemoryInfoDataGuid;
extern EFI_GUID gSiMemoryPlatformDataGuid;

#define MAX_TRACE_CACHE_TYPE  3

#define MAX_NODE        2
#define MAX_CH          4
#define MAX_DIMM        2

///
/// Host reset states from MRC.
///
#define  WARM_BOOT        2

#define R_MC_CHNL_RANK_PRESENT  0x7C
#define   B_RANK0_PRS           BIT0
#define   B_RANK1_PRS           BIT1
#define   B_RANK2_PRS           BIT4
#define   B_RANK3_PRS           BIT5

///
/// Defines taken from MRC so avoid having to include MrcInterface.h
///

//
// Matches MAX_SPD_SAVE define in MRC
//
#ifndef MAX_SPD_SAVE
#define MAX_SPD_SAVE 29
#endif

//
// MRC version description.
//
typedef struct {
  UINT8  Major;     ///< Major version number
  UINT8  Minor;     ///< Minor version number
  UINT8  Rev;       ///< Revision number
  UINT8  Build;     ///< Build number
} SiMrcVersion;

//
// Matches MrcDimmSts enum in MRC
//
#ifndef DIMM_ENABLED
#define DIMM_ENABLED     0  // DIMM/rank Pair is enabled, presence will be detected.
#endif
#ifndef DIMM_DISABLED
#define DIMM_DISABLED    1  // DIMM/rank Pair is disabled, regardless of presence.
#endif
#ifndef DIMM_PRESENT
#define DIMM_PRESENT     2  // There is a DIMM present in the slot/rank pair and it will be used.
#endif
#ifndef DIMM_NOT_PRESENT
#define DIMM_NOT_PRESENT 3  // There is no DIMM present in the slot/rank pair.
#endif

//
// Matches MrcBootMode enum in MRC
//
#ifndef __MRC_BOOT_MODE__
#define __MRC_BOOT_MODE__                 //The below values are originated from MrcCommonTypes.h
  #ifndef INT32_MAX
  #define INT32_MAX                       (0x7FFFFFFF)
  #endif  //INT32_MAX
typedef enum {
  bmCold,                                 ///< Cold boot
  bmWarm,                                 ///< Warm boot
  bmS3,                                   ///< S3 resume
  bmFast,                                 ///< Fast boot
  MrcBootModeMax,                         ///< MRC_BOOT_MODE enumeration maximum value.
  MrcBootModeDelim = INT32_MAX            ///< This value ensures the enum size is consistent on both sides of the PPI.
} MRC_BOOT_MODE;
#endif  //__MRC_BOOT_MODE__

//
// Matches MrcDdrType enum in MRC
//
#ifndef MRC_DDR_TYPE_DDR4
#define MRC_DDR_TYPE_DDR4     0
#endif
#ifndef MRC_DDR_TYPE_DDR3
#define MRC_DDR_TYPE_DDR3     1
#endif
#ifndef MRC_DDR_TYPE_LPDDR3
#define MRC_DDR_TYPE_LPDDR3   2
#endif
#ifndef MRC_DDR_TYPE_LPDDR4
#define MRC_DDR_TYPE_LPDDR4   3
#endif
#ifndef MRC_DDR_TYPE_WIO2
#define MRC_DDR_TYPE_WIO2     4
#endif
#ifndef MRC_DDR_TYPE_UNKNOWN
#define MRC_DDR_TYPE_UNKNOWN  5
#endif

#define MAX_PROFILE_NUM     4 // number of memory profiles supported
#define MAX_XMP_PROFILE_NUM 2 // number of XMP profiles supported

//
// DIMM timings
//
typedef struct {
  UINT32 tCK;       ///< Memory cycle time, in femtoseconds.
  UINT16 NMode;     ///< Number of tCK cycles for the channel DIMM's command rate mode.
  UINT16 tCL;       ///< Number of tCK cycles for the channel DIMM's CAS latency.
  UINT16 tCWL;      ///< Number of tCK cycles for the channel DIMM's minimum CAS write latency time.
  UINT16 tFAW;      ///< Number of tCK cycles for the channel DIMM's minimum four activate window delay time.
  UINT16 tRAS;      ///< Number of tCK cycles for the channel DIMM's minimum active to precharge delay time.
  UINT16 tRCDtRP;   ///< Number of tCK cycles for the channel DIMM's minimum RAS# to CAS# delay time and Row Precharge delay time.
  UINT16 tREFI;     ///< Number of tCK cycles for the channel DIMM's minimum Average Periodic Refresh Interval.
  UINT16 tRFC;      ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFCpb;    ///< Number of tCK cycles for the channel DIMM's minimum per bank refresh recovery delay time.
  UINT16 tRFC2;     ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFC4;     ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRPab;     ///< Number of tCK cycles for the channel DIMM's minimum row precharge delay time for all banks.
  UINT16 tRRD;      ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time.
  UINT16 tRRD_L;    ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for same bank groups.
  UINT16 tRRD_S;    ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for different bank groups.
  UINT16 tRTP;      ///< Number of tCK cycles for the channel DIMM's minimum internal read to precharge command delay time.
  UINT16 tWR;       ///< Number of tCK cycles for the channel DIMM's minimum write recovery time.
  UINT16 tWTR;      ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time.
  UINT16 tWTR_L;    ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for same bank groups.
  UINT16 tWTR_S;    ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for different bank groups.
  UINT16 tCCD_L;  ///< Number of tCK cycles for the channel DIMM's minimum CAS-to-CAS delay for same bank group.
} MRC_CH_TIMING;

///
/// Memory SMBIOS & OC Memory Data Hob
///
typedef struct {
  UINT8            Status;                  ///< See MrcDimmStatus for the definition of this field.
  UINT8            DimmId;
  UINT32           DimmCapacity;            ///< DIMM size in MBytes.
  UINT16           MfgId;
  UINT8            ModulePartNum[20];       ///< Module part number for DDR3 is 18 bytes however for DRR4 20 bytes as per JEDEC Spec, so reserving 20 bytes
  UINT8            RankInDimm;              ///< The number of ranks in this DIMM.
  UINT8            SpdDramDeviceType;       ///< Save SPD DramDeviceType information needed for SMBIOS structure creation.
  UINT8            SpdModuleType;           ///< Save SPD ModuleType information needed for SMBIOS structure creation.
  UINT8            SpdModuleMemoryBusWidth; ///< Save SPD ModuleMemoryBusWidth information needed for SMBIOS structure creation.
  UINT8            SpdSave[MAX_SPD_SAVE];   ///< Save SPD Manufacturing information needed for SMBIOS structure creation.
  UINT16           Speed;                   ///< The maximum capable speed of the device, in MHz
  UINT8            MdSocket;                ///< MdSocket: 0 = Memory Down, 1 = Socketed. Needed for SMBIOS structure creation.
} DIMM_INFO;

typedef struct {
  UINT8            Status;                  ///< Indicates whether this channel should be used.
  UINT8            ChannelId;
  UINT8            DimmCount;               ///< Number of valid DIMMs that exist in the channel.
  MRC_CH_TIMING    Timing[MAX_PROFILE_NUM]; ///< The channel timing values.
  DIMM_INFO        DimmInfo[MAX_DIMM];      ///< Save the DIMM output characteristics.
} CHANNEL_INFO;

typedef struct {
  UINT8            Status;                  ///< Indicates whether this controller should be used.
  UINT16           DeviceId;                ///< The PCI device id of this memory controller.
  UINT8            RevisionId;              ///< The PCI revision id of this memory controller.
  UINT8            ChannelCount;            ///< Number of valid channels that exist on the controller.
  CHANNEL_INFO     ChannelInfo[MAX_CH];     ///< The following are channel level definitions.
} CONTROLLER_INFO;

typedef struct {
  UINT64   BaseAddress;   ///< Trace Base Address
  UINT64   TotalSize;     ///< Total Trace Region of Same Cache type
  UINT8    CacheType;     ///< Trace Cache Type
  UINT8    ErrorCode;     ///< Trace Region Allocation Fail Error code
  UINT8    Rsvd[2];
} PSMI_MEM_INFO;

typedef struct {
  UINT8             Revision;
  UINT16            DataWidth;              ///< Data width, in bits, of this memory device
  /** As defined in SMBIOS 3.0 spec
    Section 7.18.2 and Table 75
  **/
  UINT8             MemoryType;             ///< DDR type: DDR3, DDR4, or LPDDR3
  UINT16            MaximumMemoryClockSpeed;///< The maximum capable speed of the device, in megahertz (MHz)
  UINT16            ConfiguredMemoryClockSpeed; ///< The configured clock speed to the memory device, in megahertz (MHz)
  /** As defined in SMBIOS 3.0 spec
    Section 7.17.3 and Table 72
  **/
  UINT8             ErrorCorrectionType;

  SiMrcVersion      Version;
  BOOLEAN           EccSupport;
  UINT8             MemoryProfile;
  UINT32            TotalPhysicalMemorySize;
  UINT32            DefaultXmptCK[MAX_XMP_PROFILE_NUM];///< Stores the tCK value read from SPD XMP profiles if they exist.
  UINT8             XmpProfileEnable;                  ///< If XMP capable DIMMs are detected, this will indicate which XMP Profiles are common among all DIMMs.
  UINT8             Ratio;
  UINT8             RefClk;
  UINT32            VddVoltage[MAX_PROFILE_NUM];
  CONTROLLER_INFO   Controller[MAX_NODE];
} MEMORY_INFO_DATA_HOB;

/**
  Memory Platform Data Hob

  <b>Revision 1:</b>
  - Initial version.
  <b>Revision 2:</b>
  - Added TsegBase, PrmrrSize, PrmrrBase, Gttbase, MmioSize, PciEBaseAddress fields
**/
typedef struct {
  UINT8             Revision;
  UINT8             Reserved[3];
  UINT32            BootMode;
  UINT32            TsegSize;
  UINT32            TsegBase;
  UINT32            PrmrrSize;
  UINT64            PrmrrBase;
  UINT32            PramSize;
  UINT64            PramBase;
  UINT64            DismLimit;
  UINT64            DismBase;
  UINT32            GttBase;
  UINT32            MmioSize;
  UINT32            PciEBaseAddress;
  PSMI_MEM_INFO     PsmiInfo[MAX_TRACE_CACHE_TYPE];
} MEMORY_PLATFORM_DATA;

typedef struct {
  EFI_HOB_GUID_TYPE    EfiHobGuidType;
  MEMORY_PLATFORM_DATA Data;
  UINT8                *Buffer;
} MEMORY_PLATFORM_DATA_HOB;

#pragma pack (pop)

#endif // _MEM_INFO_HOB_H_


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/TbtBoardInfo.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/TbtBoardInfo.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/TbtBoardInfo.h

/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_INFO_GUID_H_
#define _TBT_INFO_GUID_H_
//#include <TbtPolicyCommonDefinition.h>

#pragma pack(1)
//
// TBT Info HOB
//
typedef struct _TBT_INFO_HOB {
  EFI_HOB_GUID_TYPE      EfiHobGuidType;
  DTBT_COMMON_CONFIG     DTbtCommonConfig;                                  ///< dTbt Common Configuration
  DTBT_CONTROLLER_CONFIG DTbtControllerConfig [MAX_DTBT_CONTROLLER_NUMBER]; ///< dTbt Controller Configuration
} TBT_INFO_HOB;
#pragma pack()

#endif


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Protocol/DxeTbtPolicy.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Protocol/DxeTbtPolicy.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Protocol/DxeTbtPolicy.h

/** @file
TBT DXE Policy


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_TBT_POLICY_H_
#define _DXE_TBT_POLICY_H_

//#include <TbtPolicyCommonDefinition.h>

#pragma pack(push, 1)

#define DXE_TBT_POLICY_REVISION 1

//
// TBT Common Data Structure
//
typedef struct _TBT_COMMON_CONFIG{
  /**
    TBT Security Level
    <b>0: SL0 No Security</b>, 1: SL1 User Authorization, 2: SL2 Secure Connect, 3: SL3 Display Port and USB
  **/
  UINT32   SecurityMode      : 3;
  /**
    BIOS W/A for Hot plug of 12V USB devices cause electrical noise on PCH GPIOs
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   Gpio5Filter       : 1;
  /**
     WA for TR A0 OS_UP Command, it is only needed for TR A0 stepping
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TrA0OsupWa        : 1;
  /**
    Send Go2SxNoWake or GoSxWake according to TbtWakeupSupport
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TbtWakeupSupport  : 1;
  /**
    SMI TBT enumeration
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TbtHotSMI         : 1;
  /**
    Notify PCIe RP after Hot-Plug/Hot-Unplug occurred.
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TbtHotNotify      : 1;
  /**
    CLK REQ for all the PCIe device in TBT daisy chain.
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TbtSetClkReq      : 1;
  /**
    ASPM setting for all the PCIe device in TBT daisy chain.
    <b>0: Disabled</b>, 1: L0s, 2: L1, 3: L0sL1
  **/
  UINT32   TbtAspm           : 2;
  /**
    L1 SubState for for all the PCIe device in TBT daisy chain.
    <b>0: Disabled</b>, 1: L1.1, 2: L1.1 & L1.2
  **/
  UINT32   TbtL1SubStates    : 2;
  /**
    LTR for for all the PCIe device in TBT daisy chain.
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TbtLtr            : 1;
  /**
    PTM for for all the PCIe device in TBT daisy chain.
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TbtPtm            : 1;
  /**
    TBT Dynamic AC/DC L1.
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TbtAcDcSwitch     : 1;
  /**
    TBT RTD3 Support.
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   Rtd3Tbt           : 1;
  /**
    TBT ClkReq for RTD3 Flow.
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   Rtd3TbtClkReq     : 1;
  /**
    TBT Win10support for Tbt FW execution mode.
    <b>0: Disabled</b>, 1: Native, 2: Native + RTD3
  **/
  UINT32   Win10Support      : 2;
  /**
    TbtVtdBaseSecurity
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32   TbtVtdBaseSecurity: 1;
  /**
    Control Iommu behavior in pre-boot
    <b>0: Disabled Iommu</b>, 1: Enable Iommu, Disable exception list, 2: Enable Iommu, Enable exception list
  **/
  UINT32   ControlIommu      : 3;
  UINT32   Rsvd0             : 8; ///< Reserved bits
  UINT16   Rtd3TbtClkReqDelay;
  UINT16   Rtd3TbtOffDelay;
} TBT_COMMON_CONFIG;

//
// dTBT Resource Data Structure
//
typedef struct _DTBT_RESOURCE_CONFIG{
  UINT8  DTbtPcieExtraBusRsvd;     ///< Preserve Bus resource for PCIe RP that connect to dTBT Host Router
  UINT16 DTbtPcieMemRsvd;          ///< Preserve MEM resource for PCIe RP that connect to dTBT Host Router
  UINT8  DTbtPcieMemAddrRngMax;    ///< Alignment of Preserve MEM resource for PCIe RP that connect to dTBT Host Router
  UINT16 DTbtPciePMemRsvd;         ///< Preserve PMEM resource for PCIe RP that connect to dTBT Host Router
  UINT8  DTbtPciePMemAddrRngMax;   ///< Alignment of Preserve PMEM resource for PCIe RP that connect to dTBT Host Router
  UINT8  Reserved[1];      ///< Reserved for DWORD alignment
} DTBT_RESOURCE_CONFIG;

/**
 TBT DXE configuration\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct _DXE_TBT_POLICY_PROTOCOL {
  TBT_COMMON_CONFIG      TbtCommonConfig;                                  ///< Tbt Common Information
  DTBT_RESOURCE_CONFIG   DTbtResourceConfig[MAX_DTBT_CONTROLLER_NUMBER];   ///< dTbt Resource Configuration
} DXE_TBT_POLICY_PROTOCOL;

#pragma pack(pop)

#endif


//========================================================================================
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Library/DxeTbtSecurityLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Library/DxeTbtSecurityLib.h

/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_SECURITY_LIB_H_
#define _TBT_SECURITY_LIB_H_

//#include <Protocol/Tcg2Protocol.h>
//#include <Protocol/AcpiTable.h>
//#include <IndustryStandard/Pci.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/DebugLib.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>
//#include <Library/PcdLib.h>
//#include <Library/PciSegmentLib.h>
//#include <Library/AslUpdateLib.h>
//#include <Library/UefiLib.h>
//#include <Uefi.h>
//#include <SetupVariable.h>
//#include <OemSetup.h>
//#include <DmaRemappingTable.h>
//#include <PcieRegs.h>
//#include <Tcg2ConfigNvData.h>
//#include <TbtPolicyCommonDefinition.h>
//#include <Library/TbtCommonLib.h>

#define TBT_SECURITY_EVENT_STRING                 "DMA Protection Disabled"
#define TBT_SECURITY_EVENT_STRING_LEN             (sizeof (TBT_SECURITY_EVENT_STRING) - 1)

#define TBT_SECURITY_LEVEL_DOWNGRADED_STRING      "Security Level is Downgraded to 0"
#define TBT_SECURITY_LEVEL_DOWNGRADED_STRING_LEN  (sizeof (TBT_SECURITY_LEVEL_DOWNGRADED_STRING) - 1)

#define GET_TBT_SECURITY_MODE    0
#define SET_TBT_SECURITY_MODE    1

typedef struct {
  UINT8       EnableVtd;
  BOOLEAN     SLDowngrade;
} PCR7_DATA;

/**
  TBT Security ExtendPCR7 CallBackFunction
  If the firmware/BIOS has an option to enable and disable DMA protections via a VT-d switch in BIOS options, then the shipping configuration must be with VT-d protection enabled.
  On every boot where VT-d/DMA protection is disabled, or will be disabled, or configured to a lower security state, and a platform has a TPM enabled, then the platform SHALL extend an EV_EFI_ACTION event into PCR[7] before enabling external DMA.
  The event string SHALL be "DMA Protection Disabled". The platform firmware MUST log this measurement in the event log using the string "DMA Protection Disabled" for the Event Data.
  Measure and log launch of TBT Security, and extend the measurement result into a specific PCR.
  Extend an EV_EFI_ACTION event into PCR[7] before enabling external DMA. The event string SHALL be "DMA Protection Disabled". The platform firmware MUST log this measurement in the event log using the string "DMA Protection Disabled" for the Event Data.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
ExtendPCR7CallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  TBT Security DisableBme CallBackFunction

  BIOS will disable BME and tear down the Thunderbolt DMAR tables at ExitBootServices
  in order to hand off security of TBT hierarchies to the OS.
  The BIOS is expected to either: Disable BME from power on till the OS starts configuring the devices and enabling BME Enable BME only for devices that can be protected by VT-d in preboot environment,
  but disable BME and tear down any Thunderbolt DMAR tables at ExitBootServices()

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
TbtDisableBmeCallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  TBT Security SetDmarOptIn CallBackFunction

  A new security feature will be supported to protect against Physical DMA attacks over Thunderbolt connects.
  In order to do this, they need a new flag added to the DMAR tables that a DMA is only permitted into RMRR at ExitBootServices().  With this flag available, OS can then Bug Check if any DMA is requested outside of the RMRR before OS supported device drivers are started.
  ReadyToBoot callback routine to update DMAR BIT2
  Bit definition: DMA_CONTROL_GUARANTEE
  If Set, the platform supports blocking all DMA outside of the regions defined in the RMRR structures from ExitBootServices() until OS supported device drivers are started.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SetDmarOptInCallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


/**
  The function install DisableBme protocol for TBT Shell validation
**/
VOID
InstallDisableBmeProtocol (
  VOID
  );

/**
  Get or set Thunderbolt(TM) security mode

  @param[in]  DelayTime           - The delay time after do ForcePwr
  @param[in]  SecurityMode        - TBT Security Level
  @param[in]  Gpio3ForcePwrEn     - Force GPIO to power on or not
  @param[in]  DTbtController      - Enable/Disable DTbtController
  @param[in]  MaxControllerNumber - Number of contorller
  @param[in]  Action              - 0 = get, 1 = set

  @retval                         - Return security level
**/
UINT8
EFIAPI
GetSetSecurityMode (
  IN UINTN                       DelayTime,
  IN UINT8                       SecurityMode,
  IN UINT8                       Gpio3ForcePwrEn,
  IN UINT8                       *DTbtController,
  IN UINT8                       MaxControllerNumber,
  IN UINT8                       Action
);
#endif


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Library/TbtCommonLib.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Library/TbtCommonLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Library/TbtCommonLib.h

/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_COMMON_LIB_H_
#define _TBT_COMMON_LIB_H_

// #include <Library/BaseLib.h>
// #include <Library/PciSegmentLib.h>

#define DEFAULT_PCI_SEGMENT_NUMBER_ITBT_RP     0 // @todo : Update when once finalized
#define DEFAULT_PCI_BUS_NUMBER_ITBT_RP         0
#define DEFAULT_PCI_DEVICE_NUMBER_ITBT_RP      0x07

#define DEFAULT_PCI_SEGMENT_NUMBER_ITBT_DMA0   0
#define DEFAULT_PCI_BUS_NUMBER_ITBT_DMA0       0
#define DEFAULT_PCI_DEVICE_NUMBER_ITBT_DMA0    0x0D
#define DEFAULT_PCI_FUNCTION_NUMBER_ITBT_DMA0  0x02

#define DTBT_CONTROLLER                   0x00
#define DTBT_TYPE_PCH                     0x01
#define DTBT_TYPE_PEG                     0x02
#define ITBT_CONTROLLER                   0x80
#define TBT2PCIE_ITBT_R                   0xEC
#define PCIE2TBT_ITBT_R                   0xF0
#define TBT2PCIE_DTBT_R                   0x548
#define PCIE2TBT_DTBT_R                   0x54C

#define INVALID_RP_CONTROLLER_TYPE        0xFF

//
//  Thunderbolt FW OS capability
//
#define NO_OS_NATIVE_SUPPORT    0
#define OS_NATIVE_SUPPORT_ONLY  1
#define OS_NATIVE_SUPPORT_RTD3  2

#define ITBT_SAVE_STATE_OFFSET  BIT4 // Bits 4-7 is for ITBT (HIA0/1/2/Reserved)
#define DTBT_SAVE_STATE_OFFSET  BIT0 // Bits 0-3 is for DTBT (only bit 0 is in use)
/**
Get Tbt2Pcie Register Offset

@param[in]  Type      ITBT (0x80) or DTBT (0x00)
@retval     Register  Register Variable
**/

#define GET_TBT2PCIE_REGISTER_ADDRESS(Type, Segment, Bus, Device, Function, RegisterAddress) \
  if (Type == ITBT_CONTROLLER) { \
    RegisterAddress = PCI_SEGMENT_LIB_ADDRESS(Segment, Bus, Device, Function, TBT2PCIE_ITBT_R); \
  } else { \
    RegisterAddress = PCI_SEGMENT_LIB_ADDRESS(Segment, Bus, Device, Function, TBT2PCIE_DTBT_R); \
  }

/**
Get Pcie2Tbt Register Offset

@param[in]  Type      ITBT (0x80) or DTBT (0x00)
@retval     Register  Register Variable
**/

#define GET_PCIE2TBT_REGISTER_ADDRESS(Type, Segment, Bus, Device, Function, RegisterAddress) \
  if (Type == ITBT_CONTROLLER) { \
    RegisterAddress = PCI_SEGMENT_LIB_ADDRESS(Segment, Bus, Device, Function, PCIE2TBT_ITBT_R); \
  } else { \
    RegisterAddress = PCI_SEGMENT_LIB_ADDRESS(Segment, Bus, Device, Function, PCIE2TBT_DTBT_R); \
  }

#define PCIE2TBT_VLD_B                    BIT0
#define TBT2PCIE_DON_R                    BIT0
#define TBT_MAIL_BOX_DELAY                (100*1000)
#define TBT_5S_TIMEOUT                    50
#define TBT_1S_TIMEOUT                    10
#define TBT_3S_TIMEOUT                    30

#define PCIE2TBT_GO2SX                    (0x02 << 1)
#define PCIE2TBT_GO2SX_NO_WAKE            (0x03 << 1)
#define PCIE2TBT_SX_EXIT_TBT_CONNECTED    (0x04 << 1)
#define PCIE2TBT_SX_EXIT_NO_TBT_CONNECTED (0x05 << 1)
#define PCIE2TBT_OS_UP                    (0x06 << 1)
#define PCIE2TBT_SET_SECURITY_LEVEL       (0x08 << 1)
#define PCIE2TBT_GET_SECURITY_LEVEL       (0x09 << 1)
#define PCIE2TBT_CM_AUTH_MODE_ENTER       (0x10 << 1)
#define PCIE2TBT_CM_AUTH_MODE_EXIT        (0x11 << 1)
#define PCIE2TBT_BOOT_ON                  (0x18 << 1)
#define PCIE2TBT_BOOT_OFF                 (0x19 << 1)
#define PCIE2TBT_USB_ON                   (0x19 << 1)
#define PCIE2TBT_GET_ENUMERATION_METHOD   (0x1A << 1)
#define PCIE2TBT_SET_ENUMERATION_METHOD   (0x1B << 1)
#define PCIE2TBT_POWER_CYCLE              (0x1C << 1)
#define PCIE2TBT_PREBOOTACL               (0x1E << 1)
#define CONNECT_TOPOLOGY_COMMAND          (0x1F << 1)

#define RESET_HR_BIT                      BIT0
#define ENUMERATE_HR_BIT                  BIT1
#ifndef AUTO
#define AUTO                              0x0
#endif

//
//Thunder Bolt Device IDs
//

//
// Alpine Ridge HR device IDs
//
#define AR_HR_2C  0x1576
#define AR_HR_4C  0x1578
#define AR_XHC    0x15B5
#define AR_XHC_4C 0x15B6
#define AR_HR_LP  0x15C0
//
// Alpine Ridge C0 HR device IDs
//
#define AR_HR_C0_2C  0x15DA
#define AR_HR_C0_4C  0x15D3
//
// Titan Ridge HR device IDs
//
#define TR_HR_2C  0x15E7
#define TR_HR_4C  0x15EA
//
//End of Thunderbolt(TM) Device IDs
//

typedef struct _DEV_ID {
  UINT8 Segment;
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;
} DEV_ID;

//@todo Seems to only be used by Platform/TBT/Smm/TbtSmm.inf
//@todo should refactor this to only be present in that driver
//@todo also definitions like this should never be in a .h file anyway
//@todo this is a quick hack to get things compiling for now
#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wunused-variable"
#endif

/**
Based on the Security Mode Selection, BIOS drives FORCE_PWR.

@param[in]  GpioNumber
@param[in]  Value
**/
VOID
ForceDtbtPower(
  IN  UINT8          GpioAccessType,
  IN  UINT8          Expander,
  IN  UINT32         GpioNumber,
  IN  BOOLEAN        Value
);

/**
  Get Security Level.
  @param[in]  Type      ITBT (0x80) or DTBT (0x00)
  @param[in]  Bus       Bus number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Device    Device number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Function  Function number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Timeout   Time out with 100 ms garnularity
**/
UINT8
GetSecLevel (
  IN    BOOLEAN                 Type,
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT8                   Command,
  IN    UINT32                  Timeout
  );

/**
  Set Security Level.
  @param[in]  Data      Security State
  @param[in]  Type      ITBT (0x80) or DTBT (0x00)
  @param[in]  Bus       Bus number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Device    Device number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Function  Function number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Timeout   Time out with 100 ms garnularity
**/
BOOLEAN
SetSecLevel (
  IN    UINT8                   Data,
  IN    BOOLEAN                 Type,
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT8                   Command,
  IN    UINT32                  Timeout
  );

/**
Execute TBT Mail Box Command

@param[in]  Command   TBT Command
@param[in]  Type      ITBT (0x80) or DTBT (0x00)
@param[in]  Bus       Bus number for HIA (ITBT) or Host Router (DTBT)
@param[in]  Device    Device number for HIA (ITBT) or Host Router (DTBT)
@param[in]  Function  Function number for HIA (ITBT) or Host Router (DTBT)
@param[in]  Timeout   Time out with 100 ms garnularity
@Retval     true      if command executes succesfully
**/
BOOLEAN
TbtSetPcie2TbtCommand(
  IN    UINT8                   Command,
  IN    BOOLEAN                 Type,
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT32                  Timeout
);
/**
  Check connected TBT controller is supported or not by DeviceID

  @param[in]  DeviceID              DeviceID of of TBT controller


  @retval     TRUE                  Valid DeviceID
  @retval     FALSE                 Invalid DeviceID
**/

BOOLEAN
IsTbtHostRouter (
  IN    UINT16  DeviceID
  );

/**
  Get Pch/Peg Pcie Root Port Device and Function Number for TBT by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
**/
EFI_STATUS
EFIAPI
GetDTbtRpDevFun(
  IN  BOOLEAN Type,
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFunc
  );

/**
  Internal function to Wait for Tbt2PcieDone Bit.to Set or clear
  @param[in]  CommandOffsetAddress      Tbt2Pcie Register Address
  @param[in]  TimeOut                   Time out with 100 ms garnularity
  @param[in]  Tbt2PcieDone              Wait condition (wait for Bit to Clear/Set)
  @param[out] *Tbt2PcieValue Function   Register value
**/
BOOLEAN
InternalWaitforCommandCompletion (
  IN  UINT64   CommandOffsetAddress,
  IN  UINT32   TimeOut,
  IN  BOOLEAN  Tbt2PcieDone,
  OUT UINT32   *Tbt2PcieValue
  );

#endif


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmiHandler.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmiHandler.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmiHandler.h

/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _TBT_SMI_HANDLER_H_
#define _TBT_SMI_HANDLER_H_

//#include <Library/TbtCommonLib.h>
//#include <Library/IoLib.h>
//#include <IndustryStandard/Pci.h>

#ifdef PROGRESS_CODE
#undef PROGRESS_CODE
#endif

#define MAX_TBT_DEPTH         6

#define P2P_BRIDGE            (((PCI_CLASS_BRIDGE) << 8) | (PCI_CLASS_BRIDGE_P2P))

#define BAR_ALIGN(v, a)       ((((v) - 1) | (a)) + 1)

#define CMD_BUS_MASTER        BIT2
#define CMD_BM_IO             (CMD_BUS_MASTER | BIT0)
#define CMD_BM_MEM            (CMD_BUS_MASTER | BIT1)
#define CMD_BM_MEM_IO         (CMD_BUS_MASTER | BIT1 | BIT0)

#define DEF_CACHE_LINE_SIZE   0x20
#define DEF_RES_IO_PER_DEV    4
#define DEF_RES_MEM_PER_DEV   32
#define DEF_RES_PMEM_PER_DEV  32

#define DOCK_BUSSES           8

#define DISBL_IO_REG1C        0x01F1
#define DISBL_MEM32_REG20     0x0000FFF0
#define DISBL_PMEM_REG24      0x0001FFF1

#define count(x)              (sizeof (x) / sizeof ((x)[0]))

#define PCIE_CAP_ID_SSID_SSVID 0x0D
#define INVALID_PCI_DEVICE    0xFFFFFFFF
#define PCI_TBT_VESC_REG2     0x510

typedef struct _PortInfo {
  UINT8   IoBase;
  UINT8   IoLimit;
  UINT16  MemBase;
  UINT16  MemLimit;
  UINT64  PMemBase64;
  UINT64  PMemLimit64;
  UINT8   BusNumLimit;
  UINT8   ConfedEP;
} PORT_INFO;

typedef struct _MEM_REGS {
  UINT32  Base;
  UINT32  Limit;
} MEM_REGS;

typedef struct _PMEM_REGS {
  UINT64  Base64;
  UINT64  Limit64;
} PMEM_REGS;

typedef struct _IO_REGS {
  UINT16  Base;
  UINT16  Limit;
} IO_REGS;

typedef struct _BRDG_RES_CONFIG {
  UINT8   Cmd;
  UINT8   Cls;
  UINT8   IoBase;
  UINT8   IoLimit;
  UINT16  MemBase;
  UINT16  MemLimit;
  UINT64  PMemBase64;
  UINT64  PMemLimit64;
} BRDG_RES_CONFIG;

typedef struct _BRDG_CONFIG {
  DEV_ID          DevId;
  UINT8           PBus;
  UINT8           SBus;
  UINT8           SubBus;
  BOOLEAN         IsDSBridge;
  BRDG_RES_CONFIG Res;
} BRDG_CONFIG;

enum {
  HR_US_PORT,
  HR_DS_PORT0,
  HR_DS_PORT3,
  HR_DS_PORT4,
  HR_DS_PORT5,
  HR_DS_PORT6,
  MAX_CFG_PORTS
};

enum {
  HR_DS_PORT1   = HR_DS_PORT3
};

//
// Alpine Ridge
//
enum {
  AR_DS_PORT1 = HR_DS_PORT3,
  AR_DS_PORT2,
  AR_DS_PORT3,
  AR_DS_PORT4
};

typedef struct _HR_CONFIG {
  UINT16  DeviceId;
  UINT8   HRBus;
  UINT8   MinDSNumber;
  UINT8   MaxDSNumber;
  UINT8   BridgeLoops;
} HR_CONFIG;

STATIC const BRDG_RES_CONFIG  NOT_IN_USE_BRIDGE = {
  CMD_BUS_MASTER,
  0,
  DISBL_IO_REG1C & 0xFF,
  DISBL_IO_REG1C >> 8,
  DISBL_MEM32_REG20 & 0xFFFF,
  DISBL_MEM32_REG20 >> 16,
  DISBL_PMEM_REG24 & 0xFFFF,
  DISBL_PMEM_REG24 >> 16
};

typedef union _BRDG_CIO_MAP_REG {
  UINT32  AB_REG;
  struct {
    UINT32  NumOfDSPorts : 5;
    UINT32  CioPortMap : 27;
  } Bits;
} BRDG_CIO_MAP_REG;

//
// Functions
//
VOID
ThunderboltCallback (
  IN UINT8 Type
  );

VOID
TbtDisablePCIDevicesAndBridges (
  IN UINT8 Type
  );

VOID
EndOfThunderboltCallback(
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
);

VOID
ConfigureTbtAspm(
  IN UINT8       Type,
  IN UINT16      Aspm
);

UINT8
PcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  );

#endif


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Private/Library/PeiTbtCommonInitLib.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Private/Library/PeiTbtCommonInitLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Private/Library/PeiTbtCommonInitLib.h

/** @file
  PEI TBT Common Init Dispatch library Header file


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PEI_TBT_COMMON_INIT_LIB_H__
#define __PEI_TBT_COMMON_INIT_LIB_H__

//#include <Library/PeiServicesLib.h>
//#include <Library/DebugLib.h>
//#include <Library/PeiTbtTaskDispatchLib.h>
//#include <Library/MemoryAllocationLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/GpioLib.h>
//#include <Library/TimerLib.h>
//#include <Library/IoLib.h>
//#include <Library/PciSegmentLib.h>
//#include <Library/PcdLib.h>
//#include <Library/TbtCommonLib.h>
//#include <IndustryStandard/Pci22.h>
//#include <Library/PmcLib.h>
//#include <PlatformNvRamHookLib.h>

BOOLEAN
IsHostRouterPresentBeforeSleep(
IN  UINT8        ControllerType,
IN  UINT8        Controller
);

VOID
TbtSetSxMode(
IN    BOOLEAN                 Type,
IN    UINT8                   Bus,
IN    UINT8                   Device,
IN    UINT8                   Function,
IN    UINT8                   TbtBootOn
);

VOID
TbtClearVgaRegisters(
IN    UINTN                   Segment,
IN    UINTN                   Bus,
IN    UINTN                   Device,
IN    UINTN                   Function
);

#endif


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Ppi/PeiTbtPolicy.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Ppi/PeiTbtPolicy.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Ppi/PeiTbtPolicy.h

/** @file
TBT PEI Policy


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_TBT_POLICY_H_
#define _PEI_TBT_POLICY_H_

//#include <TbtPolicyCommonDefinition.h>

#pragma pack(push, 1)

#define PEI_TBT_POLICY_REVISION 1

/**
 TBT PEI configuration\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct _PEI_TBT_POLICY {
  DTBT_COMMON_CONFIG     DTbtCommonConfig;                                  ///< dTbt Common Configuration
  DTBT_CONTROLLER_CONFIG DTbtControllerConfig [MAX_DTBT_CONTROLLER_NUMBER]; ///< dTbt Controller Configuration
} PEI_TBT_POLICY;

#pragma pack(pop)

#endif


//========================================================================================
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Library/PeiTbtTaskDispatchLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Library/PeiTbtTaskDispatchLib.h

/** @file
  PEI TBT Task Dispatch library Header file


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PEI_TBT_TASK_DISPATCH_LIB_H__
#define __PEI_TBT_TASK_DISPATCH_LIB_H__
//#include <Library/PeiServicesLib.h>
//#include <Library/MemoryAllocationLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/DebugLib.h>
//#include <Library/GpioLib.h>
//#include <Library/IoLib.h>
//#include <Library/PciSegmentLib.h>
//#include <Library/HobLib.h>
//#include <Library/PcdLib.h>
//#include <Ppi/PeiTbtPolicy.h>

typedef
EFI_STATUS
(EFIAPI *TBT_TASK) (
  PEI_TBT_POLICY  *PeiTbtConfig
);

typedef enum {
  TBT_NULL,                ///< All policy flags turned off.
  TBT_NORMAL   = (1 << 0), ///< Execute TBT function on cold reset.
  TBT_S3       = (1 << 1), ///< Execute TBT function on S3 exit.
  TBT_S4       = (1 << 2), ///< Execute TBT function on S4 exit.
  TBT_ALL      = MAX_UINTN ///< Execute TBT function always.
} TBT_BOOT_MODE;

typedef struct {
  TBT_TASK      TbtTask;         ///< Ptr to function to execute, with parameter list.
  TBT_BOOT_MODE TbtBootModeFlag; ///< Call table base on TbtBootModeFlag
  CHAR8         *String;         ///< Output string describing this task.
} TBT_CALL_TABLE_ENTRY;

/**
  Covert the current EFI_BOOT_MODE to TBT_BOOT_MODE
**/
TBT_BOOT_MODE
TbtGetBootMode (
  VOID
);

/**
  TbtTaskDistpach: Dispatch the TBT tasks according to TBT_CALL_TABLE_ENTRY

  @param[in] TBT_CALL_TABLE_ENTRY   TbtCallTable

**/
VOID
TbtTaskDistpach (
  IN TBT_CALL_TABLE_ENTRY *TbtCallTable
);
#endif


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLibrary.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLibrary.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLibrary.h

/** @file
  Header file for the DxeTBTPolicy library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DXE_TBT_POLICY_LIBRARY_H_
#define _DXE_TBT_POLICY_LIBRARY_H_

//#include <Uefi.h>
//#include <Library/DebugLib.h>
//#include <Library/UefiLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/MemoryAllocationLib.h>
//#include <ConfigBlock.h>
////#include <SetupVariable.h>
//#include <Guid/EventGroup.h>

#endif // _DXE_TBT_POLICY_LIBRARY_H_


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Library/PeiTbtPolicyLib/PeiTbtPolicyLibrary.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Library/PeiTbtPolicyLib/PeiTbtPolicyLibrary.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Library/PeiTbtPolicyLib/PeiTbtPolicyLibrary.h

/** @file
  Header file for the PeiTBTPolicy library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_TBT_POLICY_LIBRARY_H_
#define _PEI_TBT_POLICY_LIBRARY_H_

//#include <Library/DebugLib.h>
//#include <Library/PeiServicesTablePointerLib.h>
//#include <Library/PeiServicesLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/MemoryAllocationLib.h>

#endif // _PEI_TBT_POLICY_LIBRARY_H_


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Include/Private/Library/PeiDTbtInitLib.h
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Include/Private/Library/PeiDTbtInitLib.h
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Include/Private/Library/PeiDTbtInitLib.h

/** @file
  PEI DTBT Init Dispatch library Header file


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PEI_DTBT_INIT_LIB_H__
#define __PEI_DTBT_INIT_LIB_H__

//#include <Private/Library/PeiTbtCommonInitLib.h>
//#include <Library/PeiTbtTaskDispatchLib.h>

extern TBT_CALL_TABLE_ENTRY DTbtCallTable[];

/**
  Get Thunderbolt(TM) (TBT) PEI Policy Data.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtGetPeiTbtPolicyData (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  Toggle related GPIO pin for DTBT.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtToggleGPIO (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  set tPCH25 Timing to 10 ms for DTBT.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSetTPch25Timing (
  IN  PEI_TBT_POLICY  *PeiTbtConfig
);

/**
  Do ForcePower for DTBT Controller

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtForcePower (
  IN  PEI_TBT_POLICY  *PeiTbtConfig,
  IN  UINTN DTbtControllerNumber
);

/**
  Clear VGA Registers for DTBT.

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtClearVgaRegisters (
  IN  PEI_TBT_POLICY  *PeiTbtConfig,
  IN  UINTN DTbtControllerNumber
);

/**
  Exectue Mail box command "Boot On".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtBootOn (
  IN  PEI_TBT_POLICY  *PeiTbtConfig,
  IN  UINTN DTbtControllerNumber
);

/**
  Exectue Mail box command "USB On".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtUsbOn (
  IN  PEI_TBT_POLICY  *PeiTbtConfig,
  IN  UINTN DTbtControllerNumber
);

/**
  Exectue Mail box command "Sx Exit".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSxExitFlow (
  IN  PEI_TBT_POLICY  *PeiTbtConfig,
  IN  UINTN DTbtControllerNumber
);

#endif


//========================================================================================
// = edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/Register/PchRegsPmc.h
//   edk2-platforms/Silicon/Intel/PurleyRefreshSiliconPkg/Pch/Include/Register/PchRegsPmc.h
// . edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/Register/PchRegsPmc.h
// = edk2-platforms/Silicon/Intel/SimicsIch10Pkg/Include/Register/PchRegsPmc.h

/** @file
  Register names for PCH PMC device

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, PCH registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between PCH generations are denoted by
    "_PCH_[generation_name]_" in register/bit names.
  - Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
    Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
    e.g., "_PCH_H_", "_PCH_LP_"
    Registers / bits names without _H_ or _LP_ apply for both H and LP.
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_PCH_" without [generation_name] inserted.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_REGS_PMC_H_
#define _PCH_REGS_PMC_H_

//
// PMC Registers (D31:F2)
//
#define PCI_DEVICE_NUMBER_PCH_PMC                           31
#define PCI_FUNCTION_NUMBER_PCH_PMC                         2

#define R_PCH_PMC_PM_DATA_BAR                               0x10
#define B_PCH_PMC_PM_DATA_BAR                               0xFFFFC000
#define R_PCH_PMC_ACPI_BASE                                 0x40
#define B_PCH_PMC_ACPI_BASE_BAR                             0xFFFC
#define R_PCH_PMC_ACPI_CNT                                  0x44
#define B_PCH_PMC_ACPI_CNT_PWRM_EN                          BIT8                          ///< PWRM enable
#define B_PCH_PMC_ACPI_CNT_ACPI_EN                          BIT7                          ///< ACPI eanble
#define B_PCH_PMC_ACPI_CNT_SCIS                             (BIT2 | BIT1 | BIT0)          ///< SCI IRQ select
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ9                        0
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ10                       1
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ11                       2
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ20                       4
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ21                       5
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ22                       6
#define V_PCH_PMC_ACPI_CNT_SCIS_IRQ23                       7
#define R_PCH_PMC_PWRM_BASE                                 0x48
#define B_PCH_PMC_PWRM_BASE_BAR                             0xFFFF0000                    ///< PWRM must be 64KB alignment to align the source decode.
#define R_PCH_PMC_GEN_PMCON_A                               0xA0
#define B_PCH_PMC_GEN_PMCON_A_DC_PP_DIS                     BIT30
#define B_PCH_PMC_GEN_PMCON_A_DSX_PP_DIS                    BIT29
#define B_PCH_PMC_GEN_PMCON_A_AG3_PP_EN                     BIT28
#define B_PCH_PMC_GEN_PMCON_A_SX_PP_EN                      BIT27
#define B_PCH_PMC_GEN_PMCON_A_DISB                          BIT23
#define B_PCH_PMC_GEN_PMCON_A_MEM_SR                        BIT21
#define B_PCH_PMC_GEN_PMCON_A_MS4V                          BIT18
#define B_PCH_PMC_GEN_PMCON_A_GBL_RST_STS                   BIT16
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_OPI_PLL_SD_INC0         BIT13
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_SPXB_CG_INC0            BIT12
#define B_PCH_PMC_GEN_PMCON_A_BIOS_PCI_EXP_EN               BIT10
#define B_PCH_PMC_GEN_PMCON_A_PWRBTN_LVL                    BIT9
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_L1LOW_C0                BIT7
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_L1LOW_OPI_ON            BIT6
#define B_PCH_PMC_GEN_PMCON_A_ALLOW_L1LOW_BCLKREQ_ON        BIT5
#define B_PCH_PMC_GEN_PMCON_A_SMI_LOCK                      BIT4
#define B_PCH_PMC_GEN_PMCON_A_ESPI_SMI_LOCK                 BIT3             ///< ESPI SMI lock
#define B_PCH_PMC_GEN_PMCON_A_PER_SMI_SEL                   0x0003
#define V_PCH_PMC_GEN_PMCON_A_PER_SMI_64S                   0x0000
#define V_PCH_PMC_GEN_PMCON_A_PER_SMI_32S                   0x0001
#define V_PCH_PMC_GEN_PMCON_A_PER_SMI_16S                   0x0002
#define V_PCH_PMC_GEN_PMCON_A_PER_SMI_8S                    0x0003
#define R_PCH_PMC_GEN_PMCON_B                               0xA4
#define B_PCH_PMC_GEN_PMCON_B_SLPSX_STR_POL_LOCK            BIT18            ///< Lock down SLP_S3/SLP_S4 Minimum Assertion width
#define B_PCH_PMC_GEN_PMCON_B_ACPI_BASE_LOCK                BIT17            ///< Lock ACPI BASE at 0x40, only cleared by reset when set
#define B_PCH_PMC_GEN_PMCON_B_PM_DATA_BAR_DIS               BIT16
#define B_PCH_PMC_GEN_PMCON_B_PME_B0_S5_DIS                 BIT15
#define B_PCH_PMC_GEN_PMCON_B_SUS_PWR_FLR                   BIT14
#define B_PCH_PMC_GEN_PMCON_B_WOL_EN_OVRD                   BIT13
#define B_PCH_PMC_GEN_PMCON_B_DISABLE_SX_STRETCH            BIT12
#define B_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW                    0xC00
#define V_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW_60US               0x000
#define V_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW_1MS                0x400
#define V_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW_50MS               0x800
#define V_PCH_PMC_GEN_PMCON_B_SLP_S3_MAW_2S                 0xC00
#define B_PCH_PMC_GEN_PMCON_B_HOST_RST_STS                  BIT9
#define B_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL                    0xC0
#define V_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL_64MS               0xC0
#define V_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL_32MS               0x80
#define V_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL_16MS               0x40
#define V_PCH_PMC_GEN_PMCON_B_SWSMI_RTSL_1_5MS              0x00
#define B_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW                    0x30
#define V_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW_1S                 0x30
#define V_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW_2S                 0x20
#define V_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW_3S                 0x10
#define V_PCH_PMC_GEN_PMCON_B_SLP_S4_MAW_4S                 0x00
#define B_PCH_PMC_GEN_PMCON_B_SLP_S4_ASE                    BIT3
#define B_PCH_PMC_GEN_PMCON_B_RTC_PWR_STS                   BIT2
#define B_PCH_PMC_GEN_PMCON_B_PWR_FLR                       BIT1
#define B_PCH_PMC_GEN_PMCON_B_AFTERG3_EN                    BIT0
#define R_PCH_PMC_BM_CX_CNF                                 0xA8
#define B_PCH_PMC_BM_CX_CNF_STORAGE_BREAK_EN                BIT31
#define B_PCH_PMC_BM_CX_CNF_PCIE_BREAK_EN                   BIT30
#define B_PCH_PMC_BM_CX_CNF_AZ_BREAK_EN                     BIT24
#define B_PCH_PMC_BM_CX_CNF_DPSN_BREAK_EN                   BIT19
#define B_PCH_PMC_BM_CX_CNF_XHCI_BREAK_EN                   BIT17
#define B_PCH_PMC_BM_CX_CNF_SATA3_BREAK_EN                  BIT16
#define B_PCH_PMC_BM_CX_CNF_SCRATCHPAD                      BIT15
#define B_PCH_PMC_BM_CX_CNF_PHOLD_BM_STS_BLOCK              BIT14
#define B_PCH_PMC_BM_CX_CNF_MASK_CF                         BIT11
#define B_PCH_PMC_BM_CX_CNF_BM_STS_ZERO_EN                  BIT10
#define B_PCH_PMC_BM_CX_CNF_PM_SYNC_MSG_MODE                BIT9
#define R_PCH_PMC_ETR3                                      0xAC
#define B_PCH_PMC_ETR3_CF9LOCK                              BIT31           ///< CF9h Lockdown
#define B_PCH_PMC_ETR3_USB_CACHE_DIS                        BIT21
#define B_PCH_PMC_ETR3_CF9GR                                BIT20           ///< CF9h Global Reset
#define B_PCH_PMC_ETR3_SKIP_HOST_RST_HS                     BIT19
#define B_PCH_PMC_ETR3_CWORWRE                              BIT18

//
// ACPI and legacy I/O register offsets from ACPIBASE
//
#define R_PCH_ACPI_PM1_STS                        0x00
#define S_PCH_ACPI_PM1_STS                        2
#define B_PCH_ACPI_PM1_STS_WAK                    BIT15
#define B_PCH_ACPI_PM1_STS_PCIEXP_WAKE_STS        BIT14
#define B_PCH_ACPI_PM1_STS_PRBTNOR                BIT11
#define B_PCH_ACPI_PM1_STS_RTC                    BIT10
#define B_PCH_ACPI_PM1_STS_PWRBTN                 BIT8
#define B_PCH_ACPI_PM1_STS_GBL                    BIT5
#define B_PCH_ACPI_PM1_STS_BM                     BIT4
#define B_PCH_ACPI_PM1_STS_TMROF                  BIT0
#define N_PCH_ACPI_PM1_STS_WAK                    15
#define N_PCH_ACPI_PM1_STS_PCIEXP_WAKE_STS        14
#define N_PCH_ACPI_PM1_STS_PRBTNOR                11
#define N_PCH_ACPI_PM1_STS_RTC                    10
#define N_PCH_ACPI_PM1_STS_PWRBTN                 8
#define N_PCH_ACPI_PM1_STS_GBL                    5
#define N_PCH_ACPI_PM1_STS_BM                     4
#define N_PCH_ACPI_PM1_STS_TMROF                  0

#define R_PCH_ACPI_PM1_EN                         0x02
#define S_PCH_ACPI_PM1_EN                         2
#define B_PCH_ACPI_PM1_EN_PCIEXP_WAKE_DIS         BIT14
#define B_PCH_ACPI_PM1_EN_RTC                     BIT10
#define B_PCH_ACPI_PM1_EN_PWRBTN                  BIT8
#define B_PCH_ACPI_PM1_EN_GBL                     BIT5
#define B_PCH_ACPI_PM1_EN_TMROF                   BIT0
#define N_PCH_ACPI_PM1_EN_PCIEXP_WAKE_DIS         14
#define N_PCH_ACPI_PM1_EN_RTC                     10
#define N_PCH_ACPI_PM1_EN_PWRBTN                  8
#define N_PCH_ACPI_PM1_EN_GBL                     5
#define N_PCH_ACPI_PM1_EN_TMROF                   0

#define R_PCH_ACPI_PM1_CNT                        0x04
#define S_PCH_ACPI_PM1_CNT                        4
#define B_PCH_ACPI_PM1_CNT_SLP_EN                 BIT13
#define B_PCH_ACPI_PM1_CNT_SLP_TYP                (BIT12 | BIT11 | BIT10)
#define V_PCH_ACPI_PM1_CNT_S0                     0
#define V_PCH_ACPI_PM1_CNT_S1                     BIT10
#define V_PCH_ACPI_PM1_CNT_S3                     (BIT12 | BIT10)
#define V_PCH_ACPI_PM1_CNT_S4                     (BIT12 | BIT11)
#define V_PCH_ACPI_PM1_CNT_S5                     (BIT12 | BIT11 | BIT10)
#define B_PCH_ACPI_PM1_CNT_GBL_RLS                BIT2
#define B_PCH_ACPI_PM1_CNT_BM_RLD                 BIT1
#define B_PCH_ACPI_PM1_CNT_SCI_EN                 BIT0

#define R_PCH_ACPI_PM1_TMR                        0x08
#define V_PCH_ACPI_TMR_FREQUENCY                  3579545
#define B_PCH_ACPI_PM1_TMR_VAL                    0xFFFFFF
#define V_PCH_ACPI_PM1_TMR_MAX_VAL                0x1000000       ///< The timer is 24 bit overflow

#define R_PCH_SMI_EN                              0x30
#define S_PCH_SMI_EN                              4
#define B_PCH_SMI_EN_LEGACY_USB3                  BIT31
#define B_PCH_SMI_EN_GPIO_UNLOCK_SMI              BIT27
#define B_PCH_SMI_EN_LEGACY_USB2                  BIT17
#define B_PCH_SMI_EN_PERIODIC                     BIT14
#define B_PCH_SMI_EN_TCO                          BIT13
#define B_PCH_SMI_EN_MCSMI                        BIT11
#define B_PCH_SMI_EN_BIOS_RLS                     BIT7
#define B_PCH_SMI_EN_SWSMI_TMR                    BIT6
#define B_PCH_SMI_EN_APMC                         BIT5
#define B_PCH_SMI_EN_ON_SLP_EN                    BIT4
#define B_PCH_SMI_EN_LEGACY_USB                   BIT3
#define B_PCH_SMI_EN_BIOS                         BIT2
#define B_PCH_SMI_EN_EOS                          BIT1
#define B_PCH_SMI_EN_GBL_SMI                      BIT0
#define N_PCH_SMI_EN_LEGACY_USB3                  31
#define N_PCH_SMI_EN_ESPI                         28
#define N_PCH_SMI_EN_GPIO_UNLOCK                  27
#define N_PCH_SMI_EN_INTEL_USB2                   18
#define N_PCH_SMI_EN_LEGACY_USB2                  17
#define N_PCH_SMI_EN_PERIODIC                     14
#define N_PCH_SMI_EN_TCO                          13
#define N_PCH_SMI_EN_MCSMI                        11
#define N_PCH_SMI_EN_BIOS_RLS                     7
#define N_PCH_SMI_EN_SWSMI_TMR                    6
#define N_PCH_SMI_EN_APMC                         5
#define N_PCH_SMI_EN_ON_SLP_EN                    4
#define N_PCH_SMI_EN_LEGACY_USB                   3
#define N_PCH_SMI_EN_BIOS                         2
#define N_PCH_SMI_EN_EOS                          1
#define N_PCH_SMI_EN_GBL_SMI                      0

#define R_PCH_SMI_STS                             0x34
#define S_PCH_SMI_STS                             4
#define B_PCH_SMI_STS_LEGACY_USB3                 BIT31
#define B_PCH_SMI_STS_GPIO_UNLOCK                 BIT27
#define B_PCH_SMI_STS_SPI                         BIT26
#define B_PCH_SMI_STS_MONITOR                     BIT21
#define B_PCH_SMI_STS_PCI_EXP                     BIT20
#define B_PCH_SMI_STS_PATCH                       BIT19
#define B_PCH_SMI_STS_INTEL_USB2                  BIT18
#define B_PCH_SMI_STS_LEGACY_USB2                 BIT17
#define B_PCH_SMI_STS_SMBUS                       BIT16
#define B_PCH_SMI_STS_SERIRQ                      BIT15
#define B_PCH_SMI_STS_PERIODIC                    BIT14
#define B_PCH_SMI_STS_TCO                         BIT13
#define B_PCH_SMI_STS_DEVMON                      BIT12
#define B_PCH_SMI_STS_MCSMI                       BIT11
#define B_PCH_SMI_STS_GPIO_SMI                    BIT10
#define B_PCH_SMI_STS_GPE0                        BIT9
#define B_PCH_SMI_STS_PM1_STS_REG                 BIT8
#define B_PCH_SMI_STS_SWSMI_TMR                   BIT6
#define B_PCH_SMI_STS_APM                         BIT5
#define B_PCH_SMI_STS_ON_SLP_EN                   BIT4
#define B_PCH_SMI_STS_LEGACY_USB                  BIT3
#define B_PCH_SMI_STS_BIOS                        BIT2
#define N_PCH_SMI_STS_LEGACY_USB3                 31
#define N_PCH_SMI_STS_ESPI                        28
#define N_PCH_SMI_STS_GPIO_UNLOCK                 27
#define N_PCH_SMI_STS_SPI                         26
#define N_PCH_SMI_STS_MONITOR                     21
#define N_PCH_SMI_STS_PCI_EXP                     20
#define N_PCH_SMI_STS_PATCH                       19
#define N_PCH_SMI_STS_INTEL_USB2                  18
#define N_PCH_SMI_STS_LEGACY_USB2                 17
#define N_PCH_SMI_STS_SMBUS                       16
#define N_PCH_SMI_STS_SERIRQ                      15
#define N_PCH_SMI_STS_PERIODIC                    14
#define N_PCH_SMI_STS_TCO                         13
#define N_PCH_SMI_STS_DEVMON                      12
#define N_PCH_SMI_STS_MCSMI                       11
#define N_PCH_SMI_STS_GPIO_SMI                    10
#define N_PCH_SMI_STS_GPE0                        9
#define N_PCH_SMI_STS_PM1_STS_REG                 8
#define N_PCH_SMI_STS_SWSMI_TMR                   6
#define N_PCH_SMI_STS_APM                         5
#define N_PCH_SMI_STS_ON_SLP_EN                   4
#define N_PCH_SMI_STS_LEGACY_USB                  3
#define N_PCH_SMI_STS_BIOS                        2

#define R_PCH_ACPI_GPE_CNTL                       0x40
#define B_PCH_ACPI_GPE_CNTL_SWGPE_CTRL            BIT17

#define R_PCH_DEVACT_STS                          0x44
#define S_PCH_DEVACT_STS                          2
#define B_PCH_DEVACT_STS_MASK                     0x13E1
#define B_PCH_DEVACT_STS_KBC                      BIT12
#define B_PCH_DEVACT_STS_PIRQDH                   BIT9
#define B_PCH_DEVACT_STS_PIRQCG                   BIT8
#define B_PCH_DEVACT_STS_PIRQBF                   BIT7
#define B_PCH_DEVACT_STS_PIRQAE                   BIT6
#define B_PCH_DEVACT_STS_D0_TRP                   BIT0
#define N_PCH_DEVACT_STS_KBC                      12
#define N_PCH_DEVACT_STS_PIRQDH                   9
#define N_PCH_DEVACT_STS_PIRQCG                   8
#define N_PCH_DEVACT_STS_PIRQBF                   7
#define N_PCH_DEVACT_STS_PIRQAE                   6

#define R_PCH_ACPI_PM2_CNT                        0x50
#define B_PCH_ACPI_PM2_CNT_ARB_DIS                BIT0

#define R_PCH_OC_WDT_CTL                          0x54
#define B_PCH_OC_WDT_CTL_RLD                      BIT31
#define B_PCH_OC_WDT_CTL_ICCSURV_STS              BIT25
#define B_PCH_OC_WDT_CTL_NO_ICCSURV_STS           BIT24
#define B_PCH_OC_WDT_CTL_FORCE_ALL                BIT15
#define B_PCH_OC_WDT_CTL_EN                       BIT14
#define B_PCH_OC_WDT_CTL_ICCSURV                  BIT13
#define B_PCH_OC_WDT_CTL_LCK                      BIT12
#define B_PCH_OC_WDT_CTL_TOV_MASK                 0x3FF
#define B_PCH_OC_WDT_CTL_FAILURE_STS              BIT23
#define B_PCH_OC_WDT_CTL_UNXP_RESET_STS           BIT22
#define B_PCH_OC_WDT_CTL_AFTER_POST               0x3F0000
#define V_PCH_OC_WDT_CTL_STATUS_FAILURE           1
#define V_PCH_OC_WDT_CTL_STATUS_OK                0

#define R_PCH_ACPI_GPE0_STS_31_0                  0x80
#define R_PCH_ACPI_GPE0_STS_63_32                 0x84
#define R_PCH_ACPI_GPE0_STS_95_64                 0x88
#define R_PCH_ACPI_GPE0_STS_127_96                0x8C
#define S_PCH_ACPI_GPE0_STS_127_96                4
#define B_PCH_ACPI_GPE0_STS_127_96_WADT           BIT18
#define B_PCH_ACPI_GPE0_STS_127_96_LAN_WAKE       BIT16
#define B_PCH_ACPI_GPE0_STS_127_96_PME_B0         BIT13
#define B_PCH_ACPI_GPE0_STS_127_96_ME_SCI         BIT12
#define B_PCH_ACPI_GPE0_STS_127_96_PME            BIT11
#define B_PCH_ACPI_GPE0_STS_127_96_BATLOW         BIT10
#define B_PCH_ACPI_GPE0_STS_127_96_PCI_EXP        BIT9
#define B_PCH_ACPI_GPE0_STS_127_96_RI             BIT8
#define B_PCH_ACPI_GPE0_STS_127_96_SMB_WAK        BIT7
#define B_PCH_ACPI_GPE0_STS_127_96_TC0SCI         BIT6
#define B_PCH_ACPI_GPE0_STS_127_96_SWGPE          BIT2
#define B_PCH_ACPI_GPE0_STS_127_96_HOT_PLUG       BIT1
#define N_PCH_ACPI_GPE0_STS_127_96_PME_B0         13
#define N_PCH_ACPI_GPE0_STS_127_96_PME            11
#define N_PCH_ACPI_GPE0_STS_127_96_BATLOW         10
#define N_PCH_ACPI_GPE0_STS_127_96_PCI_EXP        9
#define N_PCH_ACPI_GPE0_STS_127_96_RI             8
#define N_PCH_ACPI_GPE0_STS_127_96_SMB_WAK        7
#define N_PCH_ACPI_GPE0_STS_127_96_TC0SCI         6
#define N_PCH_ACPI_GPE0_STS_127_96_SWGPE          2
#define N_PCH_ACPI_GPE0_STS_127_96_HOT_PLUG       1

#define R_PCH_ACPI_GPE0_EN_31_0                   0x90
#define R_PCH_ACPI_GPE0_EN_63_32                  0x94
#define R_PCH_ACPI_GPE0_EN_95_64                  0x98
#define R_PCH_ACPI_GPE0_EN_127_96                 0x9C
#define S_PCH_ACPI_GPE0_EN_127_96                 4
#define B_PCH_ACPI_GPE0_EN_127_96_WADT            BIT18
#define B_PCH_ACPI_GPE0_EN_127_96_LAN_WAKE        BIT16
#define B_PCH_ACPI_GPE0_EN_127_96_PME_B0          BIT13
#define B_PCH_ACPI_GPE0_EN_127_96_ME_SCI          BIT12
#define B_PCH_ACPI_GPE0_EN_127_96_PME             BIT11
#define B_PCH_ACPI_GPE0_EN_127_96_BATLOW          BIT10
#define B_PCH_ACPI_GPE0_EN_127_96_PCI_EXP         BIT9
#define B_PCH_ACPI_GPE0_EN_127_96_RI              BIT8
#define B_PCH_ACPI_GPE0_EN_127_96_TC0SCI          BIT6
#define B_PCH_ACPI_GPE0_EN_127_96_SWGPE           BIT2
#define B_PCH_ACPI_GPE0_EN_127_96_HOT_PLUG        BIT1
#define N_PCH_ACPI_GPE0_EN_127_96_PME_B0          13
#define N_PCH_ACPI_GPE0_EN_127_96_USB3            12
#define N_PCH_ACPI_GPE0_EN_127_96_PME             11
#define N_PCH_ACPI_GPE0_EN_127_96_BATLOW          10
#define N_PCH_ACPI_GPE0_EN_127_96_PCI_EXP         9
#define N_PCH_ACPI_GPE0_EN_127_96_RI              8
#define N_PCH_ACPI_GPE0_EN_127_96_TC0SCI          6
#define N_PCH_ACPI_GPE0_EN_127_96_SWGPE           2
#define N_PCH_ACPI_GPE0_EN_127_96_HOT_PLUG        1


//
// TCO register I/O map
//
#define R_PCH_TCO_RLD                             0x0
#define R_PCH_TCO_DAT_IN                          0x2
#define R_PCH_TCO_DAT_OUT                         0x3
#define R_PCH_TCO1_STS                            0x04
#define S_PCH_TCO1_STS                            2
#define B_PCH_TCO1_STS_DMISERR                    BIT12
#define B_PCH_TCO1_STS_DMISMI                     BIT10
#define B_PCH_TCO1_STS_DMISCI                     BIT9
#define B_PCH_TCO1_STS_BIOSWR                     BIT8
#define B_PCH_TCO1_STS_NEWCENTURY                 BIT7
#define B_PCH_TCO1_STS_TIMEOUT                    BIT3
#define B_PCH_TCO1_STS_TCO_INT                    BIT2
#define B_PCH_TCO1_STS_SW_TCO_SMI                 BIT1
#define B_PCH_TCO1_STS_NMI2SMI                    BIT0
#define N_PCH_TCO1_STS_DMISMI                     10
#define N_PCH_TCO1_STS_BIOSWR                     8
#define N_PCH_TCO1_STS_NEWCENTURY                 7
#define N_PCH_TCO1_STS_TIMEOUT                    3
#define N_PCH_TCO1_STS_SW_TCO_SMI                 1
#define N_PCH_TCO1_STS_NMI2SMI                    0

#define R_PCH_TCO2_STS                            0x06
#define S_PCH_TCO2_STS                            2
#define B_PCH_TCO2_STS_SMLINK_SLV_SMI             BIT4
#define B_PCH_TCO2_STS_BAD_BIOS                   BIT3
#define B_PCH_TCO2_STS_BOOT                       BIT2
#define B_PCH_TCO2_STS_SECOND_TO                  BIT1
#define B_PCH_TCO2_STS_INTRD_DET                  BIT0
#define N_PCH_TCO2_STS_INTRD_DET                  0

#define R_PCH_TCO1_CNT                            0x08
#define S_PCH_TCO1_CNT                            2
#define B_PCH_TCO_CNT_LOCK                        BIT12
#define B_PCH_TCO_CNT_TMR_HLT                     BIT11
#define B_PCH_TCO_CNT_NMI2SMI_EN                  BIT9
#define B_PCH_TCO_CNT_NMI_NOW                     BIT8
#define N_PCH_TCO_CNT_NMI2SMI_EN                  9

#define R_PCH_TCO2_CNT                            0x0A
#define S_PCH_TCO2_CNT                            2
#define B_PCH_TCO2_CNT_OS_POLICY                  0x0030
#define B_PCH_TCO2_CNT_GPI11_ALERT_DISABLE        0x0008
#define B_PCH_TCO2_CNT_INTRD_SEL                  0x0006
#define N_PCH_TCO2_CNT_INTRD_SEL                  2

#define R_PCH_TCO_MESSAGE1                        0x0C
#define R_PCH_TCO_MESSAGE2                        0x0D
#define R_PCH_TCO_WDCNT                           0x0E
#define R_PCH_TCO_SW_IRQ_GEN                      0x10
#define B_PCH_TCO_IRQ12_CAUSE                     BIT1
#define B_PCH_TCO_IRQ1_CAUSE                      BIT0
#define R_PCH_TCO_TMR                             0x12

//
// PWRM Registers
//
#define R_PCH_WADT_AC                                       0x0                         ///< Wake Alarm Device Timer: AC
#define R_PCH_WADT_DC                                       0x4                         ///< Wake Alarm Device Timer: DC
#define R_PCH_WADT_EXP_AC                                   0x8                         ///< Wake Alarm Device Expired Timer: AC
#define R_PCH_WADT_EXP_DC                                   0xC                         ///< Wake Alarm Device Expired Timer: DC
#define R_PCH_PWRM_PRSTS                                    0x10                        ///< Power and Reset Status
#define B_PCH_PWRM_PRSTS_VE_WD_TMR_STS                      BIT7                        ///< VE Watchdog Timer Status
#define B_PCH_PWRM_PRSTS_WOL_OVR_WK_STS                     BIT5
#define B_PCH_PWRM_PRSTS_FIELD_1                            BIT4
#define B_PCH_PWRM_PRSTS_ME_WAKE_STS                        BIT0
#define R_PCH_PWRM_14                                       0x14
#define R_PCH_PWRM_CFG                                      0x18                        ///< Power Management Configuration
#define B_PCH_PWRM_CFG_ALLOW_24_OSC_SD                      BIT29                       ///< Allow 24MHz Crystal Oscillator Shutdown
#define B_PCH_PWRM_CFG_ALLOW_USB2_CORE_PG                   BIT25                       ///< Allow USB2 Core Power Gating
#define B_PCH_PWRM_CFG_RTC_DS_WAKE_DIS                      BIT21                       ///< RTC Wake from Deep S4/S5 Disable
#define B_PCH_PWRM_CFG_SSMAW_MASK                           (BIT19 | BIT18)             ///< SLP_SUS# Min Assertion Width
#define V_PCH_PWRM_CFG_SSMAW_4S                             (BIT19 | BIT18)             ///< 4 seconds
#define V_PCH_PWRM_CFG_SSMAW_1S                             BIT19                       ///< 1 second
#define V_PCH_PWRM_CFG_SSMAW_0_5S                           BIT18                       ///< 0.5 second (500ms)
#define V_PCH_PWRM_CFG_SSMAW_0S                             0                           ///< 0 second
#define B_PCH_PWRM_CFG_SAMAW_MASK                           (BIT17 | BIT16)             ///< SLP_A# Min Assertion Width
#define V_PCH_PWRM_CFG_SAMAW_2S                             (BIT17 | BIT16)             ///< 2 seconds
#define V_PCH_PWRM_CFG_SAMAW_98ms                           BIT17                       ///< 98ms
#define V_PCH_PWRM_CFG_SAMAW_4S                             BIT16                       ///< 4 seconds
#define V_PCH_PWRM_CFG_SAMAW_0S                             0                           ///< 0 second
#define B_PCH_PWRM_CFG_RPCD_MASK                            (BIT9 | BIT8)               ///< Reset Power Cycle Duration
#define V_PCH_PWRM_CFG_RPCD_1S                              (BIT9 | BIT8)               ///< 1-2 seconds
#define V_PCH_PWRM_CFG_RPCD_2S                              BIT9                        ///< 2-3 seconds
#define V_PCH_PWRM_CFG_RPCD_3S                              BIT8                        ///< 3-4 seconds
#define V_PCH_PWRM_CFG_RPCD_4S                              0                           ///< 4-5 seconds (Default)
#define R_PCH_PWRM_PCH_PM_STS                               0x1C                        ///< Contains misc. fields used to record PCH power management events
#define B_PCH_PWRM_PCH_PM_STS_PMC_MSG_FULL_STS              BIT24                       ///< MTPMC transport mechanism full indication
#define R_PCH_PWRM_MTPMC                                    0x20                        ///< Message to PMC
#define V_PCH_PWRM_MTPMC_COMMAND_PG_LANE_0_15               0xE                         ///< Command to override lanes 0-15 power gating
#define V_PCH_PWRM_MTPMC_COMMAND_PG_LANE_16_31              0xF                         ///< Command to override lanes 16-31 power gating
#define B_PCH_PWRM_MTPMC_PG_CMD_DATA                        0xFFFF0000                  ///< Data part of PowerGate Message to PMC
#define N_PCH_PWRM_MTPMC_PG_CMD_DATA                        16
#define R_PCH_PWRM_PCH_PM_STS2                              0x24                        ///< PCH Power Management Status
#define R_PCH_PWRM_S3_PWRGATE_POL                           0x28                        ///< S3 Power Gating Policies
#define B_PCH_PWRM_S3DC_GATE_SUS                            BIT1                        ///< Deep S3 Enable in DC Mode
#define B_PCH_PWRM_S3AC_GATE_SUS                            BIT0                        ///< Deep S3 Enable in AC Mode
#define R_PCH_PWRM_S4_PWRGATE_POL                           0x2C                        ///< Deep S4 Power Policies
#define B_PCH_PWRM_S4DC_GATE_SUS                            BIT1                        ///< Deep S4 Enable in DC Mode
#define B_PCH_PWRM_S4AC_GATE_SUS                            BIT0                        ///< Deep S4 Enable in AC Mode
#define R_PCH_PWRM_S5_PWRGATE_POL                           0x30                        ///< Deep S5 Power Policies
#define B_PCH_PWRM_S5DC_GATE_SUS                            BIT15                       ///< Deep S5 Enable in DC Mode
#define B_PCH_PWRM_S5AC_GATE_SUS                            BIT14                       ///< Deep S5 Enable in AC Mode
#define R_PCH_PWRM_DSX_CFG                                  0x34                        ///< Deep SX Configuration
#define B_PCH_PWRM_DSX_CFG_WAKE_PIN_DSX_EN                  BIT2                        ///< WAKE# Pin DeepSx Enable
#define B_PCH_PWRM_DSX_CFG_ACPRES_PD_DSX_DIS                BIT1                        ///< AC_PRESENT pin pulldown in DeepSx disable
#define B_PCH_PWRM_DSX_CFG_LAN_WAKE_EN                      BIT0                        ///< LAN_WAKE Pin DeepSx Enable
#define R_PCH_PWRM_CFG2                                     0x3C                        ///< Power Management Configuration Reg 2
#define B_PCH_PWRM_CFG2_PBOP                                (BIT31 | BIT30 | BIT29)     ///< Power Button Override Period (PBOP)
#define N_PCH_PWRM_CFG2_PBOP                                29                          ///< Power Button Override Period (PBOP)
#define B_PCH_PWRM_CFG2_PB_DIS                              BIT28                       ///< Power Button Native Mode Disable (PB_DIS)
#define B_PCH_PWRM_CFG2_DRAM_RESET_CTL                      BIT26                       ///< DRAM RESET# control
#define R_PCH_PWRM_EN_SN_SLOW_RING                          0x48                        ///< Enable Snoop Request to SLOW_RING
#define R_PCH_PWRM_EN_SN_SLOW_RING2                         0x4C                        ///< Enable Snoop Request to SLOW_RING 2nd Reg
#define R_PCH_PWRM_EN_SN_SA                                 0x50                        ///< Enable Snoop Request to SA
#define R_PCH_PWRM_EN_SN_SA2                                0x54                        ///< Enable Snoop Request to SA 2nd Reg
#define R_PCH_PWRM_EN_SN_SLOW_RING_CF                       0x58                        ///< Enable Snoop Request to SLOW_RING_CF
#define R_PCH_PWRM_EN_NS_SA                                 0x68                        ///< Enable Non-Snoop Request to SA
#define R_PCH_PWRM_EN_CW_SLOW_RING                          0x80                        ///< Enable Clock Wake to SLOW_RING
#define R_PCH_PWRM_EN_CW_SLOW_RING2                         0x84                        ///< Enable Clock Wake to SLOW_RING 2nd Reg
#define R_PCH_PWRM_EN_CW_SA                                 0x88                        ///< Enable Clock Wake to SA
#define R_PCH_PWRM_EN_CW_SA2                                0x8C                        ///< Enable Clock Wake to SA 2nd Reg
#define R_PCH_PWRM_EN_CW_SLOW_RING_CF                       0x98                        ///< Enable Clock Wake to SLOW_RING_CF
#define R_PCH_PWRM_EN_PA_SLOW_RING                          0xA8                        ///< Enable Pegged Active to SLOW_RING
#define R_PCH_PWRM_EN_PA_SLOW_RING2                         0xAC                        ///< Enable Pegged Active to SLOW_RING 2nd Reg
#define R_PCH_PWRM_EN_PA_SA                                 0xB0                        ///< Enable Pegged Active to SA
#define R_PCH_PWRM_EN_PA_SA2                                0xB4                        ///< Enable Pegged Active to SA 2nd Reg
#define R_PCH_PWRM_EN_MISC_EVENT                            0xC0                        ///< Enable Misc PM_SYNC Events
#define R_PCH_PWRM_PMSYNC_TPR_CONFIG                        0xC4
#define B_PCH_PWRM_PMSYNC_TPR_CONFIG_LOCK                   BIT31
#define B_PCH_PWRM_PMSYNC_PCH2CPU_TT_EN                     BIT26
#define B_PCH_PWRM_PMSYNC_PCH2CPU_TT_STATE                  (BIT25 | BIT24)
#define N_PCH_PWRM_PMSYNC_PCH2CPU_TT_STATE                  24
#define V_PCH_PWRM_PMSYNC_PCH2CPU_TT_STATE_1                1
#define R_PCH_PWRM_PMSYNC_MISC_CFG                          0xC8
#define B_PCH_PWRM_PMSYNC_PM_SYNC_LOCK                      BIT15                       ///< PM_SYNC Configuration Lock
#define B_PCH_PWRM_PMSYNC_GPIO_D_SEL                        BIT11
#define B_PCH_PWRM_PMSYNC_GPIO_C_SEL                        BIT10
#define R_PCH_PWRM_PM_SYNC_STATE_HYS                        0xD0                        ///< PM_SYNC State Hysteresis
#define R_PCH_PWRM_PM_SYNC_MODE                             0xD4                        ///< PM_SYNC Pin Mode
#define R_PCH_PWRM_CFG3                                     0xE0                        ///< Power Management Configuration Reg 3
#define B_PCH_PWRM_CFG3_DSX_WLAN_PP_EN                      BIT16                       ///< Deep-Sx WLAN Phy Power Enable
#define B_PCH_PWRM_CFG3_HOST_WLAN_PP_EN                     BIT17                       ///< Host Wireless LAN Phy Power Enable
#define B_PCH_PWRM_CFG3_PWRG_LOCK                           BIT2                        ///< Lock power gating override messages
#define R_PCH_PWRM_PM_DOWN_PPB_CFG                          0xE4                        ///< PM_DOWN PCH_POWER_BUDGET CONFIGURATION
#define R_PCH_PWRM_CFG4                                     0xE8                        ///< Power Management Configuration Reg 4
#define B_PCH_PWRM_CFG4_U2_PHY_PG_EN                        BIT30                       ///< USB2 PHY SUS Well Power Gating Enable
#define B_PCH_PWRM_CFG4_CPU_IOVR_RAMP_DUR                   (0x000001FF)                ///< CPU I/O VR Ramp Duration, [8:0]
#define N_PCH_PWRM_CFG4_CPU_IOVR_RAMP_DUR                   0
#define V_PCH_PWRM_CFG4_CPU_IOVR_RAMP_DUR_70US              0x007
#define V_PCH_PWRM_CFG4_CPU_IOVR_RAMP_DUR_240US             0x018
#define R_PCH_PWRM_CPU_EPOC                                 0xEC
#define R_PCH_PWRM_VR_MISC_CTL                              0x100
#define B_PCH_PWRM_VR_MISC_CTL_VIDSOVEN                     BIT3
#define R_PCH_PWRM_GPIO_CFG                                 0x120
#define B_PCH_PWRM_GPIO_CFG_GPE0_DW2                        (BIT11 | BIT10 | BIT9 | BIT8)
#define N_PCH_PWRM_GPIO_CFG_GPE0_DW2                        8
#define B_PCH_PWRM_GPIO_CFG_GPE0_DW1                        (BIT7 | BIT6 | BIT5 | BIT4)
#define N_PCH_PWRM_GPIO_CFG_GPE0_DW1                        4
#define B_PCH_PWRM_GPIO_CFG_GPE0_DW0                        (BIT3 | BIT2 | BIT1 | BIT0)
#define N_PCH_PWRM_GPIO_CFG_GPE0_DW0                        0
#define R_PCH_PWRM_PM_SYNC_MODE_C0                          0xF4                        ///< PM_SYNC Pin Mode in C0
#define R_PCH_PWRM_ACPI_TMR_CTL                             0xFC
#define B_PCH_PWRM_ACPI_TMR_DIS                             BIT1
#define R_PCH_PWRM_124                                      0x124
#define R_PCH_PWRM_SLP_S0_RESIDENCY_COUNTER                 0x13C
#define R_PCH_PWRM_MODPHY_PM_CFG1                           0x200
#define R_PCH_PWRM_MODPHY_PM_CFG1_MLSXSWPGP                 0xFFFF
#define R_PCH_PWRM_MODPHY_PM_CFG2                           0x204    ///< ModPHY Power Management Configuration Reg 2
#define B_PCH_PWRM_MODPHY_PM_CFG2_MLSPDDGE                  BIT30    ///< ModPHY Lane SUS Power Domain Dynamic Gating Enable
#define B_PCH_PWRM_MODPHY_PM_CFG2_EMFC                      BIT29    ///< Enable ModPHY FET Control
#define B_PCH_PWRM_MODPHY_PM_CFG2_EFRT                      (BIT28 | BIT27 | BIT26 | BIT25 | BIT24)    ///< External FET Ramp Time
#define N_PCH_PWRM_MODPHY_PM_CFG2_EFRT                      24
#define V_PCH_PWRM_MODPHY_PM_CFG2_EFRT_200US                0x0A
#define B_PCH_PWRM_MODPHY_PM_CFG2_ASLOR_UFS                 BIT16    ///< UFS ModPHY SPD SPD Override
#define R_PCH_PWRM_MODPHY_PM_CFG3                           0x208    ///< ModPHY Power Management Configuration Reg 3
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_UFS             BIT16    ///< UFS ModPHY SPD RT Request
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_XDCI            BIT15    ///< xDCI ModPHY SPD RT Request
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_XHCI            BIT14    ///< xHCI ModPHY SPD RT Request
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_GBE             BIT13    ///< GbE ModPHY SPD RT Request
#define B_PCH_PWRM_MODPHY_PM_CFG3_MSPDRTREQ_SATA            BIT12    ///< SATA ModPHY SPD RT Request
#define R_PCH_PWRM_30C                                      0x30C
#define R_PCH_PWRM_OBFF_CFG                                 0x314             ///< OBFF Configuration
#define R_PCH_PWRM_31C                                      0x31C
#define R_PCH_PWRM_CPPM_MISC_CFG                            0x320             ///< CPPM Miscellaneous Configuration
#define R_PCH_PWRM_CPPM_CG_POL1A                            0x324             ///< CPPM Clock Gating Policy Reg 1
#define R_PCH_PWRM_CPPM_CG_POL2A                            0x340             ///< CPPM Clock Gating Policy Reg 3
#define R_PCH_PWRM_34C                                      0x34C
#define R_PCH_PWRM_CPPM_CG_POL3A                            0x3A8             ///< CPPM Clock Gating Policy Reg 5
#define B_PCH_PWRM_CPPM_CG_POLXA_CPPM_GX_QUAL               BIT30             ///< CPPM Shutdown Qualifier Enable for Clock Source Group X
#define B_PCH_PWRM_CPPM_CG_POLXA_LTR_GX_THRESH              (0x000001FF)      ///< LTR Threshold for Clock Source Group X, [8:0]
#define R_PCH_PWRM_3D0                                      0x3D0
#define R_PCH_PWRM_CPPM_MPG_POL1A                           0x3E0             ///< CPPM ModPHY Gating Policy Reg 1A
#define B_PCH_PWRM_CPPM_MPG_POL1A_CPPM_MODPHY_QUAL          BIT30             ///< CPPM Shutdown Qualifier Enable for ModPHY
#define B_PCH_PWRM_CPPM_MPG_POL1A_LT_MODPHY_SEL             BIT29             ///< ASLT/PLT Selection for ModPHY
#define B_PCH_PWRM_CPPM_MPG_POL1A_LTR_MODPHY_THRESH         (0x000001FF)      ///< LTR Threshold for ModPHY, [8:0]
#define R_PCH_PWRM_CS_SD_CTL1                               0x3E8             ///< Clock Source Shutdown Control Reg 1
#define B_PCH_PWRM_CS_SD_CTL1_CS5_CTL_CFG                   (BIT22 | BIT21 | BIT20)    ///< Clock Source 5 Control Configuration
#define N_PCH_PWRM_CS_SD_CTL1_CS5_CTL_CFG                   20
#define B_PCH_PWRM_CS_SD_CTL1_CS1_CTL_CFG                   (BIT2 | BIT1 | BIT0)       ///< Clock Source 1 Control Configuration
#define N_PCH_PWRM_CS_SD_CTL1_CS1_CTL_CFG                   0
#define R_PCH_PWRM_CS_SD_CTL2                               0x3EC             ///< Clock Source Shutdown Control Reg 2
#define R_PCH_PWRM_HSWPGCR1                                 0x5D0
#define B_PCH_PWRM_SW_PG_CTRL_LOCK                          BIT31
#define B_PCH_PWRM_DFX_SW_PG_CTRL                           BIT0
#define R_PCH_PWRM_600                                      0x600
#define R_PCH_PWRM_604                                      0x604
#define R_PCH_PWRM_ST_PG_FDIS_PMC_1                         0x620 ///< Static PG Related Function Disable Register 1
#define B_PCH_PWRM_ST_PG_FDIS_PMC_1_ST_FDIS_LK              BIT31 ///< Static Function Disable Lock (ST_FDIS_LK)
#define B_PCH_PWRM_ST_PG_FDIS_PMC_1_CAM_FDIS_PMC            BIT6  ///< Camera Function Disable (PMC Version) (CAM_FDIS_PMC)
#define B_PCH_PWRM_ST_PG_FDIS_PMC_1_ISH_FDIS_PMC            BIT5  ///< SH Function Disable (PMC Version) (ISH_FDIS_PMC)
#define B_PCH_PWRM_ST_PG_FDIS_PMC_1_GBE_FDIS_PMC            BIT0  ///< GBE Function Disable (PMC Version) (GBE_FDIS_PMC)
#define R_PCH_PWRM_ST_PG_FDIS_PMC_2                         0x624 ///< Static Function Disable Control Register 2
#define V_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_FDIS_PMC       0x7FF ///< Static Function Disable Control Register 2
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_GSPI1_FDIS_PMC BIT10 ///< SerialIo Controller GSPI Device 1 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_GSPI0_FDIS_PMC BIT9  ///< SerialIo Controller GSPI Device 0 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART2_FDIS_PMC BIT8  ///< SerialIo Controller UART Device 2 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART1_FDIS_PMC BIT7  ///< SerialIo Controller UART Device 1 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_UART0_FDIS_PMC BIT6  ///< SerialIo Controller UART Device 0 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C5_FDIS_PMC  BIT5  ///< SerialIo Controller I2C Device 5 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C4_FDIS_PMC  BIT4  ///< SerialIo Controller I2C Device 4 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C3_FDIS_PMC  BIT3  ///< SerialIo Controller I2C Device 3 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C2_FDIS_PMC  BIT2  ///< SerialIo Controller I2C Device 2 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C1_FDIS_PMC  BIT1  ///< SerialIo Controller I2C Device 1 Function Disable
#define B_PCH_PWRM_ST_PG_FDIS_PMC_2_SERIALIO_I2C0_FDIS_PMC  BIT0  ///< SerialIo Controller I2C Device 0 Function Disable
#define R_PCH_PWRM_NST_PG_FDIS_1                            0x628
#define B_PCH_PWRM_NST_PG_FDIS_1_SCC_FDIS_PMC               BIT25 ///< SCC Function Disable. This is only avaiable in B0 onward.
#define B_PCH_PWRM_NST_PG_FDIS_1_XDCI_FDIS_PMC              BIT24 ///< XDCI Function Disable. This is only avaiable in B0 onward.
#define B_PCH_PWRM_NST_PG_FDIS_1_ADSP_FDIS_PMC              BIT23 ///< ADSP Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_SATA_FDIS_PMC              BIT22 ///< SATA Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_C3_FDIS_PMC           BIT13 ///< PCIe Controller C Port 3 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_C2_FDIS_PMC           BIT12 ///< PCIe Controller C Port 2 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_C1_FDIS_PMC           BIT11 ///< PCIe Controller C Port 1 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_C0_FDIS_PMC           BIT10 ///< PCIe Controller C Port 0 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_B3_FDIS_PMC           BIT9  ///< PCIe Controller B Port 3 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_B2_FDIS_PMC           BIT8  ///< PCIe Controller B Port 2 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_B1_FDIS_PMC           BIT7  ///< PCIe Controller B Port 1 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_B0_FDIS_PMC           BIT6  ///< PCIe Controller B Port 0 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_A3_FDIS_PMC           BIT5  ///< PCIe Controller A Port 3 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_A2_FDIS_PMC           BIT4  ///< PCIe Controller A Port 2 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_A1_FDIS_PMC           BIT3  ///< PCIe Controller A Port 1 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_PCIE_A0_FDIS_PMC           BIT2  ///< PCIe Controller A Port 0 Function Disable
#define B_PCH_PWRM_NST_PG_FDIS_1_XHCI_FDIS_PMC              BIT0  ///< XHCI Function Disable
#define R_PCH_PWRM_FUSE_DIS_RD_1                            0x640 ///< Fuse Disable Read 1 Register
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_E3_FUSE_DIS           BIT21 ///< PCIe Controller E Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_E2_FUSE_DIS           BIT20 ///< PCIe Controller E Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_E1_FUSE_DIS           BIT19 ///< PCIe Controller E Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_E0_FUSE_DIS           BIT18 ///< PCIe Controller E Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_D3_FUSE_DIS           BIT17 ///< PCIe Controller D Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_D2_FUSE_DIS           BIT16 ///< PCIe Controller D Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_D1_FUSE_DIS           BIT15 ///< PCIe Controller D Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_D0_FUSE_DIS           BIT14 ///< PCIe Controller D Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_C3_FUSE_DIS           BIT13 ///< PCIe Controller C Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_C2_FUSE_DIS           BIT12 ///< PCIe Controller C Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_C1_FUSE_DIS           BIT11 ///< PCIe Controller C Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_C0_FUSE_DIS           BIT10 ///< PCIe Controller C Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_B3_FUSE_DIS           BIT9  ///< PCIe Controller B Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_B2_FUSE_DIS           BIT8  ///< PCIe Controller B Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_B1_FUSE_DIS           BIT7  ///< PCIe Controller B Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_B0_FUSE_DIS           BIT6  ///< PCIe Controller B Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_A3_FUSE_DIS           BIT5  ///< PCIe Controller A Port 3 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_A2_FUSE_DIS           BIT4  ///< PCIe Controller A Port 2 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_A1_FUSE_DIS           BIT3  ///< PCIe Controller A Port 1 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_PCIE_A0_FUSE_DIS           BIT2  ///< PCIe Controller A Port 0 Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_1_XHCI_FUSE_DIS              BIT0  ///< XHCI Fuse Disable
#define R_PCH_PWRM_FUSE_DIS_RD_2                            0x644 ///< Fuse Disable Read 2 Register
#define B_PCH_PWRM_FUSE_DIS_RD_2_SPC_SS_DIS                 BIT25 ///< SPC Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SPB_SS_DIS                 BIT24 ///< SPB Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SPA_SS_DIS                 BIT23 ///< SPA Fuse Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_PSTH_FUSE_SS_DIS           BIT21 ///< PSTH Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_DMI_FUSE_SS_DIS            BIT20 ///< DMI Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_OTG_FUSE_SS_DIS            BIT19 ///< OTG Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_XHCI_SS_DIS                BIT18 ///< XHCI Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_FIA_FUSE_SS_DIS            BIT17 ///< FIA Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_DSP_FUSE_SS_DIS            BIT16 ///< DSP Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SATA_FUSE_SS_DIS           BIT15 ///< SATA Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_ICC_FUSE_SS_DIS            BIT14 ///< ICC Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_LPC_FUSE_SS_DIS            BIT13 ///< LPC Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_RTC_FUSE_SS_DIS            BIT12 ///< RTC Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_P2S_FUSE_SS_DIS            BIT11 ///< P2S Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_TRSB_FUSE_SS_DIS           BIT10 ///< TRSB Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SMB_FUSE_SS_DIS            BIT9  ///< SMB Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_ITSS_FUSE_SS_DIS           BIT8  ///< ITSS Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SERIALIO_FUSE_SS_DIS       BIT6  ///< SerialIo Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_SCC_FUSE_SS_DIS            BIT4  ///< SCC Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_P2D_FUSE_SS_DIS            BIT3  ///< P2D Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_CAM_FUSE_SS_DIS            BIT2  ///< Camera Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_ISH_FUSE_SS_DIS            BIT1  ///< ISH Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_2_GBE_FUSE_SS_DIS            BIT0  ///< GBE Fuse or Soft Strap Disable
#define R_PCH_PWRM_FUSE_DIS_RD_3                            0x648 ///< Static PG Fuse and Soft Strap Disable Read Register 3
#define B_PCH_PWRM_FUSE_DIS_RD_3_PNCRA3_FUSE_SS_DIS         BIT3  ///< PNCRA3 Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_3_PNCRA2_FUSE_SS_DIS         BIT2  ///< PNCRA2 Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_3_PNCRA1_FUSE_SS_DIS         BIT1  ///< PNCRA1 Fuse or Soft Strap Disable
#define B_PCH_PWRM_FUSE_DIS_RD_3_PNCRA_FUSE_SS_DIS          BIT0  ///< PNCRA Fuse or Soft Strap Disable


#endif


//========================================================================================
//   edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/PchReservedResources.h
//   edk2-platforms/Silicon/Intel/PurleyRefreshSiliconPkg/Pch/Include/PchReservedResources.h
//   edk2-platforms/Silicon/Intel/SimicsIch10Pkg/Include/PchReservedResources.h
//   edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/PchReservedResources.h
// = edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Fru/TglPch/Include/PchReservedResources.h


/** @file
  PCH preserved MMIO resource definitions.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_PRESERVED_RESOURCES_H_
#define _PCH_PRESERVED_RESOURCES_H_

/**
  Detailed recommended static allocation
  +-------------------------------------------------------------------------+
  | PCH preserved MMIO range, 32 MB, from 0xFC800000 to 0xFE7FFFFF          |
  +-------------------------------------------------------------------------+
  | Size        | Start       | End         | Usage                         |
  | 8 MB        | 0xFC800000  | 0xFCFFFFFF  | TraceHub SW BAR               |
  | 16 MB       | 0xFD000000  | 0xFDFFFFFF  | SBREG                         |
  | 64 KB       | 0xFE000000  | 0xFE00FFFF  | PMC MBAR                      |
  | 4 KB        | 0xFE010000  | 0xFE010FFF  | SPI BAR0                      |
  | 176 KB      | 0xFE020000  | 0xFE04BFFF  | SerialIo BAR in ACPI mode     |
  | 400 KB      | 0xFE04C000  | 0xFE0AFFFF  | Unused                        |
  | 64 KB       | 0xFE0B0000  | 0xFE0BFFFF  | eSPI LGMR BAR                 |
  | 64 KB       | 0xFE0C0000  | 0xFE0CFFFF  | eSPI2 SEGMR BAR               |
  | 192 KB      | 0xFE0D0000  | 0xFE0FFFFF  | Unused                        |
  | 1 MB        | 0xFE100000  | 0xFE1FFFFF  | TraceHub MTB BAR              |
  | 2 MB        | 0xFE200000  | 0xFE3FFFFF  | TraceHub FW BAR               |
  | 2 MB        | 0xFE400000  | 0xFE5FFFFF  | Unused                        |
  | 2 MB        | 0xFE600000  | 0xFE7FFFFF  | Temp address                  |
  +-------------------------------------------------------------------------+
**/
#define PCH_PRESERVED_BASE_ADDRESS      0xFC800000     ///< Pch preserved MMIO base address
#define PCH_PRESERVED_MMIO_SIZE         0x02000000     ///< 32MB
#define PCH_TRACE_HUB_SW_BASE_ADDRESS   0xFC800000     ///< TraceHub SW MMIO base address
#define PCH_TRACE_HUB_SW_MMIO_SIZE      0x00800000     ///< 8MB
#define PCH_PCR_BASE_ADDRESS            0xFD000000     ///< SBREG MMIO base address
#define PCH_PCR_MMIO_SIZE               0x01000000     ///< 16MB
#define PCH_PWRM_BASE_ADDRESS           0xFE000000     ///< PMC MBAR MMIO base address
#define PCH_PWRM_MMIO_SIZE              0x00010000     ///< 64KB
#define PCH_SPI_BASE_ADDRESS            0xFE010000     ///< SPI BAR0 MMIO base address
#define PCH_SPI_MMIO_SIZE               0x00001000     ///< 4KB
#define PCH_SERIAL_IO_BASE_ADDRESS      0xFE020000     ///< SerialIo MMIO base address
#define PCH_SERIAL_IO_MMIO_SIZE         0x0002C000     ///< 176KB
#define PCH_ESPI_LGMR_BASE_ADDRESS      0xFE0B0000     ///< eSPI LGMR MMIO base address
#define PCH_ESPI_LGMR_MMIO_SIZE         0x00010000     ///< 64KB
#define PCH_ESPI_SEGMR_BASE_ADDRESS     0xFE0C0000     ///< Second eSPI GMR MMIO base address
#define PCH_ESPI_SEGMR_MMIO_SIZE        0x00010000     ///< 64KB
#define PCH_TRACE_HUB_MTB_BASE_ADDRESS  0xFE100000     ///< TraceHub MTB MMIO base address
#define PCH_TRACE_HUB_MTB_MMIO_SIZE     0x00100000     ///< 1MB
#define PCH_TRACE_HUB_FW_BASE_ADDRESS   0xFE200000     ///< TraceHub FW MMIO base address
#define PCH_TRACE_HUB_FW_MMIO_SIZE      0x00200000     ///< 2MB
#define PCH_TEMP_BASE_ADDRESS           0xFE600000     ///< preserved temp address for misc usage,
#define PCH_TEMP_MMIO_SIZE              0x00200000     ///< 2MB

#endif // _PCH_PRESERVED_RESOURCES_H_


//========================================================================================
//  edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/PchAccess.h
//  edk2-platforms/Silicon/Intel/PurleyRefreshSiliconPkg/Pch/Include/PchAccess.h
//  edk2-platforms/Silicon/Intel/SimicsIch10Pkg/Include/PchAccess.h
// = edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/PchAccess.h

/** @file
  Macros that simplify accessing PCH devices's PCI registers.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_ACCESS_H_
#define _PCH_ACCESS_H_

//#include "PchLimits.h"
//#include "PchReservedResources.h"

#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND 1
#endif
#ifndef STALL_ONE_SECOND
#define STALL_ONE_SECOND 1000000
#endif

//
// Include device register definitions
//
//#include "PcieRegs.h"
//#include "Register/PchRegs.h"
//#include "Register/PchRegsPcr.h"
//#include "Register/PchRegsP2sb.h"
//#include "Register/PchRegsHda.h"
//#include "Register/PchRegsHsio.h"
//#include "Register/PchRegsLan.h"
//#include "Register/PchRegsLpc.h"
//#include "Register/PchRegsPmc.h"
//#include "Register/PchRegsPcie.h"
//#include "Register/PchRegsSata.h"
//#include "Register/PchRegsSmbus.h"
//#include "Register/PchRegsSpi.h"
//#include <Register/RegsUsb.h>
//#include "Register/PchRegsGpio.h"
//#include "Register/PchRegsThermalCnl.h"
//#include "Register/PchRegsGpioCnl.h"
//#include "Register/PchRegsSerialIoCnl.h"
//#include "Register/PchRegsSerialIo.h"
//#include "Register/PchRegsTraceHub.h"
//#include "Register/PchRegsScsCnl.h"
//#include "Register/PchRegsIsh.h"
//#include "Register/PchRegsDmi.h"
//#include "Register/PchRegsItss.h"
//#include "Register/PchRegsPsth.h"
//#include "Register/PchRegsFia.h"
//#include "Register/PchRegsDci.h"

#endif


//========================================================================================
//   edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/Library/PchPcrLib.h
//   edk2-platforms/Silicon/Intel/PurleyRefreshSiliconPkg/Pch/Include/Library/PchPcrLib.h
// = edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/Library/PchPcrLib.h
// = edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Include/Library/PchPcrLib.h


/** @file
  Header file for PchPcrLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_PCR_LIB_H_
#define _PCH_PCR_LIB_H_

//#include <PchReservedResources.h>

/**
  Definition for PCR address
  The PCR address is used to the PCR MMIO programming

  SBREG_BAR_20BITADDRESS is configured by SoC

  SBREG_BAR_20BITADDRESS=1, the format has included 16b addressing.
  +---------------------------------------------------------------------------------------------+
  | Addr[63:28]    | Addr[27:24]           | Addr[23:16]     | Addr[15:2]           | Addr[1:0] |
  +----------------+-----------------------+-----------------+----------------------------------+
  | REG_BAR[63:28] | TargetRegister[19:16] | TargetPort[7:0] | TargetRegister[15:2]             |
  +---------------------------------------------------------------------------------------------+

  SBREG_BAR_20BITADDRESS=0
  +---------------------------------------------------------------------------------------------+
  | Addr[63:24]    | Addr[27:24]           | Addr[23:16]     | Addr[15:2]           | Addr[1:0] |
  +----------------+-----------------------+-----------------+----------------------------------+
  | REG_BAR[63:24] | REG_BAR[27:24]        | TargetPort[7:0] | TargetRegister[15:2]             |
  +---------------------------------------------------------------------------------------------+
**/
#define PCH_PCR_ADDRESS(Pid, Offset)    (PCH_PCR_BASE_ADDRESS | (UINT32) (((Offset) & 0x0F0000) << 8) | ((UINT8)(Pid) << 16) | (UINT16) ((Offset) & 0xFFFF))

/**
  PCH PCR boot script accessing macro
  Those macros are only available for DXE phase.
**/
#define PCH_PCR_BOOT_SCRIPT_WRITE(Width, Pid, Offset, Count, Buffer) \
          S3BootScriptSaveMemWrite (Width, PCH_PCR_ADDRESS (Pid, Offset), Count, Buffer); \

#define PCH_PCR_BOOT_SCRIPT_READ_WRITE(Width, Pid, Offset, DataOr, DataAnd) \
          S3BootScriptSaveMemReadWrite (Width, PCH_PCR_ADDRESS (Pid, Offset), DataOr, DataAnd); \

#define PCH_PCR_BOOT_SCRIPT_READ(Width, Pid, Offset, BitMask, BitValue) \
          S3BootScriptSaveMemPoll (Width, PCH_PCR_ADDRESS (Pid, Offset), BitMask, BitValue, 1, 1);

typedef UINT8          PCH_SBI_PID;

/**
  Read PCR register.
  It returns PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT32       PCR register value.
**/
UINT32
PchPcrRead32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset
  );

/**
  Read PCR register.
  It returns PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT16       PCR register value.
**/
UINT16
PchPcrRead16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset
  );

/**
  Read PCR register.
  It returns PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of this Port ID

  @retval UINT8        PCR register value
**/
UINT8
PchPcrRead8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset
  );

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval UINT32       Value written to register
**/
UINT32
PchPcrWrite32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval  UINT16      Value written to register
**/
UINT16
PchPcrWrite16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT16                            InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  Data     Input Data. Must be the same size as Size parameter.

  @retval  UINT8       Value written to register
**/
UINT8
PchPcrWrite8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT8                             InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT32      Value written to register

**/
UINT32
PchPcrAndThenOr32 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/**
  Write PCR register and read back.
  The read back ensures the PCR cycle is completed before next operation.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT32      Value read back from the register
**/
UINT32
PchPcrAndThenOr32WithReadback (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval UINT16       Value written to register

**/
UINT16
PchPcrAndThenOr16 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT16                            AndData,
  IN  UINT16                            OrData
  );

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

  @retval  UINT8       Value written to register

**/
UINT8
PchPcrAndThenOr8 (
  IN  PCH_SBI_PID                       Pid,
  IN  UINT32                            Offset,
  IN  UINT8                             AndData,
  IN  UINT8                             OrData
  );


typedef enum {
  PchIpDmi = 1,
  PchIpIclk,
} PCH_IP_PID_ENUM;

#define PCH_INVALID_PID 0

/**
  Get PCH IP PID number

  @param[in]  IpEnum   PCH IP in PCH_IP_PID_ENUM

  @retval     0        PID of this IP is not supported
              !0       PID of the IP.
**/
PCH_SBI_PID
PchPcrGetPid (
  PCH_IP_PID_ENUM  IpEnum
  );

#endif // _PCH_PCR_LIB_H_


//========================================================================================
//   edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/Library/PchPcieRpLib.h
//   edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/Library/PchPcieRpLib.h
// = edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Include/Library/PchPcieRpLib.h

/** @file
  Header file for PchPcieRpLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_PCIERP_LIB_H_
#define _PCH_PCIERP_LIB_H_

//#include <Library/PchPcrLib.h>

/**
  PCIe controller bifurcation configuration.
**/
typedef enum {
  PcieBifurcationDefault = 0,
  PcieBifurcation4x1,
  PcieBifurcation1x2_2x1,
  PcieBifurcation2x2,
  PcieBifurcation1x4,
  PcieBifurcation4x2,
  PcieBifurcation1x4_2x2,
  PcieBifurcation2x2_1x4,
  PcieBifurcation2x4,
  PcieBifurcation1x8,
  PcieBifurcationUnknown,
  PcieBifurcationMax
} PCIE_BIFURCATION_CONFIG;

/**
  This function returns PID according to PCIe controller index

  @param[in] ControllerIndex     PCIe controller index

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
PchGetPcieControllerSbiPid (
  IN UINT32  ControllerIndex
  );

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
GetRpSbiPid (
  IN UINTN  RpIndex
  );

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber            Root port physical number. (0-based)
  @param[out] RpDev               Return corresponding root port device number.
  @param[out] RpFun               Return corresponding root port function number.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );

/**
  Get Root Port physical Number by Pch Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)

  @retval     EFI_SUCCESS           Physical root port is retrieved
  @retval     EFI_INVALID_PARAMETER RpDev and/or RpFun are invalid
  @retval     EFI_UNSUPPORTED       Root port device and function is not assigned to any physical root port
**/
EFI_STATUS
EFIAPI
GetPchPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  );

/**
  Gets pci segment base address of PCIe root port.

  @param RpIndex    Root Port Index (0 based)
  @return PCIe port base address.
**/
UINT64
PchPcieBase (
  IN  UINT32   RpIndex
  );

/**
  Determines whether L0s is supported on current stepping.

  @return TRUE if L0s is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieL0sSupported (
  VOID
  );

/**
  Some early PCH steppings require Native ASPM to be disabled due to hardware issues:
   - RxL0s exit causes recovery
   - Disabling PCIe L0s capability disables L1
  Use this function to determine affected steppings.

  @return TRUE if Native ASPM is supported, FALSE otherwise
**/
BOOLEAN
PchIsPcieNativeAspmSupported (
  VOID
  );
#endif // _PCH_PCIERP_LIB_H_


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/PciHotPlug/PciHotPlug.c
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/PciHotPlug/PciHotPlug.c
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/PciHotPlug/PciHotPlug.c

/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCI_HOT_PLUG_H_
#define _PCI_HOT_PLUG_H_

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
//#include <Base.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>
//#include <Library/DebugLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/MemoryAllocationLib.h>
//#include <IndustryStandard/Acpi10.h>
//#include <Protocol/PciHotPlugInit.h>
//#include <Protocol/PciRootBridgeIo.h>
//#include <Library/DevicePathLib.h>
//#include <Library/UefiLib.h>
//#include <Guid/HobList.h>
//#include <Library/HobLib.h>
//#include <Protocol/SaPolicy.h>

#define PCI_HOT_PLUG_DRIVER_PRIVATE_SIGNATURE SIGNATURE_32 ('G', 'U', 'L', 'P')

#define ACPI \
  { \
    { ACPI_DEVICE_PATH, ACPI_DP, { (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)), (UINT8) \
      ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8) } }, EISA_PNP_ID (0x0A03), 0 \
  }

#define PCI(device, function) \
  { \
    { HARDWARE_DEVICE_PATH, HW_PCI_DP, { (UINT8) (sizeof (PCI_DEVICE_PATH)), (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8) } }, \
      (UINTN) function, (UINTN) device \
  }

#define END \
  { \
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, { END_DEVICE_PATH_LENGTH, 0 } \
  }

#define LPC(eisaid, function) \
  { \
    { ACPI_DEVICE_PATH, ACPI_DP, { (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)), (UINT8) \
      ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8) } }, EISA_PNP_ID (eisaid), function \
  }

typedef struct PCIE_HOT_PLUG_DEVICE_PATH {
  ACPI_HID_DEVICE_PATH      PciRootBridgeNode;
  PCI_DEVICE_PATH           PciRootPortNode;
  EFI_DEVICE_PATH_PROTOCOL  EndDeviceNode;
} PCIE_HOT_PLUG_DEVICE_PATH;

typedef struct {
  UINTN                           Signature;
  EFI_HANDLE                      Handle; // Handle for protocol this driver installs on
  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  HotPlugInitProtocol;
} PCI_HOT_PLUG_INSTANCE;

/**
  This procedure returns a list of Root Hot Plug controllers that require
  initialization during boot process

  @param[in]  This      The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol.
  @param[out] HpcCount  The number of Root HPCs returned.
  @param[out] HpcList   The list of Root HPCs. HpcCount defines the number of elements in this list.

  @retval EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
GetRootHpcList (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL    *This,
  OUT UINTN                             *PhpcCount,
  OUT EFI_HPC_LOCATION                  **PhpcList
  );

/**
  This procedure Initializes one Root Hot Plug Controller
  This process may casue initialization of its subordinate buses

  @param[in]  This            The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol.
  @param[in]  HpcDevicePath   The Device Path to the HPC that is being initialized.
  @param[in]  HpcPciAddress   The address of the Hot Plug Controller function on the PCI bus.
  @param[in]  Event           The event that should be signaled when the Hot Plug Controller initialization is complete. Set to NULL if the caller wants to wait until the entire initialization process is complete. The event must be of the type EFI_EVT_SIGNAL.
  @param[out] HpcState        The state of the Hot Plug Controller hardware. The type EFI_Hpc_STATE is defined in section 3.1.

  @retval   EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
InitializeRootHpc (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  *This,
  IN  EFI_DEVICE_PATH_PROTOCOL        *PhpcDevicePath,
  IN  UINT64                          PhpcPciAddress,
  IN  EFI_EVENT                       Event, OPTIONAL
  OUT EFI_HPC_STATE                   *PhpcState
  );

/**
  Returns the resource padding required by the PCI bus that is controlled by the specified Hot Plug Controller.

  @param[in]  This           The pointer to the instance of the EFI_PCI_HOT_PLUG_INIT protocol. initialized.
  @param[in]  HpcDevicePath  The Device Path to the Hot Plug Controller.
  @param[in]  HpcPciAddress  The address of the Hot Plug Controller function on the PCI bus.
  @param[out] HpcState       The state of the Hot Plug Controller hardware. The type EFI_HPC_STATE is defined in section 3.1.
  @param[out] Padding        This is the amount of resource padding required by the PCI bus under the control of the specified Hpc. Since the caller does not know the size of this buffer, this buffer is allocated by the callee and freed by the caller.
  @param[out] Attribute      Describes how padding is accounted for.

  @retval     EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
GetResourcePadding (
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL  *This,
  IN  EFI_DEVICE_PATH_PROTOCOL        *PhpcDevicePath,
  IN  UINT64                          PhpcPciAddress,
  OUT EFI_HPC_STATE                   *PhpcState,
  OUT VOID                            **Padding,
  OUT EFI_HPC_PADDING_ATTRIBUTES      *Attributes
  );

#endif


//========================================================================================
//   /Volumes/Work/Programming/EFIProjects/edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Include/PcieRegs.h
//   /Volumes/Work/Programming/EFIProjects/edk2-platforms/Silicon/Intel/PurleyRefreshSiliconPkg/Pch/Include/PcieRegs.h
//   /Volumes/Work/Programming/EFIProjects/edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Include/PcieRegs.h
// = /Volumes/Work/Programming/EFIProjects/edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Include/PcieRegs.h


/** @file
  Register names for PCIE standard register

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCIE_REGS_H_
#define _PCIE_REGS_H_

//#include <IndustryStandard/Pci30.h>

//
// PCI type 0 Header
//
#define R_PCI_BCC_OFFSET                          0x0B

//
// PCI type 1 Header
//
#define R_PCI_BRIDGE_BNUM                         0x18 ///< Bus Number Register
#define B_PCI_BRIDGE_BNUM_SBBN                    0x00FF0000 ///< Subordinate Bus Number
#define B_PCI_BRIDGE_BNUM_SCBN                    0x0000FF00 ///< Secondary Bus Number

//
// PCI Express Capability List Register (CAPID:10h)
//
#define R_PCIE_XCAP_OFFSET                        0x02 ///< PCI Express Capabilities Register (Offset 02h)
#define B_PCIE_XCAP_DT                            (BIT7 | BIT6 | BIT5 | BIT4) ///< Device/Port Type
#define N_PCIE_XCAP_DT                            4

#define R_PCIE_DCAP_OFFSET                        0x04 ///< Device Capabilities Register (Offset 04h)
#define B_PCIE_DCAP_RBER                          BIT15 ///< Role-Based Error Reporting
#define B_PCIE_DCAP_E1AL                          (BIT11 | BIT10 | BIT9) ///< Endpoint L1 Acceptable Latency
#define N_PCIE_DCAP_E1AL                          9
#define B_PCIE_DCAP_E0AL                          (BIT8 | BIT7 | BIT6) ///< Endpoint L0s Acceptable Latency
#define N_PCIE_DCAP_E0AL                          6
#define B_PCIE_DCAP_MPS                           (BIT2 | BIT1 | BIT0) ///< Max_Payload_Size Supported

#define R_PCIE_DCTL_OFFSET                        0x08 ///< Device Control Register (Offset 08h)
#define B_PCIE_DCTL_MPS                           (BIT7 | BIT6 | BIT5) ///< Max_Payload_Size
#define N_PCIE_DCTL_MPS                           5

#define R_PCIE_LCAP_OFFSET                        0x0C ///< Link Capabilities Register (Offset 0Ch)
#define B_PCIE_LCAP_CPM                           BIT18 ///< Clock Power Management
#define B_PCIE_LCAP_EL1                           (BIT17 | BIT16 | BIT15) ///< L1 Exit Latency
#define N_PCIE_LCAP_EL1                           15
#define B_PCIE_LCAP_EL0                           (BIT14 | BIT13 | BIT12) ///< L0s Exit Latency
#define N_PCIE_LCAP_EL0                           12
#define B_PCIE_LCAP_APMS_L0S                      BIT10
#define B_PCIE_LCAP_APMS_L1                       BIT11
#define B_PCIE_LCAP_MLS                           (BIT3 | BIT2 | BIT1 | BIT0) ///< Max Link Speed
#define V_PCIE_LCAP_MLS_GEN3                      3
#define V_PCIE_LCAP_MLS_GEN4                      4

#define R_PCIE_LCTL_OFFSET                        0x10 ///< Link Control Register (Offset 10h)
#define B_PCIE_LCTL_ECPM                          BIT8 ///< Enable Clock Power Management
#define B_PCIE_LCTL_CCC                           BIT6 ///< Common Clock Configuration
#define B_PCIE_LCTL_RL                            BIT5 ///< Retrain Link
#define B_PCIE_LCTL_ASPM                          (BIT1 | BIT0) ///< Active State Power Management (ASPM) Control
#define V_PCIE_LCTL_ASPM_L0S                      1
#define V_PCIE_LCTL_ASPM_L1                       2
#define V_PCIE_LCTL_ASPM_L0S_L1                   3

#define R_PCIE_LSTS_OFFSET                        0x12 ///< Link Status Register (Offset 12h)
#define B_PCIE_LSTS_LA                            BIT13 ///< Data Link Layer Link Active
#define B_PCIE_LSTS_SCC                           BIT12 ///< Slot Clock Configuration
#define B_PCIE_LSTS_LT                            BIT11 ///< Link Training
#define B_PCIE_LSTS_NLW                           0x03F0 ///< Negotiated Link Width
#define N_PCIE_LSTS_NLW                           4
#define B_PCIE_LSTS_CLS                           0x000F ///< Current Link Speed

#define R_PCIE_SLCAP_OFFSET                       0x14 ///< Slot Capabilities Register (Offset 14h)
#define B_PCIE_SLCAP_HPC                          BIT6       ///< Hot-Plug Capable

#define R_PCIE_SLSTS_OFFSET                       0x1A ///< Slot Status Register (Offset 1Ah)
#define B_PCIE_SLSTS_PDS                          BIT6 ///< Presence Detect State

#define R_PCIE_DCAP2_OFFSET                       0x24 ///< Device Capabilities 2 Register (Offset 24h)
#define B_PCIE_DCAP2_LTRMS                        BIT11 ///< LTR Mechanism Supported

#define R_PCIE_DCTL2_OFFSET                       0x28 ///< Device Control 2 Register (Offset 28h)
#define B_PCIE_DCTL2_LTREN                        BIT10 ///< LTR Mechanism Enable

#define B_PCIE_LCTL2_TLS                          (BIT3 | BIT2 | BIT1 | BIT0) ///< Target Link Speed

//
// Latency Tolerance Reporting Extended Capability Registers (CAPID:0018h)
//
#define R_PCIE_LTRECH_CID                         0x0018

#define R_PCIE_LTRECH_MSLR_OFFSET                 0x04
#define N_PCIE_LTRECH_MSLR_VALUE                  0
#define N_PCIE_LTRECH_MSLR_SCALE                  10

#define R_PCIE_LTRECH_MNSLR_OFFSET                0x06
#define N_PCIE_LTRECH_MNSLR_VALUE                 0
#define N_PCIE_LTRECH_MNSLR_SCALE                 10

//
// Secondary PCI Express Extended Capability Header (CAPID:0019h)
//
#define R_PCIE_EX_LCTL3_OFFSET                    0x04 ///< Link Control 3 Register
#define B_PCIE_EX_LCTL3_PE                        BIT0 ///< Perform Equalization

//
// L1 Sub-States Extended Capability Register (CAPID:001Eh)
//
#define V_PCIE_EX_L1S_CID                        0x001E ///< Capability ID
#define R_PCIE_EX_L1SCAP_OFFSET                  0x04 ///< L1 Sub-States Capabilities
#define B_PCIE_EX_L1SCAP_PTV                     0x00F80000 //< Port Tpower_on value
#define N_PCIE_EX_L1SCAP_PTV                     19
#define B_PCIE_EX_L1SCAP_PTPOS                   0x00030000 //< Port Tpower_on scale
#define N_PCIE_EX_L1SCAP_PTPOS                   16
#define B_PCIE_EX_L1SCAP_CMRT                    0x0000FF00 //< Common Mode Restore time
#define N_PCIE_EX_L1SCAP_CMRT                    8
#define B_PCIE_EX_L1SCAP_L1PSS                   BIT4 ///< L1 PM substates supported
#define B_PCIE_EX_L1SCAP_AL1SS                   BIT3 ///< ASPM L1.1 supported
#define B_PCIE_EX_L1SCAP_AL12S                   BIT2 ///< ASPM L1.2 supported
#define B_PCIE_EX_L1SCAP_PPL11S                  BIT1 ///< PCI-PM L1.1 supported
#define B_PCIE_EX_L1SCAP_PPL12S                  BIT0 ///< PCI-PM L1.2 supported
#define R_PCIE_EX_L1SCTL1_OFFSET                 0x08 ///< L1 Sub-States Control 1
#define B_PCIE_EX_L1SCTL1_L1SSEIE                BIT4
#define N_PCIE_EX_L1SCTL1_L12LTRTLSV             29
#define N_PCIE_EX_L1SCTL1_L12LTRTLV              16
#define R_PCIE_EX_L1SCTL2_OFFSET                 0x0C ///< L1 Sub-States Control 2
#define N_PCIE_EX_L1SCTL2_POWT                   3

//
// PTM Extended Capability Register (CAPID:001Fh)
//
#define V_PCIE_EX_PTM_CID                         0x001F ///< Capability ID
#define R_PCIE_EX_PTMCAP_OFFSET                   0x04 ///< PTM Capabilities
#define R_PCIE_EX_PTMCTL_OFFSET                   0x08 ///< PTM Control Register

//
// Base Address Offset
//
#define B_PCI_BAR_MEMORY_TYPE_MASK                (BIT1 | BIT2)
#define B_PCI_BAR_MEMORY_TYPE_64                  BIT2

//
// PCI Express Extended Capability Header
//
#define R_PCIE_CFG_EXCAP_OFFSET                   0x100

#endif


//========================================================================================
// edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/Register/PchRegsLpc.h

/** @file
  Register names for PCH LPC/eSPI device

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position
  - In general, PCH registers are denoted by "_PCH_" in register names
  - Registers / bits that are different between PCH generations are denoted by
    "_PCH_[generation_name]_" in register/bit names.
  - Registers / bits that are specific to PCH-H denoted by "_H_" in register/bit names.
    Registers / bits that are specific to PCH-LP denoted by "_LP_" in register/bit names.
    e.g., "_PCH_H_", "_PCH_LP_"
    Registers / bits names without _H_ or _LP_ apply for both H and LP.
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a PCH generation will be just named
    as "_PCH_" without [generation_name] inserted.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_REGS_LPC_H_
#define _PCH_REGS_LPC_H_

//
// PCI to LPC Bridge Registers (D31:F0)
//
#define PCI_DEVICE_NUMBER_PCH_LPC       31
#define PCI_FUNCTION_NUMBER_PCH_LPC     0

typedef enum {
  PchHB0         = 0x01,
  PchHC0,
  PchHD0,
  PchHD1,
  PchLpB0        = 0x23,
  PchLpB1,
  PchLpC0,
  PchLpC1,
  KblPchHA0      = 0x40,
  PchSteppingMax
} PCH_STEPPING;

#define SKL_PCH_H_MIN_SUPPORTED_STEPPING          PchHB0
#define KBL_PCH_H_MIN_SUPPORTED_STEPPING          KblPchHA0
#define PCH_LP_MIN_SUPPORTED_STEPPING             PchLpB0

#define V_PCH_LPC_VENDOR_ID                       V_PCH_INTEL_VENDOR_ID

//
//
// SKL PCH Server/WS LPC Device IDs
//
#define V_SKL_PCH_H_LPC_DEVICE_ID_SVR_0             0xA149          ///< Server SKU Intel C236 Chipset
#define V_SKL_PCH_H_LPC_DEVICE_ID_SVR_1             0xA14A          ///< Server SKU Intel C232 Chipset
#define V_SKL_PCH_H_LPC_DEVICE_ID_SVR_2             0xA150          ///< Server SKU Intel CM236 Chipset
#define V_SKL_PCH_H_LPC_DEVICE_ID_A14B              0xA14B          ///< Super SKU Unlocked

//
// SKL PCH-H Desktop LPC Device IDs
//
#define V_SKL_PCH_H_LPC_DEVICE_ID_DT_SUPER_SKU      0xA141          ///< PCH H Desktop Super SKU unlocked
#define V_SKL_PCH_H_LPC_DEVICE_ID_DT_0              0xA142          ///< PCH H Desktop Super SKU locked
#define V_SKL_PCH_H_LPC_DEVICE_ID_DT_1              0xA143          ///< PCH H Desktop H110
#define V_SKL_PCH_H_LPC_DEVICE_ID_DT_2              0xA144          ///< PCH H Desktop H170
#define V_SKL_PCH_H_LPC_DEVICE_ID_DT_3              0xA145          ///< PCH H Desktop Z170
#define V_SKL_PCH_H_LPC_DEVICE_ID_DT_4              0xA146          ///< PCH H Desktop Q170
#define V_SKL_PCH_H_LPC_DEVICE_ID_DT_5              0xA147          ///< PCH H Desktop Q150
#define V_SKL_PCH_H_LPC_DEVICE_ID_DT_6              0xA148          ///< PCH H Desktop B150
//
// SKL PCH-H Mobile LPC Device IDs
//
#define V_SKL_PCH_H_LPC_DEVICE_ID_MB_SUPER_SKU    0xA141          ///< PCH H Mobile Super SKU unlocked
#define V_SKL_PCH_H_LPC_DEVICE_ID_MB_0            0xA14D          ///< PCH H Mobile QM170
#define V_SKL_PCH_H_LPC_DEVICE_ID_MB_1            0xA14E          ///< PCH H Mobile HM170
#define V_SKL_PCH_H_LPC_DEVICE_ID_MB_3            0xA151          ///< PCH H Mobile QMS180 (SFF)
#define V_SKL_PCH_H_LPC_DEVICE_ID_MB_4            0xA152          ///< KBL PCH H Mobile HM175
#define V_SKL_PCH_H_LPC_DEVICE_ID_MB_5            0xA153          ///< KBL PCH H Mobile QM175
#define V_SKL_PCH_H_LPC_DEVICE_ID_MB_6            0xA154          ///< KBL PCH H Mobile CM238
#define V_SKL_PCH_H_LPC_DEVICE_ID_MB_8            0xA156          ///< KBL PCH H Mobile QMS185 (SFF)

//
// SKL PCH-LP LPC Device IDs
//
#define V_PCH_LP_LPC_DEVICE_ID_MB_SUPER_SKU       0x9D41          ///< PCH LP Mobile Super SKU unlocked
#define V_PCH_LP_LPC_DEVICE_ID_MB_0               0x9D42          ///< PCH LP Mobile Super SKU locked
#define V_PCH_LP_LPC_DEVICE_ID_MB_1               0x9D43          ///< PCH LP Mobile (U) Base SKU
#define V_PCH_LP_LPC_DEVICE_ID_MB_2               0x9D46          ///< PCH LP Mobile (Y) Premium SKU
#define V_PCH_LP_LPC_DEVICE_ID_MB_3               0x9D48          ///< PCH LP Mobile (U) Premium SKU
#define V_PCH_LP_LPC_DEVICE_ID_MB_SUPER_SKU_1     0x9D51          ///< KBL PCH LP Mobile Super SKU unlocked
#define V_PCH_LP_LPC_DEVICE_ID_MB_4               0x9D52          ///< KBL PCH LP Mobile Super SKU locked
#define V_PCH_LP_LPC_DEVICE_ID_MB_5               0x9D53          ///< KBL PCH LP Mobile (U) Base SKU
#define V_PCH_LP_LPC_DEVICE_ID_MB_6               0x9D56          ///< KBL PCH LP Mobile (Y) Premium SKU
#define V_PCH_LP_LPC_DEVICE_ID_MB_7               0x9D58          ///< KBL PCH LP Mobile (U) Premium SKU
#define V_PCH_LP_LPC_DEVICE_ID_MB_8               0x9D4B          ///< KBL PCH LP Mobile (Y) iHDCP 2.2 Premium
#define V_PCH_LP_LPC_DEVICE_ID_MB_9               0x9D4E          ///< KBL PCH LP Mobile (U) iHDCP 2.2 Premium
#define V_PCH_LP_LPC_DEVICE_ID_MB_10              0x9D50          ///< KBL PCH LP Mobile (U) iHDCP 2.2 Base



//
// KBL PCH Server/WS LPC Device IDs
//
#define V_KBL_PCH_H_LPC_DEVICE_ID_SVR_ES_SUPER_SKU  0xA2D0        ///< ES Super SKU Unlocked. This is SKL-PCH-H in KBL-PCH-H package
#define V_KBL_PCH_H_LPC_DEVICE_ID_SVR_SUPER_SKU     0xA2D1        ///< Super SKU Unlocked
#define V_KBL_PCH_H_LPC_DEVICE_ID_SVR_0             0xA2D2        ///< Server SKU X290
#define V_KBL_PCH_H_LPC_DEVICE_ID_SVR_1             0xA2D3        ///< Server SKU C6xx
#define V_KBL_PCH_H_LPC_DEVICE_ID_SVR_2             0xA2CE        ///< Server SKU ES SuperSKU Server
#define V_KBL_PCH_H_LPC_DEVICE_ID_SVR_3             0xA2CF        ///< Server SKU SuperSKU Server
#define V_KBL_PCH_H_LPC_DEVICE_ID_SVR_4             0xA2D4        ///< Server SKU C422B


//
// KBL PCH-H Desktop LPC Device IDs
//
#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_ES_SUPER_SKU   0xA2C0        ///< PCH H Desktop ES Super SKU unlocked. This is SKL-PCH-H in KBL-PCH-H package
#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_SUPER_SKU      0xA2C1        ///< PCH H Desktop Super SKU unlocked
#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_H270           0xA2C4        ///< PCH H Desktop H270
#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_Z270           0xA2C5        ///< PCH H Desktop Z270
#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_Q270           0xA2C6        ///< PCH H Desktop Q270
#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_Q250           0xA2C7        ///< PCH H Desktop Q250
#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_B250           0xA2C8        ///< PCH H Desktop B250

#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_H215         0xA2C3          ///< PCH H Desktop H215
#define V_KBL_PCH_H_LPC_DEVICE_ID_DT_Z370         0xA2C9           ///< PCH H Desktop Z370

#define V_PCH_LPC_RID_0                           0x00
#define V_PCH_LPC_RID_1                           0x01
#define V_PCH_LPC_RID_9                           0x09
#define V_PCH_LPC_RID_10                          0x10
#define V_PCH_LPC_RID_11                          0x11
#define V_PCH_LPC_RID_20                          0x20
#define V_PCH_LPC_RID_21                          0x21
#define V_PCH_LPC_RID_30                          0x30
#define V_PCH_LPC_RID_31                          0x31
#define R_PCH_LPC_SERIRQ_CNT                      0x64
#define B_PCH_LPC_SERIRQ_CNT_SIRQEN               0x80
#define B_PCH_LPC_SERIRQ_CNT_SIRQMD               0x40
#define B_PCH_LPC_SERIRQ_CNT_SIRQSZ               0x3C
#define N_PCH_LPC_SERIRQ_CNT_SIRQSZ               2
#define B_PCH_LPC_SERIRQ_CNT_SFPW                 0x03
#define N_PCH_LPC_SERIRQ_CNT_SFPW                 0
#define V_PCH_LPC_SERIRQ_CNT_SFPW_4CLK            0x00
#define V_PCH_LPC_SERIRQ_CNT_SFPW_6CLK            0x01
#define V_PCH_LPC_SERIRQ_CNT_SFPW_8CLK            0x02
#define R_PCH_LPC_IOD                             0x80
#define B_PCH_LPC_IOD_FDD                         0x1000
#define N_PCH_LPC_IOD_FDD                         12
#define V_PCH_LPC_IOD_FDD_3F0                     0
#define V_PCH_LPC_IOD_FDD_370                     1
#define B_PCH_LPC_IOD_LPT                         0x0300
#define N_PCH_LPC_IOD_LPT                         8
#define V_PCH_LPC_IOD_LPT_378                     0
#define V_PCH_LPC_IOD_LPT_278                     1
#define V_PCH_LPC_IOD_LPT_3BC                     2
#define B_PCH_LPC_IOD_COMB                        0x0070
#define N_PCH_LPC_IOD_COMB                        4
#define V_PCH_LPC_IOD_COMB_3F8                    0
#define V_PCH_LPC_IOD_COMB_2F8                    1
#define V_PCH_LPC_IOD_COMB_220                    2
#define V_PCH_LPC_IOD_COMB_228                    3
#define V_PCH_LPC_IOD_COMB_238                    4
#define V_PCH_LPC_IOD_COMB_2E8                    5
#define V_PCH_LPC_IOD_COMB_338                    6
#define V_PCH_LPC_IOD_COMB_3E8                    7
#define B_PCH_LPC_IOD_COMA                        0x0007
#define N_PCH_LPC_IOD_COMA                        0
#define V_PCH_LPC_IOD_COMA_3F8                    0
#define V_PCH_LPC_IOD_COMA_2F8                    1
#define V_PCH_LPC_IOD_COMA_220                    2
#define V_PCH_LPC_IOD_COMA_228                    3
#define V_PCH_LPC_IOD_COMA_238                    4
#define V_PCH_LPC_IOD_COMA_2E8                    5
#define V_PCH_LPC_IOD_COMA_338                    6
#define V_PCH_LPC_IOD_COMA_3E8                    7
#define R_PCH_LPC_IOE                             0x82
#define B_PCH_LPC_IOE_ME2                         BIT13           ///< Microcontroller Enable #2, Enables decoding of I/O locations 4Eh and 4Fh to LPC
#define B_PCH_LPC_IOE_SE                          BIT12           ///< Super I/O Enable, Enables decoding of I/O locations 2Eh and 2Fh to LPC.
#define B_PCH_LPC_IOE_ME1                         BIT11           ///< Microcontroller Enable #1, Enables decoding of I/O locations 62h and 66h to LPC.
#define B_PCH_LPC_IOE_KE                          BIT10           ///< Keyboard Enable, Enables decoding of the keyboard I/O locations 60h and 64h to LPC.
#define B_PCH_LPC_IOE_HGE                         BIT9            ///< High Gameport Enable, Enables decoding of the I/O locations 208h to 20Fh to LPC.
#define B_PCH_LPC_IOE_LGE                         BIT8            ///< Low Gameport Enable, Enables decoding of the I/O locations 200h to 207h to LPC.
#define B_PCH_LPC_IOE_FDE                         BIT3            ///< Floppy Drive Enable, Enables decoding of the FDD range to LPC. Range is selected by LIOD.FDE
#define B_PCH_LPC_IOE_PPE                         BIT2            ///< Parallel Port Enable, Enables decoding of the LPT range to LPC. Range is selected by LIOD.LPT.
#define B_PCH_LPC_IOE_CBE                         BIT1            ///< Com Port B Enable, Enables decoding of the COMB range to LPC. Range is selected LIOD.CB.
#define B_PCH_LPC_IOE_CAE                         BIT0            ///< Com Port A Enable, Enables decoding of the COMA range to LPC. Range is selected LIOD.CA.
#define R_PCH_LPC_GEN1_DEC                        0x84
#define R_PCH_LPC_GEN2_DEC                        0x88
#define R_PCH_LPC_GEN3_DEC                        0x8C
#define R_PCH_LPC_GEN4_DEC                        0x90
#define B_PCH_LPC_GENX_DEC_IODRA                  0x00FC0000
#define B_PCH_LPC_GENX_DEC_IOBAR                  0x0000FFFC
#define B_PCH_LPC_GENX_DEC_EN                     0x00000001
#define R_PCH_LPC_ULKMC                           0x94
#define B_PCH_LPC_ULKMC_SMIBYENDPS                BIT15
#define B_PCH_LPC_ULKMC_TRAPBY64W                 BIT11
#define B_PCH_LPC_ULKMC_TRAPBY64R                 BIT10
#define B_PCH_LPC_ULKMC_TRAPBY60W                 BIT9
#define B_PCH_LPC_ULKMC_TRAPBY60R                 BIT8
#define B_PCH_LPC_ULKMC_SMIATENDPS                BIT7
#define B_PCH_LPC_ULKMC_PSTATE                    BIT6
#define B_PCH_LPC_ULKMC_A20PASSEN                 BIT5
#define B_PCH_LPC_ULKMC_USBSMIEN                  BIT4
#define B_PCH_LPC_ULKMC_64WEN                     BIT3
#define B_PCH_LPC_ULKMC_64REN                     BIT2
#define B_PCH_LPC_ULKMC_60WEN                     BIT1
#define B_PCH_LPC_ULKMC_60REN                     BIT0
#define R_PCH_LPC_LGMR                            0x98
#define B_PCH_LPC_LGMR_MA                         0xFFFF0000
#define B_PCH_LPC_LGMR_LMRD_EN                    BIT0

#define R_PCH_LPC_FWH_BIOS_SEL                    0xD0
#define B_PCH_LPC_FWH_BIOS_SEL_F8                 0xF0000000
#define B_PCH_LPC_FWH_BIOS_SEL_F0                 0x0F000000
#define B_PCH_LPC_FWH_BIOS_SEL_E8                 0x00F00000
#define B_PCH_LPC_FWH_BIOS_SEL_E0                 0x000F0000
#define B_PCH_LPC_FWH_BIOS_SEL_D8                 0x0000F000
#define B_PCH_LPC_FWH_BIOS_SEL_D0                 0x00000F00
#define B_PCH_LPC_FWH_BIOS_SEL_C8                 0x000000F0
#define B_PCH_LPC_FWH_BIOS_SEL_C0                 0x0000000F
#define R_PCH_LPC_FWH_BIOS_SEL2                   0xD4
#define B_PCH_LPC_FWH_BIOS_SEL2_70                0xF000
#define B_PCH_LPC_FWH_BIOS_SEL2_60                0x0F00
#define B_PCH_LPC_FWH_BIOS_SEL2_50                0x00F0
#define B_PCH_LPC_FWH_BIOS_SEL2_40                0x000F
#define R_PCH_LPC_BDE                             0xD8                          ///< BIOS decode enable
#define B_PCH_LPC_BDE_F8                          0x8000
#define B_PCH_LPC_BDE_F0                          0x4000
#define B_PCH_LPC_BDE_E8                          0x2000
#define B_PCH_LPC_BDE_E0                          0x1000
#define B_PCH_LPC_BDE_D8                          0x0800
#define B_PCH_LPC_BDE_D0                          0x0400
#define B_PCH_LPC_BDE_C8                          0x0200
#define B_PCH_LPC_BDE_C0                          0x0100
#define B_PCH_LPC_BDE_LEG_F                       0x0080
#define B_PCH_LPC_BDE_LEG_E                       0x0040
#define B_PCH_LPC_BDE_70                          0x0008
#define B_PCH_LPC_BDE_60                          0x0004
#define B_PCH_LPC_BDE_50                          0x0002
#define B_PCH_LPC_BDE_40                          0x0001
#define R_PCH_LPC_PCC                             0xE0
#define B_PCH_LPC_PCC_CLKRUN_EN                   0x0001
#define B_PCH_LPC_FVEC0_USB_PORT_CAP              0x00000C00
#define V_PCH_LPC_FVEC0_USB_14_PORT               0x00000000
#define V_PCH_LPC_FVEC0_USB_12_PORT               0x00000400
#define V_PCH_LPC_FVEC0_USB_10_PORT               0x00000800
#define B_PCH_LPC_FVEC0_SATA_RAID_CAP             0x00000080
#define B_PCH_LPC_FVEC0_SATA_PORT23_CAP           0x00000040
#define B_PCH_LPC_FVEC0_SATA_PORT1_6GB_CAP        0x00000008
#define B_PCH_LPC_FVEC0_SATA_PORT0_6GB_CAP        0x00000004
#define B_PCH_LPC_FVEC0_PCI_CAP                   0x00000002
#define R_PCH_LPC_FVEC1                           0x01
#define B_PCH_LPC_FVEC1_USB_R_CAP                 0x00400000
#define R_PCH_LPC_FVEC2                           0x02
#define V_PCH_LPC_FVEC2_PCIE_PORT78_CAP           0x00200000
#define V_PCH_LPC_FVEC2_PCH_IG_SUPPORT_CAP        0x00020000      ///< PCH Integrated Graphics Support Capability
#define R_PCH_LPC_FVEC3                           0x03
#define B_PCH_LPC_FVEC3_DCMI_CAP                  0x00002000      ///< Data Center Manageability Interface (DCMI) Capability
#define B_PCH_LPC_FVEC3_NM_CAP                    0x00001000      ///< Node Manager Capability

#define R_PCH_LPC_MDAP                            0xC0
#define B_PCH_LPC_MDAP_POLICY_EN                  BIT31
#define B_PCH_LPC_MDAP_PDMA_EN                    BIT30
#define B_PCH_LPC_MDAP_VALUE                      0x0001FFFF

//
// APM Registers
//
#define R_PCH_APM_CNT                             0xB2
#define R_PCH_APM_STS                             0xB3

#define R_PCH_LPC_BC                              0xDC            ///< Bios Control
#define S_PCH_LPC_BC                              1
#define B_PCH_LPC_BC_BILD                         BIT7            ///< BIOS Interface Lock-Down
#define B_PCH_LPC_BC_BBS                          BIT6            ///< Boot BIOS strap
#define N_PCH_LPC_BC_BBS                          6
#define V_PCH_LPC_BC_BBS_SPI                      0               ///< Boot BIOS strapped to SPI
#define V_PCH_LPC_BC_BBS_LPC                      1               ///< Boot BIOS strapped to LPC
#define B_PCH_LPC_BC_EISS                         BIT5            ///< Enable InSMM.STS
#define B_PCH_LPC_BC_TS                           BIT4            ///< Top Swap
#define B_PCH_LPC_BC_LE                           BIT1            ///< Lock Enable
#define N_PCH_LPC_BC_LE                           1
#define B_PCH_LPC_BC_WPD                          BIT0            ///< Write Protect Disable

#define R_PCH_ESPI_PCBC                           0xDC            ///< Peripheral Channel BIOS Control
#define S_PCH_ESPI_PCBC                           4               ///< Peripheral Channel BIOS Control register size
#define B_PCH_ESPI_PCBC_BWRE                      BIT11           ///< BIOS Write Report Enable
#define N_PCH_ESPI_PCBC_BWRE                      11              ///< BIOS Write Report Enable bit position
#define B_PCH_ESPI_PCBC_BWRS                      BIT10           ///< BIOS Write Report Status
#define N_PCH_ESPI_PCBC_BWRS                      10              ///< BIOS Write Report Status bit position
#define B_PCH_ESPI_PCBC_BWPDS                     BIT8            ///< BIOS Write Protect Disable Status
#define N_PCH_ESPI_PCBC_BWPDS                     8               ///< BIOS Write Protect Disable Status bit position
#define B_PCH_ESPI_PCBC_ESPI_EN                   BIT2            ///< eSPI Enable Pin Strap
#define B_PCH_ESPI_PCBC_LE                        BIT1            ///< Lock Enable
#define N_PCH_ESPI_PCBC_LE                        1

//
// eSPI slave registers
//
#define R_ESPI_SLAVE_CHA_0_CAP_AND_CONF           0x10            ///< Channel 0 Capabilities and Configurations
#define B_ESPI_SLAVE_CHA_0_CAP_AND_CONF_BME       BIT2            ///< Bus Master Enable

//
// Processor interface registers
//
#define R_PCH_NMI_SC                              0x61
#define B_PCH_NMI_SC_SERR_NMI_STS                 BIT7
#define B_PCH_NMI_SC_IOCHK_NMI_STS                BIT6
#define B_PCH_NMI_SC_TMR2_OUT_STS                 BIT5
#define B_PCH_NMI_SC_REF_TOGGLE                   BIT4
#define B_PCH_NMI_SC_IOCHK_NMI_EN                 BIT3
#define B_PCH_NMI_SC_PCI_SERR_EN                  BIT2
#define B_PCH_NMI_SC_SPKR_DAT_EN                  BIT1
#define B_PCH_NMI_SC_TIM_CNT2_EN                  BIT0
#define R_PCH_NMI_EN                              0x70
#define B_PCH_NMI_EN_NMI_EN                       BIT7

//
// Reset Generator I/O Port
//
#define R_PCH_RST_CNT                             0xCF9
#define B_PCH_RST_CNT_FULL_RST                    BIT3
#define B_PCH_RST_CNT_RST_CPU                     BIT2
#define B_PCH_RST_CNT_SYS_RST                     BIT1
#define V_PCH_RST_CNT_FULLRESET                   0x0E
#define V_PCH_RST_CNT_HARDRESET                   0x06
#define V_PCH_RST_CNT_SOFTRESET                   0x04
#define V_PCH_RST_CNT_HARDSTARTSTATE              0x02
#define V_PCH_RST_CNT_SOFTSTARTSTATE              0x00

//
// RTC register
//
#define R_PCH_RTC_INDEX                           0x70
#define R_PCH_RTC_TARGET                          0x71
#define R_PCH_RTC_EXT_INDEX                       0x72
#define R_PCH_RTC_EXT_TARGET                      0x73
#define R_PCH_RTC_INDEX_ALT                       0x74
#define R_PCH_RTC_TARGET_ALT                      0x75
#define R_PCH_RTC_EXT_INDEX_ALT                   0x76
#define R_PCH_RTC_EXT_TARGET_ALT                  0x77
#define R_PCH_RTC_REGA                            0x0A
#define B_PCH_RTC_REGA_UIP                        0x80
#define R_PCH_RTC_REGB                            0x0B
#define B_PCH_RTC_REGB_SET                        0x80
#define B_PCH_RTC_REGB_PIE                        0x40
#define B_PCH_RTC_REGB_AIE                        0x20
#define B_PCH_RTC_REGB_UIE                        0x10
#define B_PCH_RTC_REGB_DM                         0x04
#define B_PCH_RTC_REGB_HOURFORM                   0x02
#define R_PCH_RTC_REGC                            0x0C
#define R_PCH_RTC_REGD                            0x0D

//
// Private Configuration Register
// RTC PCRs (PID:RTC)
//
#define R_PCH_PCR_RTC_CONF                        0x3400               ///< RTC Configuration register
#define S_PCH_PCR_RTC_CONF                        4
#define B_PCH_PCR_RTC_CONF_UCMOS_LOCK             BIT4
#define B_PCH_PCR_RTC_CONF_LCMOS_LOCK             BIT3
#define B_PCH_PCR_RTC_CONF_RESERVED               BIT31
#define B_PCH_PCR_RTC_CONF_UCMOS_EN               BIT2                 ///< Upper CMOS bank enable
#define R_PCH_PCR_RTC_BUC                         0x3414               ///< Backed Up Control
#define B_PCH_PCR_RTC_BUC_TS                      BIT0                 ///< Top Swap
#define R_PCH_PCR_RTC_RTCDCG                      0x3418               ///< RTC Dynamic Clock Gating Control
#define R_PCH_PCR_RTC_RTCDCG_RTCPCICLKDCGEN       BIT1                 ///< ipciclk_clk (24 MHz) Dynamic Clock Gate Enable
#define R_PCH_PCR_RTC_RTCDCG_RTCROSIDEDCGEN       BIT0                 ///< rosc_side_clk (120 MHz) Dynamic Clock Gate Enable
#define R_PCH_PCR_RTC_3F00                        0x3F00
#define R_PCH_PCR_RTC_UIPSMI                      0x3F04               ///< RTC Update In Progress SMI Control

//
// LPC PCR Registers
//
#define R_PCH_PCR_LPC_HVMTCTL                     0x3410
#define R_PCH_PCR_LPC_GCFD                        0x3418
#define R_PCH_PCR_LPC_PRC                         0x341C
#define R_PCH_PCR_LPC_PCT                         0x3420
#define R_PCH_PCR_LPC_SCT                         0x3424
#define R_PCH_PCR_LPC_LPCCT                       0x3428
#define R_PCH_PCR_LPC_ULTOR                       0x3500

//
// eSPI PCR Registers
//
#define R_PCH_PCR_ESPI_SLV_CFG_REG_CTL            0x4000                  ///< Slave Configuration Register and Link Control
#define B_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRE       BIT31                   ///< Slave Configuration Register Access Enable
#define B_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRS       (BIT30 | BIT29 | BIT28) ///< Slave Configuration Register Access Status
#define N_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRS       28                      ///< Slave Configuration Register Access Status bit position
#define B_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SBLCL      BIT27                   ///< IOSF-SB eSPI Link Configuration Lock
#define V_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRS_NOERR 7                       ///< No errors (transaction completed successfully)
#define B_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SID        (BIT20 | BIT19)         ///< Slave ID
#define N_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SID        19                      ///< Slave ID bit position
#define B_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRT       (BIT17 | BIT16)         ///< Slave Configuration Register Access Type
#define N_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRT       16                      ///< Slave Configuration Register Access Type bit position
#define V_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRT_RD    0                       ///< Slave Configuration register read from address SCRA[11:0]
#define V_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRT_WR    1                       ///< Slave Configuration register write to address SCRA[11:0]
#define V_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRT_STS   2                       ///< Slave Status register read
#define V_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRT_RS    3                       ///< In-Band reset
#define B_PCH_PCR_ESPI_SLV_CFG_REG_CTL_SCRA       0x00000FFF              ///< Slave Configuration Register Address
#define R_PCH_PCR_ESPI_SLV_CFG_REG_DATA           0x4004                  ///< Slave Configuration Register Data

#define R_PCH_PCR_ESPI_PCERR_SLV0                 0x4020          ///< Peripheral Channel Error for Slave 0
#define R_PCH_PCR_ESPI_PCERR_SLV1                 0x4024          ///< Peripheral Channel Error for Slave 1
#define R_PCH_PCR_ESPI_VWERR_SLV0                 0x4030          ///< Virtual Wire Channel Error for Slave 0
#define R_PCH_PCR_ESPI_VWERR_SLV1                 0x4034          ///< Virtual Wire Channel Error for Slave 1
#define R_PCH_PCR_ESPI_FCERR_SLV0                 0x4040          ///< Flash Access Channel Error for Slave 0
#define B_PCH_PCR_ESPI_XERR_XNFEE                 (BIT14 | BIT13) ///< Non-Fatal Error Reporting Enable bits
#define N_PCH_PCR_ESPI_XERR_XNFEE                 13              ///< Non-Fatal Error Reporting Enable bit position
#define V_PCH_PCR_ESPI_XERR_XNFEE_SMI             3               ///< Enable Non-Fatal Error Reporting as SMI
#define B_PCH_PCR_ESPI_XERR_XNFES                 BIT12           ///< Fatal Error Status
#define B_PCH_PCR_ESPI_XERR_XFEE                  (BIT6 | BIT5)   ///< Fatal Error Reporting Enable bits
#define N_PCH_PCR_ESPI_XERR_XFEE                  5               ///< Fatal Error Reporting Enable bit position
#define V_PCH_PCR_ESPI_XERR_XFEE_SMI              3               ///< Enable Fatal Error Reporting as SMI
#define B_PCH_PCR_ESPI_XERR_XFES                  BIT4            ///< Fatal Error Status
#define B_PCH_PCR_ESPI_PCERR_SLV0_PCURD           BIT24           ///< Peripheral Channel Unsupported Request Detected
#define R_PCH_PCR_ESPI_LNKERR_SLV0                0x4050          ///< Link Error for Slave 0
#define S_PCH_PCR_ESPI_LNKERR_SLV0                4               ///< Link Error for Slave 0 register size
#define B_PCH_PCR_ESPI_LNKERR_SLV0_SLCRR          BIT31           ///< eSPI Link and Slave Channel Recovery Required
#define B_PCH_PCR_ESPI_LNKERR_SLV0_LFET1E         (BIT22 | BIT21) ///< Fatal Error Type 1 Reporting Enable
#define N_PCH_PCR_ESPI_LNKERR_SLV0_LFET1E         21              ///< Fatal Error Type 1 Reporting Enable bit position
#define V_PCH_PCR_ESPI_LNKERR_SLV0_LFET1E_SMI     3               ///< Enable Fatal Error Type 1 Reporting as SMI
#define B_PCH_PCR_ESPI_LNKERR_SLV0_LFET1S         BIT20           ///< Link Fatal Error Type 1 Status
#define R_PCH_PCR_ESPI_CFG_VAL                    0xC00C          ///< ESPI Enabled Strap
#define B_PCH_PCR_ESPI_ENABLE_STRAP               BIT0            ///< ESPI Enabled Strap bit position



//
//  LPC Device ID macros
//
//
// Device IDs that are SKL PCH-H Desktop specific
//
#define IS_SKL_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) \
    (  \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_DT_0) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_DT_1) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_DT_2) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_DT_3) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_DT_4) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_DT_5) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_DT_6) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_DT_SUPER_SKU) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_ES_SUPER_SKU /* SKL-PCH-H in KBL-PCH-H package */) \
    )

//
// Device IDs that are KBL PCH-H Desktop specific
//
#define IS_KBL_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) \
    (  \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_H215)  || \
    (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_Z370)  || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_SUPER_SKU) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_H270) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_Z270) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_Q270) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_Q250) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_DT_B250) \
    )

//
// Device IDs that are PCH-H Desktop specific
//
#define IS_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) \
    (  \
      IS_SKL_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) || \
      IS_KBL_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) \
    )

#define IS_PCH_LPC_DEVICE_ID_DESKTOP(DeviceId) \
    ( \
      IS_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) \
    )

//
// Device IDs that are PCH-H Mobile specific
//

#define IS_PCH_H_LPC_DEVICE_ID_MOBILE(DeviceId) \
    ( \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_MB_0) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_MB_1) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_MB_3) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_MB_4) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_MB_5) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_MB_6) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_MB_SUPER_SKU) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_MB_8) \
    )

//
// Device IDs that are PCH-LP Mobile specific
//
#define IS_PCH_LP_LPC_DEVICE_ID_MOBILE(DeviceId) \
    ( \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_SUPER_SKU) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_0) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_1) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_2) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_3) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_SUPER_SKU_1) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_4) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_5) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_6) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_7) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_8) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_9) || \
      (DeviceId == V_PCH_LP_LPC_DEVICE_ID_MB_10) \
    )

#define IS_PCH_LPC_DEVICE_ID_MOBILE(DeviceId) \
    ( \
      IS_PCH_H_LPC_DEVICE_ID_MOBILE(DeviceId) || \
      IS_PCH_LP_LPC_DEVICE_ID_MOBILE(DeviceId) \
    )

//
// Device IDS that are SKL PCH Server\Workstation specific
//
#define IS_SKL_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) \
    ( \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_SVR_ES_SUPER_SKU) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_SVR_0) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_SVR_1) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_SVR_2) || \
      (DeviceId == V_SKL_PCH_H_LPC_DEVICE_ID_A14B) \
    )

//
// Device IDS that are KBL PCH Server\Workstation specific
//
#define IS_KBL_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) \
    ( \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_SVR_SUPER_SKU) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_SVR_0) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_SVR_1) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_SVR_2) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_SVR_3) || \
      (DeviceId == V_KBL_PCH_H_LPC_DEVICE_ID_SVR_4) \
    )


#define IS_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) \
    ( \
      IS_KBL_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) || \
      IS_SKL_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) \
    )

#define IS_PCH_LPC_DEVICE_ID_SERVER(DeviceId) \
    ( \
      IS_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) \
    )

#define IS_PCH_H_LPC_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) || \
      IS_PCH_H_LPC_DEVICE_ID_MOBILE(DeviceId) || \
      IS_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) \
    )

#define IS_SKL_PCH_H_LPC_DEVICE_ID(DeviceId) \
    ( \
      IS_SKL_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) || \
      IS_PCH_H_LPC_DEVICE_ID_MOBILE(DeviceId) || \
      IS_SKL_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) \
    )

#define IS_KBL_PCH_H_LPC_DEVICE_ID(DeviceId) \
    ( \
      IS_KBL_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) || \
      IS_KBL_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) \
    )

#define IS_PCH_LP_LPC_DEVICE_ID(DeviceId) \
    ( \
     IS_PCH_LP_LPC_DEVICE_ID_MOBILE(DeviceId) \
    )

#define IS_PCH_LPC_DEVICE_ID(DeviceId) \
    ( \
      IS_PCH_H_LPC_DEVICE_ID(DeviceId) || \
      IS_PCH_LP_LPC_DEVICE_ID(DeviceId) \
    )

#define IS_SKL_PCH_LPC_DEVICE_ID(DeviceId) \
    ( \
      IS_SKL_PCH_H_LPC_DEVICE_ID_DESKTOP(DeviceId) || \
      IS_PCH_LPC_DEVICE_ID_MOBILE(DeviceId) || \
      IS_SKL_PCH_H_LPC_DEVICE_ID_SERVER(DeviceId) \
    )

#define IS_KBL_PCH_LPC_DEVICE_ID(DeviceId) \
    ( \
      IS_KBL_PCH_H_LPC_DEVICE_ID(DeviceId) \
    )
#endif


//========================================================================================
//   edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/Library/PchInfoLib.h
// = edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/Library/PchInfoLib.h
//   edk2-platforms/Silicon/Intel/PurleyRefreshSiliconPkg/Pch/Include/Library/PchInfoLib.h
//   edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Pch/Include/Library/PchInfoLib.h

/** @file
  Header file for PchInfoLib.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _PCH_INFO_LIB_H_
#define _PCH_INFO_LIB_H_

//#include <PchAccess.h>

typedef enum {
  PchH          = 1,
  PchLp,
  PchUnknownSeries
} PCH_SERIES;

typedef enum {
  SklPch        = 1,
  KblPch,
  PchUnknownGeneration
} PCH_GENERATION;

typedef enum {
  RstUnsupported  = 0,
  RstPremium,
  RstOptane,
  RstMaxMode
} RST_MODE;

typedef enum {
  PchMobileSku = 0,
  PchDesktopSku,
  PchServerSku,
  PchUnknownSku,
  PchMaxSku
} PCH_SKU_TYPE;

/**
  Return Pch stepping type

  @retval PCH_STEPPING            Pch stepping type
**/
PCH_STEPPING
EFIAPI
PchStepping (
  VOID
  );

/**
  Determine if PCH is supported

  @retval TRUE                    PCH is supported
  @retval FALSE                   PCH is not supported
**/
BOOLEAN
IsPchSupported (
  VOID
  );

/**
  Return Pch Series

  @retval PCH_SERIES                Pch Series
**/
PCH_SERIES
EFIAPI
GetPchSeries (
  VOID
  );

/**
  Return Pch Generation

  @retval PCH_GENERATION            Pch Generation
**/
PCH_GENERATION
EFIAPI
GetPchGeneration (
  VOID
  );

/**
  Get PCH SKU type

  @retval PCH_SKU_TYPE  Type of PCH SKU
**/
PCH_SKU_TYPE
GetPchSkuType (
  VOID
  );

/**
  Get Lpc Did

  @retval  UINT16                    Lpc Did
**/
UINT16
EFIAPI
GetLpcDid (
  VOID
  );

/**
  Get Pch Maximum Pcie Root Port Number

  @retval PcieMaxRootPort         Pch Maximum Pcie Root Port Number
**/
UINT8
EFIAPI
GetPchMaxPciePortNum (
  VOID
  );

/**
  Get Pch Maximum Sata Port Number

  @retval Pch Maximum Sata Port Number
**/
UINT8
EFIAPI
GetPchMaxSataPortNum (
  VOID
  );

/**
  Get Pch Usb Maximum Physical Port Number

  @retval Pch Usb Maximum Physical Port Number
**/
UINT8
EFIAPI
GetPchUsbMaxPhysicalPortNum (
  VOID
  );

/**
  Get Pch Maximum Usb2 Port Number of XHCI Controller

  @retval Pch Maximum Usb2 Port Number of XHCI Controller
**/
UINT8
EFIAPI
GetPchXhciMaxUsb2PortNum (
  VOID
  );

/**
  Get Pch Maximum Usb3 Port Number of XHCI Controller

  @retval Pch Maximum Usb3 Port Number of XHCI Controller
**/
UINT8
EFIAPI
GetPchXhciMaxUsb3PortNum (
  VOID
  );

/**
  Get PCH stepping ASCII string
  The return string is zero terminated.

  @param [in]      PchStep              Pch stepping
  @param [out]     Buffer               Output buffer of string
  @param [in,out]  BufferSize           Size of input buffer,
                                        and return required string size when buffer is too small.

  @retval EFI_SUCCESS                   String copy successfully
  @retval EFI_INVALID_PARAMETER         The stepping is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PchGetSteppingStr (
  IN     PCH_STEPPING                   PchStep,
  OUT    CHAR8                          *Buffer,
  IN OUT UINT32                         *BufferSize
  );

/**
  Get PCH series ASCII string
  The return string is zero terminated.

  @param [in]      PchSeries            Pch series
  @param [out]     Buffer               Output buffer of string
  @param [in,out]  BufferSize           Size of input buffer,
                                        and return required string size when buffer is too small.

  @retval EFI_SUCCESS                   String copy successfully
  @retval EFI_INVALID_PARAMETER         The series is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PchGetSeriesStr (
  IN     PCH_SERIES                     PchSeries,
  OUT    CHAR8                          *Buffer,
  IN OUT UINT32                         *BufferSize
  );

/**
  Get PCH Sku ASCII string
  The return string is zero terminated.

  @param [in]      LpcDid               LPC device id
  @param [out]     Buffer               Output buffer of string
  @param [in,out]  BufferSize           Size of input buffer,
                                        and return required string size when buffer is too small.

  @retval EFI_SUCCESS                   String copy successfully
  @retval EFI_INVALID_PARAMETER         The series is not supported, or parameters are NULL
  @retval EFI_BUFFER_TOO_SMALL          Input buffer size is too small
**/
EFI_STATUS
PchGetSkuStr (
  IN     UINT16                         LpcDid,
  OUT    CHAR8                          *Buffer,
  IN OUT UINT32                         *BufferSize
  );

/**
  Get RST mode supported by the silicon

  @retval RST_MODE               RST mode supported by silicon
**/
RST_MODE
EFIAPI
GetSupportedRstMode (
  VOID
  );

/**
  Check if current SKU supports Optane mode

  @retval TRUE                   This SKU supports Optane mode
  @retval FALSE                  This SKU doesn't support Optane mode
**/
BOOLEAN
EFIAPI
IsOptaneModeSupported (
  VOID
  );

/**
  Check if current SKU supports RAID feature

  @retval TRUE                   This SKU supports RAID
  @retval FALSE                  This SKU doesn't support RAID
**/
BOOLEAN
EFIAPI
IsPchRaidSupported (
  VOID
  );
#endif // _PCH_INFO_LIB_H_


//========================================================================================
// edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Pch/Include/Protocol/PchSmiDispatch.h
// edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Pch/Include/Protocol/PchSmiDispatch.h
// edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Pch/Include/Protocol/PchSmiDispatch.h

/** @file
  APIs of PCH SMI Dispatch Protocol.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_SMI_DISPATCH_PROTOCOL_H_
#define _PCH_SMI_DISPATCH_PROTOCOL_H_

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gPchSmiDispatchProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _PCH_SMI_DISPATCH_PROTOCOL         PCH_SMI_DISPATCH_PROTOCOL;

//
// Member functions
//

/**
  Callback function for an PCH SMI handler dispatch.

  @param[in] DispatchHandle             The unique handle assigned to this handler by register function.

**/
typedef
VOID
(EFIAPI *PCH_SMI_DISPATCH_CALLBACK) (
  IN EFI_HANDLE                         DispatchHandle
  );

/**
  Register a child SMI source dispatch function for specific PCH SMI dispatch event.

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchFunction           Pointer to dispatch function to be invoked for
                                        this SMI source
  @param[out] DispatchHandle            Handle of dispatch function, for when interfacing
                                        with the parent SMM driver.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR              The driver was unable to enable the SMI source.
  @retval EFI_OUT_OF_RESOURCES          Not enough memory (system or SMM) to manage this child.
  @retval EFI_ACCESS_DENIED             Return access denied if the EndOfDxe event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *PCH_SMI_DISPATCH_REGISTER) (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the EndOfDxe event has been triggered
**/
typedef
EFI_STATUS
(EFIAPI *PCH_SMI_DISPATCH_UNREGISTER) (
  IN  PCH_SMI_DISPATCH_PROTOCOL         *This,
  IN  EFI_HANDLE                        DispatchHandle
  );

/**
  Interface structure for PCH specific SMIs Dispatch Protocol
  The PCH SMI DISPATCH PROTOCOL provides the ability to dispatch function for PCH misc SMIs.
  It contains legacy SMIs and new PCH SMI types like:
  SerialIrq, McSmi, Smbus, ...
**/
struct _PCH_SMI_DISPATCH_PROTOCOL {
  /**
    This member specifies the revision of this structure. This field is used to
    indicate backwards compatible changes to the protocol.
  **/
  UINT8                                 Revision;
  /**
    Smi unregister function for PCH SMI DISPATCH PROTOCOL.
  **/
  PCH_SMI_DISPATCH_UNREGISTER           UnRegister;
  /**
    SerialIrq
    The event is triggered while the SMI# was caused by the SERIRQ decoder.
  **/
  PCH_SMI_DISPATCH_REGISTER             SerialIrqRegister;
  /**
    McSmi
    The event is triggered if there has been an access to the power management
    microcontroller range (62h or 66h) and the Microcontroller Decode Enable #1 bit
    in the LPC Bridge I/O Enables configuration register is 1 .
  **/
  PCH_SMI_DISPATCH_REGISTER             McSmiRegister;
  /**
    SmBus
    The event is triggered while the SMI# was caused by:
    1. The SMBus Slave receiving a message that an SMI# should be caused, or
    2. The SMBALERT# signal goes active and the SMB_SMI_EN bit is set and the
       SMBALERT_DIS bit is cleared, or
    3. The SMBus Slave receiving a Host Notify message and the HOST_NOTIFY_INTREN and
       the SMB_SMI_EN bits are set, or
    4. The PCH detecting the SMLINK_SLAVE_SMI command while in the S0 state.
  **/
  PCH_SMI_DISPATCH_REGISTER             SmbusRegister;
  /**
    SPI Asynchronous
    When registered, the flash controller will generate an SMI when it blocks a BIOS write or erase.
  **/
  PCH_SMI_DISPATCH_REGISTER             SpiAsyncRegister;
};

/**
  PCH SMI dispatch revision number

  Revision 1:   Initial version
**/
#define PCH_SMI_DISPATCH_REVISION                 1

#endif


//========================================================================================
// edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/Pch/PchSmiDispatcher/Smm/PchSmm.h


/** @file
  Prototypes and defines for the PCH SMM Dispatcher.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PCH_SMM_H_
#define _PCH_SMM_H_

//#include <Uefi.h>
//#include <Protocol/PciRootBridgeIo.h>
//#include <Protocol/LoadedImage.h>
//#include <Protocol/SmmControl2.h>
//#include <Protocol/SmmUsbDispatch2.h>
//#include <Protocol/SmmSxDispatch2.h>
//#include <Protocol/SmmSwDispatch2.h>
//#include <Protocol/SmmGpiDispatch2.h>
//#include <Protocol/SmmPowerButtonDispatch2.h>
//#include <Protocol/SmmPeriodicTimerDispatch2.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/DxeServicesTableLib.h>
//#include <Library/DebugLib.h>
//#include <Library/IoLib.h>
//#include <Library/BaseLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/DevicePathLib.h>
//#include <Library/SmmServicesTableLib.h>
//#include <Library/ReportStatusCodeLib.h>
//#include <Library/PerformanceLib.h>
//#include <Protocol/SmmReadyToLock.h>
//#include <IndustryStandard/Pci30.h>
//#include <Library/PchCycleDecodingLib.h>
//#include <Library/PchPcieRpLib.h>
//#include <Library/PchPcrLib.h>
//#include <Library/PciSegmentLib.h>
//#include <Library/GpioLib.h>
//#include <Library/PchInfoLib.h>
//#include <Library/EspiLib.h>
//#include <Library/GpioPrivateLib.h>
//#include <Protocol/PchTcoSmiDispatch.h>
//#include <Protocol/PchPcieSmiDispatch.h>
//#include <Protocol/PchAcpiSmiDispatch.h>
//#include <Protocol/PchSmiDispatch.h>
//#include <Protocol/PchEspiSmiDispatch.h>
//#include <Protocol/IoTrapExDispatch.h>
//#include <Library/PmcLib.h>
//#include "IoTrap.h"

#define EFI_BAD_POINTER          0xAFAFAFAFAFAFAFAFULL

extern BOOLEAN                   mReadyToLock;

///
/// Define an enumeration for all the supported protocols
///
#define PCH_SMM_PROTOCOL_TYPE_MAX       6

typedef enum {
  UsbType,
  SxType,
  SwType,
  GpiType,
  PowerButtonType,
  PeriodicTimerType,
  PchSmiDispatchType,
  PchSmmProtocolTypeMax
} PCH_SMM_PROTOCOL_TYPE;

///
/// Define all the supported types of PCH SMI
///
typedef enum {
  PchTcoSmiMchType,
  PchTcoSmiTcoTimeoutType,
  PchTcoSmiOsTcoType,
  PchTcoSmiNmiType,
  PchTcoSmiIntruderDetectType,
  PchTcoSmiSpiBiosWpType,
  PchTcoSmiLpcBiosWpType,
  PchTcoSmiNewCenturyType,
  PchPcieSmiRpHotplugType,
  PchPcieSmiRpLinkActiveType,
  PchPcieSmiRpLinkEqType,
  PchAcpiSmiPmeType,
  PchAcpiSmiPmeB0Type,
  PchAcpiSmiRtcAlarmType,
  PchAcpiSmiTmrOverflowType,
  PchEspiSmiEspiSlaveType,
  PchSmiSerialIrqType,
  PchSmiMcSmiType,
  PchSmiSmBusType,
  PchSmiSpiAsyncType,
  PchIoTrapSmiType                      ///< internal SMI type
} PCH_SMI_TYPES;

///
/// Generic funciton pointer to cover all Pch SMI function pointer types
///
typedef
VOID
(EFIAPI *PCH_SMI_CALLBACK_FUNCTIONS) (
  IN EFI_HANDLE                         DispatchHandle,
  ...
  );


///
/// SPECIFYING A REGISTER
/// We want a general way of referring to addresses.  For this case, we'll only
/// need addresses in the ACPI table (and the TCO entries within the ACPI table).
/// However, it's interesting to consider what it would take to support other types
/// of addresses.  To address Will's concern, I think it prudent to accommodate it
/// early on in the design.
///
/// Addresses we need to consider:
///
///  Type:                           Required:
///  I/O                             Yes
///    ACPI (special case of I/O)    Only if we want to
///    TCO  (special case of I/O)    Only if we want to
///  GPIO  (special case of MMIO)    Only if we want to
///  Memory (or Memory Mapped I/O)   Only if we want to
///  PCIE                            Yes, for BiosWp
///
typedef enum {
  ///
  ///  IO_ADDR_TYPE, /// unimplemented
  ///
  ACPI_ADDR_TYPE,
  TCO_ADDR_TYPE,
  ///
  ///  MEMORY_ADDR_TYPE, /// unimplemented
  ///
  GPIO_ADDR_TYPE,
  MEMORY_MAPPED_IO_ADDRESS_TYPE,
  PCIE_ADDR_TYPE,
  PCR_ADDR_TYPE,
  NUM_ADDR_TYPES,                     ///< count of items in this enum
  PCH_SMM_ADDR_TYPE_NULL        = -1  ///< sentinel to indicate NULL or to signal end of arrays
} ADDR_TYPE;

//
// Assumption: 32-bits -- enum's evaluate to integer
// Assumption: This code will only run on IA-32.  Justification: IA-64 doesn't have SMIs.
// We don't have to worry about 64-bit addresses.
// Typedef the size of addresses in case the numbers I'm using are wrong or in case
// this changes.  This is a good idea because PCI_ADDR will change, for example, when
// we add support for PciExpress.
//
typedef UINT16 IO_ADDR;
typedef IO_ADDR ACPI_ADDR;  ///< can omit
typedef IO_ADDR TCO_ADDR;   ///< can omit
typedef UINTN MEM_ADDR;
typedef MEM_ADDR *MEMORY_MAPPED_IO_ADDRESS;
typedef MEM_ADDR *GPIO_ADDR;
typedef union {
  UINT32  Raw;
  struct {
    UINT32 Reg: 16;
    UINT32 Fnc: 3;
    UINT32 Dev: 5;
    UINT32 Bus: 8;
  } Fields;
} PCIE_ADDR;

typedef union {
  UINT32  Raw;
  struct {
    UINT16 Offset;
    UINT8  Pid;
    UINT8  Base;
  } Fields;
} PCR_ADDR;

typedef struct {
  ADDR_TYPE Type;
  union {
    ///
    /// used to initialize during declaration/definition
    ///
    UINT32                    raw;

    ///
    /// used to access useful data
    ///
    IO_ADDR                   io;
    ACPI_ADDR                 acpi;
    TCO_ADDR                  tco;
    GPIO_ADDR                 gpio;
    MEM_ADDR                  mem;
    MEMORY_MAPPED_IO_ADDRESS  Mmio;
    PCIE_ADDR                 pcie;
    PCR_ADDR                  Pcr;

  } Data;

} PCH_SMM_ADDRESS;

///
/// SPECIFYING BITS WITHIN A REGISTER
/// Here's a struct that helps us specify a source or enable bit.
///
typedef struct {
  PCH_SMM_ADDRESS Reg;
  UINT8           SizeInBytes;  ///< of the register
  UINT8           Bit;
} PCH_SMM_BIT_DESC;

//
// Sometimes, we'll have bit descriptions that are unused.  It'd be great to have a
// way to easily identify them:
//
#define IS_BIT_DESC_NULL(BitDesc)   ((BitDesc).Reg.Type == PCH_SMM_ADDR_TYPE_NULL)  ///< "returns" true when BitDesc is NULL
#define NULL_THIS_BIT_DESC(BitDesc) ((BitDesc).Reg.Type = PCH_SMM_ADDR_TYPE_NULL)   ///< will "return" an integer w/ value of 0
#define NULL_BIT_DESC_INITIALIZER \
  { \
    { \
      PCH_SMM_ADDR_TYPE_NULL, \
      { \
        0 \
      } \
    }, \
    0, 0 \
  }
//
// I'd like a type to specify the callback's Sts & En bits because they'll
// be commonly used together:
//
#define NUM_EN_BITS   2
#define NUM_STS_BITS  1

//
// Flags
//
typedef UINT8 PCH_SMM_SOURCE_FLAGS;

//
// Flags required to describe the event source
//
#define PCH_SMM_NO_FLAGS          0
#define PCH_SMM_SCI_EN_DEPENDENT  1

typedef struct {
  PCH_SMM_SOURCE_FLAGS  Flags;
  PCH_SMM_BIT_DESC      En[NUM_EN_BITS];    ///< Describes the enable bit(s) for the SMI event
  PCH_SMM_BIT_DESC      Sts[NUM_STS_BITS];  ///< Describes the secondary status bit for the SMI event. Might be the same as TopLevelSmi
  PCH_SMM_BIT_DESC      PmcSmiSts;          ///< Refereing to the top level status bit in PMC SMI_STS, i.e. R_PCH_SMI_STS
} PCH_SMM_SOURCE_DESC;

///
/// Used to initialize null source descriptor
///
#define NULL_SOURCE_DESC_INITIALIZER \
  { \
    PCH_SMM_NO_FLAGS, \
    { \
      NULL_BIT_DESC_INITIALIZER, NULL_BIT_DESC_INITIALIZER \
    }, \
    { \
      NULL_BIT_DESC_INITIALIZER \
    }, \
    NULL_BIT_DESC_INITIALIZER \
  }

///
/// Define a PCIE RP event context for SmiProfileHandlerInfo tool
///
typedef struct {
  PCH_SMI_TYPES               PchSmiType;
  UINTN                       RpIndex;
} PCH_SMM_PCIE_REGISTER_CONTEXT;

///
/// CHILD CONTEXTS
/// To keep consistent w/ the architecture, we'll need to provide the context
/// to the child when we call its callback function.  After talking with Will,
/// we agreed that we'll need functions to "dig" the context out of the hardware
/// in many cases (Sx, Trap, Gpi, etc), and we'll need a function to compare those
/// contexts to prevent unnecessary dispatches.  I'd like a general type for these
/// "GetContext" functions, so I'll need a union of all the protocol contexts for
/// our internal use:
///
typedef union {
  //
  // (in no particular order)
  //
  EFI_SMM_SX_REGISTER_CONTEXT             Sx;
  EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT PeriodicTimer;
  EFI_SMM_SW_REGISTER_CONTEXT             Sw;
  EFI_SMM_POWER_BUTTON_REGISTER_CONTEXT   PowerButton;
  EFI_SMM_USB_REGISTER_CONTEXT            Usb;
  EFI_SMM_GPI_REGISTER_CONTEXT            Gpi;
  PCH_SMM_PCIE_REGISTER_CONTEXT           Pcie;
} PCH_SMM_CONTEXT;

///
/// Misc data for PchDispatcher usage.
/// For PeriodicTimer, since the ElapsedTime is removed from EFI_SMM_PERIODIC_TIMER_REGISTER_CONTEXT of EDKII,
/// and PchDispatcher needs it for every record. Thus move it here to support ElapsedTime.
///
typedef struct {
  UINTN    ElapsedTime;
  ///
  /// A switch to control periodic timer SMI enabling
  ///
  BOOLEAN  TimerSmiEnabled;
} PCH_SMM_MISC_DATA;

//
// Assumption: PeriodicTimer largest at 3x64-bits or 24 bytes
//
typedef struct _DATABASE_RECORD DATABASE_RECORD;

///
/// Assumption: the GET_CONTEXT function will be as small and simple as possible.
/// Assumption: We don't need to pass in an enumeration for the protocol because each
///    GET_CONTEXT function is written for only one protocol.
/// We also need a function to compare contexts to see if the child should be dispatched
/// In addition, we need a function to acquire CommBuffer and CommBufferSize for
///    dispatch callback function of EDKII native support.
///
typedef
VOID
(EFIAPI *GET_CONTEXT) (
  IN  DATABASE_RECORD    * Record,
  OUT PCH_SMM_CONTEXT    * Context
  );

typedef
BOOLEAN
(EFIAPI *CMP_CONTEXT) (
  IN PCH_SMM_CONTEXT     * Context1,
  IN PCH_SMM_CONTEXT     * Context2
  );

typedef
VOID
(EFIAPI *GET_COMMBUFFER) (
  IN  DATABASE_RECORD    * Record,
  OUT VOID               **CommBuffer,
  OUT UINTN              * CommBufferSize
  );

///
/// Finally, every protocol will require a "Get Context" and "Compare Context" call, so
/// we may as well wrap that up in a table, too.
///
typedef struct {
  GET_CONTEXT     GetContext;
  CMP_CONTEXT     CmpContext;
  GET_COMMBUFFER  GetCommBuffer;
} CONTEXT_FUNCTIONS;

extern CONTEXT_FUNCTIONS          ContextFunctions[PCH_SMM_PROTOCOL_TYPE_MAX];

///
/// MAPPING CONTEXT TO BIT DESCRIPTIONS
/// I'd like to have a general approach to mapping contexts to bit descriptions.
/// Sometimes, we'll find that we can use table lookups or constant assignments;
/// other times, we'll find that we'll need to use a function to perform the mapping.
/// If we define a macro to mask that process, we'll never have to change the code.
/// I don't know if this is desirable or not -- if it isn't, then we can get rid
/// of the macros and just use function calls or variable assignments.  Doesn't matter
/// to me.
/// Mapping complex contexts requires a function
///

/**
  Maps a USB context to a source description.

  @param[in] Context              The context we need to map.  Type must be USB.
  @param[out] SrcDesc             The source description that corresponds to the given context.

**/
VOID
MapUsbToSrcDesc (
  IN  PCH_SMM_CONTEXT         *Context,
  OUT PCH_SMM_SOURCE_DESC     *SrcDesc
  );

/**
  Figure out which timer the child is requesting and
  send back the source description

  @param[in] DispatchContext      The pointer to the Dispatch Context instances
  @param[out] SrcDesc             The pointer to the source description

**/
VOID
MapPeriodicTimerToSrcDesc (
  IN  PCH_SMM_CONTEXT                                         *DispatchContext,
  OUT PCH_SMM_SOURCE_DESC                                     *SrcDesc
  );

//
// Mapping simple contexts can be done by assignment or lookup table
//
extern CONST PCH_SMM_SOURCE_DESC  mSxSourceDesc;
extern CONST PCH_SMM_SOURCE_DESC  mPowerButtonSourceDesc;
extern CONST PCH_SMM_SOURCE_DESC  mSrcDescNewCentury;
extern CONST PCH_SMM_SOURCE_DESC  mGpiSourceDescTemplate;

///
/// For PCHx, APMC is UINT8 port, so the MAX SWI Value is 0xFF.
///
#define MAXIMUM_SWI_VALUE 0xFF
///
/// Open: Need to make sure this kind of type cast will actually work.
///   May need an intermediate form w/ two VOID* arguments.  I'll figure
///   that out when I start compiling.
///
typedef
VOID
(EFIAPI *PCH_SMM_CLEAR_SOURCE) (
  CONST PCH_SMM_SOURCE_DESC * SrcDesc
  );

///
/// "DATABASE" RECORD
/// Linked list data structures
///
#define DATABASE_RECORD_SIGNATURE SIGNATURE_32 ('D', 'B', 'R', 'C')

struct _DATABASE_RECORD {
  UINT32                        Signature;
  LIST_ENTRY                    Link;
  BOOLEAN                       Processed;
  ///
  /// Status and Enable bit description
  ///
  PCH_SMM_SOURCE_DESC           SrcDesc;

  ///
  /// Callback function
  ///
  EFI_SMM_HANDLER_ENTRY_POINT2  Callback;
  PCH_SMM_CONTEXT               ChildContext;
  UINTN                         ContextSize;

  ///
  /// Special handling hooks -- init them to NULL if unused/unneeded
  ///
  PCH_SMM_CLEAR_SOURCE          ClearSource;

  ///
  /// Functions required to make callback code general
  ///
  CONTEXT_FUNCTIONS             ContextFunctions;

  ///
  /// The protocol that this record dispatches
  ///
  PCH_SMM_PROTOCOL_TYPE         ProtocolType;

  ///
  /// Misc data for private usage
  ///
  PCH_SMM_MISC_DATA             MiscData;

  ///
  /// PCH SMI callback function
  ///
  PCH_SMI_CALLBACK_FUNCTIONS    PchSmiCallback;
  ///
  /// Indicate the PCH SMI types.
  ///
  PCH_SMI_TYPES                 PchSmiType;
};

#define DATABASE_RECORD_FROM_LINK(_record)  CR (_record, DATABASE_RECORD, Link, DATABASE_RECORD_SIGNATURE)
#define DATABASE_RECORD_FROM_CHILDCONTEXT(_record)  CR (_record, DATABASE_RECORD, ChildContext, DATABASE_RECORD_SIGNATURE)

///
/// HOOKING INTO THE ARCHITECTURE
///
typedef
EFI_STATUS
(EFIAPI *PCH_SMM_GENERIC_REGISTER) (
  IN  VOID                                    **This,
  IN  VOID                                    *DispatchFunction,
  IN  VOID                                    *DispatchContext,
  OUT EFI_HANDLE                              *DispatchHandle
  );
typedef
EFI_STATUS
(EFIAPI *PCH_SMM_GENERIC_UNREGISTER) (
  IN  VOID                                    **This,
  IN  EFI_HANDLE                              DispatchHandle
  );

///
/// Define a memory "stamp" equivalent in size and function to most of the protocols
///
typedef struct {
  PCH_SMM_GENERIC_REGISTER    Register;
  PCH_SMM_GENERIC_UNREGISTER  Unregister;
  UINTN                       Extra1;
  UINTN                       Extra2; ///< may not need this one
} PCH_SMM_GENERIC_PROTOCOL;

/**
  Register a child SMI dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the PCH_SMM_GENERIC_PROTOCOL instance.
  @param[in] DispatchFunction     Pointer to dispatch function to be invoked for this SMI source.
  @param[in] DispatchContext      Pointer to the dispatch function's context.
  @param[out] DispatchHandle      Handle of dispatch function, for when interfacing
                                  with the parent SMM driver, will be the address of linked
                                  list link in the call back record.

  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database record
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid
  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  registered and the SMI source has been enabled.
**/
EFI_STATUS
EFIAPI
PchPiSmmCoreRegister (
  IN  PCH_SMM_GENERIC_PROTOCOL                          *This,
  IN  EFI_SMM_HANDLER_ENTRY_POINT2                      DispatchFunction,
  IN  PCH_SMM_CONTEXT                                   *DispatchContext,
  OUT EFI_HANDLE                                        *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] This                       Protocol instance pointer.
  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
  @retval EFI_ACCESS_DENIED             Return access denied if the SmmReadyToLock event has been triggered
**/
EFI_STATUS
EFIAPI
PchPiSmmCoreUnRegister (
  IN PCH_SMM_GENERIC_PROTOCOL                           *This,
  IN EFI_HANDLE                                         *DispatchHandle
  );


/**
  Unregister a child SMI source dispatch function with a parent SMM driver.

  @param[in] This                 Pointer to the PCH_SMM_GENERIC_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
**/
EFI_STATUS
EFIAPI
PchSmmCoreUnRegister (
  IN  PCH_SMM_GENERIC_PROTOCOL                         *This,
  IN  EFI_HANDLE                                       *DispatchHandle
  );

typedef union {
  PCH_SMM_GENERIC_PROTOCOL                    Generic;
  EFI_SMM_USB_DISPATCH2_PROTOCOL              Usb;
  EFI_SMM_SX_DISPATCH2_PROTOCOL               Sx;
  EFI_SMM_SW_DISPATCH2_PROTOCOL               Sw;
  EFI_SMM_GPI_DISPATCH2_PROTOCOL              Gpi;
  EFI_SMM_POWER_BUTTON_DISPATCH2_PROTOCOL     PowerButton;
  EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL   PeriodicTimer;
} PCH_SMM_PROTOCOL;

///
/// Define a structure to help us identify the generic protocol
///
#define PROTOCOL_SIGNATURE  SIGNATURE_32 ('P', 'R', 'O', 'T')

typedef struct {
  UINTN                 Signature;

  PCH_SMM_PROTOCOL_TYPE Type;
  EFI_GUID              *Guid;
  PCH_SMM_PROTOCOL      Protocols;
} PCH_SMM_QUALIFIED_PROTOCOL;

#define QUALIFIED_PROTOCOL_FROM_GENERIC(_generic) \
  CR ( \
  _generic, \
  PCH_SMM_QUALIFIED_PROTOCOL, \
  Protocols, \
  PROTOCOL_SIGNATURE \
  )

///
/// Create private data for the protocols that we'll publish
///
typedef struct {
  LIST_ENTRY                  CallbackDataBase;
  EFI_HANDLE                  SmiHandle;
  EFI_HANDLE                  InstallMultProtHandle;
  PCH_SMM_QUALIFIED_PROTOCOL  Protocols[PCH_SMM_PROTOCOL_TYPE_MAX];
} PRIVATE_DATA;

extern PRIVATE_DATA           mPrivateData;
extern UINT16                 mAcpiBaseAddr;
extern UINT16                 mTcoBaseAddr;

/**
  The internal function used to create and insert a database record

  @param[in]  InsertRecord              Record to insert to database.
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
**/
EFI_STATUS
SmmCoreInsertRecord (
  IN  DATABASE_RECORD                   *NewRecord,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Get the Sleep type

  @param[in] Record               No use
  @param[out] Context             The context that includes SLP_TYP bits to be filled
**/
VOID
EFIAPI
SxGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  );

/**
  Register a child SMI source dispatch function for the specified software SMI.

  This service registers a function (DispatchFunction) which will be called when the software
  SMI source specified by RegisterContext->SwSmiCpuIndex is detected. On return,
  DispatchHandle contains a unique handle which may be used later to unregister the function
  using UnRegister().

  @param[in]  This                 Pointer to the EFI_SMM_SW_DISPATCH2_PROTOCOL instance.
  @param[in]  DispatchFunction     Function to register for handler when the specified software
                                   SMI is generated.
  @param[in, out] RegisterContext  Pointer to the dispatch function's context.
                                   The caller fills this context in before calling
                                   the register function to indicate to the register
                                   function which Software SMI input value the
                                   dispatch function should be invoked for.
  @param[out] DispatchHandle       Handle generated by the dispatcher to track the
                                   function instance.

  @retval EFI_SUCCESS            The dispatch function has been successfully
                                 registered and the SMI source has been enabled.
  @retval EFI_DEVICE_ERROR       The SW driver was unable to enable the SMI source.
  @retval EFI_INVALID_PARAMETER  RegisterContext is invalid. The SW SMI input value
                                 is not within a valid range or is already in use.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory (system or SMM) to manage this
                                 child.
  @retval EFI_OUT_OF_RESOURCES   A unique software SMI value could not be assigned
                                 for this dispatch.
**/
EFI_STATUS
EFIAPI
PchSwSmiRegister (
  IN  EFI_SMM_SW_DISPATCH2_PROTOCOL       *This,
  IN  EFI_SMM_HANDLER_ENTRY_POINT2        DispatchFunction,
  IN  EFI_SMM_SW_REGISTER_CONTEXT         *DispatchContext,
  OUT EFI_HANDLE                          *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function for the specified software SMI.

  This service removes the handler associated with DispatchHandle so that it will no longer be
  called in response to a software SMI.

  @param[in] This                Pointer to the EFI_SMM_SW_DISPATCH2_PROTOCOL instance.
  @param[in] DispatchHandle      Handle of dispatch function to deregister.

  @retval EFI_SUCCESS            The dispatch function has been successfully unregistered.
  @retval EFI_INVALID_PARAMETER  The DispatchHandle was not valid.
**/
EFI_STATUS
EFIAPI
PchSwSmiUnRegister (
  IN CONST EFI_SMM_SW_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                     DispatchHandle
  );

/**
  Init required protocol for Pch Sw Dispatch protocol.
**/
VOID
PchSwDispatchInit (
  VOID
  );

/**
  Check whether sleep type of two contexts match

  @param[in] Context1             Context 1 that includes sleep type 1
  @param[in] Context2             Context 2 that includes sleep type 2

  @retval FALSE                   Sleep types match
  @retval TRUE                    Sleep types don't match
**/
BOOLEAN
EFIAPI
SxCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  );

/**
  Update the elapsed time from the Interval data of DATABASE_RECORD

  @param[in] Record               The pointer to the DATABASE_RECORD.
  @param[out] HwContext           The Context to be updated.
**/
VOID
EFIAPI
PeriodicTimerGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  );

/**
  Check whether Periodic Timer of two contexts match

  @param[in] Context1             Context 1 that includes Periodic Timer  1
  @param[in] Context2             Context 2 that includes Periodic Timer  2

  @retval FALSE                   Periodic Timer match
  @retval TRUE                    Periodic Timer don't match
**/
BOOLEAN
EFIAPI
PeriodicTimerCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  );

/**
  Gather the CommBuffer information of SmmPeriodicTimerDispatch2.

  @param[in]  Record              No use
  @param[out] CommBuffer          Point to the CommBuffer structure
  @param[out] CommBufferSize      Point to the Size of CommBuffer structure
**/
VOID
EFIAPI
PeriodicTimerGetCommBuffer (
  IN  DATABASE_RECORD    *Record,
  OUT VOID               **CommBuffer,
  OUT UINTN              *CommBufferSize
  );

/**
  Get the power button status.

  @param[in] Record               The pointer to the DATABASE_RECORD.
  @param[out] Context             Calling context from the hardware, will be updated with the current power button status.
**/
VOID
EFIAPI
PowerButtonGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  );

/**
  Check whether Power Button status of two contexts match

  @param[in] Context1             Context 1 that includes Power Button status 1
  @param[in] Context2             Context 2 that includes Power Button status 2

  @retval FALSE                   Power Button status match
  @retval TRUE                    Power Button status don't match
**/
BOOLEAN
EFIAPI
PowerButtonCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  );

/**
  This function is responsible for calculating and enabling any timers that are required
  to dispatch messages to children. The SrcDesc argument isn't acutally used.

  @param[in] SrcDesc              Pointer to the PCH_SMM_SOURCE_DESC instance.
**/
VOID
EFIAPI
PchSmmPeriodicTimerClearSource (
  IN CONST PCH_SMM_SOURCE_DESC    *SrcDesc
  );

/**
  This services returns the next SMI tick period that is supported by the chipset.
  The order returned is from longest to shortest interval period.

  @param[in] This                 Pointer to the EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL instance.
  @param[in, out] SmiTickInterval Pointer to pointer of the next shorter SMI interval period that is supported by the child.

  @retval EFI_SUCCESS             The service returned successfully.
  @retval EFI_INVALID_PARAMETER   The parameter SmiTickInterval is invalid.
**/
EFI_STATUS
PchSmmPeriodicTimerDispatchGetNextShorterInterval (
  IN CONST EFI_SMM_PERIODIC_TIMER_DISPATCH2_PROTOCOL    *This,
  IN OUT UINT64                                         **SmiTickInterval
  );

/**
  Install PCH SMM periodic timer control protocol

  @param[in] Handle                     handle for this driver

  @retval EFI_SUCCESS                   Driver initialization completed successfully
**/
EFI_STATUS
EFIAPI
InstallPchSmmPeriodicTimerControlProtocol (
  IN EFI_HANDLE                         Handle
  );

/**
  When we get an SMI that indicates that we are transitioning to a sleep state,
  we need to actually transition to that state.  We do this by disabling the
  "SMI on sleep enable" feature, which generates an SMI when the operating system
  tries to put the system to sleep, and then physically putting the system to sleep.
**/
VOID
PchSmmSxGoToSleep (
  VOID
  );

/**
  Install protocols of PCH specifics SMI types, including
  PCH TCO SMI types, PCH PCIE SMI types, PCH ACPI SMI types, PCH MISC SMI types.

  @retval                               the result of protocol installation
**/
EFI_STATUS
InstallPchSmiDispatchProtocols (
  VOID
  );

/**
  The function to dispatch all callback function of PCH SMI types.

  @retval EFI_SUCCESS                   Function successfully completed
  @retval EFI_UNSUPPORTED               no
**/
EFI_STATUS
PchSmiTypeCallbackDispatcher (
  IN  DATABASE_RECORD                   *Record
  );

/**
  The register function used to register SMI handler of IoTrap event.
  This is internal function and only used by Iotrap module.

  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  IoTrapIndex               Index number of IOTRAP register
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
**/
EFI_STATUS
PchInternalIoTrapSmiRegister (
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  IN  UINTN                             IoTrapIndex,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Unregister a child SMI source dispatch function with a parent SMM driver

  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
**/
EFI_STATUS
PchInternalIoTrapSmiUnRegister (
  IN  EFI_HANDLE                        DispatchHandle
  );

/**
  Register an eSPI SMI handler based on the type

  @param[in]  DispatchFunction        Callback in an event of eSPI SMI
  @param[in]  PchSmiTypes             The eSPI type published by PchSmiDispatch
  @param[out] DispatchHandle          The callback handle

  @retval     EFI_INVALID_PARAMETER   Error with NULL SMI source description
  @retval     EFI_OUT_OF_RESOURCES    Fail to allocate pool for database record
  @retval     EFI_SUCCESS             Registration is successful.
**/
EFI_STATUS
PchInternalEspiSmiRegister (
  IN  PCH_SMI_DISPATCH_CALLBACK         DispatchFunction,
  IN  PCH_SMI_TYPES                     PchSmiTypes,
  OUT EFI_HANDLE                        *DispatchHandle
  );

/**
  Unregister an eSPI SMI handler

  @param[in] DispatchHandle             Handle of dispatch function to deregister.

  @retval EFI_SUCCESS                   The dispatch function has been successfully
                                        unregistered and the SMI source has been disabled
                                        if there are no other registered child dispatch
                                        functions for this SMI source.
  @retval EFI_INVALID_PARAMETER         Handle is invalid.
**/
EFI_STATUS
PchInternalEspiSmiUnRegister (
  IN  EFI_HANDLE                        DispatchHandle
  );

/**
  The internal function used to create and insert a database record
  for SMI record of Pch Smi types.

  @param[in]  SrcDesc                   The pointer to the SMI source description
  @param[in]  DispatchFunction          Pointer to dispatch function to be invoked for this SMI source
  @param[in]  PchSmiType                Specific SMI type of PCH SMI
  @param[out] DispatchHandle            Handle of dispatch function to register.

  @retval EFI_INVALID_PARAMETER         Error with NULL SMI source description
  @retval EFI_OUT_OF_RESOURCES          Fail to allocate pool for database record
  @retval EFI_SUCCESS                   The database record is created successfully.
**/
EFI_STATUS
PchSmiRecordInsert (
  IN  CONST PCH_SMM_SOURCE_DESC         *SrcDesc,
  IN  PCH_SMI_CALLBACK_FUNCTIONS        DispatchFunction,
  IN  PCH_SMI_TYPES                     PchSmiType,
  OUT EFI_HANDLE                        *DispatchHandle
  );

extern CONST PCH_SMM_SOURCE_DESC mSrcDescSerialIrq;
extern CONST PCH_SMM_SOURCE_DESC mSrcDescLpcBiosWp;

/**
  Clear the TCO SMI status bit and block after the SMI handling is done

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table

**/
VOID
EFIAPI
PchTcoSmiClearSourceAndBlock (
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
  );

/**
  Clear the TCO SMI status bit after the SMI handling is done

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table

**/
VOID
EFIAPI
PchTcoSmiClearSource (
  CONST PCH_SMM_SOURCE_DESC             *SrcDesc
  );

/**
  Initialize Source descriptor structure

  @param[in] SrcDesc                    Pointer to the PCH SMI source description table
**/
VOID
EFIAPI
NullInitSourceDesc (
  PCH_SMM_SOURCE_DESC                   *SrcDesc
  );

/**
  The register function used to register SMI handler of GPI SMI event.

  @param[in]  This               Pointer to the EFI_SMM_GPI_DISPATCH2_PROTOCOL instance.
  @param[in]  DispatchFunction   Function to register for handler when the specified GPI causes an SMI.
  @param[in]  RegisterContext    Pointer to the dispatch function's context.
                                 The caller fills this context in before calling
                                 the register function to indicate to the register
                                 function the GPI(s) for which the dispatch function
                                 should be invoked.
  @param[out] DispatchHandle     Handle generated by the dispatcher to track the
                                 function instance.

  @retval EFI_SUCCESS            The dispatch function has been successfully
                                 registered and the SMI source has been enabled.
  @retval EFI_ACCESS_DENIED      Register is not allowed
  @retval EFI_INVALID_PARAMETER  RegisterContext is invalid. The GPI input value
                                 is not within valid range.
  @retval EFI_OUT_OF_RESOURCES   There is not enough memory (system or SMM) to manage this child.
**/
EFI_STATUS
EFIAPI
PchGpiSmiRegister (
  IN CONST EFI_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_SMM_HANDLER_ENTRY_POINT2    DispatchFunction,
  IN       EFI_SMM_GPI_REGISTER_CONTEXT    *RegisterContext,
  OUT      EFI_HANDLE                      *DispatchHandle
  );

/**
  Unregister a GPI SMI source dispatch function with a parent SMM driver

  @param[in] This                 Pointer to the EFI_SMM_GPI_DISPATCH2_PROTOCOL instance.
  @param[in] DispatchHandle       Handle of dispatch function to deregister.

  @retval EFI_SUCCESS             The dispatch function has been successfully
                                  unregistered and the SMI source has been disabled
                                  if there are no other registered child dispatch
                                  functions for this SMI source.
  @retval EFI_INVALID_PARAMETER   Handle is invalid.
**/
EFI_STATUS
EFIAPI
PchGpiSmiUnRegister (
  IN CONST EFI_SMM_GPI_DISPATCH2_PROTOCOL  *This,
  IN       EFI_HANDLE                      DispatchHandle
  );

#endif

//========================================================================================
// edk2-platforms/Platform/Intel/MinPlatformPkg/Include/Library/AslUpdateLib.h
// edk2-platforms/Silicon/Intel/KabylakeSiliconPkg/Include/Library/AslUpdateLib.h
// edk2-platforms/Silicon/Intel/CoffeelakeSiliconPkg/Include/Library/AslUpdateLib.h
// edk2-platforms/Silicon/Intel/IntelSiliconPkg/Include/Library/AslUpdateLib.h

/** @file
  ASL dynamic update library definitions.

  This library provides dynamic update to various ASL structures.
  There may be different libraries for different environments (PEI, BS, RT, SMM).
  Make sure you meet the requirements for the library (protocol dependencies, use
  restrictions, etc).

  Note that the current version of the library updates AML.

  Copyright (c) 2020 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _ASL_UPDATE_LIB_H_
#define _ASL_UPDATE_LIB_H_

//
// Include files
//
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>


/**
  This procedure will update immediate value assigned to a Name.

  @param[in] AslSignature               The signature of Operation Region that we want to update.
  @param[in] Buffer                     source of data to be written over original aml
  @param[in] Length                     length of data to be overwritten

   @retval EFI_SUCCESS                  The function completed successfully.
   @retval EFI_NOT_FOUND                Failed to locate AcpiTable.
   @retval EFI_NOT_READY                Not ready to locate AcpiTable.
   @retval EFI_UNSUPPORTED              The function is not supported in this library.
**/
EFI_STATUS
EFIAPI
UpdateNameAslCode(
  IN     UINT32                        AslSignature,
  IN     VOID                          *Buffer,
  IN     UINTN                         Length
  );

/**
  This procedure will update immediate value assigned to a Name in SSDT table.

  @param[in] TableId           - Pointer to an ASCII string containing the OEM Table ID from the ACPI table header
  @param[in] TableIdSize       - Length of the TableId to match.  Table ID are 8 bytes long, this function
  @param[in] AslSignature      - The signature of Operation Region that we want to update.
  @param[in] Buffer            - source of data to be written over original aml
  @param[in] Length            - length of data to be overwritten

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval EFI_NOT_FOUND        - Failed to locate AcpiTable.
  @retval EFI_NOT_READY        - Not ready to locate AcpiTable.
  @retval EFI_UNSUPPORTED      - The function is not supported in this library.
**/
EFI_STATUS
EFIAPI
UpdateSsdtNameAslCode (
  IN     UINT8                         *TableId,
  IN     UINT8                         TableIdSize,
  IN     UINT32                        AslSignature,
  IN     VOID                          *Buffer,
  IN     UINTN                         Length
  );

/**
  This procedure will update the name of ASL Method.

  @param[in] AslSignature      - The signature of Operation Region that we want to update.
  @param[in] Buffer            - source of data to be written over original aml
  @param[in] Length            - length of data to be overwritten

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval EFI_NOT_FOUND        - Failed to locate AcpiTable.
  @retval EFI_NOT_READY        - Not ready to locate AcpiTable.
  @retval EFI_UNSUPPORTED      - The function is not supported in this library.
**/
EFI_STATUS
EFIAPI
UpdateMethodAslCode (
  IN     UINT32                        AslSignature,
  IN     VOID                          *Buffer,
  IN     UINTN                         Length
  );

/**
  This function uses the ACPI support protocol to locate an ACPI table.
  It is really only useful for finding tables that only have a single instance,
  e.g. FADT, FACS, MADT, etc.  It is not good for locating SSDT, etc.
  Matches are determined by finding the table with ACPI table that has
  a matching signature.

  @param[in] Signature                  Pointer to an ASCII string containing the Signature to match
  @param[in, out] Table                 Updated with a pointer to the table
  @param[in, out] Handle                AcpiSupport protocol table handle for the table found
                                        @see AcpiSupport protocol for details

  @retval EFI_SUCCESS                   The function completed successfully.
  @retval EFI_NOT_FOUND                 Failed to locate AcpiTable.
  @retval EFI_NOT_READY                 Not ready to locate AcpiTable.
  @retval EFI_UNSUPPORTED               The function is not supported in this library.
**/
EFI_STATUS
EFIAPI
LocateAcpiTableBySignature (
  IN      UINT32                        Signature,
  IN OUT  EFI_ACPI_DESCRIPTION_HEADER   **Table,
  IN OUT  UINTN                         *Handle
  );

#endif


//========================================================================================
// edk2-platforms/Silicon/Intel/TigerlakeSiliconPkg/IpBlock/CpuPcieRp/Include/CpuPcieInfo.h

/** @file
  This file contains definitions of PCIe controller information

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _CPU_PCIE_INFO_H_
#define _CPU_PCIE_INFO_H_

#define PCIE_HWEQ_COEFFS_MAX    5

//
// Device 1 Memory Mapped IO Register Offset Equates
//
#define SA_PEG_DEV_NUM     0x01
#define SA_PEG0_DEV_NUM    SA_PEG_DEV_NUM
#define SA_PEG3_DEV_NUM    0x06

//
// SA PCI Express* Port configuration
//

#define CPU_PCIE_MAX_ROOT_PORTS            4

#define SA_PEG_MAX_FUN           0x04
#define SA_PEG_MAX_LANE          0x14
#define SA_PEG_MAX_FUN_GEN3      0x03
#define SA_PEG_MAX_LANE_GEN3     0x10
#define SA_PEG_MAX_BUNDLE_GEN3   0x08

#endif


//========================================================================================
//========================================================================================

#endif // _THUNDERBOLT_H_
