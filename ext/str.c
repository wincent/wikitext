// Copyright 2008 Wincent Colaiuta
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "str.h"

str_t *str_new(void)
{
    str_t *str      = ALLOC_N(str_t, 1);
    str->ptr        = NULL;
    str->len        = 0;
    str->capacity   = 0;
    return str;
}

str_t *str_new_size(long len)
{
    str_t *str      = ALLOC_N(str_t, 1);
    str->ptr        = ALLOC_N(char, len);
    str->len        = 0;
    str->capacity   = len;
    return str;
}

str_t *str_new_copy(char *src, long len)
{
    str_t *str      = ALLOC_N(str_t, 1);
    str->ptr        = ALLOC_N(char, len);
    memcpy(str->ptr, src, len);
    str->len        = len;
    str->capacity   = len;
    return str;
}

str_t *str_new_no_copy(char *src, long len)
{
    str_t *str      = ALLOC_N(str_t, 1);
    str->ptr        = src;
    str->len        = len;
    str->capacity   = len;
    return str;
}

str_t *str_new_from_string(VALUE string)
{
    string = StringValue(string);
    return str_new_copy(RSTRING_PTR(string), RSTRING_LEN(string));
}

VALUE string_from_str(str_t *str)
{
    return rb_str_new(str->ptr, str->len);
}

void str_grow(str_t *str, long len)
{
    if (str->capacity < len)
    {
        if (str->ptr)
            REALLOC_N(str->ptr, char, len);
        else
            str->ptr = ALLOC_N(char, len);
        str->capacity = len;
    }
}

void str_append(str_t *str, char *src, long len)
{
    long new_len = str->len + len;
    if (str->capacity < new_len)
    {
        if (str->ptr)
            REALLOC_N(str->ptr, char, new_len);
        else
            str->ptr = ALLOC_N(char, new_len);
        str->capacity = new_len;
    }
    memcpy(str->ptr + str->len, src, len);
    str->len = new_len;
}

void str_append_str(str_t *str, str_t *other)
{
    str_append(str, other->ptr, other->len);
}

void str_swap(str_t **a, str_t **b)
{
    str_t *c;
    c = *a;
    *a = *b;
    *b = c;
}

void str_clear(str_t *str)
{
    str->len = 0;
}

void str_free(str_t *str)
{
    if (str->ptr)
        free(str->ptr);
    free(str);
}
