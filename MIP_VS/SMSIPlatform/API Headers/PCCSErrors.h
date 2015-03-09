/*
==============================================================================
* PC Connectivity API 3.2
*
Filename    : PCCSErrors.h
Description : Error Definitions
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

#ifndef _PCCSERRORS_H_
#define _PCCSERRORS_H_

///////////////////////////////////////////////////////////
// Connectivity API errors
///////////////////////////////////////////////////////////

#define		CONA_OK								0x00000000	// Everything ok
#define		CONA_OK_UPDATED_MEMORY_VALUES		0x00000001	// Everything ok, given data is updated because (free, used and total) memory values are changed!
#define		CONA_OK_UPDATED_MEMORY_AND_FILES	0x00000002	// Everything ok, given data is updated because files and memory values are changed!
#define		CONA_OK_UPDATED						0x00000004	// Everything ok, given data is updated, unknown reason.

#define		CONA_OK_BUT_USER_ACTION_NEEDED		0x00000100	// Everything ok, but operation needs 
															// some user action (device side)
#define		CONA_WAIT_CONNECTION_IS_BUSY		0x00000101	// Operation started ok but other application
															// is reserved connection, please wait. This 
															// result code comes via FS nofication when ConnAPI
															// is initialized by value 20 or bigger.
// Common error codes:
#define		ECONA_INIT_FAILED					0x80100000	// DLL initialization failed
#define		ECONA_INIT_FAILED_COM_INTERFACE		0x80100002	// Failed to get connection to System.
#define		ECONA_NOT_INITIALIZED				0x80100004	// API is not initialized
#define		ECONA_UNSUPPORTED_API_VERSION		0x80100005	// Failed, not supported API version
#define		ECONA_NOT_SUPPORTED_MANUFACTURER	0x80100006	// Failed, not supported manufacturer

#define		ECONA_UNKNOWN_ERROR					0x80100010	// Failed, unknown error
#define		ECONA_UNKNOWN_ERROR_DEVICE			0x80100011	// Failed, unknown error from Device
#define		ECONA_INVALID_POINTER				0x80100012	// Required pointer is invalid
#define		ECONA_INVALID_PARAMETER				0x80100013	// Invalid Parameter value 
#define		ECONA_INVALID_HANDLE				0x80100014	// Invalid HANDLE
#define		ECONA_NOT_ENOUGH_MEMORY				0x80100015  // Memory allocation failed in PC
#define		ECONA_WRONG_THREAD					0x80100016	// Failed, Called interface was marshalled for a different thread.
#define		ECONA_REGISTER_ALREADY_DONE			0x80100017	// Failed, notification interface is already registered.

#define		ECONA_CANCELLED						0x80100020	// Operation cancelled by ConnectivityAPI-User
#define		ECONA_NOTHING_TO_CANCEL				0x80100021	// No running functions, or cancel has called too late.
#define		ECONA_FAILED_TIMEOUT				0x80100022	// Operation failed because of timeout
#define		ECONA_NOT_SUPPORTED_DEVICE			0x80100023	// Device do not support operation
#define		ECONA_NOT_SUPPORTED_PC				0x80100024	// ConnectivityAPI do not support operation (not implemented)
#define		ECONA_NOT_FOUND						0x80100025	// Item was not found
#define		ECONA_FAILED						0x80100026	// Failed, the called operation failed.

#define		ECONA_API_NOT_FOUND					0x80100100	// Needed API module was not found from the system
#define		ECONA_API_FUNCTION_NOT_FOUND		0x80100101	// Called API function was not found from the loaded API module

// Device manager and device connection releated error:
#define		ECONA_DEVICE_NOT_FOUND				0x80200000  // Given phone is not connected (refresh device list)
#define		ECONA_NO_CONNECTION_VIA_MEDIA		0x80200001  // Phone is connected but not via given Media
#define		ECONA_NO_CONNECTION_VIA_DEVID		0x80200002  // Phone is not connected with given DevID
#define		ECONA_INVALID_CONNECTION_TYPE		0x80200003  // Connection type was invalid
#define		ECONA_NOT_SUPPORTED_CONNECTION_TYPE 0x80200004  // Device do not support connection type
#define		ECONA_CONNECTION_BUSY				0x80200005  // Other application is recerved connection
#define		ECONA_CONNECTION_LOST				0x80200006  // Connection is lost to Device
#define		ECONA_CONNECTION_REMOVED			0x80200007  // Connection removed, other application is reserved connection.
#define		ECONA_CONNECTION_FAILED				0x80200008  // Connection failed, unknown reason
#define		ECONA_SUSPEND						0x80200009  // Connection removed, PC goes suspend state
#define		ECONA_NAME_ALREADY_EXISTS			0x8020000A  // Friendly name already exist
#define		ECONA_MEDIA_IS_NOT_WORKING			0x8020000B  // Failed, target media is active but it is not working (e.g. BT-hardware stopped or removed)
#define		ECONA_CACHE_IS_NOT_AVAILABLE		0x8020000C	// Failed, cache is not available (CONASearchDevices)
#define		ECONA_MEDIA_IS_NOT_ACTIVE			0x8020000D  // Failed, target media is not active (or ready yet)
#define		ECONA_PORT_OPEN_FAILED				0x8020000E  // Port opening failed (only when media is CONAPI_MEDIA_SERIAL and COM port is changed).

// Device paring releated errors:
#define		ECONA_DEVICE_PAIRING_FAILED			0x80200100 // Failed, pairing failed
#define		ECONA_DEVICE_PASSWORD_WRONG			0x80200101 // Failed, wrong password on device. 
#define		ECONA_DEVICE_PASSWORD_INVALID		0x80200102 // Failed, password includes invalid characters or missing. 


// File System errors:
#define		ECONA_ALL_LISTED					0x80300000	// All items are listed
#define		ECONA_MEMORY_FULL					0x80300001	// Device memory full

// File System error for file functions:
#define		ECONA_FILE_NAME_INVALID				0x80400001	// File name includes invalid characters in Device or PC
#define		ECONA_FILE_NAME_TOO_LONG			0x80400002	// Max unicode charaters in File name (includes current/target path) 
															// is limited to 256 charaters in device. (256 charaters includes 
															// two backlashs in begin of the path and one end of the path.)
#define		ECONA_FILE_ALREADY_EXIST			0x80400003	// File already exits in Device or PC
#define		ECONA_FILE_NOT_FOUND				0x80400004	// File does not exits in Device or PC
#define		ECONA_FILE_NO_PERMISSION			0x80400005	// Not allow to perform required operation to file in Device 
#define		ECONA_FILE_COPYRIGHT_PROTECTED		0x80400006	// Not allow to perform required operation to file in Device
#define		ECONA_FILE_BUSY						0x80400007	// Other application is recerved file in Device or PC
#define		ECONA_FILE_TOO_BIG_DEVICE			0x80400008	// Device reject the operation because file size is too big
#define		ECONA_FILE_TYPE_NOT_SUPPORTED		0x80400009	// Device reject the operation because file unsupported type
#define		ECONA_FILE_NO_PERMISSION_ON_PC		0x8040000A	// Not allow to perform required operation to file in PC 
#define		ECONA_FILE_EXIST					0x8040000B	// File move or rename: File is copied to target path with new name but removing the source file failed. 
#define		ECONA_FILE_CONTENT_NOT_FOUND		0x8040000C	// Specified file content does not found (e.g. unknown file section or stored position).
#define		ECONA_FILE_OLD_FORMAT				0x8040000D	// Specified file content supports old engine.
#define		ECONA_FILE_INVALID_DATA				0x8040000E	// Specified file data is invalid.

// File System error for folder functions:
#define		ECONA_INVALID_DATA_DEVICE			0x80500000	// Device's folder contain invalid data
#define		ECONA_CURRENT_FOLDER_NOT_FOUND		0x80500001	// Current/Target folder is invalid in device (e.g MMC card removed). 
#define		ECONA_FOLDER_PATH_TOO_LONG			0x80500002	// Current/Target folder max unicode charaters count is limited to 
															// 245 charaters in device. (245 charaters includes two backlashs in 
															// begin of the path and one end of the path)
#define		ECONA_FOLDER_NAME_INVALID			0x80500003	// Folder name includes invalid characters in Device or PC
#define		ECONA_FOLDER_ALREADY_EXIST			0x80500004	// Folder is already exits in target folder
#define		ECONA_FOLDER_NOT_FOUND				0x80500005	// Folder not found (PC/Device)
#define		ECONA_FOLDER_NO_PERMISSION			0x80500006	// Not allow to perform required operation to folder in Devic
#define		ECONA_FOLDER_NOT_EMPTY				0x80500007	// Not allow to perform required operation because folder is not empty
#define		ECONA_FOLDER_NO_PERMISSION_ON_PC	0x80500008	// Not allow to perform required operation to folder in PC

// Application Installation:
#define		ECONA_DEVICE_INSTALLER_BUSY			0x80600000	// Can not start Device's installer

//Syncronization specific error codes :
#define		ECONA_UI_NOT_IDLE_DEVICE			0x80700000	// Failed, device rejects the operation. Maybe device's UI was not IDLE-state.
#define		ECONA_SYNC_CLIENT_BUSY_DEVICE		0x80700001	// Failed, device's SA sync client is busy.
#define		ECONA_UNAUTHORIZED_DEVICE 			0x80700002	// Failed, device rejects the operation. No permission.
#define		ECONA_DATABASE_LOCKED_DEVICE 		0x80700003	// Failed, device rejects the operation. Device is locked.
#define		ECONA_SETTINGS_NOT_OK_DEVICE 		0x80700004	// Failed, device rejects the operation. Maybe settings in Sync profile are wrong on Device.
#define		ECONA_SYNC_ITEM_TOO_BIG				0x80700501 // 
#define		ECONA_SYNC_ITEM_REJECT				0x80700502 // All commands,Device reject the operation...
#define		ECONA_SYNC_INSTALL_PLUGIN_FIRST		0x80700506 // 

// Versit conversion specific error codes :			
#define		ECONA_VERSIT_INVALID_PARAM				0x80800001	// Invalid parameters passed to versit converter 
#define		ECONA_VERSIT_UNKNOWN_TYPE				0x80800002	// Failed, trying to convert versit formats not supported in VersitConverter
#define		ECONA_VERSIT_INVALID_VERSIT_OBJECT		0x80800003	// Failed, validation of versit data not passed, contains invalid data

// Database specific error codes :
#define		ECONA_DB_TRANSACTION_ALREADY_STARTED	0x80800100	// Another transaction is already in progress.
#define		ECONA_DB_TRANSACTION_FAILED				0x80800101	// Some of operations within a transaction failed and transaction was rolled back.

//Backup specific error codes
#define		ECONA_DEVICE_BATTERY_LEVEL_TOO_LOW		0x80900000  // Failed, device rejects the restore operation. Device's battery level is low.
#define		ECONA_DEVICE_BUSY						0x80900001  // Failed, device rejects the backup/resore operation. Device's backup server busy.

#endif

