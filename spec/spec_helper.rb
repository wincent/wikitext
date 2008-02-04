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

require 'pathname'
require 'rubygems'
require 'spec'

module Wikitext
  if not const_defined? 'EXTDIR'
    # append the local "ext" directory to search path if not already present
    base        = File.join(File.dirname(__FILE__), '..')
    EXTDIR      = Pathname.new(File.join(base, 'ext')).realpath
    normalized  = $:.collect { |path| Pathname.new(path).realpath rescue path }
    $:.push(EXTDIR) unless normalized.include?(EXTDIR)
  end
end # module Wikitext

module UTF8
  module Invalid
    TWO_BYTES_MISSING_SECOND_BYTE       = [0b11011111].pack('C*')
    TWO_BYTES_MALFORMED_SECOND_BYTE     = [0b11011111, 0b00001111].pack('C*') # should be 10......
    OVERLONG                            = [0b11000000, 0b10000000].pack('C*') # lead byte is 110..... but code point is <= 127
    OVERLONG_ALT                        = [0b11000001, 0b10000000].pack('C*') # lead byte is 110..... but code point is <= 127
    THREE_BYTES_MISSING_SECOND_BYTE     = [0b11100000].pack('C*')
    THREE_BYTES_MISSING_THIRD_BYTE      = [0b11100000, 0b10000000].pack('C*')
    THREE_BYTES_MALFORMED_SECOND_BYTE   = [0b11100000, 0b00001111, 0b10000000].pack('C*') # should be 10......
    THREE_BYTES_MALFORMED_THIRD_BYTE    = [0b11100000, 0b10000000, 0b00001111].pack('C*') # should be 10......
    FOUR_BYTES_MISSING_SECOND_BYTE      = [0b11110000].pack('C*')
    FOUR_BYTES_MISSING_THIRD_BYTE       = [0b11110000, 0x10111111].pack('C*')
    FOUR_BYTES_MISSING_FOURTH_BYTE      = [0b11110000, 0x10111111, 0x10111111].pack('C*')
    FOUR_BYTES_ILLEGAL_FIRST_BYTE       = [0b11110101, 0x10111111, 0x10111111, 0x10111111].pack('C*')
    FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT   = [0b11110101, 0x10111111, 0x10111111, 0x10111111].pack('C*')
    FOUR_BYTES_ILLEGAL_FIRST_BYTE_ALT2  = [0b11110101, 0x10111111, 0x10111111, 0x10111111].pack('C*')
    UNEXPECTED_BYTE                     = [0b11111000].pack('C*')
  end # module Invalid
end # module UTF8

require 'wikitext'
