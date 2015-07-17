# Copyright 2007-present Greg Hurrell. All rights reserved.
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
require 'wikitext'

describe Wikitext::Parser, 'parsing <tt> spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <tt> and </tt> tags' do
    # note how in version 2.0 and above, we output <code> tags instead of <tt>
    # tags, seeing as the latter have been removed from HTML5
    @parser.parse('foo <tt>bar</tt> baz').should == "<p>foo <code>bar</code> baz</p>\n"
  end

  it 'should recognize <tt> tags case-insensitively' do
    @parser.parse('foo <TT>bar</tT> baz').should == "<p>foo <code>bar</code> baz</p>\n"
    @parser.parse('foo <tT>bar</Tt> baz').should == "<p>foo <code>bar</code> baz</p>\n"
    @parser.parse('foo <Tt>bar</TT> baz').should == "<p>foo <code>bar</code> baz</p>\n"
  end

  it 'should automatically insert missing closing tags' do
    @parser.parse('foo <tt>bar').should == "<p>foo <code>bar</code></p>\n"
  end

  it 'should automatically close unclosed spans upon hitting newline' do
    @parser.parse("foo <tt>bar\nbaz").should == "<p>foo <code>bar</code> baz</p>\n"
  end

  it 'should convert unexpected closing tags into entities' do
    @parser.parse('foo </tt>bar').should == "<p>foo &lt;/tt&gt;bar</p>\n"
  end

  it 'should handle (illegal) nested <tt> spans' do
    @parser.parse('foo <tt>bar <tt>inner</tt></tt> baz').should == "<p>foo <code>bar &lt;tt&gt;inner</code>&lt;/tt&gt; baz</p>\n"
  end

  it 'should handle (illegal) interleaved spans' do
    @parser.parse("foo <tt>bar '''inner</tt> baz'''").should == "<p>foo <code>bar <strong>inner</strong></code> baz<strong></strong></p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(' <tt>foo</tt>').should == "<pre>&lt;tt&gt;foo&lt;/tt&gt;</pre>\n"
  end

  it 'should have no effect inside <nowiki> spans' do
    @parser.parse('<nowiki><tt>foo</tt></nowiki>').should == "<p>&lt;tt&gt;foo&lt;/tt&gt;</p>\n"
  end

  it 'should have no effect if a backtick span is already open' do
    @parser.parse('foo `<tt>bar</tt>` baz').should == "<p>foo <code>&lt;tt&gt;bar&lt;/tt&gt;</code> baz</p>\n"
  end
end

describe Wikitext::Parser, 'parsing backtick spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired backticks' do
    @parser.parse('foo `bar` baz').should == "<p>foo <code>bar</code> baz</p>\n"
  end

  it 'should automatically insert missing closing backtick' do
    @parser.parse('foo `bar').should == "<p>foo <code>bar</code></p>\n"
  end

  it 'should automatically close unclosed spans upon hitting newline' do
    @parser.parse("foo `bar\nbaz").should == "<p>foo <code>bar</code> baz</p>\n"
  end

  it 'should handle (illegal) interleaved spans' do
    @parser.parse("foo `bar '''inner` baz'''").should == "<p>foo <code>bar <strong>inner</strong></code> baz<strong></strong></p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(' `foo`').should == "<pre>`foo`</pre>\n"
  end

  it 'should have no effect inside <nowiki> spans' do
    @parser.parse('<nowiki>`foo`</nowiki>').should == "<p>`foo`</p>\n"
  end

  it 'should have no effect if a <tt> span is already open' do
    @parser.parse('foo <tt>`bar`</tt> baz').should == "<p>foo <code>`bar`</code> baz</p>\n"
  end
end

