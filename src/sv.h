
#ifndef SV_H
#define SV_H

#include <stdint.h>
#include <stddef.h>

typedef int64_t Word;

typedef struct
{
    size_t count;
    char *data;
} String_view;

// Function declarations
String_view sv_from_cstr(char *cstr);
String_view sv_trim_left(String_view sv);
String_view sv_trim_right(String_view sv);
String_view sv_trim(String_view sv);
String_view sv_chop_by_delim(String_view *sv, char delim);

Word sv_to_int(String_view sv);

int sv_equal(String_view a, String_view b);

#endif // SV_H
