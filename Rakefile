require 'rake'
require 'rake/clean'
require 'rake/gempackagetask'
require 'rubygems'
require 'spec/rake/spectask'

CLEAN.include   Rake::FileList['**/*.so', '**/*.bundle', '**/*.o', 'mkmf.log', 'Makefile']
CLOBBER.include Rake::FileList['WikitextLexer.c', 'WikitextLexer.h', 'Wikitext.tokens']

task :default => :all

desc 'Build all and run all specs'
task :all => :spec

desc 'Build C extension'
task :extension do |t|
  ruby  'extconf.rb'
  sh    'make'
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
