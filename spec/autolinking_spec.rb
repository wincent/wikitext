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

describe Wikitext::Parser, 'autolinking' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should default to autolinking on' do
    @parser.autolink.should == true
  end

  describe 'on' do
    it 'should convert HTTP URIs into hyperlinks' do
      uri = 'http://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="http://example.com/" class="external">http://example.com/</a></p>\n}
    end

    it 'should convert HTTPS URIs into hyperlinks' do
      uri = 'https://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="https://example.com/" class="external">https://example.com/</a></p>\n}
    end

    it 'should convert FTP URIs into hyperlinks' do
      uri = 'ftp://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="ftp://example.com/" class="external">ftp://example.com/</a></p>\n}
    end

    it 'should convert mailto URIs into hyperlinks' do
      uri = 'mailto:user@example.com'
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="mailto">mailto:user@example.com</a></p>\n}
    end

    it 'should convert SVN URIs into hyperlinks' do
      uri = 'svn://example.com/'
      @parser.parse(uri).should == %Q{<p><a href="svn://example.com/" class="external">svn://example.com/</a></p>\n}
    end

    it 'should apple the external_link_class CSS class if set' do
      uri = 'http://example.com/'
      @parser.external_link_class = 'bar'
      @parser.parse(uri).should == %Q{<p><a href="http://example.com/" class="bar">http://example.com/</a></p>\n}
    end

    it 'should apply no CSS if external_link_class is set to nil' do
      uri = 'http://example.com/'
      @parser.external_link_class = nil
      @parser.parse(uri).should == %Q{<p><a href="http://example.com/">http://example.com/</a></p>\n}
    end

    it 'should pass through URIs unchanged inside <nowiki></nowiki> spans' do
      @parser.parse("<nowiki>http://example.com/</nowiki>").should == "<p>http://example.com/</p>\n"
    end

    it 'should autolink URIs inside <pre></pre> spans' do
      input     = ' http://example.com/'
      expected  = %Q{<pre><a href="http://example.com/" class="external">http://example.com/</a></pre>\n}
      @parser.parse(input).should == expected
      @parser.external_link_class = nil
      expected  = %Q{<pre><a href="http://example.com/">http://example.com/</a></pre>\n}
      @parser.parse(input).should == expected
    end

    it 'should convert emails into hyperlinks' do
      uri = 'user@example.com'
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="mailto">user@example.com</a></p>\n}
    end

    it 'should apply the mailto CSS class if set (raw address)' do
      uri = 'user@example.com'
      @parser.mailto_class = 'foo'
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="foo">user@example.com</a></p>\n}
    end

    it 'should apply the mailto CSS class if set (mailto URI)' do
      uri = 'mailto:user@example.com'
      @parser.mailto_class = 'foo'
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="foo">mailto:user@example.com</a></p>\n}
    end

    it 'should apply no CSS if the mailto class is set to nil (raw address)' do
      uri = 'user@example.com'
      @parser.mailto_class = nil
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com">user@example.com</a></p>\n}
    end

    it 'should apply no CSS if the mailto class is set to nil (mailto URI)' do
      uri = 'mailto:user@example.com'
      @parser.mailto_class = nil
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com">mailto:user@example.com</a></p>\n}
    end

    it 'should pass through emails unchanged inside <nowiki></nowiki> spans' do
      @parser.parse("<nowiki>user@example.com</nowiki>").should == "<p>user@example.com</p>\n"  # was a crasher
    end

    it 'should pass through emails unchanged inside <pre></pre> blocks' do
      @parser.parse(" user@example.com").should == "<pre>user@example.com</pre>\n"  # was a crasher
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

    it 'should not convert emails into hyperlinks' do
      @parser.parse('user@example.com').should == "<p>user@example.com</p>\n"
    end
  end

  # "special" URI characters are characters that are valid _within_ a URI
  # but if they appear as delimiters (at the end of a URI) we want them to be excluded from the URI
  describe 'handling "special" URI characters' do
    describe 'after HTTP URIs' do
      it 'should terminate if followed by a period' do
        input     = 'Try http://example.com/.'
        expected  = %Q{<p>Try <a href="http://example.com/" class="external">http://example.com/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on periods that occur within the URI' do
        input     = 'Try http://www.example.com/.'
        expected  = %Q{<p>Try <a href="http://www.example.com/" class="external">http://www.example.com/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a colon' do
        input     = 'Try http://example.com/:'
        expected  = %Q{<p>Try <a href="http://example.com/" class="external">http://example.com/</a>:</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on colons that occur within the URI' do
        input     = 'Try http://example.com:3000/.'
        expected  = %Q{<p>Try <a href="http://example.com:3000/" class="external">http://example.com:3000/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a comma' do
        input     = 'Try http://example.com/,'
        expected  = %Q{<p>Try <a href="http://example.com/" class="external">http://example.com/</a>,</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on commas that occur within the URI' do
        input     = 'Try http://example.com/2008,10,12,index.html.'
        expected  = %Q{<p>Try <a href="http://example.com/2008,10,12,index.html" class="external">http://example.com/2008,10,12,index.html</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by an exclamation mark' do
        input     = 'Try http://example.com/!'
        expected  = %Q{<p>Try <a href="http://example.com/" class="external">http://example.com/</a>!</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on exclamation marks that occur within the URI' do
        input     = 'Try http://example.com/fun!.html.'
        expected  = %Q{<p>Try <a href="http://example.com/fun!.html" class="external">http://example.com/fun!.html</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a question mark' do
        input     = 'Try http://example.com/?'
        expected  = %Q{<p>Try <a href="http://example.com/" class="external">http://example.com/</a>?</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on question marks that occur within the URI' do
        input     = 'Try http://example.com/fun.html?q=foo.'
        expected  = %Q{<p>Try <a href="http://example.com/fun.html?q=foo" class="external">http://example.com/fun.html?q=foo</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a semi-colon' do
        input     = 'Try http://example.com/;'
        expected  = %Q{<p>Try <a href="http://example.com/" class="external">http://example.com/</a>;</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on semi-colons that occur within the URI' do
        input     = 'Try http://example.com/fun;edit.'
        expected  = %Q{<p>Try <a href="http://example.com/fun;edit" class="external">http://example.com/fun;edit</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a right parenthesis' do
        input     = '(Try http://example.com/)'
        expected  = %Q{<p>(Try <a href="http://example.com/" class="external">http://example.com/</a>)</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on right-parentheses that occur within the URI' do
        input     = 'Try http://example.com/(fun).html.'
        expected  = %Q{<p>Try <a href="http://example.com/(fun).html" class="external">http://example.com/(fun).html</a>.</p>\n}
        @parser.parse(input).should == expected
      end
    end

    describe 'after HTTPS URIs' do
      it 'should terminate if followed by a period' do
        input     = 'Try https://example.com/.'
        expected  = %Q{<p>Try <a href="https://example.com/" class="external">https://example.com/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on periods that occur within the URI' do
        input     = 'Try https://www.example.com/.'
        expected  = %Q{<p>Try <a href="https://www.example.com/" class="external">https://www.example.com/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a colon' do
        input     = 'Try https://example.com/:'
        expected  = %Q{<p>Try <a href="https://example.com/" class="external">https://example.com/</a>:</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on colons that occur within the URI' do
        input     = 'Try https://example.com:3000/.'
        expected  = %Q{<p>Try <a href="https://example.com:3000/" class="external">https://example.com:3000/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a comma' do
        input     = 'Try https://example.com/,'
        expected  = %Q{<p>Try <a href="https://example.com/" class="external">https://example.com/</a>,</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on commas that occur within the URI' do
        input     = 'Try https://example.com/2008,10,12,index.html.'
        expected  = %Q{<p>Try <a href="https://example.com/2008,10,12,index.html" class="external">https://example.com/2008,10,12,index.html</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by an exclamation mark' do
        input     = 'Try https://example.com/!'
        expected  = %Q{<p>Try <a href="https://example.com/" class="external">https://example.com/</a>!</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on exclamation marks that occur within the URI' do
        input     = 'Try https://example.com/fun!.html.'
        expected  = %Q{<p>Try <a href="https://example.com/fun!.html" class="external">https://example.com/fun!.html</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a question mark' do
        input     = 'Try https://example.com/?'
        expected  = %Q{<p>Try <a href="https://example.com/" class="external">https://example.com/</a>?</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on question marks that occur within the URI' do
        input     = 'Try https://example.com/fun.html?q=foo.'
        expected  = %Q{<p>Try <a href="https://example.com/fun.html?q=foo" class="external">https://example.com/fun.html?q=foo</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a semi-colon' do
        input     = 'Try https://example.com/;'
        expected  = %Q{<p>Try <a href="https://example.com/" class="external">https://example.com/</a>;</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on semi-colons that occur within the URI' do
        input     = 'Try https://example.com/fun;edit.'
        expected  = %Q{<p>Try <a href="https://example.com/fun;edit" class="external">https://example.com/fun;edit</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a right parenthesis' do
        input     = '(Try https://example.com/)'
        expected  = %Q{<p>(Try <a href="https://example.com/" class="external">https://example.com/</a>)</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on right-parentheses that occur within the URI' do
        input     = 'Try https://example.com/(fun).html.'
        expected  = %Q{<p>Try <a href="https://example.com/(fun).html" class="external">https://example.com/(fun).html</a>.</p>\n}
        @parser.parse(input).should == expected
      end
    end

    # note: these are just like the HTTP specs seeing as both URI types are treated the same syntactically
    # (even though these aren't "real" FTP URIs)
    describe 'after FTP URIs' do
      it 'should terminate if followed by a period' do
        input     = 'Try ftp://example.com/.'
        expected  = %Q{<p>Try <a href="ftp://example.com/" class="external">ftp://example.com/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on periods that occur within the URI' do
        input     = 'Try ftp://www.example.com/.'
        expected  = %Q{<p>Try <a href="ftp://www.example.com/" class="external">ftp://www.example.com/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a colon' do
        input     = 'Try ftp://example.com/:'
        expected  = %Q{<p>Try <a href="ftp://example.com/" class="external">ftp://example.com/</a>:</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on colons that occur within the URI' do
        input     = 'Try ftp://example.com:3000/.'
        expected  = %Q{<p>Try <a href="ftp://example.com:3000/" class="external">ftp://example.com:3000/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a comma' do
        input     = 'Try ftp://example.com/,'
        expected  = %Q{<p>Try <a href="ftp://example.com/" class="external">ftp://example.com/</a>,</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on commas that occur within the URI' do
        input     = 'Try ftp://example.com/2008,10,12,index.html.'
        expected  = %Q{<p>Try <a href="ftp://example.com/2008,10,12,index.html" class="external">ftp://example.com/2008,10,12,index.html</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by an exclamation mark' do
        input     = 'Try ftp://example.com/!'
        expected  = %Q{<p>Try <a href="ftp://example.com/" class="external">ftp://example.com/</a>!</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on exclamation marks that occur within the URI' do
        input     = 'Try ftp://example.com/fun!.html.'
        expected  = %Q{<p>Try <a href="ftp://example.com/fun!.html" class="external">ftp://example.com/fun!.html</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a question mark' do
        input     = 'Try ftp://example.com/?'
        expected  = %Q{<p>Try <a href="ftp://example.com/" class="external">ftp://example.com/</a>?</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on question marks that occur within the URI' do
        input     = 'Try ftp://example.com/fun.html?q=foo.'
        expected  = %Q{<p>Try <a href="ftp://example.com/fun.html?q=foo" class="external">ftp://example.com/fun.html?q=foo</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a semi-colon' do
        input     = 'Try ftp://example.com/;'
        expected  = %Q{<p>Try <a href="ftp://example.com/" class="external">ftp://example.com/</a>;</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on semi-colons that occur within the URI' do
        input     = 'Try ftp://example.com/fun;edit.'
        expected  = %Q{<p>Try <a href="ftp://example.com/fun;edit" class="external">ftp://example.com/fun;edit</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a right parenthesis' do
        input     = '(Try ftp://example.com/)'
        expected  = %Q{<p>(Try <a href="ftp://example.com/" class="external">ftp://example.com/</a>)</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on right-parentheses that occur within the URI' do
        input     = 'Try ftp://example.com/(fun).html.'
        expected  = %Q{<p>Try <a href="ftp://example.com/(fun).html" class="external">ftp://example.com/(fun).html</a>.</p>\n}
        @parser.parse(input).should == expected
      end
    end

    # note: these are just like the HTTP specs seeing as both URI types are treated the same syntactically
    # (even though these aren't "real" SVN URIs)
    describe 'after SVN URIs' do
      it 'should terminate if followed by a period' do
        input     = 'Try svn://example.com/.'
        expected  = %Q{<p>Try <a href="svn://example.com/" class="external">svn://example.com/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on periods that occur within the URI' do
        input     = 'Try svn://www.example.com/.'
        expected  = %Q{<p>Try <a href="svn://www.example.com/" class="external">svn://www.example.com/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a colon' do
        input     = 'Try svn://example.com/:'
        expected  = %Q{<p>Try <a href="svn://example.com/" class="external">svn://example.com/</a>:</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on colons that occur within the URI' do
        input     = 'Try svn://example.com:3000/.'
        expected  = %Q{<p>Try <a href="svn://example.com:3000/" class="external">svn://example.com:3000/</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a comma' do
        input     = 'Try svn://example.com/,'
        expected  = %Q{<p>Try <a href="svn://example.com/" class="external">svn://example.com/</a>,</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on commas that occur within the URI' do
        input     = 'Try svn://example.com/2008,10,12,index.html.'
        expected  = %Q{<p>Try <a href="svn://example.com/2008,10,12,index.html" class="external">svn://example.com/2008,10,12,index.html</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by an exclamation mark' do
        input     = 'Try svn://example.com/!'
        expected  = %Q{<p>Try <a href="svn://example.com/" class="external">svn://example.com/</a>!</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on exclamation marks that occur within the URI' do
        input     = 'Try svn://example.com/fun!.html.'
        expected  = %Q{<p>Try <a href="svn://example.com/fun!.html" class="external">svn://example.com/fun!.html</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a question mark' do
        input     = 'Try svn://example.com/?'
        expected  = %Q{<p>Try <a href="svn://example.com/" class="external">svn://example.com/</a>?</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on question marks that occur within the URI' do
        input     = 'Try svn://example.com/fun.html?q=foo'
        expected  = %Q{<p>Try <a href="svn://example.com/fun.html?q=foo" class="external">svn://example.com/fun.html?q=foo</a></p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a semi-colon' do
        input     = 'Try svn://example.com/;'
        expected  = %Q{<p>Try <a href="svn://example.com/" class="external">svn://example.com/</a>;</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on semi-colons that occur within the URI' do
        input     = 'Try svn://example.com/fun;edit.'
        expected  = %Q{<p>Try <a href="svn://example.com/fun;edit" class="external">svn://example.com/fun;edit</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a right parenthesis' do
        input     = '(Try svn://example.com/)'
        expected  = %Q{<p>(Try <a href="svn://example.com/" class="external">svn://example.com/</a>)</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on right-parentheses that occur within the URI' do
        input     = 'Try svn://example.com/(fun).html.'
        expected  = %Q{<p>Try <a href="svn://example.com/(fun).html" class="external">svn://example.com/(fun).html</a>.</p>\n}
        @parser.parse(input).should == expected
      end
    end

    # fewer specs here because the range of "mailto:" URIs recognized by the scanner is more limited
    # (compared with, say, HTTP or HTTPS)
    describe 'after "mailto:" URIs' do
      it 'should terminate if followed by a period' do
        input     = 'Try mailto:user@example.com.'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">mailto:user@example.com</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on periods that occur within the URI' do
        input     = 'Try mailto:user.name@example.com.'
        expected  = %Q{<p>Try <a href="mailto:user.name@example.com" class="mailto">mailto:user.name@example.com</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a colon' do
        input     = 'Try mailto:user@example.com:'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">mailto:user@example.com</a>:</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a comma' do
        input     = 'Try mailto:user@example.com,'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">mailto:user@example.com</a>,</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by an exclamation mark' do
        input     = 'Try mailto:user@example.com!'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">mailto:user@example.com</a>!</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a question mark' do
        input     = 'Try mailto:user@example.com?'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">mailto:user@example.com</a>?</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on question marks that occur within the URI' do
        pending
        # this kind of autolink never worked (will require changes to the Ragel scanner)
        input     = 'Try mailto:user@example.com?subject=foo?'
        expected  = %Q{<p>Try <a href="mailto:user@example.com?subject=foo" class="external">mailto:user@example.com?subject=foo</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a semi-colon' do
        input     = 'Try mailto:user@example.com;'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">mailto:user@example.com</a>;</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a right parenthesis' do
        input     = '(Try mailto:user@example.com)'
        expected  = %Q{<p>(Try <a href="mailto:user@example.com" class="mailto">mailto:user@example.com</a>)</p>\n}
        @parser.parse(input).should == expected
      end
    end

    describe 'after email addresses' do
      it 'should terminate if followed by a period' do
        input     = 'Try user@example.com.'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">user@example.com</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on periods that occur within the email address' do
        input     = 'Try user.name@example.com.'
        expected  = %Q{<p>Try <a href="mailto:user.name@example.com" class="mailto">user.name@example.com</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a colon' do
        input     = 'Try user@example.com:'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">user@example.com</a>:</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a comma' do
        input     = 'Try user@example.com,'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">user@example.com</a>,</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by an exclamation mark' do
        input     = 'Try user@example.com!'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">user@example.com</a>!</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a question mark' do
        input     = 'Try user@example.com?'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">user@example.com</a>?</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a semi-colon' do
        input     = 'Try user@example.com;'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="mailto">user@example.com</a>;</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a right parenthesis' do
        input     = '(Try user@example.com)'
        expected  = %Q{<p>(Try <a href="mailto:user@example.com" class="mailto">user@example.com</a>)</p>\n}
        @parser.parse(input).should == expected
      end
    end

    describe 'before HTTP URIs' do
      it 'should handle left parenthesis before' do
        input = "The site (http://example.com/)"
        expected = %Q{<p>The site (<a href="http://example.com/" class="external">http://example.com/</a>)</p>\n}
        @parser.parse(input).should == expected
      end
    end

    describe 'before email addresses' do
      it 'should handle left parenthesis before' do
        input = "Email me (user@example.com)"
        expected = %Q{<p>Email me (<a href="mailto:user@example.com" class="mailto">user@example.com</a>)</p>\n}
        @parser.parse(input).should == expected
      end
    end
  end
end
