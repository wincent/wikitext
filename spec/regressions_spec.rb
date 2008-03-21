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

require File.join(File.dirname(__FILE__), 'spec_helper.rb')
require 'wikitext'

# this is a general-purpose file in which I'll add specs for former bugs to make sure that they don't regress
describe Wikitext::Parser, 'regressions' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should correctly transform example #1' do
    # turns out that this was never a bug in wikitext: it was a bug in the host application
    input = dedent <<-END
      = Leopard =
      
      * punto 1
      * punto 2
      
      Y [[otro articulo]].
    END
    expected = dedent <<-END
      <h1>Leopard</h1>
      <ul>
        <li>punto 1</li>
        <li>punto 2</li>
      </ul>
      <p>Y <a href="/wiki/otro%20articulo">otro articulo</a>.</p>
    END
    @parser.parse(input).should == expected
  end

  # this one discovered in a real Rails application
  it 'should allow empty lines in PRE blocks marked up with a leading space' do
    input = dedent <<-END
       # -d turns on debug mode: output is verbose, no actual changes are made to the log files
       sudo logrotate -d /etc/logrotate.d/nginx
       
       # if the debug output looks good, proceed with a real rotation (-v turns on verbose output)
       sudo logrotate -v /etc/logrotate.d/nginx
    END
    expected = dedent <<-END
      <pre># -d turns on debug mode: output is verbose, no actual changes are made to the log files
      sudo logrotate -d /etc/logrotate.d/nginx
      
      # if the debug output looks good, proceed with a real rotation (-v turns on verbose output)
      sudo logrotate -v /etc/logrotate.d/nginx</pre>
    END
    @parser.parse(input).should == expected
  end
end
