# Copyright 2010-present Greg Hurrell. All rights reserved.
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

$LOAD_PATH.unshift File.expand_path(File.join(File.dirname(__FILE__), 'lib'))
require 'wikitext/version'

Gem::Specification.new do |s|
  s.name                  = 'wikitext'
  s.version               =  Wikitext::VERSION
  s.author                = 'Greg Hurrell'
  s.email                 = 'greg@hurrell.net'
  s.homepage              = 'https://wincent.com/products/wikitext'
  s.rubyforge_project     = 'wikitext'
  s.platform              = Gem::Platform::RUBY
  s.license               = 'BSD'
  s.required_ruby_version = '>= 2.0.0'
  s.summary               = 'Wikitext-to-HTML translator'
  s.require_paths         = ['ext', 'lib']
  s.extensions            = ['ext/wikitext/extconf.rb']
  s.executables           = ['wikitext']
  s.files                 = Dir[
    'bin/wikitext',
    'ext/wikitext/*.{rb,c,h}',
    'ext/wikitext/depend',
    'lib/wikitext/*.rb',
    'rails/init.rb',
    'spec/*.rb',
  ]
  s.description           = <<-DESC
    Wikitext is a fast wikitext-to-HTML translator written in C.
  DESC
  s.add_development_dependency 'rake'
  s.add_development_dependency 'rspec'
  s.add_development_dependency 'thor'
  s.add_development_dependency 'yard'
  s.add_development_dependency 'wopen3'
  s.add_development_dependency 'ZenTest'
end
