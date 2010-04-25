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

require File.join(File.dirname(__FILE__), 'spec_helper.rb')

describe Wikitext::Parser, 'external links' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should format valid external HTTP links' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google</a></p>\n}
    @parser.parse('[http://google.com/ Google]').should == expected
  end

  it 'should format valid external HTTPS links' do
    expected = %Q{<p><a href="https://google.com/" class="external">Google</a></p>\n}
    @parser.parse('[https://google.com/ Google]').should == expected
  end

  it 'should format valid external FTP links' do
    expected = %Q{<p><a href="ftp://google.com/" class="external">Google</a></p>\n}
    @parser.parse('[ftp://google.com/ Google]').should == expected
  end

  it 'should format valid external SVN links' do
    expected = %Q{<p><a href="svn://google.com/" class="external">Google</a></p>\n}
    @parser.parse('[svn://google.com/ Google]').should == expected
  end

  it 'should format valid external mailto links' do
    expected = %Q{<p><a href="mailto:user@example.com" class="mailto">john</a></p>\n}
    @parser.parse('[mailto:user@example.com john]').should == expected
  end

  it 'should not treat raw email addresses as valid link targets' do
    expected = %Q{<p>[<a href="mailto:user@example.com" class="mailto">user@example.com</a> john]</p>\n}
    @parser.parse('[user@example.com john]').should == expected
  end

  it 'should format absolute path links' do
    expected = %Q{<p><a href="/foo/bar">fb</a></p>\n} # note no "external" class
    @parser.parse('[/foo/bar fb]').should == expected
  end

  it 'should format deeply nested absolute path links' do
    expected = %Q{<p><a href="/foo/bar/baz/bing">fb</a></p>\n} # note no "external" class
    @parser.parse('[/foo/bar/baz/bing fb]').should == expected
  end

  it 'should format minimal absolute path links' do
    expected = %Q{<p><a href="/">fb</a></p>\n} # note no "external" class
    @parser.parse('[/ fb]').should == expected
  end

  it 'should format absolute path links with trailing slashes' do
    expected = %Q{<p><a href="/foo/bar/">fb</a></p>\n} # note no "external" class
    @parser.parse('[/foo/bar/ fb]').should == expected
  end

  it 'should not format relative path links' do
    # relative paths don't make sense in wikitext because
    # they could be displayed anywhere (eg. /wiki/article, /dashboard/ etc)
    expected = %Q{<p>[foo/bar fb]</p>\n}
    @parser.parse('[foo/bar fb]').should == expected
  end

  it 'should treat runs of spaces after the link target as a single space' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google</a></p>\n}
    @parser.parse('[http://google.com/                  Google]').should == expected
  end

  it 'should not treat runs of spaces within the link text as a single space' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google    search</a></p>\n}
    @parser.parse('[http://google.com/ Google    search]').should == expected
  end

  it 'should format a link with emphasis in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <em>rocks</em></a></p>\n}
    @parser.parse("[http://google.com/ Google ''rocks'']").should == expected
  end

  it "should automatically close unmatched '' tags in the link text" do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <em>SOC</em></a></p>\n}
    @parser.parse("[http://google.com/ Google ''SOC]").should == expected
  end

  it 'should format a link with strong formatting in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external"><strong>Google</strong> rocks</a></p>\n}
    @parser.parse("[http://google.com/ '''Google''' rocks]").should == expected
  end

  it "should automatically close unmatched ''' tags in the link text" do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <strong>SOC</strong></a></p>\n}
    @parser.parse("[http://google.com/ Google '''SOC]").should == expected
  end

  it 'should format a link with <tt></tt> tags in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <tt>SOC</tt></a></p>\n}
    @parser.parse("[http://google.com/ Google <tt>SOC</tt>]").should == expected
  end

  it 'should automatically close unmatched <tt> tags in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <tt>SOC</tt></a></p>\n}
    @parser.parse("[http://google.com/ Google <tt>SOC]").should == expected
  end

  it 'should format a link with strong and emphasis in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <strong><em>rocks</em></strong></a></p>\n}
    @parser.parse("[http://google.com/ Google '''''rocks''''']").should == expected
  end

  it "should automatically close unmatched ''''' tags in the link text" do
    expected = %Q{<p><a href="http://google.com/" class="external">Google <strong><em>SOC</em></strong></a></p>\n}
    @parser.parse("[http://google.com/ Google '''''SOC]").should == expected
  end

  it 'should respect "<nowiki></nowiki>" tags inside the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google ] rocks</a></p>\n}
    @parser.parse("[http://google.com/ Google <nowiki>]</nowiki> rocks]").should == expected  # was a bug

    expected = %Q{<p><a href="http://google.com/" class="external">Google [ rocks</a></p>\n}
    @parser.parse("[http://google.com/ Google <nowiki>[</nowiki> rocks]").should == expected  # was a bug
  end

  it 'should pass "[" in link text through literally' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google [ rocks</a></p>\n}
    @parser.parse("[http://google.com/ Google [ rocks]").should == expected  # was a bug
  end

  it 'should pass "[[" in link text through literally' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google [[ rocks</a></p>\n}
    @parser.parse("[http://google.com/ Google [[ rocks]").should == expected  # was a bug
  end

  it 'should pass "]]" in link text through literally' do
    # note how "]]" is treated as a single token, not as a "]" which closes the link followed by another ("] rocks]")
    expected = %Q{<p><a href="http://google.com/" class="external">Google ]] rocks</a></p>\n}
    @parser.parse("[http://google.com/ Google ]] rocks]").should == expected  # was a bug
  end

  it 'should pass through ASCII entities in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google &quot;SOC&quot;</a></p>\n}  # QUOT
    @parser.parse(%Q{[http://google.com/ Google "SOC"]}).should == expected
    expected = %Q{<p><a href="http://google.com/" class="external">Google &lt;SOC&gt;</a></p>\n}      # LESS, GREATER
    @parser.parse(%Q{[http://google.com/ Google <SOC>]}).should == expected
    expected = %Q{<p><a href="http://google.com/" class="external">Google &amp; SOC</a></p>\n}        # AMP
    @parser.parse(%Q{[http://google.com/ Google & SOC]}).should == expected
  end

  it 'should pass through named entities in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google &euro;</a></p>\n}
    @parser.parse(%Q{[http://google.com/ Google &euro;]}).should == expected
  end

  it 'should pass through decimal entities in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google &#8364;</a></p>\n}
    @parser.parse(%Q{[http://google.com/ Google &#8364;]}).should == expected
  end

  it 'should pass through hexadecimal entities in the link text' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google &#x20ac;</a></p>\n}
    @parser.parse(%Q{[http://google.com/ Google &#x20ac;]}).should == expected
  end

  it 'should convert non-ASCII characters in the link text into entities' do
    expected = %Q{<p><a href="http://google.com/" class="external">Google &#x20ac;</a></p>\n}
    @parser.parse(%Q{[http://google.com/ Google €]}).should == expected
  end

  it 'should pass through unexpected external link end tokens literally' do
    @parser.parse('foo ] bar').should == "<p>foo ] bar</p>\n"                                     # in plain scope
    @parser.parse("foo '']'' bar").should == "<p>foo <em>]</em> bar</p>\n"                        # in EM scope
    @parser.parse("foo ''']''' bar").should == "<p>foo <strong>]</strong> bar</p>\n"              # in STRONG scope
    @parser.parse("foo ''''']''''' bar").should == "<p>foo <strong><em>]</em></strong> bar</p>\n" # in STRONG_EM scope
    @parser.parse('foo <tt>]</tt> bar').should == "<p>foo <tt>]</tt> bar</p>\n"                   # in TT scope
    @parser.parse('= foo ] bar =').should == "<h1>foo ] bar</h1>\n"                               # in H1 scope
    @parser.parse('== foo ] bar ==').should == "<h2>foo ] bar</h2>\n"                             # in H2 scope
    @parser.parse('=== foo ] bar ===').should == "<h3>foo ] bar</h3>\n"                           # in H3 scope
    @parser.parse('==== foo ] bar ====').should == "<h4>foo ] bar</h4>\n"                         # in H4 scope
    @parser.parse('===== foo ] bar =====').should == "<h5>foo ] bar</h5>\n"                       # in H5 scope
    @parser.parse('====== foo ] bar ======').should == "<h6>foo ] bar</h6>\n"                     # in H6 scope
    @parser.parse('> ]').should == "<blockquote>\n  <p>]</p>\n</blockquote>\n"                    # in BLOCKQUOTE scope
  end

  describe 'questionable links' do
    it 'should handle links which contain an embedded [ character' do
      # note that [ is allowed in the link text, although the result may be unexpected to the user
      expected = %Q{<p><a href="http://google.com/" class="external">[hello</a></p>\n}
      @parser.parse("[http://google.com/ [hello]").should == expected
    end

    it 'should handle links which contain an embedded ] character' do
      # note how the first ] terminates the link
      expected = %Q{<p><a href="http://google.com/" class="external">[hello</a> world]</p>\n}
      @parser.parse("[http://google.com/ [hello] world]").should == expected
    end

    it 'should handle links which contain an embedded [[ character' do
      # note that [[ is allowed in the link text
      expected = %Q{<p><a href="http://google.com/" class="external">[[hello</a></p>\n}
      @parser.parse("[http://google.com/ [[hello]").should == expected
    end

    it 'should handle links which contain an embedded ]] character' do
      # note how this time ]] does not terminate the link because it is tokenized as LINK_END rather than EXT_LINK_END
      expected = %Q{<p><a href="http://google.com/" class="external">[[hello]] world</a></p>\n}
      @parser.parse("[http://google.com/ [[hello]] world]").should == expected
    end

    it 'should allow URIs in the link text' do
      # not sure why you'd want to do this, but...
      expected = %Q{<p><a href="http://example.net/" class="external">hello http://example.com/ world</a></p>\n}
      @parser.parse('[http://example.net/ hello http://example.com/ world]').should == expected
    end
  end

  describe 'invalid links' do
    it "should pass through links which don't have a valid target" do
      expected = "<p>[well]</p>\n"
      @parser.parse("[well]").should == expected
    end

    it "should pass through links which don't have any target" do
      expected = "<p>[]</p>\n"
      @parser.parse('[]').should == expected
    end

    it 'should pass through unterminated links (EOF)' do
      expected = "<p>[</p>\n"
      @parser.parse('[').should == expected

      expected = "<p>[well</p>\n"
      @parser.parse("[well").should == expected

      expected = %Q{<p>[<a href="http://example.com/" class="external">http://example.com/</a></p>\n}
      @parser.parse("[http://example.com/").should == expected

      expected = %Q{<p>[<a href="http://example.com/" class="external">http://example.com/</a> </p>\n}
      @parser.parse("[http://example.com/ ").should == expected

      expected = %Q{<p>[<a href="http://example.com/" class="external">http://example.com/</a> visit</p>\n}
      @parser.parse("[http://example.com/ visit").should == expected # was a bug

      expected = %Q{<h6>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h6>\n}
      @parser.parse("====== [http://example.com/ visit").should == expected # was a bug

      expected = %Q{<h5>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h5>\n}
      @parser.parse("===== [http://example.com/ visit").should == expected # was a bug

      expected = %Q{<h4>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h4>\n}
      @parser.parse("==== [http://example.com/ visit").should == expected # was a bug

      expected = %Q{<h3>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h3>\n}
      @parser.parse("=== [http://example.com/ visit").should == expected # was a bug

      expected = %Q{<h2>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h2>\n}
      @parser.parse("== [http://example.com/ visit").should == expected # was a bug

      expected = %Q{<h1>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h1>\n}
      @parser.parse("= [http://example.com/ visit").should == expected # was a bug

      expected = %Q{<p>[<a href="http://example.com/" class="external">http://example.com/</a> ...</p>\n}
      @parser.parse("[http://example.com/ <nowiki>...\n").should == expected
    end

    it 'should pass through unterminated links (end-of-line)' do
      expected = "<p>[</p>\n"
      @parser.parse("[\n").should == expected

      expected = "<p>[well</p>\n"
      @parser.parse("[well\n").should == expected

      expected = %Q{<p>[<a href="http://example.com/" class="external">http://example.com/</a></p>\n}
      @parser.parse("[http://example.com/\n").should == expected

      expected = %Q{<p>[<a href="http://example.com/" class="external">http://example.com/</a> </p>\n}
      @parser.parse("[http://example.com/ \n").should == expected

      expected = %Q{<p>[<a href="http://example.com/" class="external">http://example.com/</a> visit</p>\n}
      @parser.parse("[http://example.com/ visit\n").should == expected # was a bug

      expected = %Q{<h6>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h6>\n}
      @parser.parse("====== [http://example.com/ visit\n").should == expected # was a bug

      expected = %Q{<h5>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h5>\n}
      @parser.parse("===== [http://example.com/ visit\n").should == expected # was a bug

      expected = %Q{<h4>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h4>\n}
      @parser.parse("==== [http://example.com/ visit\n").should == expected # was a bug

      expected = %Q{<h3>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h3>\n}
      @parser.parse("=== [http://example.com/ visit\n").should == expected # was a bug

      expected = %Q{<h2>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h2>\n}
      @parser.parse("== [http://example.com/ visit\n").should == expected # was a bug

      expected = %Q{<h1>[<a href="http://example.com/" class="external">http://example.com/</a> visit</h1>\n}
      @parser.parse("= [http://example.com/ visit\n").should == expected # was a bug

      # here's a slightly more complicated example using a blockquote
      expected = %Q{<blockquote>\n  <p>[<a href="http://google.com/" class="external">http://google.com/</a></p>\n</blockquote>\n}
      @parser.parse("> [http://google.com/\n").should == expected # was a bug
    end
  end

  describe 'regressions' do
    # assorted examples
    it 'should not turn failed absolute links into external hyperlinks' do
      # was emitting: <p>[<a href="/hello" class="external">/hello</a> this</p>\n
      expected =   %Q{<p>[<a href="/hello">/hello</a> this</p>\n}
      @parser.parse('[/hello this').should == expected

      # was emitting: <p>[<a href="/hello" class="external">/hello</a> </p>\n
      expected =   %Q{<p>[<a href="/hello">/hello</a> </p>\n}
      @parser.parse('[/hello ').should == expected

      # was emitting: <h1>hello [<a href="/hello" class="external">/hello</a> </h1>\n
      expected =   %Q{<h1>hello [<a href="/hello">/hello</a> </h1>\n}
      @parser.parse('= hello [/hello =').should == expected
    end
  end
end
