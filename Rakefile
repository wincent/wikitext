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
