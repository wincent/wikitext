# Copyright 2007-present Greg Hurrell. All rights reserved.
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

