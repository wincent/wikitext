#!/usr/bin/env ruby
# Copyright 2007-2008 Wincent Colaiuta
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

describe Wikitext::Parser, 'with large slab of input text' do
  before do
    @parser = Wikitext::Parser.new
  end

  # the integration spec is designed to test all aspects of the parser at once
  it 'should handle complex input containing nested structures, syntax errors, and exercising a wide variety features' do
    input = dedent <<-END
      paragraph
      second line
      
      new paragraph
      
      literal &quot; HTML &copy; entities
      
      UTF-8 characters: € etc
      
      characters which have <special> meaning in "HTML" & all that
      
      = a heading =
      
      > a blockquote
      > second line of blockquote
      >
      > new paragraph within blockquote. this one features ''an
      > unclosed em span (should be automatically closed).
      > 
      > Note how the optional space following the blockquote marker
      >has no effect.
      
      Now check out the alternative blockquote syntax:
      
      <blockquote>
      This type of syntax is useful for
      when you have a large number of lines
      in the blockquote and it would be tedious
      to prefix every line with a ">".
      
      Note that I can divide things up into
      paragraphs.
      
      <blockquote>And I can nest other
      blockquotes</blockquote>
      * and
      *# nest
      *# lists
      * too
      
      <pre>and include pre blocks as well.
      This uses the alternative pre block
      syntax as you can see.</pre>
      </blockquote>
      
      == another heading ==
      
      paragraph within ''multiple '''styles''''' and <tt>tt span</tt>
      
      similar, but with '''styles in ''different'' order'''
      
      again, a '''different ''order'''''
      
      * list item 1 [http://google.com/ unterminated
      ** nested list item 1 with [[bad link
      ** nested list item 2 with unclosed <tt>span
      ** nested list item 3
      * list item 2
      
       // this is a code block
       notice how it can contain ''markup''
       which would '''otherwise''' have <tt>special</tt> meaning
       although explicit entities &copy; are passed through unchanged
       
      a normal paragraph again
      
      > This is another blockquote which demonstrates that we
      > can nest other structures inside of it. For example, here
      > we have a code sample:
      >
      >  line 1
      >  line 2
      >
      > And now back to the normal blockquote again.
      >
      > * here
      > * is
      > * a
      > * list
      >
      > And here is a link to [[something]], and some ''other''
      > `styled` '''spans'''.
      >
      > > Finally we have
      > > a nested blockquote.
      > > # Which itself contains
      > > ## a nested list
      
      This is where we show a link to an article on [[GCC]].
      Related to that, [[GCC|a link]] to the same
      article but with custom link text.
      
      External links [http://example.com work too].
      As well as autolinks as seen http://example.com/
      here.
      
      Look at how we handle bad syntax. [[This is an unterminated
      link. And [http://example.com/ is another.
      
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
    END

    expected = dedent <<-END
      <p>paragraph second line</p>
      <p>new paragraph</p>
      <p>literal &quot; HTML &copy; entities</p>
      <p>UTF-8 characters: &#x20ac; etc</p>
      <p>characters which have &lt;special&gt; meaning in &quot;HTML&quot; &amp; all that</p>
      <h1>a heading</h1>
      <blockquote>
        <p>a blockquote second line of blockquote</p>
        <p>new paragraph within blockquote. this one features <em>an</em> unclosed em span (should be automatically closed).</p>
        <p>Note how the optional space following the blockquote marker has no effect.</p>
      </blockquote>
      <p>Now check out the alternative blockquote syntax:</p>
      <blockquote>
        <p>This type of syntax is useful for when you have a large number of lines in the blockquote and it would be tedious to prefix every line with a &quot;&gt;&quot;.</p>
        <p>Note that I can divide things up into paragraphs.</p>
        <blockquote>
          <p>And I can nest other blockquotes</p>
        </blockquote>
        <ul>
          <li>and
            <ol>
              <li>nest</li>
              <li>lists</li>
            </ol>
          </li>
          <li>too</li>
        </ul>
        <pre>and include pre blocks as well.
      This uses the alternative pre block
      syntax as you can see.</pre>
      </blockquote>
      <h2>another heading</h2>
      <p>paragraph within <em>multiple <strong>styles</strong></em> and <tt>tt span</tt></p>
      <p>similar, but with <strong>styles in <em>different</em> order</strong></p>
      <p>again, a <strong>different <em>order</em></strong></p>
      <ul>
        <li>list item 1 [<a href="http://google.com/" class="external">http://google.com/</a> unterminated
          <ul>
            <li>nested list item 1 with [[bad link</li>
            <li>nested list item 2 with unclosed <tt>span</tt></li>
            <li>nested list item 3</li>
          </ul>
        </li>
        <li>list item 2</li>
      </ul>
      <pre>// this is a code block
      notice how it can contain ''markup''
      which would '''otherwise''' have &lt;tt&gt;special&lt;/tt&gt; meaning
      although explicit entities &copy; are passed through unchanged</pre>
      <p>a normal paragraph again</p>
      <blockquote>
        <p>This is another blockquote which demonstrates that we can nest other structures inside of it. For example, here we have a code sample:</p>
        <pre>line 1
      line 2</pre>
        <p>And now back to the normal blockquote again.</p>
        <ul>
          <li>here</li>
          <li>is</li>
          <li>a</li>
          <li>list</li>
        </ul>
        <p>And here is a link to <a href="/wiki/something">something</a>, and some <em>other</em> <tt>styled</tt> <strong>spans</strong>.</p>
        <blockquote>
          <p>Finally we have a nested blockquote.</p>
          <ol>
            <li>Which itself contains
              <ol>
                <li>a nested list</li>
              </ol>
            </li>
          </ol>
        </blockquote>
      </blockquote>
      <p>This is where we show a link to an article on <a href="/wiki/GCC">GCC</a>. Related to that, <a href="/wiki/GCC">a link</a> to the same article but with custom link text.</p>
      <p>External links <a href="http://example.com" class="external">work too</a>. As well as autolinks as seen <a href="http://example.com/" class="external">http://example.com/</a> here.</p>
      <p>Look at how we handle bad syntax. [[This is an unterminated link. And [<a href="http://example.com/" class="external">http://example.com/</a> is another.</p>
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
    END

    @parser.parse(input).should == expected
  end
end
