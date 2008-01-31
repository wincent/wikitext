#!/usr/bin/env ruby
# Copyright 2007 Wincent Colaiuta

require File.join(File.dirname(__FILE__), 'spec_helper.rb')
require 'wikitext'
require 'iconv'
require 'uri'

describe Wikitext::Parser do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should default to UNIX line endings' do
    @parser.line_ending.should == "\n"
  end

  it 'should greedily match Mac line endings in input if possible' do
    # note how the embedded "\r\n" is interpreted as a single token (and converted to exactly one space)
    # it is not treated as two separate tokens
    @parser.parse("foo\r\nbar").should == "<p>foo bar</p>\n"
  end

end

describe Wikitext::Parser, 'with UNIX line endings' do

  before do
    @parser = Wikitext::Parser.new
    @parser.line_ending = "\n"
  end

  it 'should convert line endings within spans to a single space' do
    @parser.parse("foo\nbar").should == "<p>foo bar</p>\n"
  end

  it 'should normalize non-UNIX line endings' do
    @parser.parse("foo\rbar").should == "<p>foo bar</p>\n"
    @parser.parse("foo\r\nbar").should == "<p>foo bar</p>\n"
  end

end

describe Wikitext::Parser, 'with Windows line endings' do

  before do
    @parser = Wikitext::Parser.new
    @parser.line_ending = "\r"
  end

  it 'should convert line endings within spans to a single space' do
    @parser.parse("foo\rbar").should == "<p>foo bar</p>\r"
  end

  it 'should normalize non-Windows line endings' do
    @parser.parse("foo\nbar").should == "<p>foo bar</p>\r"
    @parser.parse("foo\r\nbar").should == "<p>foo bar</p>\r"
  end

end

describe Wikitext::Parser, 'with Mac line endings' do

  before do
    @parser = Wikitext::Parser.new
    @parser.line_ending = "\r\n"
  end

  it 'should convert line endings within spans to a single space' do
    @parser.parse("foo\r\nbar").should == "<p>foo bar</p>\r\n"
  end

  it 'should normalize non-Mac line endings' do
    @parser.parse("foo\nbar").should == "<p>foo bar</p>\r\n"
    @parser.parse("foo\rbar").should == "<p>foo bar</p>\r\n"
  end

end

describe Wikitext::Parser, 'parsing paragraphs' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should wrap bare text in paragraph tags' do
    @parser.parse('foo').should == "<p>foo</p>\n"
  end

  it 'should treat consecutive line breaks as paragraph breaks' do
    @parser.parse("foo\n\nbar").should == "<p>foo</p>\n<p>bar</p>\n"
  end

  it 'should not insert excess empty paragraphs or spaces when processing multiple consecutive line breaks' do
    @parser.parse("foo\n\n\n\nbar").should == "<p>foo</p>\n<p>bar</p>\n"
  end

  it 'should not translate single line breaks into spaces when they appear at the start of a paragraph' do
    @parser.parse("\nfoo").should == "<p>foo</p>\n"
  end

  it 'should not translate single line breaks into spaces when they appear at the end of a paragraph' do
    @parser.parse("foo\n").should == "<p>foo</p>\n"
  end

end

describe Wikitext::Parser, 'parsing blockquotes' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should treat ">" in first column as a blockquote marker' do
    @parser.parse('>foo').should == "<blockquote><p>foo</p>\n</blockquote>\n"
  end

  it 'should accept (and ignore) one optional space after the ">"' do
    @parser.parse('> foo').should == "<blockquote><p>foo</p>\n</blockquote>\n"
  end

  it 'should recognize consecutive ">" as continuance of blockquote section' do
    @parser.parse("> foo\n> bar").should == "<blockquote><p>foo bar</p>\n</blockquote>\n"
  end

  it 'should not give ">" special treatment when not on the far left' do
    @parser.parse('foo > bar').should == "<p>foo &gt; bar</p>\n"
  end

  it 'should allow nesting of blockquotes' do
    @parser.parse('> > foo').should == "<blockquote><blockquote><p>foo</p>\n</blockquote>\n</blockquote>\n"
  end

  it 'should allow opening of a nested blockquote after other content' do
    @parser.parse("> foo\n> > bar").should == "<blockquote><p>foo</p>\n<blockquote><p>bar</p>\n</blockquote>\n</blockquote>\n"
  end

  it 'should allow opening of a nested blockquote before other content' do
    @parser.parse("> > foo\n> bar").should == "<blockquote><blockquote><p>foo</p>\n</blockquote>\n<p>bar</p>\n</blockquote>\n"
  end

  it 'should accept an empty blockquote' do
    @parser.parse('>').should == "<blockquote></blockquote>\n"
  end

  it 'should jump out of blockquote mode on seeing a normal line of text' do
    @parser.parse("> foo\nbar").should == "<blockquote><p>foo</p>\n</blockquote>\n<p>bar</p>\n"
  end

  # TODO: tests for nesting other types of blocks

end

describe Wikitext::Parser, 'parsing <pre> blocks' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize a single-line <pre> block' do
    @parser.parse(' foo').should == "<pre>foo</pre>\n"
  end

  it 'should recognize a multiline <pre> block' do
    @parser.parse(" foo\n bar").should == "<pre>foo\nbar</pre>\n"
  end

  it 'should allow nesting inside a <blockquote> block' do
    # nesting inside single blockquotes
    @parser.parse(">  foo").should == "<blockquote><pre>foo</pre>\n</blockquote>\n"

    # same, but continued over multiple lines
    @parser.parse(">  foo\n>  bar").should == "<blockquote><pre>foo\nbar</pre>\n</blockquote>\n"

    # nesting inside double blockquotes
    @parser.parse("> >  foo").should == "<blockquote><blockquote><pre>foo</pre>\n</blockquote>\n</blockquote>\n"

    # same, but continued over multiple lines
    @parser.parse("> >  foo\n> >  bar").should == "<blockquote><blockquote><pre>foo\nbar</pre>\n</blockquote>\n</blockquote>\n"
  end

  it 'should automatically close preceding blocks at the same depth' do
    @parser.parse("> foo\n bar").should == "<blockquote><p>foo</p>\n</blockquote>\n<pre>bar</pre>\n"
    @parser.parse("> > foo\n bar").should == "<blockquote><blockquote><p>foo</p>\n</blockquote>\n</blockquote>\n<pre>bar</pre>\n"
  end

  it 'should pass <tt> and </tt> tags through without any special meaning' do
    @parser.parse(' foo <tt>bar</tt>').should == "<pre>foo &lt;tt&gt;bar&lt;/tt&gt;</pre>\n"
  end

  it 'should pass <em> and </em> tags through without any special meaning' do
    @parser.parse(" foo ''bar''").should == "<pre>foo ''bar''</pre>\n"
  end

  it 'should pass <strong> and </strong> tags through without any special meaning' do
    @parser.parse(" foo '''bar'''").should == "<pre>foo '''bar'''</pre>\n"
  end

  it 'should pass combined <strong>/<em> and </strong>/</em> tags through without any special meaning' do
    @parser.parse(" foo '''''bar'''''").should == "<pre>foo '''''bar'''''</pre>\n"
  end

  it 'should pass named entities through unchanged' do
    @parser.parse(' &euro;').should == "<pre>&euro;</pre>\n"
  end

  it 'should pass numeric (decimal) entities through unchanged' do
    @parser.parse(' &#8364;').should == "<pre>&#8364;</pre>\n"
  end

  it 'should pass numeric (hexadecimal) entities through unchanged' do
    @parser.parse(' &#x20ac;').should == "<pre>&#x20ac;</pre>\n"
    @parser.parse(' &#X20aC;').should == "<pre>&#x20ac;</pre>\n"
  end

  it 'should convert non-ASCII characters to numeric entities' do
    @parser.parse(' €').should == "<pre>&#x20ac;</pre>\n"
  end

end

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

describe Wikitext::Parser, 'parsing <h1> blocks' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <h1> and </h1> tags' do
    @parser.parse('=foo=').should == "<h1>foo</h1>\n"
  end

  it 'should strip leading and trailing whitespace from the title' do
    @parser.parse('= foo =').should == "<h1>foo</h1>\n"
    @parser.parse('=  foo  =').should == "<h1>foo</h1>\n"
    @parser.parse('=   foo   =').should == "<h1>foo</h1>\n"
  end

  it 'should accept titles with missing closing tags' do
    @parser.parse('= foo').should == "<h1>foo</h1>\n"
  end

  it 'should allow header tags to appear within titles' do
    @parser.parse('= foo = bar =').should == "<h1>foo = bar</h1>\n"
    @parser.parse('= foo == bar =').should == "<h1>foo == bar</h1>\n"
    @parser.parse('= foo === bar =').should == "<h1>foo === bar</h1>\n"
    @parser.parse('= foo ==== bar =').should == "<h1>foo ==== bar</h1>\n"
    @parser.parse('= foo ===== bar =').should == "<h1>foo ===== bar</h1>\n"
    @parser.parse('= foo ====== bar =').should == "<h1>foo ====== bar</h1>\n"
  end

  it 'should show excess characters in closing tags' do
    # the visual feedback alerts the user to the error
    @parser.parse('= foo ==').should == "<h1>foo ==</h1>\n"
  end

  it 'should be nestable inside blockquote blocks' do
    @parser.parse('> = foo =').should == "<blockquote><h1>foo</h1>\n</blockquote>\n"
  end

  it 'should have no special meaning inside <nowiki> spans' do
    @parser.parse("<nowiki>\n= foo =</nowiki>").should == "<p>\n= foo =</p>\n"
  end

end

describe Wikitext::Parser, 'parsing <h2> blocks' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <h2> and </h2> tags' do
    @parser.parse('==foo==').should == "<h2>foo</h2>\n"
  end

  it 'should strip leading and trailing whitespace from the title' do
    @parser.parse('== foo ==').should == "<h2>foo</h2>\n"
    @parser.parse('==  foo  ==').should == "<h2>foo</h2>\n"
    @parser.parse('==   foo   ==').should == "<h2>foo</h2>\n"
  end

  it 'should accept titles with missing closing tags' do
    @parser.parse('== foo').should == "<h2>foo</h2>\n"
  end

  it 'should allow header tags to appear within titles' do
    @parser.parse('== foo = bar ==').should == "<h2>foo = bar</h2>\n"
    @parser.parse('== foo == bar ==').should == "<h2>foo == bar</h2>\n"
    @parser.parse('== foo === bar ==').should == "<h2>foo === bar</h2>\n"
    @parser.parse('== foo ==== bar ==').should == "<h2>foo ==== bar</h2>\n"
    @parser.parse('== foo ===== bar ==').should == "<h2>foo ===== bar</h2>\n"
    @parser.parse('== foo ====== bar ==').should == "<h2>foo ====== bar</h2>\n"
  end

  it 'should show excess characters in closing tags' do
    # the visual feedback alerts the user to the error
    @parser.parse('== foo ===').should == "<h2>foo ===</h2>\n"
  end

  it 'should be nestable inside blockquote blocks' do
    @parser.parse('> == foo ==').should == "<blockquote><h2>foo</h2>\n</blockquote>\n"
  end

  it 'should have no special meaning inside <nowiki> spans' do
    @parser.parse("<nowiki>\n== foo ==</nowiki>").should == "<p>\n== foo ==</p>\n"
  end

end

describe Wikitext::Parser, 'parsing <h3> blocks' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <h3> and </h3> tags' do
    @parser.parse('===foo===').should == "<h3>foo</h3>\n"
  end

  it 'should strip leading and trailing whitespace from the title' do
    @parser.parse('=== foo ===').should == "<h3>foo</h3>\n"
    @parser.parse('===  foo  ===').should == "<h3>foo</h3>\n"
    @parser.parse('===   foo   ===').should == "<h3>foo</h3>\n"
  end

  it 'should accept titles with missing closing tags' do
    @parser.parse('=== foo').should == "<h3>foo</h3>\n"
  end

  it 'should allow header tags to appear within titles' do
    @parser.parse('=== foo = bar ===').should == "<h3>foo = bar</h3>\n"
    @parser.parse('=== foo == bar ===').should == "<h3>foo == bar</h3>\n"
    @parser.parse('=== foo === bar ===').should == "<h3>foo === bar</h3>\n"
    @parser.parse('=== foo ==== bar ===').should == "<h3>foo ==== bar</h3>\n"
    @parser.parse('=== foo ===== bar ===').should == "<h3>foo ===== bar</h3>\n"
    @parser.parse('=== foo ====== bar ===').should == "<h3>foo ====== bar</h3>\n"
  end

  it 'should show excess characters in closing tags' do
    # the visual feedback alerts the user to the error
    @parser.parse('=== foo ====').should == "<h3>foo ====</h3>\n"
  end

  it 'should be nestable inside blockquote blocks' do
    @parser.parse('> === foo ===').should == "<blockquote><h3>foo</h3>\n</blockquote>\n"
  end

  it 'should have no special meaning inside <nowiki> spans' do
    @parser.parse("<nowiki>\n=== foo ===</nowiki>").should == "<p>\n=== foo ===</p>\n"
  end

end

describe Wikitext::Parser, 'parsing <h4> blocks' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <h4> and </h4> tags' do
    @parser.parse('====foo====').should == "<h4>foo</h4>\n"
  end

  it 'should strip leading and trailing whitespace from the title' do
    @parser.parse('==== foo ====').should == "<h4>foo</h4>\n"
    @parser.parse('====  foo  ====').should == "<h4>foo</h4>\n"
    @parser.parse('====   foo   ====').should == "<h4>foo</h4>\n"
  end

  it 'should accept titles with missing closing tags' do
    @parser.parse('==== foo').should == "<h4>foo</h4>\n"
  end

  it 'should allow header tags to appear within titles' do
    @parser.parse('==== foo = bar ====').should == "<h4>foo = bar</h4>\n"
    @parser.parse('==== foo == bar ====').should == "<h4>foo == bar</h4>\n"
    @parser.parse('==== foo === bar ====').should == "<h4>foo === bar</h4>\n"
    @parser.parse('==== foo ==== bar ====').should == "<h4>foo ==== bar</h4>\n"
    @parser.parse('==== foo ===== bar ====').should == "<h4>foo ===== bar</h4>\n"
    @parser.parse('==== foo ====== bar ====').should == "<h4>foo ====== bar</h4>\n"
  end

  it 'should show excess characters in closing tags' do
    # the visual feedback alerts the user to the error
    @parser.parse('==== foo =====').should == "<h4>foo =====</h4>\n"
  end

  it 'should be nestable inside blockquote blocks' do
    @parser.parse('> ==== foo ====').should == "<blockquote><h4>foo</h4>\n</blockquote>\n"
  end

  it 'should have no special meaning inside <nowiki> spans' do
    @parser.parse("<nowiki>\n==== foo ====</nowiki>").should == "<p>\n==== foo ====</p>\n"
  end

end

describe Wikitext::Parser, 'parsing <h5> blocks' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <h5> and </h5> tags' do
    @parser.parse('=====foo=====').should == "<h5>foo</h5>\n"
  end

  it 'should strip leading and trailing whitespace from the title' do
    @parser.parse('===== foo =====').should == "<h5>foo</h5>\n"
    @parser.parse('=====  foo  =====').should == "<h5>foo</h5>\n"
    @parser.parse('=====   foo   =====').should == "<h5>foo</h5>\n"
  end

  it 'should accept titles with missing closing tags' do
    @parser.parse('===== foo').should == "<h5>foo</h5>\n"
  end

  it 'should allow header tags to appear within titles' do
    @parser.parse('===== foo = bar =====').should == "<h5>foo = bar</h5>\n"
    @parser.parse('===== foo == bar =====').should == "<h5>foo == bar</h5>\n"
    @parser.parse('===== foo === bar =====').should == "<h5>foo === bar</h5>\n"
    @parser.parse('===== foo ==== bar =====').should == "<h5>foo ==== bar</h5>\n"
    @parser.parse('===== foo ===== bar =====').should == "<h5>foo ===== bar</h5>\n"
    @parser.parse('===== foo ====== bar =====').should == "<h5>foo ====== bar</h5>\n"
  end

  it 'should show excess characters in closing tags' do
    # the visual feedback alerts the user to the error
    @parser.parse('===== foo ======').should == "<h5>foo ======</h5>\n"
  end

  it 'should be nestable inside blockquote blocks' do
    @parser.parse('> ===== foo =====').should == "<blockquote><h5>foo</h5>\n</blockquote>\n"
  end

  it 'should have no special meaning inside <nowiki> spans' do
    @parser.parse("<nowiki>\n===== foo =====</nowiki>").should == "<p>\n===== foo =====</p>\n"
  end

end

describe Wikitext::Parser, 'parsing <h6> blocks' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <h6> and </h6> tags' do
    @parser.parse('======foo======').should == "<h6>foo</h6>\n"
  end

  it 'should strip leading and trailing whitespace from the title' do
    @parser.parse('====== foo ======').should == "<h6>foo</h6>\n"
    @parser.parse('======  foo  ======').should == "<h6>foo</h6>\n"
    @parser.parse('======   foo   ======').should == "<h6>foo</h6>\n"
  end

  it 'should accept titles with missing closing tags' do
    @parser.parse('====== foo').should == "<h6>foo</h6>\n"
  end

  it 'should allow header tags to appear within titles' do
    @parser.parse('====== foo = bar ======').should == "<h6>foo = bar</h6>\n"
    @parser.parse('====== foo == bar ======').should == "<h6>foo == bar</h6>\n"
    @parser.parse('====== foo === bar ======').should == "<h6>foo === bar</h6>\n"
    @parser.parse('====== foo ==== bar ======').should == "<h6>foo ==== bar</h6>\n"
    @parser.parse('====== foo ===== bar ======').should == "<h6>foo ===== bar</h6>\n"
    @parser.parse('====== foo ====== bar ======').should == "<h6>foo ====== bar</h6>\n"
  end

  it 'should show excess characters in closing tags' do
    # in this case only one excess char, then the H6_END matches
    @parser.parse('====== foo =======').should == "<h6>foo =</h6>\n"
  end

  it 'should be nestable inside blockquote blocks' do
    @parser.parse('> ====== foo ======').should == "<blockquote><h6>foo</h6>\n</blockquote>\n"
  end

  it 'should have no special meaning inside <nowiki> spans' do
    @parser.parse("<nowiki>\n====== foo ======</nowiki>").should == "<p>\n====== foo ======</p>\n"
  end

end

describe Wikitext::Parser, 'parsing unordered lists' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize a single item list' do
    @parser.parse('*foo').should == "<ul>\n<li>foo</li>\n</ul>\n"
  end

  it 'should allow and consume an optional space after the last <ul> marker' do
    @parser.parse('* foo').should == "<ul>\n<li>foo</li>\n</ul>\n"    # exactly one space consumed
    @parser.parse('*  foo').should == "<ul>\n<li> foo</li>\n</ul>\n"  # exactly one space consumed (and one echoed through)
  end

  it 'should consider a space after an <ul> marker to indicate that it will be the last marker' do
    @parser.parse('* * foo').should == "<ul>\n<li>* foo</li>\n</ul>\n"
  end

  it 'should only recognize <ul> markers if they or a direct ancestor start in the left column' do
    @parser.parse(' * foo').should == "<pre>* foo</pre>\n"
  end

  it 'should recognize <ul> markers nested inside blockquote blocks' do
    @parser.parse('> * foo').should == "<blockquote><ul>\n<li>foo</li>\n</ul>\n</blockquote>\n"
  end

  it 'should display excess <ul> markers as literals' do
    # this provides feedback to the user
    @parser.parse('** foo').should == "<ul>\n<li>* foo</li>\n</ul>\n"
    @parser.parse('*** foo').should == "<ul>\n<li>** foo</li>\n</ul>\n"
  end

  it 'should recognize a multi-item, single-level list' do
    @parser.parse("* foo\n* bar").should == "<ul>\n<li>foo</li>\n<li>bar</li>\n</ul>\n"
  end

  it 'should recognize a multi-item, nested list (two levels)' do
    @parser.parse("* foo\n** bar").should == "<ul>\n<li>foo\n<ul>\n<li>bar</li>\n</ul>\n</li>\n</ul>\n"
  end

  it 'should recognize a multi-item, nested list (three levels)' do
    @parser.parse("* foo\n** bar\n*** baz").should == "<ul>\n<li>foo\n<ul>\n<li>bar\n<ul>\n<li>baz</li>\n</ul>\n</li>\n</ul>\n</li>\n</ul>\n"
  end

  it 'should recognize lists in which nesting level increases and then is maintained' do
    @parser.parse("* foo\n** bar\n** baz").should == "<ul>\n<li>foo\n<ul>\n<li>bar</li>\n<li>baz</li>\n</ul>\n</li>\n</ul>\n"
  end

  it 'should recognize lists in which nesting level increases and then decreases' do
    @parser.parse("* foo\n** bar\n* baz").should == "<ul>\n<li>foo\n<ul>\n<li>bar</li>\n</ul>\n</li>\n<li>baz</li>\n</ul>\n"
  end

  it 'should be terminated by subsequent paragraph at the same level' do
    @parser.parse("* foo\nbar").should == "<ul>\n<li>foo</li>\n</ul>\n<p>bar</p>\n"
  end

  it 'should be terminated by subsequent blockquote at the same level' do
    @parser.parse("* foo\n> bar").should == "<ul>\n<li>foo</li>\n</ul>\n<blockquote><p>bar</p>\n</blockquote>\n"
  end

  it 'should be terminated by subsequent heading at the same level' do
    @parser.parse("* foo\n====== bar ======").should == "<ul>\n<li>foo</li>\n</ul>\n<h6>bar</h6>\n"
    @parser.parse("* foo\n===== bar =====").should == "<ul>\n<li>foo</li>\n</ul>\n<h5>bar</h5>\n"
    @parser.parse("* foo\n==== bar ====").should == "<ul>\n<li>foo</li>\n</ul>\n<h4>bar</h4>\n"
    @parser.parse("* foo\n=== bar ===").should == "<ul>\n<li>foo</li>\n</ul>\n<h3>bar</h3>\n"
    @parser.parse("* foo\n== bar ==").should == "<ul>\n<li>foo</li>\n</ul>\n<h2>bar</h2>\n"
    @parser.parse("* foo\n= bar =").should == "<ul>\n<li>foo</li>\n</ul>\n<h1>bar</h1>\n"
  end

  it 'should be terminated by subsequent <pre> block at the same level' do
    @parser.parse("* foo\n bar").should == "<ul>\n<li>foo</li>\n</ul>\n<pre>bar</pre>\n"
  end

  it 'should be terminated by subsequent ordered list at the same level' do
    @parser.parse("* foo\n# bar").should == "<ul>\n<li>foo</li>\n</ul>\n<ol>\n<li>bar</li>\n</ol>\n"
  end

  it 'should recognized lists which contain nested ordered lists' do
    @parser.parse("* foo\n*# bar").should == "<ul>\n<li>foo\n<ol>\n<li>bar</li>\n</ol>\n</li>\n</ul>\n"
    input = <<INPUT
* foo
*# bar
*# baz
INPUT
    expected = <<EXPECTED
<ul>
<li>foo
<ol>
<li>bar</li>
<li>baz</li>
</ol>
</li>
</ul>
EXPECTED
    @parser.parse(input).should == expected
  end

end

describe Wikitext::Parser, 'parsing <tt> spans' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <tt> and </tt> tags' do
    @parser.parse('foo <tt>bar</tt> baz').should == "<p>foo <tt>bar</tt> baz</p>\n"
  end

  it 'should automatically insert missing closing tags' do
    @parser.parse('foo <tt>bar').should == "<p>foo <tt>bar</tt></p>\n"
  end

  it 'should automatically close unclosed spans upon hitting newline' do
    @parser.parse("foo <tt>bar\nbaz").should == "<p>foo <tt>bar</tt> baz</p>\n"
  end

  it 'should convert unexpected closing tags into entities' do
    @parser.parse('foo </tt>bar').should == "<p>foo &lt;/tt&gt;bar</p>\n"
  end

  it 'should handle (illegal) nested <tt> spans' do
    @parser.parse('foo <tt>bar <tt>inner</tt></tt> baz').should == "<p>foo <tt>bar &lt;tt&gt;inner</tt>&lt;/tt&gt; baz</p>\n"
  end

  it 'should handle (illegal) interleaved spans' do
    @parser.parse("foo <tt>bar '''inner</tt> baz'''").should == "<p>foo <tt>bar <strong>inner</strong></tt> baz<strong></strong></p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(' <tt>foo</tt>').should == "<pre>&lt;tt&gt;foo&lt;/tt&gt;</pre>\n"
  end

  it 'should have no effect inside <nowiki> spans' do
    @parser.parse('<nowiki><tt>foo</tt></nowiki>').should == "<p>&lt;tt&gt;foo&lt;/tt&gt;</p>\n"
  end

end

describe Wikitext::Parser, 'parsing <strong> spans' do

  before do
    @parser = Wikitext::Parser.new
  end

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

end

describe Wikitext::Parser, 'parsing <em> spans' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired <em> tokens' do
    @parser.parse("foo ''bar'' baz").should == "<p>foo <em>bar</em> baz</p>\n"
  end

  it 'should automatically insert missing closing tags' do
    @parser.parse("foo ''bar").should == "<p>foo <em>bar</em></p>\n"
  end

  it 'should automatically close unclosed spans upon hitting newlines' do
    @parser.parse("foo ''bar\nbaz").should == "<p>foo <em>bar</em> baz</p>\n"
  end

  it 'should handle (illegal) interleaved spans' do
    @parser.parse("foo ''bar '''inner'' baz'''").should == "<p>foo <em>bar <strong>inner</strong></em> baz<strong></strong></p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(" ''foo''").should == "<pre>''foo''</pre>\n"
  end

  it 'should have no effect inside <nowiki> spans' do
    @parser.parse("<nowiki>''foo''</nowiki>").should == "<p>''foo''</p>\n"
  end

end

describe Wikitext::Parser, 'parsing combined <strong>/<em> spans' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should recognize paired "<strong><em>" tokens' do
    @parser.parse("foo '''''bar''''' baz").should == "<p>foo <strong><em>bar</em></strong> baz</p>\n"
  end

  it 'should automatically insert missing closing tags' do
    @parser.parse("foo '''''bar").should == "<p>foo <strong><em>bar</em></strong></p>\n"    # totally missing
    @parser.parse("foo '''''bar''").should == "<p>foo <strong><em>bar</em></strong></p>\n"  # only <strong> missing
    @parser.parse("foo '''''bar'''").should == "<p>foo <strong><em>bar</em></strong></p>\n" # only <em> missing
  end

  it 'should automatically close unclosed spans upon hitting newlines' do
    @parser.parse("foo '''''bar\nbaz").should == "<p>foo <strong><em>bar</em></strong> baz</p>\n"     # totally missing
    @parser.parse("foo '''''bar''\nbaz").should == "<p>foo <strong><em>bar</em></strong> baz</p>\n"   # only <strong> missing
    @parser.parse("foo '''''bar'''\nbaz").should == "<p>foo <strong><em>bar</em></strong> baz</p>\n"  # only <em> missing
  end

  it 'should allow combined "<strong><em>" tokens to interact with separate <strong> and <em> tokens' do
    @parser.parse("foo '''bar ''baz'''''").should == "<p>foo <strong>bar <em>baz</em></strong></p>\n"
    @parser.parse("foo ''bar '''baz'''''").should == "<p>foo <em>bar <strong>baz</strong></em></p>\n"
    @parser.parse("'''''foo'' bar''' baz").should == "<p><strong><em>foo</em> bar</strong> baz</p>\n"
  end

  it 'should handle (illegal) interleaved spans' do
    # ''''' means "<strong><em>" so when we see ''' we try to close the <strong> first, which makes for illegal nesting
    @parser.parse("'''''foo''' bar'' baz").should == "<p><strong><em>foo</em></strong> bar<em> baz</em></p>\n"

    # note that if you really want ''''' to be parsed as "<em><strong>" you have to use whitespace to disambiguate
    @parser.parse("'' '''foo''' bar'' baz").should == "<p><em> <strong>foo</strong> bar</em> baz</p>\n"
  end

  it 'should have no effect inside <pre> blocks' do
    @parser.parse(" '''''foo'''''").should == "<pre>'''''foo'''''</pre>\n"
  end

  it 'should have no effect inside <nowiki> spans' do
    @parser.parse("<nowiki>'''''foo'''''</nowiki>").should == "<p>'''''foo'''''</p>\n"
  end

end

describe Wikitext::Parser, 'parsing entities' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should pass numeric (decimal) entities through unchanged' do
    @parser.parse('&#8364;').should == "<p>&#8364;</p>\n"
  end

  it 'should normalize case variations in hexadecimal entities' do
    @parser.parse('&#x20ac;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#x20AC;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#X20ac;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#X20AC;').should == "<p>&#x20ac;</p>\n"
  end

  it 'should pass named entities through unchanged' do
    @parser.parse('&Aacute;').should == "<p>&Aacute;</p>\n" # these are two different entities
    @parser.parse('&aacute;').should == "<p>&aacute;</p>\n" # ie. they are case sensitive
    @parser.parse('&euro;').should == "<p>&euro;</p>\n"
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

describe Wikitext::Parser, 'autolinking' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should default to autolinking on' do
    @parser.autolink.should == true
  end

  describe 'on' do
    it 'should convert URIs into hyperlinks' do
      uri = 'http://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="http://example.com/" class="external">http://example.com/</a></p>\n}
      @parser.external_link_class = nil
      @parser.parse(uri).should == %Q{<p><a href="http://example.com/">http://example.com/</a></p>\n}
    end

    it 'should pass through URIs unchanged inside <nowiki></nowiki> spans' do
      @parser.parse("<nowiki>http://example.com/</nowiki>").should == "<p>http://example.com/</p>\n"
    end

    it 'should autolink inside <pre></pre> spans' do
      input     = ' http://example.com/'
      expected  = %Q{<pre><a href="http://example.com/" class="external">http://example.com/</a></pre>\n}
      @parser.parse(input).should == expected
      @parser.external_link_class = nil
      expected  = %Q{<pre><a href="http://example.com/">http://example.com/</a></pre>\n}
      @parser.parse(input).should == expected
    end
  end

  describe 'off' do
    before do
      @parser.autolink = false
    end

    it 'should accept "autolink = false"' do
      @parser.autolink.should == false
    end

    it 'should not convert URIs into hyperlinks' do
      @parser.parse('http://example.com/').should == "<p>http://example.com/</p>\n"
    end
  end
end

describe Wikitext::Parser, 'external links' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should format valid external links' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google</a></p>\n}
    @parser.parse('[http://google.com/ Google]').should == expected
  end

  it 'should format a link with emphasis in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <em>rocks</em></a></p>\n}
    @parser.parse("[http://google.com/ Google ''rocks'']").should == expected
  end

  it 'should format a link with strong formatting in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external"><strong>Google</strong> rocks</a></p>\n}
    @parser.parse("[http://google.com/ '''Google''' rocks]").should == expected
  end

  it 'should format a link with <tt></tt> tags in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <tt>SOC</tt></a></p>\n}
    @parser.parse("[http://google.com/ Google <tt>SOC</tt>]").should == expected
  end

  it 'should format a link with strong and emphasis in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <strong><em>rocks</em></strong></a></p>\n}
    @parser.parse("[http://google.com/ Google '''''rocks''''']").should == expected
  end

  it "should pass through links which don't have a target" do
    expected = "<p>[well]</p>\n"
    @parser.parse("[well]").should == expected
  end

end

describe Wikitext::Parser, 'with large slab of input text' do

  before do
    @parser = Wikitext::Parser.new
  end

  it 'should handle nested lists which follow other items' do

    input = <<INPUT
paragraph
second line

new paragraph

= a heading =

> a blockquote
> second line of blockquote
>
> new paragraph within blockquote

== another heading ==

paragraph within ''multiple '''styles''''' and <tt>tt span</tt>

similar, but with '''styles in ''different'' order'''

again, a '''different ''order'''''

* list item 1
** nested list item 1
** nested list item 2
** nested list item 3
* list item 2

 // this is a code block
 notice how it can contain ''markup''
 which would '''otherwise''' have <tt>special</tt> meaning
 although explicit entities &copy; are passed through unchanged

a normal paragraph again

# this is an ordered list
# which continues
## and has another ordered list
## nested inside it
# and then falls back
#* and then nests another list
#* this time an unordered one
#** itself containing a nested list
#** which continues
#**# and finally nests yet another ordered list
#**# which continues
#* drops back quite a way
# and finally all the way
#****** and finishes with an invalid item

=== heading with missing closing tag
* list
# new list
INPUT

    expected = <<EXPECTED
<p>paragraph second line</p>
<p>new paragraph</p>
<h1>a heading</h1>
<blockquote><p>a blockquote second line of blockquote</p>
<p>new paragraph within blockquote</p>
</blockquote>
<h2>another heading</h2>
<p>paragraph within <em>multiple <strong>styles</strong></em> and <tt>tt span</tt></p>
<p>similar, but with <strong>styles in <em>different</em> order</strong></p>
<p>again, a <strong>different <em>order</em></strong></p>
<ul>
<li>list item 1
<ul>
<li>nested list item 1</li>
<li>nested list item 2</li>
<li>nested list item 3</li>
</ul>
</li>
<li>list item 2</li>
</ul>
<pre>// this is a code block
notice how it can contain ''markup''
which would '''otherwise''' have &lt;tt&gt;special&lt;/tt&gt; meaning
although explicit entities &copy; are passed through unchanged
</pre>
<p>a normal paragraph again</p>
<ol>
<li>this is an ordered list</li>
<li>which continues
<ol>
<li>and has another ordered list</li>
<li>nested inside it</li>
</ol>
</li>
<li>and then falls back
<ul>
<li>and then nests another list</li>
<li>this time an unordered one
<ul>
<li>itself containing a nested list</li>
<li>which continues
<ol>
<li>and finally nests yet another ordered list</li>
<li>which continues</li>
</ol>
</li>
</ul>
</li>
<li>drops back quite a way</li>
</ul>
</li>
<li>and finally all the way
<ul>
<li>***** and finishes with an invalid item</li>
</ul>
</li>
</ol>
<h3>heading with missing closing tag</h3>
<ul>
<li>list</li>
</ul>
<ol>
<li>new list</li>
</ol>
EXPECTED

    @parser.parse(input).should == expected
  end

end

module UTF8
  module Invalid
    TWO_BYTES_MISSING_SECOND_BYTE     = [0b11011111].pack('C*')
    TWO_BYTES_MALFORMED_SECOND_BYTE   = [0b11011111, 0b00001111].pack('C*') # should be 10......
    OVERLONG                          = [0b11000000, 0b10000000].pack('C*') # lead byte is 110..... but code point is <= 127
    OVERLONG_ALT                      = [0b11000001, 0b10000000].pack('C*') # lead byte is 110..... but code point is <= 127
    THREE_BYTES_MISSING_SECOND_BYTE   = [0b11100000].pack('C*')
    THREE_BYTES_MISSING_THIRD_BYTE    = [0b11100000, 0b10000000].pack('C*')
    THREE_BYTES_MALFORMED_SECOND_BYTE = [0b11100000, 0b00001111, 0b10000000].pack('C*') # should be 10......
    THREE_BYTES_MALFORMED_THIRD_BYTE  = [0b11100000, 0b10000000, 0b00001111].pack('C*') # should be 10......
    EXCEEDS_ENCODABLE_RANGE_FOR_UCS2  = [0b11110000].pack('C*')
    UNEXPECTED_BYTE                   = [0b11111000].pack('C*')
  end # module Invalid
end # module UTF8

describe Wikitext, 'with invalidly encoded input' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should raise an exception for missing second byte' do
    lambda { @parser.parse(UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE) }.should raise_error(RangeError, /truncated/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE)
    }.should raise_error(RangeError, /truncated/)
  end

  it 'should raise an exception for malformed second byte' do
    lambda { @parser.parse(UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE)
    }.should raise_error(RangeError, /malformed/)
  end

  it 'should raise an exception for overlong sequence' do
    lambda { @parser.parse(UTF8::Invalid::OVERLONG) }.should raise_error(RangeError, /overlong/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::OVERLONG)
    }.should raise_error(RangeError, /overlong/)

    # alternate
    lambda { @parser.parse(UTF8::Invalid::OVERLONG_ALT) }.should raise_error(RangeError, /overlong/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::OVERLONG_ALT)
    }.should raise_error(RangeError, /overlong/)
  end

  it 'should raise an exception for missing second byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE) }.should raise_error(RangeError, /truncated/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE)
    }.should raise_error(RangeError, /truncated/)
  end

  it 'should raise an exception for missing third byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE) }.should raise_error(RangeError, /truncated/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE)
    }.should raise_error(RangeError, /truncated/)
  end

  it 'should raise an exception for malformed second byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE)
    }.should raise_error(RangeError, /malformed/)
  end

  it 'should raise an exception for malformed third byte in three-byte sequence' do
    lambda { @parser.parse(UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE)
    }.should raise_error(RangeError, /malformed/)
  end

  it 'should raise an exception for characters outside of the encodable range of UCS-2' do
    lambda { @parser.parse(UTF8::Invalid::EXCEEDS_ENCODABLE_RANGE_FOR_UCS2) }.should raise_error(RangeError, /exceeds/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::EXCEEDS_ENCODABLE_RANGE_FOR_UCS2)
    }.should raise_error(RangeError, /exceeds/)
  end

  it 'should raise an exception for unexpected bytes' do
    lambda { @parser.parse(UTF8::Invalid::UNEXPECTED_BYTE) }.should raise_error(RangeError, /unexpected byte/)
    lambda {
      @parser.parse('good text' + UTF8::Invalid::UNEXPECTED_BYTE)
    }.should raise_error(RangeError, /unexpected byte/)
  end
end

# this stuff is implicitly tested above, but test it here explicitly anyway
describe Wikitext, 'converting from UTF-8 to UCS-2' do
  before do
    @a = 'a string containing only ASCII codepoints'
    @b = 'a string with non-ASCII codepoints: áàâäÁÀÂÄ€ñ'
  end

  it 'should complain if passed nil' do
    lambda { Wikitext.utf8_to_ucs2(nil) }.should raise_error
  end

  it 'should do nothing on zero-length input' do
    Wikitext.utf8_to_ucs2('').should == ''
  end

  it 'should get the same answer as iconv' do
    Wikitext.utf8_to_ucs2(@a).should == Iconv.iconv('UCS-2LE', 'UTF-8', @a).shift
    Wikitext.utf8_to_ucs2(@b).should == Iconv.iconv('UCS-2LE', 'UTF-8', @b).shift
  end

  it 'should be able to round-trip' do
    Wikitext.ucs2_to_utf8(Wikitext.utf8_to_ucs2(@a)).should == @a
    Wikitext.ucs2_to_utf8(Wikitext.utf8_to_ucs2(@b)).should == @b
  end

  # these tests intimately tied to the error messages because I want to carefully check all error pathways
  it 'should reject invalidly encoded input' do
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::TWO_BYTES_MISSING_SECOND_BYTE) }.should   raise_error(RangeError, /truncated/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::TWO_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::OVERLONG) }.should                        raise_error(RangeError, /overlong/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::OVERLONG_ALT) }.should                    raise_error(RangeError, /overlong/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::THREE_BYTES_MISSING_SECOND_BYTE) }.should raise_error(RangeError, /truncated/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::THREE_BYTES_MISSING_THIRD_BYTE) }.should  raise_error(RangeError, /truncated/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::THREE_BYTES_MALFORMED_SECOND_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::THREE_BYTES_MALFORMED_THIRD_BYTE) }.should raise_error(RangeError, /malformed/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::EXCEEDS_ENCODABLE_RANGE_FOR_UCS2) }.should raise_error(RangeError, /exceeds/)
    lambda { Wikitext.utf8_to_ucs2(UTF8::Invalid::UNEXPECTED_BYTE) }.should raise_error(RangeError, /unexpected byte/)
  end
end

module UCS2
  module Invalid
    # invalid code points lie between 0xd800 and 0xdfff inclusive
    INVALID_CODE_POINT                = [0x00, 0xd9].pack('C*') # 0xdf40: written this way for little-endian systems
  end # module Invalid
end # module UCS2

# this stuff is implicitly tested above, but test it here explicitly anyway
describe Wikitext, 'converting from UCS-2 to UTF-8' do
  before do
    @a = Iconv.iconv('UCS-2LE', 'UTF-8', 'a string containing only ASCII codepoints').shift
    @b = Iconv.iconv('UCS-2LE', 'UTF-8', 'a string with non-ASCII codepoints: áàâäÁÀÂÄ€ñ').shift
  end

  it 'should complain if passed nil' do
    lambda { Wikitext.ucs2_to_utf8(nil) }.should raise_error
  end

  it 'should do nothing on zero-length input' do
    Wikitext.ucs2_to_utf8('').should == ''
  end

  it 'should get the same answer as iconv' do
    Wikitext.ucs2_to_utf8(@a).should == Iconv.iconv('UTF-8', 'UCS-2LE', @a).shift
    Wikitext.ucs2_to_utf8(@b).should == Iconv.iconv('UTF-8', 'UCS-2LE', @b).shift
  end

  it 'should be able to round-trip' do
    Wikitext.utf8_to_ucs2(Wikitext.ucs2_to_utf8(@a)).should == @a
    Wikitext.utf8_to_ucs2(Wikitext.ucs2_to_utf8(@b)).should == @b
  end

  # this test intimately tied to the error messages because I want to carefully check all error pathways
  it 'should reject invalidly encoded input' do
    lambda { Wikitext.ucs2_to_utf8(UCS2::Invalid::INVALID_CODE_POINT) }.should raise_error(RangeError, /code point not valid/)
  end
end

describe Wikitext, 'encoding an internal link target' do
  it 'should complain if passed nil' do
    lambda { Wikitext.encode_internal_link_target(nil) }.should raise_error
  end

  it 'should do nothing on zero-length input' do
    Wikitext.encode_internal_link_target('').should == ''
  end

  it 'should convert spaces into "%20"' do
    Wikitext.encode_internal_link_target('hello world').should == 'hello%20world'
  end

  it 'should convert reserved symbols into percent escapes' do
    Wikitext.encode_internal_link_target('http://www.apple.com/q?foo').should == 'http%3a%2f%2fwww.apple.com%2fq%3ffoo'
  end

  it 'should convert non-ASCII into UTF-8 and then apply percent escapes' do
    Wikitext.encode_internal_link_target('cañon').should == 'ca%c3%b1on'
  end

  it 'should get the same answer as URI.escape' do
    reserved = Regexp.new("[^#{URI::PATTERN::UNRESERVED}]")
    ['foo bar', 'http://www.google.com/search?q=hello&foo=bar', '€'].each do |string|
      Wikitext.encode_internal_link_target(string).should == URI.escape(string, reserved).downcase
    end
  end
end

