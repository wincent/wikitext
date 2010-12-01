# Copyright 2010 Wincent Colaiuta. All rights reserved.
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

require File.join(File.dirname(__FILE__), 'lib', 'wikitext', 'version.rb')

Gem::Specification.new do |s|
  s.name              = 'wikitext'
  s.version           =  Wikitext::VERSION
  s.author            = 'Wincent Colaiuta'
  s.email             = 'win@wincent.com'
  s.homepage          = 'https://wincent.com/products/wikitext'
  s.rubyforge_project = 'wikitext'
  s.platform          = Gem::Platform::RUBY
  s.summary           = 'Wikitext-to-HTML translator'
  s.description       = <<-ENDDESC
    Wikitext is a fast wikitext-to-HTML translator written in C.
  ENDDESC
  s.require_paths     = ['ext', 'lib']
  s.has_rdoc          = false
  s.files             = Dir['bin/wikitext',
                            'ext/*.{rb,c,h}',
                            'ext/depend',
                            'lib/wikitext/*.rb',
                            'rails/init.rb',
                            'spec/*.rb']
  s.extensions        = ['ext/extconf.rb']
  s.executables       = ['wikitext']

  if !s.respond_to? :add_development_dependency
    def s.add_development_dependency gem, *requirements
      s.add_dependency gem, *requirements
    end
  end
  s.add_development_dependency 'rake'
  s.add_development_dependency 'rspec', '~> 2.0'
  s.add_development_dependency 'thor'
  s.add_development_dependency 'yard', '>= 0.5.8'
  s.add_development_dependency 'wopen3'
end
