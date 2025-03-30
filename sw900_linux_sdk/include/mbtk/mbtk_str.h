#ifndef MBTK_STR_INCLUDE
#define MBTK_STR_INCLUDE

#include "mbtk_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
* Converts all of the characters in this String to lower.
*
* Parameters:
*   src - The string should be converted.
*   dest  - The lowercase string.
*   len - The length of result string.Must be "strlen(src) + 1"
* Returns:
*   The string, converted to lowercase,or NULL for fail.
*/
void*
str_tolower
(
    const void *src,
    void *dest,
    size_t len
);

/*
* Converts all of the characters in this String to upper case.
*
* Parameters:
*   src - The string should be converted.
*   dest  - The uppercase string.
*   len - The length of result string.Must be "strlen(str_ptr) + 1"
* Returns:
*   The string, converted to uppercase.or NULL for fail.
*/
void*
str_toupper
(
    const void *src,
    void *dest,
    size_t len
);

/*
* Remove the head and tail spaces.
*/
void*
str_trim
(
    const void* str,
    void *result,
    size_t len
);

/*
* Returns true if and only if this string contains the specified sequence of char values.
*
* Parameters:
*   str   -   The substring to search from.
*   sub_str - The substring to search for.
* Returns:
*   True if str contains sub_str, false otherwise.
*/
bool
str_contains
(
    const void* str,
    const void* sub_str
);

/*
* Returns the index within this string of the first occurrence of the specified substring.
* If no such substring, then -1 is returned.
*
* Parameters:
*   str   -   The substring to search from.
*   sub_str - The substring to search for.
* Returns:
*   The index of the first occurrence of the specified substring,
*   or -1 if there is no such occurrence.
*/
ssize_t
str_indexof
(
    const void* str,
    const void* sub_str
);

/*
* Returns a new string that is a substring of this string. The substring begins
* at the specified beginIndex and extends to the character at index endIndex - 1.
* Thus the length of the substring is endIndex-beginIndex.
*
* Examples:
*   "hamburger".substring(4, 8) returns "urge"
*   "smiles".substring(1, 5) returns "mile"
*
* Parameters:
*   begin_index     The beginning index, inclusive.
*   end_index       The ending index, exclusive.
* Returns:
*   The specified substring or NULL.
*/
void*
str_substring
(
    const void* str,
    size_t begin_index,
    size_t end_index,
    void *sub_str,
    size_t len
);

bool str_startwith(const char* str, const void* prefix);

void* strstr_hex(char *haystack,int haystack_len,
                    const char *needle,int needle_len);

bool str_empty(const void *str);

#ifdef __cplusplus
} // extern "C"
#endif
#endif /* MBTK_STR_INCLUDE */
