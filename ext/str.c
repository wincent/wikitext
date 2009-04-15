// Copyright 2008-2009 Wincent Colaiuta. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

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
