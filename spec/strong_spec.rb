# Copyright 2007-2014 Wincent Colaiuta. All rights reserved.
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

describe Wikitext::Parser, 'parsing <strong> spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  describe 'marked up using wikitext shorthand' do
    it 'should recognize paired <strong> tokens' do
      @parser.parse("foo '''bar''' baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should automatically insert missing closing tags' do
      @parser.parse("foo '''bar").should == "<p>foo <strong>bar</strong></p>\n"
    end

    it 'should automatically close unclosed spans upon hitting newlines' do
      @parser.parse("foo '''bar\nbaz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should handle (illegal) interleaved spans' do
      @parser.parse("foo '''bar ''inner''' baz''").should == "<p>foo <strong>bar <em>inner</em></strong> baz<em></em></p>\n"
    end

    it 'should have no effect inside <pre> blocks' do
      @parser.parse(" '''foo'''").should == "<pre>'''foo'''</pre>\n"
    end

    it 'should have no effect inside <nowiki> spans' do
      @parser.parse("<nowiki>'''foo'''</nowiki>").should == "<p>'''foo'''</p>\n"
    end

    it "should have no effect if a strong (<strong>) span is already open" do
      @parser.parse("foo <strong>'''bar'''</strong> baz").should == "<p>foo <strong>'''bar'''</strong> baz</p>\n"
    end
  end

  describe 'marked up using HTML tags' do
    it 'should recognized paired <strong> tokens' do
      @parser.parse("foo <strong>bar</strong> baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should recognize <strong> tokens case-insensitively' do
      @parser.parse("foo <STRong>bar</STRONG> baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
      @parser.parse("foo <strONG>bar</STRong> baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
      @parser.parse("foo <STRONG>bar</strONG> baz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should automatically insert missing closing tags' do
      @parser.parse("foo <strong>bar").should == "<p>foo <strong>bar</strong></p>\n"
    end

    it 'should automatically close unclosed spans upon hitting newlines' do
      @parser.parse("foo <strong>bar\nbaz").should == "<p>foo <strong>bar</strong> baz</p>\n"
    end

    it 'should handle (illegal) interleaved spans' do
      expected = "<p>foo <strong>bar <em>inner</em></strong> baz&lt;/em&gt;</p>\n"
      @parser.parse("foo <strong>bar <em>inner</strong> baz</em>").should == expected

      expected = "<p>foo <strong>bar <em>inner</em></strong> baz<em></em></p>\n"
      @parser.parse("foo <strong>bar ''inner</strong> baz''").should == expected
    end

    it 'should handle (illegal) nested <strong> spans' do
      expected = "<p>foo <strong>bar &lt;strong&gt;inner</strong>&lt;/strong&gt; baz</p>\n"
      @parser.parse('foo <strong>bar <strong>inner</strong></strong> baz').should == expected
    end

    it 'should have no effect inside <pre> blocks' do
      @parser.parse(" <strong>foo</strong>").should == "<pre>&lt;strong&gt;foo&lt;/strong&gt;</pre>\n"
    end

    it 'should have no effect inside <nowiki> spans' do
      @parser.parse("<nowiki><strong>foo</strong></nowiki>").should == "<p>&lt;strong&gt;foo&lt;/strong&gt;</p>\n"
    end

    it "should have no effect if an strong (''') span is already open" do
      expected = "<p>foo <strong>&lt;strong&gt;bar&lt;/strong&gt;</strong> baz</p>\n"
      @parser.parse("foo '''<strong>bar</strong>''' baz").should == expected
    end
  end
end
