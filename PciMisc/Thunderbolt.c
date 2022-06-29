



//========================================================================================
// Thunderbolt functions

#define Tbt2Pcie(b) (b->TbtType == ITBT_CONTROLLER ? TBT2PCIE_ITBT_R : TBT2PCIE_DTBT_R)
#define Pcie2Tbt(b) (b->TbtType == ITBT_CONTROLLER ? PCIE2TBT_ITBT_R : PCIE2TBT_DTBT_R)


//========================================================================================
// PeiDTbtInitLib.c

/**
Execute TBT PCIE2TBT_SX_EXIT_TBT_CONNECTED Mail Box Command for S4 mode with PreBootAclEnable

@param[in]  b         Host Router (DTBT)
@param[in]  Timeout   Time out with 100 ms granularity
@Retval     true      if command executes successfully
**/
BOOLEAN
TbtSetPcie2TbtSxExitCommandWithPreBootAclEnable(
   IN    PCI_IO_DEVICE           *b,
   IN    UINT32                  Timeout
)
{
  UINT32      RegisterValue;
  BOOLEAN     ReturnFlag;
  UINT32      Command;

// If PreBootAcl is Enable, we need to enable DATA bit while sending SX EXIT MAIL BOX Command
  Command = (1 << 8) | PCIE2TBT_SX_EXIT_TBT_CONNECTED;
  PciWrite32(b, Pcie2Tbt(b), Command | PCIE2TBT_VLD_B);

  ReturnFlag = InternalWaitforCommandCompletion(b, Timeout, TRUE, &RegisterValue);

  PciWrite32 (b, Pcie2Tbt(b), 0);

  return ReturnFlag;
}

/**
Set the Sleep Mode if the Host Router is up.
@param[in]  b         Host Router (DTBT)
**/
VOID
TbtSetSxMode(
IN    PCI_IO_DEVICE           *b,
IN    UINT8                   TbtBootOn
)
{
  UINT32                          RegVal;
  UINT32                          MaxLoopCount;
  UINTN                           Delay;
  UINT8                           RetCode;
  EFI_BOOT_MODE                   BootMode;
  EFI_STATUS                      Status;

  MaxLoopCount = TBT_5S_TIMEOUT;  // Wait 5 sec
  Delay = 100 * 1000;
  RetCode = 0x62;

  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR(Status);

  if ((BootMode == BOOT_ON_S4_RESUME) && (TbtBootOn == 2)) {
    MaxLoopCount = TBT_3S_TIMEOUT;
    if (!TbtSetPcie2TbtSxExitCommandWithPreBootAclEnable(b, MaxLoopCount)) {
      //
      // Nothing to wait, Host Router is not responsive
      //
      return;
    }
  }
  else {
    if (!TbtSetPcie2TbtCommand(PCIE2TBT_SX_EXIT_TBT_CONNECTED, b, MaxLoopCount)) {
      //
      // Nothing to wait, Host Router is not responsive
      //
      return;
    }
  }

  DEBUG((DEBUG_INFO, "Wait for Dev ID != 0xFF\n"));

  while (MaxLoopCount-- > 0) {
    //
    // Check what Host Router still here
    //
    RegVal = PciRead32(b, Tbt2Pcie(b));
    if (MAX_UINT32 == RegVal) {
      RetCode = 0x6F;
      break;
    }
    //
    // Check completion of TBT link
    //
    RegVal = PciRead32(b, PCI_VENDOR_ID_OFFSET);
    if (MAX_UINT32 != RegVal) {
      RetCode = 0x61;
      break;
    }

    MicroSecondDelay(Delay);
  }

  DEBUG((DEBUG_INFO, "Return code = 0x%x\n", RetCode));
}

/**
  Clear VGA Registers for DTBT.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtClearVgaRegisters (
  IN  PCI_IO_DEVICE *b
)
{
  EFI_STATUS Status;
  UINT16     Data16;

  DEBUG ((DEBUG_INFO, "[ DTbtClearVgaRegisters\n"));

  Status = EFI_SUCCESS;

  ASSERT_EFI_ERROR(Status);
  //
  // VGA Enable and VGA 16-bit decode registers of Bridge control register of Root port where
  // Host router resides should be cleaned
  //

  Data16 = PciRead16(b->Parent, PCI_BRIDGE_CONTROL_REGISTER_OFFSET);
  Data16 &= (~(EFI_PCI_BRIDGE_CONTROL_VGA | EFI_PCI_BRIDGE_CONTROL_VGA_16));
  PciWrite16(b->Parent, PCI_BRIDGE_CONTROL_REGISTER_OFFSET, Data16);

  DEBUG ((DEBUG_INFO, "] DTbtClearVgaRegisters %r", Status));
  return Status;
}

/**
  Exectue Mail box command "Boot On".

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtBootOn(
  IN  PCI_IO_DEVICE *b
)
{
  EFI_STATUS Status;
  DEBUG((DEBUG_INFO, "[ DTbtBootOn\n"));

  Status = EFI_SUCCESS;

      //
      //When Thunderbolt(TM) boot [TbtBootOn] is enabled in bios setup we need to do the below:
      //Bios should send "Boot On" message through PCIE2TBT register
      //The Boot On command as described above would include the command and acknowledge from FW (with the default timeout in BIOS),
      //once the Boot On command is completed it is guaranteed that the AlpineRidge(AR) device is there and the PCI tunneling was done by FW,
      //next step from BIOS is enumeration using SMI
      //

      if (b->DTbtCommonConfig.TbtBootOn > 0) {
        //
        // Exectue Mail box command "Boot On / Pre-Boot ACL"
        //
        //Command may be executed only during boot/reboot and not during Sx exit flow
        if (b->DTbtCommonConfig.TbtBootOn == 1) {
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_BOOT_ON, b, TBT_5S_TIMEOUT)) {
            //
            // Nothing to wait, Host Router is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> DTbtBootOn - Boot On message sent failed \n"));
          }
        }
        if (b->DTbtCommonConfig.TbtBootOn == 2) {
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_PREBOOTACL, b, TBT_3S_TIMEOUT)) {
            //
            // Nothing to wait, Host Router is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> DTbtBootOn - Pre-Boot ACL message sent failed \n"));
          }
        }
      }

  DEBUG((DEBUG_INFO, "] DTbtBootOn %r", Status));
  return Status;
}

/**
  Exectue Mail box command "USB On".

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtUsbOn(
  IN  PCI_IO_DEVICE *b
)
{
  EFI_STATUS                      Status;
  UINT32                          MaxWaitIter;
  UINT32                          RegVal;
  EFI_BOOT_MODE                   BootMode;

  DEBUG((DEBUG_INFO, "[ DTbtUsbOn\n"));

  Status = EFI_SUCCESS;

      //
      //When Thunderbolt(TM) Usb boot [TbtUsbOn] is enabled in bios setup we need to do the below:
      //Bios should send "Usb On" message through PCIE2TBT register
      //The Usb On command as described above would include the command and acknowledge from FW (with the default timeout in BIOS),
      //once the Usb On command is completed it is guaranteed that the AlpineRidge(AR) device is there and the PCI tunneling was done by FW,
      //next step from BIOS is enumeration using SMI
      //
      if (b->DTbtCommonConfig.TbtUsbOn) {
        if (b->DTbtCommonConfig.TbtBootOn > 0) {
          MaxWaitIter = 50;   // Wait 5 sec
          //
          // Driver clears the PCIe2TBT Valid bit to support two consicutive mailbox commands
          //
          PciWrite32 (b, Pcie2Tbt(b), 0);
          while (MaxWaitIter-- > 0) {
            RegVal = PciRead32(b, Tbt2Pcie(b));
            if (MAX_UINT32 == RegVal) {
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
          if (!TbtSetPcie2TbtCommand(PCIE2TBT_USB_ON, b, TBT_5S_TIMEOUT)) {
            //
            // Nothing to wait, Host Router is not responsive
            //
            DEBUG((DEBUG_INFO, "<TbtPei> TbtBootSupport - Usb On message sent failed \n"));
          }
        }
      }

  DEBUG((DEBUG_INFO, "] DTbtUsbOn %r", Status));
  return Status;
}

/**
  Exectue Mail box command "Sx Exit".

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSxExitFlow(
  IN  PCI_IO_DEVICE *b
)
{
	EFI_STATUS                      Status;
	UINT32                          Count;
	
	DEBUG((DEBUG_INFO, "[ DTbtSxExitFlow\n"));
	
	Status = EFI_SUCCESS;
	Count = 0;
		
	if ( b->DTbtCommonConfig.TbtBootOn == 2) {
		//
		// WA: When system with TBT 3.1 device, resume SX system need to wait device ready. In document that maximum time out should be 500ms.
		//
		while (PciRead32(b, 0x08) == MAX_UINT32) { //End Device will be with Device Number 0x0, Function Number 0x0.
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
		TbtSetSxMode(b, b->DTbtCommonConfig.TbtBootOn);
	}
	
	DEBUG((DEBUG_INFO, "] DTbtSxExitFlow %r", Status));
	return Status;
}


/**
  Initialize Thunderbolt(TM)

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
TbtInit (
  IN  PCI_IO_DEVICE *b
  )
{
	//
	// Get the TBT Policy
	//
	//Status = PeiServicesLocatePpi (&gPeiTbtPolicyPpiGuid, 0, NULL, (VOID **) &PeiTbtConfig);
	//
	// Exectue Mail box command "Boot On"
	//
	Status = DTbtBootOn (b);
	//
	// Exectue Mail box command "Usb On"
	//
	Status = DTbtUsbOn (b);
	//
	//During boot, reboot and wake  (bits [1:0]) of PCH PM_CFG register should be
	//set to 11b - 10 ms (default value is 0b - 10 us)
	//
	//Status = DTbtSetTPch25Timing (b);
	//
	// Configure Tbt Force Power
	//
	//Status = DTbtForcePower (b);
	//
	// VGA Enable and VGA 16-bit decode registers of Bridge control register of Root port where
	// Host router resides should be cleaned
	//
	Status = DTbtClearVgaRegisters (b);
	//
	// Upon wake, if BIOS saved pre-Sx Host Router state as active (system went to sleep with
	// attached devices), BIOS should:
	// 1. Execute "Sx_Exit_TBT_Connected" mailbox command.
	// 2. If procedure above returns true, BIOS should perform "wait for fast link bring-up" loop
	// 3. Continue regular wake flow.
	//
	Status = DTbtSxExitFlow (b);
	return EFI_SUCCESS;
}


//========================================================================================
// Find all the Thunderbolt devices and init them

STATIC
VOID
InitAllThunderbolt (
	PCI_IO_DEVICE *HostBridge
)
{
	DEBUG ((DEBUG_INFO, "] InitAllThunderbolt\n"));

	EFI_STATUS Status;

	PCI_IO_DEVICE *b;

	for (b = HostBridge->Chain; b; b = b->Chain) {
		if (!b->DidThunderbolt) {
			b->DidThunderbolt = TRUE;
		
			if (b->PciExpressCap.Capability.Bits.DevicePortType == PCIE_DEVICE_PORT_TYPE_UPSTREAM_PORT) {
				UINT16 VendorSpecificID = 0;
				UINT32 VendorSpecificOffset;
				UINT32 NextOffset = 0;
				
				BOOLEAN MovedBuses;
				Status = VerifyBuses (b->Parent, &MovedBuses);
				if (EFI_ERROR(Status)) {
					continue;
				}

				while (VendorSpecificID != 0x1234) {
					VendorSpecificOffset = NextOffset;
					Status = LocatePciExpressCapabilityRegBlock (
						b,
						PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID,
						&VendorSpecificOffset,
						&NextOffset
					);
					if (!EFI_ERROR(Status)) {
						VendorSpecificID = PciRead16 (b, VendorSpecificOffset + 4);
					}
					else if (Status != EFI_NOT_FOUND && Status != EFI_UNSUPPORTED) {
						DEBUG((DEBUG_ERROR, "FixPCIeLinkRate: LocateCapabilityRegBlock Status %r.\n", Status));
						break;
					}
					else {
						Status = EFI_SUCCESS;
						break;
					}
				}
	
				if (VendorSpecificID == 0x1234) {
					DEBUG ((DEBUG_INFO, "[ Init found Thunderbolt device\n"));
					
					b->TbtType = DTBT_CONTROLLER;
					/*
					b->DTbtCommonConfig.TbtBootOn = ? ....
					b->DTbtControllerConfig
                    */

					Status = TbtInit (b);

					DEBUG ((DEBUG_INFO, "] Init found Thunderbolt device %r\n", Status));
				}

				if (MovedBuses) {
					UnmoveBuses (b);
				}

			} // if upstream port - check for Thunderbolt
		} // if !b->DidThunderbolt
	} // for bridge
	DEBUG ((DEBUG_INFO, "] InitAllThunderbolt\n"));
} // InitAllThunderbolt


//========================================================================================
