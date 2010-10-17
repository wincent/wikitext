# encoding: utf-8
# Copyright 2007-2010 Wincent Colaiuta. All rights reserved.
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

describe Wikitext::Parser, 'parsing <nowiki> spans' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should not echo paired <nowiki> and </nowiki> tags' do
    @parser.parse('foo <nowiki>bar</nowiki> baz').should == "<p>foo bar baz</p>\n"
  end

  it 'should automatically handle missing closing tags (at end of outpt)' do
    # note that CRLF doesn't auto-terminate a <nowiki> span, unlike other inline spans
    @parser.parse('foo <nowiki>bar').should == "<p>foo bar</p>\n"
  end

  it 'should unconditionally echo newlines inside <nowiki> spans' do
    @parser.parse("<nowiki>foo\nbar</nowiki>").should == "<p>foo\nbar</p>\n"
    @parser.parse("<nowiki>foo\n\nbar</nowiki>").should == "<p>foo\n\nbar</p>\n"
    @parser.parse("<nowiki>foo\n\n\nbar</nowiki>").should == "<p>foo\n\n\nbar</p>\n"
  end

  it 'should convert unexpected closing tags into entities' do
    @parser.parse('foo </nowiki>bar').should == "<p>foo &lt;/nowiki&gt;bar</p>\n"
  end

  it 'should convert nested opening tags into entities' do
    @parser.parse('<nowiki>foo<nowiki>bar</nowiki>baz').should == "<p>foo&lt;nowiki&gt;barbaz</p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(' <nowiki>foo</nowiki>').should == "<pre>&lt;nowiki&gt;foo&lt;/nowiki&gt;</pre>\n"
  end

  it 'should pass short BLOCKQUOTE tokens through without any special meaning' do
    @parser.parse("<nowiki>\n></nowiki>").should == "<p>\n&gt;</p>\n"
  end

  it 'should pass long BLOCKQUOTE tokens through without any special meaning' do
    @parser.parse("<nowiki>\n> </nowiki>").should == "<p>\n&gt; </p>\n"
  end

  it 'should pass <tt> and </tt> tags through without any special meaning' do
    @parser.parse('<nowiki>foo <tt>bar</tt></nowiki>').should == "<p>foo &lt;tt&gt;bar&lt;/tt&gt;</p>\n"
  end

  it 'should pass <em> and </em> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ''bar''</nowiki>").should == "<p>foo ''bar''</p>\n"
  end

  it 'should pass <strong> and </strong> tags through without any special meaning' do
    @parser.parse("<nowiki>foo '''bar'''</nowiki>").should == "<p>foo '''bar'''</p>\n"
  end

  it 'should pass combined <strong>/<em> and </strong>/</em> tags through without any special meaning' do
    @parser.parse("<nowiki>foo '''''bar'''''</nowiki>").should == "<p>foo '''''bar'''''</p>\n"
  end

  it 'should pass <h1> tags through without any special meaning' do
    @parser.parse("<nowiki>\n= foo</nowiki>").should == "<p>\n= foo</p>\n"
  end

  it 'should pass </h1> tags through without any special meaning' do
    @parser.parse("<nowiki>foo =\n</nowiki>").should == "<p>foo =\n</p>\n"
  end

  it 'should pass <h2> tags through without any special meaning' do
    @parser.parse("<nowiki>\n== foo</nowiki>").should == "<p>\n== foo</p>\n"
  end

  it 'should pass </h2> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ==\n</nowiki>").should == "<p>foo ==\n</p>\n"
  end

  it 'should pass <h3> tags through without any special meaning' do
    @parser.parse("<nowiki>\n=== foo</nowiki>").should == "<p>\n=== foo</p>\n"
  end

  it 'should pass </h3> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ===\n</nowiki>").should == "<p>foo ===\n</p>\n"
  end

  it 'should pass <h4> tags through without any special meaning' do
    @parser.parse("<nowiki>\n==== foo</nowiki>").should == "<p>\n==== foo</p>\n"
  end

  it 'should pass </h4> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ====\n</nowiki>").should == "<p>foo ====\n</p>\n"
  end

  it 'should pass <h5> tags through without any special meaning' do
    @parser.parse("<nowiki>\n===== foo</nowiki>").should == "<p>\n===== foo</p>\n"
  end

  it 'should pass </h5> tags through without any special meaning' do
    @parser.parse("<nowiki>foo =====\n</nowiki>").should == "<p>foo =====\n</p>\n"
  end

  it 'should pass <h6> tags through without any special meaning' do
    @parser.parse("<nowiki>\n====== foo</nowiki>").should == "<p>\n====== foo</p>\n"
  end

  it 'should pass </h6> tags through without any special meaning' do
    @parser.parse("<nowiki>foo ======\n</nowiki>").should == "<p>foo ======\n</p>\n"
  end

  it 'should pass link start tokens through unchanged' do
    @parser.parse('<nowiki>[[</nowiki>').should == "<p>[[</p>\n"
  end

  it 'should pass link end tokens through unchanged' do
    @parser.parse('<nowiki>]]</nowiki>').should == "<p>]]</p>\n"
  end

  it 'should pass external link start tokens through unchanged' do
    @parser.parse('<nowiki>[</nowiki>').should == "<p>[</p>\n"
  end

  it 'should pass external link end tokens through unchanged' do
    @parser.parse('<nowiki>]</nowiki>').should == "<p>]</p>\n"
  end

  it 'should pass named entities through unchanged' do
    @parser.parse('<nowiki>&euro;</nowiki>').should == "<p>&euro;</p>\n"
  end

  it 'should pass numeric (decimal) entities through unchanged' do
    @parser.parse('<nowiki>&#8364;</nowiki>').should == "<p>&#8364;</p>\n"
  end

  it 'should pass numeric (hexadecimal) entities through unchanged' do
    @parser.parse('<nowiki>&#x20ac;</nowiki>').should == "<p>&#x20ac;</p>\n"
    @parser.parse('<nowiki>&#X20Ac;</nowiki>').should == "<p>&#x20ac;</p>\n"
  end

  it 'should convert non-ASCII characters to numeric entities' do
    @parser.parse('<nowiki>€</nowiki>').should == "<p>&#x20ac;</p>\n"
  end
end
