# Copyright 2007-2010 Wincent Colaiuta
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
require 'rake/rdoctask'
require 'rubygems'
require 'spec/rake/spectask'
require File.join(File.dirname(__FILE__), 'lib', 'wikitext', 'version.rb')

CLEAN.include   Rake::FileList['**/*.so', '**/*.bundle', '**/*.o', '**/mkmf.log', '**/Makefile']
CLOBBER.include Rake::FileList['ext/wikitext_ragel.c']

task :default => :all

desc 'Build all and run all specs'
task :all => [:make, :spec]

extension_makefile  = 'ext/Makefile'
ragel               = 'ext/wikitext_ragel.c'
built_extension     = "ext/wikitext.#{Config::CONFIG['DLEXT']}" # wikitext.bundle (Darwin), wikitext.so (Linux)
extension_files     = FileList[
  'ext/Makefile',
  'ext/ary.c',
  'ext/ary.h',
  'ext/parser.c',
  'ext/parser.h',
  'ext/ruby_compat.h',
  'ext/str.c',
  'ext/str.h',
  'ext/token.c',
  'ext/token.h',
  'ext/wikitext.c',
  'ext/wikitext.h',
  'ext/wikitext_ragel.c',
  'ext/wikitext_ragel.h'
]

desc 'Build C extension'
task :make => [ragel, extension_makefile, built_extension]

file ragel => ['ext/wikitext_ragel.rl'] do
  Dir.chdir('ext') do
    # pass the -s switch here because otherwise Ragel is totally silent
    # I like to have visual confirmation that it's actually run
    sh 'ragel -G2 -s wikitext_ragel.rl'
  end
end

file extension_makefile => ['ext/extconf.rb', 'ext/depend', ragel] do
  Dir.chdir('ext') do
    ruby 'extconf.rb'
  end
end

file built_extension => extension_files do
  Dir.chdir('ext') do
    sh 'make && touch .built'
  end
end

desc 'Run specs'
Spec::Rake::SpecTask.new('spec') do |t|
  t.spec_files  = FileList['spec/**/*_spec.rb']
  t.spec_opts   = ['--color']
end

Rake::RDocTask.new do |t|
  t.rdoc_files.include 'doc/README', 'doc/RELEASE-NOTES', 'doc/rdoc.rb'
  t.options           << '--charset' << 'UTF-8' << '--inline-source'
  t.main              = 'doc/README'
  t.title             = 'Wikitext documentation'
end

desc 'Upload RDoc to RubyForge website'
task :upload_rdoc => :rdoc do
  sh 'scp -r html/* rubyforge.org:/var/www/gforge-projects/wikitext/'
end

SPEC = Gem::Specification.new do |s|
  s.name              = 'wikitext'
  s.version           =  Wikitext::VERSION
  s.author            = 'Wincent Colaiuta'
  s.email             = 'win@wincent.com'
  s.homepage          = 'http://wikitext.rubyforge.org/'
  s.rubyforge_project = 'wikitext'
  s.platform          = Gem::Platform::RUBY
  s.summary           = 'Wikitext-to-HTML translator'
  s.description       = <<-ENDDESC
    Wikitext is a fast wikitext-to-HTML translator written in C.
  ENDDESC
  s.require_paths     = ['ext', 'lib']
  s.has_rdoc          = true
  s.files             = FileList[ 'bin/*',
                                  'ext/wikitext_ragel.c',
                                  'ext/*.{rb,c,h}',
                                  'ext/depend',
                                  'lib/wikitext/*',
                                  'rails/init.rb',
                                  'spec/*' ].to_a
  s.extensions        = ['ext/extconf.rb']
  s.executables       = ['wikitext']
  if s.respond_to? :add_development_dependency
    s.add_development_dependency 'rspec'
    s.add_development_dependency 'wopen3'
  else
    s.add_dependency 'rspec'
    s.add_dependency 'wopen3'
  end
end

task :gem => [:make]

task :package => [:clobber, :all, :gem]
Rake::GemPackageTask.new(SPEC) do |t|
  t.need_tar      = true
end
