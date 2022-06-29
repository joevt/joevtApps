# joevtApps
This edk2 package is a collection of most of my EFI drivers and apps.

Some of the drivers use libraries from OpenCorePkg. I currently build the drivers with the edk2 environment setup by `joevt/CloverBootloader`. Note that  CloverBootloader require updates from `tianorcore/edk2` for these files (already included in my fork):
- `MdePkg/Include/IndustryStandard/Pci22.h`
- `MdePkg/Include/IndustryStandard/PciExpress21.h`
- `MdePkg/Include/IndustryStandard/PciExpress40.h`
- `MdePkg/Include/Library/BaseLib.h`
- `MdePkg/Include/Library/DebugLib.h`
- `MdePkg/Include/Uefi/UefiSpec.h`

Clover is built by executing `./buildme`. Select `1` from the menu to begin the build.

After Clover is built, use the following command to build `joevtApps`
`./ebuild.sh --xcode8 --buildtarget=RELEASE --platform=joevtApps/joevtApps.dsc 2>&1 | sed -E "/make: Nothing to be done for \`tbuild'./d"`

I suppose it should be possible to build this from OpenCore's edk2 environment.

## FakeUEFI2
This is the origin for the ForgeUefi feature of OpenCore. It allows some UEFI2 GPUs to work on EFI Macs such as the MacPro3,1.

## ReloadPCIRom
This is the origin for the ReloadOptionRoms feature of OpenCore. If a PCI option ROM didn't load (a protocol was missing or the EFI version was too low or some other reason), then ReloadPCIRom/ReloadOptionRoms can try to load the PCI option ROM again after OpenCore or RefindPlus or a similar app alters the EFI environment (such as by using FakeUEFI2/ForgeUefi or by loading missing drivers).

## FixPCIeLinkRate
Some machines, such as the MacPro3,1, may set the link rate of a PCIe gen 3 or later device to 2.5 GT/s instead of 5 GT/s. This driver will try to set the link rate to the maximum supported by both the device and it's parent root port or bridge downstream port.

## Fv2OnFvThunk
From `tianocore/edk2/EdkCompatibilityPkg/Compatibility`. Implements `EFI_FIRMWARE_VOLUME2_PROTOCOL` on top of `EFI_FIRMWARE_VOLUME_PROTOCOL`.

## FvSimpleFileSystemDxe
From `tianocore/edk2/MdeModulePkg/Universal`. Uses the `EFI_FIRMWARE_VOLUME2_PROTOCOL` to expose files in firmware volumes via the the `EFI_SIMPLE_FILESYSTEM_PROTOCOL` and `EFI_FILE_PROTOCOL`. This allows viewing the firmware as a file system in the EFI Shell to extract files or run firmware apps such as the Apple Startup Manager on the MacPro3,1.

## pciids
- `ImportPciIds.sh` is a bash shell script to convert the pci.ids database into a .h include file or a UCS-2 (little-endian) text file. You can include the database from the .h file into your source code, or you can have `pciidsFile.c` read the text file into memory.
- `pciidsFile.c` is used to access the pci.ids database to return info about a device vendor/product/class code.

## PciMisc
Contains PCI and Thunderbolt related code. It creates a tree structure containing info about all PCI devices. It can iterate over the tree and use a callback to perform a task for all the PCI devices. FixPCIeLinkRate uses this for setting the link rate. It can list all the PCI devices using the pci.ids database info like my pcitree.sh script can (To Do: make a pcitree EFI app). It can also be used to find Thunderbolt devices that are hidden, and then possibly unhide them. For example, to enable USB functionality of a GC-ALPINE RIDGE installed in my MacPro3,1.

## Thunderbolt
Miscellaneous code from `tianocore/edk2-platforms` that I haven't tried yet. One goal would be to initiate PCIe tunnelling for a Thunderbolt 3 or Thunderbolt 4 add-in card installed in a computer that doesn't have firmware support for Thunderbolt add-in cards.

## UgaOnGop
Standalone driver to implement UGA protocol on GOP protocol. Untested. It uses UgaPassThrough from OpenCore. This feature is implemented by OpenCore and RefindPlus. The goal with this driver is to have UGA on GOP setup before the Apple Startup Manager loads by loading the driver using the Driver#### mechanism.

## XhciDxe
Driver from `tianocore/edk2/MdeModulePkg/Bus/Pci` that implements USB 3.x support. Modified to support EFI Macs. Unfinished. The goal is to be able to boot from a USB 3.x card in old EFI Macs that don't have an XHCI driver. The current solution is to use the XHCI driver (and maybe a USB driver) from a MacPro6,1 but I would prefer a method that includes source code.
