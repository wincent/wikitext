# Copyright 2008-present Greg Hurrell. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
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

require 'spec_helper'

# this is the inverse of the dedent method in spec_helper.rb
# it's only in this file because it isn't needed anywhere else
def indent spaces, string
  string.gsub /^/, ' ' * spaces
end

def nodent string
  string.gsub /^ */, ''
end

describe Wikitext::Parser, 'indentation' do
  before do
    @parser         = Wikitext::Parser.new
    @input          = '* foo'
    @default_output = dedent <<-END
      <ul>
        <li>foo</li>
      </ul>
    END
  end

  it 'should default to no additional indentation' do
    @parser.parse(@input).should == @default_output
  end

  it 'should add additional indentation as indicated by the "indent" option' do
    @parser.parse(@input, :indent => 1).should == indent(1, @default_output)
    @parser.parse(@input, :indent => 2).should == indent(2, @default_output)
    @parser.parse(@input, :indent => 3).should == indent(3, @default_output)
    @parser.parse(@input, :indent => 4).should == indent(4, @default_output)
    @parser.parse(@input, :indent => 5).should == indent(5, @default_output)
    @parser.parse(@input, :indent => 6).should == indent(6, @default_output)
  end

  it 'should perform no indentation when "indent" is "false"' do
    @parser.parse(@input, :indent => false).should == nodent(@default_output)
  end

  it 'should complain if the "indent" option is nil' do
    lambda { @parser.parse(@input, :indent => nil) }.should raise_error(TypeError)
  end

  it 'should complain if the "indent" options is not an integer' do
    lambda { @parser.parse(@input, :indent => 'bar') }.should raise_error(TypeError)
    lambda { @parser.parse(@input, :indent => /baz/) }.should raise_error(TypeError)
  end

  it 'should treat a negative "indent" as though it were zero' do
    @parser.parse(@input, :indent => -4).should == @default_output
  end

  it 'should coerce a float "indent" into an integer' do
    @parser.parse(@input, :indent => 0.0).should == @default_output
    @parser.parse(@input, :indent => 2.0).should == <<-END
  <ul>
    <li>foo</li>
  </ul>
END
  end
end
