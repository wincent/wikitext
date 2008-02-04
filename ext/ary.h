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

#include <ruby/ruby.h>

typedef struct
{
    int     count;
    int     max;
    int     *entries;
} ary_t;

inline VALUE ary_new(void)
{
    return rb_ary_new();
}

inline VALUE ary_entry(VALUE ary, long idx)
{
    return rb_ary_entry(ary, idx);
}

#ifdef DEBUG
long deleted = 0;
#endif

// deleting from end: just adjust count
// deleting from middle: insert marker
inline VALUE ary_delete_at(VALUE ary, long idx)
{
#ifdef DEBUG
    // called 2086 times in all
    // of these, only 18 calls have an idx parameter other than -1
    // and in all 18 cases we're still only deleting the last item in the array
    // so we only have to worry about optimizing for that case
    // will support deletions prior to the end by inserting a special value rather than moving memory around
    if (idx != -1)
    {
        deleted++;
        printf("deleting %d at idx %d (len is %d)\n", deleted, idx, RARRAY_LEN(ary));
    }
#endif /* DEBUG */
    return rb_ary_delete_at(ary, idx);
}

#ifdef DEBUG
long max_len = 0;
#endif

inline VALUE ary_push(VALUE ary, VALUE obj)
{
    VALUE ret = rb_ary_push(ary, obj);
#ifdef DEBUG
    if (RARRAY_LEN(ary) > max_len)
    {
        // in typical work loads the array length goes no higher than 25 or 26
        max_len++;
        printf("max len %d\n", max_len);
    }
#endif /* DEBUG */
    return ret;
}

inline VALUE ary_includes(VALUE ary, VALUE obj)
{
    return rb_ary_includes(ary, obj);
}
