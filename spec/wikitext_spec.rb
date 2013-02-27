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

require 'spec_helper'
require 'wikitext'

describe Wikitext::Parser do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should autolink by default' do
    @parser.autolink.should == true
  end

  it 'should use linefeed as default line ending' do
    @parser.line_ending.should == "\n"
  end

  it 'should use "external" as default external link class' do
    @parser.external_link_class.should == 'external'
  end

  it 'should use "mailto" as default mailto class' do
    @parser.mailto_class.should == 'mailto'
  end

  it 'should use "/wiki/" as default internal link prefix' do
    @parser.internal_link_prefix.should == '/wiki/'
  end

  it 'should use "/images/" as default img prefix' do
    @parser.img_prefix.should == '/images/'
  end

  it 'should turn space-to-underscore on by default' do
    @parser.space_to_underscore.should == true
  end

  describe 'overriding defaults at initialization time' do
    it 'should allow overriding of autolink' do
      Wikitext::Parser.new(:autolink => false).autolink.should == false
    end

    it 'should allow overriding of line ending' do
      Wikitext::Parser.new(:line_ending => "\r").line_ending.should == "\r"
    end

    it 'should allow overriding of external link class' do
      Wikitext::Parser.new(:external_link_class => 'foo').external_link_class.should == 'foo'
    end

    it 'should allow overriding of mailto class' do
      Wikitext::Parser.new(:mailto_class => 'bar').mailto_class.should == 'bar'
    end

    it 'should allow overriding of internal link prefix' do
      Wikitext::Parser.new(:internal_link_prefix => '/baz/').internal_link_prefix.should == '/baz/'
    end

    it 'should allow overriding of the img prefix' do
      Wikitext::Parser.new(:img_prefix => '/bar/').img_prefix.should == '/bar/'
      Wikitext::Parser.new(:img_prefix => '').img_prefix.should == ''
      Wikitext::Parser.new(:img_prefix => nil).img_prefix.should == nil
    end

    it 'should allow overriding of space-to-underscore' do
      Wikitext::Parser.new(:space_to_underscore => false).space_to_underscore.should == false
    end
  end

  describe 'overriding defaults at parse time' do
    it 'should ignore unknown options' do
      @parser.parse('foo', :bar => 'baz').should == "<p>foo</p>\n"
    end
  end
end

describe Wikitext::Parser, 'parsing non-ASCII input' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should convert non-ASCII characters to numeric entities' do
    @parser.parse('€').should == "<p>&#x20ac;</p>\n"
  end
end

describe Wikitext::Parser, 'parsing characters which have special meaning in HTML' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should convert "<" into the corresponding named entity' do
    @parser.parse('<').should == "<p>&lt;</p>\n"
  end

  it 'should convert ">" into the corresponding named entity' do
    # can't put ">" in the first column as that would indicate a blockquote
    @parser.parse("foo >").should == "<p>foo &gt;</p>\n"
  end

  it 'should convert "&" into the corresponding named entity' do
    @parser.parse('&').should == "<p>&amp;</p>\n"
  end

  it 'should convert \'"\' into the corresponding named entity' do
    @parser.parse('"').should == "<p>&quot;</p>\n"
  end
end
