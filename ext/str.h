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
    char *ptr;
    long len;
    long capacity;
} str_t;

// Mark the str struct designated by ptr as a participant in Ruby's mark-and-sweep garbage collection scheme.
// A variable named name is placed on the C stack to prevent the structure from being prematurely collected.
#define GC_WRAP_STR(ptr, name) volatile VALUE name __attribute__((unused)) = Data_Wrap_Struct(rb_cObject, 0, str_free, ptr)

// create a new, empty string struct
str_t *str_new(void);

// create a new, empty string struct with capacity len
str_t *str_new_size(long len);

// create a new string struct and initialize it with a copy of the buffer of length len pointed to by src
str_t *str_new_copy(char *src, long len);

// create a new string struct and initialize it with the buffer of length len pointed to by src
// no copy is made; the struct takes ownership of the buffer and will free it when the struct is disposed of
str_t *str_new_no_copy(char *src, long len);

// convenience method for testing
str_t *str_new_from_string(VALUE string);

// convenience method for testing
VALUE string_from_str(str_t *str);

// grows a string's capacity to the specified length
void str_grow(str_t *str, long len);

void str_append(str_t *str, char *src, long len);

// appends the "other" string struct onto str
void str_append_str(str_t *str, str_t *other);

// this is a temporary convenience measure
// later on if I develop in-place variants of some functions this won't be needed
void str_swap(str_t **a, str_t **b);

// don't actually free the memory yet
// this makes str structs very useful when reusing buffers because it avoids reallocation
void str_clear(str_t *str);

void str_free(str_t *str);
