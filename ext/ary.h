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

#include "ruby_compat.h"

typedef struct
{
    int     count;
    int     max;
    int     *entries;
} ary_t;

// in the test suite array count goes no higher than 25 or 26
#define DEFAULT_ENTRY_COUNT 64

#define NO_ITEM(item) (item == INT_MAX)

ary_t *ary_new(void);
int ary_entry(ary_t *ary, int idx);
void ary_clear(ary_t *ary);
int ary_pop(ary_t *ary);
void ary_push(ary_t *ary, int val);
int ary_includes(ary_t *ary, int val);

// returns a count indicating the number of times the value appears in the collection
// refactored from _Wikitext_count()
int ary_count(ary_t *ary, int item);

void ary_free(ary_t *ary);
