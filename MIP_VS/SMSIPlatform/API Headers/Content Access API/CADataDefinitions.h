/*
==============================================================================
* CA Data Definitions 
*
*  Copyright (c) 2006-2007 Nokia Corporation.
*  This material, including documentation and any related 
*  computer programs, is protected by copyright controlled by 
*  Nokia Corporation. All rights are reserved. Copying, 
*  including reproducing, storing, adapting or translating, any 
*  or all of this material requires the prior written consent of 
*  Nokia Corporation. This material also contains confidential 
*  information which may not be disclosed to others without the 
*  prior written consent of Nokia Corporation.
* 
==============================================================================
*/
#ifndef __CADATADEFINITIONS_7347326__
#define __CADATADEFINITIONS_7347326__

	////////////////////////////////////////////////////
	// Available CA connection targets
	//
	// Info: 
	// CA connection targets
	// 
	#define CA_TARGET_CONTACTS					0x0001
	#define CA_TARGET_CALENDAR					0x0002
	#define CA_TARGET_NOTES						0x0003
	#define CA_TARGET_SMS_MESSAGES				0x0004
	#define CA_TARGET_MMS_MESSAGES				0x0005
	#define CA_TARGET_BOOKMARKS					0x0006

	////////////////////////////////////////////////////
	// Available data formats 
	//
	// Info: 
	// Data formats. 	
	// 
	#define CA_DATA_FORMAT_STRUCT				0x0001
	#define CA_DATA_FORMAT_VERSIT				0x0002

	////////////////////////////////////////////////////
	//
	// Info
	// Macros for data handling in CA_DATA_MSG 
	//
	#define CA_GET_DATA_FORMAT(Info)			Info & 0x0F
	#define CA_GET_DATA_CODING(Info)			Info & 0xF0  
	#define CA_GET_MESSAGE_STATUS(Info)			(Info & 0x0F00) >> 8
	#define CA_GET_MESSAGE_TYPE(Info)			(Info & 0xF000) >> 12
	
	#define CA_SET_DATA_FORMAT(Info,Value)		Info|= (Value & 0x0F)
	#define CA_SET_DATA_CODING(Info,Value)		Info|= (Value & 0xF0)
	#define CA_SET_MESSAGE_STATUS(Info,Value)	Info|= ((Value & 0x0F) << 8)
	#define CA_SET_MESSAGE_TYPE(Info,Value)		Info|= ((Value & 0x0F) << 12)

	////////////////////////////////////////////////////
	//
	// Info
	// Macros for data dwRecurrence in CA_DATA_CALENDAR
	//
	#define CA_GET_RECURRENCE(Info)						Info & 0xFF
	#define CA_SET_RECURRENCE(Info,Value)				Info|= Value

	#define CA_GET_RECURRENCE_INTERVAL(Info)			(Info >>8) & 0xFF
	#define CA_SET_RECURRENCE_INTERVAL(Info,Value)		Info|= (Value << 8)
			
	////////////////////////////////////////////////////
	// Input data format definitions
	//
	// Info: 
	// This format is used in CA_DATA_MSG structure's 
	// dwInfoField parameter to tell the format of the data
	// in data buffer
	//
	// Also used in CA_DATA_VERSIT structure's dwInfoField	
	// to inform about format of the data in structure
	// 
	#define	CA_DATA_FORMAT_UNICODE				0x01
	#define	CA_DATA_FORMAT_DATA					0x02	

	////////////////////////////////////////////////////
	// Input data coding definitions 
	//
	// Info: 
	// This format is used in CA_DATA_MSG structure 
	// to inform about the coding of the data 
	// 
	#define	CA_DATA_CODING_7BIT					0x10
	#define	CA_DATA_CODING_8BIT					0x20
	#define	CA_DATA_CODING_UNICODE				0x40

	////////////////////////////////////////////////////
	// Message type definitions
	//
	// Info: 
	// Message type definitions 
	// 
	#define CA_SMS_DELIVER						0x01        
	#define CA_SMS_SUBMIT						0x03        

	////////////////////////////////////////////////////
	// Message folder definitions
	//
	// Info: 
	// Message folder definitions. 
	// 
	// Folder ID definitions 
	#define CA_MESSAGE_FOLDER_INBOX				0x01
	#define CA_MESSAGE_FOLDER_OUTBOX			0x02
	#define CA_MESSAGE_FOLDER_SENT				0x03
	#define CA_MESSAGE_FOLDER_ARCHIVE			0x04
	#define CA_MESSAGE_FOLDER_DRAFTS			0x05
	#define CA_MESSAGE_FOLDER_TEMPLATES			0x06 
	#define CA_MESSAGE_FOLDER_USER_FOLDERS		0x10
	
	////////////////////////////////////////////////////
	// Phonebook memory definitions 
	//
	// Info: 
	// Phonebook memory definitions 
	// 
	// Folder ID definitions 
	#define CA_PHONEBOOK_MEMORY_PHONE			0x01
	#define CA_PHONEBOOK_MEMORY_SIM				0x02
		
	////////////////////////////////////////////////////
	// Message status definitions ..
	//
	// Info: 
	// SMS Message status definitions .. 
	// 
	#define CA_MESSAGE_STATUS_SENT				0x02	// Message has been sent 
	#define CA_MESSAGE_STATUS_UNREAD			0x04	// Message hasn't been read 
	#define CA_MESSAGE_STATUS_READ				0x05	// Message has been read 
	#define CA_MESSAGE_STATUS_DRAFT				0x06	// Message is a draft 
	#define CA_MESSAGE_STATUS_PENDING			0x07	// Message is pending 
	#define CA_MESSAGE_STATUS_DELIVERED			0x09	// Message has been delivered 
	#define CA_MESSAGE_STATUS_SENDING			0x0C	// Message is being sent

	////////////////////////////////////////////////////
	// Address type definitions 
	//
	// Info: 
	// Type of address passed in address structure
	// 
	#define CA_MSG_ADDRESS_TYPE_EMAIL			0x01
	#define CA_MSG_ADDRESS_TYPE_NUMBER			0x02	

	////////////////////////////////////////////////////
	// Calendar item type definitions 
	//
	// Info: 
	// Defines different calendar items. Used in CA_DATA_CALENDAR in dwInfoField.
	// 
	#define CA_CALENDAR_ITEM_MEETING			0x01
	#define CA_CALENDAR_ITEM_CALL				0x02
	#define CA_CALENDAR_ITEM_BIRTHDAY			0x03
	#define CA_CALENDAR_ITEM_MEMO				0x04
	#define CA_CALENDAR_ITEM_REMINDER			0x05
	#define CA_CALENDAR_ITEM_NOTE				0x06
	#define CA_CALENDAR_ITEM_TODO				0x07

	//////////////////////////////////////////////////////
	// Field type definitions 
	//
	// Info: 
	// Field type values for data items
	//	
	// For contacts
	#define	CA_FIELD_TYPE_CONTACT_PI			0x0001	// Personal information
	#define	CA_FIELD_TYPE_CONTACT_NUMBER		0x0002	// Number information
	#define	CA_FIELD_TYPE_CONTACT_ADDRESS		0x0003	// Address information
	#define	CA_FIELD_TYPE_CONTACT_GENERAL		0x0004	// General information
	// For calendar
	#define	CA_FIELD_TYPE_CALENDAR				0x0010	// Calendar item

	//////////////////////////////////////////////////////
	// Recurrence values for calendar items 
	//
	// Info: 
	// Recurrence values to be used in calendar interfaces
	//		
	#define CA_CALENDAR_RECURRENCE_NONE			0x00
	#define CA_CALENDAR_RECURRENCE_DAILY		0x01
	#define	CA_CALENDAR_RECURRENCE_WEEKLY		0x02
	#define CA_CALENDAR_RECURRENCE_MONTHLY		0x03
	#define CA_CALENDAR_RECURRENCE_YEARLY		0x04

	
	//////////////////////////////////////////////////////
	// Calendar TODO item priority values 
	//
	// Info: 
	//
	#define CA_CALENDAR_TODO_PRIORITY_HIGH		0x01
	#define CA_CALENDAR_TODO_PRIORITY_NORMAL    0x02
	#define CA_CALENDAR_TODO_PRIORITY_LOW       0x03

	////////////////////////////////////////////////////
	// Calendar TODO item status definitions
	//
	// Info: 
	//
	#define CA_CALENDAR_TODO_STATUS_NEEDS_ACTION    0x01
	#define CA_CALENDAR_TODO_STATUS_COMPLETED       0x02

	////////////////////////////////////////////////////
	// Calendar Alarm State value definitions
	//
	// Info: 
	// CA_CALENDAR_ALARM_SILENT value is not supported 
	// in S60 devices.
	//
	#define CA_CALENDAR_ALARM_NOT_SET				0x01
	#define CA_CALENDAR_ALARM_SILENT			    0x02
	#define CA_CALENDAR_ALARM_WITH_TONE				0x03

	////////////////////////////////////////////////////
	// Field sub type definitions 
	//
	// Info: 
	//	Field sub type definitions for data items.
	//																___Description__________|___Valid CA_DATA_ITEM member_______	
	// Personal information											|						|									|
	#define CA_FIELD_SUB_TYPE_NAME							0x0001	// Name field			|   pstrText
	#define	CA_FIELD_SUB_TYPE_FN							0x0002	// First name			|	pstrText
	#define	CA_FIELD_SUB_TYPE_MN							0x0003	// Midle name			|	pstrText
	#define	CA_FIELD_SUB_TYPE_LN							0x0004	// Last name			|	pstrText
	#define	CA_FIELD_SUB_TYPE_TITLE							0x0005	// Title				|	pstrText
	#define	CA_FIELD_SUB_TYPE_SUFFIX						0x0006	// Suffix				|	pstrText
	#define	CA_FIELD_SUB_TYPE_COMPANY						0x0007	// Company				|	pstrText
	#define	CA_FIELD_SUB_TYPE_JOB_TITLE						0x0008	// Job title			|	pstrText
	#define	CA_FIELD_SUB_TYPE_BIRTHDAY						0x0009	// Birthday				|	pCustomData as CA_DATA_DATE
	#define	CA_FIELD_SUB_TYPE_PICTURE						0x000A	// Picture				|	pCustomData as CA_DATA_PICTURE
	#define	CA_FIELD_SUB_TYPE_NICKNAME						0x000B	// Nickname				|	pstrText
	#define	CA_FIELD_SUB_TYPE_FORMAL_NAME					0x000C	// Formal name			|	pstrText
	#define CA_FIELD_SUB_TYPE_GIVEN_NAME_PRONUNCIATION		0x000D	// Pronunciation field	|	pstrText
	#define CA_FIELD_SUB_TYPE_FAMILY_NAME_PRONUNCIATION		0x000E	// Pronunciation field	|	pstrText
	#define CA_FIELD_SUB_TYPE_COMPANY_NAME_PRONUNCIATION	0x000F	// Pronunciation field	|	pstrText
	// Numbers
	#define	CA_FIELD_SUB_TYPE_TEL							0x0020	// Telephone			|	pstrText
	#define	CA_FIELD_SUB_TYPE_TEL_HOME						0x0021	// Home number			|	pstrText
	#define	CA_FIELD_SUB_TYPE_TEL_WORK						0x0022	// Work number			|	pstrText
	#define	CA_FIELD_SUB_TYPE_TEL_PREF						0x0023	// Preferred number		|	pstrText
	#define	CA_FIELD_SUB_TYPE_TEL_CAR						0x0024	// Car number			|	pstrText
	#define	CA_FIELD_SUB_TYPE_TEL_DATA						0x0025	// Data number			|	pstrText
	#define	CA_FIELD_SUB_TYPE_PAGER							0x0030	// Pager				|	pstrText	
	#define	CA_FIELD_SUB_TYPE_MOBILE						0x0040	// Mobile				|	pstrText
	#define CA_FIELD_SUB_TYPE_MOBILE_HOME					0x0041	// Mobile				|   pstrText
	#define CA_FIELD_SUB_TYPE_MOBILE_WORK					0x0042	// Mobile				|   pstrText	
	#define	CA_FIELD_SUB_TYPE_FAX							0x0050	// Fax					|	pstrText
	#define CA_FIELD_SUB_TYPE_FAX_HOME						0x0051	// Fax					|   pstrText
	#define CA_FIELD_SUB_TYPE_FAX_WORK						0x0052	// Fax					|   pstrText
	#define CA_FIELD_SUB_TYPE_VIDEO							0x0060	// Video call number	|   pstrText
	#define CA_FIELD_SUB_TYPE_VIDEO_HOME					0x0061	// Video call number	|   pstrText
	#define CA_FIELD_SUB_TYPE_VIDEO_WORK					0x0062	// Video call number	|   pstrText
	#define CA_FIELD_SUB_TYPE_VOIP							0x0070	// Voice Over IP		|   pstrText
	#define CA_FIELD_SUB_TYPE_VOIP_HOME						0x0071	// Voice Over IP		|   pstrText
	#define CA_FIELD_SUB_TYPE_VOIP_WORK						0x0072	// Voice Over IP		|   pstrText

	// Addresses
	#define	CA_FIELD_SUB_TYPE_POSTAL						0x0100	// Postal address		|	pCustomData as CA_DATA_POSTAL_ADDRESS
	#define	CA_FIELD_SUB_TYPE_POSTAL_BUSINESS				0x0101	// Business address		|	pCustomData as CA_DATA_POSTAL_ADDRESS
	#define	CA_FIELD_SUB_TYPE_POSTAL_PRIVATE				0x0102	// Private address		|	pCustomData as CA_DATA_POSTAL_ADDRESS
	#define	CA_FIELD_SUB_TYPE_EMAIL							0x0103	// Email address		|	pstrText
	#define	CA_FIELD_SUB_TYPE_EMAIL_HOME					0x0104	// Email address		|	pstrText
	#define	CA_FIELD_SUB_TYPE_EMAIL_WORK					0x0105	// Email address		|	pstrText
	#define	CA_FIELD_SUB_TYPE_WEB							0x0110	// Web address			|	pstrText
	#define	CA_FIELD_SUB_TYPE_WEB_HOME						0x0111	// Web address			|	pstrText
	#define	CA_FIELD_SUB_TYPE_WEB_WORK						0x0112	// Web address			|	pstrText	
	#define	CA_FIELD_SUB_TYPE_PTT							0x0120	// PTT address			|	pstrText
	#define	CA_FIELD_SUB_TYPE_SIP_FOR_VIDEO					0x0121	// SIP for Video sharing|	pstrText
	#define CA_FIELD_SUB_TYPE_SWIS							0x0130	// SWIS					|   pstrText

	// General fields
	#define	CA_FIELD_SUB_TYPE_NOTE							0x0200	// Note field			|	pstrText
	#define	CA_FIELD_SUB_TYPE_DTMF							0x0201	// DTMF field			|	pstrText
	#define	CA_FIELD_SUB_TYPE_UID							0x0202	// UID field			|	pstrText
	#define CA_FIELD_SUB_TYPE_WIRELESS_VILLAGE				0x0203	// Village user ID		|	pstrText

	// Calandar sub types
	#define	CA_FIELD_SUB_TYPE_DESCRIPTION					0x0300	// Description			|	pstrText
	#define	CA_FIELD_SUB_TYPE_LOCATION						0x0301	// Location				|	pstrText
	#define	CA_FIELD_SUB_TYPE_ITEM_DATA						0x0302	// Generic Item data	|	pstrText / dwData
	#define	CA_FIELD_SUB_TYPE_TODO_PRIORITY					0x0303	// Todo priotiy			|	dwData
	#define	CA_FIELD_SUB_TYPE_TODO_STATUS					0x0304	// Todo status			|	dwData

	// CA_FIELD_SUB_TYPE_ITEM_DATA	usage varies among different calendar item types.
	// In CA_CALENDAR_ITEM_MEETING		Meeting detail information in pstrText parameter
	// in CA_CALENDAR_ITEM_CALL,		Phone number information in pstrText
	// In CA_CALENDAR_ITEM_BIRTHDAY		Birth year information in dwData paramter
	////////////////////////////////////////////////////
	// CA_DATA_DATE
	//
	// Description:
	// This structure contains date information in separated 
	// members.
	//  
	// Parameters:
	//	dwSize			Size of the structure (must be set!)
	//	wYear			Year information in four digit format (2005) 
	//	bMonth			Month information (1 - 12, january = 1) 
	//	bDay			Day of month (1-31)
	//	bHour			Hours after midnight (0-23)			
	//	bMinute			Minutes after hour (0-59)
	//	bSecond			Seconds after minute (0-59)
	//  lTimeZoneBias	Time zone bias in minutes (+120 for GMT+0200)
	//  lBias			Daylight bias.This contains value for offset in minutes 
	//					for time zone, for example +60. 
	
	typedef struct
	{
		DWORD	dwSize;
		WORD	wYear;
		BYTE	bMonth;
		BYTE	bDay;
		BYTE	bHour;
		BYTE	bMinute;
		BYTE	bSecond;
		long	lTimeZoneBias; 
		long	lBias; 
	} CA_DATA_DATE;
	
	/////////////////////////////////////////////////////
	// CA_DATA_ADDRESS
	//
	// Description:
	// This structure contains address information of the message. 
	//  
	// Parameters:
	//	dwSize			Size of the structure (must be set!)
	//  dwAddressInfo	Contains type information of address 
	//					See "Address type definitions" on top
	//  pstrAddress		Address data in unicode format
	//
	typedef struct
	{	
		DWORD	dwSize;
		DWORD	dwAddressInfo;
		WCHAR* 	pstrAddress;
	} CA_DATA_ADDRESS;

	/////////////////////////////////////////////////////
	// CA_DATA_POSTAL_ADDRESS
	//
	// Description:
	// This structure contains postal address information. 
	//  
	// Parameters:
	//	dwSize				Size of the structure (must be set!)
	//	pstrPOBox			PO Box address
	//	pstrStreet			Street address
	//	pstrPostalCode		Postal code information
	//	pstrCity			City 
	//	pstrState			State
	//	pstrCountry			Country 
	//	pstrExtendedData	Extended address information
	//
	typedef struct
	{	
		DWORD	dwSize;
		WCHAR* 	pstrPOBox;
		WCHAR* 	pstrStreet;
		WCHAR* 	pstrPostalCode;
		WCHAR* 	pstrCity;
		WCHAR* 	pstrState;
		WCHAR* 	pstrCountry;
		WCHAR* 	pstrExtendedData;
	} CA_DATA_POSTAL_ADDRESS;

	/////////////////////////////////////////////////////
	// CA_DATA_PICTURE
	//
	// Description:
	// This structure contains picture information and data
	//  
	// Parameters:
	//	dwSize			Size of the structure (must be set!)
	//	pstrFileName	Picture file name/picture format  
	//	dwDataLen		Picture data buffer length
	//	pbData			picture data
	//
	typedef struct
	{	
		DWORD	dwSize;
		WCHAR*	pstrFileName; 
		DWORD	dwDataLen;
		BYTE	*pbData; 
	} CA_DATA_PICTURE;

	/////////////////////////////////////////////////////
	// CA_DATA_ITEM
	//
	// Info: 
	// Generic data structure used with contacts and calendar item
	// dwFieldType defines the format of the data (which union member
	// contains valid data) . 
	//
	// Parameters:
	//	dwSize			Size of the structure (must be set!)
	//	dwFieldId		Field specific ID used in field access operations in content Access API. 
	//					Identifies certain field in content item. This is returned in CAReadItem. 
	//	dwFieldType		For different field types:
	//					see "Field type definitions " on top of this header
	//	dwFieldSubType	For different field sub types:
	//					see "Field sub type definitions" on top of this header
	//	ItemData		According to defined field sub type parameter, this union is 
	//					filled with valid data. See details in table
	//					"Field sub type definitions "
	//
	typedef struct
	{
		DWORD dwSize; 
		DWORD dwFieldId;
		DWORD dwFieldType;
		DWORD dwFieldSubType;		
		union
		{
			DWORD	dwData;
			WCHAR*	pstrText;			
			LPVOID	pCustomData;
		} ItemData;
	} CA_DATA_ITEM;

	/////////////////////////////////////////////////////
	// CA_DATA_CONTACT
	//
	// Info: 
	// Contact data item. 
	//
	// Parameters:
	//	dwSize			Size of the structure (must be set!)
	//	bPICount		Amount of personal information fields 
	//	pPIFields		Personal information field data
	//	bNumberCount	Amount of contact number information fields 
	//	pNumberFields	Contact number information data
	//	bAddressCount	Amount of address fields 
	//	pAddressFields	Address field data 
	//	bGeneralCount	Amount of general fields 
	//	pGeneralFields	General field data 
	//
	typedef struct
	{
		DWORD	dwSize;
		BYTE	bPICount;
		CA_DATA_ITEM* pPIFields;
		BYTE	bNumberCount;
		CA_DATA_ITEM* pNumberFields;
		BYTE	bAddressCount;
		CA_DATA_ITEM* pAddressFields;
		BYTE	bGeneralCount;
		CA_DATA_ITEM* pGeneralFields;
	} CA_DATA_CONTACT;

	/////////////////////////////////////////////////////
	// CA_DATA_CALENDAR
	//
	// Info: 
	// Calendar item structure
	//
	// Parameters:
	//	dwSize				Size of the structure (must be set!)
	//	dwInfoField			Type of the calendar item
	//	noteStartDate		Start date of the note
	//	noteEndDate			End date of the note
	//  dwAlarmState		For possible values , see "Calendar Alarm State value definitions"
	//	noteAlarmTime		Alarm time of the note (defines also if no alarm for note)
	//	dwRecurrence		Requrrency of the note. For possible values see 
	//						"Recurrence values for calendar items" in this header file. 
	//						Use macros CA_GET_RECURRENCE and CA_SET_RECURRENCE for setting
	//						and getting value from this macro. Upper WORD is reserved for 
	//						future use
	//	recurrenceEndDate	End date if note has requrrence
	//	bItemCount			Amount of items belonging to note
	//	pDataItems			Calendar data items
	//
	typedef struct
	{			
		DWORD			dwSize;
		DWORD			dwInfoField; 		
		CA_DATA_DATE	noteStartDate; 
		CA_DATA_DATE	noteEndDate; 
		DWORD			dwAlarmState; 
		CA_DATA_DATE	noteAlarmTime; 
		DWORD 			dwRecurrence; 
		CA_DATA_DATE	recurrenceEndDate; 
		BYTE 			bItemCount;
		CA_DATA_ITEM   *pDataItems;
	} CA_DATA_CALENDAR;

	/////////////////////////////////////////////////////
	// CA_DATA_MSG
	//
	// Description:
	// CA data stucture for message data
	// 
	// 
	//  
	// Parameters:
	//	dwSize			Size of the structure (must be set!), use sizeof(CA_DATA_MSG)
	//  dwInfoField		Contains status information of message (used encoding,input data 
	//					format, message status 
	//					In dwInfoField following definitions are used now : 
	//					0x0X value defines format of input data
	//					0xX0 value defines how input data is passed to the phone
	//					0x0X00 value defines status of the message (read / unread...)
	//					0xX000 value defines message type (submit / deliver )
	// 			
	//  dwDataLength	size of pbData byte array 
	//  pbData			Actual user data 
	//  bAddressCount	Amount of addresses included in pAddress array  (currently only one address supported)
	//	pAddress		Pointer to addesses.
	//	messageDate		This struct is used when generating time stamps for the message
	//
	typedef struct
	{	
		DWORD	dwSize;
		DWORD	dwInfoField;
		DWORD	dwDataLength;
		BYTE*	pbData;
		BYTE	bAddressCount;
		CA_DATA_ADDRESS* pAddress;
		CA_DATA_DATE messageDate; 
	} CA_DATA_MSG;

	/////////////////////////////////////////////////////
	// CA_DATA_VERSIT 
	//
	// Description:
	// CA data stucture for versit data. This is used in CAReadItem
	// and CAWriteItem operarations when selected data format is 
	// CA_DATA_FORMAT_VERSIT
	// 
	// Format of versit object has dependency to Content Access API 
	// target content. Following table presents the format information : 
	//
	//	CAAPI Target Content		Versit Object character format
	//	----------------------		---------------------------------
	// 	CA_TARGET_CONTACTS			UTF-8 (vCard)
	//	CA_TARGET_CALENDAR			UTF-8 (vCal)	
	//	CA_TARGET_NOTES				CA_DATA_FORMAT_VERSIT not supported,
	//								CA_DATA_FORMAT_STRUCT must be used for CA_TARGET_NOTES
	//  CA_TARGET_SMS_MESSAGES		Unicode (UTF-16,vMsg)			
	//	CA_TARGET_MMS_MESSAGES		Versit Format not supported, 
	//								CA_DATA_FORMAT_STRUCT used for CA_TARGET_MMS_MESSAGES
	//	CA_TARGET_BOOKMARKS			UTF-8 (vBookmark)
	//  
	// Parameters:
	//	dwSize			Size of the structure (must be set!)
	//	dwInfoField		Contains status information of versit data object
	//					In dwInfoField following definitions are used now : 
	//					0x0X value defines format of input data, see "Input data format definitions"
	//	dwDataLenght	Lenght of the data
	//	pbVersitObject	Pointer to versit object data.
	//
	typedef struct
	{			
		DWORD	dwSize;
		DWORD	dwInfoField; 
		DWORD	dwDataLenght;
		BYTE*	pbVersitObject;
	} CA_DATA_VERSIT;


	//=========================================================
	// CA_MMS_DATA
	//
	// Description:
	// This structure defines MMS message data
	//  
	// Parameters:
	//	dwSize			The total size of this structure in bytes (sizeof(CA_MMS_DATA)).
	//	dwInfoField		Contains status information of message (message status)
	//					In dwInfoField following definitions are used now : 
	//					0x0X00 value defines status of the message (read / unread...)	
	//  bAddressCount	Amount of addresses in message (currently only one address supported)
	//					Address is returned when reading the message, but in the writing address
	//					is included in binary MMS
	//  pAddress		Address array 
	//	messageDate		Message date
	//	dwDataLength	Size of MMS data buffer
	//	pbData			Actual MMS data 	
	//

	typedef struct
	{
		DWORD	dwSize;
		DWORD	dwInfoField; 		
		BYTE	bAddressCount;
		CA_DATA_ADDRESS* pAddress;
		CA_DATA_DATE messageDate; 
		DWORD	dwDataLength;
		BYTE	*pbData; 
	} CA_MMS_DATA;

	//=========================================================
	// CA_DATA_NOTE
	//
	// Description:
	// This structure defines calendar note data 
	//  
	// Parameters:
	//	dwSize			The total size of this structure in bytes (sizeof(CA_DATA_ITEM)).
	//	pstrText		Note text
	//
	typedef struct
	{
		DWORD	dwSize;
		WCHAR *pstrText;
	} CA_DATA_NOTE;

	/////////////////////////////////////////////////////
	// CA_DATA_BOOKMARK
	//
	// Info: 
	// Bookmark data structure
	//
	// Parameters:
	//	dwSize			Size of the structure (must be set!)
	//	pstrTitle		Title of bookmark
	//	pstrBookMarkUrl	Bookmark URL 
	//  pstrUrlShortcut Url shortcut
	//	
	typedef struct
	{	
		DWORD	dwSize;
		WCHAR*	pstrTitle;
		WCHAR*	pstrBookMarkUrl;
		WCHAR*	pstrUrlShortcut; 
	} CA_DATA_BOOKMARK;

#endif // __CADATADEFINITIONS_7347326__
