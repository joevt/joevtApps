#include <Library/UefiLib.h>
#include <Library/ShellLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HandleParsingLib.h>
#include <Library/MemoryAllocationLib.h>
//#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
//#include <Library/UefiRuntimeServicesTableLib.h>

#include <Protocol/PciIo.h>

#include <IndustryStandard/Pci22.h>


typedef EFI_STATUS(* DoOneDeviceFunctType) (IN CONST VOID* State, IN EFI_HANDLE DeviceHandle, IN UINTN Depth);


EFI_STATUS
DoDevTreeForHandle(
	IN CONST DoOneDeviceFunctType Callback,
	IN CONST VOID*      State,
	IN CONST EFI_HANDLE TheHandle,
	IN CONST UINTN      Depth
	)
{
	/*
	Taken from edk2/ShellPkg/Library/UefiShellDriver1CommandsLib/DevTree.c DoDevTreeForHandle
	*/
	EFI_STATUS          Status;

	EFI_HANDLE          *ChildHandleBuffer;
	UINTN               ChildCount;
	UINTN               LoopVar;

	Status              = EFI_SUCCESS;
	ChildHandleBuffer   = NULL;
	ChildCount          = 0;

	if (ShellGetExecutionBreakFlag()) {
		return SHELL_ABORTED;
	}

#if 0
	//
	// We want controller handles. they will not have LoadedImage or DriverBinding (or others...)
	//
	Status = gBS->OpenProtocol (
				TheHandle,
				&gEfiDriverBindingProtocolGuid,
				NULL,
				NULL,
				NULL,
				EFI_OPEN_PROTOCOL_TEST_PROTOCOL
			);
	if (!EFI_ERROR (Status)) {
		return EFI_SUCCESS;
	}

	Status = gBS->OpenProtocol (
				TheHandle,
				&gEfiLoadedImageProtocolGuid,
				NULL,
				NULL,
				NULL,
				EFI_OPEN_PROTOCOL_TEST_PROTOCOL
			);
	if (!EFI_ERROR (Status)) {
		return EFI_SUCCESS;
	}
#endif

	Status = Callback(State, TheHandle, Depth);

	//
	// recurse on each child handle with IndentCharCount + 2
	//
	ParseHandleDatabaseForChildControllers(TheHandle, &ChildCount, &ChildHandleBuffer);
	for (LoopVar = 0 ; LoopVar < ChildCount && Status == EFI_SUCCESS; LoopVar++){
		Status = DoDevTreeForHandle(Callback, State, ChildHandleBuffer[LoopVar], Depth+1);
	}

	if (ChildHandleBuffer != NULL) {
		FreePool(ChildHandleBuffer);
	}

	return (Status);
}


EFI_STATUS
LoopDevices (
	IN CONST DoOneDeviceFunctType Callback,
	IN CONST VOID* State
)
{
	/*
	Taken from edk2/ShellPkg/Library/UefiShellDriver1CommandsLib/DevTree.c : ShellCommandRunDevTree
	*/
	EFI_STATUS          Status;
	UINTN               LoopVar;
	EFI_HANDLE          TheHandle;
	UINTN               ParentControllerHandleCount;

	Status = EFI_SUCCESS;

	for (LoopVar = 1 ; ; LoopVar++){
		TheHandle = ConvertHandleIndexToHandle(LoopVar);
		if (TheHandle == NULL){
			break;
		}

		//
		// Skip handles that do not have device path protocol
		//
		Status = gBS->OpenProtocol (
				TheHandle,
				&gEfiDevicePathProtocolGuid,
				NULL,
				NULL,
				NULL,
				EFI_OPEN_PROTOCOL_TEST_PROTOCOL
			);
		if (EFI_ERROR (Status)) {
			continue;
		}

		//
		// Skip handles that do have parents
		//
		Status = PARSE_HANDLE_DATABASE_PARENTS (
				TheHandle,
				&ParentControllerHandleCount,
				NULL
			);
		if (ParentControllerHandleCount > 0) {
			continue;
		}

		//
		// Start a devtree from TheHandle that has a device path and no parents
		//
		Status = DoDevTreeForHandle(Callback, State, TheHandle, 0);
	}
	return Status;
}


STATIC
VOID
PrintMem (
		CONST VOID *Buffer,
		UINTN Count
	)
{
	CONST UINT8 *Bytes;
	UINTN       Idx;

	Bytes = Buffer;
	for (Idx = 0; Idx < Count; Idx++) {
		ShellPrintEx(-1, -1, L"%02x", Bytes[Idx]);
	}
}


typedef struct {
	EFI_DEVICE_PATH_PROTOCOL Header;
	UINT8 SasAddress[4]; // reduced from 8 to 4 bytes
	UINT8 Lun[4]; // reduced from 8 to 4 bytes
	UINT16 DeviceTopology;
	UINT16 RelativeTargetPort;
} ANVME_DEVICE_PATH;


#define psize(x) sizeof(((EFI_DEV_PATH *)0)->x)

typedef struct {
	UINT8 Type;
	UINT8 SubType;
	UINTN SizeMin;
	UINTN SizeMax; // 0 = same as min, 1 = infinite
	UINTN SizeInc;
} DEVICE_NODE_TO_SIZE;

const DEVICE_NODE_TO_SIZE NodeSize[] = {
	{HARDWARE_DEVICE_PATH  , HW_PCI_DP                        , psize(Pci)                       , 0, 0 },
	{HARDWARE_DEVICE_PATH  , HW_PCCARD_DP                     , psize(PcCard)                    , 0, 0 },
	{HARDWARE_DEVICE_PATH  , HW_MEMMAP_DP                     , psize(MemMap)                    , 0, 0 },
	{HARDWARE_DEVICE_PATH  , HW_VENDOR_DP                     , psize(Vendor)                    , 1, 1 },
	{HARDWARE_DEVICE_PATH  , HW_CONTROLLER_DP                 , psize(Controller)                , 0, 0 },
	{HARDWARE_DEVICE_PATH  , HW_BMC_DP                        , psize(Bmc)                       , 0, 0 },
	{ACPI_DEVICE_PATH      , ACPI_DP                          , psize(Acpi)                      , 0, 0 },
	{ACPI_DEVICE_PATH      , ACPI_EXTENDED_DP                 , psize(ExtendedAcpi)              , 0, 0 },
	{ACPI_DEVICE_PATH      , ACPI_ADR_DP                      , psize(AcpiAdr)                   , 1, psize(AcpiAdr.ADR) },
	{MESSAGING_DEVICE_PATH , MSG_ATAPI_DP                     , psize(Atapi)                     , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_SCSI_DP                      , psize(Scsi)                      , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_FIBRECHANNEL_DP              , psize(FibreChannel)              , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_FIBRECHANNELEX_DP            , psize(FibreChannelEx)            , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_SASEX_DP                     , sizeof(ANVME_DEVICE_PATH), psize(SasEx), psize(SasEx) - sizeof(ANVME_DEVICE_PATH) }, // Patched to support both sizes
	{MESSAGING_DEVICE_PATH , MSG_NVME_NAMESPACE_DP            , psize(NvmeNamespace)             , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_UFS_DP                       , psize(Ufs)                       , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_SD_DP                        , psize(Sd)                        , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_EMMC_DP                      , psize(Emmc)                      , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_1394_DP                      , psize(F1394)                     , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_USB_DP                       , psize(Usb)                       , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_USB_WWID_DP                  , psize(UsbWwid) + sizeof(CHAR16)  , 1, sizeof(CHAR16) }, // bug exists with zero length string, so add CHAR(16) for minimum
	{MESSAGING_DEVICE_PATH , MSG_DEVICE_LOGICAL_UNIT_DP       , psize(LogicUnit)                 , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_USB_CLASS_DP                 , psize(UsbClass)                  , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_SATA_DP                      , psize(Sata)                      , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_I2O_DP                       , psize(I2O)                       , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_MAC_ADDR_DP                  , psize(MacAddr)                   , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_IPv4_DP                      , psize(Ipv4) - psize(Ipv4.GatewayIpAddress) - psize(Ipv4.SubnetMask  ), psize(Ipv4), psize(Ipv4.GatewayIpAddress) + psize(Ipv4.SubnetMask  ) },
	{MESSAGING_DEVICE_PATH , MSG_IPv6_DP                      , psize(Ipv6) - psize(Ipv6.GatewayIpAddress) - psize(Ipv6.PrefixLength), psize(Ipv6), psize(Ipv6.GatewayIpAddress) + psize(Ipv6.PrefixLength) },
	{MESSAGING_DEVICE_PATH , MSG_INFINIBAND_DP                , psize(InfiniBand)                , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_UART_DP                      , psize(Uart)                      , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_VENDOR_DP                    , psize(Vendor)                    , 1, 1 },
	{MESSAGING_DEVICE_PATH , MSG_ISCSI_DP                     , psize(Iscsi)                     , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_VLAN_DP                      , psize(Vlan)                      , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_DNS_DP                       , psize(Dns)                       , 1, psize(Dns.DnsServerIp[0]) },
	{MESSAGING_DEVICE_PATH , MSG_URI_DP                       , psize(Uri)                       , 1, psize(Uri.Uri[0]) },
	{MESSAGING_DEVICE_PATH , MSG_BLUETOOTH_DP                 , psize(Bluetooth)                 , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_WIFI_DP                      , psize(WiFi)                      , 0, 0 },
	{MESSAGING_DEVICE_PATH , MSG_BLUETOOTH_LE_DP              , sizeof(BLUETOOTH_LE_DEVICE_PATH) , 0, 0 },
	{MEDIA_DEVICE_PATH     , MEDIA_HARDDRIVE_DP               , psize(HardDrive)                 , 0, 0 },
	{MEDIA_DEVICE_PATH     , MEDIA_CDROM_DP                   , psize(CD)                        , 0, 0 },
	{MEDIA_DEVICE_PATH     , MEDIA_VENDOR_DP                  , psize(Vendor)                    , 1, 1 },
	{MEDIA_DEVICE_PATH     , MEDIA_PROTOCOL_DP                , psize(MediaProtocol)             , 0, 0 },
	{MEDIA_DEVICE_PATH     , MEDIA_FILEPATH_DP                , psize(FilePath)                  , 1, psize(FilePath.PathName[0]) }, // should check for terminating null
	{MEDIA_DEVICE_PATH     , MEDIA_PIWG_FW_VOL_DP             , psize(FirmwareVolume)            , 0, 0 },
	{MEDIA_DEVICE_PATH     , MEDIA_PIWG_FW_FILE_DP            , psize(FirmwareFile)              , 0, 0 },
	{MEDIA_DEVICE_PATH     , MEDIA_RELATIVE_OFFSET_RANGE_DP   , psize(Offset)                    , 0, 0 },
	{MEDIA_DEVICE_PATH     , MEDIA_RAM_DISK_DP                , psize(RamDisk)                   , 0, 0 },
	{BBS_DEVICE_PATH       , BBS_BBS_DP                       , psize(Bbs)                       , 0, 0 },
	{END_DEVICE_PATH_TYPE  , END_INSTANCE_DEVICE_PATH_SUBTYPE , psize(DevPath)                   , 0, 0 },
	{END_DEVICE_PATH_TYPE  , END_ENTIRE_DEVICE_PATH_SUBTYPE   , psize(DevPath)                   , 0, 0 },
	{0                     , 0                                , psize(DevPath)                   , 1, 1 },
};

EFI_STATUS
VerifyDevicePathNodeSizes(CONST VOID *DevicePath) {
	EFI_DEVICE_PATH_PROTOCOL *Node;
	UINTN Index;
	BOOLEAN found;
	
	if (DevicePath == NULL) {
		return EFI_INVALID_PARAMETER;
	}
	
	EFI_STATUS result = EFI_SUCCESS;

	//
	// Process each device path node
	//
	Node = (EFI_DEVICE_PATH_PROTOCOL *) DevicePath;
	while (1) {
		//
		// Find the handler to dump this device path node
		// If not found, use a generic function
		//

		found = FALSE;
		for (Index = 0; NodeSize[Index].Type != 0; Index += 1) {
			if (
				DevicePathType (Node) == NodeSize[Index].Type &&
				DevicePathSubType (Node) == NodeSize[Index].SubType
			) {
				found = TRUE;
				break;
			}
		}
		if (!found) {
			ShellPrintEx(-1, -1, L"# Node at offset %ld has unknown type 0x%02x or sub type 0x%02x.\n", (void*)Node-(void*)DevicePath, DevicePathType (Node), DevicePathSubType (Node));
		}
		if (
			(DevicePathNodeLength (Node) < NodeSize[Index].SizeMin)
			||
			(NodeSize[Index].SizeMax == 0 && DevicePathNodeLength (Node) > NodeSize[Index].SizeMin)
		) {
			ShellPrintEx(-1, -1, L"# Node at offset %ld has length %llu which is not as expected %d.\n", (void*)Node-(void*)DevicePath, DevicePathNodeLength (Node), NodeSize[Index].SizeMin);
			result = EFI_INVALID_PARAMETER;
		}
		else if (NodeSize[Index].SizeMax > 3 && DevicePathNodeLength (Node) > NodeSize[Index].SizeMax) {
			ShellPrintEx(-1, -1, L"# Node at offset %ld has length %llu which is greater than expected %d.\n", (void*)Node-(void*)DevicePath, DevicePathNodeLength (Node), NodeSize[Index].SizeMax);
			result = EFI_INVALID_PARAMETER;
		}
		else if (NodeSize[Index].SizeInc > 1 && ((DevicePathNodeLength (Node) - NodeSize[Index].SizeMin) % NodeSize[Index].SizeInc) != 0 ) {
			ShellPrintEx(-1, -1, L"# Node at offset %ld has length %llu which is not equal to %d + n * %d.\n", (void*)Node-(void*)DevicePath, DevicePathNodeLength (Node), NodeSize[Index].SizeMin, NodeSize[Index].SizeInc);
			result = EFI_INVALID_PARAMETER;
		}
		
		if (IsDevicePathEnd (Node)) {
			break;
		}
		Node = NextDevicePathNode (Node);
	}
	
	return result;
}


STATIC
EFI_STATUS
PrintBytePath (
		CONST EFI_DEVICE_PATH_PROTOCOL *DevicePath
	)
{
	if (!IsDevicePathValid(DevicePath, 4096))
	{
		ShellPrintEx(-1, -1, L"# Invalid device path.\n");
		return EFI_INVALID_PARAMETER;
	}
	
	UINTN bytepathlen = 0;
	for (CONST EFI_DEVICE_PATH_PROTOCOL *node = DevicePath;;)
	{
		UINTN nodelen = node->Length[0] | node->Length[1] << 8;
		bytepathlen += nodelen;
		if (node->Type == END_DEVICE_PATH_TYPE && node->SubType == END_ENTIRE_DEVICE_PATH_SUBTYPE) break;
		node = (EFI_DEVICE_PATH_PROTOCOL *)((UINT8*)node + nodelen);
	}
	PrintMem (DevicePath, bytepathlen);
	ShellPrintEx(-1, -1, L"\n");
	
	return EFI_SUCCESS;
}


EFI_STATUS
DumpOneDevice (
	IN CONST VOID* State,
	IN EFI_HANDLE  DeviceHandle,
	IN UINTN       Depth
)
{
	ShellPrintEx(-1, -1, L"Handle:%x\n", ConvertHandleToHandleIndex(DeviceHandle));

	if (gEfiShellProtocol) {
		CHAR16 *ComponentName = NULL;
		gEfiShellProtocol->GetDeviceName(DeviceHandle, EFI_DEVICE_NAME_USE_COMPONENT_NAME, NULL, &ComponentName);
		if (ComponentName) {
			ShellPrintEx(-1, -1, L"Component:%s\n", ComponentName);
			FreePool(ComponentName);
		}
	}

	EFI_STATUS Status;
	EFI_PCI_IO_PROTOCOL *PciIo;
	PCI_TYPE00 Pci;
	UINTN Segment;
	UINTN Bus;
	UINTN Device;
	UINTN Function;
	EFI_DEVICE_PATH_PROTOCOL *DevicePath = NULL;

	Status = gBS->HandleProtocol(DeviceHandle, &gEfiPciIoProtocolGuid, (VOID **)&PciIo);
	if (!EFI_ERROR (Status)) {
		// Read PCI BUS
		PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
		Status = PciIo->Pci.Read (
							PciIo,
							EfiPciIoWidthUint32,
							0,
							sizeof (Pci) / sizeof (UINT32),
							&Pci
							);
		ShellPrintEx(-1, -1, L"PCI:%02x:%02x.%02x %04x:%04x\n", Bus, Device, Function, Pci.Hdr.VendorId, Pci.Hdr.DeviceId);
	}

//	if (!ComponentName || StrCmp(ComponentName, L"Generic Usb Keyboard")) {
		Status = gBS->OpenProtocol(
			DeviceHandle,
			&gEfiDevicePathProtocolGuid,
			(VOID**)&DevicePath,
			gImageHandle,
			NULL,
			EFI_OPEN_PROTOCOL_GET_PROTOCOL);
	
		if (!EFI_ERROR(Status) && DevicePath != NULL) {
			ShellPrintEx(-1, -1, L"Bytes:");
			Status = PrintBytePath(DevicePath);
		}

		if (!EFI_ERROR(Status)) {
			Status = VerifyDevicePathNodeSizes(DevicePath);
		}

		if (!EFI_ERROR(Status)) {
			//
			// use device path to text on the device path
			//
		
			// Note: devtree uses DisplayOnly=TRUE which can produce ambiguous paths that are not reversible.
			// For example: it may output "Ata(0x0)" instead of "Ata(Primary,Master,0x0)" or "Ata(Primary,Slave,0x0)"
			CHAR16 *Name1 = NULL;
			CHAR16 *Name2 = NULL;
			CHAR16 *Name3 = NULL;
			CHAR16 *Name4 = NULL;

			Name1 = ConvertDevicePathToText(DevicePath, FALSE, TRUE);
			Name2 = ConvertDevicePathToText(DevicePath, TRUE, TRUE);
			Name3 = ConvertDevicePathToText(DevicePath, FALSE, FALSE);
			Name4 = ConvertDevicePathToText(DevicePath, TRUE, TRUE);

			if (Name1 && Name1[0]) ShellPrintEx(-1, -1, L"Path:%s\n", Name1);
			if (Name2 && Name2[0] && StrCmp(Name2, Name1?Name1:L" ")) ShellPrintEx(-1, -1, L"Displayonly:%s\n", Name2);
			if (Name3 && Name3[0] && StrCmp(Name3, Name1?Name1:L" ") && StrCmp(Name3, Name2?Name2:L" ")) ShellPrintEx(-1, -1, L"NoShortcut:%s\n", Name3);
			if (Name4 && Name4[0] && StrCmp(Name4, Name1?Name1:L" ") && StrCmp(Name4, Name2?Name2:L" ") && StrCmp(Name4, Name3?Name3:L" ")) ShellPrintEx(-1, -1, L"NoShortcutDisplayonly:%s\n", Name4);

			if (Name1) FreePool(Name1);
			if (Name2) FreePool(Name2);
			if (Name3) FreePool(Name3);
			if (Name4) FreePool(Name4);
		}
//	}

	ShellPrintEx(-1, -1, L"\n");
	
	return EFI_SUCCESS;
} // GetOneDevice



EFI_STATUS
DumpAllPaths ()
{
	EFI_STATUS status = LoopDevices(&DumpOneDevice, NULL);
	return status;
}


EFI_STATUS
PrintDevicePathUtilToText(void* bytepath, UINTN bytepathlen)
{
	if (!IsDevicePathValid(bytepath, bytepathlen))
	{
		ShellPrintEx(-1, -1, L"# Invalid device path.\n");
		return EFI_INVALID_PARAMETER;
	}

	//EFI_STATUS Status = VerifyDevicePathNodeSizes(bytepath);
	CHAR16* textpath = ConvertDevicePathToText(bytepath, FALSE, TRUE);

	ShellPrintEx(
		-1,
		-1,
		L"%s",
		textpath?textpath:L"Unknown");

	if (textpath != NULL) {
		FreePool(textpath);
	}
	ShellPrintEx(-1, -1, L"\n");

	return EFI_SUCCESS;
}


EFI_STATUS
OutputDevicePathUtilFromText(CONST CHAR16* textpath)
{
	EFI_STATUS result = EFI_SUCCESS;

	EFI_DEVICE_PATH_PROTOCOL *bytepath = ConvertTextToDevicePath(textpath);
	if (bytepath)
	{
		PrintBytePath(bytepath);
		FreePool(bytepath);
	}
	else
	{
		result = EFI_INVALID_PARAMETER;
	}

	return result;
}

#define IS_DIGIT(x) (((CHAR16)(x) >= '0') && ((CHAR16)(x) <= '9'))

#define IS_HEX(x) ((IS_DIGIT(x)) || (((CHAR16)(x) >= 'a') && ((CHAR16)(x) <= 'f')) || \
	    (((CHAR16)(x) >= 'A') && ((CHAR16)(x) <= 'F')))

UINTN hexchar(CHAR16 c)
{
	return (c - (c < 'A' ? '0' : ((c < 'a' ? 'A' : 'a') - 10)));
}

UINTN hextobyte(CONST CHAR16* c)
{
	return (hexchar(*c) << 4) + hexchar(c[1]);
}

UINTN allhex(CONST CHAR16 *arg, UINTN len)
{
	for (UINTN i = 0; i < len; i++)
		if (!IS_HEX(arg[i]))
			return 0;
	return 1;
}

EFI_STATUS
parse_generic_option(CONST CHAR16 *arg)
{
	UINTN arglen = StrLen(arg);
	
	if (!arg || !arglen)
	{
		return EFI_INVALID_PARAMETER;
	}

	if ((arglen & 1) == 0 && allhex(arg, arglen))
	{
		int bytepathlenmax = arglen / 2;
		int bytepathlen = 0;
		UINT8* bytepath = AllocatePool(bytepathlenmax);
		if (!bytepath) return 1;
	
		// convert hex "xxd -p" path
		for (UINTN i = 0; i < arglen; i += 2)
		{
			if (bytepathlen >= bytepathlenmax)
			{
				bytepath = ReallocatePool(bytepathlenmax, bytepathlenmax + 1000, bytepath);
				if (!bytepath)
					return EFI_BAD_BUFFER_SIZE;
				bytepathlenmax += 1000;
			}
			bytepath[bytepathlen++] = hextobyte(arg + i);
		}
		EFI_STATUS r = PrintDevicePathUtilToText(bytepath, bytepathlen);
		FreePool(bytepath);
		return r;
	}

	return OutputDevicePathUtilFromText(arg);
}


SHELL_PARAM_ITEM gfxutilParamList[] = {
	{ L"input", TypePosition },
	{ NULL, TypeMax },
};

EFI_STATUS
gfxutilMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	static CHAR16 const Usage[] = L"%HUsage: gfxutil [-b] [path]\n  -b: paginate%N\n";
	LIST_ENTRY* Package;
	EFI_STATUS Status;
	
//	EfiGetSystemConfigurationTable (&gEfiDxeServicesTableGuid, (VOID **) &gDS);

	Status = ShellCommandLineParseEx(gfxutilParamList, &Package, NULL, TRUE, FALSE);
	if (EFI_ERROR(Status)) {
		ShellPrintEx(-1, -1, &Usage[0]);
		return EFI_INVALID_PARAMETER;
	}
	//ShellPrintEx(-1, -1, L"ShellCommandLineGetCount %d\n", ShellCommandLineGetCount(Package) );
	
	if (ShellCommandLineGetCount(Package) == 1) {
		Status = DumpAllPaths();
	}
	else if (ShellCommandLineGetCount(Package) == 2) {
		Status = parse_generic_option(ShellCommandLineGetRawValue(Package, 1));
	}
	else {
		ShellPrintEx(-1, -1, &Usage[0]);
		Status = EFI_INVALID_PARAMETER;
	}
	ShellCommandLineFreeVarList(Package);

	return Status;
}
