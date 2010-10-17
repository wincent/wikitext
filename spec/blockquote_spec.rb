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

describe Wikitext::Parser, 'standard blockquotes (">" in first column)' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should treat ">" in first column as a blockquote marker' do
    expected = dedent <<-END
      <blockquote>
        <p>foo</p>
      </blockquote>
    END
    @parser.parse('>foo').should == expected
  end

  it 'should accept (and ignore) one optional space after the ">"' do
    expected = dedent <<-END
      <blockquote>
        <p>foo</p>
      </blockquote>
    END
    @parser.parse('> foo').should == expected
  end

  it 'should recognize consecutive ">" as continuance of blockquote section' do
    expected = dedent <<-END
      <blockquote>
        <p>foo bar</p>
      </blockquote>
    END
    @parser.parse("> foo\n> bar").should == expected
  end

  it 'should not give ">" special treatment when not on the far left' do
    @parser.parse('foo > bar').should == "<p>foo &gt; bar</p>\n"
  end

  it 'should allow nesting of blockquotes' do
    expected = dedent <<-END
      <blockquote>
        <blockquote>
          <p>foo</p>
        </blockquote>
      </blockquote>
    END
    @parser.parse('> > foo').should == expected
  end

  it 'should allow opening of a nested blockquote after other content' do
    expected = dedent <<-END
      <blockquote>
        <p>foo</p>
        <blockquote>
          <p>bar</p>
        </blockquote>
      </blockquote>
    END
    @parser.parse("> foo\n> > bar").should == expected
  end

  it 'should allow opening of a nested blockquote before other content' do
    expected = dedent <<-END
      <blockquote>
        <blockquote>
          <p>foo</p>
        </blockquote>
        <p>bar</p>
      </blockquote>
    END
    @parser.parse("> > foo\n> bar").should == expected
  end

  it 'should accept an empty blockquote' do
    expected = dedent <<-END
      <blockquote>
      </blockquote>
    END
    @parser.parse('>').should == expected
  end

  it 'should jump out of blockquote mode on seeing a normal line of text' do
    expected =  dedent <<-END
      <blockquote>
        <p>foo</p>
      </blockquote>
      <p>bar</p>
    END
    @parser.parse("> foo\nbar").should == expected
  end

  it 'should allow nesting of h1 blocks' do
    expected = dedent <<-END
      <blockquote>
        <h1>foo</h1>
      </blockquote>
    END
    @parser.parse('> = foo =').should == expected
  end

  it 'should allow nesting of h2 blocks' do
    expected =  dedent <<-END
      <blockquote>
        <h2>foo</h2>
      </blockquote>
    END
    @parser.parse('> == foo ==').should == expected
  end

  it 'should allow nesting of h3 blocks' do
    expected = dedent <<-END
      <blockquote>
        <h3>foo</h3>
      </blockquote>
    END
    @parser.parse('> === foo ===').should == expected
  end

  it 'should allow nesting of h4 blocks' do
    expected = dedent <<-END
      <blockquote>
        <h4>foo</h4>
      </blockquote>
    END
    @parser.parse('> ==== foo ====').should == expected
  end

  it 'should allow nesting of h5 blocks' do
    expected = dedent <<-END
      <blockquote>
        <h5>foo</h5>
      </blockquote>
    END
    @parser.parse('> ===== foo =====').should == expected
  end

  it 'should allow nesting of h6 blocks' do
     expected = dedent <<-END
      <blockquote>
        <h6>foo</h6>
      </blockquote>
    END
    @parser.parse('> ====== foo ======').should == expected
  end

  it 'should allow alternating nested paragraphs and pre blocks' do
    # was a bug
    input = dedent <<-END
      > para 1
      >
      >  pre 1
      >  pre 2
      >
      > para 2
    END
    expected = dedent <<-END
      <blockquote>
        <p>para 1</p>
        <pre>pre 1
      pre 2</pre>
        <p>para 2</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should allow nesting of styled spans inside blockquotes' do
    input = dedent <<-END
      > link to [[something]], and ''other''
      > `styled` '''spans'''.
    END
    expected = dedent <<-END
      <blockquote>
        <p>link to <a href="/wiki/something">something</a>, and <em>other</em> <code>styled</code> <strong>spans</strong>.</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should allow complex nestings inside blockquotes' do
    # was a bug: further reduced in the following example, "should handle TT spans inside blockquotes"
    input = dedent <<-END
       code block
      
      a normal para
      
      > will nest
      >
      >  line 1
      >  line 2
      >
      > other
      >
      > * here
      > * is
      > * a
      > * list
      >
      > outer para with ''styled''
      > `stuff` in it
      >
      > > inner
      > > blockquote
      > > # inner list
      > > ## nested list
      
      follow-up para
    END
    expected = dedent <<-END
      <pre>code block</pre>
      <p>a normal para</p>
      <blockquote>
        <p>will nest</p>
        <pre>line 1
      line 2</pre>
        <p>other</p>
        <ul>
          <li>here</li>
          <li>is</li>
          <li>a</li>
          <li>list</li>
        </ul>
        <p>outer para with <em>styled</em> <code>stuff</code> in it</p>
        <blockquote>
          <p>inner blockquote</p>
          <ol>
            <li>inner list
              <ol>
                <li>nested list</li>
              </ol>
            </li>
          </ol>
        </blockquote>
      </blockquote>
      <p>follow-up para</p>
    END
    @parser.parse(input).should == expected
  end

  it 'should handle TT spans inside blockquotes' do
    # was a bug: this is a minimally reduced test case extracted from the integration tests
    input = dedent <<-END
      > some
      > `styled`
    END
    expected = dedent <<-END
      <blockquote>
        <p>some <code>styled</code></p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end


  it 'should handled nested lists which immediately follow paragraphs' do
    # was a bug: this is a minimally reduced test case extracted from the integration tests
    input = dedent <<-END
      > Finally
      > # Which
    END
    expected = dedent <<-END
      <blockquote>
        <p>Finally</p>
        <ol>
          <li>Which</li>
        </ol>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  # TODO: tests for nesting other types of blocks
end

describe Wikitext::Parser, 'literal BLOCKQUOTE_START/BLOCKQUOTE_END tags' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should accept literal BLOCKQUOTE_START/BLOCKQUOTE_END tags as an alternative to the standard syntax' do
    input = '<blockquote>hello</blockquote>'
    expected = dedent <<-END
      <blockquote>
        <p>hello</p>
      </blockquote>
    END
    @parser.parse(input).should == expected

    # alternative 1
    input = dedent <<-END
      <blockquote>
      hello
      </blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>hello</p>
      </blockquote>
    END
    @parser.parse(input).should == expected

    # alternative 2
    input = dedent <<-END
      <blockquote>hello
      </blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>hello</p>
      </blockquote>
    END
    @parser.parse(input).should == expected

    # alternative 3
    input = dedent <<-END
      <blockquote>
      hello</blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>hello</p>
      </blockquote>
    END
    @parser.parse(input).should == expected

    # note what happens if we indent (whitespace gets carried through; it is not identified as a PRE block
    # in reality you'd never indent when editing wikitext anyway; the idea is to free yourself from details like that
    input = dedent <<-END
      <blockquote>
        hello
      </blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>  hello</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should merge consecutive lines into a single paragraph' do
    input = dedent <<-END
      <blockquote>foo
      bar
      baz</blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>foo bar baz</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should process paragraph breaks' do
    input = dedent <<-END
      <blockquote>foo
      
      baz</blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>foo</p>
        <p>baz</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should pass through PRE tokens unaltered' do
    input = dedent <<-END
      <blockquote>foo
       bar</blockquote>
    END

    # note the extra space: one for the CRLF and another for the PRE token
    expected = dedent <<-END
      <blockquote>
        <p>foo  bar</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should terminate open span-level elements on hitting the newline' do
    # for now just test with EM; potentially add more examples later
    input = dedent <<-END
      <blockquote>foo ''bar
      baz</blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>foo <em>bar</em> baz</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should pass through BLOCKQUOTE tokens escaped' do
    input = dedent <<-END
      <blockquote>foo
      > bar
      baz</blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>foo &gt; bar baz</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should be able to nest single-item unordered lists' do
    input = '<blockquote>* foo</blockquote>'
    expected = dedent <<-END
      <blockquote>
        <ul>
          <li>foo</li>
        </ul>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should be able to nest multi-item unordered lists' do
    input = dedent <<-END
      <blockquote>
      * foo
      * bar
      * baz
      </blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <ul>
          <li>foo</li>
          <li>bar</li>
          <li>baz</li>
        </ul>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should be able to nest nested unordered lists' do
    input = dedent <<-END
      <blockquote>
      * foo
      ** bar
      * baz
      </blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <ul>
          <li>foo
            <ul>
              <li>bar</li>
            </ul>
          </li>
          <li>baz</li>
        </ul>
      </blockquote>
    END
    @parser.parse(input).should == expected

    # note that the exact placement of the closing tag doesn't matter
    input = dedent <<-END
      <blockquote>
      * foo
      ** bar
      * baz</blockquote>
    END
    @parser.parse(input).should == expected

    # likewise for the opening tag
    input = dedent <<-END
      <blockquote>* foo
      ** bar
      * baz
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should be able to nest blockquotes' do
    input = dedent <<-END
      <blockquote>
      foo
      <blockquote>
      bar
      </blockquote>
      baz
      </blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>foo</p>
        <blockquote>
          <p>bar</p>
        </blockquote>
        <p>baz</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should be able to nest pre blocks' do
    input = dedent <<-END
      <blockquote>
      outer 1
      <pre>inner 1
      inner 2</pre>
      outer 2
      </blockquote>
    END
    expected = dedent <<-END
      <blockquote>
        <p>outer 1</p>
        <pre>inner 1
      inner 2</pre>
        <p>outer 2</p>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end

  it 'should support nesting of H1 spans' do
    input = dedent <<-END
      <blockquote>
      = foo =
      bar
      </blockquote>
    END

    expected = dedent <<-END
      <blockquote>
        <h1>foo</h1>
        <p>bar</p>
      </blockquote>
    END
    @parser.parse(input).should == expected

    # but note that this won't work
    # the second "=" is not recognized as an H1_END because the scanner has no lookahead at the token level
    input = '<blockquote>= foo =</blockquote>'
    expected = dedent <<-END
      <blockquote>
        <h1>foo =</h1>
      </blockquote>
    END
    @parser.parse(input).should == expected
  end
end
