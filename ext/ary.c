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

#include "ary.h"

ary_t *ary_new(void)
{
    ary_t *ary      = ALLOC_N(ary_t, 1);
    ary->count      = 0;
    ary->max        = DEFAULT_ENTRY_COUNT;
    ary->entries    = ALLOC_N(int, DEFAULT_ENTRY_COUNT);
    return ary;
}

int ary_entry(ary_t *ary, int idx)
{
    if (idx < 0)
        idx = ary->count + idx;
    return (idx >= 0 && ary->count > idx) ? ary->entries[idx] : INT_MAX;
}

void ary_clear(ary_t *ary)
{
    ary->count = 0;
}

int ary_pop(ary_t *ary)
{
    if (ary->count > 0)
    {
        ary->count--;
        return 1;
    }
    return 0;
}

void ary_push(ary_t *ary, int val)
{
    if (ary->count == ary->max)
    {
        ary->max += DEFAULT_ENTRY_COUNT;
        REALLOC_N(ary->entries, int, ary->max);
    }
    ary->entries[ary->count] = val;
    ary->count++;
}

int ary_includes(ary_t *ary, int val)
{
    for (int i = 0, max = ary->count; i < max; i++)
    {
        if (ary->entries[i] == val)
            return 1;
    }
    return 0;
}

int ary_count(ary_t *ary, int item)
{
    int count = 0;
    for (int i = 0, max = ary->count; i < max; i++)
    {
        if (ary->entries[i] == item)
            count++;
    }
    return count;
}

void ary_free(ary_t *ary)
{
    free(ary->entries);
    free(ary);
}
