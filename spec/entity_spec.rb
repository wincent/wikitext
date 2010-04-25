#!/usr/bin/env ruby
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

describe Wikitext::Parser, 'parsing entities' do
  before do
    @parser = Wikitext::Parser.new
  end

  it 'should pass numeric (decimal) entities through unchanged' do
    @parser.parse('&#8364;').should == "<p>&#8364;</p>\n"
  end

  it 'should normalize case variations in hexadecimal entities' do
    @parser.parse('&#x20ac;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#x20AC;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#X20ac;').should == "<p>&#x20ac;</p>\n"
    @parser.parse('&#X20AC;').should == "<p>&#x20ac;</p>\n"
  end

  it 'should pass named entities through unchanged' do
    @parser.parse('&Aacute;').should == "<p>&Aacute;</p>\n" # these are two different entities
    @parser.parse('&aacute;').should == "<p>&aacute;</p>\n" # ie. they are case sensitive
    @parser.parse('&euro;').should == "<p>&euro;</p>\n"
  end
end
