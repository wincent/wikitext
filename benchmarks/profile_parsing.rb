#!/usr/bin/env ruby
# encoding: utf-8
# Copyright 2008-2009 Wincent Colaiuta. All rights reserved.
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

require File.join(File.dirname(__FILE__), '..', 'ext', 'wikitext')
require 'benchmark'

# 91 characters, 91 bytes
short_slab_of_ASCII_text  = '* Lorem [[ipsum|hello]] dolor sit amet, `consectetuer` http://example.com/ adipiscing elit.'

# compare against the expected output to ensure correctness
short_slab_of_ASCII_text_output = <<SLAB
<ul>
  <li>Lorem <a href="/wiki/ipsum">hello</a> dolor sit amet, <tt>consectetuer</tt> <a href="http://example.com/" class="external">http://example.com/</a> adipiscing elit.</li>
</ul>
SLAB

# 91 characters, 122 bytes
short_slab_of_UTF8_text   = '* Lór€m [[ïpsûm|h€llö]] dólór sït àm€t, `cóñs€ct€tû€r` http://example.com/ àdïpïscïñg €lït.'

short_slab_of_UTF8_text_output = <<SLAB
<ul>
  <li>L&#x00f3;r&#x20ac;m <a href="/wiki/%c3%afps%c3%bbm">h&#x20ac;ll&#x00f6;</a> d&#x00f3;l&#x00f3;r s&#x00ef;t &#x00e0;m&#x20ac;t, <tt>c&#x00f3;&#x00f1;s&#x20ac;ct&#x20ac;t&#x00fb;&#x20ac;r</tt> <a href="http://example.com/" class="external">http://example.com/</a> &#x00e0;d&#x00ef;p&#x00ef;sc&#x00ef;&#x00f1;g &#x20ac;l&#x00ef;t.</li>
</ul>
SLAB

# 1415 characters, 1415 bytes
longer_slab_of_ASCII_text = <<SLAB
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
SLAB

longer_slab_of_ASCII_text_output = <<SLAB
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
SLAB

# 1415 characters, 2061 bytes
longer_slab_of_UTF8_text  = <<SLAB
pärägräph
∫€cöñd lîñ€

ñ€w pärägräph

= ä h€ädîñg =

> ä blöckquöt€
> ∫€cöñd lîñ€ öf blöckquöt€
>
> ñ€w pärägräph wîthîñ blöckquöt€

== äñöth€r h€ädîñg ==

pärägräph wîthîñ ''multîpl€ '''∫tyl€∫''''' äñd <tt>tt ∫päñ</tt>

∫îmîlär, but wîth '''∫tyl€∫ îñ ''dîff€r€ñt'' örd€r'''

ägäîñ, ä '''dîff€r€ñt ''örd€r'''''

* lî∫t ît€m 1
** ñ€∫t€d lî∫t ît€m 1
** ñ€∫t€d lî∫t ît€m 2
** ñ€∫t€d lî∫t ît€m 3
* lî∫t ît€m 2

 // thî∫ î∫ ä cöd€ blöck
 ñötîc€ höw ît cäñ cöñtäîñ ''märkup''
 whîch wöuld '''öth€rwî∫€''' häv€ <tt>∫p€cîäl</tt> m€äñîñg
 älthöugh €xplîcît €ñtîtî€∫ &cöpy; är€ pä∫∫€d thröugh uñchäñg€d

ä ñörmäl pärägräph ägäîñ

Thî∫ î∫ wh€r€ w€ ∫höw ä lîñk tö äñ ärtîcl€ öñ [[GCC]].
R€lät€d tö thät, [[GCC|ä lîñk]] tö th€ ∫äm€
ärtîcl€ but wîth cu∫töm lîñk t€xt.

Ext€rñäl lîñk∫ [http://example.com wörk töö].
A∫ w€ll ä∫ äutölîñk∫ ä∫ ∫€€ñ http://example.com/
her€.

Löök ät höw w€ häñdl€ bäd ∫yñtäx. [[Thî∫ î∫ äñ uñt€rmîñät€d
lîñk. Añd [http://example.com/ î∫ äñöth€r.

# thî∫ î∫ äñ örd€r€d lî∫t
# whîch cöñtîñu€∫
## äñd hä∫ äñöth€r örd€r€d lî∫t
## ñ€∫t€d îñ∫îd€ ît
# äñd th€ñ fäll∫ bäck
#* äñd th€ñ ñ€∫t∫ äñöth€r lî∫t
#* thî∫ tîm€ äñ uñörd€r€d öñ€
#** ît∫€lf cöñtäîñîñg ä ñ€∫t€d lî∫t
#** whîch cöñtîñu€∫
#**# äñd fîñälly ñ€∫t∫ y€t äñöth€r örd€r€d lî∫t
#**# whîch cöñtîñu€∫
#* dröp∫ bäck quît€ ä wäy
# äñd fîñälly äll th€ wäy
#****** äñd fîñî∫h€∫ wîth äñ îñvälîd ît€m

=== h€ädîñg wîth mî∫∫îñg clö∫îñg täg
* lî∫t
# ñ€w lî∫t
SLAB

longer_slab_of_UTF8_text_output = <<SLAB
<p>p&#x00e4;r&#x00e4;gr&#x00e4;ph &#x222b;&#x20ac;c&#x00f6;&#x00f1;d l&#x00ee;&#x00f1;&#x20ac;</p>
<p>&#x00f1;&#x20ac;w p&#x00e4;r&#x00e4;gr&#x00e4;ph</p>
<h1>&#x00e4; h&#x20ac;&#x00e4;d&#x00ee;&#x00f1;g</h1>
<blockquote>
  <p>&#x00e4; bl&#x00f6;ckqu&#x00f6;t&#x20ac; &#x222b;&#x20ac;c&#x00f6;&#x00f1;d l&#x00ee;&#x00f1;&#x20ac; &#x00f6;f bl&#x00f6;ckqu&#x00f6;t&#x20ac;</p>
  <p>&#x00f1;&#x20ac;w p&#x00e4;r&#x00e4;gr&#x00e4;ph w&#x00ee;th&#x00ee;&#x00f1; bl&#x00f6;ckqu&#x00f6;t&#x20ac;</p>
</blockquote>
<h2>&#x00e4;&#x00f1;&#x00f6;th&#x20ac;r h&#x20ac;&#x00e4;d&#x00ee;&#x00f1;g</h2>
<p>p&#x00e4;r&#x00e4;gr&#x00e4;ph w&#x00ee;th&#x00ee;&#x00f1; <em>mult&#x00ee;pl&#x20ac; <strong>&#x222b;tyl&#x20ac;&#x222b;</strong></em> &#x00e4;&#x00f1;d <tt>tt &#x222b;p&#x00e4;&#x00f1;</tt></p>
<p>&#x222b;&#x00ee;m&#x00ee;l&#x00e4;r, but w&#x00ee;th <strong>&#x222b;tyl&#x20ac;&#x222b; &#x00ee;&#x00f1; <em>d&#x00ee;ff&#x20ac;r&#x20ac;&#x00f1;t</em> &#x00f6;rd&#x20ac;r</strong></p>
<p>&#x00e4;g&#x00e4;&#x00ee;&#x00f1;, &#x00e4; <strong>d&#x00ee;ff&#x20ac;r&#x20ac;&#x00f1;t <em>&#x00f6;rd&#x20ac;r</em></strong></p>
<ul>
  <li>l&#x00ee;&#x222b;t &#x00ee;t&#x20ac;m 1
    <ul>
      <li>&#x00f1;&#x20ac;&#x222b;t&#x20ac;d l&#x00ee;&#x222b;t &#x00ee;t&#x20ac;m 1</li>
      <li>&#x00f1;&#x20ac;&#x222b;t&#x20ac;d l&#x00ee;&#x222b;t &#x00ee;t&#x20ac;m 2</li>
      <li>&#x00f1;&#x20ac;&#x222b;t&#x20ac;d l&#x00ee;&#x222b;t &#x00ee;t&#x20ac;m 3</li>
    </ul>
  </li>
  <li>l&#x00ee;&#x222b;t &#x00ee;t&#x20ac;m 2</li>
</ul>
<pre>// th&#x00ee;&#x222b; &#x00ee;&#x222b; &#x00e4; c&#x00f6;d&#x20ac; bl&#x00f6;ck
&#x00f1;&#x00f6;t&#x00ee;c&#x20ac; h&#x00f6;w &#x00ee;t c&#x00e4;&#x00f1; c&#x00f6;&#x00f1;t&#x00e4;&#x00ee;&#x00f1; ''m&#x00e4;rkup''
wh&#x00ee;ch w&#x00f6;uld '''&#x00f6;th&#x20ac;rw&#x00ee;&#x222b;&#x20ac;''' h&#x00e4;v&#x20ac; &lt;tt&gt;&#x222b;p&#x20ac;c&#x00ee;&#x00e4;l&lt;/tt&gt; m&#x20ac;&#x00e4;&#x00f1;&#x00ee;&#x00f1;g
&#x00e4;lth&#x00f6;ugh &#x20ac;xpl&#x00ee;c&#x00ee;t &#x20ac;&#x00f1;t&#x00ee;t&#x00ee;&#x20ac;&#x222b; &amp;c&#x00f6;py; &#x00e4;r&#x20ac; p&#x00e4;&#x222b;&#x222b;&#x20ac;d thr&#x00f6;ugh u&#x00f1;ch&#x00e4;&#x00f1;g&#x20ac;d</pre>
<p>&#x00e4; &#x00f1;&#x00f6;rm&#x00e4;l p&#x00e4;r&#x00e4;gr&#x00e4;ph &#x00e4;g&#x00e4;&#x00ee;&#x00f1;</p>
<p>Th&#x00ee;&#x222b; &#x00ee;&#x222b; wh&#x20ac;r&#x20ac; w&#x20ac; &#x222b;h&#x00f6;w &#x00e4; l&#x00ee;&#x00f1;k t&#x00f6; &#x00e4;&#x00f1; &#x00e4;rt&#x00ee;cl&#x20ac; &#x00f6;&#x00f1; <a href="/wiki/GCC">GCC</a>. R&#x20ac;l&#x00e4;t&#x20ac;d t&#x00f6; th&#x00e4;t, <a href="/wiki/GCC">&#x00e4; l&#x00ee;&#x00f1;k</a> t&#x00f6; th&#x20ac; &#x222b;&#x00e4;m&#x20ac; &#x00e4;rt&#x00ee;cl&#x20ac; but w&#x00ee;th cu&#x222b;t&#x00f6;m l&#x00ee;&#x00f1;k t&#x20ac;xt.</p>
<p>Ext&#x20ac;r&#x00f1;&#x00e4;l l&#x00ee;&#x00f1;k&#x222b; <a href="http://example.com" class="external">w&#x00f6;rk t&#x00f6;&#x00f6;</a>. A&#x222b; w&#x20ac;ll &#x00e4;&#x222b; &#x00e4;ut&#x00f6;l&#x00ee;&#x00f1;k&#x222b; &#x00e4;&#x222b; &#x222b;&#x20ac;&#x20ac;&#x00f1; <a href="http://example.com/" class="external">http://example.com/</a> her&#x20ac;.</p>
<p>L&#x00f6;&#x00f6;k &#x00e4;t h&#x00f6;w w&#x20ac; h&#x00e4;&#x00f1;dl&#x20ac; b&#x00e4;d &#x222b;y&#x00f1;t&#x00e4;x. [[Th&#x00ee;&#x222b; &#x00ee;&#x222b; &#x00e4;&#x00f1; u&#x00f1;t&#x20ac;rm&#x00ee;&#x00f1;&#x00e4;t&#x20ac;d l&#x00ee;&#x00f1;k. A&#x00f1;d [<a href="http://example.com/" class="external">http://example.com/</a> &#x00ee;&#x222b; &#x00e4;&#x00f1;&#x00f6;th&#x20ac;r.</p>
<ol>
  <li>th&#x00ee;&#x222b; &#x00ee;&#x222b; &#x00e4;&#x00f1; &#x00f6;rd&#x20ac;r&#x20ac;d l&#x00ee;&#x222b;t</li>
  <li>wh&#x00ee;ch c&#x00f6;&#x00f1;t&#x00ee;&#x00f1;u&#x20ac;&#x222b;
    <ol>
      <li>&#x00e4;&#x00f1;d h&#x00e4;&#x222b; &#x00e4;&#x00f1;&#x00f6;th&#x20ac;r &#x00f6;rd&#x20ac;r&#x20ac;d l&#x00ee;&#x222b;t</li>
      <li>&#x00f1;&#x20ac;&#x222b;t&#x20ac;d &#x00ee;&#x00f1;&#x222b;&#x00ee;d&#x20ac; &#x00ee;t</li>
    </ol>
  </li>
  <li>&#x00e4;&#x00f1;d th&#x20ac;&#x00f1; f&#x00e4;ll&#x222b; b&#x00e4;ck
    <ul>
      <li>&#x00e4;&#x00f1;d th&#x20ac;&#x00f1; &#x00f1;&#x20ac;&#x222b;t&#x222b; &#x00e4;&#x00f1;&#x00f6;th&#x20ac;r l&#x00ee;&#x222b;t</li>
      <li>th&#x00ee;&#x222b; t&#x00ee;m&#x20ac; &#x00e4;&#x00f1; u&#x00f1;&#x00f6;rd&#x20ac;r&#x20ac;d &#x00f6;&#x00f1;&#x20ac;
        <ul>
          <li>&#x00ee;t&#x222b;&#x20ac;lf c&#x00f6;&#x00f1;t&#x00e4;&#x00ee;&#x00f1;&#x00ee;&#x00f1;g &#x00e4; &#x00f1;&#x20ac;&#x222b;t&#x20ac;d l&#x00ee;&#x222b;t</li>
          <li>wh&#x00ee;ch c&#x00f6;&#x00f1;t&#x00ee;&#x00f1;u&#x20ac;&#x222b;
            <ol>
              <li>&#x00e4;&#x00f1;d f&#x00ee;&#x00f1;&#x00e4;lly &#x00f1;&#x20ac;&#x222b;t&#x222b; y&#x20ac;t &#x00e4;&#x00f1;&#x00f6;th&#x20ac;r &#x00f6;rd&#x20ac;r&#x20ac;d l&#x00ee;&#x222b;t</li>
              <li>wh&#x00ee;ch c&#x00f6;&#x00f1;t&#x00ee;&#x00f1;u&#x20ac;&#x222b;</li>
            </ol>
          </li>
        </ul>
      </li>
      <li>dr&#x00f6;p&#x222b; b&#x00e4;ck qu&#x00ee;t&#x20ac; &#x00e4; w&#x00e4;y</li>
    </ul>
  </li>
  <li>&#x00e4;&#x00f1;d f&#x00ee;&#x00f1;&#x00e4;lly &#x00e4;ll th&#x20ac; w&#x00e4;y
    <ul>
      <li>***** &#x00e4;&#x00f1;d f&#x00ee;&#x00f1;&#x00ee;&#x222b;h&#x20ac;&#x222b; w&#x00ee;th &#x00e4;&#x00f1; &#x00ee;&#x00f1;v&#x00e4;l&#x00ee;d &#x00ee;t&#x20ac;m</li>
    </ul>
  </li>
</ol>
<h3>h&#x20ac;&#x00e4;d&#x00ee;&#x00f1;g w&#x00ee;th m&#x00ee;&#x222b;&#x222b;&#x00ee;&#x00f1;g cl&#x00f6;&#x222b;&#x00ee;&#x00f1;g t&#x00e4;g</h3>
<ul>
  <li>l&#x00ee;&#x222b;t</li>
</ul>
<ol>
  <li>&#x00f1;&#x20ac;w l&#x00ee;&#x222b;t</li>
</ol>
SLAB

def parse job, description, parser, input
  job.report(description) do
    parser.profiling_parse input
  end
end

parser = Wikitext::Parser.new

# sanity check
raise 'mismatch (short slab of ASCII text)' unless (parser.parse(short_slab_of_ASCII_text) == short_slab_of_ASCII_text_output)
raise 'mismatch (short slab of UTF-8 text)' unless (parser.parse(short_slab_of_UTF8_text) == short_slab_of_UTF8_text_output)
raise 'mismatch (longer slab of ASCII text)' unless (parser.parse(longer_slab_of_ASCII_text) == longer_slab_of_ASCII_text_output)
raise 'mismatch (longer slab of UTF-8 text)' unless (parser.parse(longer_slab_of_UTF8_text) == longer_slab_of_UTF8_text_output)

Benchmark.bmbm do |job|
  parse job, 'short slab of ASCII text', parser, short_slab_of_ASCII_text
  parse job, 'short slab of UTF-8 text', parser, short_slab_of_UTF8_text
  parse job, 'longer slab of ASCII text', parser, longer_slab_of_ASCII_text
  parse job, 'longer slab of UTF-8 text', parser, longer_slab_of_UTF8_text
end
