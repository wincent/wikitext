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

#include "ruby_compat.h"

typedef struct
{
    char *ptr;
    long len;
    long capacity;
} str_t;

// Mark the str struct designated by ptr as a participant in Ruby's mark-and-sweep garbage collection scheme.
// A variable named name is placed on the C stack to prevent the structure from being prematurely collected.
#define GC_WRAP_STR(ptr, name) volatile VALUE name __attribute__((unused)) = Data_Wrap_Struct(rb_cObject, 0, str_free, ptr)

// create a new, empty string struct
str_t *str_new(void);

// create a new string struct and initialize it with a copy of the buffer of length len pointed to by src
str_t *str_new_copy(const char *src, long len);

// convenience method for testing
str_t *str_new_from_string(VALUE string);

// convenience method for testing
VALUE string_from_str(str_t *str);

// grows a string's capacity to the specified length
void str_grow(str_t *str, long len);

void str_append(str_t *str, const char *src, long len);

// appends the "other" string struct onto str
void str_append_str(str_t *str, str_t *other);

// appends the "other" string (a Ruby String) onto str
void str_append_string(str_t *str, VALUE other);

// don't actually free the memory yet
// this makes str structs very useful when reusing buffers because it avoids reallocation
void str_clear(str_t *str);

void str_free(str_t *str);
