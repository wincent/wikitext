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

inline VALUE ary_new(void)
{
    return rb_ary_new();
}

inline VALUE ary_entry(VALUE ary, long idx)
{
    return rb_ary_entry(ary, idx);
}

inline VALUE ary_delete_at(VALUE ary, long idx)
{
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
