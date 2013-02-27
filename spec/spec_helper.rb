# Copyright 2007-2013 Wincent Colaiuta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

require 'pathname'
require 'rspec'

# allow indenting of multiline spec data for better readability
# but must dedent it before actually doing the comparison
def dedent(spaces, string = nil)
  if spaces.kind_of? String
    if !string.nil?
      raise 'When first argument is a String, second argument must be nil'
    else
      # default use: single String parameter, dedent by 6
      string = spaces
      spaces = 6
    end
  elsif spaces.kind_of? Integer
    if string.nil? || !string.kind_of?(String)
      raise 'When first argument is a number, second must be a String'
    end
  else
    raise 'Invalid argument'
  end
  string.each_line do |line|
    if line !~ /\A {#{spaces.to_i}}/
      raise "Underlength indent for line: #{line.inspect}"
    end
  end
  string.gsub /^ {#{spaces.to_i}}/, ''
end

# prepend local directories to search path if not already present
basedir     = Pathname.new(__dir__) + '..'
extdir      = (basedir + 'ext').realpath
libdir      = (basedir + 'lib').realpath
normalized  = $:.map { |path| Pathname.new(path).realpath rescue path }
[libdir, extdir].each { |d| $:.unshift(d) unless normalized.include?(d) }

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

ENV['RUBYOPT'] = nil
