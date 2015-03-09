/*
==============================================================================
* Device Management API 3.2
*
Filename    : DMAPIDefinitions.h
Description : Device Management API definitions 
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
#ifndef _DMAPIDEFINITIONS_H__
#define _DMAPIDEFINITIONS_H__

//=========================================================
// Common data type definitions used in Connectivity API
//
	#include "PCCSTypeDefinitions.h"
//=========================================================

//=========================================================
// Device Management API definitions

	// ----------------------------------------------------
	// Device Management handle
	typedef APIHANDLE		DMHANDLE;
	// ----------------------------------------------------

	// ----------------------------------------------------
	// Connection info structure
	typedef struct
	{
		DWORD		dwDeviceID;
		DWORD		dwMedia;		// See definitions for media types in PCCSTypeDefinitions.h 
		WCHAR*		pstrDeviceName;
		WCHAR*		pstrAddress;
		DWORD		dwState;		// See definitions for State values
	} CONAPI_CONNECTION_INFO;

	// Device info structure
	typedef struct
	{
		WCHAR*		pstrSerialNumber;
		WCHAR*		pstrFriendlyName;
		WCHAR*		pstrModel;
		WCHAR*		pstrManufacturer;
		DWORD		dwNumberOfItems;
		CONAPI_CONNECTION_INFO* pItems;
	} CONAPI_DEVICE;

	// General device info structure
	typedef struct
	{
		DWORD		dwSize;
		DWORD		dwType;
		WCHAR*		pstrTypeName;
		WCHAR*		pstrSWVersion;
		WCHAR*		pstrUsedLanguage;
		DWORD		dwSyncSupport;
		DWORD		dwFileSystemSupport;
	} CONAPI_DEVICE_GEN_INFO;

	// Device product info structure
	typedef struct
	{
		DWORD		dwSize;
		LPAPIWCHAR	pstrProductCode;
	} CONAPI_DEVICE_INFO_PRODUCT;

	// Device device icon structure
	typedef struct
	{
		DWORD		dwSize;			// [in] Size
		DWORD		dwParam;		// [in] Reserved for future use. Must be 0.
		LPAPIWCHAR	pstrTarget;		// [in] Target drive info. Must include memory type (e.g. "MMC" or "DEV").
		DWORD		dwDataLength;	// [out] Icon data length.
		unsigned char* pData;		// [out] Icon data.
	} CONAPI_DEVICE_INFO_ICON;

	// Device property info structure
	typedef struct
	{
		DWORD		dwSize;					// [in] Size
		DWORD		dwTargetPropertyType;	// [in] Target property type
		LPAPIWCHAR	pstrPropertyName;		// [in] Target Property name
		DWORD		dwResult;				// [out] Result code. CONA_OK if succeeded, otherwise error code
		LPAPIWCHAR	pstrPropertyValue;		// [out] Result string. If not found pointer is NULL 
	} CONAPI_GET_PROPERTY;

	typedef struct
	{
		DWORD		dwSize;					// [in] Size
		DWORD		dwNumberOfStructs;		// [in] Count of CONAPI_GET_PROPERTY struct
		CONAPI_GET_PROPERTY* pGetPropertyInfoStructs; // [in] Pointer toCONAPI_GET_PROPERTY structs
	} CONAPI_DEVICE_INFO_PROPERTIES;


	// ----------------------------------------------------

	// Definitions used with swState value in CONAPI_CONNECTION_INFO stucture:
	#define	CONAPI_DEVICE_NOT_FUNCTIONAL		0x00000000	// Device is not working or unsupported device.
	#define	CONAPI_DEVICE_UNPAIRED				0x00000001	// Device is not paired
	#define	CONAPI_DEVICE_PAIRED				0x00000002	// Device is paired
	#define	CONAPI_DEVICE_PCSUITE_TRUSTED		0x00000004	// Device is PC Suite trusted
	#define	CONAPI_DEVICE_WRONG_MODE			0x00000008	// Device is connected in wrong mode.

	// Macros used to check device's swState value: 
	#define	CONAPI_IS_DEVICE_UNPAIRED(dwState)	(dwState & 0x01)		// Returns 1 if true
	#define	CONAPI_IS_DEVICE_PAIRED(dwState)	((dwState >> 1) & 0x01) // Returns 1 if true
	#define	CONAPI_IS_PCSUITE_TRUSTED(dwState)	((dwState >> 2) & 0x01) // Returns 1 if true
	#define	CONAPI_IS_DEVICE_WRONG_MODE(dwState)((dwState >> 3) & 0x01)	// Returns 1 if true

	// Definitions used with CONASearchDevices function:
	#define	CONAPI_ALLOW_TO_USE_CACHE			0x00001000	// Get all devices from cache if available
	#define	CONAPI_GET_ALL_PHONES				0x00002000	// Get all devices from target media
	#define	CONAPI_GET_PAIRED_PHONES			0x00004000	// Get all paired devices from target media
	#define	CONAPI_GET_TRUSTED_PHONES			0x00008000	// Get all PC Suite trusted devices from target media.
 
	// Definitions used with CONAChangeDeviceTrustedState function:
	#define	CONAPI_PAIR_DEVICE					0x00000100	// Pair device
	#define	CONAPI_UNPAIR_DEVICE				0x00000200	// Unpair device
	#define	CONAPI_SET_PCSUITE_TRUSTED			0x00000400	// Set device to PC Suite trusted 
	#define	CONAPI_SET_PCSUITE_UNTRUSTED		0x00000800	// Remove PC Suite trusted information.

	// Definitions used with CONAGetDeviceInfo function:
	#define CONAPI_DEVICE_GENERAL_INFO			0x00010000  // Get CONAPI_DEVICE_GEN_INFO struct.
	#define CONAPI_DEVICE_PRODUCT_INFO			0x00100000  // Get CONAPI_DEVICE_INFO_PRODUCT struct.
	#define CONAPI_DEVICE_PROPERTIES_INFO		0x01000000  // Get CONAPI_DEVICE_INFO_PROPERTIES struct.
	#define CONAPI_DEVICE_ICON_INFO				0x10000000  // Get CONAPI_DEVICE_ICON struct.

	//Definitions used with CONAPI_DEVICE_INFO_PROPERTIES struct
	#define	CONAPI_DEVICE_GET_PROPERTY			0x00000001	// Get value from configuration file.
															// pstrPropertyName must be include target property name.
	#define	CONAPI_DEVICE_IS_APP_SUPPORTED		0x00000002	// Check is the application supported in configuration file.
															// pstrPropertyName must be include target application name.
	
	//The next properties are returned from device's OBEX Capability object:
	#define	CONAPI_DEVICE_GET_CURRENT_NETWORK	0x01000004	// Get Current Network string.
	#define	CONAPI_DEVICE_GET_COUNTRY_CODE		0x02000004	// Get Country Code string.
	#define	CONAPI_DEVICE_GET_NETWORK_ID		0x03000004	// Get Network ID string.
	#define	CONAPI_DEVICE_GET_VERSION			0x00100004	// Get Version string from CONAPI_CO_xxx_SERVICE Service.
	#define	CONAPI_DEVICE_GET_UUID				0x00200004	// Get UUID string from CONAPI_CO_xxx_SERVICE Service.
	#define	CONAPI_DEVICE_GET_OBJTYPE			0x00300004	// Get Object type string from CONAPI_CO_xxx_SERVICE Service.
	#define	CONAPI_DEVICE_GET_FILEPATH			0x00400004	// Get file path string from CONAPI_CO_xxx_SERVICE Service.
															// pstrPropertyName must be include type of file.
	#define	CONAPI_DEVICE_GET_FOLDERPATH		0x00500004	// Get folder path string from CONAPI_CO_xxx_SERVICE Service.
															// pstrPropertyName must be include type of folder (e.g. "Images").
	#define	CONAPI_DEVICE_GET_FOLDERMEMTYPE		0x00600004	// Get folder memory type string from CONAPI_CO_xxx_SERVICE Service. 
															// pstrPropertyName must be include type of folder.
	#define	CONAPI_DEVICE_GET_FOLDEREXCLUDE		0x00700004	// Get folder exclude path string from CONAPI_CO_xxx_SERVICE Service.
															// pstrPropertyName must be include type of folder.
	#define	CONAPI_DEVICE_GET_ALL_VALUES		0x00800004	// Get all values from CONAPI_CO_xxx_SERVICE Service. Values are separated with hash mark (#).
															// pstrPropertyName must be include type of item.
	//Definitions for Services
	#define	CONAPI_DS_SERVICE					0x00001000	// Data Synchronication Service
	#define	CONAPI_DM_SERVICE					0x00002000	// Device Management Service
	#define	CONAPI_NEF_SERVICE					0x00003000	// NEF Service
	#define	CONAPI_DS_SMS_SERVICE				0x00004000	// Data Synchronication SMS Service
	#define	CONAPI_DS_MMS_SERVICE				0x00005000	// Data Synchronication MMS Service
	#define	CONAPI_DS_BOOKMARKS_SERVICE			0x00006000	// Data Synchronication Bookmarks Service
	#define	CONAPI_FOLDER_BROWSING_SERVICE		0x00007000	// Folder-Browsing Service
	#define	CONAPI_USER_DEFINED_SERVICE			0x00008000	// User defined Service. The service name must be set to pstrPropertyName. 
															// If item type is used, it must separate with hash mark (#), format: ServiceName#TypeOfItem
	// Definitions used with General device info structure
	// Device types:
	#define CONAPI_UNKNOWN_DEVICE				0x00000000	// Unknown device.
	#define CONAPI_SERIES40_DEVICE				0x01000001	// Series 40 device
	#define CONAPI_SERIES60_2ED_DEVICE			0x02000010	// Series 60 2nd edition device.
	#define CONAPI_SERIES60_3ED_DEVICE			0x02000020	// Series 60 3nd edition device.
	#define CONAPI_SERIES80_DEVICE				0x02000100	// Series 80 device.
	#define CONAPI_NOKIA7710_DEVICE				0x02001000	// Nokia 7710 device.
	//Synchronication support:
	#define CONAPI_SYNC_NOT_SUPPORTED			0x00000000	// Device does not support synchronication.
	#define CONAPI_SYNC_SA_DS					0x00000001	// Device supports Server Alerted  (SA) Data Synchronication. 
	#define CONAPI_SYNC_SA_DM					0x00000002	// Device supports Server Alerted  (SA) Device Management. 
	#define CONAPI_SYNC_CI_DS					0x00000010	// Device supports Client Initated (CI) Data Synchronication.
	//File System support: 
	#define CONAPI_FS_NOT_SUPPORTED				0x00000000	// Device does not support file system.
	#define CONAPI_FS_SUPPORTED					0x00000001	// Device supports file system.
	#define CONAPI_FS_INSTALL_JAVA_APPLICATIONS	0x00000010	// Device supports Java MIDlet installation.
	#define CONAPI_FS_INSTALL_SIS_APPLICATIONS	0x00000020	// Device supports SIS applications' installation. 
	#define CONAPI_FS_INSTALL_SISX_APPLICATIONS	0x00000040	// Device supports SISX applications' installation. 
	#define CONAPI_FS_FILE_CONVERSION			0x00000100	// Device supports file conversion.
	#define CONAPI_FS_LIST_APPLICATIONS			0x00000200	// Device supports installed applications' listing.
	#define CONAPI_FS_UNINSTALL_APPLICATIONS	0x00000400	// Device supports installed applications' uninstallation.
	#define CONAPI_FS_EXTENDED_OPERATIONS		0x00000800	// Device supports extended File System operations (e.g. Copy folder).

	// Definitions used in CONASetDeviceListOption function
	// Option types:
	#define DMAPI_OPTION_SET_MANUFACTURER		0x00000001	// pstrValue contains the manufacturer name

	// ----------------------------------------------------
	// DeviceNotifyCallbackFunction
	//
	//	This is the function prototype of the callback method
	//
	//	DWORD DeviceNotifyCallbackFunction(	DWORD dwStatus, WCHAR* pstrSerialNumber);
	//	
	//	Status value uses the following format:
	//
	//		----------------DWORD------------------
	//		WORD for info		WORD for status
	//		0000 0000 0000 0000 0000 0000 0000 0000
	//
	//	Status value is the one of the values defined below describing main reason for the notification.
	//	Info part consist of two parts:
	//		LOBYTE: Info part contains change info value. See info values below.
	//		HIBYTE:	Info data value. Depends of info value.
	//	See info value definitions for more information.
	//	Use predefined macros to extract needed part from the status value.
	//
	typedef DWORD (CALLBACK *PFN_CONA_DEVICE_CALLBACK)(	DWORD dwStatus, WCHAR* pstrSerialNumber);

	// Device callback status values
	#define	CONAPI_DEVICE_LIST_UPDATED		0x00	// List is updated. No any specific information.
	#define	CONAPI_DEVICE_ADDED				0x01	// A new device is added to the list.
	#define	CONAPI_DEVICE_REMOVED			0x02	// Device is removed from the list.
	#define	CONAPI_DEVICE_UPDATED			0x04	// Device is updated. A connection is added or removed
													// or device is renamed. Device still exist in the list.
	// Device callback info values
	#define	CONAPI_CONNECTION_ADDED			0x01	// Note! HIBYTE == media, LOBYTE == CONAPI_CONNECTION_ADDED
	#define	CONAPI_CONNECTION_REMOVED		0x02	// Note! HIBYTE == media, LOBYTE == CONAPI_CONNECTION_REMOVED
	#define	CONAPI_DEVICE_RENAMED			0x04	// Friendly name of the device is changed

	// Device callback macros
	#define	GET_CONAPI_CB_STATUS(Status)	(0x0000FFFF & Status)
	#define	GET_CONAPI_CB_INFO(Status)		((0x00FF0000 & Status)>>16)
	#define	GET_CONAPI_CB_INFO_DATA(Status)	((0xFF000000 & Status)>>24)
	// ----------------------------------------------------

	// ----------------------------------------------------
	// IDMAPIDeviceNotify
	//
		#ifdef __cplusplus
			class IDMAPIDeviceNotify
			{
			public:
				virtual void OnDeviceListUpdated() = 0;
				virtual void OnDeviceAdded(const WCHAR* pstrSN, DWORD dwStatus) = 0;
				virtual void OnDeviceRemoved(const WCHAR* pstrSN, DWORD dwStatus) = 0;
				virtual void OnDeviceUpdated(const WCHAR* pstrSN, DWORD dwStatus) = 0;
			};
		#endif
	// ----------------------------------------------------

	// ----------------------------------------------------------------------
	// DeviceSearchOperationCallbackFunction
	//
	// Description
	// Device Search operation callback functions are defined as: 
	//	DWORD (DeviceSearchOperationCallbackFunction)(DWORD dwState, 
	//					CONAPI_CONNECTION_INFO* pConnInfoStructure)
	//
	//	The Connectivity API calls this function periodically (or if the
	//	System has found the device during this time) and adds one 
	//	to the function state value. The used time period is counted by using 
	//	dwSearchTime parameter. E.g. If dwSearchTime paramater value is 240,
	//	time period  (240/100) is 2.4 seconds.
	//	If the function state is 100 and any device have not been found during 
	//	this (dwSearchTime) time the CONASearchDevices function fails with the 
	//	error code ECONA_FAILED_TIMEOUT.
	//
	// Parameters
	//	dwState				[in] Function state (0-100%).
	//	pConnInfoStructure	[in] Reserved for future use, the value is NULL.
	//
	// Return values
	// The Connectivity API-user must return the CONA_OK value. If the callback 
	// function returns the error code ECONA_CANCELLED to the Connectivity API, 
	// the CONASearchDevices function will be cancelled with the error code ECONA_CANCELLED.
	//
	// Type definition: 
	typedef DWORD (CALLBACK *PFN_CONA_SEARCH_CALLBACK)(	DWORD dwState, CONAPI_CONNECTION_INFO* pConnInfoStructure);
	// -----------------------------------------------------------------------

//=========================================================

#endif //_DMAPIDEFINITIONS_H__
