require 'rake'
require 'rake/clean'

CLEAN.include   Rake::FileList['**/*.so', '**/*.bundle', '**/*.o', 'mkmf.log', 'Makefile']
CLOBBER.include Rake::FileList['WikiTextLexer.c', 'WikiTextLexer.h', 'WikiText.tokens']

task :default => :all

desc 'Build all and run all specs'
task :all => :spec

desc 'Run all specs'
task :spec => [:run_extension_specs, :run_plugin_specs]

desc 'Build C extension'
task :extension do |t|
  ruby  'extconf.rb'
  sh    'make'
end

desc 'Run specs for wikitext extension'
task :run_extension_specs => :extension do
  sh 'spec', 'wiki_text_spec.rb'
end

desc 'Build Rails plug-in'
task :plugin => :extension do
  case PLATFORM
  when /darwin/i  then  ext_file = 'wiki_text.bundle'
  when /linux/i   then  ext_file = 'wiki_text.so'
  else            raise "unsupported platform #{PLATFORM}"
  end
  cp ext_file, 'plugin/lib/'
end

desc 'Run specs for Rails plug-in'
task :run_plugin_specs => :plugin do
  # not yet implemented
end

