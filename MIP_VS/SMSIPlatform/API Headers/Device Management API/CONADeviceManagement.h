/*
==============================================================================
* Device Management API 3.2
*
Filename    : CONADeviceManagement.h
Description : Device Management API functions
Version     : 3.2

Copyright (c) 2005-2007 Nokia Corporation.
This software, including but not limited to documentation and any related 
computer programs ("Software"), is protected by intellectual property rights 
of Nokia Corporation and/or its licensors. All rights are reserved. By using 
the Software you agree to the terms and conditions hereunder. If you do not 
agree you must cease using the software immediately.
Reproducing, disclosing, modifying, translating, or distributing any or all 
of the Software requires the prior written consent of Nokia Corporation. 
Nokia Corporation retains the right to make changes to the Software at any 
time without notice.

A copyright license is hereby granted to use of the Software to make, publish, 
distribute, sub-license and/or sell new Software utilizing this Software. 
The Software may not constitute the primary value of any new software utilizing 
this software. No other license to any other intellectual property rights of 
Nokia or a third party is granted. 

THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS 
OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF NON-INFRINGEMENT, 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
NOKIA CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, 
OR CONSEQUENTIAL LOSS OR DAMAGES, INCLUDING BUT NOT LIMITED TO, LOST PROFITS 
OR REVENUE, LOSS OF USE, COST OF SUBSTITUTE PROGRAM, OR LOSS OF DATA OR EQUIPMENT 
ARISING OUT OF THE USE OR INABILITY TO USE THE MATERIAL, EVEN IF 
NOKIA CORPORATION HAS BEEN ADVISED OF THE LIKELIHOOD OF SUCH DAMAGES OCCURRING. 
==============================================================================
*/
#ifndef _CONADEVICEMANAGEMENT_H_
#define _CONADEVICEMANAGEMENT_H_

///////////////////////////////////////////////////////////
// PC Connectivity API
///////////////////////////////////////////////////////////
	#ifndef PCCS_DMAPI
		#define PCCS_DMAPI DWORD STDAPICALLTYPE
	#endif
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Device Management API definitions
///////////////////////////////////////////////////////////
	#include "DMAPIDefinitions.h"
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Device Management API
///////////////////////////////////////////////////////////

	//=========================================================
	// Device Management API versions 
	//
		#define DMAPI_VERSION_30		30
		#define DMAPI_VERSION_31		31
		#define DMAPI_VERSION_32		32
	//=========================================================

	//=========================================================
	// DMAPI_Initialize
	//
	// Description:
	//	DMAPI_Initialize initializes the API. This must be called once and before any other DMAPI call!
	//  It's not allowed to call this function like this 
	//		DMAPI_Initialize(DMAPI_GetAPIVersion(), NULL);
	//	You must call it like this
	//		DMAPI_Initialize(DMAPI_VERSION_32, NULL);
	//
	// Parameters:
	//	dwAPIVersion	[in] DMAPI version requested.
	//	pdwParam		[in] Reserved for future use. Must be NULL.
	//
	// Return values:
	//
		PCCS_DMAPI DMAPI_Initialize(DWORD dwAPIVersion, DWORD* pdwParam);
	//=========================================================

	//=========================================================
	// DMAPI_Terminate
	//
	// Description:
	//	DMAPI_Terminate terminates the API. This must be called once and as the last DMAPI call!
	//
	// Parameters:
	//	pdwParam		[in] Reserved for future use. Must be NULL.
	//
	// Return values:
	//
		PCCS_DMAPI DMAPI_Terminate(DWORD* pdwParam);
	//=========================================================

	//=========================================================
	// DMAPI_GetAPIVersion
	//
	// Description:
	//	Returns currently installed version of this DMAPI. 	
	//
	// Parameters:
	//
	// Return values:
	//	API version number. 
	//
		PCCS_DMAPI DMAPI_GetAPIVersion();
	//=========================================================

	//=========================================================
	// CONAOpenDM
	//
	// Description:
	//	CONAOpenDM opens the device management connection. 
	//	It returns a handle to the device management context.
	//	This must be used in all device management functions.
	//
	// Parameters:
	//	phDMHandle		[out] Device manager handle
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_NOT_INITIALIZED
	// ECONA_NOT_ENOUGH_MEMORY
	// ECONA_INIT_FAILED_COM_INTERFACE
	// ECONA_UNKNOWN_ERROR
	// 
		PCCS_DMAPI CONAOpenDM(DMHANDLE* phDMHandle);
	//=========================================================

	//=========================================================
	// CONACloseDM
	//
	// Description:
	//	CONACloseDM is used to close device management connection. 
	//	This must be called for each device management handle 
	//	when it is not needed anymore.
	//
	// Parameters:
	//	hDMHandle		[in] Device manager handle
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	// 
		PCCS_DMAPI CONACloseDM(DMHANDLE hDMHandle);
	//=========================================================

	//=========================================================
	// CONASetDeviceListOption
	//
	// Description:
	//	CONASetDeviceListOption is used to set different options for DMAPI.
	//	Currently only DMAPI_OPTION_SET_MANUFACTURER option is supported. It
	//	can be used to set manufacturer info for the DMAPI. This information 
	//	is used when filtering showed devices. If this option is not is not
	//	set, all connected devices are added to the device list.
	//	This function can be called multiple times with different manufacturer 
	//	names to support devices from multiple manufacturers.
	//	Once called with pstrValue set as NULL clears the filtering.
	//
	// Parameters:
	//	hDMHandle			[in] Device manager handle
	//	dwOptions			[in] Options. Currently only DMAPI_OPTION_SET_MANUFACTURER is supported.
	//	pstrValue			[in] String buffer containing the name of the manufacturer.
	//							 If NULL, filtering is disables.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	// 
		PCCS_DMAPI CONASetDeviceListOption(DMHANDLE hDMHandle, DWORD dwOptions, const WCHAR* pstrValue);
	//=========================================================

	//=========================================================
	// CONAGetDeviceCount
	//
	// Description:
	//	CONAGetDeviceCount puts the count of devices into the 
	//	given DWORD. 
	//
	// Parameters:
	//	hDMHandle		[in]  Device manager handle
	//	pdwCount		[out] Pointer to the variable receiving 
	//						  the count of devices.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_HANDLE
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	// 
		PCCS_DMAPI CONAGetDeviceCount(DMHANDLE hDMHandle, DWORD* pdwCount);
	//=========================================================

	//=========================================================
	// CONAGetDevices
	//
	// Description:
	//	CONAGetDevices allocates and sets devices information to 
	//	allocate the CONAPI_DEVICE structs and returns the count 
	//	of the used CONAPI_DEVICE structs.  The allocated memory 
	//	MUST be freed by calling the CONAFreeDeviceStructure function. 
	//	
	//	Every CONAPI_DEVICE struct includes CONAPI_CONNECTION_INFO 
	//	sturct(s), which include the media type information. 
	//	If the device is connected via different medias at the 
	//	same time, there is an individual CONAPI_CONNECTION_INFO for 
	//	each media type.

	//
	// Parameters:
	//  hDMHandle		[in]  Device manager handle
	//  pdwCount		[in,out] In: Number of allocated CONAPI_DEVICE structs.
	//							 Out: Number of used CONAPI_DEVICE structs.
	//  pDevices		[out] Pointer to receiving CONAPI_DEVICE structures.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_HANDLE
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONAGetDevices(
							DMHANDLE hDMHandle, 
							DWORD* pdwCount, 
							CONAPI_DEVICE* pDevices );
	//=========================================================

	//=========================================================
	// CONAGetDevice
	//
	// Description:
	//	CONAGetDevice function allocates and sets the device's 
	//	information to the allocated CONAPI_DEVICE struct.
	//
	// Parameters:
	//	hDMHandle			[in]  Device manager handle
	//  pstrSerialNumber	[in]  Device's serial number.
	//  pDevice				[out] Pointer to device struct
	// 
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_HANDLE
	// ECONA_DEVICE_NOT_FOUND
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONAGetDevice(
							DMHANDLE hDMHandle, 
							const WCHAR* pstrSerialNumber, 
							CONAPI_DEVICE* pDevice );
	//=========================================================

	//=========================================================
	// CONAFreeDeviceStructure
	//
	// Description:
	//	CONAFreeDeviceStructure releases the memory, which 
	//	ConnectivityAPI is allocated inside the CONAPI_DEVICE structs.
	//
	// Parameters:
	//	dwCount			[in] Number of the used CONAPI_DEVICE structs. 
	//		The same number value if CONAGetDevices function is used. 
	//		The value must be the number of the used structs (pdwCount). 
	//		If the CONAGetDevice function is used, the value must be 1.
	//	pDevices		[in] Pointer to the device's struct list.
	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_POINTER
	//	ECONA_INVALID_PARAMETER
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONAFreeDeviceStructure( DWORD dwCount, CONAPI_DEVICE* pDevices );
	//=========================================================

	//=========================================================
	// CONARefreshDeviceList
	//
	// Description:
	//	CONARefreshDeviceList function starts to refresh the 
	//	Device list. All changes will be notified via notifications.
	//
	// Parameters:
	//	hDMHandle		[in] Device manager handle
	//	dwValue			[in] Reserved for future use. Must be zero.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_HANDLE
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONARefreshDeviceList(DMHANDLE hDMHandle, DWORD dwValue );
	//=========================================================

	//=========================================================
	// CONARenameFriendlyName
	//
	// Description:
	//	Renames the device's friendly name. Current device's 
	//	friendly name string is in the CONAPI_DEVICE struct; 
	//	When the function returns a successful result, the 
	//	Connectivity API sends a DeviceList nofication with the 
	//	CONAPI_DEVICE_UPDATED value to all Connectivity API-users. 
	//
	// Parameters:
	//	hDMHandle			[in] Device manager handle
	//	pstrSerialNumber	[in] Serial number of the device.
	//	pstrNewFriendlyName [in] New Device Friendly Name .
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_HANDLE
	// ECONA_DEVICE_NOT_FOUND
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONARenameFriendlyName(
								DMHANDLE hDMHandle, 
								const WCHAR* pstrSerialNumber, 
								const WCHAR* pstrNewFriendlyName );
	//=========================================================

	//=========================================================
	// CONARegisterNotifyCallback
	//
	// Description:
	//	CONARegisterNotifyCallback is used to register or unregister 
	//	a callback function for the Connectivity API device list 
	//	notifications.
	//
	// Parameters:
	//	hDMHandle		[in] Device manager handle
	//	dwState			[in] Used to define the action:
	//						 API_REGISTER used in registeration
	//						 API_UNREGISTER.used in removing the registeration
	//	pfnNotify		[in] Function pointer of the call back method
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_HANDLE
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONARegisterNotifyCallback(
								DMHANDLE hDMHandle, 
								DWORD dwState, 
								PFN_CONA_DEVICE_CALLBACK pfnNotify);
	//=========================================================

	//=========================================================
	// CONARegisterDMNotifyIF
	//
	// Description:
	//	CONARegisterDMNotifyIF is used to register or unregister 
	//	a notification interface for the DMAPI API device list 
	//	notifications.
	//
	// Parameters:
	//	hDMHandle		[in] Device manager handle
	//	dwState			[in] Used to define the action:
	//						 API_REGISTER used in registeration
	//						 API_UNREGISTER.used in removing the registeration
	//	pINotify		[in] Pointer to IDMAPIDeviceNotify implementation
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_HANDLE
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	//
	#ifdef __cplusplus
		PCCS_DMAPI CONARegisterDMNotifyIF(
								DMHANDLE hDMHandle, 
								DWORD dwState, 
								IDMAPIDeviceNotify* pINotify );
	#endif
	//=========================================================

	//=========================================================
	// CONASearchDevices 
	//
	// Description:
	//	CCONASearchDevices functions search all devices from the target media. 
	//  The target media that can be used is Bluetooth at the moment.
	//
	//  CONASearchDevices allocates and sets devices information to 
	//  CONAPI_CONNECTION_INFO structs and returns pointer to structs.  
	//  Connectivity API-user MUST releases the returned pointer by calling the 
	//  CONAFreeConnectionInfoStructures function. 
	//
	//	Every CONAPI_CONNECTION_INFO struct includes the media and device information.
	//	The struct's dwState parameter defines the paired and trusted information from 
	//  device. It has the following values:
	//  Parameter value					Description					Macros for check the values 
	//																(If value is true, macro returns 1)
	//	CONAPI_DEVICE_UNPAIRED			Device in not paired.		CONAPI_IS_DEVICE_UNPAIRED(dwState)
	//	CONAPI_DEVICE_PAIRED			Device is paired.			CONAPI_IS_DEVICE_PAIRED(dwState)
	//	CONAPI_DEVICE_PCSUITE_TRUSTED	Device is PC Suite trusted.	CONAPI_IS_PCSUITE_TRUSTED(dwState)
	//	CONAPI_DEVICE_NOT_ACTIVE		Device is not active.		CONAPI_IS_DEVICE_NOT_ACTIVE(dwState)
	//
	//	Connectivity API can add more values afterwards so Connectivity API-user should 
	//	always use defined macros to check those values!
	//
	// Parameters:
	//	hDMHandle				[in] Existing device management handle.
	//	dwSearchOptions			[in] Search options values:
	//			CONAPI_MEDIA_BLUETOOTH: Get devices from bluetooth media. 
	//				This value must be used.
	//			CONAPI_ALLOW_TO_USE_CACHE: Get all devices from cache if available. 
	//				If cache is not available function fails with error: ECONA_CACHE_IS_NOT_AVAILABLE.
	//				This value is optional and can be used with other values.
	//			One of the next values can be used at the time:
	//			CONAPI_GET_ALL_PHONES: Get all phones from target media. Includes unpaired, 
	//				paired and PC Suite trusted phones. 
	//			CONAPI_GET_PAIRED_PHONES:Get all paired phones from target media. Includes 
	//				paired (and PC Suite trusted) phones. 
	//			CONAPI_GET_TRUSTED_PHONES:Get all PC Suite trusted phones from target media. 
	//				Includes all PC Suite trusted phones, which are paired.
	//	dwSearchTime			[in]  Maximum search time in seconds. Note: Bluetooth device
	//			discovery can takes several minutes if there are a lot of devices on range!
	//	pfnSearchNotify			[in]  Pointer to search notification callback function. Value
	//			can be NULL if notification is not needed.
	//	pdwNumberOfStructures	[out] Number of CONAPI_CONNECTION_INFO structures. 
	//	ppConnInfoStructures	[out] Pointer to CONAPI_CONNECTION_INFO structure(s). 
	//	hDMHandle				[in] Device manager handle
	//	pstrSerialNumber		[in] Serial number of the device.
	//	pstrNewFriendlyName		[in] New Device Friendly Name .
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_HANDLE
	// ECONA_DEVICE_NOT_FOUND
	// ECONA_FAILED_TIMEOUT
	// ECONA_MEDIA_IS_NOT_ACTIVE
	// ECONA_MEDIA_IS_NOT_WORKING
	// ECONA_CACHE_IS_NOT_AVAILABLE
	// ECONA_SUSPEND
	// ECONA_NOT_ENOUGH_MEMORY
	// ECONA_NOT_INITIALIZED
	// ECONA_NOT_SUPPORTED_PC
	// ECONA_CANCELLED
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONASearchDevices(
								DMHANDLE	hDMHandle,
								DWORD		dwSearchOptions,
								DWORD		dwSearchTime,
								PFN_CONA_SEARCH_CALLBACK	pfnSearchNotify,
								DWORD*		pdwNumberOfStructures,
								CONAPI_CONNECTION_INFO**	ppConnInfoStructures
								);
	//=========================================================

	//=========================================================
	// CONAFreeConnectionInfoStructures
	//
	// Description:
	//	CONAFreeDeviceStructure releases the CONAPI_CONNECTION_INFO structs, 
	//	which CONASearchDevices function is allocated.
	//
	// Parameters:
	//	dwNumberOfStructures	[in] Number of CONAPI_CONNECTION_INFO structures.
	//	ppConnInfoStructures	[in] Pointer to CONAPI_CONNECTION_INFO structure(s).
	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_POINTER
	//	ECONA_INVALID_PARAMETER
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONAFreeConnectionInfoStructures( 
												DWORD dwNumberOfStructures, 
												CONAPI_CONNECTION_INFO** ppConnInfoStructures );
	//=========================================================

	//=========================================================
	// CONAChangeDeviceTrustedState
	//
	// Description:
	//	CONAChangeDeviceTrustedState functions changes device's 
	//	trusted state. It has the following operation values:
	//	Value							Description
	//	CONAPI_PAIR_DEVICE				Pair device.
	//	CONAPI_UNPAIR_DEVICE			Unpair device from Bluetooth stack.
	//	CONAPI_SET_PCSUITE_TRUSTED		Set device to PC Suite trusted state.
	//		System recognise the device and sets it to trusted state.  
	//		After that device is available via Connectivity API. Note: Device should be 
	//		paired before of this. Value can be used with CONAPI_PAIR_DEVICE. 
	//	CONAPI_SET_PCSUITE_UNTRUSTED	Remove PC Suite trusted information from 
	//		System. After that device is not available via Connectivity API. 
	//		Note: Device can be paired or unpaired state. Value can be used 
	//		with CONAPI_UNPAIR_DEVICE.

	//
	// Parameters:
	//	hDMHandle			[in] Existing device management handle.
	//	dwTrustedOperation	[in] Operation values: 
	//								CONAPI_PAIR_DEVICE
	//								CONAPI_UNPAIR_DEVICE
	//								CONAPI_SET_PCSUITE_TRUSTED 
	//								CONAPI_SET_PCSUITE_UNTRUSTED
	//	pstrAddress			[in] Device address. If device is connected via Bluetooth 
	//				media, address must be Device BT address (see pstrAddress parameter 
	//				from CONAPI_CONNECTION_INFO Structure)
	//	pstrPassword		[in] Password string for pairing. String can include only 
	//				the numbers (0-9) characters.Value used only with CONAPI_PAIR_DEVICE 
	//				operation. With other operations value should be NULL.
	//	pstrUserName			[in] Reserved for future use, the value must be NULL.

	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_POINTER
	//	ECONA_INVALID_PARAMETER
	//	ECONA_INVALID_HANDLE
	//	ECONA_DEVICE_NOT_FOUND
	//	ECONA_NOT_SUPPORTED_DEVICE
	//	ECONA_CONNECTION_FAILED
	//	ECONA_CONNECTION_BUSY
	//	ECONA_CONNECTION_LOST
	//	ECONA_DEVICE_PAIRING_FAILED
	//	ECONA_DEVICE_PASSWORD_WRONG
	//	ECONA_DEVICE_PASSWORD_INVALID
	//	ECONA_FAILED_TIMEOUT
	//	ECONA_MEDIA_IS_NOT_ACTIVE
	//	ECONA_MEDIA_IS_NOT_WORKING
	//	ECONA_SUSPEND
	//	ECONA_NOT_ENOUGH_MEMORY
	//	ECONA_NOT_INITIALIZED
	//	ECONA_NOT_SUPPORTED_PC
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONAChangeDeviceTrustedState(
								DMHANDLE		hDMHandle,
								DWORD			dwTrustedOperation,
								const WCHAR*	pstrAddress,
								const WCHAR*	pstrPassword,
								const WCHAR*	pstrUserName
								);
	//=========================================================

	//=========================================================
	// CONAGetDeviceInfo
	//
	// Description:
	//	CONAGetDeviceInfo function sets the device specific information to the structure. 
	//  The structure type and allocation depends on which information is requested.
	//	The structure must be freed or clear by calling the CONAFreeDeviceInfoStructure function.
	//
	// Parameters:
	//	hDMHandle			[in]  Existing device management handle.
	//	pstrSerialNumber	[in]  Device's serial number.
	//	dwStructureType		[in]  Structure type value:
	//								CONAPI_DEVICE_GENERAL_INFO
	//								CONAPI_DEVICE_PRODUCT_INFO
	//								CONAPI_DEVICE_PROPERTIES_INFO
	//								CONAPI_DEVICE_ICON_INFO
	//	ppStructure			[out] Pointer to the information structure.

	// 
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_HANDLE
	// ECONA_DEVICE_NOT_FOUND
	// ECONA_NOT_INITIALIZED
	// ECONA_NOT_SUPPORTED_DEVICE
	// ECONA_NOT_SUPPORTED_MANUFACTURER
	// ECONA_NOT_ENOUGH_MEMORY
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONAGetDeviceInfo(
							DMHANDLE	 hDMHandle, 
							const WCHAR* pstrSerialNumber, 
							DWORD		 dwStructureType,
							LPVOID*		 ppStructure
							);
	//=========================================================

	//=========================================================
	// CONAFreeDeviceInfoStructure
	//
	// Description:
	//	CONAFreeDeviceInfoStructure releases the (e.g.  
	//  CONAPI_DEVICE_GEN_INFO) structure that CONAGetDeviceInfo function has allocated.
	//
	// Parameters:
	//	dwStructureType	[in] Structure type value:
	//							CONAPI_DEVICE_GENERAL_INFO
	//							CONAPI_DEVICE_PRODUCT_INFO
	//							CONAPI_DEVICE_PROPERTIES_INFO
	//							CONAPI_DEVICE_ICON_INFO
	//	pStructure		[in] Pointer to the structure that the CONAGetDeviceInfo function has allocated
	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_POINTER
	//	ECONA_INVALID_PARAMETER
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_DMAPI CONAFreeDeviceInfoStructure( DWORD	dwStructureType, LPVOID	pStructure );
	//=========================================================

#endif //_CONADEVICEMANAGEMENT_H_
