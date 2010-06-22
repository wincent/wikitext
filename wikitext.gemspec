# Copyright 2010 Wincent Colaiuta
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
  s.add_development_dependency 'rspec'
  s.add_development_dependency 'thor'
  s.add_development_dependency 'yard', '>= 0.5.7'
  s.add_development_dependency 'wopen3'
end
