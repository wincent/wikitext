# Copyright 2007-2008 Wincent Colaiuta
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

require 'mkmf'

def missing item
  puts "couldn't find #{item} (required)"
  exit 1
end

have_header('ruby.h') or missing 'ruby.h'
have_header('antlr3.h') or missing 'antlr3.h'
have_library('antlr3c', 'antlr3ParserNew') or missing 'antlr3ParserNew function (antlr3c library)'
create_makefile('wikitext')

