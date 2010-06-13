# Copyright 2009 Wincent Colaiuta. All rights reserved.
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
require 'wikitext/parser'

describe Wikitext::Parser do
  it 'should provide access to the shared parser instance for customization' do
    Wikitext::Parser.shared_parser.should be_instance_of(Wikitext::Parser)
  end

  it 'should persist customizations made to shared parser instance' do
    # fisrt pass
    parser                             = Wikitext::Parser.shared_parser
    original_prefix                    = '/images/'
    parser.img_prefix                  = original_prefix
    parser.img_prefix.should           == original_prefix
    parser.parse('{{foo.png}}').should == %Q{<p><img src="/images/foo.png" alt="foo.png"></p>\n}

    # second pass
    parser                             = Wikitext::Parser.shared_parser
    parser.img_prefix.should           == original_prefix
    new_prefix                         = '/totally-different-prefix/'
    parser.img_prefix                  = new_prefix
    parser.img_prefix.should           == new_prefix
    parser.parse('{{bar.png}}').should == %Q{<p><img src="/totally-different-prefix/bar.png" alt="bar.png"></p>\n}
  end
end
