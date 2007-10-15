# Copyright 2007 Wincent Colaiuta

require 'mkmf'

def missing item
  puts "couldn't find #{item} (required)"
  exit 1
end

have_header('antlr3.h') or missing 'antlr3.h'
have_library('antlr3c', 'antlr3ParserNew') or missing 'antlr3ParserNew function (antlr3c library)'
create_makefile('wiki_text')

