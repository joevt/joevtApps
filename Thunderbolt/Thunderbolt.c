#include "Thunderbolt.h"

#include <Pi/PiPeiCis.h>

#include <Ppi/ReadOnlyVariable2.h>

#include <Library/DebugLib.h>
//#include <Library/UefiLib.h>
//#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>
//#include <Library/UefiDriverEntryPoint.h>
//#include <Library/PrintLib.h>
//#include <Library/HandleParsingLib.h>
#include <Library/DevicePathLib.h>
//#include <Library/SortLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/SmmServicesTableLib.h>

//
//#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
//#include <Protocol/GraphicsOutput.h>
//#include <Protocol/SimpleTextInEx.h>
//#include <Protocol/ComponentName.h>
//#include <Protocol/DriverBinding.h>
//#include <Protocol/DriverDiagnostics2.h>
//#include <Protocol/DriverConfiguration2.h>
//#include <Protocol/PciIo.h>
//#include <Protocol/PciRootBridgeIo.h>
//#include <Protocol/Decompress.h>
#include <Protocol/MmSxDispatch.h>

#include <IndustryStandard/Pci.h>
//#include <IndustryStandard/PeImage.h>


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Library/PeiDxeSmmTbtCommonLib/TbtCommonLib.c
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Library/PeiDxeSmmTbtCommonLib/TbtCommonLib.c
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Library/PeiDxeSmmTbtCommonLib/TbtCommonLib.c


/** @file
  PeiTbtInit library implementition with empty functions.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//#include <Library/DebugLib.h>
//#include <Uefi/UefiBaseType.h>
//#include <Library/PchPcieRpLib.h>
//#include <Library/TbtCommonLib.h>
//#include <Library/PciSegmentLib.h>
//#include <Library/TimerLib.h>
//#include <Library/BaseLib.h>
//#include <Library/GpioLib.h>


/**
  Selects the proper TBT Root port to assign resources
  based on the user input value

  @param[in]  SetupData          Pointer to Setup data

  @retval     TbtSelectorChosen  Rootport number.
**/
VOID
GetRootporttoSetResourcesforTbt (
  IN UINTN                              RpIndex,
  OUT UINT8                             *RsvdExtraBusNum,
  OUT UINT16                            *RsvdPcieMegaMem,
  OUT UINT8                             *PcieMemAddrRngMax,
  OUT UINT16                            *RsvdPciePMegaMem,
  OUT UINT8                             *PciePMemAddrRngMax,
  OUT BOOLEAN                           *SetResourceforTbt
  )
{
  UINTN TbtRpNumber;
  TbtRpNumber = (UINTN) PcdGet8 (PcdDTbtPcieRpNumber);

    if (RpIndex == (TbtRpNumber - 1)) {
        *RsvdExtraBusNum = PcdGet8 (PcdDTbtPcieExtraBusRsvd);
        *RsvdPcieMegaMem = PcdGet16 (PcdDTbtPcieMemRsvd);
        *PcieMemAddrRngMax = PcdGet8 (PcdDTbtPcieMemAddrRngMax);
        *RsvdPciePMegaMem = PcdGet16 (PcdDTbtPciePMemRsvd);
        *PciePMemAddrRngMax = PcdGet8 (PcdDTbtPciePMemAddrRngMax);
        *SetResourceforTbt = TRUE;
      }
      else {
        *SetResourceforTbt = FALSE;
      }
  }

/**
  Internal function to Wait for Tbt2PcieDone Bit.to Set or clear
  @param[in]  CommandOffsetAddress      Tbt2Pcie Register Address
  @param[in]  TimeOut                   Time out with 100 ms garnularity
  @param[in]  Tbt2PcieDone              Wait condition (wait for Bit to Clear/Set)
  @param[out] *Tbt2PcieValue Function   Register value
**/
BOOLEAN
InternalWaitforCommandCompletion(
  IN  UINT64   CommandOffsetAddress,
  IN  UINT32   TimeOut,
  IN  BOOLEAN  Tbt2PcieDone,
  OUT UINT32   *Tbt2PcieValue
  )
{
  BOOLEAN ReturnFlag;
  UINT32  Tbt2PcieCheck;

  ReturnFlag = FALSE;
  while (TimeOut-- > 0) {
    *Tbt2PcieValue = PciSegmentRead32 (CommandOffsetAddress);

    if (0xFFFFFFFF == *Tbt2PcieValue ) {
      //
      // Device is not here return now
      //
      ReturnFlag     = FALSE;
      break;
    }

    if(Tbt2PcieDone) {
      Tbt2PcieCheck  =  *Tbt2PcieValue & TBT2PCIE_DON_R;
    } else {
      Tbt2PcieCheck  = !(*Tbt2PcieValue & TBT2PCIE_DON_R);
    }

    if (Tbt2PcieCheck) {
      ReturnFlag     = TRUE;
      break;
    }

    MicroSecondDelay(TBT_MAIL_BOX_DELAY);
  }
  return ReturnFlag;
}
/**
  Get Security Level.
  @param[in]  Type      ITBT (0x80) or DTBT (0x00)
  @param[in]  Bus       Bus number Host Router (DTBT)
  @param[in]  Device    Device number for Host Router (DTBT)
  @param[in]  Function  Function number for  Host Router (DTBT)
  @param[in]  Command   Command for  Host Router (DTBT)
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
  )
{
  UINT64       Pcie2Tbt;
  UINT64       Tbt2Pcie;
  UINT32       RegisterValue;
  UINT8        ReturnFlag;

  ReturnFlag           = 0xFF;

  DEBUG ((DEBUG_INFO, "GetSecLevel() \n"));

  GET_TBT2PCIE_REGISTER_ADDRESS(Type, 0, Bus, Device, Function, Tbt2Pcie)
  GET_PCIE2TBT_REGISTER_ADDRESS(Type, 0, Bus, Device, Function, Pcie2Tbt)

  PciSegmentWrite32 (Pcie2Tbt, Command | PCIE2TBT_VLD_B);

  if(InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue)) {
    ReturnFlag     = (UINT8) (0xFF & (RegisterValue >> 8));
  }

  PciSegmentWrite32 (Pcie2Tbt, 0);

  InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, FALSE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "Security Level configured to %x \n", ReturnFlag));

  return ReturnFlag;
}

/**
  Set Security Level.
  @param[in]  Data      Security State
  @param[in]  Type      ITBT (0x80) or DTBT (0x00)
  @param[in]  Bus       Bus number for Host Router (DTBT)
  @param[in]  Device    Device number for Host Router (DTBT)
  @param[in]  Function  Function number for Host Router (DTBT)
  @param[in]  Command   Command for  Host Router (DTBT)
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
  )
{
  UINT64       Pcie2Tbt;
  UINT64       Tbt2Pcie;
  UINT32       RegisterValue;
  BOOLEAN      ReturnFlag;

  ReturnFlag   = FALSE;

  DEBUG ((DEBUG_INFO, "SetSecLevel() \n"));

  GET_TBT2PCIE_REGISTER_ADDRESS(Type, 0, Bus, Device, Function, Tbt2Pcie)
  GET_PCIE2TBT_REGISTER_ADDRESS(Type, 0, Bus, Device, Function, Pcie2Tbt)

  PciSegmentWrite32 (Pcie2Tbt, (Data << 8) | Command | PCIE2TBT_VLD_B);

  ReturnFlag = InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "RegisterValue %x \n", RegisterValue));
  PciSegmentWrite32 (Pcie2Tbt, 0);

  InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, FALSE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "Return value %x \n", ReturnFlag));
  return ReturnFlag;
}

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
)
{
  if (GpioAccessType == 0x01) {
    // PCH
    GpioSetOutputValue (GpioNumber, (UINT32)Value);
  } else if (GpioAccessType == 0x02) {
    // IoExpander {TCA6424A}
    GpioExpSetOutput (Expander, (UINT8)GpioNumber, (UINT8)Value);
  }
}

/**
Execute TBT Mail Box Command

@param[in]  Command   TBT Command
@param[in]  Type      ITBT (0x80) or DTBT (0x00)
@param[in]  Bus       Bus number for  Host Router (DTBT)
@param[in]  Device    Device number for  Host Router (DTBT)
@param[in]  Function  Function number for  Host Router (DTBT)
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
)
{
   UINT64      Pcie2Tbt;
   UINT64      Tbt2Pcie;
   UINT32      RegisterValue;
   BOOLEAN     ReturnFlag;

   GET_TBT2PCIE_REGISTER_ADDRESS(Type, 0, Bus, Device, Function, Tbt2Pcie)
   GET_PCIE2TBT_REGISTER_ADDRESS(Type, 0, Bus, Device, Function, Pcie2Tbt)

   PciSegmentWrite32 (Pcie2Tbt, Command | PCIE2TBT_VLD_B);

   ReturnFlag = InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue);

   PciSegmentWrite32(Pcie2Tbt, 0);

   return ReturnFlag;
}
/**
  Get Pch/Peg Pcie Root Port Device and Function Number for TBT by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER If Invalid Root Port Number or TYPE is Passed
**/
EFI_STATUS
EFIAPI
GetDTbtRpDevFun (
  IN  BOOLEAN Type,
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFunc
  )
{
  EFI_STATUS            Status;
  UINTN                 TbtRpDev;
  UINTN                 TbtRpFunc;

  Status = EFI_INVALID_PARAMETER; // Update the Status to EFI_SUCCESS if valid input found.
  //
  // PCH-H can support up to 24 root ports. PEG0,PEG1 and PEG2 will be
  // with device number 0x1 and Function number 0,1 and 2 respectively.
  //
  if (Type == DTBT_TYPE_PEG)
  {
    //
    //  PEG Rootport
    //
    if (RpNumber <= 2) {
      *RpDev  =   0x01;
      *RpFunc =   RpNumber;
      Status  =   EFI_SUCCESS;
    }
  }
  if (Type == DTBT_TYPE_PCH)
  {
    //
    //  PCH Rootport
    //
    if (RpNumber <= 23) {
      Status  = GetPchPcieRpDevFun (RpNumber, &TbtRpDev, &TbtRpFunc);
      *RpDev  = TbtRpDev;
      *RpFunc = TbtRpFunc;
    }
  }

  ASSERT_EFI_ERROR (Status);
  return Status;
}

BOOLEAN
IsTbtHostRouter (
  IN    UINT16  DeviceID
  )
{
  switch (DeviceID) {
  case AR_HR_2C:
  case AR_HR_4C:
  case AR_HR_LP:
  case AR_HR_C0_2C:
  case AR_HR_C0_4C:
  case TR_HR_2C:
  case TR_HR_4C:
    return TRUE;
  }

  return FALSE;
} // IsTbtHostRouter


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Library/Private/PeiDTbtInitLib/PeiDTbtInitLib.c
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Library/Private/PeiDTbtInitLib/PeiDTbtInitLib.c
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Library/Private/PeiDTbtInitLib/PeiDTbtInitLib.c

/** @file
  Thunderbolt(TM) Pei Library


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

// #include <Library/PeiServicesLib.h>
// #include <Library/MemoryAllocationLib.h>
// #include <Library/BaseMemoryLib.h>
// #include <Library/DebugLib.h>
// #include <Library/GpioLib.h>
// #include <GpioPinsSklLp.h>
// #include <GpioPinsSklH.h>
// #include <Library/TimerLib.h>
// #include <Library/IoLib.h>
// #include <Library/MmPciLib.h>
// #include <Library/HobLib.h>
// #include <Library/PcdLib.h>
// #include <Library/GpioExpanderLib.h>
// #include <Ppi/ReadOnlyVariable2.h>
// 
// #include <Base.h>
// #include <Library/TbtCommonLib.h>
// #include <TbtBoardInfo.h>
// #include <IndustryStandard/Pci22.h>
// #include <Library/PchCycleDecodingLib.h>
// #include <Ppi/PeiTbtPolicy.h>
// #include <Library/PciSegmentLib.h>
// #include <Library/PeiTbtPolicyLib.h>
// #include <Library/PchPmcLib.h>
// #include <Private/Library/PeiDTbtInitLib.h>

/**
Is host router (For dTBT) or End Point (For iTBT) present before sleep

@param[in] ControllerType - DTBT_CONTROLLER or ITBT_CONTROLLER
@param[in] Controller     - Controller begin offset of CMOS

@Retval     TRUE      There is a TBT HostRouter presented before sleep
@Retval     FALSE     There is no TBT HostRouter presented before sleep

BOOLEAN
IsHostRouterPresentBeforeSleep(
IN  UINT8        ControllerType,
IN  UINT8        Controller
)
{
  UINT8 SavedState;

  SavedState = (UINT8)GetTbtHostRouterStatus();
  if (ControllerType == DTBT_CONTROLLER){
    return ((SavedState & (DTBT_SAVE_STATE_OFFSET << Controller)) == (DTBT_SAVE_STATE_OFFSET << Controller));
  } else {
    if (ControllerType == ITBT_CONTROLLER) {
      return ((SavedState & (ITBT_SAVE_STATE_OFFSET << Controller)) == (ITBT_SAVE_STATE_OFFSET << Controller));
    }
  }
  return 0;
}
**/

/**
Execute TBT PCIE2TBT_SX_EXIT_TBT_CONNECTED Mail Box Command for S4 mode with PreBootAclEnable

@param[in]  Bus       Bus number for Host Router (DTBT)
@param[in]  Device    Device number for Host Router (DTBT)
@param[in]  Function  Function number for Host Router (DTBT)
@param[in]  Timeout   Time out with 100 ms garnularity
@Retval     true      if command executes succesfully
**/
BOOLEAN
TbtSetPcie2TbtSxExitCommandWithPreBootAclEnable(
   IN    UINT8                   Bus,
   IN    UINT8                   Device,
   IN    UINT8                   Function,
   IN    UINT32                  Timeout
)
{
  UINT64      Pcie2Tbt;
  UINT64      Tbt2Pcie;
  UINT32      RegisterValue;
  BOOLEAN     ReturnFlag;
  UINT32      Command;

  GET_TBT2PCIE_REGISTER_ADDRESS(DTBT_CONTROLLER, 0, Bus, Device, Function, Tbt2Pcie)
  GET_PCIE2TBT_REGISTER_ADDRESS(DTBT_CONTROLLER, 0, Bus, Device, Function, Pcie2Tbt)

// If PreBootAcl is Enable, we need to enable DATA bit while sending SX EXIT MAIL BOX Command
  Command = (1 << 8) | PCIE2TBT_SX_EXIT_TBT_CONNECTED;
  PciSegmentWrite32 (Pcie2Tbt, Command | PCIE2TBT_VLD_B);

  ReturnFlag = InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue);

  PciSegmentWrite32(Pcie2Tbt, 0);

  return ReturnFlag;
}

/**
Set the Sleep Mode if the HR is up.
@param[in]  Bus       Bus number for Host Router (DTBT)
@param[in]  Device    Device number for Host Router (DTBT)
@param[in]  Function  Function number for Host Router (DTBT)
**/
VOID
TbtSetSxMode(
IN    BOOLEAN                 Type,
IN    UINT8                   Bus,
IN    UINT8                   Device,
IN    UINT8                   Function,
IN    UINT8                   TbtBootOn
)
{
  UINT64                          TbtUsDevId;
  UINT64                          Tbt2Pcie;
  UINT32                          RegVal;
  UINT32                          MaxLoopCount;
  UINTN                           Delay;
  UINT8                           RetCode;
  EFI_BOOT_MODE                   BootMode;
  EFI_STATUS                      Status;

  TbtUsDevId = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, 0);
  GET_TBT2PCIE_REGISTER_ADDRESS(Type, 0, Bus, Device, Function, Tbt2Pcie)

  MaxLoopCount = TBT_5S_TIMEOUT;  // Wait 5 sec
  Delay = 100 * 1000;
  RetCode = 0x62;

  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR(Status);

  if ((BootMode == BOOT_ON_S4_RESUME) && (TbtBootOn == 2)) {
    MaxLoopCount = TBT_3S_TIMEOUT;
    if (!TbtSetPcie2TbtSxExitCommandWithPreBootAclEnable(Bus, Device, Function, MaxLoopCount)) {
      //
      // Nothing to wait, HR is not responsive
      //
      return;
    }
  }
  else {
    if (!TbtSetPcie2TbtCommand(PCIE2TBT_SX_EXIT_TBT_CONNECTED, Type, Bus, Device, Function, MaxLoopCount)) {
      //
      // Nothing to wait, HR is not responsive
      //
      return;
    }
  }

  DEBUG((DEBUG_INFO, "Wait for Dev ID != 0xFF\n"));

  while (MaxLoopCount-- > 0) {
    //
    // Check what HR still here
    //
    RegVal = PciSegmentRead32(Tbt2Pcie);
    if (0xFFFFFFFF == RegVal) {
      RetCode = 0x6F;
      break;
    }
    //
    // Check completion of TBT link
    //
    RegVal = PciSegmentRead32(TbtUsDevId);
    if (0xFFFFFFFF != RegVal) {
      RetCode = 0x61;
      break;
    }

    MicroSecondDelay(Delay);
  }

  DEBUG((DEBUG_INFO, "Return code = 0x%x\n", RetCode));
}
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
)
{
  DEBUG ((DEBUG_INFO, "DTbtSetTPch25Timing call Inside\n"));
  UINT32                PchPwrmBase;

  //
  //During boot, reboot and wake  tPCH25 Timing should be set to 10 ms
  //
  PchPwrmBaseGet (&PchPwrmBase);
  MmioOr32 (
    (UINTN) (PchPwrmBase + R_PCH_PWRM_CFG),
    (BIT0 | BIT1)
    );

  DEBUG((DEBUG_INFO, "DTbtSetTPch25Timing call Return\n"));
  return EFI_SUCCESS;
}

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
)
{

  DEBUG ((DEBUG_INFO, "DTbtForcePower call Inside\n"));

      if (PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr) {
        DEBUG((DEBUG_INFO, "ForcePwrGpio.GpioPad = %x \n", PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].ForcePwrGpio.GpioPad));
        ForceDtbtPower(PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].ForcePwrGpio.GpioAccessType,PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].ForcePwrGpio.Expander, PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].ForcePwrGpio.GpioPad, PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].ForcePwrGpio.GpioLevel);
        DEBUG((DEBUG_INFO, "ForceDtbtPower asserted \n"));
        MicroSecondDelay(PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly * 1000);
        DEBUG((DEBUG_INFO, "Delay after ForceDtbtPower = 0x%x ms \n", PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly));
      }

  DEBUG ((DEBUG_INFO, "DTbtForcePower call Return\n"));
  return EFI_SUCCESS;
}

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
)
{
  UINTN      RpDev;
  UINTN      RpFunc;
  EFI_STATUS Status;
  UINT64     BridngeBaseAddress;
  UINT16     Data16;

  DEBUG ((DEBUG_INFO, "DTbtClearVgaRegisters call Inside\n"));

  Status = EFI_SUCCESS;

  Status = GetDTbtRpDevFun(PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].Type, PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].PcieRpNumber - 1, &RpDev, &RpFunc);
  ASSERT_EFI_ERROR(Status);
  //
  // VGA Enable and VGA 16-bit decode registers of Bridge control register of Root port where
  // Host router resides should be cleaned
  //

  BridngeBaseAddress = PCI_SEGMENT_LIB_ADDRESS(0, 0, (UINT32)RpDev, (UINT32)RpFunc, 0);
  Data16 = PciSegmentRead16(BridngeBaseAddress + PCI_BRIDGE_CONTROL_REGISTER_OFFSET);
  Data16 &= (~(EFI_PCI_BRIDGE_CONTROL_VGA | EFI_PCI_BRIDGE_CONTROL_VGA_16));
  PciSegmentWrite16(BridngeBaseAddress + PCI_BRIDGE_CONTROL_REGISTER_OFFSET, Data16);

  DEBUG ((DEBUG_INFO, "DTbtClearVgaRegisters call Return\n"));
  return Status;
}

/**
  Exectue Mail box command "Boot On".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtBootOn(
  IN  PEI_TBT_POLICY  *PeiTbtConfig,
  IN  UINTN DTbtControllerNumber
)
{
  EFI_STATUS Status;
  UINT32     OrgBusNumberConfiguration;
  UINTN      RpDev;
  UINTN      RpFunc;

  DEBUG((DEBUG_INFO, "DTbtBootOn call Inside\n"));

  Status = EFI_SUCCESS;

      Status = GetDTbtRpDevFun(PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].Type, PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].PcieRpNumber - 1, &RpDev, &RpFunc);
      ASSERT_EFI_ERROR(Status);
      OrgBusNumberConfiguration = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
      //
      // Set Sec/Sub buses to 0xF0
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), 0x00F0F000);
      //
      //When Thunderbolt(TM) boot [TbtBootOn] is enabled in bios setup we need to do the below:
      //Bios should send "Boot On" message through PCIE2TBT register
      //The Boot On command as described above would include the command and acknowledge from FW (with the default timeout in BIOS),
      //once the Boot On command is completed it is guaranteed that the AlpineRidge(AR) device is there and the PCI tunneling was done by FW,
      //next step from BIOS is enumeration using SMI
      //

      if (PeiTbtConfig->DTbtCommonConfig.TbtBootOn > 0) {
        //
        // Exectue Mail box command "Boot On / Pre-Boot ACL"
        //
        //Command may be executed only during boot/reboot and not during Sx exit flow
        if (PeiTbtConfig->DTbtCommonConfig.TbtBootOn == 1) {
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_BOOT_ON, DTBT_CONTROLLER, 0xF0, 0, 0, TBT_5S_TIMEOUT)) {
            //
            // Nothing to wait, HR is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> DTbtBootOn - Boot On message sent failed \n"));
          }
        }
        if (PeiTbtConfig->DTbtCommonConfig.TbtBootOn == 2) {
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_PREBOOTACL, DTBT_CONTROLLER, 0xF0, 0, 0, TBT_3S_TIMEOUT)) {
            //
            // Nothing to wait, HR is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> DTbtBootOn - Pre-Boot ACL message sent failed \n"));
          }
        }
      }
      //
      // Reset Sec/Sub buses to original value
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBusNumberConfiguration);

  DEBUG((DEBUG_INFO, "DTbtBootOn call Return\n"));
  return Status;
}

/**
  Exectue Mail box command "USB On".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtUsbOn(
  IN  PEI_TBT_POLICY  *PeiTbtConfig,
  IN  UINTN DTbtControllerNumber
)
{
  EFI_STATUS                      Status;
  UINTN                           RpDev;
  UINTN                           RpFunc;
  UINT32                          OrgBusNumberConfiguration;
  UINT64                          TbtBaseAddress;
  UINT32                          MaxWaitIter;
  UINT32                          RegVal;
  EFI_BOOT_MODE                   BootMode;

  DEBUG((DEBUG_INFO, "DTbtUsbOn call Inside\n"));

  Status = EFI_SUCCESS;

      Status = GetDTbtRpDevFun(PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].Type, PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].PcieRpNumber - 1, &RpDev, &RpFunc);
      ASSERT_EFI_ERROR(Status);
      OrgBusNumberConfiguration = PciSegmentRead32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
      //
      // Set Sec/Sub buses to 0xF0
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), 0x00F0F000);

      //
      //When Thunderbolt(TM) Usb boot [TbtUsbOn] is enabled in bios setup we need to do the below:
      //Bios should send "Usb On" message through PCIE2TBT register
      //The Usb On command as described above would include the command and acknowledge from FW (with the default timeout in BIOS),
      //once the Usb On command is completed it is guaranteed that the AlpineRidge(AR) device is there and the PCI tunneling was done by FW,
      //next step from BIOS is enumeration using SMI
      //
      if (PeiTbtConfig->DTbtCommonConfig.TbtUsbOn) {
        if (PeiTbtConfig->DTbtCommonConfig.TbtBootOn > 0) {
          MaxWaitIter = 50;   // Wait 5 sec
          TbtBaseAddress = PCI_SEGMENT_LIB_ADDRESS(0, 0xF0, 0, 0, 0);
          //
          // Driver clears the PCIe2TBT Valid bit to support two consicutive mailbox commands
          //
          PciSegmentWrite32(TbtBaseAddress + PCIE2TBT_DTBT_R, 0);
          DEBUG((DEBUG_INFO, "TbtBaseAddress + PCIE2TBT_DTBT_R = 0x%lx \n", TbtBaseAddress + PCIE2TBT_DTBT_R));
          while (MaxWaitIter-- > 0) {
            RegVal = PciSegmentRead32(TbtBaseAddress + TBT2PCIE_DTBT_R);
            if (0xFFFFFFFF == RegVal) {
              //
              // Device is not here return now
              //
              DEBUG((DEBUG_INFO, "TBT device is not present \n"));
              break;
            }

            if (!(RegVal & TBT2PCIE_DON_R)) {
              break;
            }
            MicroSecondDelay(100 * 1000);
          }
        }

        Status = PeiServicesGetBootMode(&BootMode);
        ASSERT_EFI_ERROR(Status);

        //
        // Exectue Mail box command "Usb On"
        //
        //Command may be executed only during boot/reboot and not during S3 exit flow
        //In case of S4 Exit send USB ON cmd only if Host Router was inactive/not present during S4 entry
        if (BootMode == BOOT_ON_S4_RESUME ) {
          // USB_ON cmd not required
        } else {
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_USB_ON, DTBT_CONTROLLER, 0xF0, 0, 0, TBT_5S_TIMEOUT)) {
            //
            // Nothing to wait, HR is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> TbtBootSupport - Usb On message sent failed \n"));
          }
        }
      }
      //
      // Reset Sec/Sub buses to original value
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBusNumberConfiguration);

  DEBUG((DEBUG_INFO, "DTbtUsbOn call return\n"));
  return Status;
}

/**
  Exectue Mail box command "Sx Exit".

  @param[in]  PEI_TBT_POLICY   PeiTbtConfig

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSxExitFlow(
  IN  PEI_TBT_POLICY  *PeiTbtConfig,
  IN  UINTN DTbtControllerNumber
)
{
  EFI_STATUS                      Status;
  UINT32                          OrgBusNumberConfiguration;
  UINTN                           RpDev;
  UINTN                           RpFunc;
  UINT32                          Count;

  DEBUG((DEBUG_INFO, "DTbtSxExitFlow call Inside\n"));

  Status = EFI_SUCCESS;
  Count = 0;

      Status = GetDTbtRpDevFun(PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].Type, PeiTbtConfig-> DTbtControllerConfig [DTbtControllerNumber].PcieRpNumber - 1, &RpDev, &RpFunc);
      ASSERT_EFI_ERROR(Status);
      OrgBusNumberConfiguration = PciSegmentRead32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
      //
      // Set Sec/Sub buses to 0xF0
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), 0x00F0F000);

      if ( PeiTbtConfig->DTbtCommonConfig.TbtBootOn == 2) {
        //
        // WA: When system with TBT 3.1 device, resume SX system need to wait device ready. In document that maximum time out should be 500ms.
        //
        while (PciSegmentRead32(PCI_SEGMENT_LIB_ADDRESS(0, 0xf0, 0x0, 0x0, 0x08)) == 0xffffffff) { //End Device will be with Device Number 0x0, Function Number 0x0.
          MicroSecondDelay(STALL_ONE_MICRO_SECOND * 1000); // 1000usec
          Count++;
          if (Count > 10000) { //Allowing Max Delay of 10 sec for CFL-S board.
          break;
          }
        }

        //
        // Upon wake, if BIOS saved pre-Sx Host Router state as active (system went to sleep with
        // attached devices), BIOS should:
        // 1. Execute "Sx_Exit_TBT_Connected" mailbox command.
        // 2. If procedure above returns true, BIOS should perform "wait for fast link bring-up" loop
        // 3. Continue regular wake flow.
        //
        //
        // Exectue Mail box command and perform "wait for fast link bring-up" loop
        //
        TbtSetSxMode(DTBT_CONTROLLER, 0xF0, 0, 0, PeiTbtConfig->DTbtCommonConfig.TbtBootOn);
      }
      //
      // Reset Sec/Sub buses to original value
      //
      PciSegmentWrite32(PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBusNumberConfiguration);

  DEBUG((DEBUG_INFO, "DTbtSxExitFlow call Return\n"));
  return Status;
}


/**
  Initialize Thunderbolt(TM)

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
TbtInit (
  IN  UINTN DTbtControllerNumber
  )
{
  EFI_STATUS            Status;
  PEI_TBT_POLICY             *PeiTbtConfig;

  //
  // Get the TBT Policy
  //
  Status = PeiServicesLocatePpi (
             &gPeiTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiTbtPolicyPpiGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);
  //
  // Exectue Mail box command "Boot On"
  //
  Status = DTbtBootOn (PeiTbtConfig, DTbtControllerNumber);
  //
  // Exectue Mail box command "Usb On"
  //
  Status = DTbtUsbOn (PeiTbtConfig, DTbtControllerNumber);
  //
  //During boot, reboot and wake  (bits [1:0]) of PCH PM_CFG register should be
  //set to 11b - 10 ms (default value is 0b - 10 us)
  //
  Status = DTbtSetTPch25Timing (PeiTbtConfig);
  //
  // Configure Tbt Force Power
  //
  Status = DTbtForcePower (PeiTbtConfig, DTbtControllerNumber);
  //
  // VGA Enable and VGA 16-bit decode registers of Bridge control register of Root port where
  // Host router resides should be cleaned
  //
  Status = DTbtClearVgaRegisters (PeiTbtConfig, DTbtControllerNumber);
  //
  // Upon wake, if BIOS saved pre-Sx Host Router state as active (system went to sleep with
  // attached devices), BIOS should:
  // 1. Execute "Sx_Exit_TBT_Connected" mailbox command.
  // 2. If procedure above returns true, BIOS should perform "wait for fast link bring-up" loop
  // 3. Continue regular wake flow.
  //
  Status = DTbtSxExitFlow (PeiTbtConfig, DTbtControllerNumber);
  return EFI_SUCCESS;
}


//========================================================================================
// edk2-platforms//Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Library/PeiTbtPolicyLib/PeiTbtPolicyLib.c
// edk2-platforms//Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Library/PeiTbtPolicyLib/PeiTbtPolicyLib.c
// edk2-platforms//Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Library/PeiTbtPolicyLib/PeiTbtPolicyLib.c

/** @file
  This file is PeiTbtPolicyLib library.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//#include <Library/PeiServicesLib.h>
//#include <Library/GpioLib.h>
//#include <PiPei.h>
//#include <PeiTbtPolicyLibrary.h>
//#include <Ppi/ReadOnlyVariable2.h>
//#include <Ppi/PeiTbtPolicy.h>
//#include <Base.h>
//#include <GpioConfig.h>

/**
  Update PEI TBT Policy Callback
**/
VOID
EFIAPI
UpdatePeiTbtPolicy (
  VOID
  )
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  PEI_TBT_POLICY                   *PeiTbtConfig;

  PeiTbtConfig = NULL;
  Status = EFI_NOT_FOUND;

  DEBUG ((DEBUG_INFO, "UpdatePeiTbtPolicy \n"));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesLocatePpi (
             &gPeiTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiTbtPolicyPpiGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);

for (UINT8 Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
  //
  // Update DTBT Policy
  //
  PeiTbtConfig-> DTbtControllerConfig [Index].DTbtControllerEn = PcdGet8 (PcdDTbtControllerEn);
  if (PcdGet8 (PcdDTbtControllerType) == TYPE_PEG)
  {
    PeiTbtConfig-> DTbtControllerConfig [Index].Type = (UINT8) TYPE_PEG;
    PeiTbtConfig-> DTbtControllerConfig [Index].PcieRpNumber = 1; // PEG RP 1 (Function no. 0)
  }
  else {
    PeiTbtConfig-> DTbtControllerConfig [Index].PcieRpNumber = PcdGet8 (PcdDTbtPcieRpNumber);
    PeiTbtConfig-> DTbtControllerConfig [Index].Type = PcdGet8 (PcdDTbtControllerType);
  }
  PeiTbtConfig->DTbtControllerConfig [Index].CioPlugEventGpio.GpioPad = (GPIO_PAD) PcdGet32 (PcdDTbtCioPlugEventGpioPad);
  if (GpioCheckFor2Tier(PeiTbtConfig->DTbtControllerConfig [Index].CioPlugEventGpio.GpioPad)) {
    PeiTbtConfig->DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignaturePorting = 0;
    PeiTbtConfig->DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignature = SIGNATURE_32('X', 'T', 'B', 'T');
  }
  else {
    PeiTbtConfig->DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignaturePorting = 1;
    //
    // Update Signature based on platform GPIO.
    //
    PeiTbtConfig->DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignature = SIGNATURE_32('X', 'T', 'B', 'T');
  }
}
  PeiTbtConfig->DTbtCommonConfig.TbtBootOn = PcdGet8 (PcdDTbtBootOn);
  PeiTbtConfig->DTbtCommonConfig.TbtUsbOn = PcdGet8 (PcdDTbtUsbOn);
  PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr = PcdGet8 (PcdDTbtGpio3ForcePwr);
  PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly = PcdGet16 (PcdDTbtGpio3ForcePwrDly);

  return;
}

/**
  Print PEI TBT Policy
**/
VOID
EFIAPI
TbtPrintPeiPolicyConfig (
  VOID
  )
{
  DEBUG_CODE_BEGIN ();
  EFI_STATUS                       Status;
  PEI_TBT_POLICY                   *PeiTbtConfig;

  PeiTbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  DEBUG ((DEBUG_INFO, "TbtPrintPolicyConfig Start\n"));

  Status = PeiServicesLocatePpi (
             &gPeiTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiTbtPolicyPpiGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);

  //
  // Print DTBT Policy
  //
  DEBUG ((DEBUG_INFO, "\n------------------------ TBT Policy (PEI) Print BEGIN -----------------\n"));
  DEBUG ((DEBUG_INFO, "Revision : 0x%x\n", PEI_TBT_POLICY_REVISION));
  DEBUG ((DEBUG_INFO, "------------------------ PEI_TBT_CONFIG  -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", PEI_TBT_POLICY_REVISION));

  for (UINT8 Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].DTbtControllerEn = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].DTbtControllerEn));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].Type = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].Type));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].PcieRpNumber = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].PcieRpNumber));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].ForcePwrGpio.GpioPad = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].ForcePwrGpio.GpioPad));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].ForcePwrGpio.GpioLevel = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].ForcePwrGpio.GpioLevel));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].PcieRstGpio.GpioPad = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].PcieRstGpio.GpioPad));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].PcieRstGpio.GpioLevel = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].PcieRstGpio.GpioLevel));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].CioPlugEventGpio.GpioPad = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].CioPlugEventGpio.GpioPad));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].CioPlugEventGpio.AcpiGpeSignature = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignature));
    DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtControllerConfig[%x].CioPlugEventGpio.AcpiGpeSignaturePorting = %x\n", Index, PeiTbtConfig-> DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignaturePorting));
  }

  //
  // Print DTBT Common Policy
  //
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.TbtBootOn = %x\n", PeiTbtConfig->DTbtCommonConfig.TbtBootOn));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.TbtUsbOn = %x\n", PeiTbtConfig->DTbtCommonConfig.TbtUsbOn));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr = %x\n", PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly = %x\n", PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.DTbtSharedGpioConfiguration = %x\n", PeiTbtConfig->DTbtCommonConfig.DTbtSharedGpioConfiguration));
  DEBUG ((DEBUG_INFO, "PeiTbtConfig->DTbtCommonConfig.PcieRstSupport = %x\n", PeiTbtConfig->DTbtCommonConfig.PcieRstSupport));

  DEBUG ((DEBUG_INFO, "\n------------------------ TBT Policy (PEI) Print END -----------------\n"));
  DEBUG_CODE_END ();

  return;
}

/**
  Install Tbt Policy

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallPeiTbtPolicy (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_PEI_PPI_DESCRIPTOR        *PeiTbtPolicyPpiDesc;
  PEI_TBT_POLICY                *PeiTbtConfig;

  DEBUG ((DEBUG_INFO, "Install PEI TBT Policy\n"));

  PeiTbtConfig = NULL;

  //
  // Allocate memory for PeiTbtPolicyPpiDesc
  //
  PeiTbtPolicyPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  ASSERT (PeiTbtPolicyPpiDesc != NULL);
  if (PeiTbtPolicyPpiDesc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Allocate memory and initialize all default to zero for PeiTbtPolicy
  //
  PeiTbtConfig = (PEI_TBT_POLICY *) AllocateZeroPool (sizeof (PEI_TBT_POLICY));
  ASSERT (PeiTbtConfig != NULL);
  if (PeiTbtConfig == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize PPI
  //
  PeiTbtPolicyPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  PeiTbtPolicyPpiDesc->Guid = &gPeiTbtPolicyPpiGuid;
  PeiTbtPolicyPpiDesc->Ppi = PeiTbtConfig;

  Status = PeiServicesInstallPpi (PeiTbtPolicyPpiDesc);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Install PEI TBT Policy failed\n"));
  }
  return Status;
}


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/TbtInit/Pei/PeiTbtInit.c
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/TbtInit/Pei/PeiTbtInit.c
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/TbtInit/Pei/PeiTbtInit.c

/** @file
  Source code file for TBT Init PEI module


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//#include <Library/IoLib.h>
//#include <Library/HobLib.h>
//#include <Library/DebugLib.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/PeiServicesLib.h>
//#include <Library/PciSegmentLib.h>
//#include <Library/PeiTbtPolicyLib.h>
//#include <Ppi/SiPolicy.h>
//#include <Ppi/PeiTbtPolicy.h>
//#include <Ppi/EndOfPeiPhase.h>
//#include <TbtBoardInfo.h>
//#include <Private/Library/PeiDTbtInitLib.h>

/**
  This function Update and Print PEI TBT Policy after TbtPolicyBoardInitDone

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/


/**
  This function pass PEI TBT Policy to Hob at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/


EFI_STATUS
EFIAPI
PassTbtPolicyToHob (
VOID
  )
{
  EFI_STATUS            Status;
  EFI_BOOT_MODE         BootMode;
  TBT_INFO_HOB          *TbtInfoHob;
  PEI_TBT_POLICY        *PeiTbtConfig;

  DEBUG ((DEBUG_INFO, "PassTbtPolicyToHob\n"));

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  if (BootMode == BOOT_ON_S3_RESUME ) {
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (
             &gPeiTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiTbtPolicyPpiGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);

  //
  // Create HOB for TBT Data
  //
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TBT_INFO_HOB),
             (VOID **) &TbtInfoHob
             );
  DEBUG ((DEBUG_INFO, "TbtInfoHob Created \n"));
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize the TBT INFO HOB data.
  //
  TbtInfoHob->EfiHobGuidType.Name = gTbtInfoHobGuid;


  for (UINT8 Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
    //
    // Update DTBT Policy
    //
    TbtInfoHob-> DTbtControllerConfig [Index].DTbtControllerEn = PeiTbtConfig-> DTbtControllerConfig [Index].DTbtControllerEn;
    TbtInfoHob-> DTbtControllerConfig [Index].Type = PeiTbtConfig-> DTbtControllerConfig [Index].Type;
    TbtInfoHob-> DTbtControllerConfig [Index].PcieRpNumber = PeiTbtConfig-> DTbtControllerConfig [Index].PcieRpNumber;
    TbtInfoHob-> DTbtControllerConfig [Index].ForcePwrGpio.GpioPad = PeiTbtConfig-> DTbtControllerConfig [Index].ForcePwrGpio.GpioPad;
    TbtInfoHob-> DTbtControllerConfig [Index].ForcePwrGpio.GpioLevel = PeiTbtConfig-> DTbtControllerConfig [Index].ForcePwrGpio.GpioLevel;
    TbtInfoHob-> DTbtControllerConfig [Index].CioPlugEventGpio.GpioPad = PeiTbtConfig-> DTbtControllerConfig [Index].CioPlugEventGpio.GpioPad;
    TbtInfoHob-> DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignature = PeiTbtConfig-> DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignature;
    TbtInfoHob-> DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignaturePorting = PeiTbtConfig-> DTbtControllerConfig [Index].CioPlugEventGpio.AcpiGpeSignaturePorting;
    TbtInfoHob-> DTbtControllerConfig [Index].PcieRstGpio.GpioPad = PeiTbtConfig-> DTbtControllerConfig [Index].PcieRstGpio.GpioPad;
    TbtInfoHob-> DTbtControllerConfig [Index].PcieRstGpio.GpioLevel = PeiTbtConfig-> DTbtControllerConfig [Index].PcieRstGpio.GpioLevel;
  }

  TbtInfoHob->DTbtCommonConfig.TbtBootOn = PeiTbtConfig->DTbtCommonConfig.TbtBootOn;
  TbtInfoHob->DTbtCommonConfig.TbtUsbOn = PeiTbtConfig->DTbtCommonConfig.TbtUsbOn;
  TbtInfoHob->DTbtCommonConfig.Gpio3ForcePwr = PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwr;
  TbtInfoHob->DTbtCommonConfig.Gpio3ForcePwrDly = PeiTbtConfig->DTbtCommonConfig.Gpio3ForcePwrDly;
  TbtInfoHob->DTbtCommonConfig.DTbtSharedGpioConfiguration = PeiTbtConfig->DTbtCommonConfig.DTbtSharedGpioConfiguration;
  TbtInfoHob->DTbtCommonConfig.PcieRstSupport = PeiTbtConfig->DTbtCommonConfig.PcieRstSupport;

  return EFI_SUCCESS;
}


/**
  This function handles TbtInit task at the end of PEI

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/

EFI_STATUS
EFIAPI
TbtInitEndOfPei (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         DTbtExisted;
  PEI_TBT_POLICY  *PeiTbtConfig;

  DEBUG ((DEBUG_INFO, "TbtInitEndOfPei Entry\n"));

  Status       = EFI_SUCCESS;
  PeiTbtConfig = NULL;

  Status = PeiServicesLocatePpi (
             &gPeiTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiTbtConfig
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, " gPeiTbtPolicyPpiGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);

  for (UINT8 Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
    DTbtExisted  = FALSE;
    if (PeiTbtConfig-> DTbtControllerConfig [Index].DTbtControllerEn == 1) {
      DTbtExisted = TRUE;
    }

    if (DTbtExisted == TRUE) {
      //
      // Call Init function
      //
      Status = TbtInit (Index);
    }
  }

  return EFI_SUCCESS;
}

/**
  TBT Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
TbtInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS     Status;

  DEBUG ((DEBUG_INFO, "TBT PEI EntryPoint\n"));

  //
  // Install PEI TBT Policy
  //
  Status = InstallPeiTbtPolicy ();
  ASSERT_EFI_ERROR (Status);


  UpdatePeiTbtPolicy ();

  TbtPrintPeiPolicyConfig ();
  //
  // Performing PassTbtPolicyToHob and TbtInitEndOfPei
  //
  Status = PassTbtPolicyToHob ();

  Status = TbtInitEndOfPei ();

  return Status;
}


//========================================================================================
// edk2-platforms//Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLib.c
// edk2-platforms//Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLib.c
// edk2-platforms//Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/Library/DxeTbtPolicyLib/DxeTbtPolicyLib.c


/** @file
  This file is DxeTbtPolicyLib library.

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//#include <DxeTbtPolicyLibrary.h>
//#include <TbtBoardInfo.h>
//#include <Protocol/DxeTbtPolicy.h>
//#include <Guid/HobList.h>
//#include <Library/HobLib.h>


/**
  Update Tbt Policy Callback
Need to add PCDs for setup options
**/

VOID
EFIAPI
UpdateTbtPolicyCallback (
  VOID
  )
{
  EFI_STATUS                     Status;
  DXE_TBT_POLICY_PROTOCOL        *DxeTbtConfig;

  DxeTbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  DEBUG ((DEBUG_INFO, "UpdateTbtPolicyCallback\n"));

  Status = gBS->LocateProtocol (
                  &gDxeTbtPolicyProtocolGuid,
                  NULL,
                  (VOID **) &DxeTbtConfig
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " gDxeTbtPolicyProtocolGuid Not installed!!!\n"));
  } else {

	for (UINT8 Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
		DxeTbtConfig->DTbtResourceConfig[Index].DTbtPcieExtraBusRsvd   = PcdGet8 (PcdDTbtPcieExtraBusRsvd);
		DxeTbtConfig->DTbtResourceConfig[Index].DTbtPcieMemRsvd        = PcdGet16 (PcdDTbtPcieMemRsvd);
		DxeTbtConfig->DTbtResourceConfig[Index].DTbtPcieMemAddrRngMax  = PcdGet8 (PcdDTbtPcieMemAddrRngMax);
		DxeTbtConfig->DTbtResourceConfig[Index].DTbtPciePMemRsvd       = PcdGet16 (PcdDTbtPciePMemRsvd);
		DxeTbtConfig->DTbtResourceConfig[Index].DTbtPciePMemAddrRngMax = PcdGet8 (PcdDTbtPciePMemAddrRngMax);
	}

    DxeTbtConfig->TbtCommonConfig.TbtAspm          = PcdGet8 (PcdDTbtAspm);
    DxeTbtConfig->TbtCommonConfig.TbtHotNotify     = PcdGet8 (PcdDTbtHotNotify);
    DxeTbtConfig->TbtCommonConfig.TbtHotSMI        = PcdGet8 (PcdDTbtHotSMI);
    DxeTbtConfig->TbtCommonConfig.TbtSetClkReq     = PcdGet8 (PcdDTbtSetClkReq);
    DxeTbtConfig->TbtCommonConfig.TbtWakeupSupport = PcdGet8 (PcdDTbtWakeupSupport);
    DxeTbtConfig->TbtCommonConfig.SecurityMode     = PcdGet8 (PcdDTbtSecurityMode);

    DxeTbtConfig->TbtCommonConfig.Gpio5Filter      = PcdGet8 (PcdDTbtGpio5Filter);
    DxeTbtConfig->TbtCommonConfig.TbtAcDcSwitch    = PcdGet8 (PcdDTbtAcDcSwitch);

    DxeTbtConfig->TbtCommonConfig.Rtd3Tbt          = PcdGet8 (PcdRtd3Tbt);
    DxeTbtConfig->TbtCommonConfig.Rtd3TbtOffDelay  = PcdGet16 (PcdRtd3TbtOffDelay);
    DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReq    = PcdGet8 (PcdRtd3TbtClkReq);
    DxeTbtConfig->TbtCommonConfig.Win10Support    = PcdGet8 (PcdDTbtWin10Support);
  }

  return;
}

/**
  Print DXE TBT Policy
**/
VOID
TbtPrintDxePolicyConfig (
  VOID
  )
{
  EFI_STATUS                       Status;
  UINT8                            Index;
  DXE_TBT_POLICY_PROTOCOL          *DxeTbtConfig;

  DEBUG ((DEBUG_INFO, "TbtPrintDxePolicyConfig Start\n"));

  DxeTbtConfig = NULL;
  Status = EFI_NOT_FOUND;
  Status = gBS->LocateProtocol (
                  &gDxeTbtPolicyProtocolGuid,
                  NULL,
                  (VOID **) &DxeTbtConfig
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, " gDxeTbtPolicyProtocolGuid Not installed!!!\n"));
  }
  ASSERT_EFI_ERROR (Status);
  //
  // Print DTBT Policy
  //
  DEBUG ((DEBUG_ERROR, " ========================= DXE TBT POLICY ========================= \n"));
  for (Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
    DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig[%x].DTbtPcieExtraBusRsvd = %x\n", Index, DxeTbtConfig->DTbtResourceConfig[Index].DTbtPcieExtraBusRsvd));
    DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig[%x].DTbtPcieMemRsvd = %x\n", Index, DxeTbtConfig->DTbtResourceConfig[Index].DTbtPcieMemRsvd));
    DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig[%x].DTbtPcieMemAddrRngMax = %x\n", Index, DxeTbtConfig->DTbtResourceConfig[Index].DTbtPcieMemAddrRngMax));
    DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig[%x].DTbtPciePMemRsvd = %x\n", Index, DxeTbtConfig->DTbtResourceConfig[Index].DTbtPciePMemRsvd));
    DEBUG ((DEBUG_INFO, "DxeTbtConfig->DTbtResourceConfig[%x].DTbtPciePMemAddrRngMax = %x\n", Index, DxeTbtConfig->DTbtResourceConfig[Index].DTbtPciePMemAddrRngMax));
  }

  //
  // Print TBT Common Policy
  //
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtAspm = %x\n", DxeTbtConfig->TbtCommonConfig.TbtAspm));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtL1SubStates = %x\n", DxeTbtConfig->TbtCommonConfig.TbtL1SubStates));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtHotNotify = %x\n", DxeTbtConfig->TbtCommonConfig.TbtHotNotify));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtHotSMI = %x\n", DxeTbtConfig->TbtCommonConfig.TbtHotSMI));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtLtr = %x\n", DxeTbtConfig->TbtCommonConfig.TbtLtr));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtPtm = %x\n", DxeTbtConfig->TbtCommonConfig.TbtPtm));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtSetClkReq = %x\n", DxeTbtConfig->TbtCommonConfig.TbtSetClkReq));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtWakeupSupport = %x\n", DxeTbtConfig->TbtCommonConfig.TbtWakeupSupport));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.SecurityMode = %x\n", DxeTbtConfig->TbtCommonConfig.SecurityMode));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Gpio5Filter = %x\n", DxeTbtConfig->TbtCommonConfig.Gpio5Filter));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TrA0OsupWa = %x\n", DxeTbtConfig->TbtCommonConfig.TrA0OsupWa));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtAcDcSwitch = %x\n", DxeTbtConfig->TbtCommonConfig.TbtAcDcSwitch));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Rtd3Tbt = %x\n", DxeTbtConfig->TbtCommonConfig.Rtd3Tbt));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Rtd3TbtOffDelay = %x\n", DxeTbtConfig->TbtCommonConfig.Rtd3TbtOffDelay));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReq = %x\n", DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReq));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReqDelay = %x\n", DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReqDelay));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.Win10Support = %x\n", DxeTbtConfig->TbtCommonConfig.Win10Support));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.TbtVtdBaseSecurity = %x\n", DxeTbtConfig->TbtCommonConfig.TbtVtdBaseSecurity));
  DEBUG ((DEBUG_INFO, "DxeTbtConfig->TbtCommonConfig.ControlIommu = %x\n", DxeTbtConfig->TbtCommonConfig.ControlIommu));
  return;
}

/**
  Install Tbt Policy

  @param[in] ImageHandle                Image handle of this driver.

  @retval EFI_SUCCESS                   The policy is installed.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer

**/
EFI_STATUS
EFIAPI
InstallTbtPolicy (
  IN  EFI_HANDLE                    ImageHandle
  )
{
  EFI_STATUS                    Status;
  DXE_TBT_POLICY_PROTOCOL       *DxeTbtPolicy;

  DEBUG ((DEBUG_INFO, "Install DXE TBT Policy\n"));

  DxeTbtPolicy = NULL;
  //Alloc memory for DxeTbtPolicy
  DxeTbtPolicy = (DXE_TBT_POLICY_PROTOCOL *) AllocateZeroPool (sizeof (DXE_TBT_POLICY_PROTOCOL));
  if (DxeTbtPolicy == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gDxeTbtPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  DxeTbtPolicy
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Install Tbt Secure Boot List protocol failed\n"));
  }
  return Status;
}


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmm.c
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmm.c
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmm.c

/** @file

`  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Module specific Includes
//
//#include <Library/BaseMemoryLib.h>
//#include <Library/BaseLib.h>
//#include <Library/GpioLib.h>
//#include <TbtBoardInfo.h>
//#include <Protocol/TbtNvsArea.h>
//#include <PchAccess.h>
//#include <Library/BaseLib.h>
//#include <Library/PciSegmentLib.h>
//#include <Library/PchInfoLib.h>
//#include <Library/IoLib.h>
//#include <Library/SmmServicesTableLib.h>
//#include <Protocol/SmmSxDispatch2.h>
//#include <Protocol/SmmSwDispatch2.h>
//#include <Uefi/UefiSpec.h>
//#include <Library/UefiLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/DebugLib.h>
//#include <Library/HobLib.h>
//#include <Guid/HobList.h>
//#include "TbtSmiHandler.h"
//#include <PcieRegs.h>
//#include <Protocol/SaPolicy.h>
//#include <Protocol/DxeTbtPolicy.h>
//#include <Library/PchPmcLib.h>
#define P2P_BRIDGE                    (((PCI_CLASS_BRIDGE) << 8) | (PCI_CLASS_BRIDGE_P2P))

#define CMD_BM_MEM_IO                 (CMD_BUS_MASTER | BIT1 | BIT0)

#define DISBL_IO_REG1C                0x01F1
#define DISBL_MEM32_REG20             0x0000FFF0
#define DISBL_PMEM_REG24              0x0001FFF1

#define DOCK_BUSSES                   8

#define PCI_CAPABILITY_ID_PCIEXP      0x10
#define PCI_CAPBILITY_POINTER_OFFSET  0x34

#define LTR_MAX_SNOOP_LATENCY_VALUE             0x0846    ///< Intel recommended maximum value for Snoop Latency  can we put like this ?
#define LTR_MAX_NON_SNOOP_LATENCY_VALUE         0x0846    ///< Intel recommended maximum value for Non-Snoop Latency can we put like this ?


GLOBAL_REMOVE_IF_UNREFERENCED TBT_NVS_AREA                *mTbtNvsAreaPtr;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       gCurrentDiscreteTbtRootPort;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       gCurrentDiscreteTbtRootPortType;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16                      TbtLtrMaxSnoopLatency;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16                      TbtLtrMaxNoSnoopLatency;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                       gDTbtPcieRstSupport;
GLOBAL_REMOVE_IF_UNREFERENCED TBT_INFO_HOB                *gTbtInfoHob = NULL;
STATIC UINTN                                              mPciExpressBaseAddress;
STATIC UINT8                TbtSegment        = 0;
VOID
GpioWrite (
  IN  UINT32         GpioNumber,
  IN  BOOLEAN        Value
  )
{
  GpioSetOutputValue (GpioNumber, (UINT32)Value);
}

/**
  Search and return the offset of desired Pci Express Capability ID
  CAPID list:
    0x0001 = Advanced Error Reporting Capability
    0x0002 = Virtual Channel Capability
    0x0003 = Device Serial Number Capability
    0x0004 = Power Budgeting Capability

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                Extended CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT16
PcieFindExtendedCapId (
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT16  CapId
  )
{
  UINT16  CapHeaderOffset;
  UINT16  CapHeaderId;
  UINT64  DeviceBase;

  DeviceBase = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Device, Function, 0);

  ///
  /// Start to search at Offset 0x100
  /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  ///
  CapHeaderId     = 0;
  CapHeaderOffset = 0x100;
  while (CapHeaderOffset != 0 && CapHeaderId != 0xFFFF) {
    CapHeaderId = PciSegmentRead16 (DeviceBase + CapHeaderOffset);
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }
    ///
    /// Each capability must be DWORD aligned.
    /// The bottom two bits of all pointers are reserved and must be implemented as 00b
    /// although software must mask them to allow for future uses of these bits.
    ///
    CapHeaderOffset = (PciSegmentRead16 (DeviceBase + CapHeaderOffset + 2) >> 4) & ((UINT16) ~(BIT0 | BIT1));
  }

  return 0;
}

/**
  Find the Offset to a given Capabilities ID
  CAPID list:
    0x01 = PCI Power Management Interface
    0x04 = Slot Identification
    0x05 = MSI Capability
    0x10 = PCI Express Capability

  @param[in] Bus                  Pci Bus Number
  @param[in] Device               Pci Device Number
  @param[in] Function             Pci Function Number
  @param[in] CapId                CAPID to search for

  @retval 0                       CAPID not found
  @retval Other                   CAPID found, Offset of desired CAPID
**/
UINT8
PcieFindCapId (
  IN UINT8   Segment,
  IN UINT8   Bus,
  IN UINT8   Device,
  IN UINT8   Function,
  IN UINT8   CapId
  )
{
  UINT8   CapHeaderOffset;
  UINT8   CapHeaderId;
  UINT64  DeviceBase;

  DeviceBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);

  if ((PciSegmentRead8 (DeviceBase + PCI_PRIMARY_STATUS_OFFSET) & EFI_PCI_STATUS_CAPABILITY) == 0x00) {
    ///
    /// Function has no capability pointer
    ///
    return 0;
  }

  ///
  /// Check the header layout to determine the Offset of Capabilities Pointer Register
  ///
  if ((PciSegmentRead8 (DeviceBase + PCI_HEADER_TYPE_OFFSET) & HEADER_LAYOUT_CODE) == (HEADER_TYPE_CARDBUS_BRIDGE)) {
    ///
    /// If CardBus bridge, start at Offset 0x14
    ///
    CapHeaderOffset = 0x14;
  } else {
    ///
    /// Otherwise, start at Offset 0x34
    ///
    CapHeaderOffset = 0x34;
  }
  ///
  /// Get Capability Header, A pointer value of 00h is used to indicate the last capability in the list.
  ///
  CapHeaderId     = 0;
  CapHeaderOffset = PciSegmentRead8 (DeviceBase + CapHeaderOffset) & ((UINT8) ~(BIT0 | BIT1));
  while (CapHeaderOffset != 0 && CapHeaderId != 0xFF) {
    CapHeaderId = PciSegmentRead8 (DeviceBase + CapHeaderOffset);
    if (CapHeaderId == CapId) {
      return CapHeaderOffset;
    }
    ///
    /// Each capability must be DWORD aligned.
    /// The bottom two bits of all pointers (including the initial pointer at 34h) are reserved
    /// and must be implemented as 00b although software must mask them to allow for future uses of these bits.
    ///
    CapHeaderOffset = PciSegmentRead8 (DeviceBase + CapHeaderOffset + 1) & ((UINT8) ~(BIT0 | BIT1));
  }

  return 0;
}
/**
  This function configures the L1 Substates.
  It can be used for Rootport and endpoint devices.

  @param[in] DownstreamPort               Indicates if the device about to be programmed is a downstream port
  @param[in] DeviceBase                   Device PCI configuration base address
  @param[in] L1SubstateExtCapOffset       Pointer to L1 Substate Capability Structure
  @param[in] PortL1SubstateCapSupport     L1 Substate capability setting
  @param[in] PortCommonModeRestoreTime    Common Mode Restore Time
  @param[in] PortTpowerOnValue            Tpower_on Power On Wait Time
  @param[in] PortTpowerOnScale            Tpower-on Scale

  @retval none
**/
VOID
ConfigureL1s (
  IN UINTN                              DeviceBase,
  IN UINT16                             L1SubstateExtCapOffset,
  IN UINT32                             PortL1SubstateCapSupport,
  IN UINT32                             PortCommonModeRestoreTime,
  IN UINT32                             PortTpowerOnValue,
  IN UINT32                             PortTpowerOnScale,
  IN UINT16                             MaxLevel
  )
{

  PciSegmentAndThenOr32 (
    DeviceBase + L1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
    (UINT32) ~(0xFF00),
    (UINT32) PortCommonModeRestoreTime << 8
    );

  PciSegmentAnd32(DeviceBase + L1SubstateExtCapOffset + R_PCIE_EX_L1SCTL2_OFFSET, 0xFFFFFF04);

  PciSegmentOr32(DeviceBase + L1SubstateExtCapOffset + R_PCIE_EX_L1SCTL2_OFFSET,(UINT32) ((PortTpowerOnValue << N_PCIE_EX_L1SCTL2_POWT) | PortTpowerOnScale));

  PciSegmentAndThenOr32 (
    DeviceBase + L1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
    (UINT32) ~(0xE3FF0000),
    (UINT32) (BIT30 | BIT23 | BIT21)
    );

}

VOID
RootportL1sSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT16  RootL1SubstateExtCapOffset,
  IN UINT16  MaxL1Level
  )
{
  UINTN       ComponentABaseAddress;
  UINTN       ComponentBBaseAddress;
  UINT8       SecBus;
  UINT32      PortL1SubstateCapSupport;
  UINT32      PortCommonModeRestoreTime;
  UINT32      PortTpowerOnValue;
  UINT32      PortTpowerOnScale;
  UINT16      ComponentBL1SubstateExtCapOffset;
  UINT32      ComponentBL1Substates;
  UINT32      ComponentBCommonModeRestoreTime;
  UINT32      ComponentBTpowerOnValue;
  UINT32      ComponentBTpowerOnScale;
  UINT32      Data32;

  PortL1SubstateCapSupport  = 0;
  PortCommonModeRestoreTime = 0;
  PortTpowerOnValue = 0;
  PortTpowerOnScale = 0;
  Data32 = 0;

  ComponentABaseAddress  = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  if (RootL1SubstateExtCapOffset != 0) {
    Data32 = PciSegmentRead32 (ComponentABaseAddress + RootL1SubstateExtCapOffset + R_PCIE_EX_L1SCAP_OFFSET);
    PortL1SubstateCapSupport  = (Data32) & 0x0F;
    PortCommonModeRestoreTime = (Data32 >> 8) & 0xFF;
    PortTpowerOnScale         = (Data32 >> 16) & 0x3;
    PortTpowerOnValue         = (Data32 >> 19) & 0x1F;
  } else {
    MaxL1Level                = 0; // If L1 Substates from Root Port side is disable, then Disable from Device side also.
  }

  SecBus                = PciSegmentRead8 (ComponentABaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  ComponentBBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, SecBus, 0, 0, 0);

  if (PciSegmentRead16 (ComponentBBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    ComponentBL1SubstateExtCapOffset = PcieFindExtendedCapId (
                                  SecBus,
                                  0,
                                  0,
                                  V_PCIE_EX_L1S_CID
                                  );
    if (ComponentBL1SubstateExtCapOffset != 0) {
      ComponentBL1Substates = PciSegmentRead32 (ComponentBBaseAddress + ComponentBL1SubstateExtCapOffset + R_PCIE_EX_L1SCAP_OFFSET);
      ComponentBCommonModeRestoreTime = (ComponentBL1Substates >> 8) & 0xFF;
      ComponentBTpowerOnScale         = (ComponentBL1Substates >> 16) & 0x3;
      ComponentBTpowerOnValue         = (ComponentBL1Substates >> 19) & 0x1F;

      if (MaxL1Level == 3) {
        if (Data32 >= ComponentBL1Substates) {
          if (~(Data32 | BIT2)) {
            MaxL1Level = 1;
          }
        }
        else {
          if (~(ComponentBL1Substates | BIT2)) {
          MaxL1Level = 1;
        }
      }
    }

      if (MaxL1Level == 3) {
        ConfigureL1s (
          ComponentABaseAddress,
          RootL1SubstateExtCapOffset,
          PortL1SubstateCapSupport,
          ComponentBCommonModeRestoreTime,
          ComponentBTpowerOnValue,
          ComponentBTpowerOnScale,
          MaxL1Level
          );

      ConfigureL1s (
          ComponentBBaseAddress,
          ComponentBL1SubstateExtCapOffset,
          ComponentBL1Substates,
          PortCommonModeRestoreTime,
          PortTpowerOnValue,
          PortTpowerOnScale,
          MaxL1Level
          );
      }

      if (MaxL1Level == 1) {
        PciSegmentOr32 (
          ComponentABaseAddress + RootL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
          (UINT32) (BIT3 | BIT1)
          );

        PciSegmentOr32 (
          ComponentBBaseAddress + ComponentBL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
          (UINT32) (BIT3 | BIT1)
          );
      }
      else {
        if (RootL1SubstateExtCapOffset != 0) {
          PciSegmentOr32 (
            ComponentABaseAddress + RootL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
            (UINT32) (BIT3 | BIT1)
            );

          PciSegmentOr32 (
            ComponentABaseAddress + RootL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
            (UINT32) (BIT2 | BIT0)
            );
        }
        if (ComponentBL1SubstateExtCapOffset != 0) {
          PciSegmentOr32 (
            ComponentBBaseAddress + ComponentBL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
            (UINT32) (BIT3 | BIT1)
           );

          PciSegmentOr32 (
            ComponentBBaseAddress + ComponentBL1SubstateExtCapOffset + R_PCIE_EX_L1SCTL1_OFFSET,
            (UINT32) (BIT2 | BIT0)
            );
        }
      }
    }
  }
}

VOID
MultiFunctionDeviceAspm (
  IN UINT8   Bus,
  IN UINT8   Dev
  )
{
  UINT16  LowerAspm;
  UINT16  AspmVal;
  UINT8   Fun;
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffset;

  LowerAspm = 3; // L0s and L1 Supported
  for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
    //
    // Check for Device availability
    //
    DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
    if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
      // Device not present
      continue;
    }

    CapHeaderOffset = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);

    AspmVal = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;
    if (LowerAspm > AspmVal) {
      LowerAspm = AspmVal;
    }
  } //Fun

  for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
    //
    // Check for Device availability
    //
    DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
    if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
      //
      // Device not present
      //
      continue;
    }

    CapHeaderOffset = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);

    PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, LowerAspm);
  } //Fun
}

UINT16
LimitAspmLevel (
  IN UINT16  SelectedAspm,
  IN UINT16  MaxAspmLevel
  )
{
  SelectedAspm = SelectedAspm & MaxAspmLevel;

  return SelectedAspm;
}

UINT16
FindOptimalAspm (
  IN UINT16   ComponentAaspm,
  IN UINT16   ComponentBaspm
  )
{
  UINT16  SelectedAspm;

  SelectedAspm = ComponentAaspm & ComponentBaspm;

  return SelectedAspm;
}

UINT16
FindComponentBaspm (
  IN UINT8   Bus,
  IN UINT8   MaxBus
  )
{
  UINT8   BusNo;
  UINT8   DevNo;
  UINT8   FunNo;
  UINT64  DevBaseAddress;
  UINT8   RegVal;
  UINT8   SecBusNo;
  UINT16  SelectedAspm; // No ASPM Support
  UINT8   CapHeaderOffset_B;
  BOOLEAN AspmFound;

  SelectedAspm  = 0;
  AspmFound     = FALSE;

  for (BusNo = MaxBus; (BusNo != 0xFF) && (!AspmFound); --BusNo) {
    for (DevNo = 0; (DevNo <= PCI_MAX_DEVICE) && (!AspmFound); ++DevNo) {
      for (FunNo = 0; (FunNo <= PCI_MAX_FUNC) && (!AspmFound); ++FunNo) {
        //
        // Check for Device availability
        //
        DevBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, BusNo, DevNo, FunNo, 0);
        if (PciSegmentRead16 (DevBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          //
          // Device not present
          //
          continue;
        }

        RegVal = PciSegmentRead8 (DevBaseAddress + PCI_HEADER_TYPE_OFFSET);
        if ((RegVal & (BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5 + BIT6)) != 0x01) {
          //
          // Not a PCI-to-PCI bridges device
          //
          continue;
        }

        SecBusNo = PciSegmentRead8 (DevBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);

        if (SecBusNo == Bus) {
          //
          // This is the Rootbridge for the given 'Bus' device
          //
          CapHeaderOffset_B = PcieFindCapId (TbtSegment, BusNo, DevNo, FunNo, 0x10);
          SelectedAspm      = (PciSegmentRead16 (DevBaseAddress + CapHeaderOffset_B + 0x00C) >> 10) & 3;
          AspmFound         = TRUE;
        }
      } //FunNo
    } //DevNo
  } //BusNo

  return (SelectedAspm);
}

VOID
NoAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset
  )
{
  UINT64 DeviceBaseAddress;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, 0x00);
}

VOID
EndpointAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset,
  IN UINT8   MaxBus,
  IN UINT16  MaxAspmLevel
  )
{
  UINT64  DeviceBaseAddress;
  UINT16  ComponentAaspm;
  UINT16  ComponentBaspm;
  UINT16  SelectedAspm;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  ComponentAaspm    = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;
  ComponentBaspm    = FindComponentBaspm (Bus, MaxBus);
  SelectedAspm      = FindOptimalAspm (ComponentAaspm, ComponentBaspm);
  SelectedAspm      = LimitAspmLevel (SelectedAspm, MaxAspmLevel);
  PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, SelectedAspm);
}

VOID
UpstreamAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset,
  IN UINT8   MaxBus,
  IN UINT16  MaxAspmLevel
  )
{
  UINT64  DeviceBaseAddress;
  UINT16  ComponentAaspm;
  UINT16  ComponentBaspm;
  UINT16  SelectedAspm;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  ComponentAaspm    = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;
  ComponentBaspm    = FindComponentBaspm (Bus, MaxBus);
  SelectedAspm      = FindOptimalAspm (ComponentAaspm, ComponentBaspm);
  SelectedAspm      = LimitAspmLevel (SelectedAspm, MaxAspmLevel);
  PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, SelectedAspm);
}

VOID
DownstreamAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset,
  IN UINT16  MaxAspmLevel
  )
{
  UINT64  ComponentABaseAddress;
  UINT64  ComponentBBaseAddress;
  UINT16  ComponentAaspm;
  UINT16  ComponentBaspm;
  UINT16  SelectedAspm;
  UINT8   SecBus;
  UINT8   CapHeaderOffset_B;

  ComponentABaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  ComponentAaspm        = (PciSegmentRead16 (ComponentABaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;

  SecBus                = PciSegmentRead8 (ComponentABaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  ComponentBBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, SecBus, 0, 0, 0);
  ComponentBaspm        = 0; // No ASPM Support
  if (PciSegmentRead16 (ComponentBBaseAddress + PCI_DEVICE_ID_OFFSET) != 0xFFFF) {
    CapHeaderOffset_B = PcieFindCapId (TbtSegment, SecBus, 0, 0, 0x10);
    ComponentBaspm    = (PciSegmentRead16 (ComponentBBaseAddress + CapHeaderOffset_B + 0x00C) >> 10) & 3;
  }

  SelectedAspm = FindOptimalAspm (ComponentAaspm, ComponentBaspm);
  SelectedAspm = LimitAspmLevel (SelectedAspm, MaxAspmLevel);
  PciSegmentAndThenOr16 (ComponentABaseAddress + CapHeaderOffset + 0x10, 0xFFFC, SelectedAspm);
}

VOID
RootportAspmSupport (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT8   CapHeaderOffset,
  IN UINT16  MaxAspmLevel
  )
{
  UINT64  ComponentABaseAddress;
  UINT64  ComponentBBaseAddress;
  UINT16  ComponentAaspm;
  UINT16  ComponentBaspm;
  UINT16  SelectedAspm;
  UINT8   SecBus;
  UINT8   CapHeaderOffset_B;

  ComponentABaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  ComponentAaspm        = (PciSegmentRead16 (ComponentABaseAddress + CapHeaderOffset + 0x00C) >> 10) & 3;

  SecBus                = PciSegmentRead8 (ComponentABaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  ComponentBBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, SecBus, 0, 0, 0);
  ComponentBaspm        = 0; // No ASPM Support
  if (PciSegmentRead16 (ComponentBBaseAddress + PCI_DEVICE_ID_OFFSET) != 0xFFFF) {
    CapHeaderOffset_B = PcieFindCapId (TbtSegment, SecBus, 0, 0, 0x10);
    ComponentBaspm    = (PciSegmentRead16 (ComponentBBaseAddress + CapHeaderOffset_B + 0x00C) >> 10) & 3;
  }

  SelectedAspm = FindOptimalAspm (ComponentAaspm, ComponentBaspm);
  SelectedAspm = LimitAspmLevel (SelectedAspm, MaxAspmLevel);
  PciSegmentAndThenOr16 (ComponentABaseAddress + CapHeaderOffset + 0x10, 0xFFFC, SelectedAspm);
}

VOID
ThunderboltEnableAspmWithoutLtr (
  IN   UINT16     MaxAspmLevel,
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   RootBus;
  UINT8   RootDev;
  UINT8   RootFun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;
  UINT8   RegVal;
  UINT8   CapHeaderOffset;
  UINT16  DevicePortType;

  MinBus  = 0;
  MaxBus  = 0;

  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;

  RootBus = (UINT8)RpBus;
  RootDev = (UINT8)RpDevice;
  RootFun = (UINT8)RpFunction;

  //
  //  Enumerate all the bridges and devices which are available on TBT host controller
  //
  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      //
      // Check for Device availability
      //
      DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, 0, 0);
      if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
        //
        // Device not present
        //
        continue;
      }

      RegVal = PciSegmentRead8 (DeviceBaseAddress + PCI_HEADER_TYPE_OFFSET);
      if ((RegVal & BIT7) == 0) {
        //
        // Not a multi-function device
        //
        continue;
      }

      MultiFunctionDeviceAspm(Bus, Dev);
    } //Dev
  } //Bus


  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          //
          // Device not present
          //
          continue;
        }

        CapHeaderOffset = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);
        DevicePortType  = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x002) >> 4) & 0xF;
        if(PciSegmentRead8 (DeviceBaseAddress + PCI_CLASSCODE_OFFSET) == PCI_CLASS_SERIAL) {
          MaxAspmLevel = (UINT16) 0x1;
        }

        switch (DevicePortType) {
        case 0:
          //
          // PCI Express Endpoint
          //
          EndpointAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        case 1:
          //
          // Legacy PCI Express Endpoint
          //
          EndpointAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        case 4:
          //
          // Root Port of PCI Express Root Complex
          //
          RootportAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxAspmLevel);
          break;

        case 5:
          //
          // Upstream Port of PCI Express Switch
          //
          UpstreamAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        case 6:
          //
          // Downstream Port of PCI Express Switch
          //
          DownstreamAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxAspmLevel);
          break;

        case 7:
          //
          // PCI Express to PCI/PCI-X Bridge
          //
          NoAspmSupport (Bus, Dev, Fun, CapHeaderOffset);
          break;

        case 8:
          //
          // PCI/PCI-X to PCI Express Bridge
          //
          NoAspmSupport (Bus, Dev, Fun, CapHeaderOffset);
          break;

        case 9:
          //
          // Root Complex Integrated Endpoint
          //
          EndpointAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        case 10:
          //
          // Root Complex Event Collector
          //
          EndpointAspmSupport (Bus, Dev, Fun, CapHeaderOffset, MaxBus, MaxAspmLevel);
          break;

        default:
          break;
        }
        //
        // switch(DevicePortType)
        //
      }
      //
      // Fun
      //
    }
    //
    // Dev
    //
  }
  //
  // Bus
  //
  CapHeaderOffset = PcieFindCapId (TbtSegment, RootBus, RootDev, RootFun, 0x10);
  RootportAspmSupport (RootBus, RootDev, RootFun, CapHeaderOffset, MaxAspmLevel);
}



VOID
ThunderboltEnableL1Sub (
  IN   UINT16     MaxL1Level,
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT16  CapHeaderOffsetExtd;

  RpBus   = 0;

  CapHeaderOffsetExtd = PcieFindExtendedCapId ((UINT8) RpBus, (UINT8) RpDevice, (UINT8) RpFunction, V_PCIE_EX_L1S_CID);
  RootportL1sSupport ((UINT8) RpBus, (UINT8) RpDevice, (UINT8) RpFunction, CapHeaderOffsetExtd, MaxL1Level);
}

VOID
ThunderboltDisableAspmWithoutLtr (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   RootBus;
  UINT8   RootDev;
  UINT8   RootFun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffset;

  MinBus  = 0;
  MaxBus  = 0;

  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;
  RootBus = (UINT8)RpBus;
  RootDev = (UINT8)RpDevice;
  RootFun = (UINT8)RpFunction;

  //
  //  Enumerate all the bridges and devices which are available on TBT host controller
  //
  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          //
          // Device not present
          //
          continue;
        }

        CapHeaderOffset = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);
        PciSegmentAndThenOr16 (DeviceBaseAddress + CapHeaderOffset + 0x10, 0xFFFC, 0x00);
      } //Fun
    } //Dev
  } //Bus

  CapHeaderOffset = PcieFindCapId (TbtSegment, RootBus, RootDev, RootFun, 0x10);
  NoAspmSupport(RootBus, RootDev, RootFun, CapHeaderOffset);
}

VOID
TbtProgramClkReq (
  IN        UINT8  Bus,
  IN        UINT8  Device,
  IN        UINT8  Function,
  IN        UINT8  ClkReqSetup
  )
{
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffset;
  UINT16  Data16;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Device, Function, 0);
  CapHeaderOffset   = PcieFindCapId (TbtSegment, Bus, Device, Function, 0x10);

  //
  // Check if CLKREQ# is supported
  //
  if ((PciSegmentRead32 (DeviceBaseAddress + CapHeaderOffset + 0x0C) & BIT18) != 0) {
    Data16 = PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x010);

    if (ClkReqSetup) {
      Data16 = Data16 | BIT8; // Enable Clock Power Management
    } else {
      Data16 =  Data16 & (UINT16)(~BIT8); // Disable Clock Power Management
    }

    PciSegmentWrite16 (DeviceBaseAddress + CapHeaderOffset + 0x010, Data16);
  }
}
VOID
TbtProgramPtm(
   IN        UINT8  Bus,
   IN        UINT8  Device,
   IN        UINT8  Function,
   IN        UINT8  PtmSetup,
   IN        BOOLEAN IsRoot
)
{
   UINT64  DeviceBaseAddress;
   UINT16  CapHeaderOffset;
   UINT16  PtmControlRegister;
   UINT16  PtmCapabilityRegister;

   DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS(TbtSegment, Bus, Device, Function, 0);
   CapHeaderOffset = PcieFindExtendedCapId(Bus, Device, Function, 0x001F /*V_PCIE_EX_PTM_CID*/);
   if(CapHeaderOffset != 0) {
      PtmCapabilityRegister = PciSegmentRead16(DeviceBaseAddress + CapHeaderOffset + 0x04);
     //
     // Check if PTM Requester/ Responder capability for the EP/Down stream etc
     //
     if ((PtmCapabilityRegister & (BIT1 | BIT0)) != 0) {
        PtmControlRegister = PciSegmentRead16(DeviceBaseAddress + CapHeaderOffset + 0x08);

        if (PtmSetup) {
           PtmControlRegister = PtmControlRegister | BIT0; // Enable PTM
           if(IsRoot) {
             PtmControlRegister = PtmControlRegister | BIT1; // Enable PTM
           }
           PtmControlRegister = PtmControlRegister | (PtmCapabilityRegister & 0xFF00); // Programm Local Clock Granularity
        } else {
           PtmControlRegister = PtmControlRegister & (UINT16)(~(BIT0 | BIT1)); // Disable Clock Power Management
        }

        PciSegmentWrite16(DeviceBaseAddress + CapHeaderOffset + 0x08, PtmControlRegister);
     }
   }
}

VOID
ConfigureTbtPm (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction,
  IN   UINT8      Configuration    // 1- Clk Request , 2- PTM ,
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;

  MinBus  = 0;
  MaxBus  = 0;

  if ((Configuration != 1) && (Configuration != 2)) {
    return;
  }
  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;
  //
  //  Enumerate all the bridges and devices which are available on TBT host controller
  //
  for (Bus = MaxBus; Bus >= MinBus; --Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          if (Fun == 0) {
            //
            // IF Fun is zero, stop enumerating other functions of the particular bridge
            //
            break;
          }
          //
          // otherwise, just skip checking for CLKREQ support
          //
          continue;
        }
        switch (Configuration) {
          case 1:
            TbtProgramClkReq (Bus, Dev, Fun, (UINT8) mTbtNvsAreaPtr->TbtSetClkReq);
            break;
          case 2:
            TbtProgramPtm (Bus, Dev, Fun, (UINT8) mTbtNvsAreaPtr->TbtPtm, FALSE);
            TbtProgramPtm((UINT8) RpBus, (UINT8) RpDevice, (UINT8) RpFunction, (UINT8) mTbtNvsAreaPtr->TbtPtm, TRUE);
            break;
          default:
            break;
        }
      } //Fun
    } // Dev
  } // Bus
}

/**
  1) Check LTR support in device capabilities 2 register (bit 11).
  2) If supported enable LTR in device control 2 register (bit 10).

**/
VOID
TbtProgramLtr (
  IN        UINT8  Bus,
  IN        UINT8  Device,
  IN        UINT8  Function,
  IN        UINT8  LtrSetup
  )
{
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffset;
  UINT16  Data16;

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Device, Function, 0);
  CapHeaderOffset   = PcieFindCapId (TbtSegment, Bus, Device, Function, 0x10);

  //
  // Check if LTR# is supported
  //
  if ((PciSegmentRead32 (DeviceBaseAddress + CapHeaderOffset + 0x24) & BIT11) != 0) {
    Data16 = PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffset + 0x028);

    if (LtrSetup) {
      Data16 = Data16 | BIT10; // LTR Mechanism Enable
    } else {
      Data16 =  Data16 & (UINT16)(~BIT10); // LTR Mechanism Disable
    }

    PciSegmentWrite16 (DeviceBaseAddress + CapHeaderOffset + 0x028, Data16);
  }
}

VOID
ConfigureLtr (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;

  MinBus  = 0;
  MaxBus  = 0;

  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;
  //
  //  Enumerate all the bridges and devices which are available on TBT host controller
  //
  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          if (Fun == 0) {
            //
            // IF Fun is zero, stop enumerating other functions of the particular bridge
            //
            break;
          }
          //
          // otherwise, just skip checking for LTR support
          //
          continue;
        }

        TbtProgramLtr (Bus, Dev, Fun, (UINT8) mTbtNvsAreaPtr->TbtLtr);

      } //Fun
    } // Dev
  } // Bus
  TbtProgramLtr ((UINT8) RpBus, (UINT8) RpDevice, (UINT8) RpFunction, (UINT8) mTbtNvsAreaPtr->TbtLtr);
}

/*
  US ports and endpoints which declare support must also have the LTR capability structure (cap ID 18h).
  In this structure you need to enter the max snoop latency and max non-snoop latency in accordance with the format specified in the PCIe spec.
  The latency value itself is platform specific so you'll need to get it from the platform architect or whatever.
*/
VOID
ThunderboltGetLatencyLtr (
  VOID
  )
{
  PCH_SERIES       PchSeries;

  PchSeries = GetPchSeries ();

  if(gCurrentDiscreteTbtRootPortType == DTBT_TYPE_PEG) {
  // PEG selector
  TbtLtrMaxSnoopLatency = LTR_MAX_SNOOP_LATENCY_VALUE;
  TbtLtrMaxNoSnoopLatency = LTR_MAX_NON_SNOOP_LATENCY_VALUE;
  } else if (gCurrentDiscreteTbtRootPortType == DTBT_TYPE_PCH) {
  // PCH selector

    if (PchSeries == PchLp) {
      TbtLtrMaxSnoopLatency = 0x1003;
      TbtLtrMaxNoSnoopLatency = 0x1003;
    }
    if (PchSeries == PchH) {
      TbtLtrMaxSnoopLatency = 0x0846;
      TbtLtrMaxNoSnoopLatency = 0x0846;
    }
  }
}

VOID
SetLatencyLtr (
  IN UINT8   Bus,
  IN UINT8   Dev,
  IN UINT8   Fun,
  IN UINT16  CapHeaderOffsetExtd,
  IN UINT16  LtrMaxSnoopLatency,
  IN UINT16  LtrMaxNoSnoopLatency
  )
{
  UINT64 DeviceBaseAddress;
  if(CapHeaderOffsetExtd == 0) {
    return;
  }
  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
  PciSegmentWrite16 (DeviceBaseAddress + CapHeaderOffsetExtd + 0x004, LtrMaxSnoopLatency);
  PciSegmentWrite16 (DeviceBaseAddress + CapHeaderOffsetExtd + 0x006, LtrMaxNoSnoopLatency);
}

VOID
ThunderboltSetLatencyLtr (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   MinBus;
  UINT8   MaxBus;
  UINT16  DeviceId;
  UINT64  DeviceBaseAddress;
  UINT8   CapHeaderOffsetStd;
  UINT16  CapHeaderOffsetExtd;
  UINT16  DevicePortType;

  MinBus  = 0;
  MaxBus  = 0;

  MinBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET));
  MaxBus    = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET));
  DeviceId  = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (RpSegment, MinBus, 0x00, 0x00, PCI_DEVICE_ID_OFFSET));
  if (!(IsTbtHostRouter (DeviceId))) {
    return;
  }

  TbtSegment = (UINT8)RpSegment;

  for (Bus = MinBus; Bus <= MaxBus; ++Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        //
        // Check for Device availability
        //
        DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, Bus, Dev, Fun, 0);
        if (PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
          //
          // Device not present
          //
          continue;
        }

        CapHeaderOffsetStd = PcieFindCapId (TbtSegment, Bus, Dev, Fun, 0x10);
        DevicePortType  = (PciSegmentRead16 (DeviceBaseAddress + CapHeaderOffsetStd + 0x002) >> 4) & 0xF;

        CapHeaderOffsetExtd = PcieFindExtendedCapId (Bus, Dev, Fun, 0x0018);

        switch (DevicePortType) {
        case 0:
          //
          // PCI Express Endpoint
          //
          SetLatencyLtr (Bus, Dev, Fun, CapHeaderOffsetExtd, TbtLtrMaxSnoopLatency, TbtLtrMaxNoSnoopLatency);
          break;

        case 1:
          //
          // Legacy PCI Express Endpoint
          //
          SetLatencyLtr (Bus, Dev, Fun, CapHeaderOffsetExtd, TbtLtrMaxSnoopLatency, TbtLtrMaxNoSnoopLatency);
          break;

        case 4:
          //
          // Root Port of PCI Express Root Complex
          //
          // Do-nothing
          break;

        case 5:
          //
          // Upstream Port of PCI Express Switch
          //
          SetLatencyLtr (Bus, Dev, Fun, CapHeaderOffsetExtd, TbtLtrMaxSnoopLatency, TbtLtrMaxNoSnoopLatency);
          break;

        case 6:
          //
          // Downstream Port of PCI Express Switch
          //
          // Do-nothing
          break;

        case 7:
          //
          // PCI Express to PCI/PCI-X Bridge
          //
          // Do-nothing
          break;

        case 8:
          //
          // PCI/PCI-X to PCI Express Bridge
          //
          // Do-nothing
          break;

        case 9:
          //
          // Root Complex Integrated Endpoint
          //
          // Do-nothing
          break;

        case 10:
          //
          // Root Complex Event Collector
          //
          // Do-nothing
          break;

        default:
          break;
        }
        //
        // switch(DevicePortType)
        //
      }
      //
      // Fun
      //
    }
    //
    // Dev
    //
  }
  //
  // Bus
  //
}

static
VOID
Stall (
  UINTN     Usec
  )
{
  UINTN   Index;
  UINT32  Data32;
  UINT32  PrevData;
  UINTN   Counter;

  Counter = (UINTN) ((Usec * 10) / 3);
  //
  // Call WaitForTick for Counter + 1 ticks to try to guarantee Counter tick
  // periods, thus attempting to ensure Microseconds of stall time.
  //
  if (Counter != 0) {

    PrevData = IoRead32 (PcdGet16 (PcdAcpiBaseAddress) + R_PCH_ACPI_PM1_TMR);
    for (Index = 0; Index < Counter;) {
      Data32 = IoRead32 (PcdGet16 (PcdAcpiBaseAddress) + R_PCH_ACPI_PM1_TMR);
      if (Data32 < PrevData) {
        //
        // Reset if there is a overlap
        //
        PrevData = Data32;
        continue;
      }

      Index += (Data32 - PrevData);
      PrevData = Data32;
    }
  }

  return ;
}
/**
  Called during Sx entry, initates TbtSetPcie2TbtCommand HandShake to set GO2SX_NO_WAKE
  for Tbt devices if WakeupSupport is not present.

  @param[in] DispatchHandle         - The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in] DispatchContext        - Points to an optional handler context which was specified when the
                                      handler was registered.
  @param[in, out] CommBuffer        - A pointer to a collection of data in memory that will
                                      be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize    - The size of the CommBuffer.

  @retval EFI_SUCCESS               - The interrupt was handled successfully.
**/
EFI_STATUS
EFIAPI
SxDTbtEntryCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer OPTIONAL,
  IN  UINTN                         *CommBufferSize OPTIONAL
  )
{
  UINT16          DeviceId;
  UINT8           CableConnected;
  UINT8           RootportSelected;
  UINT8           HoustRouteBus;
  volatile UINT32 *PowerState;
  UINT32          PowerStatePrev;
  BOOLEAN         SecSubBusAssigned;
  UINT64          DeviceBaseAddress;
  UINT8           CapHeaderOffset;
  UINTN           RpDev;
  UINTN           RpFunc;
  EFI_STATUS      Status;
  UINT32          Timeout;
  UINT32          RegisterValue;
  UINT64          Tbt2Pcie;
  UINTN           Index;
  UINT32          TbtCioPlugEventGpioNo;
  UINT32          TbtFrcPwrGpioNo;
  UINT8           TbtFrcPwrGpioLevel;
  UINT32          TbtPcieRstGpioNo;
  UINT8           TbtPcieRstGpioLevel;
  EFI_SMM_SX_REGISTER_CONTEXT   *EntryDispatchContext;

  CableConnected    = 0;
  HoustRouteBus     = 3;
  SecSubBusAssigned = FALSE;
  Timeout = 600;
  RootportSelected      = 0;
  TbtCioPlugEventGpioNo = 0;
  TbtFrcPwrGpioNo       = 0;
  TbtFrcPwrGpioLevel    = 0;
  TbtPcieRstGpioNo      = 0;
  TbtPcieRstGpioLevel   = 0;
  Index = 0;

  EntryDispatchContext = (EFI_SMM_SX_REGISTER_CONTEXT*) DispatchContext;

//  CableConnected = GetTbtHostRouterStatus ();
  //SaveTbtHostRouterStatus (CableConnected & 0xF0);
  //
  // Get the Power State and Save
  //
  if (((mTbtNvsAreaPtr->DTbtControllerEn0 == 0) && (Index == 0)))  {

  RootportSelected      = mTbtNvsAreaPtr->RootportSelected0;
  TbtCioPlugEventGpioNo = mTbtNvsAreaPtr->TbtCioPlugEventGpioNo0;
  TbtFrcPwrGpioNo       = mTbtNvsAreaPtr->TbtFrcPwrGpioNo0;
  TbtFrcPwrGpioLevel    = mTbtNvsAreaPtr->TbtFrcPwrGpioLevel0;
  TbtPcieRstGpioNo      = mTbtNvsAreaPtr->TbtPcieRstGpioNo0;
  TbtPcieRstGpioLevel   = mTbtNvsAreaPtr->TbtPcieRstGpioLevel0;
  }

  Status = GetDTbtRpDevFun (gCurrentDiscreteTbtRootPortType, RootportSelected - 1, &RpDev, &RpFunc);
  ASSERT_EFI_ERROR (Status);
  CapHeaderOffset = PcieFindCapId (TbtSegment, 0x00, (UINT8)RpDev, (UINT8)RpFunc, 0x01);
  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSegment, 0x00, (UINT32)RpDev, (UINT32)RpFunc, 0);
  PowerState        = &*((volatile UINT32 *) (mPciExpressBaseAddress + DeviceBaseAddress + CapHeaderOffset + 4)); //PMCSR
  PowerStatePrev    = *PowerState;
  *PowerState &= 0xFFFFFFFC;

  HoustRouteBus = PciSegmentRead8 (DeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  //
  // Check the Subordinate bus .If it is Zero ,assign temporary bus to
  // find the device presence .
  //
  if (HoustRouteBus == 0) {
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, 0xF0);
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0xF0);
    HoustRouteBus     = 0xF0;
    SecSubBusAssigned = TRUE;
  }
  //
  // Clear Interrupt capability of TBT CIO Plug Event Pin to make sure no SCI is getting generated,
  // This GPIO will be reprogrammed while resuming as part of Platform GPIO Programming.
  //
  GpioSetPadInterruptConfig (TbtCioPlugEventGpioNo, GpioIntDis);
  //
  // Read the TBT Host router DeviceID
  //
  DeviceId = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (TbtSegment, HoustRouteBus, 0, 0, PCI_DEVICE_ID_OFFSET));

  //
  // Check For HostRouter Presence
  //
  if (IsTbtHostRouter (DeviceId)) {
    //    CableConnected = GetTbtHostRouterStatus ();
    if (!((CableConnected & (DTBT_SAVE_STATE_OFFSET << Index)) == (DTBT_SAVE_STATE_OFFSET << Index))) {
      CableConnected = CableConnected | (DTBT_SAVE_STATE_OFFSET << Index);
   //     SaveTbtHostRouterStatus (CableConnected);
    }
  }

  //
  // Check value of Tbt2Pcie reg, if Tbt is not present, bios needs to apply force power prior to sending mailbox command
  //
  GET_TBT2PCIE_REGISTER_ADDRESS(DTBT_CONTROLLER, TbtSegment, HoustRouteBus, 0x00, 0x00, Tbt2Pcie)
  RegisterValue = PciSegmentRead32 (Tbt2Pcie);
  if (0xFFFFFFFF == RegisterValue) {

    GpioWrite (TbtFrcPwrGpioNo,TbtFrcPwrGpioLevel);

    while (Timeout -- > 0) {
      RegisterValue = PciSegmentRead32 (Tbt2Pcie);
      if (0xFFFFFFFF != RegisterValue) {
        break;
      }
      Stall(1* (UINTN)1000);
    }
    //
    // Before entering Sx state BIOS should execute GO2SX/NO_WAKE mailbox command for AIC.
    // However BIOS shall not execute go2sx mailbox command on S5/reboot cycle.
    //

    if( (EntryDispatchContext->Type == SxS3) || (EntryDispatchContext->Type == SxS4))
    {
      if(!mTbtNvsAreaPtr->TbtWakeupSupport) {
        //Wake Disabled, GO2SX_NO_WAKE Command
        TbtSetPcie2TbtCommand (PCIE2TBT_GO2SX_NO_WAKE, DTBT_CONTROLLER, HoustRouteBus, 0, 0, TBT_5S_TIMEOUT);
      } else {
        //Wake Enabled, GO2SX Command
        TbtSetPcie2TbtCommand (PCIE2TBT_GO2SX, DTBT_CONTROLLER, HoustRouteBus, 0, 0, TBT_5S_TIMEOUT);
      }
    }
    if (mTbtNvsAreaPtr->TbtFrcPwrEn == 0) {
      GpioWrite (TbtFrcPwrGpioNo,!(TbtFrcPwrGpioLevel));
    }
  } else {
    //
    // Before entering Sx state BIOS should execute GO2SX/NO_WAKE mailbox command for AIC.
    // However BIOS shall not execute go2sx mailbox command on S5/reboot cycle.
    //
    if( (EntryDispatchContext->Type == SxS3) || (EntryDispatchContext->Type == SxS4))
    {
      if(!mTbtNvsAreaPtr->TbtWakeupSupport) {
        //Wake Disabled, GO2SX_NO_WAKE Command
        TbtSetPcie2TbtCommand (PCIE2TBT_GO2SX_NO_WAKE, DTBT_CONTROLLER, HoustRouteBus, 0, 0, TBT_5S_TIMEOUT);
      } else {
        //Wake Enabled, GO2SX Command
        TbtSetPcie2TbtCommand (PCIE2TBT_GO2SX, DTBT_CONTROLLER, HoustRouteBus, 0, 0, TBT_5S_TIMEOUT);
      }
    }
  }
  *PowerState = PowerStatePrev;
  //
  // Restore the bus number in case we assigned temporarily
  //
  if (SecSubBusAssigned) {
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, 0x00);
    PciSegmentWrite8 (DeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0x00);
  }
  if (gDTbtPcieRstSupport) {
    GpioWrite (TbtPcieRstGpioNo,TbtPcieRstGpioLevel);
  }
  return EFI_SUCCESS;
}

VOID
ThunderboltSwSmiCallback (
  IN UINT8 Type
  )
{
  UINT8 ThunderboltSmiFunction;

  DEBUG ((DEBUG_INFO, "ThunderboltSwSmiCallback Entry\n"));
  ThunderboltSmiFunction = mTbtNvsAreaPtr->ThunderboltSmiFunction;
  DEBUG ((DEBUG_INFO, "ThunderboltSwSmiCallback. ThunderboltSmiFunction=%d\n", ThunderboltSmiFunction));
  if (Type == DTBT_CONTROLLER) {
    gCurrentDiscreteTbtRootPort     = mTbtNvsAreaPtr->CurrentDiscreteTbtRootPort;
    gCurrentDiscreteTbtRootPortType = mTbtNvsAreaPtr->CurrentDiscreteTbtRootPortType;
  }

  switch (ThunderboltSmiFunction) {
  case 21:
    ThunderboltCallback (Type);
    break;

  case 22:
    TbtDisablePCIDevicesAndBridges (Type);
    break;

  case 23:
    ConfigureTbtAspm (Type, (UINT16) 0x02);
    break;

  case 24:
    ConfigureTbtAspm (Type, (UINT16) 0x01);
    break;

  default:
    break;
  }
  DEBUG ((DEBUG_INFO, "ThunderboltSwSmiCallback Exit.\n"));
}
STATIC
EFI_STATUS
EFIAPI
DiscreteThunderboltSwSmiCallback (
  IN EFI_HANDLE                     DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer OPTIONAL,
  IN  UINTN                         *CommBufferSize OPTIONAL
  )
{
  ThunderboltSwSmiCallback(DTBT_CONTROLLER);
  return EFI_SUCCESS;
}
EFI_STATUS
TbtRegisterHandlers (
  IN BOOLEAN Type
  )
{
  EFI_STATUS                    Status;
  UINTN                         SmiInputValue;
  EFI_SMM_HANDLER_ENTRY_POINT2   SxHandler;
  EFI_SMM_HANDLER_ENTRY_POINT2   SwHandler;
  EFI_SMM_SX_DISPATCH2_PROTOCOL *SxDispatchProtocol;
  EFI_SMM_SW_DISPATCH2_PROTOCOL *SwDispatch;
  EFI_SMM_SX_REGISTER_CONTEXT   EntryDispatchContext;
  EFI_SMM_SW_REGISTER_CONTEXT   SwContext;
  EFI_HANDLE                    SwDispatchHandle;
  EFI_HANDLE                    S3DispatchHandle;
  EFI_HANDLE                    S4DispatchHandle;
  EFI_HANDLE                    S5DispatchHandle;

  Status = EFI_UNSUPPORTED;

  if(Type == DTBT_CONTROLLER) {
    SxHandler = SxDTbtEntryCallback;
    SwHandler = DiscreteThunderboltSwSmiCallback;
    SmiInputValue = PcdGet8 (PcdSwSmiDTbtEnumerate);
    gDTbtPcieRstSupport = gTbtInfoHob->DTbtCommonConfig.PcieRstSupport;
    Status = EFI_SUCCESS;
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SwDispatchHandle        = NULL;
  S3DispatchHandle        = NULL;
  S4DispatchHandle        = NULL;
  S5DispatchHandle        = NULL;

   Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSxDispatch2ProtocolGuid,
                    NULL,
                    (VOID **) &SxDispatchProtocol
                    );
  ASSERT_EFI_ERROR (Status);
  //
  // Register S3 entry phase call back function
  //
  EntryDispatchContext.Type   = SxS3;
  EntryDispatchContext.Phase  = SxEntry;
  Status = SxDispatchProtocol->Register (
                                SxDispatchProtocol,
                                SxHandler,
                                &EntryDispatchContext,
                                &S3DispatchHandle
                                );
  ASSERT_EFI_ERROR (Status);
  //
  // Register S4 entry phase call back function
  //
  EntryDispatchContext.Type   = SxS4;
  EntryDispatchContext.Phase  = SxEntry;
  Status = SxDispatchProtocol->Register (
                                SxDispatchProtocol,
                                SxHandler,
                                &EntryDispatchContext,
                                &S4DispatchHandle
                                );
  ASSERT_EFI_ERROR (Status);
  //
  // Register S5 entry phase call back function
  //
  EntryDispatchContext.Type   = SxS5;
  EntryDispatchContext.Phase  = SxEntry;
  Status = SxDispatchProtocol->Register (
                                SxDispatchProtocol,
                                SxHandler,
                                &EntryDispatchContext,
                                &S5DispatchHandle
                                );
  ASSERT_EFI_ERROR (Status);
  //
  // Locate the SMM SW dispatch protocol
  //
  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmSwDispatch2ProtocolGuid,
                    NULL,
                    (VOID **) &SwDispatch
                    );

  ASSERT_EFI_ERROR (Status);
  //
  // Register SWSMI handler
  //
  SwContext.SwSmiInputValue = SmiInputValue;
  Status = SwDispatch->Register (
                        SwDispatch,
                        SwHandler,
                        &SwContext,
                        &SwDispatchHandle
                        );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
EFI_STATUS
InSmmFunction (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                    Status;

  Status = EFI_SUCCESS;

  Status = TbtRegisterHandlers(DTBT_CONTROLLER);
  return Status;
}

EFI_STATUS
EFIAPI
TbtSmmEntryPoint (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  TBT_NVS_AREA_PROTOCOL         *TbtNvsAreaProtocol;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "TbtSmmEntryPoint\n"));

  mPciExpressBaseAddress = PcdGet64 (PcdPciExpressBaseAddress);
  //
  // Locate Tbt shared data area
  //
  Status = gBS->LocateProtocol (&gTbtNvsAreaProtocolGuid, NULL, (VOID **) &TbtNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  mTbtNvsAreaPtr = TbtNvsAreaProtocol->Area;

  //
  // Get TBT INFO HOB
  //
  gTbtInfoHob = (TBT_INFO_HOB *) GetFirstGuidHob (&gTbtInfoHobGuid);
  if (gTbtInfoHob == NULL) {
    return EFI_NOT_FOUND;
  }

  return InSmmFunction (ImageHandle, SystemTable);
}

VOID
EndOfThunderboltCallback (
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  if(mTbtNvsAreaPtr->TbtL1SubStates != 0) {
    ThunderboltEnableL1Sub (mTbtNvsAreaPtr->TbtL1SubStates, RpSegment, RpBus, RpDevice, RpFunction);
  }
  ConfigureTbtPm(RpSegment, RpBus, RpDevice, RpFunction, 1);
  if (!mTbtNvsAreaPtr->TbtAspm) { //Aspm disable case
    ThunderboltDisableAspmWithoutLtr (RpSegment, RpBus, RpDevice, RpFunction);
  } else { //Aspm enable case
    ThunderboltEnableAspmWithoutLtr ((UINT16)mTbtNvsAreaPtr->TbtAspm, RpSegment, RpBus, RpDevice, RpFunction);
  }

  if (mTbtNvsAreaPtr->TbtLtr) {
    ThunderboltGetLatencyLtr ();
    ThunderboltSetLatencyLtr (RpSegment, RpBus, RpDevice, RpFunction);
  }
  ConfigureLtr (RpSegment, RpBus, RpDevice, RpFunction);
  ConfigureTbtPm(RpSegment, RpBus, RpDevice, RpFunction, 2);
} // EndOfThunderboltCallback

VOID
ConfigureTbtAspm (
  IN UINT8        Type,
  IN UINT16       Aspm
  )
{
  UINTN                         RpSegment = 0;
  UINTN                         RpBus = 0;
  UINTN                         RpDevice;
  UINTN                         RpFunction;

  if(Type == DTBT_CONTROLLER) {
    if (gCurrentDiscreteTbtRootPort == 0) {
      return;
    }
    GetDTbtRpDevFun(DTBT_CONTROLLER, gCurrentDiscreteTbtRootPort - 1, &RpDevice, &RpFunction);

    ConfigureTbtPm (RpSegment, RpBus, RpDevice, RpFunction, 1);
    if (!mTbtNvsAreaPtr->TbtAspm) { //Aspm disable case
      ThunderboltDisableAspmWithoutLtr (RpSegment, RpBus, RpDevice, RpFunction);
    } else { //Aspm enable case
      ThunderboltEnableAspmWithoutLtr ((UINT16) Aspm, RpSegment, RpBus, RpDevice, RpFunction);
    }

  if (mTbtNvsAreaPtr->TbtLtr) {
      ThunderboltGetLatencyLtr ();
      ThunderboltSetLatencyLtr (RpSegment, RpBus, RpDevice, RpFunction);
    }
    ConfigureLtr (RpSegment, RpBus, RpDevice, RpFunction);
  } // EndOfThunderboltCallback
}


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmiHandler.c
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmiHandler.c
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/TbtInit/Smm/TbtSmiHandler.c

/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//#include "TbtSmiHandler.h"
//#include <Library/IoLib.h>
//#include <Library/BaseLib.h>
//#include <Library/DebugLib.h>
//#include <Protocol/SmmVariable.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/SmmServicesTableLib.h>
//#include <Library/PciSegmentLib.h>
#define MEM_PER_SLOT  (DEF_RES_MEM_PER_DEV << 4)
#define PMEM_PER_SLOT (DEF_RES_PMEM_PER_DEV << 4)
#define IO_PER_SLOT   (DEF_RES_IO_PER_DEV << 2)

GLOBAL_REMOVE_IF_UNREFERENCED UINTN                 gDeviceBaseAddress;
//
//US(X:0:0), DS(X+1:3:0),DS(X+1:4:0),DS(X+1:5:0),DS(X+1:6:0)
//
GLOBAL_REMOVE_IF_UNREFERENCED BRDG_CONFIG           HrConfigs[MAX_CFG_PORTS];

extern UINT8                      gCurrentDiscreteTbtRootPort;
extern UINT8                      gCurrentDiscreteTbtRootPortType;

BOOLEAN isLegacyDevice          = FALSE;
STATIC UINT8 TbtSmiHandler_TbtSegment         = 0;

STATIC
VOID
PortInfoInit (
  IN  OUT PORT_INFO *PortInfo
  )
{
  PortInfo->BusNumLimit = 4;
}

STATIC
VOID
UnsetVesc (
  IN       UINT8     Bus,
  IN       UINT8     Dev,
  IN       UINT8     Fun
  )
{
  UINT8 Dbus;
  UINT32 Data32;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, Bus, Dev, Fun, 0);

  //
  // Check for abcence of DS bridge
  //
  if(0xFFFF == PciSegmentRead16(gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
    return;
  }

  //
  // Unset vesc_reg2[23] bit (to have an option to access below DS)
  //
  Data32 = PciSegmentRead32 (gDeviceBaseAddress + PCI_TBT_VESC_REG2);
  Data32 &= 0xFF7FFFFF;
  PciSegmentWrite32(gDeviceBaseAddress + PCI_TBT_VESC_REG2, Data32);
  //
  // Go to Device behind DS
  //
  Dbus = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  DEBUG((DEBUG_INFO, "Dbus = %d\n",Dbus));
  //
  // Check if there is something behind this Downstream Port (Up or Ep)
  // If there nothing  behind Downstream Port Set vesc_reg2[23] bit -> this will flush all future MemWr
  //
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, Dbus, 0x00, 0x00, 0);
  if(0xFFFF == PciSegmentRead16(gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET))
  {
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, Bus, Dev, Fun, 0);
  Data32 = PciSegmentRead32 (gDeviceBaseAddress + PCI_TBT_VESC_REG2);
  Data32 |= 0x00800000;
  PciSegmentWrite32 (gDeviceBaseAddress + PCI_TBT_VESC_REG2, Data32);
  }
}// Unset_VESC_REG2

STATIC
UINT16
MemPerSlot (
  IN    UINT16 CurrentUsage
  )
{
  if (CurrentUsage == 0) {
    return 0;
  }

  if (CurrentUsage <= 16) {
    return 16;
  }

  if (CurrentUsage <= 64) {
    return 64;
  }

  if (CurrentUsage <= 128) {
    return 128;
  }

  if (CurrentUsage <= 256) {
    return 256;
  }

  if (CurrentUsage <= 512) {
    return 512;
  }

  if (CurrentUsage <= 1024) {
    return 1024;
  }

  return CurrentUsage;
} // MemPerSlot

STATIC
UINT64
PMemPerSlot (
  IN    UINT64 CurrentUsage
  )
{
  if (CurrentUsage == 0) {
    return 0;
  }

  if (CurrentUsage <= 1024ULL) {
    return 1024ULL;
  }

  if (CurrentUsage <= 4096ULL) {
    return 4096ULL;
  }

  return CurrentUsage;
} // PMemPerSlot

STATIC
VOID
SetPhyPortResources (
  IN       UINT8      Bus,
  IN       UINT8      Dev,
  IN       UINT8      SubBus,
  IN       INT8       Depth,
  IN       PORT_INFO  *CurrentPi,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8   Cmd;
  UINT16  DeltaMem;
  UINT64  DeltaPMem;

  Cmd               = CMD_BUS_MASTER;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, Bus, Dev, 0x00, 0);

  PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SubBus);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);

  DeltaMem = PortInfo->MemBase - CurrentPi->MemBase;
  if (isLegacyDevice) {
    if (Depth >= 0 && (DeltaMem < MEM_PER_SLOT)) {
      PortInfo->MemBase += MEM_PER_SLOT - DeltaMem;
    }
  } else {
    if (DeltaMem < MemPerSlot (DeltaMem)) {
      PortInfo->MemBase += MemPerSlot (DeltaMem) - DeltaMem;
    }
  }

  if (PortInfo->MemBase > CurrentPi->MemBase && (PortInfo->MemBase - 0x10) <= PortInfo->MemLimit) {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), CurrentPi->MemBase);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit), PortInfo->MemBase - 0x10);
    Cmd |= CMD_BM_MEM;
  } else {
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), DISBL_MEM32_REG20);
    PortInfo->MemBase = CurrentPi->MemBase;
  }

  DeltaPMem = PortInfo->PMemBase64 - CurrentPi->PMemBase64;
  if (isLegacyDevice) {
    if ((Depth >= 0) && ((UINTN)DeltaPMem < (UINTN)PMEM_PER_SLOT)) {
      PortInfo->PMemBase64 += PMEM_PER_SLOT - DeltaPMem;
    }
  } else {
    if (DeltaPMem < PMemPerSlot (DeltaPMem)) {
      PortInfo->PMemBase64 += PMemPerSlot (DeltaPMem) - DeltaPMem;
    }
  }

  if (PortInfo->PMemBase64 > CurrentPi->PMemBase64 && (PortInfo->PMemBase64 - 0x10) <= PortInfo->PMemLimit64) {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), (UINT16) (CurrentPi->PMemBase64 & 0xFFFF));
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit), (UINT16) ((PortInfo->PMemBase64 - 0x10) & 0xFFFF));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), (UINT32) (CurrentPi->PMemBase64 >> 16));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), (UINT32) ((PortInfo->PMemBase64 - 0x10) >> 16));
    Cmd |= CMD_BM_MEM;
  } else {
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), DISBL_PMEM_REG24);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), 0);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), 0);
    PortInfo->PMemBase64 = CurrentPi->PMemBase64;
  }

  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
} // SetPhyPortResources

STATIC
UINT32
SaveSetGetRestoreBar (
  IN  UINTN  Bar
  )
{
  UINT32  BarReq;
  UINT32  OrigBar;

  OrigBar = PciSegmentRead32(Bar);     // Save BAR
  PciSegmentWrite32(Bar, 0xFFFFFFFF);  // Set BAR
  BarReq = PciSegmentRead32(Bar);      // Get BAR
  PciSegmentWrite32(Bar, OrigBar);     // Restore BAR

  return BarReq;
} // SaveSetGetRestoreBar

STATIC
VOID
SetIoBar (
  IN            UINTN    BAR,
  IN            UINT32   BarReq,
  IN  OUT       UINT8    *Cmd,
  IN  OUT       IO_REGS  *IoReg
  )
{
  UINT16  Alignment;
  UINT16  Size;
  UINT16  NewBase;

  Alignment = ~(BarReq & 0xFFFC);
  Size      = Alignment + 1;

  if (IoReg->Base > IoReg->Limit || !Size) {
    return ;

  }

  NewBase = BAR_ALIGN (IoReg->Base, Alignment);
  if (NewBase > IoReg->Limit || NewBase + Size - 1 > IoReg->Limit) {
    return ;

  }
  PciSegmentWrite16(BAR, NewBase);
  IoReg->Base = NewBase + Size; // Advance to new position
  *Cmd      |= CMD_BM_IO; // Set Io Space Enable
} // SetIoBar

STATIC
VOID
SetMemBar (
  IN            UINTN     BAR,
  IN            UINT32    BarReq,
  IN  OUT       UINT8     *Cmd,
  IN  OUT       MEM_REGS  *MemReg
  )
{
  UINT32  Alignment;
  UINT32  Size;
  UINT32  NewBase;

  Alignment = ~(BarReq & 0xFFFFFFF0);
  Size      = Alignment + 1;

  if (MemReg->Base > MemReg->Limit || !Size) {
    return ;

  }

  NewBase = BAR_ALIGN (MemReg->Base, Alignment);
  if (NewBase > MemReg->Limit || NewBase + Size - 1 > MemReg->Limit) {
    return ;

  }

  PciSegmentWrite32(BAR, NewBase);
  MemReg->Base = NewBase + Size; // Advance to new position
  *Cmd       |= CMD_BM_MEM; // Set Memory Space Enable
} // SetMemBar

STATIC
VOID
SetPMem64Bar (
  IN              UINTN      BAR,
  IN              BOOLEAN    IsMaxBar,
  IN              UINT32     BarReq,
  IN    OUT       UINT8      *Cmd,
  IN    OUT       PMEM_REGS  *MemReg
  )
{
  UINT32  Alignment;
  UINT32  Size;
  UINT64  NewBase;

  Alignment = ~(BarReq & 0xFFFFFFF0);
  Size      = Alignment + 1;

  if (MemReg->Base64 > MemReg->Limit64 || !Size) {
    return ;
  }

  NewBase = BAR_ALIGN (MemReg->Base64, Alignment);
  if (NewBase > MemReg->Limit64 || NewBase + Size - 1 > MemReg->Limit64) {
    return ;
  }
  PciSegmentWrite32(BAR, (UINT32)(NewBase & 0xFFFFFFFF));
  if (!IsMaxBar) {
    BAR++;
    PciSegmentWrite32(BAR, (UINT32)(NewBase >> 32));
  }
  MemReg->Base64 = NewBase + Size; // Advance to new position
  *Cmd         |= CMD_BM_MEM; // Set Memory Space Enable
} // SetPMem64Bar

STATIC
VOID
SetDevResources (
  IN       UINT8      Bus,
  IN       UINT8      Dev,
  IN       UINT8      MaxFun,  // PCI_MAX_FUNC for devices, 1 for bridge
  IN       UINT8      MaxBar,     // PCI_BAR5 for devices, PCI_BAR1 for bridge
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8     Fun;
  UINT8     Reg;
  UINT32    BarReq;
  IO_REGS   Io;
  MEM_REGS  Mem;
  PMEM_REGS PMem;
  UINT8     Cmd;

  Io.Base       = PortInfo->IoBase << 8;
  Io.Limit      = (PortInfo->IoLimit << 8) | 0xFF;
  Mem.Base      = PortInfo->MemBase << 16;
  Mem.Limit     = (PortInfo->MemLimit << 16) | 0xFFFF;
  PMem.Base64   = PortInfo->PMemBase64 << 16;
  PMem.Limit64  = (PortInfo->PMemLimit64 << 16) | 0xFFFF;

  for (Fun = 0; Fun < MaxFun; ++Fun) {
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, Bus, Dev, Fun, 0);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, CMD_BUS_MASTER);
    Cmd = PciSegmentRead8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET);
    if (0xFFFF == PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
      continue;

    }

    for (Reg = PCI_BASE_ADDRESSREG_OFFSET; Reg <= MaxBar; Reg += 4) {
      BarReq = SaveSetGetRestoreBar(gDeviceBaseAddress + Reg); // Perform BAR sizing

      if (BarReq & BIT0) {
        //
        // I/O BAR
        //
        SetIoBar (
         (gDeviceBaseAddress + Reg),
          BarReq,
          &Cmd,
          &Io
          );
        continue;
      }

      if (BarReq & BIT3) {
        //
        // P-Memory BAR
        //
        SetPMem64Bar ((gDeviceBaseAddress + Reg), MaxBar == Reg, BarReq, &Cmd, &PMem);
      } else {
        SetMemBar ((gDeviceBaseAddress + Reg), BarReq, &Cmd, &Mem);
      }

      if (BIT2 == (BarReq & (BIT2 | BIT1))) {
        //
        // Base address is 64 bits wide
        //
        Reg += 4;
        if (!(BarReq & BIT3)) {
          //
          // 64-bit memory bar
          //
          PciSegmentWrite32 (gDeviceBaseAddress + Reg, 0);
        }
      }
    }

    if (Cmd & BIT1) {
      //
      // If device uses I/O and MEM mapping use only MEM mepping
      //
      Cmd &= ~BIT0;
    }

    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
  }
  //
  // Update PortInfo if any changes
  //
  if (Io.Base > ((UINT32) PortInfo->IoBase << 8)) {
    PortInfo->IoBase = (UINT8) (BAR_ALIGN (Io.Base, 0xFFF) >> 8);
  }

  if (Mem.Base > ((UINT32) PortInfo->MemBase << 16)) {
    PortInfo->MemBase = (UINT16) (BAR_ALIGN (Mem.Base, 0xFFFFF) >> 16);
  }

  if (PMem.Base64 > (PortInfo->PMemBase64 << 16)) {
    PortInfo->PMemBase64 = (BAR_ALIGN (PMem.Base64, 0xFFFFF) >> 16);
  }
} // SetDevResources

STATIC
VOID
InitARHRConfigs(
  IN HR_CONFIG *Hr_Config,
  IN UINT8 BusNumLimit,
  IN OUT BRDG_RES_CONFIG* HrResConf
)
{
  UINT8 i,j;

  //
  // DS port for USB device
  //
  HrConfigs[AR_DS_PORT2].DevId.Bus = HrConfigs[HR_US_PORT].DevId.Bus + 1;
  HrConfigs[AR_DS_PORT2].DevId.Dev = 2;
  HrConfigs[AR_DS_PORT2].DevId.Fun = 0;
  HrConfigs[AR_DS_PORT2].PBus = HrConfigs[AR_DS_PORT2].DevId.Bus;
  HrConfigs[AR_DS_PORT2].SBus = HrConfigs[AR_DS_PORT2].PBus + 1;
  HrConfigs[AR_DS_PORT2].SubBus = HrConfigs[AR_DS_PORT2].PBus + 1;
  //
  // CIO port
  //
  HrConfigs[AR_DS_PORT1].DevId.Bus = HrConfigs[HR_US_PORT].DevId.Bus + 1;
  HrConfigs[AR_DS_PORT1].DevId.Dev = 1;
  HrConfigs[AR_DS_PORT1].DevId.Fun = 0;
  HrConfigs[AR_DS_PORT1].PBus = HrConfigs[AR_DS_PORT1].DevId.Bus;
  HrConfigs[AR_DS_PORT1].SBus = HrConfigs[HR_DS_PORT0].SubBus + 1;
  HrConfigs[AR_DS_PORT1].SubBus = BusNumLimit;

  switch(Hr_Config->DeviceId)
  {
    //
    // HR with 1 DS and 1 USB
    //
    case AR_HR_2C:
    case AR_HR_LP:
    case AR_HR_C0_2C:
    case TR_HR_2C:
      Hr_Config->MinDSNumber = HrConfigs[AR_DS_PORT1].DevId.Dev;
      Hr_Config->MaxDSNumber = HrConfigs[AR_DS_PORT2].DevId.Dev;
      Hr_Config->BridgeLoops = 4;
      break;
    //
    // HR with 2 DS and 1 USB
    //
    case AR_HR_4C:
    case TR_HR_4C:
    case AR_HR_C0_4C:
      Hr_Config->MinDSNumber = 1;
      Hr_Config->MaxDSNumber = 4;
      Hr_Config->BridgeLoops = 6;
      for(j = 2, i = Hr_Config->MinDSNumber; j < count(HrConfigs) && i <= Hr_Config->MaxDSNumber; ++j, ++i)
      {
        HrConfigs[j].DevId.Bus = HrConfigs[HR_US_PORT].DevId.Bus + 1;
        HrConfigs[j].DevId.Dev = i;
        HrConfigs[j].DevId.Fun = 0;
        HrConfigs[j].PBus = HrConfigs[j].DevId.Bus;
        HrConfigs[j].Res.Cls = DEF_CACHE_LINE_SIZE;
      }
    break;
  }
}//InitARHRConfigs


STATIC
VOID
InitCommonHRConfigs (
  IN       HR_CONFIG        *Hr_Config,
  IN       UINT8            BusNumLimit,
  IN  OUT  BRDG_RES_CONFIG  *HrResConf
  )
{
  UINT8 i;

  UINT8 j;
  for(i = 0; i < count(HrConfigs); ++i) {
    HrConfigs[i].IsDSBridge = TRUE;
  }
  //
  // US(HRBus:0:0)
  //
  HrConfigs[HR_US_PORT].DevId.Bus   = Hr_Config->HRBus;
  HrConfigs[HR_US_PORT].DevId.Dev   = 0;
  HrConfigs[HR_US_PORT].DevId.Fun   = 0;
  HrConfigs[HR_US_PORT].Res         = *HrResConf;
  HrConfigs[HR_US_PORT].Res.IoBase  = 0xF1;
  HrConfigs[HR_US_PORT].Res.IoLimit = 0x01;
  HrConfigs[HR_US_PORT].PBus        = HrConfigs[HR_US_PORT].DevId.Bus;
  HrConfigs[HR_US_PORT].SBus        = HrConfigs[HR_US_PORT].PBus + 1;
  HrConfigs[HR_US_PORT].SubBus      = BusNumLimit;
  HrConfigs[HR_US_PORT].IsDSBridge  = FALSE;

  //
  // HIA resides here
  //
  HrConfigs[HR_DS_PORT0].DevId.Bus    = HrConfigs[HR_US_PORT].DevId.Bus + 1;
  HrConfigs[HR_DS_PORT0].DevId.Dev    = 0;
  HrConfigs[HR_DS_PORT0].DevId.Fun    = 0;
  HrConfigs[HR_DS_PORT0].Res          = NOT_IN_USE_BRIDGE;
  HrConfigs[HR_DS_PORT0].Res.MemBase  = HrResConf->MemLimit;
  HrConfigs[HR_DS_PORT0].Res.MemLimit = HrResConf->MemLimit;
  HrResConf->MemLimit                -= 0x10; //This 1 MB chunk will be used by HIA
  HrConfigs[HR_DS_PORT0].Res.Cmd      = CMD_BM_MEM;
  HrConfigs[HR_DS_PORT0].Res.Cls      = DEF_CACHE_LINE_SIZE;
  HrConfigs[HR_DS_PORT0].PBus         = HrConfigs[HR_DS_PORT0].DevId.Bus;
  HrConfigs[HR_DS_PORT0].SBus         = HrConfigs[HR_DS_PORT0].PBus + 1;
  HrConfigs[HR_DS_PORT0].SubBus       = HrConfigs[HR_DS_PORT0].PBus + 1;

  switch (Hr_Config->DeviceId) {
  //
  // Alpine Ridge
  //
  case AR_HR_2C:
  case AR_HR_C0_2C:
  case AR_HR_LP:
  case AR_HR_4C:
  case AR_HR_C0_4C:
  //
  // Titan Ridge
  //
  case TR_HR_2C:
  case TR_HR_4C:
    InitARHRConfigs(Hr_Config, BusNumLimit, HrResConf);
    break;

  default:
    //
    // DS(HRBus+2:3-6:0)
    //
    Hr_Config->MinDSNumber  = 3;
    Hr_Config->MaxDSNumber  = 6;
    Hr_Config->BridgeLoops  = count (HrConfigs);

    for (j = 2, i = Hr_Config->MinDSNumber; j < count (HrConfigs) && i <= Hr_Config->MaxDSNumber; ++j, ++i) {
      HrConfigs[j].DevId.Bus  = HrConfigs[HR_US_PORT].DevId.Bus + 1;
      HrConfigs[j].DevId.Dev  = i;
      HrConfigs[j].DevId.Fun  = 0;
      HrConfigs[j].PBus       = HrConfigs[j].DevId.Bus;
      HrConfigs[j].Res.Cls    = DEF_CACHE_LINE_SIZE;
    }
  }
} // InitCommonHRConfigs

STATIC
VOID
InitHRDSPort_Disable (
  IN       UINT8        id,
  IN  OUT  BRDG_CONFIG  *BrdgConf
  )
{
  HrConfigs[id].Res     = NOT_IN_USE_BRIDGE;
  HrConfigs[id].SBus    = BrdgConf->SBus;
  HrConfigs[id].SubBus  = BrdgConf->SBus;

  BrdgConf->SBus++;
} // InitHRDSPort_Disable

//AR only

STATIC
VOID
InitARDSPort_1Port(
  IN  OUT  BRDG_CONFIG* BrdgConf
)
{
  UINT16 MemBase    = BrdgConf->Res.MemBase & 0xFFF0;
  UINT64 PMemBase64 = BrdgConf->Res.PMemBase64 & ~0xFULL;
  UINT8  BusRange = BrdgConf->SubBus - BrdgConf->PBus - 2;

  HrConfigs[AR_DS_PORT1].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT1].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT1].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT1].Res.MemBase = MemBase;
  HrConfigs[AR_DS_PORT1].Res.MemLimit = BrdgConf->Res.MemLimit - 1;
  HrConfigs[AR_DS_PORT1].Res.PMemBase64 = PMemBase64;
  HrConfigs[AR_DS_PORT1].Res.PMemLimit64 = BrdgConf->Res.PMemLimit64;
  HrConfigs[AR_DS_PORT1].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT1].SubBus = BrdgConf->SBus + BusRange;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT1].SubBus + 1;

  HrConfigs[AR_DS_PORT2].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT2].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT2].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT2].Res.MemBase = BrdgConf->Res.MemLimit;
  HrConfigs[AR_DS_PORT2].Res.MemLimit = BrdgConf->Res.MemLimit;
  HrConfigs[AR_DS_PORT2].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT2].SubBus = BrdgConf->SBus;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT2].SubBus + 1;
}//InitARDSPort_1Port

STATIC
VOID
InitARDSPort_2Port(
  IN OUT BRDG_CONFIG* BrdgConf
)
{
  UINT16 MemBase    = BrdgConf->Res.MemBase & 0xFFF0;
  UINT64 PMemBase64 = BrdgConf->Res.PMemBase64 & ~0xFULL;
  UINT8  BusRange = BrdgConf->SubBus - BrdgConf->PBus - 3;

  // Busses are split between ports 1 and 4
  BusRange /= 2;

  HrConfigs[AR_DS_PORT1].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT1].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT1].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT1].Res.MemBase = MemBase;
  HrConfigs[AR_DS_PORT1].Res.MemLimit = MemBase + 0x17F0 - 1;
  HrConfigs[AR_DS_PORT1].Res.PMemBase64 = PMemBase64;
  HrConfigs[AR_DS_PORT1].Res.PMemLimit64 = PMemBase64 + 0x2000 - 1;
  HrConfigs[AR_DS_PORT1].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT1].SubBus = BrdgConf->SBus + BusRange;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT1].SubBus + 1;

  HrConfigs[AR_DS_PORT2].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT2].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT2].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT2].Res.MemBase = MemBase + 0x17F0;
  HrConfigs[AR_DS_PORT2].Res.MemLimit = MemBase + 0x1800 - 1;
  HrConfigs[AR_DS_PORT2].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT2].SubBus = BrdgConf->SBus;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT2].SubBus + 1;


  HrConfigs[AR_DS_PORT4].Res = NOT_IN_USE_BRIDGE;
  HrConfigs[AR_DS_PORT4].Res.Cls = DEF_CACHE_LINE_SIZE;
  HrConfigs[AR_DS_PORT4].Res.Cmd = CMD_BM_MEM;
  HrConfigs[AR_DS_PORT4].Res.MemBase = MemBase + 0x1800;
  HrConfigs[AR_DS_PORT4].Res.MemLimit = BrdgConf->Res.MemLimit;
  HrConfigs[AR_DS_PORT4].Res.PMemBase64 = PMemBase64 + 0x2000;
  HrConfigs[AR_DS_PORT4].Res.PMemLimit64 = BrdgConf->Res.PMemLimit64;
  HrConfigs[AR_DS_PORT4].SBus = BrdgConf->SBus;
  HrConfigs[AR_DS_PORT4].SubBus = BrdgConf->SubBus;

  BrdgConf->SBus = HrConfigs[AR_DS_PORT4].SubBus + 1;
}//InitARDSPort_2Port


STATIC
BOOLEAN
CheckLimits (
  IN    BOOLEAN          Is2PortDev,
  IN    BRDG_RES_CONFIG  *HrResConf,
  IN    UINT8            BusRange
  )
{
  UINT16  MemBase;
  UINT16  MemLimit;
  UINT64  PMemBase64;
  UINT64  PMemLimit64;

  MemBase     = HrResConf->MemBase & 0xFFF0;
  MemLimit    = HrResConf->MemLimit & 0xFFF0;
  PMemBase64  = HrResConf->PMemBase64 & 0xFFF0;
  PMemLimit64 = HrResConf->PMemLimit64 & 0xFFF0;
  //
  // Check memoty alignment
  //
  if (MemBase & 0x3FF) {
    DEBUG((DEBUG_INFO, "M alig\n"));
    return FALSE;
  }

  if (PMemBase64 & 0xFFF) {
    DEBUG((DEBUG_INFO, "PM alig\n"));
    return FALSE;
  }

  if (Is2PortDev) {
    //
    // Check mem size
    //
    if (MemLimit + 0x10 - MemBase < 0x2E00) {
      DEBUG((DEBUG_INFO, "M size\n"));
      return FALSE;
    }
    //
    // Check P-mem size
    //
    if (PMemLimit64 + 0x10 - PMemBase64 < 0x4A00) {
      DEBUG((DEBUG_INFO, "PM size\n"));
      return FALSE;
    }
    //
    // Check bus range
    //
    if (BusRange < 106) {
      DEBUG((DEBUG_INFO, "Bus range\n"));
      return FALSE;
    }
  } else {
    //
    // Check mem size
    //
    if (MemLimit + 0x10 - MemBase < 0x1600) {
      DEBUG((DEBUG_INFO, "M size\n"));
      return FALSE;
    }
    //
    // Check P-mem size
    //
    if (PMemLimit64 + 0x10 - PMemBase64 < 0x2200) {
      DEBUG((DEBUG_INFO, "PM size\n"));
      return FALSE;
    }
    //
    // Check bus range
    //
    if (BusRange < 56) {
      DEBUG((DEBUG_INFO, "Bus range\n"));
      return FALSE;
    }
  }

  return TRUE;
} // CheckLimits

STATIC
BOOLEAN
InitHRResConfigs (
  IN  OUT HR_CONFIG      *Hr_Config,
  IN    UINT8            BusNumLimit,
  IN  OUT BRDG_RES_CONFIG*HrResConf
  )
{

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"

  BRDG_CONFIG  BrdgConf = { { 0 } };

#pragma clang diagnostic pop

  InitCommonHRConfigs (Hr_Config, BusNumLimit, HrResConf);
  BrdgConf.PBus   = Hr_Config->HRBus + 2;// Take into account busses
  BrdgConf.SBus   = Hr_Config->HRBus + 3;// for US and DS of HIA
  BrdgConf.SubBus = BusNumLimit;
  BrdgConf.Res    = *HrResConf;
  while (TRUE) {
    switch (Hr_Config->DeviceId) {
    case AR_HR_4C:
    case TR_HR_4C:
    case AR_HR_C0_4C:
      //
      // 2 Port host
      //
      if (CheckLimits (TRUE, HrResConf, BusNumLimit - Hr_Config->HRBus)) {


          InitARDSPort_2Port(&BrdgConf);
          DEBUG((DEBUG_INFO, "AR2\n"));

        return TRUE;
      } else {
       return FALSE;
      }
    // AR only
  case AR_HR_2C: // 1 port host
  case AR_HR_C0_2C:
  case AR_HR_LP:
  case TR_HR_2C:
    DEBUG((DEBUG_INFO, "AR1\n"));
    InitARDSPort_1Port(&BrdgConf);
    return TRUE;

    default:
      InitHRDSPort_Disable (HR_DS_PORT3, &BrdgConf);
      InitHRDSPort_Disable (HR_DS_PORT4, &BrdgConf);
      InitHRDSPort_Disable (HR_DS_PORT5, &BrdgConf);
      InitHRDSPort_Disable (HR_DS_PORT6, &BrdgConf);
      return FALSE;
    }
  }
} // InitHRResConfigs

STATIC
BOOLEAN
InitializeHostRouter (
  OUT  HR_CONFIG  *Hr_Config,
  IN   UINTN      RpSegment,
  IN   UINTN      RpBus,
  IN   UINTN      RpDevice,
  IN   UINTN      RpFunction
  )
{
  UINT8           BusNumLimit;
  BRDG_RES_CONFIG HrResConf = { 0 };
  UINT8           i;
  BOOLEAN         Ret;

  Ret = TRUE;

  gDeviceBaseAddress   = PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, 0);
  Hr_Config->HRBus    = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  gDeviceBaseAddress   = PCI_SEGMENT_LIB_ADDRESS (RpSegment, Hr_Config->HRBus, 0x00, 0x00, 0);
  Hr_Config->DeviceId = PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
  if (!(IsTbtHostRouter (Hr_Config->DeviceId))) {
    return FALSE;
  }
  TbtSmiHandler_TbtSegment = (UINT8)RpSegment;

  HrResConf.Cmd          = CMD_BM_MEM;
  HrResConf.Cls          = DEF_CACHE_LINE_SIZE;
  gDeviceBaseAddress      = PCI_SEGMENT_LIB_ADDRESS (RpSegment, RpBus, RpDevice, RpFunction, 0);
  HrResConf.IoBase       = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase));
  HrResConf.IoLimit      = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit));
  HrResConf.MemBase      = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase));
  HrResConf.MemLimit     = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit));
  HrResConf.PMemBase64   = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase));
  HrResConf.PMemLimit64  = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit));
  HrResConf.PMemBase64  |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32))) << 16;
  HrResConf.PMemLimit64 |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32))) << 16;
  BusNumLimit = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);

  Ret         = InitHRResConfigs (Hr_Config, BusNumLimit, &HrResConf);

  for (i = 0; i < Hr_Config->BridgeLoops; ++i) {
    UINT8 Bus;
    UINT8 Dev;
    UINT8 Fun;
    Bus               = HrConfigs[i].DevId.Bus;
    Dev               = HrConfigs[i].DevId.Dev;
    Fun               = HrConfigs[i].DevId.Fun;
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, Dev, Fun, 0);

    PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, HrConfigs[i].Res.Cls);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, HrConfigs[i].PBus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, HrConfigs[i].SBus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, HrConfigs[i].SubBus);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), HrConfigs[i].Res.MemBase);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit), HrConfigs[i].Res.MemLimit);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), (UINT16) (HrConfigs[i].Res.PMemBase64 & 0xFFFF));
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit), (UINT16) (HrConfigs[i].Res.PMemLimit64 & 0xFFFF));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), (UINT32) (HrConfigs[i].Res.PMemBase64 >> 16));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), (UINT32) (HrConfigs[i].Res.PMemLimit64 >> 16));
    PciSegmentWrite8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase), HrConfigs[i].Res.IoBase);
    PciSegmentWrite8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit), HrConfigs[i].Res.IoLimit);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBaseUpper16), 0x00000000);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, HrConfigs[i].Res.Cmd);
  }
  if (Hr_Config->DeviceId == AR_HR_2C || Hr_Config->DeviceId == AR_HR_4C || Hr_Config->DeviceId == AR_HR_LP) {
    for (i = 0; i < Hr_Config->BridgeLoops; ++i) {
      if(HrConfigs[i].IsDSBridge) {
        UnsetVesc(HrConfigs[i].DevId.Bus, HrConfigs[i].DevId.Dev, HrConfigs[i].DevId.Fun);
      }
    }
  }

  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,(Hr_Config->HRBus + 2), 0x00, 0x00, 0);
  PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX0 * 4), HrConfigs[HR_DS_PORT0].Res.MemLimit << 16);
  PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX1 * 4), (HrConfigs[HR_DS_PORT0].Res.MemLimit + 0x4) << 16);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, CMD_BM_MEM);
  return Ret;
} // InitializeHostRouter
STATIC
UINT8
ConfigureSlot (
  IN       UINT8      Bus,
  IN       UINT8      MAX_DEVICE,
  IN       INT8       Depth,
  IN       BOOLEAN    ArPcie,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8      Device;
  UINT8      SBus;
  UINT8      UsedBusNumbers;
  UINT8      RetBusNum;
  PORT_INFO  CurrentSlot;

  RetBusNum = 0;

  for (Device = 0; Device < MAX_DEVICE; Device++) {
    //
    // Continue if device is absent
    //
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, Bus, Device, 0x00, 0);
    if (0xFFFF == PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
      continue;

    }

    if (P2P_BRIDGE != PciSegmentRead16 (gDeviceBaseAddress + (PCI_CLASSCODE_OFFSET + 1))) {
      SetDevResources (
        Bus,
        Device,
        PCI_MAX_FUNC,
        PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX5 * 4),
        PortInfo
        );
      continue;
    }
    //
    // Else Bridge
    //
    CopyMem (&CurrentSlot, PortInfo, sizeof (PORT_INFO));

    ++RetBusNum; // UP Bridge
    SBus = Bus + RetBusNum; // DS Bridge

    if (SBus + 1 >= PortInfo->BusNumLimit) {
      continue;

    }

    SetDevResources (Bus, Device, 1, PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX1 * 4), PortInfo);

    //
    // Init UP Bridge to reach DS Bridge
    //
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, Bus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, SBus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, PortInfo->BusNumLimit);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, CMD_BM_MEM);

  if(ArPcie) {
    UnsetVesc(Bus, Device, 0x00);
  }

  UsedBusNumbers = ConfigureSlot(SBus, PCI_MAX_DEVICE + 1, -1, FALSE, PortInfo);
  RetBusNum += UsedBusNumbers;

    SetPhyPortResources (
      Bus,
      Device,
      SBus + UsedBusNumbers,
      Depth,
      &CurrentSlot,
      PortInfo
      );
  }
  //
  // for (Device = 0; Device <= PCI_MAX_DEVICE; Device++)
  //
  return RetBusNum;
} // ConfigureSlot

STATIC
VOID
SetCioPortResources (
  IN       UINT8     Bus,
  IN       UINT8     Dev,
  IN       UINT8     SBus,
  IN       UINT8     SubBus,
  IN       PORT_INFO  *portInfoBeforeChange,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8 Cmd;
  Cmd               = CMD_BUS_MASTER;

  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, Dev, 0x00, 0);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, Bus);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, SBus);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, SubBus);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);

  if (PortInfo->IoBase <= PortInfo->IoLimit) {
    PciSegmentWrite8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase), PortInfo->IoBase);
    PciSegmentWrite8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit), PortInfo->IoLimit);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBaseUpper16), 0x00000000);
    Cmd |= CMD_BM_IO;
  } else {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase), DISBL_IO_REG1C);
  }

  if (PortInfo->MemBase <= PortInfo->MemLimit) {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), PortInfo->MemBase);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit), PortInfo->MemLimit);
    Cmd |= CMD_BM_MEM;
  } else {
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), DISBL_MEM32_REG20);
  }

  if (PortInfo->PMemBase64 <= PortInfo->PMemLimit64) {
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), (UINT16) (PortInfo->PMemBase64 & 0xFFFF));
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit), (UINT16) (PortInfo->PMemLimit64 & 0xFFFF));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), (UINT32) (PortInfo->PMemBase64 >> 16));
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), (UINT32) (PortInfo->PMemLimit64 >> 16));
    Cmd |= CMD_BM_MEM;
  } else {
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), DISBL_PMEM_REG24);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), 0);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32), 0);
  }

  PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, Cmd);
  PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
} // SetCioPortResources

STATIC
VOID
SetSlotsAsUnused (
  IN       UINT8      Bus,
  IN       UINT8      MaxSlotNum,
  IN       UINT8      CioSlot,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  UINT8 Slot;
  for (Slot = MaxSlotNum; Slot > CioSlot; --Slot) {
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, Slot, 0x00, 0);
    if (0xFFFF == PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
      continue;
    }

    PciSegmentWrite8 (gDeviceBaseAddress + PCI_CACHELINE_SIZE_OFFSET, DEF_CACHE_LINE_SIZE);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, Bus);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, PortInfo->BusNumLimit);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, PortInfo->BusNumLimit);
    PciSegmentWrite16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase), DISBL_IO_REG1C);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase), DISBL_MEM32_REG20);
    PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase), DISBL_PMEM_REG24);
    PciSegmentWrite8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, CMD_BUS_MASTER);
    PortInfo->BusNumLimit--;
  }
} // SetSlotsAsUnused

STATIC
UINT16
FindVendorSpecificHeader(
  IN  UINT8  Bus
)
{
  PCI_EXP_EXT_HDR   *ExtHdr;
  UINT32            ExtHdrValue;
  UINT16            ExtendedRegister;

  ExtHdr = (PCI_EXP_EXT_HDR*) &ExtHdrValue;
  ExtendedRegister  = 0x100;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, 0x00, 0x00, 0);
  while (ExtendedRegister) {
    ExtHdrValue = PciSegmentRead32 (gDeviceBaseAddress + ExtendedRegister);
    if (ExtHdr->CapabilityId == 0xFFFF) {
      return 0x0000; // No Vendor-Specific Extended Capability header
    }

    if (PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID == ExtHdr->CapabilityId) {
      return ExtendedRegister;
    }

    ExtendedRegister = (UINT16) ExtHdr->NextCapabilityOffset;
  }
  return 0x0000; // No Vendor-Specific Extended Capability header
}

STATIC
UINT8
FindSsid_SsvidHeader (
  IN    UINT8  Bus
  )
{
  UINT8 CapHeaderId;
  UINT8 CapHeaderOffset;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, 0x00, 0x00, 0);
  CapHeaderOffset   = PciSegmentRead8 (gDeviceBaseAddress + PCI_CAPBILITY_POINTER_OFFSET);

  while (CapHeaderOffset != 0) {
    CapHeaderId = PciSegmentRead8 (gDeviceBaseAddress + CapHeaderOffset);

    if (CapHeaderId == PCIE_CAP_ID_SSID_SSVID) {
      return CapHeaderOffset;
    }

    CapHeaderOffset = PciSegmentRead8 (gDeviceBaseAddress + CapHeaderOffset + 1);
  }

  DEBUG((DEBUG_INFO, "SID0\n"));
  return 0;
} // FindSsid_SsvidHeader

STATIC
BOOLEAN
GetCioSlotByDevId (
  IN   UINT8  Bus,
  OUT  UINT8  *CioSlot,
  OUT  UINT8  *MaxSlotNum,
  OUT  BOOLEAN *ArPcie
  )
{
  UINT16            VSECRegister;
  BRDG_CIO_MAP_REG  BridgMap;
  UINT32            BitScanRes;
  UINT16            DevId;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, Bus, 0x00, 0x00, 0);
  DevId             = PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);

  //
  // Init out params in case device is not recognised
  //
  *CioSlot    = 4;
  *MaxSlotNum = 7;
  *ArPcie     = FALSE;

  switch (DevId) {
    //
    // For known device IDs
    //
    case 0x1578:
      *ArPcie = TRUE;
  }

  switch (DevId) {
  //
  // For known device IDs
  //
  case 0x1513:
  case 0x151A:
  case 0x151B:
  case 0x1547:
  case 0x1548:
    return TRUE; // Just return
  case 0x1549:
    return FALSE; // Just return
  }

  VSECRegister = FindVendorSpecificHeader(Bus);
  if (!VSECRegister) {
    return TRUE; // Just return
  }
  //
  // Go to Bridge/CIO map register
  //
  VSECRegister += 0x18;
  BridgMap.AB_REG = PciSegmentRead32(gDeviceBaseAddress + VSECRegister);
  //
  // Check for range
  //
  if (BridgMap.Bits.NumOfDSPorts < 1 || BridgMap.Bits.NumOfDSPorts > 27) {
    return TRUE;
  //
  // Not a valid register
  //
  }
  //
  // Set OUT params
  //
  *MaxSlotNum = (UINT8) BridgMap.Bits.NumOfDSPorts;

#ifdef _MSC_VER
  if(!_BitScanForward(&BitScanRes, BridgMap.Bits.CioPortMap)) { // No DS bridge which is CIO port
    return FALSE;
  }
#else
#ifdef __GNUC__
  if (BridgMap.Bits.CioPortMap == 0) {
    return FALSE;
  }
  BitScanRes = __builtin_ctz (BridgMap.Bits.CioPortMap);
#else
#error Unsupported Compiler
#endif
#endif

  *CioSlot = (UINT8)BitScanRes;
  return TRUE;
} // GetCioSlotByDevId

#define TBT_LEGACY_SUB_SYS_ID 0x11112222

STATIC
BOOLEAN
IsLegacyDevice (
  IN    UINT8  Bus
  )
{
  UINT32  Sid;
  UINT8   SidRegister;
  UINT16  DevId;

  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, 0x00, 0x00, 0);
  DevId             = PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
  switch (DevId) {
  //
  // For known device IDs
  //
  case 0x1513:
  case 0x151A:
  case 0x151B:
    DEBUG((DEBUG_INFO, "Legacy "));
    DEBUG((DEBUG_INFO, "DevId = %d\n",DevId));
    return TRUE;
    //
    // Legacy device by Device Id
    //
  }

  SidRegister = FindSsid_SsvidHeader(Bus);

  if (!SidRegister) {
    return TRUE; // May be absent for legacy devices
  }
  //
  // Go to register
  //
  SidRegister += 0x4;
  Sid = PciSegmentRead32(gDeviceBaseAddress + SidRegister);
  DEBUG((DEBUG_INFO, "SID"));
  DEBUG((DEBUG_INFO, " = %d\n", Sid));

return TBT_LEGACY_SUB_SYS_ID == Sid || 0 == Sid;
} // IsLegacyDevice

STATIC
VOID
UnsetVescEp(
  IN  UINT8     Bus,
  IN  UINT8     MaxSlotNum
  )
{
  UINT8 i;

  for (i = 0; i <= MaxSlotNum; ++i)
  {
    UnsetVesc(Bus, i, 0);
  }
}// Unset_VESC_REG2_EP

STATIC
BOOLEAN
ConfigureEP (
  IN       INT8      Depth,
  IN  OUT  UINT8     *Bus,
  IN  OUT  PORT_INFO *PortInfo
  )
{
  UINT8      SBus;
  UINT8      CioSlot;
  UINT8      MaxSlotNum;
  BOOLEAN    ArPcie;
  UINT8      MaxPHYSlots;
  UINT8      UsedBusNumbers;
  UINT8      cmd;
  BOOLEAN    CioSlotPresent;
  BOOLEAN    Continue;
  PORT_INFO  PortInfoOrg;
  UINT8      CioBus;

  CioSlot     = 4;
  MaxSlotNum  = 7;
  CopyMem (&PortInfoOrg, PortInfo, sizeof (PORT_INFO));

  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, *Bus, 0x00, 0x00, 0);
  cmd               = PciSegmentRead8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET);
  // AR ONLY
  // Endpoint on CIO slot, but not a bridge device
  if (P2P_BRIDGE != PciSegmentRead16 (gDeviceBaseAddress + (PCI_CLASSCODE_OFFSET + 1))) {
    DEBUG((DEBUG_INFO, "UEP\n"));
    // Check whether EP already configured by examining CMD register
    if(cmd & CMD_BUS_MASTER) // Yes, no need to touch this EP
    {
      DEBUG((DEBUG_INFO, "BMF\n"));
      return FALSE;
    }
    // Configure it as regular PCIe device
    ConfigureSlot(*Bus, PCI_MAX_DEVICE + 1, -1, FALSE, PortInfo);

    return FALSE;
  }

  //
  // Based on Device ID assign Cio slot and max number of PHY slots to scan
  //
  CioSlotPresent  =  GetCioSlotByDevId(*Bus, &CioSlot, &MaxSlotNum, &ArPcie);
  MaxPHYSlots     = MaxSlotNum;
  //
  // Check whether EP already configured by examining CMD register
  //

  if (cmd & CMD_BUS_MASTER) {
    //
    // Yes no need to touch this EP, just move to next one in chain
    //
    CioBus = *Bus + 1;
    if(ArPcie){
      UnsetVescEp(CioBus, MaxSlotNum);
    }
    if (!CioSlotPresent) {
      //
      // Cio slot is not present in EP, just return FALSE
      //
      DEBUG((DEBUG_INFO, "BMF\n"));
      return FALSE;
    }
    //
    // Take all resources from Cio slot and return
    //
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,CioBus, CioSlot, 0x00, 0);
    PortInfo->BusNumLimit   = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
    PortInfo->IoBase        = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase));
    PortInfo->IoLimit       = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit));
    PortInfo->MemBase       = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase));
    PortInfo->MemLimit      = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit));
    PortInfo->PMemBase64    = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase)) & 0xFFF0;
    PortInfo->PMemLimit64   = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit)) & 0xFFF0;
    PortInfo->PMemBase64   |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32))) << 16;
    PortInfo->PMemLimit64  |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32))) << 16;
    PortInfo->PMemLimit64  |= 0xF;
    //
    // Jump to next EP
    //
    *Bus = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
    //
    // Should we continue?
    //
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,*Bus, 0x00, 0x00, 0);
    Continue          = 0xFFFF != PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
    return Continue;
  }
  //
  // Set is legacy dvice
  //
  isLegacyDevice = IsLegacyDevice (*Bus);

  SetCioPortResources (
    *Bus,
    0, // Assign all available resources to US port of EP
    *Bus + 1,
    PortInfo->BusNumLimit,
    0,
    PortInfo
    );

  SBus = *Bus + 1;// Jump to DS port

  if (CioSlotPresent) {
    MaxPHYSlots = CioSlot;
  }

  UsedBusNumbers = ConfigureSlot(SBus, MaxPHYSlots, Depth, ArPcie, PortInfo);
  if (!CioSlotPresent) {
    return FALSE;
    //
    // Stop resource assignment on this chain
    //
  }
  //
  // Set rest of slots us unused
  //
  SetSlotsAsUnused (SBus, MaxSlotNum, CioSlot, PortInfo);

  SetCioPortResources (
    SBus,
    CioSlot,
    SBus + UsedBusNumbers + 1,
    PortInfo->BusNumLimit,
    &PortInfoOrg,
    PortInfo
    );
  *Bus = SBus + UsedBusNumbers + 1;// Go to next EP
  if(ArPcie) {
    UnsetVesc(SBus, CioSlot, 0x00);
  }
  if (*Bus > PortInfo->BusNumLimit - 2) {
    //
    // In case of bus numbers are exhausted stop enumeration
    //
    return FALSE;
  }
  //
  // Check whether we should continue on this chain
  //
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,*Bus, 0x00, 0x00, 0);
  Continue          = 0xFFFF != PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
  return Continue;
} // ConfigureEP

STATIC
VOID
GetPortResources (
  IN       UINT8      Bus,
  IN       UINT8      Dev,
  IN       UINT8      Fun,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, Dev, Fun, 0);
  PortInfo->BusNumLimit   = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
  PortInfo->IoBase        = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoBase)) & 0xF0;
  PortInfo->IoLimit       = PciSegmentRead8 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.IoLimit)) & 0xF0;
  PortInfo->MemBase       = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryBase)) & 0xFFF0;
  PortInfo->MemLimit      = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.MemoryLimit)) & 0xFFF0;
  PortInfo->PMemBase64    = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryBase)) & 0xFFF0;
  PortInfo->PMemLimit64   = PciSegmentRead16 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableMemoryLimit)) & 0xFFF0;
  PortInfo->PMemBase64   |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32))) << 16;
  PortInfo->PMemLimit64  |= (UINT64)(PciSegmentRead32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableLimitUpper32))) << 16;
  PortInfo->IoLimit |= 0xF;
  PortInfo->MemLimit |= 0xF;
  PortInfo->PMemLimit64 |= 0xF;
} // GetPortResources

STATIC
VOID
ConfigurePort (
  IN       UINT8      Bus,
  IN       UINT8      Dev,
  IN       UINT8      Fun,
  IN  OUT  PORT_INFO  *PortInfo
  )
{
  INT8  i;
  UINT8 USBusNum;
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, Dev, Fun, 0);
  USBusNum          = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
  gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment, USBusNum, 0x00, 0x00, 0);
  if (0xFFFF == PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET)) {
    //
    // Nothing to do if TBT device is not connected
    //
    return ;
  }

  GetPortResources(Bus, Dev, Fun, PortInfo);// Take reserved resources from DS port
  //
  // Assign resources to EPs
  //
  for (i = 0; i < MAX_TBT_DEPTH; ++i) {
    PortInfo->ConfedEP++;
    if (!ConfigureEP (i, &USBusNum, PortInfo)) {
      return ;
    }
  }
} // ConfigurePort

VOID
ThunderboltCallback (
  IN UINT8 Type
  )
{
  PORT_INFO                     PortInfoOrg  = { 0 };
  HR_CONFIG                     HrConfig  = { 0 };
  UINT8                         i;
  UINTN                         Segment = 0;
  UINTN                         Bus = 0;
  UINTN                         Device;
  UINTN                         Function;

  DEBUG((DEBUG_INFO, "ThunderboltCallback.Entry\n"));

  DEBUG((DEBUG_INFO, "PortInfo Initialization\n"));
  PortInfoInit (&PortInfoOrg);
  if(Type == DTBT_CONTROLLER) {
    if (gCurrentDiscreteTbtRootPort == 0) {
      DEBUG((DEBUG_ERROR, "Invalid RP Input\n"));
      return;
    }
    GetDTbtRpDevFun(gCurrentDiscreteTbtRootPortType, gCurrentDiscreteTbtRootPort - 1, &Device, &Function);
    DEBUG((DEBUG_INFO, "InitializeHostRouter. \n"));
    if (!InitializeHostRouter (&HrConfig, Segment, Bus, Device, Function)) {
      return ;
    }
  //
  // Configure DS ports
  //
  for (i = HrConfig.MinDSNumber; i <= HrConfig.MaxDSNumber; ++i) {
    DEBUG((DEBUG_INFO, "ConfigurePort. \n"));
    ConfigurePort (HrConfig.HRBus + 1, i,0, &PortInfoOrg);
  }

  DEBUG((DEBUG_INFO, "EndOfThunderboltCallback.\n"));
  EndOfThunderboltCallback (Segment, Bus, Device, Function);

  }
  DEBUG((DEBUG_INFO, "ThunderboltCallback.Exit\n"));
} // ThunderboltCallback

VOID
DisablePCIDevicesAndBridges (
  IN UINT8 MinBus,
  IN UINT8 MaxBus
  )
{
  UINT8   Bus;
  UINT8   Dev;
  UINT8   Fun;
  UINT8   RegVal;
  //
  //  Disable PCI device First, and then Disable PCI Bridge
  //
  for (Bus = MaxBus; Bus > MinBus; --Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, Dev, Fun, 0);
        if (INVALID_PCI_DEVICE == PciSegmentRead32 (gDeviceBaseAddress + PCI_VENDOR_ID_OFFSET)) {
          if (Fun == 0) {
            break;

          }

          continue;
        }

        RegVal = PciSegmentRead8 (gDeviceBaseAddress + PCI_HEADER_TYPE_OFFSET);
        if (HEADER_TYPE_DEVICE == (RegVal & 1)) {
          //
          // ********     Disable PCI Device   ********
          // BIT0  I/O Space Enabled    BIT1  Memory Space Enabled
          // BIT2  Bus Master Enabled   BIT4  Memory Write and Invalidation Enable
          //
          PciSegmentAnd8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, (UINT8)~(BIT0 | BIT1 | BIT2 | BIT4));
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX0 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX1 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX2 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX3 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX4 * 4), 0);
          PciSegmentWrite32 (gDeviceBaseAddress + PCI_BASE_ADDRESSREG_OFFSET + (PCI_BAR_IDX5 * 4), 0);
        }
      }
    }
  }
  //
  // now no more PCI dev on another side of PCI Bridge can safty disable PCI Bridge
  //
  for (Bus = MaxBus; Bus > MinBus; --Bus) {
    for (Dev = 0; Dev <= PCI_MAX_DEVICE; ++Dev) {
      for (Fun = 0; Fun <= PCI_MAX_FUNC; ++Fun) {
        gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (TbtSmiHandler_TbtSegment,Bus, Dev, Fun, 0);
        if (INVALID_PCI_DEVICE == PciSegmentRead32 (gDeviceBaseAddress + PCI_VENDOR_ID_OFFSET)) {
          if (Fun == 0) {
            break;
          }

          continue;
        }

        RegVal = PciSegmentRead8 (gDeviceBaseAddress + PCI_HEADER_TYPE_OFFSET);
        if (HEADER_TYPE_PCI_TO_PCI_BRIDGE == (RegVal & BIT0)) {
          PciSegmentAnd8 (gDeviceBaseAddress + PCI_COMMAND_OFFSET, (UINT8)~(BIT0 | BIT1 | BIT2 | BIT4));
          PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
          PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, 0);
          PciSegmentWrite8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, 0);
          PciSegmentWrite32 (gDeviceBaseAddress + OFFSET_OF (PCI_TYPE01, Bridge.PrefetchableBaseUpper32), 0);
        }
      } // for ( Fun .. )
    } // for ( Dev ... )
  } // for ( Bus ... )
} // DisablePCIDevicesAndBridges

VOID
TbtDisablePCIDevicesAndBridges (
  IN UINT8 Type
  )
{
  UINTN         Segment = 0;
  UINTN         Bus = 0;
  UINTN         Device;
  UINTN         Function;
  UINT8         MinBus;
  UINT8         MaxBus;
  UINT16        DeviceId;

  MinBus = 1;
  if(Type == DTBT_CONTROLLER) {
    //
    // for(Dev = 0; Dev < 8; ++Dev)
    // {
    // PciOr8(PCI_LIB_ADDRESS(2, Dev, 0, PCI_BRIDGE_CONTROL_REGISTER_OFFSET), 0x40);
    // gBS->Stall(2000);      // 2msec
    // PciAnd8(PCI_LIB_ADDRESS(2, Dev, 0, PCI_BRIDGE_CONTROL_REGISTER_OFFSET), 0xBF);
    // }
    // gBS->Stall(200 * 1000);        // 200 msec
    //
    if (gCurrentDiscreteTbtRootPort == 0) {
      DEBUG((DEBUG_ERROR, "Invalid RP Input\n"));
      return;
    }
    GetDTbtRpDevFun(gCurrentDiscreteTbtRootPortType, gCurrentDiscreteTbtRootPort - 1, &Device, &Function);
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Device, Function, 0);
    MinBus            = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET);
    MaxBus            = PciSegmentRead8 (gDeviceBaseAddress + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET);
    gDeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, MinBus, 0x00, 0x00, 0);
    DeviceId          = PciSegmentRead16 (gDeviceBaseAddress + PCI_DEVICE_ID_OFFSET);
    if (!(IsTbtHostRouter (DeviceId))) {
      return;
    }
    TbtSmiHandler_TbtSegment = (UINT8)Segment;
    MinBus++;
    //
    // @todo : Move this out when we dont have Loop for ITBT
    //
    DisablePCIDevicesAndBridges(MinBus, MaxBus);

  }
} // DisablePCIDevicesAndBridges


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/Tbt/TbtInit/Dxe/TbtDxe.c
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/Tbt/TbtInit/Dxe/TbtDxe.c
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/Tbt/TbtInit/Dxe/TbtDxe.c

/** @file

  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//#include <Uefi.h>
//#include <Library/BaseMemoryLib.h>
//#include <Library/TbtCommonLib.h>
//#include <Library/DxeTbtPolicyLib.h>
//#include <TbtBoardInfo.h>
//#include <Protocol/DxeTbtPolicy.h>
//#include <Protocol/TbtNvsArea.h>
//#include <Library/DebugLib.h>
//#include <Library/HobLib.h>
//#include <Library/UefiBootServicesTableLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>
//#include <Library/UefiLib.h>
//#include <Uefi/UefiSpec.h>
//#include <Library/PcdLib.h>
//#include <Library/AslUpdateLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED TBT_NVS_AREA_PROTOCOL                     mTbtNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED TBT_INFO_HOB                              *TbtDxe_gTbtInfoHob = NULL;

/**
  TBT NVS Area Initialize

**/

VOID
TbtNvsAreaInit (
  IN  VOID              **mTbtNvsAreaPtr
  )
{
  UINTN                         Pages;
  EFI_PHYSICAL_ADDRESS          Address;
  EFI_STATUS                    Status;
  TBT_NVS_AREA_PROTOCOL         *TbtNvsAreaProtocol;
  DXE_TBT_POLICY_PROTOCOL       *DxeTbtConfig;

  DEBUG ((DEBUG_INFO, "TbtNvsAreaInit Start\n"));
  Status = gBS->LocateProtocol (
              &gDxeTbtPolicyProtocolGuid,
              NULL,
              (VOID **) &DxeTbtConfig
              );
  ASSERT_EFI_ERROR (Status);

  Pages = EFI_SIZE_TO_PAGES (sizeof (TBT_NVS_AREA));
  Address = 0xffffffff; // allocate address below 4G.

  Status  = gBS->AllocatePages (
                   AllocateMaxAddress,
                   EfiACPIMemoryNVS,
                   Pages,
                   &Address
                   );
  ASSERT_EFI_ERROR (Status);

  *mTbtNvsAreaPtr = (VOID *) (UINTN) Address;
  SetMem (*mTbtNvsAreaPtr, sizeof (TBT_NVS_AREA), 0);

  //
  // TBTNvsAreaProtocol default value init here
  //
  TbtNvsAreaProtocol = (TBT_NVS_AREA_PROTOCOL *) &Address;

  //
  // Initialize default values
  //
  TbtNvsAreaProtocol->Area->WAKFinished             = 0;
  TbtNvsAreaProtocol->Area->DiscreteTbtSupport      = ((TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].DTbtControllerEn == 1 ) ? TRUE : FALSE);
  TbtNvsAreaProtocol->Area->TbtAcpiRemovalSupport   = 0;
  TbtNvsAreaProtocol->Area->TbtGpioFilter           = (UINT8) DxeTbtConfig->TbtCommonConfig.Gpio5Filter;
//  TbtNvsAreaProtocol->Area->TrOsup                  = (UINT8) DxeTbtConfig->TbtCommonConfig.TrA0OsupWa;
  TbtNvsAreaProtocol->Area->TbtFrcPwrEn             = TbtDxe_gTbtInfoHob->DTbtCommonConfig.Gpio3ForcePwr;
  TbtNvsAreaProtocol->Area->TbtAspm                 = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtAspm;
//  TbtNvsAreaProtocol->Area->TbtL1SubStates          = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtL1SubStates;
  TbtNvsAreaProtocol->Area->TbtSetClkReq            = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtSetClkReq;
  TbtNvsAreaProtocol->Area->TbtLtr                  = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtLtr;
//  TbtNvsAreaProtocol->Area->TbtPtm                  = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtPtm;
  TbtNvsAreaProtocol->Area->TbtWakeupSupport        = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtWakeupSupport;
  TbtNvsAreaProtocol->Area->TbtAcDcSwitch           = (UINT8) DxeTbtConfig->TbtCommonConfig.TbtAcDcSwitch;
  TbtNvsAreaProtocol->Area->Rtd3TbtSupport          = (UINT8) DxeTbtConfig->TbtCommonConfig.Rtd3Tbt;             // TBT RTD3 Enable.
  TbtNvsAreaProtocol->Area->Rtd3TbtOffDelay         = (UINT16) DxeTbtConfig->TbtCommonConfig.Rtd3TbtOffDelay;    // TBT RTD3 Off delay in ms.
  TbtNvsAreaProtocol->Area->Rtd3TbtClkReq           = (UINT8) DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReq;       // TBT RTD3 ClkReq Mask Enable.
  TbtNvsAreaProtocol->Area->Rtd3TbtClkReqDelay      = (UINT16) DxeTbtConfig->TbtCommonConfig.Rtd3TbtClkReqDelay; // TBT RTD3 ClkReq mask delay in ms.
  TbtNvsAreaProtocol->Area->TbtWin10Support         = (UINT8) DxeTbtConfig->TbtCommonConfig.Win10Support; // TBT FW Execution Mode

  //
  // DTBT Controller 1
  //
  TbtNvsAreaProtocol->Area->DTbtControllerEn0       = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].DTbtControllerEn;
  TbtNvsAreaProtocol->Area->RootportSelected0       = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].PcieRpNumber;
  TbtNvsAreaProtocol->Area->RootportSelected0Type   = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].Type;
  TbtNvsAreaProtocol->Area->RootportEnabled0        = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].DTbtControllerEn;
  TbtNvsAreaProtocol->Area->TbtFrcPwrGpioNo0        = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].ForcePwrGpio.GpioPad;
  TbtNvsAreaProtocol->Area->TbtFrcPwrGpioLevel0     = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].ForcePwrGpio.GpioLevel;
  TbtNvsAreaProtocol->Area->TbtCioPlugEventGpioNo0  = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].CioPlugEventGpio.GpioPad;
  TbtNvsAreaProtocol->Area->TbtPcieRstGpioNo0       = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].PcieRstGpio.GpioPad;
  TbtNvsAreaProtocol->Area->TbtPcieRstGpioLevel0    = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].PcieRstGpio.GpioLevel;

  TbtNvsAreaProtocol->Area->TBtCommonGpioSupport    = TbtDxe_gTbtInfoHob->DTbtCommonConfig.DTbtSharedGpioConfiguration;

  DEBUG ((DEBUG_INFO, "TbtNvsAreaInit End\n"));
}

/**
  This function gets registered as a callback to patch TBT ASL code

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
  can we put this also in read me
**/
VOID
EFIAPI
TbtAcpiEndOfDxeCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;
  UINT32                                Signature;

  Address = (UINT32) (UINTN) mTbtNvsAreaProtocol.Area;
  Length  = (UINT16) sizeof (TBT_NVS_AREA);
  DEBUG ((DEBUG_INFO, "Patch TBT NvsAreaAddress: TBT NVS Address %x Length %x\n", Address, Length));
  Status  = UpdateNameAslCode (SIGNATURE_32 ('T','N','V','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('T','N','V','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  if (TbtDxe_gTbtInfoHob != NULL) {
    if (TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].DTbtControllerEn == 1) {
      if (TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].CioPlugEventGpio.AcpiGpeSignaturePorting == TRUE) {
        DEBUG ((DEBUG_INFO, "Patch ATBT Method Name\n"));
        Signature = TbtDxe_gTbtInfoHob-> DTbtControllerConfig[0].CioPlugEventGpio.AcpiGpeSignature;
        Status  = UpdateNameAslCode (SIGNATURE_32 ('A','T','B','T'), &Signature, sizeof (Signature));
        ASSERT_EFI_ERROR (Status);
      }
    }
  }

  return;
}

/**
  Initialize Thunderbolt(TM) SSDT ACPI tables

  @retval EFI_SUCCESS    ACPI tables are initialized successfully
  @retval EFI_NOT_FOUND  ACPI tables not found
**/

EFI_STATUS
EFIAPI
TbtDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;
 // EFI_EVENT               EndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "TbtDxeEntryPoint \n"));

  //
  // Get TBT INFO HOB
  //
  TbtDxe_gTbtInfoHob = (TBT_INFO_HOB *) GetFirstGuidHob (&gTbtInfoHobGuid);
  if (TbtDxe_gTbtInfoHob == NULL) {
    return EFI_NOT_FOUND;
  }
  InstallTbtPolicy (ImageHandle);
  //
  // Update DXE TBT Policy
  //
  UpdateTbtPolicyCallback ();

  //
  // Print DXE TBT Policy
  //
  TbtPrintDxePolicyConfig ();

  //
  // Initialize Tbt Nvs Area
  //
  TbtNvsAreaInit ((VOID **) &mTbtNvsAreaProtocol.Area);


  //
  // [ACPI] Thunderbolt ACPI table
  //


  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gTbtNvsAreaProtocolGuid,
                  &mTbtNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register an end of DXE event for TBT ACPI to do some patch can be put as description
  //
  /**
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  TbtAcpiEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
**/
  return EFI_SUCCESS;
}


//========================================================================================
// edk2-platforms/Platform/Intel/KabylakeOpenBoardPkg/Features/PciHotPlug/PciHotPlug.c
// edk2-platforms/Platform/Intel/WhiskeylakeOpenBoardPkg/Features/PciHotPlug/PciHotPlug.c
// edk2-platforms/Platform/Intel/CometlakeOpenBoardPkg/Features/PciHotPlug/PciHotPlug.c

/** @file
  Pci Hotplug Driver : This file will perform specific PCI-EXPRESS
  Devics resource configuration.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//
//#include "PciHotPlug.h"
//#include <Ppi/SiPolicy.h>
//#include <TbtBoardInfo.h>
//#include <Library/PchPcieRpLib.h>
//#include <Library/TbtCommonLib.h>

#define PCIE_NUM  (20)
#define PEG_NUM   (3)
#define PADDING_BUS (1)
#define PADDING_NONPREFETCH_MEM (1)
#define PADDING_PREFETCH_MEM (1)
#define PADDING_IO (1)
#define PADDING_NUM (PADDING_BUS + PADDING_NONPREFETCH_MEM + PADDING_PREFETCH_MEM + PADDING_IO)

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HPC_LOCATION          mPcieLocation[PCIE_NUM + PEG_NUM];

GLOBAL_REMOVE_IF_UNREFERENCED UINTN mHpcCount = 0;

GLOBAL_REMOVE_IF_UNREFERENCED PCIE_HOT_PLUG_DEVICE_PATH mHotplugPcieDevicePathTemplate = {
  ACPI,
  PCI(0xFF, 0xFF), // Dummy Device no & Function no
  END
};

/**
  Entry point for the driver.

  This routine reads the PlatformType GPI on FWH and produces a protocol
  to be consumed by the chipset driver to effect those settings.

  @param[in]  ImageHandle    An image handle.
  @param[in]  SystemTable    A pointer to the system table.

  @retval     EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
PciHotPlug (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                       Status;
  PCI_HOT_PLUG_INSTANCE            *PciHotPlug;
  UINTN                            Index;
  UINTN                            RpDev;
  UINTN                            RpFunc;
  PCIE_HOT_PLUG_DEVICE_PATH       *HotplugPcieDevicePath;
  UINT32                           PcieRootPortHpeData = 0;

  DEBUG ((DEBUG_INFO, "PciHotPlug Entry\n"));

  PcieRootPortHpeData = PcdGet32 (PcdPchPcieRootPortHpe);
  //
  // PCH Rootports Hotplug device path creation
  //
  for (Index = 0; Index < PCIE_NUM; Index++) {
    if (((PcieRootPortHpeData >> Index) & BIT0) == BIT0) { // Check the Rootport no's hotplug is set
      Status = GetPchPcieRpDevFun (Index, &RpDev, &RpFunc); // Get the actual device/function no corresponding to the Rootport no provided
      ASSERT_EFI_ERROR (Status);

      HotplugPcieDevicePath = NULL;
      HotplugPcieDevicePath = AllocatePool (sizeof (PCIE_HOT_PLUG_DEVICE_PATH));
      ASSERT (HotplugPcieDevicePath != NULL);
      if (HotplugPcieDevicePath == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      CopyMem (HotplugPcieDevicePath, &mHotplugPcieDevicePathTemplate, sizeof (PCIE_HOT_PLUG_DEVICE_PATH));
      HotplugPcieDevicePath->PciRootPortNode.Device = (UINT8) RpDev; // Update real Device no
      HotplugPcieDevicePath->PciRootPortNode.Function = (UINT8) RpFunc; // Update real Function no

      mPcieLocation[mHpcCount].HpcDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)HotplugPcieDevicePath;
      mPcieLocation[mHpcCount].HpbDevicePath = (EFI_DEVICE_PATH_PROTOCOL *)HotplugPcieDevicePath;
      mHpcCount++;

      DEBUG ((DEBUG_INFO, "(%02d) PciHotPlug (PCH RP#) : Bus 0x00, Device 0x%x, Function 0x%x is added to the Hotplug Device Path list \n", mHpcCount, RpDev, RpFunc));
    }
  }


  PciHotPlug = AllocatePool (sizeof (PCI_HOT_PLUG_INSTANCE));
  ASSERT (PciHotPlug != NULL);
  if (PciHotPlug == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Initialize driver private data.
  //
  ZeroMem (PciHotPlug, sizeof (PCI_HOT_PLUG_INSTANCE));

  PciHotPlug->Signature                               = PCI_HOT_PLUG_DRIVER_PRIVATE_SIGNATURE;
  PciHotPlug->HotPlugInitProtocol.GetRootHpcList      = GetRootHpcList;
  PciHotPlug->HotPlugInitProtocol.InitializeRootHpc   = InitializeRootHpc;
  PciHotPlug->HotPlugInitProtocol.GetResourcePadding  = GetResourcePadding;

  Status = gBS->InstallProtocolInterface (
                  &PciHotPlug->Handle,
                  &gEfiPciHotPlugInitProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &PciHotPlug->HotPlugInitProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}


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
  IN EFI_PCI_HOT_PLUG_INIT_PROTOCOL    *This,
  OUT UINTN                            *HpcCount,
  OUT EFI_HPC_LOCATION                 **HpcList
  )
{
  *HpcCount = mHpcCount;
  *HpcList  = mPcieLocation;

  return EFI_SUCCESS;
}


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
  IN  EFI_PCI_HOT_PLUG_INIT_PROTOCOL      *This,
  IN  EFI_DEVICE_PATH_PROTOCOL            *HpcDevicePath,
  IN  UINT64                              HpcPciAddress,
  IN  EFI_EVENT                           Event, OPTIONAL
  OUT EFI_HPC_STATE                       *HpcState
  )
{
  if (Event) {
    gBS->SignalEvent (Event);
  }

  *HpcState = EFI_HPC_STATE_INITIALIZED;

  return EFI_SUCCESS;
}


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
  IN  EFI_DEVICE_PATH_PROTOCOL        *HpcDevicePath,
  IN  UINT64                          HpcPciAddress,
  OUT EFI_HPC_STATE                   *HpcState,
  OUT VOID                            **Padding,
  OUT EFI_HPC_PADDING_ATTRIBUTES      *Attributes
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *PaddingResource;
  EFI_STATUS                        Status;
  UINT8                             RsvdExtraBusNum = 0;
  UINT16                            RsvdPcieMegaMem = 10;
  UINT8                             PcieMemAddrRngMax = 0;
  UINT16                            RsvdPciePMegaMem = 10;
  UINT8                             PciePMemAddrRngMax = 0;
  UINT8                             RsvdTbtExtraBusNum = 0;
  UINT16                            RsvdTbtPcieMegaMem = 10;
  UINT8                             TbtPcieMemAddrRngMax = 0;
  UINT16                            RsvdTbtPciePMegaMem = 10;
  UINT8                             TbtPciePMemAddrRngMax = 0;
  UINT8                             RsvdPcieKiloIo = 4;
  BOOLEAN                           SetResourceforTbt = FALSE;
  UINTN                             RpIndex;
  UINTN                             RpDev;
  UINTN                             RpFunc;

DEBUG ((DEBUG_INFO, "GetResourcePadding : Start \n"));

  PaddingResource = AllocatePool (PADDING_NUM * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));
  ASSERT (PaddingResource != NULL);
  if (PaddingResource == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *Padding = (VOID *) PaddingResource;

  RpDev = (UINTN) ((HpcPciAddress >> 16) & 0xFF);
  RpFunc = (UINTN) ((HpcPciAddress >> 8) & 0xFF);

  // Get the actual Rootport no corresponding to the device/function no provided
  if (RpDev == SA_PEG_DEV_NUM) {
    // PEG
    RpIndex = PCIE_NUM + RpFunc;
    DEBUG ((DEBUG_INFO, "GetResourcePadding : PEG Rootport no %02d Bus 0x00, Device 0x%x, Function 0x%x \n", (RpIndex-PCIE_NUM), RpDev, RpFunc));
  } else {
    // PCH
    Status = GetPchPcieRpNumber (RpDev, RpFunc, &RpIndex);
    DEBUG ((DEBUG_INFO, "GetResourcePadding : PCH Rootport no %02d Bus 0x00, Device 0x%x, Function 0x%x \n", RpIndex, RpDev, RpFunc));
  }

  GetRootporttoSetResourcesforTbt(RpIndex, &RsvdTbtExtraBusNum, &RsvdTbtPcieMegaMem ,&TbtPcieMemAddrRngMax ,&RsvdTbtPciePMegaMem ,&TbtPciePMemAddrRngMax, &SetResourceforTbt);
    if (SetResourceforTbt) {
      RsvdExtraBusNum = RsvdTbtExtraBusNum;
      RsvdPcieMegaMem = RsvdTbtPcieMegaMem;
      PcieMemAddrRngMax = TbtPcieMemAddrRngMax;
      RsvdPciePMegaMem = RsvdTbtPciePMegaMem;
      PciePMemAddrRngMax = TbtPciePMemAddrRngMax;
    }

  //
  // Padding for bus
  //
  ZeroMem (PaddingResource, PADDING_NUM * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));
  *Attributes                   = EfiPaddingPciBus;

  PaddingResource->Desc         = 0x8A;
  PaddingResource->Len          = 0x2B;
  PaddingResource->ResType      = ACPI_ADDRESS_SPACE_TYPE_BUS;
  PaddingResource->GenFlag      = 0x0;
  PaddingResource->SpecificFlag = 0;
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrRangeMax = 0;
  PaddingResource->AddrLen      = RsvdExtraBusNum;

  //
  // Padding for non-prefetchable memory
  //
  PaddingResource++;
  PaddingResource->Desc                 = 0x8A;
  PaddingResource->Len                  = 0x2B;
  PaddingResource->ResType              = ACPI_ADDRESS_SPACE_TYPE_MEM;
  PaddingResource->GenFlag              = 0x0;
    if (SetResourceforTbt) {
    PaddingResource->AddrSpaceGranularity = 32;
  } else {
    PaddingResource->AddrSpaceGranularity = 32;
  }
  PaddingResource->SpecificFlag         = 0;
  //
  // Pad non-prefetchable
  //
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = RsvdPcieMegaMem * 0x100000;
  if (SetResourceforTbt) {
    PaddingResource->AddrRangeMax = (1 << PcieMemAddrRngMax) - 1;
  } else {
    PaddingResource->AddrRangeMax = 1;
  }

  //
  // Padding for prefetchable memory
  //
  PaddingResource++;
  PaddingResource->Desc                 = 0x8A;
  PaddingResource->Len                  = 0x2B;
  PaddingResource->ResType              = ACPI_ADDRESS_SPACE_TYPE_MEM;
  PaddingResource->GenFlag              = 0x0;
    if (SetResourceforTbt) {
    PaddingResource->AddrSpaceGranularity = 32;
  } else {
    PaddingResource->AddrSpaceGranularity = 32;
  }
  PaddingResource->SpecificFlag         = 06;
  //
  // Padding for prefetchable memory
  //
  PaddingResource->AddrRangeMin = 0;
  if (SetResourceforTbt) {
    PaddingResource->AddrLen      = RsvdPciePMegaMem * 0x100000;
  } else {
    PaddingResource->AddrLen      = RsvdPcieMegaMem * 0x100000;
  }
  //
  // Pad 16 MB of MEM
  //
  if (SetResourceforTbt) {
    PaddingResource->AddrRangeMax = (1 << PciePMemAddrRngMax) - 1;
  } else {
    PaddingResource->AddrRangeMax = 1;
  }
  //
  // Alignment
  //
  // Padding for I/O
  //
  PaddingResource++;
  PaddingResource->Desc         = 0x8A;
  PaddingResource->Len          = 0x2B;
  PaddingResource->ResType      = ACPI_ADDRESS_SPACE_TYPE_IO;
  PaddingResource->GenFlag      = 0x0;
  PaddingResource->SpecificFlag = 0;
  PaddingResource->AddrRangeMin = 0;
  PaddingResource->AddrLen      = RsvdPcieKiloIo * 0x400;
  //
  // Pad 4K of IO
  //
  PaddingResource->AddrRangeMax = 1;
  //
  // Alignment
  //
  // Terminate the entries.
  //
  PaddingResource++;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Desc     = ACPI_END_TAG_DESCRIPTOR;
  ((EFI_ACPI_END_TAG_DESCRIPTOR *) PaddingResource)->Checksum = 0x0;

  *HpcState = EFI_HPC_STATE_INITIALIZED | EFI_HPC_STATE_ENABLED;

  return EFI_SUCCESS;
}


//========================================================================================
