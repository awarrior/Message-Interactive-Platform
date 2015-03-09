/*
Filename    : PCCSUtils.cpp
Part of     : PCCS example codes
Description : PCCS helper functions
Version     : 3.2

This example is only to be used with PC Connectivity API version 3.2.
Compability ("as is") with future versions is not quaranteed.

Copyright (c) 2007 Nokia Corporation.
 
This material, including but not limited to documentation and any related 
computer programs, is protected by intellectual property rights of Nokia 
Corporation and/or its licensors.
All rights are reserved. Reproducing, modifying, translating, or 
distributing any or all of this material requires the prior written consent 
of Nokia Corporation. Nokia Corporation retains the right to make changes 
to this material at any time without notice. A copyright license is hereby 
granted to download and print a copy of this material for personal use only.
No other license to any other intellectual property rights is granted. The 
material is provided "as is" without warranty of any kind, either express or 
implied, including without limitation, any warranty of non-infringement, 
merchantability and fitness for a particular purpose. In no event shall 
Nokia Corporation be liable for any direct, indirect, special, incidental, 
or consequential loss or damages, including but not limited to, lost profits 
or revenue,loss of use, cost of substitute program, or loss of data or 
equipment arising out of the use or inability to use the material, even if 
Nokia Corporation has been advised of the likelihood of such damages occurring.
*/

#include "stdafx.h"
#include "Shlwapi.h"

//===================================================================
// CONAError2String --  Returns error text for given CONA error code
//
//
//===================================================================
CString CONAError2String(DWORD error)
{
    switch(error)
    {
    case CONA_OK: return L"CONA_OK: Succeeded";
    case CONA_OK_UPDATED_MEMORY_VALUES: return L"CONA_OK_UPDATED_MEMORY_VALUES: Everything OK, given data is updated because (free, used and total) memory values are changed!";
    case CONA_OK_UPDATED_MEMORY_AND_FILES: return L"CONA_OK_UPDATED_MEMORY_AND_FILES: Everything OK, given data is updated because files and memory values are changed!";
    case CONA_OK_UPDATED: return L"CONA_OK_UPDATED: Everything OK, given data is updated, unknown reason.";
	case CONA_OK_BUT_USER_ACTION_NEEDED: return L"CONA_OK_BUT_USER_ACTION_NEEDED: Operation needs some user action on Device";
	case CONA_WAIT_CONNECTION_IS_BUSY: return L"CONA_WAIT_CONNECTION_IS_BUSY: Operation started ok but other application has reserved the connection";

	// Common error codes:
    case ECONA_INIT_FAILED: return L"ECONA_INIT_FAILED: DLL initialization failed";
    case ECONA_INIT_FAILED_COM_INTERFACE: return L"ECONA_INIT_FAILED_COM_INTERFACE: DLL initialization failed";
    case ECONA_NOT_INITIALIZED: return L"ECONA_NOT_INITIALIZED: API is not initialized";
    case ECONA_UNSUPPORTED_API_VERSION: return L"ECONA_UNSUPPORTED_API_VERSION: API version not supported";
    case ECONA_NOT_SUPPORTED_MANUFACTURER: return L"ECONA_NOT_SUPPORTED_MANUFACTURER: Manufacturer is not supported";

    case ECONA_UNKNOWN_ERROR: return L"ECONA_UNKNOWN_ERROR: Failed, unknown error";
    case ECONA_UNKNOWN_ERROR_DEVICE: return L"ECONA_UNKNOWN_ERROR_DEVICE: Failed, unknown error from Device";
    case ECONA_INVALID_POINTER: return L"ECONA_INVALID_POINTER: Required pointer is invalid";
    case ECONA_INVALID_PARAMETER: return L"ECONA_INVALID_PARAMETER: Invalid parameter value";
    case ECONA_INVALID_HANDLE: return L"ECONA_INVALID_HANDLE: Invalid HANDLE";
    case ECONA_NOT_ENOUGH_MEMORY: return L"ECONA_NOT_ENOUGH_MEMORY: Memory allocation failed in PC";
    case ECONA_WRONG_THREAD: return L"ECONA_WRONG_THREAD: Failed, Called interface was marshalled for a different thread";
    case ECONA_REGISTER_ALREADY_DONE: return L"ECONA_REGISTER_ALREADY_DONE: Failed, notification interface is already registered.";

    case ECONA_CANCELLED: return L"ECONA_CANCELLED: Operation cancelled by ConnectivityAPI-User";
    case ECONA_NOTHING_TO_CANCEL: return L"ECONA_NOTHING_TO_CANCEL: No running functions";
    case ECONA_FAILED_TIMEOUT: return L"ECONA_FAILED_TIMEOUT: Operation failed because of timeout";
    case ECONA_NOT_SUPPORTED_DEVICE: return L"ECONA_NOT_SUPPORTED_DEVICE: Device does not support operation";
    case ECONA_NOT_SUPPORTED_PC: return L"ECONA_NOT_SUPPORTED_PC: Connectivity API does not support operation (not implemented)";
    case ECONA_NOT_FOUND: return L"ECONA_NOT_FOUND: Item was not found";
    case ECONA_FAILED: return L"ECONA_FAILED: The called operation failed.";

    case ECONA_API_NOT_FOUND: return L"ECONA_API_NOT_FOUND: Needed API module was not found from the system";
    case ECONA_API_FUNCTION_NOT_FOUND: return L"ECONA_API_FUNCTION_NOT_FOUND: Called API function was not found from the loaded API module";

	// Device manager and device connection releated error:
    case ECONA_DEVICE_NOT_FOUND: return L"ECONA_DEVICE_NOT_FOUND: Given phone is not connected (refresh device list)";
    case ECONA_NO_CONNECTION_VIA_MEDIA: return L"ECONA_NO_CONNECTION_VIA_MEDIA: Phone is connected but not via given Media";
    case ECONA_NO_CONNECTION_VIA_DEVID: return L"ECONA_NO_CONNECTION_VIA_DEVID: Phone is not connected with given DevID";
    case ECONA_INVALID_CONNECTION_TYPE: return L"ECONA_INVALID_CONNECTION_TYPE: Connection type was invalid";
    case ECONA_NOT_SUPPORTED_CONNECTION_TYPE: return L"ECONA_NOT_SUPPORTED_CONNECTION_TYPE: Device does not support connection type";
    case ECONA_CONNECTION_BUSY: return L"ECONA_CONNECTION_BUSY: Other application is reserved connection";
    case ECONA_CONNECTION_LOST: return L"ECONA_CONNECTION_LOST: Connection is lost to Device";
    case ECONA_CONNECTION_REMOVED: return L"ECONA_CONNECTION_REMOVED: Connection removed, other application has reserved connection.";
    case ECONA_CONNECTION_FAILED: return L"ECONA_CONNECTION_FAILED: Connection failed, unknown reason";
    case ECONA_SUSPEND: return L"ECONA_SUSPEND: Connection removed, PC goes suspend state";
    case ECONA_NAME_ALREADY_EXISTS: return L"ECONA_NAME_ALREADY_EXISTS: Friendly name already exist";
    case ECONA_MEDIA_IS_NOT_WORKING: return L"ECONA_MEDIA_IS_NOT_WORKING: Target media is active but it is not working (e.g. BT-hardware stopped or removed)";
    case ECONA_CACHE_IS_NOT_AVAILABLE: return L"ECONA_CACHE_IS_NOT_AVAILABLE: Cache is not available (CONASearchDevices)";
    case ECONA_MEDIA_IS_NOT_ACTIVE: return L"ECONA_MEDIA_IS_NOT_ACTIVE: Target media is active (or ready yet)";
	case ECONA_PORT_OPEN_FAILED: return L"ECONA_PORT_OPEN_FAILED: Cannot open the changed COM port.";

	// Device pairing releated errors:
    case ECONA_DEVICE_PAIRING_FAILED: return L"ECONA_DEVICE_PAIRING_FAILED: Pairing failed.";
    case ECONA_DEVICE_PASSWORD_WRONG: return L"ECONA_DEVICE_PASSWORD_WRONG: Wrong password on device.";
    case ECONA_DEVICE_PASSWORD_INVALID: return L"ECONA_DEVICE_PASSWORD_INVALID: Password includes invalid characters or missing.";

	// File System errors:
    case ECONA_ALL_LISTED: return L"ECONA_ALL_LISTED: All items are listed";
    case ECONA_MEMORY_FULL: return L"ECONA_MEMORY_FULL: Device memory full";

	// File System errors for file functions:
    case ECONA_FILE_NAME_INVALID: return L"ECONA_FILE_NAME_INVALID: File name includes invalid characters in Device or PC";
    case ECONA_FILE_NAME_TOO_LONG: return L"ECONA_FILE_NAME_TOO_LONG: File name includes too many characters in Device or PC";
    case ECONA_FILE_ALREADY_EXIST: return L"ECONA_FILE_ALREADY_EXIST: File already exists in Device or PC";
    case ECONA_FILE_NOT_FOUND: return L"ECONA_FILE_NOT_FOUND: File does not exist in Device or PC";
    case ECONA_FILE_NO_PERMISSION: return L"ECONA_FILE_NO_PERMISSION: Not allowed to perform required operation to file in Device or PC";
    case ECONA_FILE_COPYRIGHT_PROTECTED: return L"ECONA_FILE_COPYRIGHT_PROTECTED: Not allowed to perform required operation to file in Device or PC";
    case ECONA_FILE_BUSY: return L"ECONA_FILE_BUSY: Other application has reserved file in Device or PC";
    case ECONA_FILE_TOO_BIG_DEVICE: return L"ECONA_FILE_TOO_BIG_DEVICE: Device rejected the operation because file size is too big";
    case ECONA_FILE_TYPE_NOT_SUPPORTED: return L"ECONA_FILE_TYPE_NOT_SUPPORTED: Device rejected the operation because file is unsupported type";
    case ECONA_FILE_NO_PERMISSION_ON_PC: return L"ECONA_FILE_NO_PERMISSION_ON_PC: ECONA_FILE_NO_PERMISSION_ON_PC";
	case ECONA_FILE_EXIST: return L"ECONA_FILE_EXIST: File operation to target path succeed but removing the source file failed";
	case ECONA_FILE_CONTENT_NOT_FOUND: return L"ECONA_FILE_CONTENT_NOT_FOUND: Specified file content is not found (e.g. unknown file section or stored position)";
	case ECONA_FILE_OLD_FORMAT: return L"ECONA_FILE_OLD_FORMAT: Specified file content supports old engine";
	case ECONA_FILE_INVALID_DATA: return L"ECONA_FILE_INVALID_DATA: Specified file data is invalid";

	// File System errors for folder functions:
    case ECONA_INVALID_DATA_DEVICE: return L"ECONA_INVALID_DATA_DEVICE: Device's folder contains invalid data";
    case ECONA_CURRENT_FOLDER_NOT_FOUND: return L"ECONA_CURRENT_FOLDER_NOT_FOUND: Current folder is invalid in device (e.g MMC card removed).";
    case ECONA_FOLDER_PATH_TOO_LONG: return L"ECONA_FOLDER_PATH_TOO_LONG: Current folder max unicode charaters count is limited to 255.";
    case ECONA_FOLDER_NAME_INVALID: return L"ECONA_FOLDER_NAME_INVALID: Folder name includes invalid characters in Device or PC";
    case ECONA_FOLDER_ALREADY_EXIST: return L"ECONA_FOLDER_ALREADY_EXIST: Folder already exists in target folder";
    case ECONA_FOLDER_NOT_FOUND: return L"ECONA_FOLDER_NOT_FOUND: Folder does not exist in target folder";
    case ECONA_FOLDER_NO_PERMISSION: return L"ECONA_FOLDER_NO_PERMISSION: Not allowed to perform required operation to folder in Device";
    case ECONA_FOLDER_NOT_EMPTY: return L"ECONA_FOLDER_NOT_EMPTY: Not allowed to perform required operation because folder is not empty";
    case ECONA_FOLDER_NO_PERMISSION_ON_PC: return L"ECONA_FOLDER_NO_PERMISSION_ON_PC: Not allowed to perform required operation to folder in PC";

	// Application Installation:
    case ECONA_DEVICE_INSTALLER_BUSY: return L"ECONA_DEVICE_INSTALLER_BUSY: Can not start Device's installer";

	// Syncronization specific error codes:
	case ECONA_UI_NOT_IDLE_DEVICE: return L"ECONA_UI_NOT_IDLE_DEVICE: Device rejected the operation. Device's UI might have been not in IDLE-state.";
	case ECONA_SYNC_CLIENT_BUSY_DEVICE: return L"ECONA_SYNC_CLIENT_BUSY_DEVICE: Device's SA sync client is busy.";
	case ECONA_UNAUTHORIZED_DEVICE: return L"ECONA_UNAUTHORIZED_DEVICE: Device rejected the operation. No permission.";
	case ECONA_DATABASE_LOCKED_DEVICE: return L"ECONA_DATABASE_LOCKED_DEVICE: Device rejected the operation. Device is locked.";
	case ECONA_SETTINGS_NOT_OK_DEVICE: return L"ECONA_SETTINGS_NOT_OK_DEVICE: Device rejected the operation. Sync profile settings might be wrong on Device.";
	case ECONA_SYNC_ITEM_TOO_BIG: return L"ECONA_SYNC_ITEM_TOO_BIG: Device rejected the operation"; 
	case ECONA_SYNC_ITEM_REJECT: return L"ECONA_SYNC_ITEM_REJECT: Device rejected the operation";
	case ECONA_SYNC_INSTALL_PLUGIN_FIRST: return L"ECONA_SYNC_INSTALL_PLUGIN_FIRST: Device rejected the operation";

	// Versit conversion specific error codes:
	case ECONA_VERSIT_INVALID_PARAM: return L"Invalid parameters passed to versit converter";
	case ECONA_VERSIT_UNKNOWN_TYPE: return L"Failed, trying to convert versit formats not supported in VersitConverter";
	case ECONA_VERSIT_INVALID_VERSIT_OBJECT: return L"Failed, validation of versit data not passed, contains invalid data";

	// Database specific error codes:
	case ECONA_DB_TRANSACTION_ALREADY_STARTED: return L"Another transaction is already in progress";
	case ECONA_DB_TRANSACTION_FAILED: return L"Some of operations within a transaction failed and transaction was rolled back";

	// Backup specific error codes:
	case ECONA_DEVICE_BATTERY_LEVEL_TOO_LOW: return L"Failed, device rejects the restore operation. Device's battery level is low.";
	case ECONA_DEVICE_BUSY: return L"Failed, device rejects the backup/resore operation. Device's backup server busy.";

    default: return L"Undefined error code"; // shouldn't occur
    }
}

//===================================================================
// ErrorMessageDlg --  Show an errormessage
//
//
//===================================================================
int ErrorMessageDlg(CString strError, DWORD dwError)
{
		CString str;
		str.Format(L"%s\n\nError: 0x%x\n%s", strError, dwError, CONAError2String(dwError));
		return AfxMessageBox(str);
}

//===================================================================
// ItemUidToString
// 
// Converts item uid to string for viewing it
// 
//===================================================================
void ItemUidToString(CA_ITEM_ID *pItemID, LPWSTR pstrUid)
{
	if(pItemID && pItemID->dwUidLen > 0)
	{
		int iLen = (3 * pItemID->dwUidLen) + 1;
		pstrUid = new WCHAR[iLen];
		memset(pstrUid, 0, iLen);
 		for(DWORD i = 0; i < pItemID->dwUidLen; i++)
		{
			_itow_s(pItemID->pbUid[i], &pstrUid[3 * i], 3, 10);
		}
		pstrUid[(3 * pItemID->dwUidLen)] = L'\0';
	}
}

//===================================================================
// GetTempImageFileName
// 
// Returns temporary image file name with path in pszTempFileName
// 
//===================================================================
void GetTempImageFileName(LPWSTR pszTempFileName, LPCWSTR pszExt)
{
	TCHAR szTempPath[_MAX_PATH];
	::GetTempPath(_MAX_PATH, szTempPath);
	::GetTempFileName(szTempPath, _T("PN"), 0, pszTempFileName);
	::DeleteFile(pszTempFileName); // ::GetTempFileName() creates this file
	::PathRemoveExtension(pszTempFileName);
	if(pszExt)
	{
		::PathAddExtension(pszTempFileName, pszExt);
	}
	else
	{
		::PathAddExtension(pszTempFileName, L".jpg");
	}
}

//===================================================================
// GetFormattedDate
// 
// Converts SYSTEMTIME to formatted date string
// 
//===================================================================
void GetFormattedDate(CONST SYSTEMTIME *lpDate, LPWSTR lpDateStr, int cchDate)
{
	if(GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, lpDate, NULL, lpDateStr, cchDate))
	{
		wcscat_s(lpDateStr, 128, _T(" "));
		GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, lpDate, NULL, &lpDateStr[wcslen(lpDateStr)], cchDate - (int)wcslen(lpDateStr));
	}
}

//===================================================================
// CADataDateToSystemTime
// 
// Converts CA_DATA_DATE to SYSTEMTIME
// 
//===================================================================
void CADataDateToSystemTime(CA_DATA_DATE& pimDate, SYSTEMTIME& sTime)
{
	sTime.wYear = pimDate.wYear;
	sTime.wMonth = pimDate.bMonth;
	sTime.wDay = pimDate.bDay;
	sTime.wHour = pimDate.bHour;
	sTime.wMinute = pimDate.bMinute;
	sTime.wSecond = pimDate.bSecond;
	sTime.wMilliseconds = 0;
}

//===================================================================
// GetCurrentCADate
// 
// Gets current local date/time in CA_DATA_DATE format
// 
//===================================================================
void GetCurrentCADate(CA_DATA_DATE& pimDate)
{
	SYSTEMTIME sTime;
	::GetLocalTime(&sTime);
	pimDate.dwSize = sizeof(CA_DATA_DATE);
	pimDate.wYear = sTime.wYear;
	pimDate.bMonth = (BYTE)sTime.wMonth;
	pimDate.bDay = (BYTE)sTime.wDay;
	pimDate.bHour = (BYTE)sTime.wHour;
	pimDate.bMinute = (BYTE)sTime.wMinute;
	pimDate.bSecond = (BYTE)sTime.wSecond;
}

//===================================================================
// CAFieldType2String
// 
// Converts PIM field type values to string
// 
//===================================================================
CString CAFieldType2String(CA_DATA_ITEM pimData)
{
    CString str(L"");
	switch(pimData.dwFieldType)
    {
    case CA_FIELD_TYPE_CONTACT_PI:
        str = L"PI"; // Personal information
        break;
    case CA_FIELD_TYPE_CONTACT_NUMBER:
        str = L"Number";
        break;
    case CA_FIELD_TYPE_CONTACT_ADDRESS:
        str = L"Address";
        break;
    case CA_FIELD_TYPE_CONTACT_GENERAL:
        str = L"";	// General information
        break;
    case CA_FIELD_TYPE_CALENDAR:
        str = L"";  // Calendar item
        break;
    default:
        str = L"Unknown field type"; // shouldn't occur
        break;
    }
    if(str.GetLength() > 0)
    {
        str += ", ";
    }
	switch(pimData.dwFieldSubType)
    {
	// Personal information
    case CA_FIELD_SUB_TYPE_NAME:
        str += L"name";
        break;
    case CA_FIELD_SUB_TYPE_FN:
        str += L"first name";
        break;
    case CA_FIELD_SUB_TYPE_MN:
        str += L"middle name";
        break;
    case CA_FIELD_SUB_TYPE_LN:
        str += L"last name";
        break;
    case CA_FIELD_SUB_TYPE_TITLE:
        str += L"title";
        break;
    case CA_FIELD_SUB_TYPE_SUFFIX:
        str += L"suffix";
        break;
    case CA_FIELD_SUB_TYPE_COMPANY:
        str += L"company";
        break;
    case CA_FIELD_SUB_TYPE_JOB_TITLE:
        str += L"job title";
        break;
    case CA_FIELD_SUB_TYPE_BIRTHDAY:
        str += L"birthday";
        break;
    case CA_FIELD_SUB_TYPE_PICTURE:
        str += L"picture";
        break;
	case CA_FIELD_SUB_TYPE_NICKNAME:
        str += L"nickname";
        break;
	case CA_FIELD_SUB_TYPE_FORMAL_NAME:
        str += L"formal name";
        break;
	case CA_FIELD_SUB_TYPE_GIVEN_NAME_PRONUNCIATION:
        str += L"given name pronunciation";
        break;
	case CA_FIELD_SUB_TYPE_FAMILY_NAME_PRONUNCIATION:
        str += L"family name pronunciation";
        break;
	case CA_FIELD_SUB_TYPE_COMPANY_NAME_PRONUNCIATION:
        str += L"company name pronunciation";
        break;
	// Numbers
	case CA_FIELD_SUB_TYPE_TEL:
        str += L"telephone";
        break;
	case CA_FIELD_SUB_TYPE_TEL_HOME:
        str += L"home phone";
        break;
	case CA_FIELD_SUB_TYPE_TEL_WORK:
        str += L"work phone";
        break;
	case CA_FIELD_SUB_TYPE_TEL_PREF:
        str += L"preferred";
        break;
	case CA_FIELD_SUB_TYPE_TEL_CAR:
        str += L"car";
        break;
	case CA_FIELD_SUB_TYPE_TEL_DATA:
        str += L"data";
        break;
	case CA_FIELD_SUB_TYPE_PAGER:
        str += L"pager";
        break;
	case CA_FIELD_SUB_TYPE_MOBILE:
        str += L"mobile";
        break;
	case CA_FIELD_SUB_TYPE_MOBILE_HOME:
        str += L"home mobile";
        break;
	case CA_FIELD_SUB_TYPE_MOBILE_WORK:
        str += L"work mobile";
        break;
	case CA_FIELD_SUB_TYPE_FAX:
        str += L"fax";
        break;
	case CA_FIELD_SUB_TYPE_FAX_HOME:
        str += L"home fax";
        break;
	case CA_FIELD_SUB_TYPE_FAX_WORK:
        str += L"work fax";
        break;
	case CA_FIELD_SUB_TYPE_VIDEO:
        str += L"video";
        break;
	case CA_FIELD_SUB_TYPE_VIDEO_HOME:
        str += L"home video";
        break;
	case CA_FIELD_SUB_TYPE_VIDEO_WORK:
        str += L"work video";
        break;
	case CA_FIELD_SUB_TYPE_VOIP:
        str += L"VOIP";
        break;
	case CA_FIELD_SUB_TYPE_VOIP_HOME:
        str += L"home VOIP";
        break;
	case CA_FIELD_SUB_TYPE_VOIP_WORK:
        str += L"work VOIP";
        break;
	// Addresses
    case CA_FIELD_SUB_TYPE_POSTAL:
        str += L"postal";
        break;
    case CA_FIELD_SUB_TYPE_POSTAL_BUSINESS:
        str += L"postal, business";
        break;
    case CA_FIELD_SUB_TYPE_POSTAL_PRIVATE:
        str += L"postal, private";
        break;
    case CA_FIELD_SUB_TYPE_EMAIL:
        str += L"email";
        break;
    case CA_FIELD_SUB_TYPE_EMAIL_HOME:
        str += L"home email";
        break;
    case CA_FIELD_SUB_TYPE_EMAIL_WORK:
        str += L"work email";
        break;
    case CA_FIELD_SUB_TYPE_WEB:
        str += L"web";
        break;
    case CA_FIELD_SUB_TYPE_WEB_HOME:
        str += L"home web";
        break;
    case CA_FIELD_SUB_TYPE_WEB_WORK:
        str += L"work web";
        break;
    case CA_FIELD_SUB_TYPE_PTT:
        str += L"PTT";
        break;
	case CA_FIELD_SUB_TYPE_SIP_FOR_VIDEO:
        str += L"SIP";
        break;
	case CA_FIELD_SUB_TYPE_SWIS:
        str += L"SWIS";
        break;
	// General fields
    case CA_FIELD_SUB_TYPE_NOTE:
        str += L"Note";
        break;
    case CA_FIELD_SUB_TYPE_DTMF:
        str += L"DTMF";
        break;
    case CA_FIELD_SUB_TYPE_UID:
        str += L"UID";
        break;
	case CA_FIELD_SUB_TYPE_WIRELESS_VILLAGE:
        str += L"wireless village";
        break;
	// Calendar sub types
    case CA_FIELD_SUB_TYPE_DESCRIPTION:
        str += L"Subject";
        break;
    case CA_FIELD_SUB_TYPE_LOCATION:
        str += L"Location";
        break;
    case CA_FIELD_SUB_TYPE_ITEM_DATA:
		if(pimData.dwFieldType == CA_CALENDAR_ITEM_MEETING)
		{
	        str += L"Details";
		}
		else if(pimData.dwFieldType == CA_CALENDAR_ITEM_CALL)
		{
	        str += L"Phone number";
		}
		else if(pimData.dwFieldType == CA_CALENDAR_ITEM_BIRTHDAY)
		{
	        str += L"Birth year";
		}
		else
		{
	        str += L"Item data";
		}
        break;
	case CA_FIELD_SUB_TYPE_TODO_PRIORITY:
        str += L"Priority";
        break;
	case CA_FIELD_SUB_TYPE_TODO_STATUS:
        str += L"Status";
        break;
    default:
        str += L"unknown field sub type"; // shouldn't occur
        break;
    }
    return str;
}

//===================================================================
// CalendarItemType2String
// 
// Converts calendar item type values to string
// 
//===================================================================
CString CalendarItemType2String(DWORD dwInfoField)
{
	switch(dwInfoField)
    {
	case CA_CALENDAR_ITEM_MEETING:    return L"Meeting";
	case CA_CALENDAR_ITEM_CALL:       return L"Call";
	case CA_CALENDAR_ITEM_BIRTHDAY:   return L"Birthday";
	case CA_CALENDAR_ITEM_MEMO:       return L"Memo";
	case CA_CALENDAR_ITEM_REMINDER:   return L"Reminder";
	case CA_CALENDAR_ITEM_NOTE:       return L"Note";
	case CA_CALENDAR_ITEM_TODO:       return L"To do";
    default:    return L"Unknown";
    }
}

//===================================================================
// RecurrenceToString
// 
// Converts recurrence values to string
// 
//===================================================================
CString RecurrenceToString(DWORD dwInfo)
{
	DWORD dwRec = CA_GET_RECURRENCE(dwInfo);
	switch(dwRec)
    {
	case CA_CALENDAR_RECURRENCE_NONE:       return L"None";
	case CA_CALENDAR_RECURRENCE_DAILY:      return L"Daily";
	case CA_CALENDAR_RECURRENCE_WEEKLY:     return L"Weekly";
	case CA_CALENDAR_RECURRENCE_MONTHLY:    return L"Monthly";
	case CA_CALENDAR_RECURRENCE_YEARLY:     return L"Yearly";
    default:    return L"Unknown";
    }
}

//===================================================================
// TodoPriorityToString
// 
// Converts to do priority values to string
// 
//===================================================================
CString TodoPriorityToString(DWORD dwInfo)
{
	switch(dwInfo)
    {
	case CA_CALENDAR_TODO_PRIORITY_HIGH:    return L"High";
	case CA_CALENDAR_TODO_PRIORITY_NORMAL:  return L"Normal";
	case CA_CALENDAR_TODO_PRIORITY_LOW:     return L"Low";
    default:    return L"Unknown";
    }
}

//===================================================================
// TodoStatusToString
// 
// Converts to do status values to string
// 
//===================================================================
CString TodoStatusToString(DWORD dwInfo)
{
	switch(dwInfo)
    {
	case CA_CALENDAR_TODO_STATUS_NEEDS_ACTION: return L"Needs action";
	case CA_CALENDAR_TODO_STATUS_COMPLETED:    return L"Completed";
    default:    return L"Unknown";
    }
}

//===================================================================
// AlarmStateToString
// 
// Converts alarm state values to string
// 
//===================================================================
CString AlarmStateToString(DWORD dwInfo)
{
	switch(dwInfo)
    {
	case CA_CALENDAR_ALARM_NOT_SET:   return L"Not set";
	case CA_CALENDAR_ALARM_SILENT:    return L"Silent";
	case CA_CALENDAR_ALARM_WITH_TONE: return L"With tone";
    default:    return L"Unknown";
    }
}

//===================================================================
// MessageStatusToString
// 
// Converts message status values to string
// 
//===================================================================
CString MessageStatusToString(DWORD dwInfo)
{
    DWORD dwStatus = CA_GET_MESSAGE_STATUS(dwInfo);
	switch(dwStatus)
    {
	case CA_MESSAGE_STATUS_SENT:       return L"Sent";
	case CA_MESSAGE_STATUS_UNREAD:     return L"Hasn't been read";
	case CA_MESSAGE_STATUS_READ:       return L"Has been read";
	case CA_MESSAGE_STATUS_DRAFT:      return L"Draft";
	case CA_MESSAGE_STATUS_PENDING:	   return L"Pending";
	case CA_MESSAGE_STATUS_DELIVERED:  return L"Delivered";
	case CA_MESSAGE_STATUS_SENDING:    return L"Being sent";
    default:    return L"Unknown";
    }
}

