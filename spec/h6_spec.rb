# Copyright 2007-2014 Wincent Colaiuta. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
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
    @parser.parse('> ====== foo ======').should == "<blockquote>\n  <h6>foo</h6>\n</blockquote>\n"
  end

  it 'should have no special meaning inside <nowiki> spans' do
    @parser.parse("<nowiki>\n====== foo ======</nowiki>").should == "<p>\n====== foo ======</p>\n"
  end
end
