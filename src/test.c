/*  test.c 
    Test suite for my string library
    (C)2024 Morgan Evans */

#include "libstring.h"

int main(int argc, char * argv[])
{
    char * new_string = createString(" Hello world! ");
    printf("String '%s' is %lu characters long.\n", new_string, getStringLength(new_string));
    char * lower_case_new_string =  copyString(new_string, STRING_FORMAT_LOWERCASE);
    printf("Lower case of '%s' is '%s'.\n", new_string, lower_case_new_string);
    char * upper_case_new_string = copyString(new_string, STRING_FORMAT_UPPERCASE | STRING_FORMAT_TRIM);
    printf("Trimmed upper case version of '%s' is '%s'.\n", new_string, upper_case_new_string);
    printf("Case sensitive match of '%s' and '%s' is %s.\n", new_string, lower_case_new_string, 
        compareStrings(new_string, lower_case_new_string, STRING_FORMAT_CASE_SENSITIVE) ? "true" : "false");
    printf("Case insensitive match of '%s' and '%s' is %s.\n", new_string, lower_case_new_string, 
        compareStrings(new_string, lower_case_new_string, STRING_FORMAT_CASE_INSENSITIVE) ? "true" : "false");

    size_t tokens_array_size;
    char delimeters[] = " ";
    char ** tokens_array = createStringTokensArray(new_string, delimeters, STRING_FORMAT_TRIM | STRING_FORMAT_LOWERCASE, &tokens_array_size);
    printf("Got %lu trimmed, lower case, string tokens:\n", tokens_array_size);
    for (size_t i = 0; i < tokens_array_size; i++) {
        printf("\t'%s'\n", tokens_array[i]);
    }
    freeStringTokensArray(tokens_array, tokens_array_size);

    freeString(new_string);
    freeString(lower_case_new_string);
    freeString(upper_case_new_string);
    return 0;
}