# Copyright 2008-2009 Wincent Colaiuta
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

# Avoid Rails bug #2266 by not requiring during "rake gems:build"
# See: https://rails.lighthouseapp.com/projects/8994/tickets/2266
unless $gems_build_rake_task
  require 'wikitext/nil_class'
  require 'wikitext/string'
  require 'wikitext/rails'
end
