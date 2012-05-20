/*
 * jsutils.h
 *
 *  Created on: 18 Apr 2012
 *      Author: gw
 */

#ifndef JSUTILS_H_
#define JSUTILS_H_

#ifndef ARM
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#endif

// surely bool is defined??
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#define true (1)
#define false (0)
#ifdef ARM
typedef unsigned int size_t;
#endif

/// Reference for variables
typedef unsigned short JsVarRef;
// We treat 0 as null

typedef long JsVarInt;
typedef unsigned long JsVarIntUnsigned;
#ifdef USE_FLOATS
typedef float JsVarFloat;
#else
typedef double JsVarFloat;
#endif

#define JSVAR_DATA_STRING_LEN  8 // Actually 9 seems like a good number as 'prototype'==9
#define JSVAR_STRING_OP_BUFFER_SIZE 256 // FIXME - we need to do this properly
#define JSLEX_MAX_TOKEN_LENGTH  64
#define JS_ERROR_BUF_SIZE 64 // size of buffer error messages are written into
#define JS_ERROR_TOKEN_BUF_SIZE 16

#define JSPARSE_MAX_SCOPES  32
#define JSPARSE_MAX_LOOP_ITERATIONS 8192

// javascript specific names
#define JSPARSE_RETURN_VAR "return"
#define JSPARSE_THIS_VAR "this"
#define JSPARSE_PROTOTYPE_CLASS "prototype"
// internal names that hopefully nobody will be able to access
#define JSPARSE_FUNCTION_CODE_NAME "#code#"

#ifndef SDCC
 #define assert(X) if (!(X)) jsAssertFail(__FILE__,__LINE__);
 #define PACKED_FLAGS  __attribute__ ((__packed__))  
#else
 #define assert(X) 
 #define PACKED_FLAGS 
#endif


typedef enum {
  // OPT: These can be packed as there's no point being an array AND a float
    JSV_NUMERICMASK = 8,
    JSV_VARTYPEMASK = 15,

    JSV_UNUSED   = 0,
    // UNDEFINED is now just stored using '0' as the variable Ref
    JSV_NULL        = 1, // it seems null is its own data type
    JSV_STRING      = 2, // string
    JSV_STRING_EXT  = 3, // extra character data for string (if it didn't fit in first JsVar). These use unused pointer fields for extra characters
    JSV_ARRAY       = 4,
    JSV_OBJECT      = 5,
    JSV_FUNCTION    = 6,
    JSV_INTEGER     = 8, // integer number (note JSV_NUMERICMASK)
    JSV_FLOAT       = 9, // floating point double (note JSV_NUMERICMASK)

    JSV_NAME        = 16, // a NAME of a variable - this isn't a variable itself (and can be an int/string/etc)
    JSV_NATIVE      = 32, // to specify this is a native function
    JSV_TEMP        = 64, // mainly for debugging so we can see if a temp var got used wrongly

    JSV_FUNCTION_PARAMETER = JSV_FUNCTION | JSV_NAME, // this is inside a function, so it should be quite obvious
    // these are useful ONLY because the debugger picks them up :)
    JSV_NAME_AS_STRING = JSV_NAME | JSV_STRING,
    JSV_NAME_AS_INT = JSV_NAME | JSV_INTEGER,
    JSV_NATIVE_FUNCTION = JSV_NATIVE | JSV_FUNCTION
} PACKED_FLAGS JsVarFlags;

typedef enum LEX_TYPES {
    LEX_EOF = 0,
    LEX_ID = 256,
    LEX_INT,
    LEX_FLOAT,
    LEX_STR,

    LEX_EQUAL,
    LEX_TYPEEQUAL,
    LEX_NEQUAL,
    LEX_NTYPEEQUAL,
    LEX_LEQUAL,
    LEX_LSHIFT,
    LEX_LSHIFTEQUAL,
    LEX_GEQUAL,
    LEX_RSHIFT,
    LEX_RSHIFTUNSIGNED,
    LEX_RSHIFTEQUAL,
    LEX_PLUSEQUAL,
    LEX_MINUSEQUAL,
    LEX_PLUSPLUS,
    LEX_MINUSMINUS,
    LEX_ANDEQUAL,
    LEX_ANDAND,
    LEX_OREQUAL,
    LEX_OROR,
    LEX_XOREQUAL,
    // reserved words
#define LEX_R_LIST_START LEX_R_IF
    LEX_R_IF,
    LEX_R_ELSE,
    LEX_R_DO,
    LEX_R_WHILE,
    LEX_R_FOR,
    LEX_R_BREAK,
    LEX_R_CONTINUE,
    LEX_R_FUNCTION,
    LEX_R_RETURN,
    LEX_R_VAR,
    LEX_R_TRUE,
    LEX_R_FALSE,
    LEX_R_NULL,
    LEX_R_UNDEFINED,
    LEX_R_NEW,

    LEX_R_LIST_END /* always the last entry */
} LEX_TYPES;

bool isWhitespace(char ch);
bool isNumeric(char ch);
bool isHexadecimal(char ch);
bool isAlpha(char ch);
bool isIDString(const char *s);

/* convert hex, binary, octal or decimal string into an int. strtoint is broken on PIC32 */
JsVarInt stringToInt(const char *s);

// forward decl
struct JsLex;
// ------------

void jsError(const char *message);
void jsErrorAt(const char *message, struct JsLex *lex, int tokenPos);
void jsWarn(const char *message);
void jsWarnAt(const char *message, struct JsLex *lex, int tokenPos);
void jsAssertFail(const char *file, int line);

/// This is the place that all text is output from TinyJS. It could be overwridden if required
void jsPrint(const char *txt);
/// Helper function - prints an integer
void jsPrintInt(int d);

#ifdef SDCC
void exit(int errcode);
#endif
#ifdef ARM
void exit(int errcode);
void strncat(char *dst, const char *src, int c);
void strncpy(char *dst, const char *src, int c);
int strlen(const char *s);
int strcmp(char *a, const char *b);
void memcpy(char *dst, const char *src, int size);
int rand();
#endif

// FIXME: use itoa/ftoa direct - sprintf is huge
#define itoa(val,str,base) sprintf(str,"%d",(int)val)
#define ftoa(val,str) sprintf(str,"%f",val)

#endif /* JSUTILS_H_ */
