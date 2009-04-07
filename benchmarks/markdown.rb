#!/usr/bin/env ruby
# Copyright 2009 Wincent Colaiuta
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

require File.join(File.dirname(__FILE__), '..', 'ext', 'wikitext')
require 'benchmark'

markdown = File.read(File.join(File.dirname(__FILE__), 'markdown-readme.32')).to_s

def parse job, description, parser, input
  job.report(description) do
    parser.parse input
  end
end

parser = Wikitext::Parser.new
Benchmark.bmbm do |job|
  parse job, 'markdown README concatenated 32 times', parser, markdown
end
