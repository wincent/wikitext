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

  # turns out that this was never a bug in wikitext -- it was a bug in the host application -- but keeping the test does no harm
  it 'should correctly transform example #1' do
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

  # discovered at: http://rails.wincent.com/wiki/nginx_log_rotation
  # fixed by 0a328f1
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

  # discovered at: http://rails.wincent.com/wiki/Installing_Ragel_5.2.0_on_Mac_OS_X_Tiger
  # fixed by ?
  it 'should handle PRE_START blocks which follow unordered lists' do
    input = dedent <<-END
      * Get link to latest source code from: http://www.cs.queensu.ca/~thurston/ragel/
      
      <pre>wget http://www.cs.queensu.ca/~thurston/ragel/ragel-5.20.tar.gz
      tar xzvf ragel-5.20.tar.gz
      cd ragel-5.20</pre>
    END
    expected = dedent <<-END
      <ul>
        <li>Get link to latest source code from: <a href="http://www.cs.queensu.ca/~thurston/ragel/" class="external">http://www.cs.queensu.ca/~thurston/ragel/</a></li>
      </ul>
      <pre>wget <a href="http://www.cs.queensu.ca/~thurston/ragel/ragel-5.20.tar.gz" class="external">http://www.cs.queensu.ca/~thurston/ragel/ragel-5.20.tar.gz</a>
      tar xzvf ragel-5.20.tar.gz
      cd ragel-5.20</pre>
    END
    @parser.parse(input).should == expected
  end

  # discovered at: http://rails.wincent.com/wiki/Movable_Type_security_notes
  # fixed by ?
  it 'should handle PRE_START blocks which follow unordered lists' do
    input = dedent <<-END
      # Turn off the [[Movable Type]] search function; use Google instead (it's better anyway) with a form something like this:
      
      <pre><form method="get"...></pre>
    END
    expected = dedent <<-END
      <ol>
        <li>Turn off the <a href="/wiki/Movable%20Type">Movable Type</a> search function; use Google instead (it's better anyway) with a form something like this:</li>
      </ol>
      <pre>&lt;form method=&quot;get&quot;...&gt;</pre>
    END
    @parser.parse(input).should == expected
  end

  # discovered at: http://rails.wincent.com/wiki/Movable_Type_security_notes
  # fixed by ?
  it 'should respect additional indentation found inside PRE blocks' do
    pending

    # note the two extra spaces on each line
    input = dedent <<-END
        <input type="text" name="q" size="20" maxlength="255" value="" />
        <input type="hidden" name="hl" value="en" />
    END

    # first line had only one additional space
    # second line had no additional spaces at all
    expected = dedent <<-END
      <pre>  &lt;input type="text" name="q" size="20" maxlength="255" value="" /&gt;
        &lt;input type="hidden" name="hl" value="en" /&gt;</pre>
    END
    @parser.parse(input).should == expected
  end
end
