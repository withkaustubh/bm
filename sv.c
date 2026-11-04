
#include "sv.h"

#include <ctype.h>
#include <string.h>
// #include<stdio.h>

// #define SV_fmt "%.*s"
// #define SV_arg(x) (int)x.count, x.data

String_view sv_from_cstr(char *cstr)
{
    return (String_view){
        .count = strlen(cstr),
        .data = cstr
    };
}

String_view sv_trim_left(String_view sv)
{
    while (sv.count > 0 && isspace((unsigned char)*sv.data))
    {
        sv.data++;
        sv.count--;
    }

    return sv;
}

String_view sv_trim_right(String_view sv)
{
    while (sv.count > 0 &&
           isspace((unsigned char)sv.data[sv.count - 1]))
    {
        sv.count--;
    }

    return sv;
}

String_view sv_trim(String_view sv)
{
    return sv_trim_right(sv_trim_left(sv));
}

String_view sv_chop_by_delim(String_view *sv, char delim)
{
    size_t i = 0;

    while (i < sv->count && sv->data[i] != delim)
    {
        ++i;
    }

    String_view result = {
        .count = i,
        .data = sv->data
    };

    if (i < sv->count)
    {
        sv->data += i + 1;
        sv->count -= i + 1;
    }
    else
    {
        sv->data += i;
        sv->count = 0;
    }

    return result;
}

Word sv_to_int(String_view sv)
{
    Word result = 0;
    Word sign = 1;

    size_t i = 0;

    if (sv.count > 0 && sv.data[0] == '-')
    {
        sign = -1;
        i++;
    }

    for (; i < sv.count && isdigit((unsigned char)sv.data[i]); ++i)
    {
        result = result * 10 + (sv.data[i] - '0');
    }

    return result * sign;
}

int sv_equal(String_view a, String_view b)
{
    if (a.count != b.count)
        return 0;

    return memcmp(a.data, b.data, a.count) == 0;
}

// int main(){
//     char *mychar = "hello world";
//     String_view msv = sv_from_cstr(mychar);

//     String_view second = sv_chop_by_delim(&msv, ':');
//     printf("first is " SV_fmt"\n", SV_arg(msv));
//      printf("second is " SV_fmt"\n", SV_arg(second));
    

// }