#!/usr/bin/env ruby
# Copyright 2009 Wincent Colaiuta. All rights reserved.
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

require File.join(File.dirname(__FILE__), 'spec_helper.rb')
require 'wikitext/parser'

describe Wikitext::Parser, 'base_heading_level' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should default to 0' do
    @parser.base_heading_level.should == 0
  end

  it 'should be settable after initialization' do
    @parser.base_heading_level = 4
    @parser.base_heading_level.should == 4
  end

  it 'should be overrideable at initialization time' do
    parser = Wikitext::Parser.new :base_heading_level => 3
    parser.base_heading_level.should == 3
  end

  it 'should be overrideable at parse time' do
    lambda { @parser.parse 'foo', :base_heading_level => 2 }.should_not raise_error
  end

  it 'should complain if overridden with non-integer value' do
    lambda { @parser.parse 'foo', :base_heading_level => 'foo' }.should raise_error(TypeError)
    lambda {
      @parser.base_heading_level = 'bar'
      @parser.parse 'foo' # error actually gets raised only at parse time
    }.should raise_error(TypeError)
    lambda {
      parser = Wikitext::Parser.new :base_heading_level => 'baz'
      parser.parse 'foo'  # error actually gets raised only at parse time
    }.should raise_error(TypeError)
  end

  # these tested additionally below
  it 'should handle negative levels' do
    @parser.base_heading_level = -1
    @parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    @parser.base_heading_level = -2
    @parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    @parser.base_heading_level = -3
    @parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    @parser.base_heading_level = -4
    @parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    @parser.base_heading_level = -5
    @parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    @parser.base_heading_level = -6
    @parser.parse('== hello ==').should == "<h2>hello</h2>\n"
  end

  # these tested additionally below
  it 'should handle oversized levels (greater than six)' do
    @parser.base_heading_level = 7
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
    @parser.base_heading_level = 8
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
    @parser.base_heading_level = 9
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
    @parser.base_heading_level = 10
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
    @parser.base_heading_level = 11
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
    @parser.base_heading_level = 12
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
  end

  # here we use a negative value
  it 'should treat the three different override methods as equivalent (level: -1)' do
    @parser.base_heading_level = -1
    @parser.parse('= hello =').should == "<h1>hello</h1>\n"
    @parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    @parser.parse('=== hello ===').should == "<h3>hello</h3>\n"
    @parser.parse('==== hello ====').should == "<h4>hello</h4>\n"
    @parser.parse('===== hello =====').should == "<h5>hello</h5>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => -1).should == "<h1>hello</h1>\n"
    @parser.parse('== hello ==', :base_heading_level => -1).should == "<h2>hello</h2>\n"
    @parser.parse('=== hello ===', :base_heading_level => -1).should == "<h3>hello</h3>\n"
    @parser.parse('==== hello ====', :base_heading_level => -1).should == "<h4>hello</h4>\n"
    @parser.parse('===== hello =====', :base_heading_level => -1).should == "<h5>hello</h5>\n"
    @parser.parse('====== hello ======', :base_heading_level => -1).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => -1
    parser.parse('= hello =').should == "<h1>hello</h1>\n"
    parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    parser.parse('=== hello ===').should == "<h3>hello</h3>\n"
    parser.parse('==== hello ====').should == "<h4>hello</h4>\n"
    parser.parse('===== hello =====').should == "<h5>hello</h5>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  it 'should treat the three different override methods as equivalent (level: 0)' do
    @parser.base_heading_level = 0
    @parser.parse('= hello =').should == "<h1>hello</h1>\n"
    @parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    @parser.parse('=== hello ===').should == "<h3>hello</h3>\n"
    @parser.parse('==== hello ====').should == "<h4>hello</h4>\n"
    @parser.parse('===== hello =====').should == "<h5>hello</h5>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => 0).should == "<h1>hello</h1>\n"
    @parser.parse('== hello ==', :base_heading_level => 0).should == "<h2>hello</h2>\n"
    @parser.parse('=== hello ===', :base_heading_level => 0).should == "<h3>hello</h3>\n"
    @parser.parse('==== hello ====', :base_heading_level => 0).should == "<h4>hello</h4>\n"
    @parser.parse('===== hello =====', :base_heading_level => 0).should == "<h5>hello</h5>\n"
    @parser.parse('====== hello ======', :base_heading_level => 0).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => 0
    parser.parse('= hello =').should == "<h1>hello</h1>\n"
    parser.parse('== hello ==').should == "<h2>hello</h2>\n"
    parser.parse('=== hello ===').should == "<h3>hello</h3>\n"
    parser.parse('==== hello ====').should == "<h4>hello</h4>\n"
    parser.parse('===== hello =====').should == "<h5>hello</h5>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  it 'should treat the three different override methods as equivalent (level: 1)' do
    @parser.base_heading_level = 1
    @parser.parse('= hello =').should == "<h2>hello</h2>\n"
    @parser.parse('== hello ==').should == "<h3>hello</h3>\n"
    @parser.parse('=== hello ===').should == "<h4>hello</h4>\n"
    @parser.parse('==== hello ====').should == "<h5>hello</h5>\n"
    @parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => 1).should == "<h2>hello</h2>\n"
    @parser.parse('== hello ==', :base_heading_level => 1).should == "<h3>hello</h3>\n"
    @parser.parse('=== hello ===', :base_heading_level => 1).should == "<h4>hello</h4>\n"
    @parser.parse('==== hello ====', :base_heading_level => 1).should == "<h5>hello</h5>\n"
    @parser.parse('===== hello =====', :base_heading_level => 1).should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======', :base_heading_level => 1).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => 1
    parser.parse('= hello =').should == "<h2>hello</h2>\n"
    parser.parse('== hello ==').should == "<h3>hello</h3>\n"
    parser.parse('=== hello ===').should == "<h4>hello</h4>\n"
    parser.parse('==== hello ====').should == "<h5>hello</h5>\n"
    parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  it 'should treat the three different override methods as equivalent (level: 2)' do
    @parser.base_heading_level = 2
    @parser.parse('= hello =').should == "<h3>hello</h3>\n"
    @parser.parse('== hello ==').should == "<h4>hello</h4>\n"
    @parser.parse('=== hello ===').should == "<h5>hello</h5>\n"
    @parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => 2).should == "<h3>hello</h3>\n"
    @parser.parse('== hello ==', :base_heading_level => 2).should == "<h4>hello</h4>\n"
    @parser.parse('=== hello ===', :base_heading_level => 2).should == "<h5>hello</h5>\n"
    @parser.parse('==== hello ====', :base_heading_level => 2).should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====', :base_heading_level => 2).should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======', :base_heading_level => 2).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => 2
    parser.parse('= hello =').should == "<h3>hello</h3>\n"
    parser.parse('== hello ==').should == "<h4>hello</h4>\n"
    parser.parse('=== hello ===').should == "<h5>hello</h5>\n"
    parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  it 'should treat the three different override methods as equivalent (level: 3)' do
    @parser.base_heading_level = 3
    @parser.parse('= hello =').should == "<h4>hello</h4>\n"
    @parser.parse('== hello ==').should == "<h5>hello</h5>\n"
    @parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => 3).should == "<h4>hello</h4>\n"
    @parser.parse('== hello ==', :base_heading_level => 3).should == "<h5>hello</h5>\n"
    @parser.parse('=== hello ===', :base_heading_level => 3).should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====', :base_heading_level => 3).should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====', :base_heading_level => 3).should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======', :base_heading_level => 3).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => 3
    parser.parse('= hello =').should == "<h4>hello</h4>\n"
    parser.parse('== hello ==').should == "<h5>hello</h5>\n"
    parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  it 'should treat the three different override methods as equivalent (level: 4)' do
    @parser.base_heading_level = 4
    @parser.parse('= hello =').should == "<h5>hello</h5>\n"
    @parser.parse('== hello ==').should == "<h6>hello</h6>\n"
    @parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => 4).should == "<h5>hello</h5>\n"
    @parser.parse('== hello ==', :base_heading_level => 4).should == "<h6>hello</h6>\n"
    @parser.parse('=== hello ===', :base_heading_level => 4).should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====', :base_heading_level => 4).should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====', :base_heading_level => 4).should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======', :base_heading_level => 4).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => 4
    parser.parse('= hello =').should == "<h5>hello</h5>\n"
    parser.parse('== hello ==').should == "<h6>hello</h6>\n"
    parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  it 'should treat the three different override methods as equivalent (level: 5)' do
    @parser.base_heading_level = 5
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
    @parser.parse('== hello ==').should == "<h6>hello</h6>\n"
    @parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => 5).should == "<h6>hello</h6>\n"
    @parser.parse('== hello ==', :base_heading_level => 5).should == "<h6>hello</h6>\n"
    @parser.parse('=== hello ===', :base_heading_level => 5).should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====', :base_heading_level => 5).should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====', :base_heading_level => 5).should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======', :base_heading_level => 5).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => 5
    parser.parse('= hello =').should == "<h6>hello</h6>\n"
    parser.parse('== hello ==').should == "<h6>hello</h6>\n"
    parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  it 'should treat the three different override methods as equivalent (level: 6)' do
    @parser.base_heading_level = 6
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
    @parser.parse('== hello ==').should == "<h6>hello</h6>\n"
    @parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => 6).should == "<h6>hello</h6>\n"
    @parser.parse('== hello ==', :base_heading_level => 6).should == "<h6>hello</h6>\n"
    @parser.parse('=== hello ===', :base_heading_level => 6).should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====', :base_heading_level => 6).should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====', :base_heading_level => 6).should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======', :base_heading_level => 6).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => 6
    parser.parse('= hello =').should == "<h6>hello</h6>\n"
    parser.parse('== hello ==').should == "<h6>hello</h6>\n"
    parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  # here we exceed the limit
  it 'should treat the three different override methods as equivalent (level: 7)' do
    @parser.base_heading_level = 7
    @parser.parse('= hello =').should == "<h6>hello</h6>\n"
    @parser.parse('== hello ==').should == "<h6>hello</h6>\n"
    @parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======').should == "<h6>hello</h6>\n"

    @parser.parse('= hello =', :base_heading_level => 7).should == "<h6>hello</h6>\n"
    @parser.parse('== hello ==', :base_heading_level => 7).should == "<h6>hello</h6>\n"
    @parser.parse('=== hello ===', :base_heading_level => 7).should == "<h6>hello</h6>\n"
    @parser.parse('==== hello ====', :base_heading_level => 7).should == "<h6>hello</h6>\n"
    @parser.parse('===== hello =====', :base_heading_level => 7).should == "<h6>hello</h6>\n"
    @parser.parse('====== hello ======', :base_heading_level => 7).should == "<h6>hello</h6>\n"

    parser = Wikitext::Parser.new :base_heading_level => 7
    parser.parse('= hello =').should == "<h6>hello</h6>\n"
    parser.parse('== hello ==').should == "<h6>hello</h6>\n"
    parser.parse('=== hello ===').should == "<h6>hello</h6>\n"
    parser.parse('==== hello ====').should == "<h6>hello</h6>\n"
    parser.parse('===== hello =====').should == "<h6>hello</h6>\n"
    parser.parse('====== hello ======').should == "<h6>hello</h6>\n"
  end

  # for bad markup, we adjust any headings that we output (h2 -> h4),
  # but we show the bad markup (===) as it was entered (not-adjusted)
  it 'should show unbalanced markup exactly as it was entered' do
    @parser.base_heading_level = 2

    # missing trailing =
    @parser.parse('== hello =').should == "<h4>hello =</h4>\n"

    # excess trailing =
    @parser.parse('== hello ===').should == "<h4>hello ===</h4>\n"
  end

  it 'should show markup in <nowiki> spans exactly as it was entered' do
    @parser.base_heading_level = 3
    @parser.parse("<nowiki>\n== foo ==\n</nowiki>").should == "<p>\n== foo ==\n</p>\n"
  end

  it 'should show markup in <pre> blocks exactly as it was entered' do
    @parser.base_heading_level = 1
    @parser.parse("<pre>\n== bar ==\n</pre>").should == "<pre>\n== bar ==\n</pre>\n"
  end
end
