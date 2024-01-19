/*  libstring.h
    Additional string methods I find useful in my apps.
    (C)2024 */

#ifndef COM_PLUS_MEVANSPN_LIBSTRING
#define COM_PLUS_MEVANSPN_LIBSTRING

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define _MAXIMUM_ALLOWED_STRING_LENGTH 65536
#define _STRINGLIB_BUFFER_SIZE 5

#define STRING_FORMAT_UNDEFINED 0
#define STRING_FORMAT_LOWERCASE 2
#define STRING_FORMAT_UPPERCASE 4
#define STRING_FORMAT_AS_IS (STRING_FORMAT_LOWERCASE + STRING_FORMAT_UPPERCASE)
#define STRING_FORMAT_TRIM_LEFT 8
#define STRING_FORMAT_TRIM_RIGHT 16
#define STRING_FORMAT_TRIM (STRING_FORMAT_TRIM_LEFT + STRING_FORMAT_TRIM_RIGHT)

#define STRING_FORMAT_CASE_SENSITIVE 0
#define STRING_FORMAT_CASE_INSENSITIVE 1

typedef struct __StringHeader {
    char id[4];
    size_t length;
    struct __StringHeader * structure_ptr;
} _StringHeader;

/*  PUBLIC METHODS */

char * createString(const char * SOURCE_STRING);
char * copyString(const char * STRING, size_t OUTPUT_STRING_FORMAT);
void freeString(const char * STRING);
bool isString(const char * STRING, _StringHeader ** _string_header_ptr);
size_t getStringLength(const char * STRING);
bool compareStrings(const char * STRING1, const char * STRING2, size_t STRING_COMPARISON_FORMAT);

char ** createStringTokensArray(const char * STRING, const char * DELIMETERS_CHAR_ARRAY, size_t OUTPUT_TOKEN_STRING_FORMAT, size_t * tokens_array_size_ptr);
void freeStringTokensArray(char ** string_tokens_array, size_t string_tokens_array_length);

/*  PRIVATE METHODS */

char * _createEmptyString(const size_t LENGTH);
_StringHeader * _getStringStructure(const char * STRING);


#endif