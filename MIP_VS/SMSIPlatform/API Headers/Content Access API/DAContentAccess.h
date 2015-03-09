/*
==============================================================================
* Content Access API 3.2
*
Filename    : DAContentAccess.h
Description : Content Access API functions
Version     : 3.2

Copyright (c) 2006-2007 Nokia Corporation.
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
#ifndef __DACONTENTACCESSAPI_H__
#define __DACONTENTACCESSAPI_H__

///////////////////////////////////////////////////////////
// PC Connectivity API
///////////////////////////////////////////////////////////
	#ifndef PCCS_CAAPI
		#define PCCS_CAAPI DWORD STDAPICALLTYPE
		#define PCCS_CAAPI_EX(type) type STDAPICALLTYPE
	#endif
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Content Access API definitions
///////////////////////////////////////////////////////////
	#include "DAContentAccessDefinitions.h"
	#include "CADataDefinitions.h"
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// Content Access API 
///////////////////////////////////////////////////////////////

	//=========================================================
	// Content Access API version
	//
		#define CAAPI_VERSION_30	30
		#define CAAPI_VERSION_31	31
		#define CAAPI_VERSION_32	32
	//=========================================================

	//=========================================================
	// CAAPI_Initialize
	//
	// Description:
	//	CAAPI_Initialize initializes the API. This must be called once and before any other API call!
	//  It's not allowed to call this function like this 
	//		CAAPI_Initialize(CAGetAPIVersion(), NULL);
	//	You must call it like this
	//		CAAPI_Initialize(CAAPI_VERSION_32, NULL);
	//
	// Parameters:
	//	dwAPIVersion	[in] CAAPI version requested.
	//	pdwParam		[in] Reserved for future use. Must be NULL.
	//
	// Return values:
	//
		PCCS_CAAPI CAAPI_Initialize(DWORD dwAPIVersion, DWORD* pdwParam);
	//=========================================================

	//=========================================================
	// CAAPI_Terminate
	//
	// Description:
	//	CAAPI_Terminate terminates the API. This must be called once and as the last API call!
	//
	// Parameters:
	//	pdwParam		[in] Reserved for future use. Must be NULL.
	//
	// Return values:
	//
		PCCS_CAAPI CAAPI_Terminate(DWORD* pdwParam);
	//=========================================================

	//=========================================================
	// CAGetAPIVersion
	//
	// Description:
	//	Returns current version of this API. 	
	//
	// Parameters:
	//	-
	//
	// Return values:
	// DWORD- function returns the Content Access API version number. 
	//
		PCCS_CAAPI CAGetAPIVersion();
	//=========================================================
	
	//=========================================================
	// DAOpenCA
	//
	// Description:
	// Open content access connection for specified content type
	//  
	// Parameters:
	// pstrSN		[in] Serial number (IMEI) of the device to be connected	
	// pdwMedia		[in,out] Media used for connecting 		
	// dwTarget		[in] Target content to be opened, 
	//				see "Available PIM connection targets" in DADefinitions.h
	// phCAHandle	[out] Handle to opened CA connection
	//
	// Return values:
	// CONA_OK
	// ECONA_DEVICE_NOT_FOUND
	// ECONA_INVALID_POINTER	
	// ECONA_INVALID_PARAMETER
	// ECONA_DEVICE_NOT_FOUND
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI DAOpenCA(WCHAR* pstrSN, 						
							DWORD* pdwMedia,
							DWORD dwTarget,
							CAHANDLE *phCAHandle);
	//=========================================================

	//=========================================================
	// DACloseCA
	//
	// Description:
	// Closes content access connection. If operations have been 
	// started but not closed for connection specified by connection 
	// handle, those will be closed also. 
	//  
	// Parameters:
	// hCAHandle	[in]	Handle to content access connection.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI DACloseCA(CAHANDLE hCAHandle);
    //=========================================================

	//=========================================================
	// CAGetFolderInfo
	//
	// Description:
	// Get's content specific item paths from the device. For some content
	// types there only exists root path which will be returned as '\\' in output
	// parameters
	//
	// Parameters:
	// hCAHandle		[in]	Handle to opened CA connection	
	// pFolderInfo		[out]	Content specific target paths, 
	//							"\\" is the root for target CA connection.
	//							Some connection types only have root ("\\") folder defined.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_POINTER
	// ECONA_CONNECTION_BUSY	
	// ECONA_CONNECTION_LOST
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CAGetFolderInfo(	CAHANDLE hCAHandle,
									CA_FOLDER_INFO *pFolderInfo );
	//=========================================================

	//=========================================================
	// CAFreeFolderInfoStructure 
	//
	// Description:
	// CAFreeItemPathStructure frees item path structure, which was 
	// allocated when CAGetItemPaths was called
	//
	// Parameters:
	// pFolderInfo		[in]	Content specific folder structure
	//							
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_INVALID_PARAMETER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CAFreeFolderInfoStructure (CA_FOLDER_INFO *pFolderInfo );
	//=========================================================

	//=========================================================
	// CAGetIDList
	//
	// Description:
	// Reads content specific ID list from the device.
	// 
	// In S60 devices, if device is missing support for specific 
	// content ( SMS , MMS or BOOKMARKS), error code 
	// ECONA_SYNC_INSTALL_PLUGIN_FIRST will be returned .. 
	//  
	// Parameters:
	// hCAHandle		[in]	Handle to opened CA connection
	// dwFolderId		[in]	Amount of UID's returned
	// dwOptions		[in]	Options for ID listing. Following options 
	//							are available : 
	//							CA_OPTION_USE_CACHE option reads item ID list from 
	//							from cache DB. If cache database is empty,
	//							ID listing is read from device.
	//							CA_OPTION_USE_ONLY_CACHE option reads item ID list
	//							only from cache DB, and returns amount of ID's in 
	//							cache
	// pIDList			[out]	Structure containing list of item ID's 
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_POINTER
	// ECONA_UNKNOWN_ERROR_DEVICE
	// ECONA_INVALID_PARAMETER
	// ECONA_CONNECTION_BUSY	
	// ECONA_CONNECTION_LOST
	// ECONA_SYNC_INSTALL_PLUGIN_FIRST
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CAGetIDList(	CAHANDLE hCAHandle,
								DWORD dwFolderId,
								DWORD dwOptions,
								CA_ID_LIST* pIDList );
	//=========================================================
	
	//=========================================================
	// CAFreeIdListStructure
	//
	// Description:
	// Frees memory allocated in CAGetIdList/CACommitOperation call
	//
	// Parameters:
	// pIDList		[in]	ID list structure to be freed
	//								
	// Return values:
	// CONA_OK
	// ECONA_INVALID_POINTER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CAFreeIdListStructure( CA_ID_LIST *pIDList );
	//=========================================================
	
	//=========================================================
	// CABeginOperation
	//
	// Description:
	// Begins operations through Content Access API, returns handle
	// through which Read/Write/Delete operations are accessed. 
	//  
	// Parameters:
	// hCAHandle		[in]	Handle to opened CA connection	
	// dwParam			[in]	For future use, set to zero.
	// phOperHandle		[out]	Handle to new operation 
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_UNKNOWN_ERROR
	//	
		PCCS_CAAPI CABeginOperation(	CAHANDLE hCAHandle, 
										DWORD dwParam,		
										CAOPERATIONHANDLE* phOperHandle);
	//=========================================================

	//=========================================================
	// CAReadItem
	//
	// Description:
	// Reads content item from the device.  Output parameter's contain
	// target specific structures for content
	//  
	// Parameters:
	// hOperHandle		[in]		Handle to current operation
	// pID				[in]		Item to be read from the device 
	// dwOptions		[in]		Options for read operation, see 
	//								"Options for item reading" in DAContentAccessDefinitions.h
	// dwDataFormat		[in]		Data format used in reading
	// pCaItemData		[in,out]	Item data returned
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_POINTER
	// ECONA_NOT_FOUND
	// ECONA_CONNECTION_BUSY	
	// ECONA_CONNECTION_LOST
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CAReadItem(	CAOPERATIONHANDLE hOperHandle, 
								CA_ITEM_ID* pID, 
								DWORD dwOptions,
								DWORD dwDataFormat,
								LPVOID pItemData);
	//=========================================================

	//=========================================================
	// CAWriteItem
	//
	// Description:
	// Writes new item to the device. Actual writing will be committed
	// when CAEndOperation is called. CA_ITEM_ID parameter contains 
	// temporary ID for the item to be created which can be used after 
	// end operation to check result of specific item write.
	//  
	// Parameters:
	// hOperHandle		[in] Handle to current operation
	// pID				[in,out] Contains path info and returns 
	//					temporary ID for writing.
	// dwOptions		[in] See "Options for item writing" in DAContentAccessDefinitions.h
	// pCaItemData		[in] Item data to be saved to device
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_POINTER
	// ECONA_CONNECTION_LOST
	// ECONA_UNKNOWN_ERROR
	//	
		PCCS_CAAPI CAWriteItem(	CAOPERATIONHANDLE hOperHandle, 
								CA_ITEM_ID* pID, 
								DWORD dwOptions, 
								DWORD dwDataFormat,
								LPVOID pItemData);
	//=========================================================

	//=========================================================
	// CADeleteItem
	//
	// Description:
	// Marks item for deletion , actual deletion will be committed
	// when CAEndOperation is called. Returns temporary ID for deletion
	// operation.
	//  
	// Parameters:
	// hOperHandle		[in]		Handle to current operation
	// pID				[in,out]	Item to be deleted
	// dwOptions		[in]		For future use, must be set to zero
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE	
	// ECONA_INVALID_PARAMETER
	// ECONA_CONNECTION_LOST
	// ECONA_INVALID_POINTER
	// ECONA_NOT_FOUND
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CADeleteItem(	CAOPERATIONHANDLE hOperHandle, 
									CA_ITEM_ID* pID,
									DWORD dwOptions);
	//=========================================================
	
	//=========================================================
	// CAWriteField
	//
	// Description:
	// CAWriteField method is used for updating content item in device.  If item does not 
	// exist in the device, it will be created. Actual writing to the device is done in 
	// CACommitOperations method. 
	// 
	// Currently only supported with Series 40 device’s phonebook content. Other device types or 
	// content types will return ECONA_NOT_SUPPORTED_PC error to the caller.
	//  
	// Parameters:
	// hOperHandle		[in]		Handle to current operation
	// pID				[in]		Contains path info and returns temporary ID for writing.	
	// dwOptions		[in]		For future use, must be set to zero
	// pDataItem		[in]		Data item containing the data of the field. When updating, dwFieldID member must
	//								be a valid field ID received from CAReadItem call. When adding a new field,dwFieldID
	//								member must be set to zero.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_NOT_SUPPORTED_PC
	// ECONA_INVALID_POINTER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CAWriteField(CAOPERATIONHANDLE hOperHandle, 
								CA_ITEM_ID* pID,
								DWORD dwOptions,
								CA_DATA_ITEM* pDataItem );
	//=========================================================

	//=========================================================
	// CADeleteField
	//
	// Description:
	// CADeleteField method is used for deleting single field from content item in device.  
	// Actual deletion will be done in CACommitOperations method. 
	// 
	// Currently only supported with Series 40 device’s phonebook content. Other device types 
	// or content types will return ECONA_NOT_SUPPORTED_PC error to the caller.
	//  
	// Parameters:
	// hOperHandle		[in]		Handle to current operation
	// pID				[in]		ID for the item from where field will be deleted
	// dwOptions		[in]		For future use, must be set to zero
	// pDataItem		[in]		Data item containing the data of the field. When deleting, dwFieldID member must
	//								be a valid field ID received from CAReadItem call.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_NOT_SUPPORTED_PC
	// ECONA_INVALID_POINTER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CADeleteField(	CAOPERATIONHANDLE hOperHandle, 
									CA_ITEM_ID* pID,
									DWORD dwOptions,
									CA_DATA_ITEM* pDataItem );
	//=========================================================

	//=========================================================
	// CACreateFolder
	//
	// Description:
	// Creates folder to the device.  Returns temporary folder ID for the 
	// folder to be created, and this ID can be used for saving items to folder. 
	// Actual folder creation to the device will be done when CACommitOperations is called. 
	// 
	// Supported content types are :
	// CA_TARGET_SMS_MESSAGES, CA_TARGET_MMS_MESSAGES,CA_TARGET_BOOKMARKS
	//  
	// Parameters:
	// hOperHandle		[in]		Handle to current operation
	// pID				[in]		Pointer to CA_ITEM_ID structure, temporary ID 
	//								for folder will be returned in this struct.
	//								temporary ID for the folder can be used when writing items to
	//								specified folder
	// pstrFolderName 	[in]		Folder to be created. Parameter value can contain 
	//								multiple subfolder levels (e.g folder1\folder2). 
	// 
	//								In sms and mms message cases, folders can only be created 
	//								under folder CA_MESSAGE_FOLDER_USER_FOLDERS. 
	//								Format of this parameter in messaging case would be for example: 
	//								"predefuserfolders\folder1". If "predefuserfolders" part is not 
	//								included to pstrFolderPath parameter, path is considered as invalid
	//
	//								ECONA_NOT_SUPPORTED_DEVICE error will be returned if target 
	//								content does not support folder creation
	//								
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_NOT_SUPPORTED_DEVICE
	// ECONA_NOT_SUPPORTED_PC
	// ECONA_INVALID_POINTER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CACreateFolder(	CAOPERATIONHANDLE hOperHandle, 
									CA_ITEM_ID* pID,
									WCHAR *pstrFolderName); 
	//=========================================================
		
	//=========================================================
	// CADeleteFolder
	//
	// Description:
	// Deletes folder from the device. 
	// Actual deletion will be done in CACommitOperations method. 
	// 
	// Supported content types are :
	// CA_TARGET_SMS_MESSAGES, CA_TARGET_MMS_MESSAGES,CA_TARGET_BOOKMARKS
	//  
	// Parameters:
	// hOperHandle		[in]		Handle to current operation
	// pID				[in]		CA_ITEM_ID structure containing folder ID 
	//								information about the folder to be deleted
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_NOT_SUPPORTED_PC
	// ECONA_INVALID_POINTER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CADeleteFolder(	CAOPERATIONHANDLE hOperHandle, 
									CA_ITEM_ID* pID); 
	//=========================================================
	
	//=========================================================
	// CACommitOperations
	//
	// Description:
	// Commits item write, item delete , folder create , folder delete 
	// operations to the phone and returns status information for operations
	//  
	// Parameters:
	// hOperHandle	[in]	Handle to current operation
	// pIDList		[out]	Item status list for committed operations 
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_CONNECTION_BUSY	
	// ECONA_CONNECTION_LOST
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CACommitOperations(	CAOPERATIONHANDLE hOperHandle, 
										CA_ID_LIST* pIDList );	
	//=========================================================

	//=========================================================
	// CAEndOperation
	//
	// Description:
	// Closes opened operation without committing changes.
	//  
	// Parameters:
	// hOperHandle	[in]	Handle to current operation
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_UNKNOWN_ERROR
	//	
		PCCS_CAAPI CAEndOperation(	CAOPERATIONHANDLE hOperHandle); 
	//=========================================================
									
	//=========================================================
	// CAFreeItemData
	//
	// Description:
	// Frees the item data reserved by Content Access API
	//  
	// Parameters:
	// hCAHandle	[in]	Handle to the existing API connection
	// dwDataFormat	[in]	Data format used when memory was reserved ..
	// caItemData	[in]	Pointer to the allocated structure ..
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_POINTER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CAFreeItemData(	CAHANDLE hCAHandle, 
									DWORD dwDataFormat, 
									LPVOID pItemData);
	//=========================================================

	//=========================================================
	// CARegisterNotifyCallback
	//
	// Description:
	//	Registers and unregisters callback function for notifications
	//  
	// Parameters:
	//  hCAHandle	[in]	Handle to the existing API connection
	//	dwState		[in]	Used to define the action:
	//						 API_REGISTER used in registeration
	//						 API_UNREGISTER.used in removing the registeration
	//	pfnNotify	[in]	Function pointer of the call back method
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CARegisterNotifyCallback(CAHANDLE hCAHandle, 
											DWORD dwState, 
											PFN_CA_NOTIFY_CALLBACK pfnNotify);
    //=========================================================

	//=========================================================
	// CARegisterOperationCallback
	//
	// Description:
	//	Registers and unregisters callback function for operation 
	//  notifications
	//  
	// Parameters:
	//  hOperHandle	[in]	Handle to the existing API connection
	//	dwState		[in]	Used to define the action:
	//						 API_REGISTER used in registeration
	//						 API_UNREGISTER.used in removing the registeration
	//	pfnNotify	[in]	Function pointer of the call back method
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CARegisterOperationCallback(	CAOPERATIONHANDLE hOperHandle, 
												DWORD dwState, 
												PFN_CA_OPERATION_CALLBACK pfnNotify);
    //=========================================================

	#ifdef __cplusplus
	//=========================================================
	// CARegisterNotifyIF
	//
	// Description:
	//	Registers and unregisters notification interface
	//  
	// Parameters:
	//  hCAHandle	[in]	Handle to the existing API connection
	//	dwState		[in]	Used to define the action:
	//						 API_REGISTER used in registeration
	//						 API_UNREGISTER used in removing the registeration
	//	pINotify	[in]	ICANotify interface pointer 
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CARegisterNotifyIF(	CAHANDLE hCAHandle, 
										DWORD dwState, 
										ICANotify* pINotify);
    //=========================================================

	//=========================================================
	// CARegisterOperationNotifyIF
	//
	// Description:
	//	Registers and unregisters operation notification interface
	//  
	// Parameters:
	//  hCAOperHandle[in]	Handle to the existing API connection
	//	dwState		 [in]	Used to define the action:
	//						 API_REGISTER used in registeration
	//						 API_UNREGISTER used in removing the registeration
	//	pIOperNotify [in]	ICAOperationNotify interface pointer 
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_CAAPI CARegisterOperationNotifyIF(	CAOPERATIONHANDLE hCAOperHandle, 
												DWORD dwState, 
												ICAOperationNotify* pIOperNotify);
    //=========================================================
	#endif

#endif //__DACONTENTACCESSAPI_H__
