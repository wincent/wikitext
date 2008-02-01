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
    EXTDIR      = Pathname.new(base).realpath
    normalized  = $:.collect { |path| Pathname.new(path).realpath rescue path }
    $:.push(EXTDIR) unless normalized.include?(EXTDIR)
  end
end # module Wikitext

require 'wikitext'
