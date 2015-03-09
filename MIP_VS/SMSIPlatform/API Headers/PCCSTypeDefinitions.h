/*
==============================================================================
* PC Connectivity API 3.0
*
Filename    : PCCSTypeDefinitions.h
Description : PC Connectivity Solution Type Definitions for all APIs
Version     : 3.0

Copyright (c) 2006 Nokia Corporation.
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
#ifndef __PCCSTYPEDEFINITIONS_H__
#define __PCCSTYPEDEFINITIONS_H__
#pragma once

// Type definition for INT8
#ifndef INT8
	typedef signed char				INT8;
#endif
#ifndef UINT8
	typedef unsigned char			UINT8;
#endif

// Type definition for INT16
#ifndef INT16
	typedef signed short			INT16;
#endif
#ifndef UINT16
	typedef unsigned short			UINT16;
#endif

// Type definition for INT32
#ifndef INT32
	typedef signed int				INT32;
#endif
#ifndef UINT32
	typedef unsigned int			UINT32;
#endif

// Type definition for INT64 (64 bit integer)
#ifndef INT64
	typedef signed __int64   		INT64;
#endif
#ifndef UINT64
	typedef unsigned __int64		UINT64;
#endif

// Type definition for LPVOID
#ifndef LPVOID
	typedef void* 					LPVOID;
#endif

// Type definition for BYTE
#ifndef BYTE
	typedef unsigned char			BYTE;
#endif

// Type definition for WORD
#ifndef WORD
	typedef unsigned short			WORD;
#endif

// Type definition for DWORD
#ifndef DWORD
	typedef unsigned long			DWORD;
#endif

// Type definition for BOOL
#ifndef BOOL
	typedef int						BOOL;
#endif

// Type definition for APIWCHAR
#ifndef APIWCHAR
	typedef unsigned short			APIWCHAR;
	typedef APIWCHAR*				LPAPIWCHAR;
	typedef const APIWCHAR*			LPCAPIWCHAR;
#endif

// Type definition for APIHANDLE
#ifndef APIHANDLE
	typedef LPVOID 					APIHANDLE;
#endif

// Values used in API notification registeration methods
#ifndef API_REGISTER
	#define	API_REGISTER			0x10
#endif
#ifndef API_UNREGISTER
	#define	API_UNREGISTER			0x20
#endif

// Media types used in APIs 
#define	API_MEDIA_ALL				0x01
#define	API_MEDIA_IRDA				0x02
#define	API_MEDIA_SERIAL			0x04
#define	API_MEDIA_BLUETOOTH			0x08
#define	API_MEDIA_USB				0x10

// Type definition for API_DATE_TIME
#ifndef API_DATE_TIME
	typedef struct
	{
		DWORD	dwSize;			// Size of the structure. Must be sizeof(API_DATE_TIME).
		WORD	wYear;
		BYTE	bMonth;
		BYTE	bDay;
		BYTE	bHour;
		BYTE	bMinute;
		BYTE	bSecond;
		long	lTimeZoneBias; 	// Time zone bias in minutes (+120 for GMT+0200)
		long	lDaylightBias;	// Daylight bias
	} API_DATE_TIME;
#endif

// Type definition for API_VARIANT
#ifndef API_VARIANT
	#define API_VARIANT_TYPE_BYTE			0x00000001
	#define API_VARIANT_TYPE_WORD			0x00000002
	#define API_VARIANT_TYPE_DWORD			0x00000004
	#define API_VARIANT_TYPE_INT64			0x00000008
	#define API_VARIANT_TYPE_DOUBLE			0x00000010
	#define API_VARIANT_TYPE_LPBYTE			0x00000100
	#define API_VARIANT_TYPE_LPWORD			0x00000200
	#define API_VARIANT_TYPE_LPDWORD		0x00000400
	#define API_VARIANT_TYPE_LPVOID			0x00000800
	#define API_VARIANT_TYPE_LPAPIWCHAR		0x00010000
	#define API_VARIANT_TYPE_DATETIME		0x01000000
	typedef struct
	{
		DWORD	dwSize;				// Size of the structure. Must be sizeof(API_VARIANT).
		DWORD	dwType;				// Data type
		DWORD	dwTypeInfo;			// Data type specific info parameter
		union APIDATATYPE
		{
			BYTE			bValue;
			WORD			wValue;
			DWORD			dwValue;
			INT64			i64Value;
			double			dValue;
			BYTE*			pbBuffer;
			WORD*			pwBuffer;
			DWORD*			pdwBuffer;
			INT64*			pi64Buffer;
			LPVOID			pBuffer;
			LPAPIWCHAR		pstrValue;
			API_DATE_TIME	DateTime;
		} VarData;
	} API_VARIANT;
#endif

// Type definition for _FILETIME
#if !defined(_WINBASE_) && !defined(_FILETIME_)
	#pragma message ("\tConnAPI.h header included before Windows headers!") 
	#pragma message ("\tNote: If Windows headers will be used those MUST BE included before ConnAPI.h!")
	typedef struct _FILETIME {
		DWORD dwLowDateTime;
		DWORD dwHighDateTime;
	} FILETIME, *PFILETIME, *LPFILETIME;
	#define _FILETIME_
#endif

// Type definition for CALLBACK
#ifndef CALLBACK
	#define CALLBACK		__stdcall
#endif

// Type definition for EXTERN_C
#ifndef EXTERN_C
	#ifdef __cplusplus
		#define EXTERN_C    extern "C"
	#else
		#define EXTERN_C    extern
	#endif
#endif

// Type definition for STDAPICALLTYPE
#ifndef STDAPICALLTYPE
	#define STDAPICALLTYPE	__stdcall
#endif

// Type definition for PCCS API
#ifndef PCCSAPI
	#define PCCSAPI DWORD STDAPICALLTYPE
	#define PCCSAPI_EX(type) type STDAPICALLTYPE
#endif

#ifndef GUID_DEFINED
	#define GUID_DEFINED
	typedef struct _GUID {
	  unsigned long  Data1;
	  unsigned short Data2;
	  unsigned short Data3;
	  unsigned char  Data4[ 8 ];
	} GUID;
#endif

#endif //__PCCSTYPEDEFINITIONS_H__
