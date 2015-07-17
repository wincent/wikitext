// Copyright 2008-present Greg Hurrell. All rights reserved.
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

// when allocating memory, reserve a little more than was asked for,
// which can help to avoid subsequent allocations
#define STR_OVERALLOC 256

str_t *str_new(void)
{
    str_t *str      = ALLOC_N(str_t, 1);
    str->ptr        = NULL;
    str->len        = 0;
    str->capacity   = 0;
    return str;
}

str_t *str_new_copy(const char *src, long len)
{
    str_t *str      = ALLOC_N(str_t, 1);
    str->ptr        = ALLOC_N(char, len + STR_OVERALLOC);
    memcpy(str->ptr, src, len);
    str->len        = len;
    str->capacity   = len + STR_OVERALLOC;
    return str;
}

str_t *str_new_from_string(VALUE string)
{
    string = StringValue(string);
    return str_new_copy(RSTRING_PTR(string), RSTRING_LEN(string));
}

VALUE string_from_str(str_t *str)
{
    VALUE string = rb_str_new(str->ptr, str->len);
    rb_funcall(string, rb_intern("force_encoding"), 1, rb_str_new2("UTF-8"));
    return string;
}

void str_grow(str_t *str, long len)
{
    if (str->capacity < len)
    {
        if (str->ptr)
            REALLOC_N(str->ptr, char, len + STR_OVERALLOC);
        else
            str->ptr = ALLOC_N(char, len + STR_OVERALLOC);
        str->capacity = len + STR_OVERALLOC;
    }
}

void str_append(str_t *str, const char *src, long len)
{
    long new_len = str->len + len;
    if (str->capacity < new_len)
    {
        if (str->ptr)
            REALLOC_N(str->ptr, char, new_len + STR_OVERALLOC);
        else
            str->ptr = ALLOC_N(char, new_len + STR_OVERALLOC);
        str->capacity = new_len + STR_OVERALLOC;
    }
    memcpy(str->ptr + str->len, src, len);
    str->len = new_len;
}

void str_append_str(str_t *str, str_t *other)
{
    str_append(str, other->ptr, other->len);
}

void str_append_string(str_t *str, VALUE other)
{
    str_append(str, RSTRING_PTR(other), RSTRING_LEN(other));
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
