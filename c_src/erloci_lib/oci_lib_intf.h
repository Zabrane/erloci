/* Copyright 2012 K2Informatics GmbH, Root Laengenbold, Switzerland
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

//#define MAX_RESP_SIZE 0xFFFFFFF0
#define MAX_RESP_SIZE 0x00040000UL
#define MAX_COLUMNS 500

#ifndef __WIN32__
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#define memcpy_s(_dest, _noelms, _src, _count)	memcpy((_dest), (_src), (_count))
#define sprintf_s(_a, _b, _c, ...)				sprintf((_a), (_c), __VA_ARGS__)
#define strncpy_s(_a, _b, _c, _d)               strncpy((_a), (_c), (_d))
#define vsprintf_s(_a, _b, _c, _d)              vsprintf((_a), (_c), (_d))
#define REMOTE_LOG(_str, ...)		if (log_flag) log_remote(("[debug] [_PRT_] {%s:%s:%d} "_str),__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
#define REMOTE_LOG_SINGLE(_str, ...)	if (log_flag) log_remote((_str),##__VA_ARGS__)
#else
#define REMOTE_LOG(_str, ...)			if (log_flag) log_remote(("[debug] [_PRT_] {%s:%s:%d} "_str),__FILE__,__FUNCTION__,__LINE__,__VA_ARGS__)
#define REMOTE_LOG_SINGLE(_str, ...)	if (log_flag) log_remote((_str),__VA_ARGS__)
#endif

typedef enum _INTF_RET {
    SUCCESS				= 0,
    CONTINUE_WITH_ERROR	= 1,
    FAILURE				= 2,
    ERROR				= 3,
    MORE				= 4,
    DONE				= 5,
} INTF_RET;

typedef enum _DATA_TYPES {
    NUMBER	= 0,
    STRING	= 1,
} DATA_TYPES;

typedef enum _DATA_DIR {
    DIR_IN		= 0,
    DIR_OUT		= 1,
    DIR_INOUT	= 2,
} DATA_DIR;

typedef struct inp_t {
    struct inp_t * next;
    void		 * bndp;
    DATA_TYPES	dty;
    DATA_DIR	dir;
    int			vlen;
    void		*vp;
} inp_t;

typedef struct intf_ret {
	void		*handle;
	char		gerrbuf[512]; // from oracle example
	int			gerrcode;
	INTF_RET	fn_ret;
} intf_ret;

// External linkages (import)
extern bool	log_flag;

//
// Exposed linkages (export)
//

extern void log_remote(const char *, ...);

extern intf_ret oci_get_session(void **, const char *, const int, const char *, const int, const char *, const int);
extern intf_ret oci_free_session(void *);

extern intf_ret	oci_exec_sql(const void *, void **, const unsigned char *, int, inp_t *, void *, void (*)(const char *, const char *, const unsigned int, void *));
extern intf_ret	oci_produce_rows(void *, void *, void (*)(const char *, size_t, void *), void (*)(const void *, void *), size_t (*)(void *), int);
extern intf_ret oci_close_statement(void *);

/* Error checking functions and macros */
#define checkerr(errhp, status) checkerr0((errhp), OCI_HTYPE_ERROR, (status), __FUNCTION__, __LINE__)
#define checkenv(envhp, status) checkerr0((envhp), OCI_HTYPE_ENV, (status), __FUNCTION__, __LINE__)
extern void checkerr0(intf_ret *, unsigned int, int, const char *, int);
