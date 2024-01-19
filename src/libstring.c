/*  libstring.c 
    Additional string methods I find useful in my apps 
    (C)2024 */

#include "libstring.h"

char * createString(const char * CHAR_ARRAY)
{
    const size_t STRING_LENGTH = CHAR_ARRAY ? strlen(CHAR_ARRAY) > _MAXIMUM_ALLOWED_STRING_LENGTH ? _MAXIMUM_ALLOWED_STRING_LENGTH : strlen(CHAR_ARRAY) : 0;
    const size_t ACTUAL_LENGTH = STRING_LENGTH + 1 + sizeof(_StringHeader);
    char * new_string_data = (char *) calloc(ACTUAL_LENGTH, 1);
    if (!new_string_data) return NULL;

    _StringHeader * header = (_StringHeader *) new_string_data;
    strncpy(header->id, "STR", 4);
    header->length = STRING_LENGTH;
    header->structure_ptr = header;

    char * string_char_ptr = new_string_data + sizeof(_StringHeader);
    if (CHAR_ARRAY) strcpy(string_char_ptr, CHAR_ARRAY);

    return string_char_ptr;
}

char * _createEmptyString(const size_t LENGTH)
{
    const size_t ACTUAL_LENGTH = LENGTH + 1 + sizeof(_StringHeader);
    char * new_string_data = (char *) calloc(ACTUAL_LENGTH, 1);
    if (!new_string_data) return NULL;

    _StringHeader * header = (_StringHeader *) new_string_data;
    strncpy(header->id, "STR", 4);
    header->length = LENGTH;
    header->structure_ptr = header;

    char * string_char_ptr = new_string_data + sizeof(_StringHeader);
    return string_char_ptr;
}

_StringHeader * _getStringStructure(const char * STRING)
{
    return (_StringHeader *) (STRING - sizeof(_StringHeader));
}

bool isString(const char * STRING, _StringHeader ** _string_header_ptr)
{
    if (!STRING) return false;
    _StringHeader * header = _getStringStructure(STRING);
    if (_string_header_ptr) *_string_header_ptr = header;
    return header && strcmp(header->id, "STR") == 0;
}

void freeString(const char * STRING)
{
    _StringHeader * header;
    if (isString(STRING, &header)) free(header);
}

size_t getStringLength(const char * STRING)
{
    _StringHeader * header;
    if (!isString(STRING, &header)) return 0;
    return header->length;
}

char * copyString(const char * STRING, size_t OUTPUT_STRING_FORMAT)
{
    if (OUTPUT_STRING_FORMAT == STRING_FORMAT_UNDEFINED) OUTPUT_STRING_FORMAT = STRING_FORMAT_AS_IS;

    _StringHeader * header;
    if (!isString(STRING, &header)) return NULL;

    size_t start = 0, end = header->length - 1;
    if (OUTPUT_STRING_FORMAT & STRING_FORMAT_TRIM_LEFT) {
        while (start < end && isblank(STRING[start])) start++;
    }
    if (OUTPUT_STRING_FORMAT & STRING_FORMAT_TRIM_RIGHT) {
        while (end > start && isblank(STRING[end])) end--;
    }

    const size_t REQUIRED_LENGTH = (end - start) + 1;
    char * string_copy = _createEmptyString(REQUIRED_LENGTH);
    for (size_t i = REQUIRED_LENGTH; i--;) {
        size_t si = i + start;
        if ((OUTPUT_STRING_FORMAT & STRING_FORMAT_AS_IS) == STRING_FORMAT_AS_IS) string_copy[i] = STRING[si];
        else if (OUTPUT_STRING_FORMAT & STRING_FORMAT_LOWERCASE) string_copy[i] = STRING[si] + (32 * (STRING[si] >= 'A' && STRING[si] <= 'Z'));
        else if (OUTPUT_STRING_FORMAT & STRING_FORMAT_UPPERCASE) string_copy[i] = STRING[si] - (32 * (STRING[si] >= 'a' && STRING[si] <= 'z'));
    }
    return string_copy;
}

bool compareStrings(const char * STRING1, const char * STRING2, size_t STRING_COMPARISON_FORMAT)
{
    _StringHeader * string1Header, * string2Header;
    if (!isString(STRING1, &string1Header) || !isString(STRING2, &string2Header)) return false;
    if (string1Header->length != string2Header->length) return false;
    bool match = true;
    for (size_t i = string1Header->length; match && i--;) {
        if (STRING_COMPARISON_FORMAT != STRING_FORMAT_CASE_INSENSITIVE) match = STRING1[i] == STRING2[i];
        else {
            match = STRING1[i] == STRING2[i] || 
                    (STRING1[i] >= 'A' && STRING1[i] <='Z' && STRING2[i] == STRING1[i] + 32) ||
                    (STRING1[i] >= 'a' && STRING1[i] <='z' && STRING2[i] == STRING1[i] - 32);
        }
    }
    return match;
}

void freeStringTokensArray(char ** string_tokens_array, size_t string_tokens_array_length)
{
    if (!string_tokens_array || string_tokens_array_length < 1) return;
    for (size_t i = 0; i < string_tokens_array_length; i++) {
        freeString(string_tokens_array[i]);
        string_tokens_array[i] = NULL;
    }
    free(string_tokens_array);
}

char ** createStringTokensArray(const char * STRING, const char * DELIMETERS_CHAR_ARRAY, size_t OUTPUT_TOKEN_STRING_FORMAT, size_t * tokens_array_size_ptr)
{
    _StringHeader * header;
    if (!isString(STRING, &header) || !DELIMETERS_CHAR_ARRAY || !tokens_array_size_ptr) return NULL;
    if (header->length == 0) return NULL;

    char ** tokens_array = (char **) malloc(sizeof(char *) * _STRINGLIB_BUFFER_SIZE);
    if (!tokens_array) return NULL;
    size_t tokens_array_size = _STRINGLIB_BUFFER_SIZE;
    size_t tokens_array_wp = 0;

    char * string_copy = (char *) malloc(header->length + 1);
    strncpy(string_copy, STRING, header->length);

    char * token = strtok(string_copy, DELIMETERS_CHAR_ARRAY);
    bool failed = false;
    do {
        char * token_string = createString(token);
        if (OUTPUT_TOKEN_STRING_FORMAT == STRING_FORMAT_AS_IS) {
            tokens_array[tokens_array_wp++] = token_string;
        } else {
            tokens_array[tokens_array_wp++] = copyString(token_string, OUTPUT_TOKEN_STRING_FORMAT);
            freeString(token_string);
        }
        if (tokens_array_wp == tokens_array_size) {
            char ** new_tokens_array = (char **) realloc(tokens_array, sizeof(char *) * (tokens_array_size + _STRINGLIB_BUFFER_SIZE));
            if (!new_tokens_array) failed = true;
            else {
                tokens_array = new_tokens_array;
                tokens_array_size += _STRINGLIB_BUFFER_SIZE;
            }
        }
    } while (!failed && (token = strtok(NULL, DELIMETERS_CHAR_ARRAY)));

    if (failed) {
        freeStringTokensArray(tokens_array, tokens_array_wp);
        tokens_array = NULL;
    } else {
        *tokens_array_size_ptr = tokens_array_wp;
    }

    return tokens_array;
}