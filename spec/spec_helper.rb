require 'pathname'
require 'rubygems'
require 'spec'

module Wikitext
  if not const_defined? 'EXTDIR'
    # append the local "ext" directory to search path if not already present
    base        = File.join(File.dirname(__FILE__), '..')
    EXTDIR      = Pathname.new(File.join(base, 'ext')).realpath
    normalized  = $:.collect { |path| Pathname.new(path).realpath rescue path }
    $:.push(EXTDIR) unless normalized.include?(EXTDIR)
  end
end # module Wikitext

require 'wikitext'
