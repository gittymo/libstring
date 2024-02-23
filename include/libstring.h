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

/*  String formats */

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

/*  Note:   Strings created in this library are not simply arrays of chars.  The pointer returned by many of these methods does point directly to the
            copied char array, however, the string data is prefixed with a _StringHeader structure which allows much greated flexibility when working
            with strings, but means that the developer needs to realise that these methods cannot be used on any char array strings created outside of
            this library.  Most methods will simply return a NULL or zero value if the string was not created by the library, however, I cannot 
            guarantee any unexpected results, should the memory before the string or the string's char array data itself be altered outside of library
            control.  */

/*  PUBLIC METHODS */

/// @brief Creates and returns a pointer to a string whose characters are copied from the given character array.
/// @param CHAR_ARRAY Pointer to the source characters to use for the string.  The string will make a copy of these characters, so the original
///                   char array can be discarded if no longer needed.
/// @return A pointer to the string's char array.  This array can be used like any other string char array.
char * createString(const char * CHAR_ARRAY);

/// @brief Creates and returns a pointer to a string which is a copy of the given source string.  However, the format of the string can be altered using
///         various flag values (to make the copy lower case for instance, or to remove any unnecessary spaces around the text).
/// @param STRING Pointer to the source string's character array.
/// @param OUTPUT_STRING_FORMAT Defines the format of the string by combining the #defined string format values (see string formats section, above).
/// @return A pointer to a copy of the source character array, formatted as requested.
char * copyString(const char * STRING, size_t OUTPUT_STRING_FORMAT);

/// @brief Frees memory allocated to a string.
/// @param STRING Pointer to the character array of the string to free.
void freeString(const char * STRING);

/// @brief Returns true or false to indicate if the given char array pointer points to an array created by this library (true) or not (false).
/// @param STRING Pointer to the character array to check.
/// @param _string_header_ptr Used internally by the library.  User methods should call isString, passing NULL for this parameter's value.
/// @return Boolean value indicating if the character array was created by this library (true) or not (false).
bool isString(const char * STRING, _StringHeader ** _string_header_ptr);

/// @brief If the character array was created by this library, will return the length of the string in characters, or zero if the char array was created
///         outside of the library.
/// @param STRING Pointer to the string's character array.
/// @return Length of the string if created by this library, or zero (0) if not.
size_t getStringLength(const char * STRING);

/// @brief Compares two strings and returns true if they match or false if not.  The calling method can provide a comparison type modifier which can
///         be used to refine the accuracy of the result.
/// @param STRING1 Pointer to the first string's character array.
/// @param STRING2 Pointer to the second string's character array.
/// @param STRING_COMPARISON_FORMAT Can be set to define how the comparison is performed (e.g. case sensitivity).  See 'String Formats' section for
///         the #defined values.
/// @return True if the strings match under the given comparison criteria, or false if not.  Will also return false if either string pointer is NULL or
///         if either of the pointers are to character arrays created outside of this library.
bool compareStrings(const char * STRING1, const char * STRING2, size_t STRING_COMPARISON_FORMAT);

/// @brief Breaks a string into parts (tokens), where each part has been seperated by one a number of given delimeter characters.  Returns an array of string 
///         character array pointers, formatted as per the given string formatting requirements.
/// @param STRING Pointer to the source string's character array.
/// @param DELIMETERS_CHAR_ARRAY A regular (not generated by this library) array of characters defining the delimeters to use when breaking the string into
///         parts.
/// @param OUTPUT_TOKEN_STRING_FORMAT Can be set to format the generated parts (tokens), for example, resulting in an array of lower case strings.
/// @param tokens_array_size_ptr A required (cannot be NULL) pointer to a variable which will be used to hold the size of the token array.  This can be
///                                 zero if the source string pointer if invalid or if the source string length is zero.
/// @return A pointer to an array of string character arrays which can be manipulated by this library, or NULL if the source string is invalid or zero length.
char ** createStringTokensArray(const char * STRING, const char * DELIMETERS_CHAR_ARRAY, size_t OUTPUT_TOKEN_STRING_FORMAT, size_t * tokens_array_size_ptr);

/// @brief Frees an array of string character arrays previously created by createStringTokensArray.
/// @param string_tokens_array Pointer to the array of string character arrays.
/// @param string_tokens_array_length The length of the array.  This must be the same as the value passed back to variable referenced by tokens_array_size_ptr
///                                     in the createStringTokensArray.  Passing an invalid value will result in memory leaks or corruption!
void freeStringTokensArray(char ** string_tokens_array, size_t string_tokens_array_length);

/// @brief Returns the hash value for the given string.
/// @param STRING Pointer to the string's char array.
/// @return Hash value for the given string (or zero on failure) as an unsigned long (size_t) value.
size_t getStringHash(const char * STRING);

/*  PRIVATE METHODS */

char * _createEmptyString(const size_t LENGTH);
_StringHeader * _getStringStructure(const char * STRING);

#endif