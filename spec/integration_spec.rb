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
INPUT

    expected = <<EXPECTED
<p>paragraph second line</p>
<p>new paragraph</p>
<h1>a heading</h1>
<blockquote>
  <p>a blockquote second line of blockquote</p>
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
although explicit entities &copy; are passed through unchanged</pre>
<p>a normal paragraph again</p>
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
EXPECTED

    @parser.parse(input).should == expected
  end
end
