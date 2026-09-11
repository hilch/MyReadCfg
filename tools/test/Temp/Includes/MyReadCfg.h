/* Automation Studio generated header file */
/* Do not edit ! */
/* MyReadCfg 1.00.0 */

#ifndef _MYREADCFG_
#define _MYREADCFG_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _MyReadCfg_VERSION
#define _MyReadCfg_VERSION 1.00.0
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG3
		#include "runtime.h"
		#include "ArEventLog.h"
		#include "FileIO.h"
		#include "AsBrStr.h"
		#include "sys_lib.h"
		#include "standard.h"
		#include "brsystem.h"
#endif
#ifdef _SG4
		#include "runtime.h"
		#include "ArEventLog.h"
		#include "FileIO.h"
		#include "AsBrStr.h"
		#include "sys_lib.h"
		#include "standard.h"
		#include "brsystem.h"
#endif
#ifdef _SGC
		#include "runtime.h"
		#include "ArEventLog.h"
		#include "FileIO.h"
		#include "AsBrStr.h"
		#include "sys_lib.h"
		#include "standard.h"
		#include "brsystem.h"
#endif


/* Constants */
#ifdef _REPLACE_CONST
 #define MRC_MAX_HANDLES 8U
 #define MRC_MAX_FILE_SIZE 1048576U
 #define MRC_MAX_KEY_LENGTH 64U
 #define MRC_MAX_VALUE_LENGTH 255U
 #define MRC_READ_CHUNK 8192U
 #define MRC_FILEIO_MAX_CALLS 100000U
 #define MRC_LOGGER_SIZE 65536U
 #define MRC_EVENT_INFO 1610612736
 #define MRC_EVENT_WARN (-1610612736)
 #define MRC_EVENT_ERR (-536870912)
 #define MRC_ERR_OK 0
 #define MRC_ERR_PARAM (-1064370176)
 #define MRC_ERR_INVALID_IDENT (-1064370175)
 #define MRC_ERR_NO_HANDLE (-1064370174)
 #define MRC_ERR_FILE (-1064370173)
 #define MRC_ERR_FILE_TOO_BIG (-1064370172)
 #define MRC_ERR_MEMORY (-1064370171)
 #define MRC_ERR_KEY_NOT_FOUND (-1064370170)
 #define MRC_ERR_CONVERSION (-1064370169)
 #define MRC_ERR_TRUNCATED (-1064370168)
 #define MRC_ERR_TIMEOUT (-1064370167)
#else
 _GLOBAL_CONST unsigned long MRC_MAX_HANDLES;
 _GLOBAL_CONST unsigned long MRC_MAX_FILE_SIZE;
 _GLOBAL_CONST unsigned long MRC_MAX_KEY_LENGTH;
 _GLOBAL_CONST unsigned long MRC_MAX_VALUE_LENGTH;
 _GLOBAL_CONST unsigned long MRC_READ_CHUNK;
 _GLOBAL_CONST unsigned long MRC_FILEIO_MAX_CALLS;
 _GLOBAL_CONST unsigned long MRC_LOGGER_SIZE;
 _GLOBAL_CONST signed long MRC_EVENT_INFO;
 _GLOBAL_CONST signed long MRC_EVENT_WARN;
 _GLOBAL_CONST signed long MRC_EVENT_ERR;
 _GLOBAL_CONST signed long MRC_ERR_OK;
 _GLOBAL_CONST signed long MRC_ERR_PARAM;
 _GLOBAL_CONST signed long MRC_ERR_INVALID_IDENT;
 _GLOBAL_CONST signed long MRC_ERR_NO_HANDLE;
 _GLOBAL_CONST signed long MRC_ERR_FILE;
 _GLOBAL_CONST signed long MRC_ERR_FILE_TOO_BIG;
 _GLOBAL_CONST signed long MRC_ERR_MEMORY;
 _GLOBAL_CONST signed long MRC_ERR_KEY_NOT_FOUND;
 _GLOBAL_CONST signed long MRC_ERR_CONVERSION;
 _GLOBAL_CONST signed long MRC_ERR_TRUNCATED;
 _GLOBAL_CONST signed long MRC_ERR_TIMEOUT;
#endif







/* Prototyping of functions and function blocks */
_BUR_PUBLIC unsigned long MRC_Open(unsigned long pDeviceName, unsigned long pFileName);
_BUR_PUBLIC signed long MRC_Close(unsigned long Ident);
_BUR_PUBLIC signed long MRC_GetLastError(unsigned long Ident);
_BUR_PUBLIC plcbit MRC_KeyExists(unsigned long Ident, unsigned long pKey);
_BUR_PUBLIC plcbit MRC_ReadBOOL(unsigned long Ident, unsigned long pKey, plcbit Default);
_BUR_PUBLIC signed long MRC_ReadDINT(unsigned long Ident, unsigned long pKey, signed long Default);
_BUR_PUBLIC double MRC_ReadLREAL(unsigned long Ident, unsigned long pKey, double Default);
_BUR_PUBLIC signed long MRC_ReadSTRING(unsigned long Ident, unsigned long pKey, unsigned long pValue, unsigned long MaxSize, unsigned long pDefault);


#ifdef __cplusplus
};
#endif
#endif /* _MYREADCFG_ */

