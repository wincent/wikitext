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
require 'rubygems'
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
task :spec => :make do
  sh 'spec spec'
end

desc 'Build the YARD HTML files'
task :yard do
  sh 'yardoc -o html --title Wikitext doc/*.rb - doc/RELEASE-NOTES'
end

desc 'Upload YARD HTML'
task :upload_yard => :yard do
  require 'yaml'
  config = YAML.load_file('.config.yml')
  raise ':yardoc_host not configured' unless config.has_key?(:yardoc_host)
  raise ':yardoc_path not configured' unless config.has_key?(:yardoc_path)
  sh "scp -r html/* #{config[:yardoc_host]}:#{config[:yardoc_path]}"
end

desc 'Build gem ("gem build")'
task :build => :make do
  system 'gem build wikitext.gemspec'
end

desc 'Push gem to Gemcutter ("gem push")'
task :push => :build do
  system "gem push wikitext-#{Wikitext::VERSION}.gem"
end
