#!/usr/bin/env ruby
# Copyright 2008 Wincent Colaiuta
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

require File.join(File.dirname(__FILE__), '..', 'ext', 'wikitext')

# 1415 characters, 1415 bytes
longer_slab_of_ASCII_text = <<SLAB
paragraph
second line

new paragraph

= a heading =

> a blockquote
> second line of blockquote
>
> new paragraph within blockquote

== another heading ==

paragraph within ''multiple '''styles''''' and <tt>tt span</tt>

similar, but with '''styles in ''different'' order'''

again, a '''different ''order'''''

* list item 1
** nested list item 1
** nested list item 2
** nested list item 3
* list item 2

 // this is a code block
 notice how it can contain ''markup''
 which would '''otherwise''' have <tt>special</tt> meaning
 although explicit entities &copy; are passed through unchanged

a normal paragraph again

This is where we show a link to an article on [[GCC]].
Related to that, [[GCC|a link]] to the same
article but with custom link text.

External links [http://example.com work too].
As well as autolinks as seen http://example.com/
here.

Look at how we handle bad syntax. [[This is an unterminated
link. And [http://example.com/ is another.

# this is an ordered list
# which continues
## and has another ordered list
## nested inside it
# and then falls back
#* and then nests another list
#* this time an unordered one
#** itself containing a nested list
#** which continues
#**# and finally nests yet another ordered list
#**# which continues
#* drops back quite a way
# and finally all the way
#****** and finishes with an invalid item

=== heading with missing closing tag
* list
# new list
SLAB

# the purpose of this file is to test memory usage in the face of invalid input
# we want to confirm that all resources are being freed, including C structs,
# if we exit abnormally from the parse method (via an exception)
input = longer_slab_of_ASCII_text + [0b11110101, 0x10111111, 0x10111111, 0x10111111].pack('C*')

# sanity check: confirm that the input really is bad
parser = Wikitext::Parser.new
input_is_bad = false
begin
  parser.parse input
rescue Wikitext::Parser::Error
  input_is_bad = true
end
raise unless input_is_bad

puts "Starting"
100.times do
  STDERR.print "."
  1_000.times do
    begin
      parser.parse input
    rescue Wikitext::Parser::Error
    end
  end
end
puts "\nDone"
