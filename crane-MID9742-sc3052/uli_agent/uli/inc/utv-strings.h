/* utv-strings.h: UpdateTV Common Core - UpdateTV string table

   In order to ensure UpdateTV network compatibility, customers must not
   change UpdateTV Common Core code.

   Copyright (c) 2004 - 2009 UpdateLogic Incorporated. All rights reserved.

*/
#ifndef __UTVSTRINGS_H__
#define __UTVSTRINGS_H__

#define UTV_INTERNET_HTTP_SERVICE 0
#define UTV_INET_ACTION_REQUEST_REGISTERIDD 1
#define UTV_INET_ACTION_REQUEST_IDDREGISTERED 2
#define UTV_INET_ACTION_REQUEST_UPDATE_QUERY 3
#define UTV_INET_ACTION_REQUEST_SCHEDULE_DLOAD 4
#define UTV_INET_ACTION_REQUEST_DLOAD_ENDED 5
#define UTV_INET_ACTION_REQUEST_STATUS_CHANGE 6
#define UTV_INET_ACTION_REQUEST_PROVISIONIDD 7
#define UTV_INET_ACTION_REQUEST_IDDPROVISIONED 8
#define UTV_XML_STATUS_CHANGE_UTV_STATUS_FMT 9
#define UTV_INET_POSTDATA_CEM_STATUS 10
#define UTV_INET_POSTDATA_CEM_PRIVATE_STATUS 11
#define UTV_XML_KEY_UTVOK 12
#define UTV_XML_CLOSURE_UTVOK 13
#define UTV_XML_KEY_ERROR 14
#define UTV_XML_KEY_CEM 15
#define UTV_XML_CLOSURE_CEM 16
#define UTV_XML_KEY_NAME 17
#define UTV_XML_CLOSURE_NAME 18
#define UTV_XML_KEY_VALUE 19
#define UTV_XML_CLOSURE_VALUE 20
#define UTV_XML_KEY_OBJECTS_AVAILABLE 21
#define UTV_XML_KEY_REVOCATION_LIST 22
#define UTV_XML_KEY_PROVISION_INFO 23
#define UTV_XML_KEY_GUID 24
#define UTV_XML_KEY_OWNER 25
#define UTV_XML_KEY_ENCRYPTION 26
#define UTV_XML_KEY_DELIVERY_INFO 27
#define UTV_XML_KEY_CRC 28
#define UTV_XML_KEY_OBJECT_ID 29
#define UTV_XML_KEY_OBJECTS_IN_SET 30
#define UTV_XML_KEY_OBJECTS_INDEX 31
#define UTV_XML_KEY_SET_ID 32
#define UTV_XML_KEY_ULID 33
#define UTV_XML_KEY_DEVID 34
#define UTV_XML_KEY_QUERY_IN_SECONDS 35
#define UTV_XML_KEY_DEVICE_HINT 36
#define UTV_XML_KEY_SOFTWARE_UPDATE 37
#define UTV_XML_KEY_OUI 38
#define UTV_XML_KEY_MODEL_GROUP 39
#define UTV_XML_KEY_MODULE_VERSION 40
#define UTV_XML_KEY_ATTRIBUTES 41
#define UTV_XML_CLOSURE_ATTRIBUTES 42
#define UTV_XML_KEY_TIME 43
#define UTV_XML_CLOSURE_TIME 44
#define UTV_XML_STATUS_MASK 45
#define UTV_XML_STATUS_MASK_CLOSURE 46
#define UTV_XML_CMD_MASK 47
#define UTV_XML_CMD_MASK_CLOSURE 48
#define UTV_XML_KEY_COMPATIBILITY 49
#define UTV_XML_KEY_HARDWARE_MODEL_BEGIN 50
#define UTV_XML_KEY_HARDWARE_MODEL_END 51
#define UTV_XML_KEY_SOFTWARE_VERSION_BEGIN 52
#define UTV_XML_KEY_SOFTWARE_VERSION_END 53
#define UTV_XML_CLOSURE_COMPATIBILITY 54
#define UTV_XML_KEY_DOWNLOAD_IN_SECONDS 55
#define UTV_XML_KEY_HOSTNAME 56
#define UTV_XML_KEY_FILE_PATH 57
#define UTV_XML_KEY_FILE_SIZE 58
#define UTV_INET_ERROR_SERIAL_RANGE 59
#define UTV_INET_ERROR_ALREADY_REGISTERED 60
#define UTV_USB_CMD 61
#define UTV_PRE_BOOT_CMD 62
#define UTV_POST_EVERY_BOOT_CMD 63
#define UTV_POST_BOOT_CMD 64
#define UTV_XML_NOC_HINT 65
#define UTV_QUERY_HOST 66
#define UTV_CMD_TARGET 67
#define UTV_ULPK_TRANSIT_KEY 68
#define UTV_ULPK_TRANSIT_VECTOR 69
#define UTV_ULPK_SECURE_KEY 70
#define UTV_ULPK_SECURE_VECTOR 71


UTV_RESULT UtvStringsInit( void );
UTV_RESULT UtvStringsRelease( void );
char *UtvStringsLookup( UTV_UINT32 id );


#endif
