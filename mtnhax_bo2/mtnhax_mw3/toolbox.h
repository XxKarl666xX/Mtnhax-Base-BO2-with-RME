#include "stdafx.h"
#include <stdio.h>
#ifndef _COMMON_TYPES_
#define _COMMON_TYPES_
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned __int64	u64;
typedef signed char			s8;
typedef signed short		s16;
typedef signed int			s32;
typedef signed __int64		s64;
#ifndef NULL
#define NULL	0
#endif // NULL
typedef unsigned __int64 QWORD;
#endif // _COMMON_TYPES_
#ifndef _KERNEL_H
#define _KERNEL_H
#define Module_XAM "xam.xex"
#define __isync()       __emit(0x4C00012C)
#define DBG_SERIAL_RCV		(DWORD volatile*)0x7FEA1010
#define DBG_SERIAL_XMIT		(DWORD volatile*)0x7FEA1014
#define DBG_SERIAL_STS		(DWORD volatile*)0x7FEA1018
#define DBG_SERIAL_CNTRL	(DWORD volatile*)0x7FEA101C
#define PWR_REAS_PWRBTN		0x11
#define PWR_REAS_EJECT		0x12
#define PWR_REAS_ALARM		0x15
#define PWR_REAS_REMOPWR	0x20
#define PWR_REAS_REMOX		0x22
#define PWR_REAS_WINBTN		0x24
#define PWR_REAS_RESET		0x30
#define PWR_REAS_WIRELESS	0x55
#define PWR_REAS_WIRED		0x5A
#define CONSTANT_OBJECT_STRING(s)   { strlen( s ) / sizeof( OCHAR ), (strlen( s ) / sizeof( OCHAR ))+1, s }
#define MAKE_STRING(s)   {(USHORT)(strlen(s)), (USHORT)((strlen(s))+1), (PCHAR)s}
#define EXPORTNUM(x)
#define STATUS_SUCCESS	0
#define NT_EXTRACT_ST(Status)			((((ULONG)(Status)) >> 30)& 0x3)
#define NT_SUCCESS(Status)              (((NTSTATUS)(Status)) >= 0)
#define NT_INFORMATION(Status)          (NT_EXTRACT_ST(Status) == 1)
#define NT_WARNING(Status)              (NT_EXTRACT_ST(Status) == 2)
#define NT_ERROR(Status)                (NT_EXTRACT_ST(Status) == 3)
#define NEG_ONE_AS_DWORD				((DWORD)-1)
#define STATUS_SUCCESS	0
#define FILE_SYNCHRONOUS_IO_NONALERT	0x20
#define OBJ_CASE_INSENSITIVE			0x40
#define IDLE_PROC	0
#define USER_PROC	1
#define SYSTEM_PROC 2

typedef long			NTSTATUS;
typedef ULONG			ACCESS_MASK;

typedef struct _STRING {
	USHORT Length;
	USHORT MaximumLength;
	PCHAR Buffer;
} STRING, *PSTRING;

typedef struct _CSTRING {
	USHORT Length;
	USHORT MaximumLength;
	CONST char *Buffer;
} CSTRING, *PCSTRING;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef STRING			OBJECT_STRING;
typedef CSTRING			COBJECT_STRING;
typedef PSTRING			POBJECT_STRING;
typedef PCSTRING		PCOBJECT_STRING;
typedef STRING			OEM_STRING;
typedef PSTRING			POEM_STRING;
typedef CHAR			OCHAR;
typedef CHAR*			POCHAR;
typedef PSTR			POSTR;
typedef PCSTR			PCOSTR;
typedef CHAR*			PSZ;
typedef CONST CHAR*		PCSZ;
typedef STRING			ANSI_STRING;
typedef PSTRING			PANSI_STRING;
typedef CSTRING			CANSI_STRING;
typedef PCSTRING		PCANSI_STRING;
#define ANSI_NULL		((CHAR)0)
typedef CONST UNICODE_STRING*	PCUNICODE_STRING;
#define UNICODE_NULL			((WCHAR)0)

#define OTEXT(quote) __OTEXT(quote)

typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID Pointer;
	} st;
	ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef VOID(NTAPI *PIO_APC_ROUTINE) (
	IN PVOID ApcContext,
	IN PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG Reserved
	);

typedef struct _OBJECT_ATTRIBUTES {
	HANDLE RootDirectory;
	POBJECT_STRING ObjectName;
	ULONG Attributes;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

#define InitializeObjectAttributes( p, name, attrib, root){		\
	(p)->RootDirectory = root;                            \
	(p)->Attributes = attrib;                             \
	(p)->ObjectName = name;                               \
}

typedef struct _FILE_NETWORK_OPEN_INFORMATION {
	LARGE_INTEGER  CreationTime;
	LARGE_INTEGER  LastAccessTime;
	LARGE_INTEGER  LastWriteTime;
	LARGE_INTEGER  ChangeTime;
	LARGE_INTEGER  AllocationSize;
	LARGE_INTEGER  EndOfFile;
	ULONG  FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

typedef struct _XBOX_HARDWARE_INFO {
	DWORD Flags;
	unsigned char NumberOfProcessors;
	unsigned char PCIBridgeRevisionID;
	unsigned char Reserved[6];
	unsigned short BldrMagic;
	unsigned short BldrFlags;
} XBOX_HARDWARE_INFO, *PXBOX_HARDWARE_INFO;

typedef struct _XEX_IMPORT_TABLE_ENT {
	DWORD ImportDestAddr;
	DWORD ImportStubAddr;
} XEX_IMPORT_TABLE_ENT, *PXEX_IMPORT_TABLE_ENT;

typedef struct _XEX_IMPORT_TABLE {
	DWORD TableSize;
	BYTE NextImportDigest[20];
	DWORD ModuleNumber;
	DWORD Version[2];
	BYTE Unused;
	BYTE ModuleIndex;
	WORD ImportCount;
	DWORD ImportStubAddr[1];
} XEX_IMPORT_TABLE, *PXEX_IMPORT_TABLE;

typedef struct _XEX_IMPORT_DESCRIPTOR {
	DWORD Size;
	DWORD NameTableSize;
	DWORD ModuleCount;
} XEX_IMPORT_DESCRIPTOR, *PXEX_IMPORT_DESCRIPTOR;

typedef struct _IMAGE_EXPORT_ADDRESS_TABLE {
	DWORD Magic[3];
	DWORD ModuleNumber[2];
	DWORD Version[3];
	DWORD ImageBaseAddress;
	DWORD Count;
	DWORD Base;
	DWORD ordOffset[1];
} IMAGE_EXPORT_ADDRESS_TABLE, *PIMAGE_EXPORT_ADDRESS_TABLE;

typedef struct _XEX_SECURITY_INFO {
	unsigned long Size;
	DWORD ImageSize;
	BYTE Signature[256];
	DWORD InfoSize;
	DWORD ImageFlags;
	DWORD LoadAddress;
	BYTE ImageHash[20];
	DWORD ImportTableCount;
	BYTE ImportDigest[20];
	BYTE MediaID[16];
	BYTE ImageKey[16];
	PIMAGE_EXPORT_ADDRESS_TABLE ExportTableAddress;
	BYTE HeaderHash[20];
	DWORD GameRegion;
	DWORD AllowedMediaTypes;
	DWORD PageDescriptorCount;
} XEX_SECURITY_INFO, *PXEX_SECURITY_INFO;

typedef struct _XEX_HEADER_STRING {
	ULONG Size;
	UCHAR Data[1];
} XEX_HEADER_STRING, *PXEX_HEADER_STRING;

typedef struct _IMAGE_XEX_HEADER {
	DWORD Magic;
	DWORD ModuleFlags;
	DWORD SizeOfHeaders;
	DWORD SizeOfDiscardableHeaders;
	PXEX_SECURITY_INFO SecurityInfo;
	DWORD HeaderDirectoryEntryCount;
} IMAGE_XEX_HEADER, *PIMAGE_XEX_HEADER;

typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InClosureOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID NtHeadersBase;
	PVOID ImageBase;
	DWORD SizeOfNtImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	DWORD Flags;
	DWORD SizeOfFullImage;
	PVOID EntryPoint;
	WORD LoadCount;
	WORD ModuleIndex;
	PVOID DllBaseOriginal;
	DWORD CheckSum;
	DWORD ModuleLoadFlags;
	DWORD TimeDateStamp;
	PVOID LoadedImports;
	PVOID XexHeaderBase;
	union {
		STRING LoadFileName;
		struct {
			PVOID ClosureRoot;
			PVOID TraversalParent;
		} asEntry;
	} inf;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef struct _HV_IMAGE_IMPORT_TABLE
{
	BYTE NextImportDigest[0x14];
	DWORD ModuleNumber;
	DWORD Version[2];
	BYTE Unused;
	BYTE ModuleIndex;
	WORD ImportCount;
} HV_IMAGE_IMPORT_TABLE, *PHV_IMAGE_IMPORT_TABLE;

typedef struct _XEX_IMPORT_TABLE_ORG
{
	DWORD TableSize;
	HV_IMAGE_IMPORT_TABLE ImportTable;
} XEX_IMPORT_TABLE_ORG, *PXEX_IMPORT_TABLE_ORG;

typedef struct _XEX_EXECUTION_ID {
	DWORD		 MediaID;
	DWORD		 Version;
	DWORD		 BaseVersion;
	union {
		struct {
			WORD PublisherID;
			WORD GameID;
		};
		DWORD	 TitleID;
	};
	BYTE		 Platform;
	BYTE		 ExecutableType;
	BYTE		 DiscNum;
	BYTE		 DiscsInSet;
	DWORD		 SaveGameID;
} XEX_EXECUTION_ID, *PXEX_EXECUTION_ID;

typedef struct _XBOX_KRNL_VERSION {
	USHORT Major;
	USHORT Minor;
	USHORT Build;
	USHORT Qfe;
} XBOX_KRNL_VERSION, *PXBOX_KRNL_VERSION;

typedef enum _POOL_TYPE {
	NonPagedPool = 0,
	PagedPool = 1,
	NonPagedPoolMustSucceed = 2,
	DontUseThisType = 3,
	NonPagedPoolCacheAligned = 4,
	PagedPoolCacheAligned = 5,
	NonPagedPoolCacheAlignedMustS = 6
} POOL_TYPE;

typedef enum {
	HalHaltRoutine = 0x0,
	HalRebootRoutine = 0x1,
	HalKdRebootRoutine = 0x2,
	HalFatalErrorRebootRoutine = 0x3,
	HalResetSMCRoutine = 0x4,
	HalPowerDownRoutine = 0x5,
	HalRebootQuiesceRoutine = 0x6,
	HalForceShutdownRoutine = 0x7,
	HalPowerCycleQuiesceRoutine = 0x8,
	HalMaximumRoutine = 0x9,
} FIRMWARE_REENTRY;

#ifdef __cplusplus
extern "C" {
#endif

	VOID XapiThreadStartup(
		IN		VOID(__cdecl *StartRoutine)(VOID *),
		IN		PVOID StartContext,
		IN		DWORD dwExitCode
	);

	NTSYSAPI
		EXPORTNUM(3)
		void
		NTAPI
		DbgPrint(
			const char* s,
			...
		);

	NTSYSAPI
		EXPORTNUM(9)
		PVOID
		NTAPI
		ExAllocatePool(
			IN  DWORD NumberOfBytes
		);

	NTSYSAPI
		EXPORTNUM(10)
		PVOID
		NTAPI
		ExAllocatePoolWithTag(
			IN  DWORD NumberOfBytes,
			IN  DWORD Tag
		);

	NTSYSAPI
		EXPORTNUM(11)
		PVOID
		NTAPI
		ExAllocatePoolTypeWithTag(
			IN  DWORD NumberOfBytes,
			IN  DWORD Tag,
			IN  POOL_TYPE PoolType
		);

	EXPORTNUM(12)
		extern PDWORD ExConsoleGameRegion;

	NTSYSAPI
		EXPORTNUM(13)
		DWORD
		NTAPI
		ExCreateThread(
			IN		PHANDLE pHandle,
			IN		DWORD dwStackSize,
			IN		LPDWORD lpThreadId,
			IN		PVOID apiThreadStartup,
			IN		LPTHREAD_START_ROUTINE lpStartAddress,
			IN		LPVOID lpParameter,
			IN		DWORD dwCreationFlagsMod
		);

	NTSYSAPI
		EXPORTNUM(15)
		VOID
		NTAPI
		ExFreePool(
			IN PVOID  pPool
		);

	NTSYSAPI
		EXPORTNUM(16)
		NTSTATUS
		NTAPI
		ExGetXConfigSetting(
			IN		WORD dwCategory,
			IN		WORD dwSetting,
			OUT		PVOID pBuffer,
			IN		WORD cbBuffer,
			OUT		PWORD szSetting
		);

	NTSYSAPI
		EXPORTNUM(24)
		NTSTATUS
		NTAPI
		ExSetXConfigSetting(
			IN		WORD dwCategory,
			IN		WORD dwSetting,
			IN		PVOID pBuffer,
			IN		WORD szSetting
		);

	NTSYSAPI
		EXPORTNUM(25)
		VOID
		NTAPI
		ExTerminateThread(
			IN DWORD  exitCode
		);

	NTSYSAPI
		EXPORTNUM(40)
		VOID
		NTAPI
		HalReturnToFirmware(
			IN		DWORD dwPowerDownMode
		);

	NTSYSAPI
		EXPORTNUM(77)
		VOID
		NTAPI
		KeAcquireSpinLockAtRaisedIrql(
			IN OUT	PDWORD spinVar
		);

	NTSYSAPI
		UCHAR
		EXPORTNUM(102)
		NTAPI
		KeGetCurrentProcessType(
			VOID
		);

	NTSYSAPI
		EXPORTNUM(132)
		NTSTATUS
		NTAPI
		KeQuerySystemTime(
			OUT		PFILETIME CurrentTime
		);

	NTSYSAPI
		EXPORTNUM(137)
		VOID
		NTAPI
		KeReleaseSpinLockFromRaisedIrql(
			IN OUT	PDWORD spinVar
		);

	NTSYSAPI
		EXPORTNUM(157)
		HRESULT
		NTAPI
		KeSetEvent(
			IN 		HANDLE  Event,
			IN		DWORD  Increment,
			IN		BOOL  Wait
		);

	NTSYSAPI
		EXPORTNUM(168)
		BYTE
		NTAPI
		KeStallExecutionProcessor(
			IN		DWORD period
		);

	NTSYSAPI
		EXPORTNUM(177)
		BYTE
		NTAPI
		KfAcquireSpinLock(
			IN OUT	PDWORD spinVar
		);

	NTSYSAPI
		EXPORTNUM(178)
		BYTE
		NTAPI
		KfRaiseIrql(
			IN		BYTE irql
		);

	NTSYSAPI
		EXPORTNUM(180)
		VOID
		NTAPI
		KfReleaseSpinLock(
			IN OUT	PDWORD spinVar,
			IN		BYTE oldIrql
		);

	NTSYSAPI
		EXPORTNUM(179)
		VOID
		NTAPI
		KfLowerIrql(
			IN		BYTE irql
		);

	NTSYSAPI
		EXPORTNUM(190)
		PVOID
		NTAPI
		MmGetPhysicalAddress(
			IN		PVOID Address
		);

	NTSYSAPI
		EXPORTNUM(224)
		NTSTATUS
		NTAPI
		NtOpenSymbolicLinkObject(
			OUT		PHANDLE LinkHandle,
			IN		POBJECT_ATTRIBUTES ObjectAttributes
		);

	NTSYSAPI
		EXPORTNUM(227)
		NTSTATUS
		NTAPI
		NtQueueApcThread(
			IN		HANDLE ThreadHandle,
			IN		PIO_APC_ROUTINE ApcRoutine,
			IN		PVOID ApcRoutineContext OPTIONAL,
			IN		PIO_STATUS_BLOCK ApcStatusBlock OPTIONAL,
			IN		DWORD ApcReserved OPTIONAL
		);

	NTSYSAPI
		EXPORTNUM(231)
		NTSTATUS
		NTAPI
		NtQueryFullAttributesFile(
			IN		POBJECT_ATTRIBUTES ObjectAttributes,
			OUT		PFILE_NETWORK_OPEN_INFORMATION Attributes
		);

	NTSYSAPI
		EXPORTNUM(236)
		NTSTATUS
		NTAPI
		NtQuerySymbolicLinkObject(
			IN		HANDLE LinkHandle,
			IN OUT	PSTRING LinkTarget,
			OUT		PULONG ReturnedLength OPTIONAL
		);

	NTSYSAPI
		EXPORTNUM(259)
		HRESULT
		NTAPI
		ObCreateSymbolicLink(
			IN		PSTRING SymbolicLinkName,
			IN		PSTRING DeviceName
		);

	NTSYSAPI
		EXPORTNUM(260)
		HRESULT
		NTAPI
		ObDeleteSymbolicLink(
			IN		PSTRING SymbolicLinkName
		);

	NTSYSAPI
		EXPORTNUM(261)
		VOID
		NTAPI
		ObDereferenceObject(
			IN		PVOID Object
		);

	NTSYSAPI
		EXPORTNUM(299)
		PVOID
		NTAPI
		RtlImageXexHeaderField(
			IN		PVOID XexHeaderBase,
			IN		DWORD ImageField
		);

	NTSYSAPI
		EXPORTNUM(300)
		VOID
		NTAPI
		RtlInitAnsiString(
			IN OUT	PANSI_STRING DestinationString,
			IN		PCSZ  SourceString
		);

	EXPORTNUM(342)
		extern PXBOX_HARDWARE_INFO XboxHardwareInfo;

	EXPORTNUM(344)
		extern PXBOX_KRNL_VERSION XboxKrnlVersion;

	NTSYSAPI
		EXPORTNUM(394)
		VOID
		NTAPI
		XeCryptRandom(
			IN OUT	BYTE * pb,
			IN		DWORD cb
		);

	NTSYSAPI
		EXPORTNUM(404)
		BOOL
		NTAPI
		XexCheckExecutablePrivilege(
			IN		DWORD priviledge
		);

	EXPORTNUM(403)
		extern PLDR_DATA_TABLE_ENTRY* XexExecutableModuleHandle;

	NTSYSAPI
		EXPORTNUM(405)
		NTSTATUS
		NTAPI
		XexGetModuleHandle(
			IN		PSZ moduleName,
			IN OUT	PHANDLE hand
		);

	NTSYSAPI
		EXPORTNUM(407)
		DWORD
		NTAPI
		XexGetProcedureAddress(
			IN		HANDLE hand,
			IN		DWORD dwOrdinal,
			IN		PVOID Address
		);

	NTSYSAPI
		EXPORTNUM(408)
		DWORD
		NTAPI
		XexLoadExecutable(
			IN		PCHAR xexName,
			IN OUT	PHANDLE handle,
			IN		DWORD typeInfo,
			IN		DWORD ver
		);

	NTSYSAPI
		EXPORTNUM(409)
		DWORD
		NTAPI
		XexLoadImage(
			IN		LPCSTR xexName,
			IN		DWORD typeInfo,
			IN		DWORD ver,
			IN OUT	PHANDLE modHandle
		);

	NTSYSAPI
		EXPORTNUM(410)
		NTSTATUS
		NTAPI
		XexLoadImageFromMemory(
			IN		PVOID pvXexBuffer,
			IN		DWORD dwSize,
			IN		LPCSTR szXexName,
			IN		DWORD dwModuleTypeFlags,
			IN		DWORD dwMinimumVersion,
			IN OUT	PHANDLE pHandle
		);

	NTSYSAPI
		EXPORTNUM(412)
		PVOID
		NTAPI
		XexPcToFileHeader(
			IN		PVOID address,
			OUT		PLDR_DATA_TABLE_ENTRY* ldatOut
		);

	NTSYSAPI
		EXPORTNUM(486)
		NTSTATUS
		NTAPI
		XInputdReadState(
			IN		PDWORD DeviceContext,
			OUT		PDWORD pdwPacketNumber,
			OUT		PXINPUT_GAMEPAD pInputData
		);

	NTSYSAPI
		EXPORTNUM(579)
		NTSTATUS
		NTAPI
		XeKeysGenerateRandomKey(
			IN		WORD KeyID,
			OUT		PBYTE pbKey
		);

	NTSYSAPI
		EXPORTNUM(585)
		BOOL
		NTAPI
		XeKeysHmacSha(
			IN		DWORD keySel,
			IN		CONST PBYTE pbInp1,
			IN		DWORD cbInp1,
			IN		CONST PBYTE pbInp2,
			IN		DWORD cbInp2,
			IN		CONST PBYTE pbInp3,
			IN		DWORD cbInp3,
			OUT	    PBYTE pbOut,
			IN		DWORD cbOut
		);

	NTSYSAPI
		EXPORTNUM(607)
		DWORD
		NTAPI
		XeKeysExecute(
			IN		PVOID pbBuffer,
			IN		DWORD cbBuffer,
			IN OUT	PVOID Arg1,
			IN OUT	PVOID Arg2,
			IN OUT	PVOID Arg3,
			IN OUT	PVOID Arg4
		);

	NTSYSAPI
		EXPORTNUM(611)
		VOID
		NTAPI
		AniTerminateAnimation(
			VOID
		);

	NTSYSAPI
		EXPORTNUM(620)
		VOID
		NTAPI
		HalGetPowerUpCause(
			PBYTE reply
		);

#ifdef __cplusplus
}
#endif
#endif	//_KERNEL_DEFINES_H
VOID PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked);
VOID HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination);
DWORD PatchModuleImport(PCHAR Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
DWORD ResolveFunction(PCHAR ModuleName, DWORD Ordinal);
VOID XNotifyUI(PWCHAR pwszStringParam);
VOID Thread(LPTHREAD_START_ROUTINE lpStartAddress);