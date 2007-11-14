# Copyright 2007 Wincent Colaiuta
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

require 'rake'
require 'rake/clean'
require 'rake/gempackagetask'
require 'rubygems'
require 'spec/rake/spectask'

CLEAN.include   Rake::FileList['**/*.so', '**/*.bundle', '**/*.o', '**/mkmf.log', '**/Makefile']
CLOBBER.include Rake::FileList['WikitextLexer.c', 'WikitextLexer.h', 'Wikitext.tokens']

task :default => :all

desc 'Build all and run all specs'
task :all => [:make, :spec]

extension_makefile  = 'ext/Makefile'
built_extension     = "ext/wikitext.#{Config::CONFIG['DLEXT']}" # wikitext.bundle (Darwin), wikitext.so (Linux)
extension_files     = FileList[
  'ext/Makefile',
  'ext/wikitext.c',
  'ext/wikitext.h',
  'ext/WikitextLexer.c',
  'ext/WikitextLexer.h',
  'ext/Wikitext.g'
]

desc 'Build C extension'
task :make => [extension_makefile, built_extension]

file extension_makefile => ['ext/extconf.rb', 'ext/depend'] do
  Dir.chdir('ext') do
    if RUBY_PLATFORM =~ /darwin/
      sh "env ARCHFLAGS='-arch i386' ruby extconf.rb"
    else
      ruby 'extconf.rb'
    end
  end
end

file built_extension => extension_files do
  Dir.chdir('ext') do
    sh 'make'
  end
end

desc 'Run specs'
Spec::Rake::SpecTask.new('spec') do |t|
  t.spec_files  = FileList['spec/**/*_spec.rb']
end

SPEC = Gem::Specification.new do |s|
  s.name          = 'wikitext'
  s.version       = '0.1'
  s.author        = 'Wincent Colaiuta'
  s.email         = 'win@wincent.com'
  s.homepage      = 'http://wincent.com/'
  s.platform      = Gem::Platform::RUBY
  s.summary       = 'Wikitext-to-HTML translator'
  s.description   = <<-ENDDESC
    Wikitext is a fast wikitext-to-HTML translator written in C.
  ENDDESC
  s.require_paths = ['ext']
  s.autorequire   = 'wikitext'
  s.has_rdoc      = true
  s.files         = FileList['spec/*', 'ext/*.{rb,c,h}'].to_a # TODO: add 'docs' subdirectory, 'README.txt' when they're done
  s.extensions    = ['ext/extconf.rb']
end

Rake::GemPackageTask.new(SPEC) do |t|
  t.need_tar      = true
end
