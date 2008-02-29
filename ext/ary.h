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

// Mark the ary struct designated by ptr as a participant in Ruby's mark-and-sweep garbage collection scheme.
// A variable named name is placed on the C stack to prevent the structure from being prematurely collected.
#define GC_WRAP_ARY(ptr, name) volatile VALUE name __attribute__((unused)) = Data_Wrap_Struct(rb_cObject, 0, ary_free, ptr)

ary_t *ary_new(void);
int ary_entry(ary_t *ary, int idx);
void ary_clear(ary_t *ary);
int ary_pop(ary_t *ary);
void ary_push(ary_t *ary, int val);
int ary_includes(ary_t *ary, int val);

// returns a count indicating the number of times the value appears in the collection
// refactored from _Wikitext_count()
int ary_count(ary_t *ary, int item);

// this method not inlined so its address can be passed to the Data_Wrap_Struct function.
void ary_free(ary_t *ary);
