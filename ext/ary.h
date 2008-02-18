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

#include <ruby.h>

typedef struct
{
    int     count;
    int     max;
    int     *entries;
} ary_t;

// in the test suite array count goes no higher than 25 or 26
#define DEFAULT_ENTRY_COUNT 64

#define NO_ITEM(item) (item == INT_MAX)

inline ary_t *ary_new(void)
{
    ary_t *ary      = ALLOC_N(ary_t, 1);
    ary->count      = 0;
    ary->max        = DEFAULT_ENTRY_COUNT;
    ary->entries    = ALLOC_N(int, DEFAULT_ENTRY_COUNT);
    return ary;
}

inline void ary_free(ary_t *ary)
{
    free(ary->entries);
    free(ary);
}

inline int ary_entry(ary_t *ary, int idx)
{
    if (idx < 0)
        idx = ary->count + idx;
    return (idx >= 0 && ary->count > idx) ? ary->entries[idx] : INT_MAX;
}

inline void ary_clear(ary_t *ary)
{
    ary->count = 0;
}

inline int ary_pop(ary_t *ary)
{
    if (ary->count > 0)
    {
        ary->count--;
        return 1;
    }
    return 0;
}

inline void ary_push(ary_t *ary, int val)
{
    if (ary->count == ary->max)
    {
        ary->max += DEFAULT_ENTRY_COUNT;
        REALLOC_N(ary->entries, int, ary->max);
    }
    ary->entries[ary->count] = val;
    ary->count++;
}

inline int ary_includes(ary_t *ary, int val)
{
    for (int i = 0, max = ary->count; i < max; i++)
    {
        if (ary->entries[i] == val)
            return 1;
    }
    return 0;
}

// returns a count indicating the number of times the value appears in the collection
// refactored from _Wikitext_count()
inline int ary_count(ary_t *ary, int item)
{
    int count = 0;
    for (int i = 0, max = ary->count; i < max; i++)
    {
        if (ary->entries[i] == item)
            count++;
    }
    return count;
}

