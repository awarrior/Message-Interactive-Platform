/*
==============================================================================
* Common Functions API 1.0
*
Filename    : CommonFunctionsAPI.h
Description : Common Functions API definition
Version     : 1.0

Copyright (c) 2006, 2007 Nokia Corporation.
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
#ifndef _COMMONFUNCTIONSAPI_H_
#define _COMMONFUNCTIONSAPI_H_

///////////////////////////////////////////////////////////
// PC Connectivity API
///////////////////////////////////////////////////////////
	#ifndef PCCS_CFAPI
		#define PCCS_CFAPI DWORD STDAPICALLTYPE
		#define PCCS_CFAPI_EX(type) type STDAPICALLTYPE
	#endif
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// PC Connectivity API type definitions
///////////////////////////////////////////////////////////
	#include "PCCSTypeDefinitions.h"
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Common Functions API 
///////////////////////////////////////////////////////////

	//=========================================================
	// CFAPI version
	//
		#define CFAPI_VERSION_10		10
	//=========================================================

	//=========================================================
	// CFAPI_Initialize
	//
	// Description:
	//	CFAPI_Initialize initializes the API. This must be called once and before any other CFAPI call!
	//  It's not allowed to call this function like this 
	//		CFAPI_Initialize(CFAPI_GetAPIVersion(), NULL);
	//	You must call it like this
	//		CFAPI_Initialize(CFAPI_VERSION_10, NULL);
	//
	// Parameters:
	//	dwAPIVersion	[in] CFAPI version requested.
	//	pdwParam		[in] Reserved for future use. Must be NULL.
	//
	// Return values:
	//
		PCCS_CFAPI CFAPI_Initialize(DWORD dwAPIVersion, DWORD* pdwParam);
	//=========================================================

	//=========================================================
	// CFAPI_Terminate
	//
	// Description:
	//	CFAPI_Terminate terminates the API. This must be called once and as the last CFAPI call!
	//
	// Parameters:
	//	pdwParam		[in] Reserved for future use. Must be NULL.
	//
	// Return values:
	//
		PCCS_CFAPI CFAPI_Terminate(DWORD* pdwParam);
	//=========================================================

	//=========================================================
	// CFAPI_GetAPIVersion
	//
	// Description:
	//	Returns currently installed version of this CFAPI. 	
	//
	// Parameters:
	//
	// Return values:
	//	API version number. 
	//
		PCCS_CFAPI CFAPI_GetAPIVersion();
	//=========================================================

	//=========================================================
	// CONAAllocString
	//
	// Description:
	//	CONAAllocString allocates an unicode string from unicode input.
	//	Allocated string must be freed by using CONAFreeString. 
	//
	// Parameters:
	//	pstrString			[in] Pointer to a NULL terminated 
	//                       Unicode string for copying.
	//
	// Return values:
	//	LPAPICHAR - allocated memory pointer. Returns NULL if it fails.
	//
		PCCS_CFAPI_EX(LPAPIWCHAR) CONAAllocString( LPAPIWCHAR pstrString );
	//=========================================================

	//=========================================================
	// CONAAllocStringMB
	//
	// Description:
	//	CONAAllocStringMB allocates an unicode string from multibyte input.
	//	Allocated string must be freed by using CONAFreeString. 
	//
	// Parameters:
	//	pstrString			[in] Pointer to a NULL terminated multibyte string for copying.
	//
	// Return values:
	//	LPAPICHAR - allocated memory pointer. Returns NULL if it fails.
	//
		PCCS_CFAPI_EX(LPAPIWCHAR) CONAAllocStringMB( const char* pstrString );
	//=========================================================

	//=========================================================
	// CONAFreeString
	//
	// Description:
	//	CONAFreeString frees the allocated string.
	//
	// Parameters:
	//	pstrString			[in] Pointer to a string allocated with CONAAllocString.
	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_POINTER
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_CFAPI CONAFreeString( LPAPIWCHAR pstrString );
	//=========================================================

	//=========================================================
	// CONAAllocMemory
	//
	// Description:
	//	CONAAllocMemory allocates requested amount of memory.
	//	Buffer must be freed using the CONAFreeMemory.
	//
	// Parameters:
	//	dwSize			[in] Amount of memory to be allocated in bytes.
	//
	// Return values:
	//	NULL if allocation fails
	//
		PCCS_CFAPI_EX(LPVOID) CONAAllocMemory( DWORD dwSize );
	//=========================================================

	//=========================================================
	// CONAFreeMemory
	//
	// Description:
	//	CONAFreeMemory frees the memory block allocated with CONAAllocMemory.
	//
	// Parameters:
	//	pBuffer			[in] Pointer to the buffer to be freed
	//	dwSize			[in] Size of the buffer. Can be zero if memory area is
	//						 not wanted to be initialized with zeros (default).
	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_POINTER
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_CFAPI CONAFreeMemory( LPVOID pBuffer, DWORD dwSize = 0 );
	//=========================================================

///////////////////////////////////////////////////////////

#endif

