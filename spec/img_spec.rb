# encoding: utf-8
# Copyright 2008-2010 Wincent Colaiuta. All rights reserved.
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

describe Wikitext::Parser, 'embedding img tags' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should convert valid markup into inline image tags' do
    expected = %Q{<p><img src="/images/foo.png" alt="foo.png"></p>\n}
    @parser.parse('{{foo.png}}').should == expected
  end

  it 'should produce XML output if passed option at parse time' do
    expected = %Q{<p><img src="/images/foo.png" alt="foo.png" /></p>\n}
    @parser.parse('{{foo.png}}', :output_style => :xml).should == expected
  end

  it 'should produce XML output if set via instance variable' do
    expected = %Q{<p><img src="/images/foo.png" alt="foo.png" /></p>\n}
    parser = Wikitext::Parser.new
    parser.output_style = :xml
    parser.parse('{{foo.png}}').should == expected
  end

  it 'should produce XML output if option set during initialization' do
    expected = %Q{<p><img src="/images/foo.png" alt="foo.png" /></p>\n}
    parser = Wikitext::Parser.new :output_style => :xml
    parser.parse('{{foo.png}}').should == expected
  end

  it 'should produce HTML output if passed unrecognized output style' do
    expected = %Q{<p><img src="/images/foo.png" alt="foo.png"></p>\n}

    # the _only_ recognized override is :xml (case-sensitive)
    @parser.parse('{{foo.png}}', :output_style => :html).should == expected
    @parser.parse('{{foo.png}}', :output_style => false).should == expected
    @parser.parse('{{foo.png}}', :output_style => nil).should == expected
    @parser.parse('{{foo.png}}', :output_style => 42).should == expected
    @parser.parse('{{foo.png}}', :output_style => 'XML').should == expected
    @parser.parse('{{foo.png}}', :output_style => :XML).should == expected
  end

  it 'should appear embedded in an inline flow' do
    expected = %Q{<p>before <img src="/images/foo.png" alt="foo.png"> after</p>\n}
    @parser.parse('before {{foo.png}} after').should == expected
  end

  it 'should allow images in subdirectories' do
    expected = %Q{<p><img src="/images/foo/bar.png" alt="foo/bar.png"></p>\n}
    @parser.parse('{{foo/bar.png}}').should == expected
  end

  it 'should pass through empty image tags unchanged' do
    @parser.parse('{{}}').should == %Q{<p>{{}}</p>\n}
  end

  it 'should not append prefix if img src starts with a slash' do
    expected = %Q{<p><img src="/foo.png" alt="/foo.png"></p>\n}
    @parser.parse('{{/foo.png}}').should == expected
  end

  it 'should work in BLOCKQUOTE blocks' do
    expected = dedent <<-END
      <blockquote>
        <p><img src="/images/foo.png" alt="foo.png"></p>
      </blockquote>
    END
    @parser.parse('> {{foo.png}}').should == expected
  end

  it 'should work in unordered lists' do
    input = dedent <<-END
      * {{foo.png}}
      * {{bar.png}}
      * {{baz.png}}
    END
    expected = dedent <<-END
      <ul>
        <li><img src="/images/foo.png" alt="foo.png"></li>
        <li><img src="/images/bar.png" alt="bar.png"></li>
        <li><img src="/images/baz.png" alt="baz.png"></li>
      </ul>
    END
    @parser.parse(input).should == expected
  end

  it 'should work in ordered lists' do
    input = dedent <<-END
      # {{foo.png}}
      # {{bar.png}}
      # {{baz.png}}
    END
    expected = dedent <<-END
      <ol>
        <li><img src="/images/foo.png" alt="foo.png"></li>
        <li><img src="/images/bar.png" alt="bar.png"></li>
        <li><img src="/images/baz.png" alt="baz.png"></li>
      </ol>
    END
    @parser.parse(input).should == expected
  end

  it 'should work in <h1> headings' do
    expected = %Q{<h1><img src="/images/foo.png" alt="foo.png"></h1>\n}
    @parser.parse('= {{foo.png}} =').should == expected
  end

  it 'should work in <h2> headings' do
    expected = %Q{<h2><img src="/images/foo.png" alt="foo.png"></h2>\n}
    @parser.parse('== {{foo.png}} ==').should == expected
  end

  it 'should work in <h3> headings' do
    expected = %Q{<h3><img src="/images/foo.png" alt="foo.png"></h3>\n}
    @parser.parse('=== {{foo.png}} ===').should == expected
  end

  it 'should work in <h4> headings' do
    expected = %Q{<h4><img src="/images/foo.png" alt="foo.png"></h4>\n}
    @parser.parse('==== {{foo.png}} ====').should == expected
  end

  it 'should work in <h5> headings' do
    expected = %Q{<h5><img src="/images/foo.png" alt="foo.png"></h5>\n}
    @parser.parse('===== {{foo.png}} =====').should == expected
  end

  it 'should work in <h6> headings' do
    expected = %Q{<h6><img src="/images/foo.png" alt="foo.png"></h6>\n}
    @parser.parse('====== {{foo.png}} ======').should == expected
  end

  it 'should pass single curly braces through unaltered' do
    @parser.parse('{foo.png}').should == %Q{<p>{foo.png}</p>\n}
  end

  it 'should have no effect inside PRE blocks' do
    @parser.parse(' {{foo.png}}').should == %Q{<pre>{{foo.png}}</pre>\n}
  end

  it 'should have no effect inside PRE_START blocks' do
    expected = %Q{<pre>{{foo.png}}</pre>\n}
    @parser.parse('<pre>{{foo.png}}</pre>').should == expected
  end

  it 'should have no effect inside NO_WIKI spans' do
    expected = %Q{<p>{{foo.png}}</p>\n}
    @parser.parse('<nowiki>{{foo.png}}</nowiki>').should == expected
  end

  it 'should be passed through in internal link targets' do
    expected = %Q{<p><a href="/wiki/%7b%7bfoo.png%7d%7d">{{foo.png}}</a></p>\n}
    @parser.parse('[[{{foo.png}}]]').should == expected
  end

  it 'should be passed through in internal link text' do
    expected = %Q{<p><a href="/wiki/article">{{foo.png}}</a></p>\n}
    @parser.parse('[[article|{{foo.png}}]]').should == expected
  end

  it 'should not be allowed as an external link target' do
    expected = %Q{<p>[<img src="/images/foo.png" alt="foo.png"> the link]</p>\n}
    @parser.parse('[{{foo.png}} the link]').should == expected
  end

  it 'should be passed through in external link text' do
    expected = %Q{<p><a href="http://example.com/" class="external">{{foo.png}}</a></p>\n}
    @parser.parse('[http://example.com/ {{foo.png}}]').should == expected
  end

  it 'should not allow embedded quotes' do
    expected = %Q{<p>{{&quot;fun&quot;.png}}</p>\n}
    @parser.parse('{{"fun".png}}').should == expected
  end

  it 'should not allow embedded spaces' do
    @parser.parse('{{foo bar.png}}').should == %Q{<p>{{foo bar.png}}</p>\n}
  end

  it 'should not allow characters beyond printable ASCII' do
    @parser.parse('{{500€.png}}').should == %Q{<p>{{500&#x20ac;.png}}</p>\n}
  end

  it 'should allow overrides of the image prefix at initialization time' do
    parser = Wikitext::Parser.new(:img_prefix => '/gfx/')
    expected = %Q{<p><img src="/gfx/foo.png" alt="foo.png"></p>\n}
    parser.parse('{{foo.png}}').should == expected
  end

  it 'should suppress the image prefix if passed an empty string at initialization time' do
    parser = Wikitext::Parser.new(:img_prefix => '')
    expected = %Q{<p><img src="foo.png" alt="foo.png"></p>\n}
    parser.parse('{{foo.png}}').should == expected
  end

  it 'should suppress image prefix if passed nil at initialization time' do
    parser = Wikitext::Parser.new(:img_prefix => nil)
    expected = %Q{<p><img src="foo.png" alt="foo.png"></p>\n}
    parser.parse('{{foo.png}}').should == expected
  end

  it 'should allow overrides of the image prefix after initialization' do
    @parser.img_prefix = '/gfx/'
    expected = %Q{<p><img src="/gfx/foo.png" alt="foo.png"></p>\n}
    @parser.parse('{{foo.png}}').should == expected
  end

  it 'should suppress image if prefix set to an empty string after initialization' do
    @parser.img_prefix = ''
    expected = %Q{<p><img src="foo.png" alt="foo.png"></p>\n}
    @parser.parse('{{foo.png}}').should == expected
  end

  it 'should suppress image if prefix set to nil after initialization' do
    @parser.img_prefix = nil
    expected = %Q{<p><img src="foo.png" alt="foo.png"></p>\n}
    @parser.parse('{{foo.png}}').should == expected
  end
end
