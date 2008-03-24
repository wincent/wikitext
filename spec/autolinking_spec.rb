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
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="external">mailto:user@example.com</a></p>\n}
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

    it 'should apply the mailto CSS class if set' do
      uri = 'user@example.com'
      @parser.mailto_class = 'foo'
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com" class="foo">user@example.com</a></p>\n}
    end

    it 'should apply no CSS if the mailto class is set to nil' do
      uri = 'user@example.com'
      @parser.mailto_class = nil
      @parser.parse(uri).should == %Q{<p><a href="mailto:user@example.com">user@example.com</a></p>\n}
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
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="external">mailto:user@example.com</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should not terminate on periods that occur within the URI' do
        input     = 'Try mailto:user.name@example.com.'
        expected  = %Q{<p>Try <a href="mailto:user.name@example.com" class="external">mailto:user.name@example.com</a>.</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a colon' do
        input     = 'Try mailto:user@example.com:'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="external">mailto:user@example.com</a>:</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a comma' do
        input     = 'Try mailto:user@example.com,'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="external">mailto:user@example.com</a>,</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by an exclamation mark' do
        input     = 'Try mailto:user@example.com!'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="external">mailto:user@example.com</a>!</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a question mark' do
        input     = 'Try mailto:user@example.com?'
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="external">mailto:user@example.com</a>?</p>\n}
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
        expected  = %Q{<p>Try <a href="mailto:user@example.com" class="external">mailto:user@example.com</a>;</p>\n}
        @parser.parse(input).should == expected
      end

      it 'should terminate if followed by a right parenthesis' do
        input     = '(Try mailto:user@example.com)'
        expected  = %Q{<p>(Try <a href="mailto:user@example.com" class="external">mailto:user@example.com</a>)</p>\n}
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
  end
end
